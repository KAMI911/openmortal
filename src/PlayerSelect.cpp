/***************************************************************************
                          PlayerSelect.cpp  -  description
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include <stdio.h>

#include "PlayerSelect.h"
 
#include "SDL.h"
#include "SDL_video.h"
#include "SDL_image.h"
#include "sge_primitives.h"
 
#include "common.h"
#include "Audio.h"
#include "sge_bm_text.h"
#include "gfx.h"
#include "RlePack.h"
#include "Backend.h"
#include "State.h"
#include "MortalNetwork.h"


#define CHOOSERLEFT		158
#define CHOOSERTOP		74
#define CHOOSERHEIGHT	80
#define CHOOSERWIDTH	80
#define CHOOSERROWS		5
#define CHOOSERCOLS		4

#ifndef NULL
#define NULL 0
#endif


/***************************************************************************
                     PUBLIC EXPORTED VARIABLES
***************************************************************************/


PlayerSelect g_oPlayerSelect;


/***************************************************************************
                     PRIVATE VARIABLES (perl variable space)
***************************************************************************/

/*
int		p1		= 0;
int		p2		= 3;
bool	done1	= false;
bool	done2	= false;
*/


/*
FighterEnum ChooserCells[CHOOSERROWS][CHOOSERCOLS] = {
	{ ZOLI, UPI, CUMI, SIRPI },
	{ ULMAR, MACI, BENCE, GRIZLI },
	{ AMBRUS, DESCANT, SURBA, DANI },
	{ UNKNOWN, KINGA, MISI, UNKNOWN },
};
*/

FighterEnum ChooserCells[CHOOSERROWS][CHOOSERCOLS] = {
	{ ZOLI, UPI, CUMI, SIRPI },
	{ ULMAR, MACI, GRIZLI, DESCANT },
	{ DANI, AMBRUS, BENCE, SURBA },
	{ (FighterEnum)100, (FighterEnum)101, (FighterEnum)102, (FighterEnum)103 },
	{ (FighterEnum)104, (FighterEnum)105, KINGA, MISI }
};



PlayerSelect::PlayerSelect()
{
	for ( int i=0; i<2; ++i )
	{
		m_aoPlayers[i].m_enFighter = UNKNOWN;
		m_aoPlayers[i].m_enTint = NO_TINT;
		m_aoPlayers[i].m_poPack = NULL;
	}
	
	m_iP1 = 0;
	m_iP2 = CHOOSERCOLS-1;
}


const PlayerInfo& PlayerSelect::GetPlayerInfo( int a_iPlayer )
{
	return m_aoPlayers[ a_iPlayer ? 1 : 0 ];
}


const char* PlayerSelect::GetFighterName( int a_iPlayer )
{
	return m_aoPlayers[ a_iPlayer ? 1 : 0 ].m_sFighterName.c_str();
}


int PlayerSelect::GetFighterNameWidth( int a_iPlayer )
{
	return m_aiFighterNameWidth[ a_iPlayer ? 1 : 0 ];
}


bool PlayerSelect::IsFighterAvailable( FighterEnum a_enFighter )
{
	if ( a_enFighter <= UNKNOWN )
	{
		return false;
	}
	
	g_oBackend.PerlEvalF("GetFighterStats(%d);", a_enFighter);
	const char* pcDatafile = g_oBackend.GetPerlString("Datafile");
	return ( pcDatafile && *pcDatafile );
}



/** LoadFighter simply looks up the filename associated with the given
fighter, loads it, and returns the RlePack. 

\return The freshly loaded RlePack, or NULL if it could not be loaded.
*/

RlePack* PlayerSelect::LoadFighter( FighterEnum m_enFighter )		// static
{
	char a_pcFilename[FILENAME_MAX+1];
	const char* s;
	
	g_oBackend.PerlEvalF( "GetFighterStats(%d);", m_enFighter );
	s = g_oBackend.GetPerlString( "Datafile" );

	strcpy( a_pcFilename, DATADIR );
	strcat( a_pcFilename, "/characters/" );
	strcat( a_pcFilename, s );

	RlePack* pack = new RlePack( a_pcFilename, COLORSPERPLAYER );
	if ( pack->Count() <= 0 )
	{
		debug( "Couldn't load RlePack: '%s'\n", a_pcFilename );
		delete pack;
		return NULL;
	}
	
	return pack;
}



