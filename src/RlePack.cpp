/***************************************************************************
                          RlePack.cpp  -  description
                             -------------------
    begin                : Mon Sep 24 2001
    copyright            : (C) 2001 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_video.h>

#include "common.h"
#include "RlePack.h"


typedef struct RLE_SPRITE           /* a RLE compressed sprite */
{
   int w, h;                        /* width and height in pixels */
   int color_depth;                 /* color depth of the image */
   int size;                        /* size of sprite data in bytes */
   signed char dat[0];
} RLE_SPRITE;


struct RlePack_P
{
	SDL_Color palette[256];
	int count, arraysize;
	RLE_SPRITE** sprites;
};

RlePack::RlePack( const char* filename )
{
	p = new RlePack_P;
	p->count = p->arraysize = 0;
	p->sprites = NULL;
	
	//@ Load file and stuff
	FILE* f;
	
	f = fopen( filename, "rb" );
	if (f==NULL)
	{
		debug( "Can't open file '%s'.\n", filename );
		return;
	}
	
	int datacount;
	
	#define READDW(I) {														\
		unsigned char data[4];														\
		fread( data, 4, 1, f );												\
		(I) = (data[0]<<24) + (data[1]<<16) + (data[2]<<8) + data[3]; }
	#define READW(I) {														\
		unsigned char data[2];														\
		fread( data, 2, 1, f );												\
		(I) = (data[0]<<8) + data[1]; }
	#define READCH(S,C) {													\
		fread( S, C, 1, f ); S[C] = 0; }
	
	fseek( f, 8, SEEK_SET );		// Skip header
	READDW( datacount );
	
	debug( "File '%s' contains %d entries.\n", filename, datacount );
	if (datacount>500) datacount = 500;			// Sanity
	
	p->arraysize = datacount;
	p->sprites = new (RLE_SPRITE*)[ datacount ];
	
	while( (!feof(f)) && (!ferror(f)) && (datacount>0) )
	{
		char s[10];
		READCH( s, 4 );
		if ( !strcmp( s, "prop" ))				// Found a property
		{
			fseek( f, 4, SEEK_CUR );
			unsigned int propsize;
			READDW( propsize );
			fseek( f, propsize, SEEK_CUR );
		}
		else if (!strcmp( s, "RLE " ))			// Found an RLE_SPRITE
		{
			datacount--;
			
			unsigned int length, bpp, width, height, size;
			
			READDW( length );
			READDW( length );
			READW( bpp );
			READW( width );
			READW( height );
			READDW( size );
			
			RLE_SPRITE* sprite = (RLE_SPRITE*) malloc( sizeof(RLE_SPRITE) + size );
			p->sprites[ p->count ] = sprite;
			(p->count)++;
			sprite->w = width;
			sprite->h = height;
			sprite->color_depth = bpp;
			sprite->size = size;
			fread( sprite->dat, 1, size, f );
		}
		else if (!strcmp( s, "PAL "))			// Found a palette
		{
			datacount--;
			
			unsigned int length, pallength;
			READDW( length );
			READDW( length );
			pallength = length>1024 ? 1024 : length;
			pallength /= 4;
			
			for (uint i=0; i< pallength; i++)
			{
				char c[4];
				fread( c, 4, 1, f );
				p->palette[i].r = c[0]*4;
				p->palette[i].g = c[1]*4;
				p->palette[i].b = c[2]*4;
				p->palette[i].unused = 0;
			}
			
			fseek( f, length - pallength*4, SEEK_CUR );
		}
		else									// Found something else
		{
			debug( "Unknown: %s.", s );
			datacount--;
			
			unsigned int length;
			READDW( length );
			READDW( length );
			fseek( f, length, SEEK_CUR );
		}
	}
	
	fclose( f );
}

RlePack::~RlePack()
{
	if (!p)
		return;
	
	if (p->sprites)
	{
		for ( int i=0; i<p->count; ++i )
		{
			delete p->sprites[i];
			p->sprites[i] = NULL;
		}
		delete[] p->sprites;
		p->sprites = NULL;
	}
	
	delete( p );
	p = NULL;
}

void OffsetRLESprite( RLE_SPRITE* spr, int offset )
{
	if (!spr || !offset) return;

	signed char *s = spr->dat;
	signed char c;
	int y;
	
	for (y=0; y<spr->h; y++)
	{
		c = *s++;
		
		while (c)
		{
			// For positive c: solid pixels.
			for ( ; c>0; c-- )
			{
				*s = (*s) + offset;
				s++;
			}
			c = *s++;
		}
	}
}

void RlePack::offsetSprites( int offset )
{
	if ( (offset<=0) || (offset>255) )
		return;

	int i;
	
	// Offset every RLE_SPRITE
	
	for ( i=0; i<p->count; ++i )
	{
		OffsetRLESprite( p->sprites[i], offset );
	}
}

