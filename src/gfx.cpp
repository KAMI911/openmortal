/***************************************************************************
                          gfx.cpp  -  description
                             -------------------
    begin                : Tue Apr 10 2001
    copyright            : (C) 2001 by UPi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include <string.h>

#include "SDL.h"
#include "SDL_video.h"
#include "SDL_image.h"
#include "sge_tt_text.h"
#include "sge_surface.h"

#ifndef  __COMMON_H
#include "common.h"
#endif
#ifndef  _GFX_H
#include "gfx.h"
#endif
#include "State.h"


void sge_TTF_SizeText( _sge_TTFont*font, const char* string, int* x, int* y )
{
	SDL_Rect r = sge_TTF_TextSize( font, (char*) string );
	*x = r.w;
	*y = r.h;
}

int DrawTextMSZ( const char* string, _sge_TTFont* font, int x, int y, int flags,
	int fg, SDL_Surface* target )
{
    int retval = 0;

	if (!string || !*string) return retval;

	if (flags & UseTilde)
	{
		char *str2 = strdup( string );
		char onechar[2];
		char *c1, *c2, *c3;
		int w = 0;
		int i, j;
		bool notend;

		if (flags & AlignHCenter)
		{
			// Determine width of the string without the stupid tildes
			c1 = c2 = str2;
			notend = true;


			while (notend)
			{
				c2 = c1;								// c1: start of this run
				while (*c2 && (*c2!='~')) c2++;			// c2: end of this run
				notend = *c2;
				*c2 = 0;

				sge_TTF_SizeText( font, c1, &i, &j);
				w += i;

				if (notend) { *c2='~'; c1=c2+1; }		// next run..
			}

			x -= w/2;
		}

		flags &= ~(UseTilde | AlignHCenter);

		c1 = str2;
		onechar[1]=0;
		notend = true;

		while (1)
		{
			c2 = c1;
			while (*c2 && (*c2!='~')) c2++;			// c2: end of this run
			notend = *c2;
			*c2 = 0;

			sge_TTF_SizeText( font, c1, &i, &j);
			DrawTextMSZ( c1, font, x, y, flags, fg, target );
			x += i;

			// At this point, we're either at a ~ or end of the text (notend)
			if (!notend) break;

			onechar[0]= *++c2;						// Could be 0, if ~ at end.
			if (!onechar[0]) break;
			sge_TTF_SizeText( font, onechar, &i, &j);
			DrawTextMSZ( onechar, font, x, y, flags, C_LIGHTCYAN, target );
			x += i;
			retval += i;
			c1 = c2+1;
			

			if (!*c1) break;						// ~X was end of string
		}
		
		delete( str2 );
		return retval;
	}
	
	SDL_Rect dest;
	int w, h;
	sge_TTF_SizeText( font, string, &w, &h );
	dest.w = retval = w;
	dest.h = h;
	dest.x = flags & AlignHCenter ? x-dest.w/2 : x;
	dest.y = flags & AlignVCenter ? y-dest.h/2 : y;

	//debug( "X: %d, Y: %d, W: %d, H: %d\n", dest.x, dest.y, dest.w, dest.h );
	if ( flags & UseShadow )
	{
		sge_tt_textout_UTF8( target, font, string, dest.x+2, dest.y+2+sge_TTF_FontAscent(font), C_BLACK, C_BLACK, 255 );
	}

	sge_TTF_AAOn();
	dest = sge_tt_textout_UTF8( target, font, string, dest.x, dest.y+sge_TTF_FontAscent(font), fg, C_BLACK, 255 );
	sge_TTF_AAOff();
	
	return dest.w;
}



void DrawGradientText( const char* text, _sge_TTFont* font, int y, SDL_Surface* target )
{
	int i, j;

	// 1. CREATE OFFSCREEN SURFACE
	
	SDL_Rect size = sge_TTF_TextSize( font, (char*)text );
	size.w += 2;
	size.h += 2;
	size.x = 320 - size.w / 2;
	if ( size.x < 0 ) size.x = 0;
	size.y = y;
	SDL_Surface* surface = SDL_CreateRGBSurface( SDL_SRCCOLORKEY, size.w, size.h, 8, 0,0,0,0 );

	if ( NULL == surface )
	{
		debug( "DrawGradientText: Couldn't allocate %d by %d surface!\n", size.w, size.h );
		return;
	}

	// 2. SET OFFSCREEN SURFACE COLORS

	SDL_SetColorKey( surface, SDL_SRCCOLORKEY, 0 );
	SDL_Color colors[256];
	colors[0].r = colors[0].g = colors[0].b = 0;
	colors[1] = colors[0];

	// The rest is red->yellow gradient.

	for ( i=2; i<255; ++i )
	{
		int j = i > 25 ? i-25 : 0;
		colors[i].r = 255;
		colors[i].g = 255-j;
		colors[i].b = 0;
	}

	SDL_SetColors( surface, colors, 0, 256 );

	// 3. DRAW TEXT, APPLY BORDER, APPLY GRADIENT.

	int y1 = sge_TTF_FontAscent(font);
	sge_tt_textout_UTF8( surface, font, text,
		1, y1, 255, 0, 255);


	for ( y=1; y<size.h-1; ++y )
	{
		int color = 254 * y / (size.h-1) + 1;
		unsigned char *p0, *p1, *p2;
		p1 = (unsigned char*) surface->pixels;
		p1 += surface->pitch * y + 1;
		p0 = p1 - surface->pitch;
		p2 = p1 + surface->pitch;
		
		for ( int x=1; x<size.w-1; ++x, ++p0, ++p1, ++p2 )
		{
			if ( *p1 > 2 )
			{
				*p1 = color;
			}
			else
			{
				if ( (*(p1-1) > 2) || (*(p1+1) > 2) || *p0 > 2 || *p2 > 2 )
				{
					*p1 = 1;
				}
			}
		}
	}

	// 4. FINALLY

	SDL_BlitSurface( surface, NULL, target, &size );
	SDL_FreeSurface( surface );
	SDL_UpdateRect( target, size.x, size.y, size.w, size.h );
}



SDL_Color MakeColor( Uint8 r, Uint8 g, Uint8 b )
{
	SDL_Color color;
	color.r = r; color.g = g; color.b = b; color.unused = 0;
	return color;
}


SDLKey GetKey()
{
	SDL_Event event;
	
	while (SDL_WaitEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				g_oState.m_bQuitFlag = true;
				return SDLK_ESCAPE;
			
			case SDL_KEYDOWN:
			{
				return event.key.keysym.sym;
			}
			break;
		}	// switch statement
	}	// Polling events

	return SDLK_ESCAPE;
}



SDL_Surface* LoadBackground( const char* a_pcFilename, int a_iNumColors, int a_iPaletteOffset )
{
	char filepath[FILENAME_MAX+1];
	strcpy( filepath, DATADIR );
	strcat( filepath, "/gfx/" );
	strcat( filepath, a_pcFilename );

	SDL_Surface* poBackground = IMG_Load( filepath );
	if (!poBackground)
	{
		debug( "Can't load file: %s\n", filepath );
		return NULL;
	}
	
	SDL_Palette* pal = poBackground->format->palette;
	if ( pal )
	{
		int ncolors = pal->ncolors;
		if (ncolors>a_iNumColors) ncolors = a_iNumColors;
		if (ncolors+a_iPaletteOffset > 255) ncolors = 255 - a_iPaletteOffset;
		SDL_SetColors( gamescreen, pal->colors, a_iPaletteOffset, ncolors );
	}
	
	SDL_Surface* poRetval = SDL_DisplayFormat( poBackground );
	SDL_FreeSurface( poBackground );

	return poRetval;
}


