/***************************************************************************
                          PlayerSelect.cpp  -  description
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include <stdio.h>
 
#include <SDL/SDL.h>
#include <SDL/SDL_video.h>
#include <SDL/SDL_image.h>
#include "sge_primitives.h"
 
#include "common.h"
#include "Audio.h"
#include "gfx.h"
#include "RlePack.h"
#include "Backend.h"
#include "State.h"


#define CHOOSERLEFT		158
#define CHOOSERTOP		98
#define CHOOSERHEIGHT	80
#define CHOOSERWIDTH	80
#define CHOOSERROWS		4
#define CHOOSERCOLS		4

#ifndef NULL
#define NULL 0
#endif


/***************************************************************************
                     PUBLIC EXPORTED VARIABLES
***************************************************************************/

FighterEnum Fighter1 = ZOLI;
FighterEnum Fighter2 = ZOLI;
RlePack* pack1;
RlePack* pack2;


/***************************************************************************
                     PRIVATE VARIABLES (perl variable space)
***************************************************************************/

int		p1		= 0;
int		p2		= 3;
bool	done1	= false;
bool	done2	= false;



FighterEnum ChooserCells[CHOOSERROWS][CHOOSERCOLS] = {
	{ ZOLI, UPI, CUMI, SIRPI },
	{ ULMAR, MACI, BENCE, GRIZLI },
	{ AMBRUS, DESCANT, SURBA, DANI },
	{ UNKNOWN, KINGA, MISI, UNKNOWN },
};




bool IsFighterAvailable( FighterEnum a_enFighter )
{
	static bool g_abFighterAvailable[ LASTFIGHTER ] =
	{
		false, 
		true, true, true, true,
		true, true, true, true, 
		true, false, false, false,
		false, false
	};
	
	if ( a_enFighter < UNKNOWN 
		|| a_enFighter >= LASTFIGHTER )
	{
		return false;
	}
	
	return g_abFighterAvailable[ a_enFighter ];
}



RlePack* LoadFighter( FighterEnum fighter, int offset )
{
	char filename[FILENAME_MAX+1];
	const char* s;

	switch (fighter)
	{
		case ZOLI:		s = "ZOLIDATA.DAT"; break;
		case UPI:		s = "UPIDATA.DAT"; break;
		case ULMAR:		s = "ULMARDATA.DAT"; break;
		case CUMI:		s = "CUMIDATA.DAT"; break;
		case SIRPI:		s = "SIRPIDATA.DAT"; break;
		case MACI:		s = "MACIDATA.DAT"; break;
		case BENCE:		s = "BENCEDATA.DAT"; break;
		case DESCANT:	s = "DESCANTDATA.DAT"; break;
		case GRIZLI:	s = "GRIZLIDATA.DAT"; break;
		default:		s = "ZOLIDATA.DAT"; break;
	}

	strcpy( filename, DATADIR );
	strcat( filename, "/characters/" );
	strcat( filename, s );

	RlePack* pack = new RlePack( filename );
	pack->offsetSprites( offset );
	SDL_SetColors( gamescreen, pack->getPalette(), offset, 64 );
	return pack;
}


void TintFighter( RlePack* fighter, TintEnum tint, int offset )
{
	if ( tint == RANDOM_TINT )
	{
		switch (rand() % 4 )
		{
			case 0: tint = ZOMBIE_TINT; break;
			case 1: tint = GRAY_TINT; break;
			case 2: tint = DARK_TINT; break;
			case 3: tint = INVERTED_TINT; break;
		}
	}

	SDL_Color palette[64];
	int i;

	memcpy( palette, fighter->getPalette(), sizeof(palette) );

	switch( tint )
	{
		case ZOMBIE_TINT:
		{
			for ( i=0; i<64; ++i )
			{
				palette[i].r = 0;
				palette[i].b = 0;
			}
			break;
		}

		case GRAY_TINT:
		{
			int j;
			for ( i=0; i<64; ++i )
			{
				j = (palette[i].r + palette[i].g + palette[i].b)/4;
				palette[i].r = j;
				palette[i].g = j;
				palette[i].b = j;
			}
			break;
		}

		case DARK_TINT:
		{
			for ( i=0; i<64; ++i )
			{
				palette[i].r = int(palette[i].r) * 2 / 3;
				palette[i].g = int(palette[i].g) * 2 / 3;
				palette[i].b = int(palette[i].b) * 2 / 3;
			}
			break;
		}

		case INVERTED_TINT:
		{
			for ( i=0; i<64; ++i )
			{
				palette[i].r = 255 - palette[i].r;
				palette[i].g = 255 - palette[i].g;
				palette[i].b = 255 - palette[i].b;
			}
			break;
		}

		default:
			break;
	}

	SDL_SetColors( gamescreen, palette, offset, 64 );
}