SDL_Color* RlePack::getPalette()
{
	return p->palette;
}


void draw_rle_sprite_v_flip( RLE_SPRITE* src, int dx, int dy )
{
#define RLE_PTR					signed char*
#define RLE_IS_EOL(c)			((c) == 0)
#define PIXEL_PTR				unsigned char*
#define OFFSET_PIXEL_PTR(p,x)	((PIXEL_PTR) (p) + (x))
#define INC_PIXEL_PTR(p)		((p)++)
#define DEC_PIXEL_PTR(p)		((p)--)
#define PUT_PIXEL(p,c)			(*((unsigned char *)(p)) = (c))
//#define PUT_PIXEL(p,c)         bmp_write8((unsigned long) (p), (c))

   int x, y, w, h;		// width and height of visible area
   int dxbeg, dybeg;	// beginning in destination
   int sxbeg, sybeg;	// beginning in source
   RLE_PTR s;

   SDL_Surface* dst = gamescreen;
   // Clip to dst->clip_rect
   int dst_cl = dst->clip_rect.x;
   int dst_cr = dst->clip_rect.w + dst_cl;
   int dst_ct = dst->clip_rect.y;
   int dst_cb = dst->clip_rect.h + dst_ct;

// if (dst->clip)
   if (1)
   {

      int tmp;

      dxbeg = dx;
      if ( dst_cl > dx ) dxbeg = dst_cl;

      tmp = dx + src->w - dst_cr;
      sxbeg = ((tmp < 0) ? 0 : tmp);

      tmp = dx + src->w;
      if (tmp > dst_cr ) tmp = dst_cr;
      w = tmp - dxbeg;
      if (w <= 0)
	 return;
	
      tmp = dst_ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst_cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if (h <= 0)
	 return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy;
   }

   s = (RLE_PTR) (src->dat);
   dxbeg += w;

   /* Clip top.  */
   for (y = sybeg - 1; y >= 0; y--) {
      long c = *s++;

      while (!RLE_IS_EOL(c)) {
	 if (c > 0)
	    s += c;
	 c = *s++;
      }
   }

   //@@@ bmp_select(dst);

   /* Visible part.  */
   for (y = 0; y < h; y++) {
      //@@@ PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);
      PIXEL_PTR d = (PIXEL_PTR) dst->pixels;
      d += (dybeg+y)*dst->pitch;
      d = OFFSET_PIXEL_PTR( d, dxbeg );
      long c = *s++;

      /* Clip left.  */
      for (x = sxbeg; x > 0; ) {
	 if (RLE_IS_EOL(c))
	    goto next_line;
	 else if (c > 0) {
	    /* Run of solid pixels.  */
	    if ((x - c) >= 0) {
	       /* Fully clipped.  */
	       x -= c;
	       s += c;
	    }
	    else {
	       /* Visible on the right.  */
	       c -= x;
	       s += x;
	       break;
	    }
	 }
	 else {
	    /* Run of transparent pixels.  */
	    if ((x + c) >= 0) {
	       /* Fully clipped.  */
	       x += c;
	    }
	    else {
	       /* Visible on the right.  */
	       c += x;
	       break;
	    }
	 }

	 c = *s++;
      }

      /* Visible part.  */
      for (x = w; x > 0; ) {
	 if (RLE_IS_EOL(c))
	    goto next_line;
	 else if (c > 0) {
	    /* Run of solid pixels.  */
	    if ((x - c) >= 0) {
	       /* Fully visible.  */
	       x -= c;
	       for (c--; c >= 0; s++, DEC_PIXEL_PTR(d), c--) {
		  unsigned long col = *s;
		  PUT_PIXEL(d, col);
	       }
	    }
	    else {
	       /* Clipped on the right.  */
	       c -= x;
	       for (x--; x >= 0; s++, DEC_PIXEL_PTR(d), x--) {
		  unsigned long col = *s;
		  PUT_PIXEL(d, col);
	       }
	       break;
	    }
	 }
	 else {
	    /* Run of transparent pixels.  */
	    x += c;
	    d = OFFSET_PIXEL_PTR(d, c);
	 }

	 c = *s++;
      }

      /* Clip right.  */
      while (!RLE_IS_EOL(c)) {
	 if (c > 0)
	    s += c;
	 c = *s++;
      }

   next_line: ;
   }

   //@@@bmp_unwrite_line(dst);
}