/** SetPlayer loads the given fighter for the given player.

The RlePack is loaded first. If that succeeds, then the perl backend is
set too. The tint and palette of both players are set. */

void PlayerSelect::SetPlayer( int a_iPlayer, FighterEnum a_enFighter )
{
	if ( a_iPlayer ) a_iPlayer = 1;		// It's 0 or 1.
	
	if ( m_aoPlayers[a_iPlayer].m_enFighter == a_enFighter )
	{
		return;
	}
	if ( !IsFighterAvailable( a_enFighter ) )
	{
		return;
	}
	
	int iOffset = a_iPlayer ? COLOROFFSETPLAYER2 : COLOROFFSETPLAYER1;
	RlePack* poPack = LoadFighter( a_enFighter );
	poPack->OffsetSprites( iOffset );
	
	if ( NULL == poPack )
	{
		debug( "SetPlayer(%d,%d): Couldn't load RlePack\n", a_iPlayer, a_enFighter );
		return;
	} 
	
	delete m_aoPlayers[a_iPlayer].m_poPack;
	m_aoPlayers[a_iPlayer].m_poPack = poPack;
	m_aoPlayers[a_iPlayer].m_enFighter = a_enFighter;
	
	g_oBackend.PerlEvalF( "SetPlayerNumber(%d,%d);", a_iPlayer, a_enFighter );
	m_aoPlayers[a_iPlayer].m_sFighterName = g_oBackend.GetPerlString( "PlayerName" );
	m_aiFighterNameWidth[a_iPlayer] = sge_BF_TextSize( fastFont, GetFighterName(a_iPlayer) ).w;
	
	TintEnum enTint = NO_TINT;
	
	if ( m_aoPlayers[0].m_enFighter == m_aoPlayers[1].m_enFighter )
	{
		enTint = TintEnum( (rand() % 4) + 1 );
	}
	SetTint( 1, enTint );
	m_aoPlayers[a_iPlayer].m_poPack->ApplyPalette();
}



void PlayerSelect::SetTint( int a_iPlayer, TintEnum a_enTint )
{
	m_aoPlayers[a_iPlayer].m_enTint = a_enTint;
	if ( m_aoPlayers[a_iPlayer].m_poPack )
	{
		m_aoPlayers[a_iPlayer].m_poPack->SetTint( a_enTint );
		m_aoPlayers[a_iPlayer].m_poPack->ApplyPalette();
	}
}



bool PlayerSelect::IsNetworkGame()
{
	return SState::IN_NETWORK == g_oState.m_enGameMode;
}


void PlayerSelect::HandleKey( int a_iPlayer, int a_iKey )
{
	// If we are in network mode, all keys count as the local player's...
	if ( IsNetworkGame() )
	{
		a_iPlayer = g_poNetwork->IsMaster() ? 0 : 1;
	}
	
	int& riP = a_iPlayer ? m_iP2 : m_iP1;
	int iOldP = riP;
	
	bool& rbDone = a_iPlayer ? m_bDone2 : m_bDone1;
	if ( rbDone )
	{
		return;
	}
	
	switch ( a_iKey )
	{
		case 0:		// up
			if ( riP >= CHOOSERCOLS ) riP -= CHOOSERCOLS;
			break;
		case 1:		// down
			if ( (riP/CHOOSERCOLS) < (CHOOSERROWS-1) ) riP += CHOOSERCOLS;
			break;
		case 2:		// left
			if ( (riP % CHOOSERCOLS) > 0 ) riP--;
			break;
		case 3:		// right
			if ( (riP % CHOOSERCOLS) < (CHOOSERCOLS-1) ) riP++;
			break;
		default:
			if ( IsFighterAvailable( ChooserCells[riP/CHOOSERCOLS][riP%CHOOSERCOLS] ) )
			{
				Audio->PlaySample("magic.voc");
				
				rbDone = true;
				g_oBackend.PerlEvalF( "PlayerSelected(%d);", a_iPlayer );
				if ( IsNetworkGame() )
				{
					g_poNetwork->SendFighter( ChooserCells[riP/CHOOSERCOLS][riP%CHOOSERCOLS] );
					g_poNetwork->SendReady();
				}
				return;
			}
	}

	if ( iOldP != riP )
	{
		Audio->PlaySample("strange_quack.voc");
		if ( IsFighterAvailable( ChooserCells[riP/CHOOSERCOLS][riP%CHOOSERCOLS] ) )
		{
			if ( IsNetworkGame() )
			{
				g_poNetwork->SendFighter( ChooserCells[riP/CHOOSERCOLS][riP%CHOOSERCOLS] );
			}
			SetPlayer( a_iPlayer, ChooserCells[riP/CHOOSERCOLS][riP%CHOOSERCOLS] );
		}
	}
}