void SetPlayer( int player, FighterEnum fighter )
{
	int offset = player ? 176 : 112;
	RlePack* pack = LoadFighter( fighter, offset );
	
	if ( player )
	{
		delete pack2;
		pack2 = pack;
	}
	else
	{
		delete pack1;
		pack1 = pack;
	}
	PERLCALL( "SetPlayerNumber", player, fighter );

	FighterEnum& thisPlayer = player ? Fighter2 : Fighter1;

	thisPlayer = fighter;
	if ( pack2 )
	{
		TintFighter( pack2, Fighter1 == Fighter2 ? RANDOM_TINT : NO_TINT, 176 );
	}
}







void handleKey( int player, int key )
{
	int& p = player ? p2 : p1;
	int oldp = p;
	bool& done = player ? done2 : done1;
	if ( done )
	{
		return;
	}
	
	
	switch ( key )
	{
		case 0:		// up
			if ( p >= CHOOSERCOLS ) p -= CHOOSERCOLS;
			break;
		case 1:		// down
			if ( (p/CHOOSERCOLS) < (CHOOSERROWS-1) ) p += CHOOSERCOLS;
			break;
		case 2:		// left
			if ( (p % CHOOSERCOLS) > 0 ) p--;
			break;
		case 3:		// right
			if ( (p % CHOOSERCOLS) < (CHOOSERCOLS-1) ) p++;
			break;
		default:
			if ( ChooserCells[p/4][p%4] )
			{
				done = true;
				PERLCALL( "PlayerSelected", player, 0 );
				Audio->PlaySample("magic.voc");
				return;
			}
	}

	if ( !IsFighterAvailable( ChooserCells[p/CHOOSERCOLS][p%CHOOSERCOLS] ) )
	{
		p = oldp;
	}

	if ( oldp != p )
	{
		Audio->PlaySample("strange_quack.voc");
		SetPlayer( player, ChooserCells[p/CHOOSERCOLS][p%CHOOSERCOLS] );
	}
}


void drawRect( int pos, int color )
{
	int row = pos / CHOOSERCOLS;
	int col = pos % CHOOSERCOLS;
	SDL_Rect r, r1;
	
	r.x = CHOOSERLEFT + col * CHOOSERWIDTH;
	r.y = CHOOSERTOP  + row * CHOOSERHEIGHT;
	r.w = CHOOSERWIDTH + 5;
	r.h = 5;
	r1 = r;
	SDL_FillRect( gamescreen, &r1, color );
	
	r.y += CHOOSERHEIGHT;
	r1 = r;
	SDL_FillRect( gamescreen, &r1, color );
	
	r.y -= CHOOSERHEIGHT;
	r.w = 5;
	r.h = CHOOSERHEIGHT + 5;
	r1 = r;
	SDL_FillRect( gamescreen, &r1, color );

	r.x += CHOOSERWIDTH;
	r1 = r;
	SDL_FillRect( gamescreen, &r1, color );
}


void checkPlayer( SDL_Surface* background, int row, int col )
{
	int x1, y1;

	x1 = CHOOSERLEFT + col * CHOOSERWIDTH +5;
	y1 = CHOOSERTOP  + row * CHOOSERHEIGHT +5;
	
	sge_Line(background, x1+5, y1+5, x1 + CHOOSERWIDTH-10, y1 + CHOOSERHEIGHT-10, 252);
	sge_Line(background, x1 + CHOOSERWIDTH-10, y1+5, x1+5, y1 + CHOOSERHEIGHT-10, 252);
	x1++;
	sge_Line(background, x1+5, y1+5, x1 + CHOOSERWIDTH-10, y1 + CHOOSERHEIGHT-10, 252);
	sge_Line(background, x1 + CHOOSERWIDTH-10, y1+5, x1+5, y1 + CHOOSERHEIGHT-10, 252);
	y1++;
	sge_Line(background, x1+5, y1+5, x1 + CHOOSERWIDTH-10, y1 + CHOOSERHEIGHT-10, 252);
	sge_Line(background, x1 + CHOOSERWIDTH-10, y1+5, x1+5, y1 + CHOOSERHEIGHT-10, 252);
	x1--;
	sge_Line(background, x1+5, y1+5, x1 + CHOOSERWIDTH-10, y1 + CHOOSERHEIGHT-10, 252);
	sge_Line(background, x1 + CHOOSERWIDTH-10, y1+5, x1+5, y1 + CHOOSERHEIGHT-10, 252);
}


