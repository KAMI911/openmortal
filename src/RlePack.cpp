/***************************************************************************
                          RlePack.cpp  -  description
                             -------------------
    begin                : Mon Sep 24 2001
    copyright            : (C) 2001 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include "RlePack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "common.h"


/// Sanity: This is the maximal number of entries in a .DAT file.
#define MAXDATACOUNT	65530


inline void ChangeEndian32( Uint32& a_riArg )
{
	char* pcArg = (char*)&a_riArg;
	char cTemp;
	cTemp = pcArg[0]; pcArg[0] = pcArg[3]; pcArg[3] = cTemp;
	cTemp = pcArg[1]; pcArg[1] = pcArg[2]; pcArg[2] = cTemp;
}


inline void ChangeEndian16( Uint16& a_riArg )
{
	char* pcArg = (char*)&a_riArg;
	char cTemp;
	cTemp = pcArg[0]; pcArg[0] = pcArg[1]; pcArg[1] = cTemp;
}


inline Uint32 ConvertEndian32( Uint32 a_iArg )
{
	char* pcArg = (char*)&a_iArg;
	char cTemp;
	cTemp = pcArg[0]; pcArg[0] = pcArg[3]; pcArg[3] = cTemp;
	cTemp = pcArg[1]; pcArg[1] = pcArg[2]; pcArg[2] = cTemp;
	return a_iArg;
}


inline Uint16 ConvertEndian16( Uint16 a_iArg )
{
	char* pcArg = (char*)&a_iArg;
	char cTemp;
	cTemp = pcArg[0]; pcArg[0] = pcArg[1]; pcArg[1] = cTemp;
	return a_iArg;
}



typedef struct RLE_SPRITE           /* a RLE compressed sprite */
{
	Uint16 dummy;					// For better alignment... NASTY NASTY HACK!!
	Uint16 color_depth;                 /* color depth of the image */
	Uint16 w, h;                        /* width and height in pixels */
	Uint32 size;
	signed char dat[0];
} RLE_SPRITE;


struct RlePack_P
{
	SDL_Color		m_aoPalette[256];
	SDL_Color		m_aoTintedPalette[256];
	TintEnum		m_enTint;
	int				m_iCount;
	int				m_iArraysize;
	RLE_SPRITE**	m_pSprites;
	void*			m_pData;
	
	int				m_iColorCount;
	int				m_iColorOffset;
};



RlePack::RlePack( const char* a_pcFilename, int a_iNumColors )
{
	p = new RlePack_P;
	p->m_enTint = NO_TINT;
	p->m_iCount = 0;
	p->m_iArraysize = 0;
	p->m_pSprites = NULL;
	p->m_pData = NULL;
	
	p->m_iColorCount = 0;
	p->m_iColorOffset = 0;
	
	// Load file and stuff
	
	LoadFile( a_pcFilename, a_iNumColors );
}


RlePack::~RlePack()
{
	if (!p)
		return;
	
	if (p->m_pSprites)
	{
		delete[] p->m_pSprites;
		p->m_pSprites = NULL;
	}
	
	free( p->m_pData );
	delete( p );
	p = NULL;
}


void RlePack::Clear()
{
	if ( p && p->m_pSprites )
	{
		delete[] p->m_pSprites;
		p->m_pSprites = NULL;
	}
}


int RlePack::LoadFile( const char* a_pcFilename, int a_iNumColors )
{
	FILE* f;
	
	f = fopen( a_pcFilename, "rb" );
	if (f==NULL)
	{
		debug( "Can't open file '%s'.\n", a_pcFilename );
		return -1;
	}
	
	fseek( f, 0, SEEK_END );
	long iFileSize = ftell ( f );
	p->m_pData = malloc( iFileSize );
	if ( NULL == p->m_pData )
	{
		fclose( f );
		return -1;
	}
	
	fseek( f, 0, SEEK_SET );
	int iRead = fread( p->m_pData, 1, iFileSize, f );
	fclose( f );
	
	p->m_iColorCount = a_iNumColors;
	
	if ( iFileSize != iRead )
	{
		debug( "Warning RlePack(): iFileSize=%d, iRead=%d\n", iFileSize, iRead );
	}
	
	struct SHeader
	{
		char	acDummy[8];
		Uint32	iDatacount;
	} *poHeader = (SHeader*) p->m_pData;
	
	ChangeEndian32( poHeader->iDatacount );
	debug( "File '%s' contains %d entries.\n", a_pcFilename, poHeader->iDatacount );
	
	if (poHeader->iDatacount>MAXDATACOUNT) poHeader->iDatacount = MAXDATACOUNT;		// Sanity
	
	p->m_iArraysize = poHeader->iDatacount;
	p->m_pSprites = new RLE_SPRITE*[ poHeader->iDatacount ];
	
	char* pcNext = ((char*)p->m_pData) + sizeof(SHeader);
	char* pcEnd = ((char*)p->m_pData) + iFileSize;
	
	while ( pcNext < pcEnd )
	{
		if ( 0 == strncmp( pcNext, "prop", 4 ) )
		{
			struct SProperty
			{
				char	acName[4];
				Uint32	iSize;
			} *poProperty = (SProperty*) (pcNext+4);
			ChangeEndian32( poProperty->iSize );
			
			pcNext += 4 + sizeof(SProperty) + poProperty->iSize;
		}
		else if ( 0 == strncmp( pcNext, "RLE ", 4 ) )
		{
			struct SRLE
			{
				RLE_SPRITE	oSprite;
			} *poRle = (SRLE*) (pcNext+10);
			poRle->oSprite.color_depth = ConvertEndian16(poRle->oSprite.color_depth);
			poRle->oSprite.w = ConvertEndian16(poRle->oSprite.w);
			poRle->oSprite.h = ConvertEndian16(poRle->oSprite.h);
			poRle->oSprite.size = ConvertEndian32(poRle->oSprite.size);
			
			p->m_pSprites[p->m_iCount] = &(poRle->oSprite);
			p->m_iCount++;
			pcNext += 10 + sizeof( SRLE ) + poRle->oSprite.size;
		}
		else if ( 0 == strncmp( pcNext, "PAL ", 4 ) )
		{
			struct SPAL
			{
				Uint32		iLength1;
				Uint32		iLength;
				SDL_Color	aoColors[256];
			} *poPal = (SPAL*) (pcNext+4);
			ChangeEndian32( poPal->iLength );
			
			int iNumColors = poPal->iLength>1024 ? 1024 : poPal->iLength;
			iNumColors /= 4;
			
			for (int i=0; i< iNumColors; i++)
			{
				p->m_aoPalette[i].r = poPal->aoColors[i].r*4;
				p->m_aoPalette[i].g = poPal->aoColors[i].g*4;
				p->m_aoPalette[i].b = poPal->aoColors[i].b*4;
				p->m_aoPalette[i].unused = 0;
				p->m_aoTintedPalette[i] = p->m_aoPalette[i];
			}
			
			pcNext += 4 + 8 + poPal->iLength;
		}
		else
		{
			struct SUnknown
			{
				Uint32	iSize;
			} *poUnknown = (SUnknown*) (pcNext+4);
			ChangeEndian32( poUnknown->iSize );
			
			debug( "Unknown: '%4s', size: %d\n", pcNext, poUnknown->iSize );
			
			pcNext += 4 + sizeof(SUnknown) + poUnknown->iSize;
		}
	}
	
	return p->m_iCount;
	
#if 0
	
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
	p->sprites = new RLE_SPRITE*[ datacount ];
	
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
			
			for (unsigned int i=0; i< pallength; i++)
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
#endif
}