void PlayerSelect::HandleNetwork()
{
	g_poNetwork->Update();
	
	bool bMaster = g_poNetwork->IsMaster();
	int iPlayer = bMaster ? 1 : 0;
	int& riP = bMaster ? m_iP2 : m_iP1;
	bool& rbDone = bMaster ? m_bDone2 : m_bDone1;

	if ( rbDone )
	{
		return;
	}

	int iOldP = riP;
	FighterEnum enOldFighter = ChooserCells[iOldP/CHOOSERCOLS][iOldP%CHOOSERCOLS];
	FighterEnum enRemoteFighter = g_poNetwork->GetRemoteFighter();

	if ( enOldFighter != enRemoteFighter
		&& enRemoteFighter != UNKNOWN )
	{
		SetPlayer( iPlayer, enRemoteFighter );
		int i, j;
		for ( i=0; i<CHOOSERROWS; ++i )
		{
			for ( int j=0; j<CHOOSERCOLS; ++j )
			{
				if ( ChooserCells[i][j] == enRemoteFighter )
				{
					riP = i * CHOOSERCOLS + j;
					break;
				}
			}
		}
	}

	bool bDone = g_poNetwork->IsRemoteSideReady();
	if ( bDone )
	{
		rbDone = true;
		Audio->PlaySample("magic.voc");
		g_oBackend.PerlEvalF( "PlayerSelected(%d);", iPlayer );
	}
}


void PlayerSelect::DrawRect( int a_iPos, int a_iColor )
{
	int iRow = a_iPos / CHOOSERCOLS;
	int iCol = a_iPos % CHOOSERCOLS;
	SDL_Rect r, r1;
	
	r.x = CHOOSERLEFT + iCol * CHOOSERWIDTH;
	r.y = CHOOSERTOP  + iRow * CHOOSERHEIGHT;
	r.w = CHOOSERWIDTH + 5;
	r.h = 5;
	r1 = r;
	SDL_FillRect( gamescreen, &r1, a_iColor );
	
	r.y += CHOOSERHEIGHT;
	r1 = r;
	SDL_FillRect( gamescreen, &r1, a_iColor );
	
	r.y -= CHOOSERHEIGHT;
	r.w = 5;
	r.h = CHOOSERHEIGHT + 5;
	r1 = r;
	SDL_FillRect( gamescreen, &r1, a_iColor );

	r.x += CHOOSERWIDTH;
	r1 = r;
	SDL_FillRect( gamescreen, &r1, a_iColor );
}


void PlayerSelect::CheckPlayer( SDL_Surface* a_poBackground, int a_iRow, int a_iCol )
{
	int x1, y1;

	x1 = CHOOSERLEFT + a_iCol * CHOOSERWIDTH +5;
	y1 = CHOOSERTOP  + a_iRow * CHOOSERHEIGHT +5;
	
	sge_Line(a_poBackground, x1+5, y1+5, x1 + CHOOSERWIDTH-10, y1 + CHOOSERHEIGHT-10, 252);
	sge_Line(a_poBackground, x1 + CHOOSERWIDTH-10, y1+5, x1+5, y1 + CHOOSERHEIGHT-10, 252);
	x1++;
	sge_Line(a_poBackground, x1+5, y1+5, x1 + CHOOSERWIDTH-10, y1 + CHOOSERHEIGHT-10, 252);
	sge_Line(a_poBackground, x1 + CHOOSERWIDTH-10, y1+5, x1+5, y1 + CHOOSERHEIGHT-10, 252);
	y1++;
	sge_Line(a_poBackground, x1+5, y1+5, x1 + CHOOSERWIDTH-10, y1 + CHOOSERHEIGHT-10, 252);
	sge_Line(a_poBackground, x1 + CHOOSERWIDTH-10, y1+5, x1+5, y1 + CHOOSERHEIGHT-10, 252);
	x1--;
	sge_Line(a_poBackground, x1+5, y1+5, x1 + CHOOSERWIDTH-10, y1 + CHOOSERHEIGHT-10, 252);
	sge_Line(a_poBackground, x1 + CHOOSERWIDTH-10, y1+5, x1+5, y1 + CHOOSERHEIGHT-10, 252);
}