void PlayerSelect()
{
	SDL_FillRect( gamescreen, NULL, C_BLACK );
	SDL_Flip( gamescreen );

	char filename[FILENAME_MAX+1];
	strcpy( filename, DATADIR );
	strcat( filename, "/gfx/" );
	strcat( filename, "PlayerSelect.png" );
	
	SDL_Surface* background = IMG_Load( filename );
	if (!background)
	{
		debug( "Can't load file: %s\n", filename );
		g_oState.m_bQuitFlag = true;
		return;
	}

	SDL_Palette* pal = background->format->palette;
	if ( pal )
	{
		int ncolors = pal->ncolors;
		if (ncolors>111) ncolors = 111;
		SDL_SetColors( gamescreen, pal->colors, 0, ncolors );
	}
	background = SDL_DisplayFormat( background );
	
	DrawGradientText( "Choose A Fighter Dammit", titleFont, 10, background );
	
	for ( int i=0; i<CHOOSERROWS; ++i )
	{
		for ( int j=0; j<CHOOSERCOLS; ++j )
		{
			if ( !IsFighterAvailable(ChooserCells[i][j]) &&
				UNKNOWN != ChooserCells[i][j] )
			{
				checkPlayer( background, i, j );
			}
		}
	}
	
	if (pack1) SDL_SetColors( gamescreen, pack1->getPalette(), 112, 64 );
	if (pack2) SDL_SetColors( gamescreen, pack2->getPalette(), 176, 64 );

	PERLEVAL( "SelectStart();" );

	done1 = done2 = false;
	
	int thisTick, lastTick, gameSpeed;

	gameSpeed = 12 ;
	thisTick = SDL_GetTicks() / gameSpeed;
	lastTick = thisTick - 1;
	
	int		i		= 0;
	bool	bDraw	= true;
	int		over	= 0;
	
	int		iCourtain = 0;
	int		iCourtainSpeed = 0;
	int		iCourtainTime = 80;
	
	SDL_Event event;

	while (1)
	{
		// 1. Wait for the next tick (on extremely fast machines..)
		while (1)
		{
			thisTick = SDL_GetTicks() / gameSpeed;
			if ( thisTick==lastTick )
			{
				SDL_Delay(1);
			}
			else
			{
				break;
			}
		}

		// 2. Advance as many ticks as necessary..
		
		if ( iCourtainTime > 0 )
		{
			int iAdvance = thisTick - lastTick;
			if ( iAdvance > 5 ) iAdvance = 5;
			
			if ( iCourtain + iCourtainSpeed * iCourtainTime /2 < 320 * 4 )
				iCourtainSpeed += iAdvance;
			else
				iCourtainSpeed -= iAdvance;
				
			iCourtain += iCourtainSpeed * iAdvance;
			iCourtainTime -= iAdvance;
			
			if ( iCourtainTime > 0 )
			{
				SDL_Rect oRect;
				oRect.x = 320 - iCourtain/4; oRect.y = 0;
				oRect.w = iCourtain / 2; oRect.h = gamescreen->h;
				if ( oRect.x < 0 ) oRect.x = 0;
				if ( oRect.w > gamescreen->w ) oRect.w = gamescreen->w;
				SDL_SetClipRect( gamescreen, &oRect );
			}
			else
			{
				SDL_SetClipRect( gamescreen, NULL );
			}
		}
		
		int iNumFrames = thisTick - lastTick;
		if ( iNumFrames>5 ) iNumFrames = 5;
		for ( int i=0; i<iNumFrames; ++i )
		{
			g_oBackend.AdvancePerl();
		}
		lastTick = thisTick;
		
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					g_oState.m_bQuitFlag = true;
					break;
				case SDL_KEYDOWN:
				{
					if ( event.key.keysym.sym == SDLK_ESCAPE )
					{
						DoMenu( false );
						break;
					}

					for (int i=0; i<2; i++)
					{
						for (int j=0; j<9; j++ )
						{
							if (g_oState.m_aiPlayerKeys[i][j] == event.key.keysym.sym)
							{
								drawRect( p1, 240 );
								drawRect( p2, 240 );
								handleKey( i, j );
							}
						}
					}
				}
				break;
			}	// switch statement
		}	// Polling events
		
		int p1x = SvIV(get_sv("p1x", FALSE));
		int p1y = SvIV(get_sv("p1y", FALSE));
		int p1f = SvIV(get_sv("p1f", FALSE));
		int p2x = SvIV(get_sv("p2x", FALSE));
		int p2y = SvIV(get_sv("p2y", FALSE));
		int p2f = SvIV(get_sv("p2f", FALSE));
		over = SvIV(get_sv("over", FALSE));
		
		SDL_BlitSurface( background, NULL, gamescreen, NULL );
		if (p1f) pack1->draw( ABS(p1f)-1, p1x, p1y, p1f<0 );
		if (p2f) pack2->draw( ABS(p2f)-1, p2x, p2y, p2f<0 );
		
		if ( !done1) drawRect( p1, 250 );
		if ( !done2) drawRect( p2, 253 );
		SDL_Flip( gamescreen );

		if (over || g_oState.m_bQuitFlag || SState::IN_DEMO == g_oState.m_enGameMode) break;
	}

	SDL_FreeSurface( background );
	SDL_SetClipRect( gamescreen, NULL );
	return;
}