void draw_rle_sprite( RLE_SPRITE* src, int dx, int dy )
{
#define RLE_PTR					signed char*
#define RLE_IS_EOL(c)			((c) == 0)
#define PIXEL_PTR				unsigned char*
#define OFFSET_PIXEL_PTR(p,x)	((PIXEL_PTR) (p) + (x))
#define INC_PIXEL_PTR(p)		((p)++)
#define DEC_PIXEL_PTR(p)		((p)--)
#define PUT_PIXEL(p,c)			(*((unsigned char *)(p)) = (c))
//#define PUT_PIXEL(p,c)         bmp_write8((unsigned long) (p), (c))

   int x, y, w, h;		// width and height of visible area
   int dxbeg, dybeg;	// beginning in destination
   int sxbeg, sybeg;	// beginning in source
   RLE_PTR s;

   SDL_Surface* dst = gamescreen;
   // Clip to dst->clip_rect
   int dst_cl = dst->clip_rect.x;
   int dst_cr = dst->clip_rect.w + dst_cl;
   int dst_ct = dst->clip_rect.y;
   int dst_cb = dst->clip_rect.h + dst_ct;

// if (dst->clip)
   if (1)
   {

      int tmp;

      tmp = dst_cl - dx;
      sxbeg = ((tmp < 0) ? 0 : tmp);
      dxbeg = sxbeg + dx;

      tmp = dst_cr - dx;
      w = ((tmp > src->w) ? src->w : tmp) - sxbeg;
      if ( w<=0 ) return;

      tmp = dst_ct - dy;
      sybeg = ((tmp < 0) ? 0 : tmp);
      dybeg = sybeg + dy;

      tmp = dst_cb - dy;
      h = ((tmp > src->h) ? src->h : tmp) - sybeg;
      if ( h<=0 ) return;
   }
   else {
      w = src->w;
      h = src->h;
      sxbeg = 0;
      sybeg = 0;
      dxbeg = dx;
      dybeg = dy;
   }

   s = (RLE_PTR) (src->dat);

   /* Clip top.  */
   for (y = sybeg - 1; y >= 0; y--) {
      long c = *s++;

      while (!RLE_IS_EOL(c)) {
	 if (c > 0)
	    s += c;
	 c = *s++;
      }
   }

   //@@@ bmp_select(dst);

   /* Visible part.  */
   for (y = 0; y < h; y++) {
      //@@@ PIXEL_PTR d = OFFSET_PIXEL_PTR(bmp_write_line(dst, dybeg + y), dxbeg);
      PIXEL_PTR d = (PIXEL_PTR) dst->pixels;
      d += (dybeg+y)*dst->pitch;
      d = OFFSET_PIXEL_PTR( d, dxbeg );
      long c = *s++;

      /* Clip left.  */
      for (x = sxbeg; x > 0; ) {
	 if (RLE_IS_EOL(c))
	    goto next_line;
	 else if (c > 0) {
	    /* Run of solid pixels.  */
	    if ((x - c) >= 0) {
	       /* Fully clipped.  */
	       x -= c;
	       s += c;
	    }
	    else {
	       /* Visible on the right.  */
	       c -= x;
	       s += x;
	       break;
	    }
	 }
	 else {
	    /* Run of transparent pixels.  */
	    if ((x + c) >= 0) {
	       /* Fully clipped.  */
	       x += c;
	    }
	    else {
	       /* Visible on the right.  */
	       c += x;
	       break;
	    }
	 }

	 c = *s++;
      }

      /* Visible part.  */
      for (x = w; x > 0; ) {
	 if (RLE_IS_EOL(c))
	    goto next_line;
	 else if (c > 0) {
	    /* Run of solid pixels.  */
	    if ((x - c) >= 0) {
	       /* Fully visible.  */
	       x -= c;
	       for (c--; c >= 0; s++, INC_PIXEL_PTR(d), c--) {
		  unsigned long col = *s;
		  PUT_PIXEL(d, col);
	       }
	    }
	    else {
	       /* Clipped on the right.  */
	       c -= x;
	       for (x--; x >= 0; s++, INC_PIXEL_PTR(d), x--) {
		  unsigned long col = *s;
		  PUT_PIXEL(d, col);
	       }
	       break;
	    }
	 }
	 else {
	    /* Run of transparent pixels.  */
	    x += c;
	    d = OFFSET_PIXEL_PTR(d, -c);
	 }

	 c = *s++;
      }

      /* Clip right.  */
      while (!RLE_IS_EOL(c)) {
	 if (c > 0)
	    s += c;
	 c = *s++;
      }

   next_line: ;
   }

   //@@@bmp_unwrite_line(dst);
}

void RlePack::draw( int index, int x, int y, bool flipped )
{
	if ( (index<0) || (index>=p->count) )
		return;
	
	RLE_SPRITE* sprite = p->sprites[index];
	if (!sprite)
		return;
	
	if ( flipped )
		draw_rle_sprite_v_flip( sprite, x, y );
	else
		draw_rle_sprite( sprite, x, y );
	
}