void PlayerSelect::DoPlayerSelect()
{
	// 1. Set up: Load background, mark unavailable fighters

	SDL_FillRect( gamescreen, NULL, C_BLACK );
	SDL_Flip( gamescreen );

	SDL_Surface* poBackground = LoadBackground( "PlayerSelect.png", 111 );
	
	DrawGradientText( "Choose A Fighter Dammit", titleFont, 10, poBackground );

	int i, j;	
	for ( i=0; i<CHOOSERROWS; ++i )
	{
		for ( int j=0; j<CHOOSERCOLS; ++j )
		{
			if ( !IsFighterAvailable(ChooserCells[i][j]) &&
				UNKNOWN != ChooserCells[i][j] )
			{
				CheckPlayer( poBackground, i, j );
			}
		}
	}

	for ( i=0; i<2; ++i )
	{
		if ( m_aoPlayers[i].m_poPack ) m_aoPlayers[i].m_poPack->ApplyPalette();
	}	
	
	SetPlayer( 0, ChooserCells[m_iP1/CHOOSERCOLS][m_iP1%CHOOSERCOLS] );
	SetPlayer( 1, ChooserCells[m_iP2/CHOOSERCOLS][m_iP2%CHOOSERCOLS] );
	
	// 2. Run selection screen
	
	g_oBackend.PerlEvalF( "SelectStart();" );

	m_bDone1 = m_bDone2 = false;
	
	int thisTick, lastTick, gameSpeed;

	gameSpeed = 12 ;
	thisTick = SDL_GetTicks() / gameSpeed;
	lastTick = thisTick - 1;
	
			i		= 0;
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
		for ( i=0; i<iNumFrames; ++i )
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

					for ( i=0; i<2; i++ )
					{
						for ( j=0; j<9; j++ )
						{
							if (g_oState.m_aiPlayerKeys[i][j] == event.key.keysym.sym)
							{
								DrawRect( m_iP1, 240 );
								DrawRect( m_iP2, 240 );
								HandleKey( i, j );
							}
						}
					}
				}
				break;
			}	// switch statement
		}	// Polling events
		
		if ( IsNetworkGame() )
		{
			HandleNetwork();
		}
		
		g_oBackend.ReadFromPerl();
		
		over = g_oBackend.m_iGameOver;
		
		SDL_BlitSurface( poBackground, NULL, gamescreen, NULL );
		if ( !m_bDone1) DrawRect( m_iP1, 250 );
		if ( !m_bDone2) DrawRect( m_iP2, 253 );
		
		for ( i=0; i<2; ++i )
		{
			if ( g_oBackend.m_aoPlayers[i].m_iFrame )
			{
				m_aoPlayers[i].m_poPack->Draw( 
					ABS(g_oBackend.m_aoPlayers[i].m_iFrame)-1,
					g_oBackend.m_aoPlayers[i].m_iX, g_oBackend.m_aoPlayers[i].m_iY,
					g_oBackend.m_aoPlayers[i].m_iFrame < 0 );
			}
			int x = ( CHOOSERLEFT - m_aiFighterNameWidth[i] ) / 2;
			if ( x<10 ) x = 10;
			if ( i ) x = gamescreen->w - x - m_aiFighterNameWidth[i];
			
			sge_BF_textout( gamescreen, fastFont, GetFighterName(i),
				x, gamescreen->h - 30 );
		}
		
		SDL_Flip( gamescreen );

		if (over || g_oState.m_bQuitFlag || SState::IN_DEMO == g_oState.m_enGameMode) break;
	}

	SDL_FreeSurface( poBackground );
	SDL_SetClipRect( gamescreen, NULL );
	return;
}

