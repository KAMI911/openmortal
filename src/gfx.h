/***************************************************************************
                          gfx.h  -  description
                             -------------------
    begin                : Tue Apr 10 2001
    copyright            : (C) 2001 by UPi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/


#ifndef _GFX_H
#define _GFX_H


struct _sge_TTFont;

enum GFX_Constants {
	AlignHCenter	= 1,
	AlignVCenter	= 2,
	AlignCenter		= 3,
	UseTilde		= 4,
	UseShadow		= 8,
};

int				DrawTextMSZ( const char* text, _sge_TTFont* font, int x, int y,
					int flags, int fg, SDL_Surface* target, bool a_bTranslate = true );

void			DrawGradientText( const char* text, _sge_TTFont* font, int y,
					SDL_Surface* target, bool a_bTranslate = true );

SDL_Color		MakeColor( Uint8 r, Uint8 g, Uint8 b );

SDLKey			GetKey();
SDL_Surface*	LoadBackground( const char* a_pcFilename, int a_iNumColors, int a_iPaletteOffset=0 );

extern _sge_TTFont* titleFont;		// Largest font, for titles
extern _sge_TTFont* inkFont;		// Medium-size front, headings
extern _sge_TTFont* impactFont;		// Smallest font, for long descriptions

#ifdef sge_bm_text_H
extern sge_bmpFont* fastFont;		// In-game text, e.g. combo text
extern sge_bmpFont* creditsFont;
extern sge_bmpFont* storyFont;
#endif

#endif