int RlePack::Count()
{
	return p->m_iCount;
}


void OffsetRLESprite( RLE_SPRITE* spr, int offset ) // Static method
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

void RlePack::OffsetSprites( int a_iOffset )
{
	if ( (a_iOffset<=0) || (a_iOffset>255) )
		return;

	p->m_iColorOffset = a_iOffset;

	int i;
	
	// Offset every RLE_SPRITE
	
	for ( i=0; i<p->m_iCount; ++i )
	{
		OffsetRLESprite( p->m_pSprites[i], a_iOffset );
	}
}


void RlePack::SetTint( TintEnum a_enTint )
{
	int i;

	switch( a_enTint )
	{
		case ZOMBIE_TINT:
		{
			for ( i=0; i<p->m_iColorCount; ++i )
			{
				p->m_aoTintedPalette[i].r = 0;
				p->m_aoTintedPalette[i].g = p->m_aoPalette[i].g;
				p->m_aoTintedPalette[i].b = 0;
			}
			break;
		}

		case GRAY_TINT:
		{
			int j;
			for ( i=0; i<p->m_iColorCount; ++i )
			{
				j = (p->m_aoPalette[i].r + p->m_aoPalette[i].g + p->m_aoPalette[i].b)/4;
				p->m_aoTintedPalette[i].r = j;
				p->m_aoTintedPalette[i].g = j;
				p->m_aoTintedPalette[i].b = j;
			}
			break;
		}

		case DARK_TINT:
		{
			for ( i=0; i<p->m_iColorCount; ++i )
			{
				p->m_aoTintedPalette[i].r = int(p->m_aoPalette[i].r) * 2 / 3;
				p->m_aoTintedPalette[i].g = int(p->m_aoPalette[i].g) * 2 / 3;
				p->m_aoTintedPalette[i].b = int(p->m_aoPalette[i].b) * 2 / 3;
			}
			break;
		}

		case INVERTED_TINT:
		{
			for ( i=0; i<p->m_iColorCount; ++i )
			{
				p->m_aoTintedPalette[i].r = 255 - p->m_aoPalette[i].r;
				p->m_aoTintedPalette[i].g = 255 - p->m_aoPalette[i].g;
				p->m_aoTintedPalette[i].b = 255 - p->m_aoPalette[i].b;
			}
			break;
		}

		case NO_TINT:
		default:
		{
			for ( i=0; i<p->m_iColorCount; ++i )
			{
				p->m_aoTintedPalette[i] = p->m_aoPalette[i];
			}
			break;
		}
		
	} // end of switch( a_enTint )

}


void RlePack::ApplyPalette()
{
	SDL_SetColors( gamescreen, p->m_aoTintedPalette, p->m_iColorOffset, p->m_iColorCount );
}

/*
SDL_Color* RlePack::getPalette()
{
	return p->palette;
}
*/


void draw_rle_sprite_v_flip( RLE_SPRITE* src, int dx, int dy )	// static method
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

void draw_rle_sprite( RLE_SPRITE* src, int dx, int dy )		// static method
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

void RlePack::Draw( int a_iIndex, int a_iX, int a_iY, bool a_bFlipped )
{
	if ( (a_iIndex<0) || (a_iIndex>=p->m_iCount) )
		return;
	
	RLE_SPRITE* poSprite = p->m_pSprites[a_iIndex];
	if (!poSprite)
		return;
	
	if ( a_bFlipped )
		draw_rle_sprite_v_flip( poSprite, a_iX, a_iY );
	else
		draw_rle_sprite( poSprite, a_iX, a_iY );
	
}

