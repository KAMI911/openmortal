/***************************************************************************
                          Game.cpp  -  description
                             -------------------
    begin                : Mon Sep 24 2001
    copyright            : (C) 2001 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include <string.h>
#include <stdio.h>
#include "SDL_image.h"
#include "sge_surface.h"
#include "sge_primitives.h"
#include "sge_bm_text.h"
#include "SDL_events.h"
#include "SDL_keysym.h"
#include <fstream>

#include "PlayerSelect.h"
#include "common.h"
#include "gfx.h"
#include "Backend.h"
#include "RlePack.h"
#include "State.h"
#include "Game.h"
#include "Audio.h"


#include <EXTERN.h>
#include <perl.h>


extern PerlInterpreter*	my_perl;


/*

GAME PALETTE

From   To   Num   Desc
---------------------------------------
  0 ..  63   64   Background
 64 .. 111   48   Doodads
112 .. 175   64   1st Fighter
176 .. 239   64   2nd Fighter
240 .. 255   16   basic colors

*/

#define MAXFRAMESKIP 5



struct SFpsCounter
{
	int m_iLastCheck;		// Last second then Tick() was called
	int m_iFrames;			// The number of frames in this second so far
	int m_iFps;				// The number of frames in the last second
	
	void Reset()
	{
		m_iLastCheck = m_iFrames = m_iFps = 0;
	}
	
	void Tick()
	{
		int iSecond = SDL_GetTicks() / 1000;
		if ( iSecond > m_iLastCheck )
		{
			m_iLastCheck = iSecond;
			m_iFps = m_iFrames;
			m_iFrames = 0;
			fprintf( stderr, "%d ", m_iFps);
		}
		++m_iFrames;
	}
	
} oFpsCounter;








/***************************************************************************
                     GAME PUBLIC METHODS
***************************************************************************/

Game::Game( bool a_bIsReplay, bool a_bDebug)
{
	m_bIsReplay = a_bIsReplay;
	m_bDebug = a_bDebug;
	
	m_poBackground = LoadBackground( "level1.png", 64 );
	m_poDoodads = LoadBackground( "Doodads.png", 48, 64 );
	
	m_aiRoundsWonByPlayer[0] = m_aiRoundsWonByPlayer[1] = 0;
	m_iNumberOfRounds = 0;
}


Game::~Game()
{
	SDL_FreeSurface( m_poBackground );
	m_poBackground = NULL;
	SDL_FreeSurface( m_poDoodads );
	m_poDoodads = NULL;
}


int Game::Run()
{
	do
	{
		m_sReplayString.clear();
		m_aReplayOffsets.clear();
		DoOneRound();
		
		if ( g_oState.m_bQuitFlag 
			|| SState::IN_DEMO == g_oState.m_enGameMode )
		{
			return -1;
		}
	} while ( m_aiRoundsWonByPlayer[0] < 2
		&& m_aiRoundsWonByPlayer[1] < 2
		&& m_iNumberOfRounds < 3 );
	
	if ( m_aiRoundsWonByPlayer[1] > m_aiRoundsWonByPlayer[0] ) return 1;
	if ( m_aiRoundsWonByPlayer[1] < m_aiRoundsWonByPlayer[0] ) return 0;
	return -1;
}


std::string& Game::GetReplay()
{
	return m_sReplayString;
}


/***************************************************************************
                     GAME DRAWING METHODS
***************************************************************************/


void Game::DrawHitPointDisplay()
{
	int hp1 = g_oBackend.m_aoPlayers[0].m_iHitPoints;// * 100 / g_oState.m_iHitPoints;
	int hp2 = g_oBackend.m_aoPlayers[1].m_iHitPoints;// * 100 / g_oState.m_iHitPoints;
	SDL_Rect src, dst;

	src.y = 154;
	src.h = 20;
	dst.y = 15;
	
	// Player 1, green part.
	dst.x = 40;
	src.x = 0;
	src.w = hp1*2;
	SDL_BlitSurface( m_poDoodads, &src, gamescreen, &dst );

	// Player 1, red part.
	dst.x += hp1*2;
	src.x = (100 + hp1)*2;
	src.w = (100-hp1)*2;
	SDL_BlitSurface( m_poDoodads, &src, gamescreen, &dst );

	// Player 2, red part.
	dst.x = 400;
	src.x = 200;
	src.w = (100-hp2)*2;
	SDL_BlitSurface( m_poDoodads, &src, gamescreen, &dst );
	
	// Player 2, green part.
	dst.x = 400 + (100-hp2)*2;
	src.x = (100-hp2)*2;
	src.w = hp2*2;
	SDL_BlitSurface( m_poDoodads, &src, gamescreen, &dst );
	
	// "Won" icon for Player 1
	src.x = 0; src.y = 276; src.w = 32; src.h = 32;
	if ( m_aiRoundsWonByPlayer[0] > 0 )
	{
		dst.x = 4; dst.y = 11;
		SDL_BlitSurface( m_poDoodads, &src, gamescreen, &dst );
	}
	if ( m_aiRoundsWonByPlayer[1] > 0 )
	{
		dst.x = 604; dst.y = 11;
		SDL_BlitSurface( m_poDoodads, &src, gamescreen, &dst );
	}
}



void Game::DrawBackground()
{
	sge_Blit( m_poBackground, gamescreen, 
		g_oBackend.m_iBgX, g_oBackend.m_iBgY,
		0, 0, SCREENWIDTH, SCREENHEIGHT );
}



void Game::DrawPoly( const char* sName, int color )
{
	AV *iList;
	int n;

	iList = get_av( sName, FALSE );
	if ( iList == NULL )
	{
		return;
	}

	n = av_len( iList ) + 1;

	if ( n< 2 )
	{
		return;
	}

	for ( int i=0; i<n; i += 2 )
	{
		int j = (i+2) % n;

		int x1 = SvIV( *av_fetch( iList, i, false) );
		int y1 = SvIV( *av_fetch( iList, i+1, false) );
		int x2 = SvIV( *av_fetch( iList, j, false) );
		int y2 = SvIV( *av_fetch( iList, j+1, false) );

		sge_Line( gamescreen, x1, y1, x2, y2, color ) ;
	}
}




void Game::DrawDoodads()
{
	for ( int i=0; i<g_oBackend.m_iNumDoodads; ++i )
	{
		Backend::SDoodad& roDoodad = g_oBackend.m_aoDoodads[i];
		if ( 0 == roDoodad.m_iType )
		{
			// Handle text doodads
			const char *s = roDoodad.m_sText.c_str();
			
			int iWidth = sge_BF_TextSize(fastFont, s).w;
			int iDoodadX = roDoodad.m_iX - iWidth/2;
			if ( iDoodadX + iWidth > 640 ) iDoodadX = 640 - iWidth;
			if ( iDoodadX < 0 ) iDoodadX = 0;
			int iDoodadY = roDoodad.m_iY;
			
			sge_BF_textout( gamescreen, fastFont, s, iDoodadX, iDoodadY );
			continue;
		}

		SDL_Rect rsrc, rdst;
		rdst.x = roDoodad.m_iX;
		rdst.y = roDoodad.m_iY;
		rsrc.x = 64 * roDoodad.m_iFrame;
		rsrc.y = 0;
		rsrc.w = 64;
		rsrc.h = 64;
		
		SDL_BlitSurface( m_poDoodads, &rsrc, gamescreen, &rdst );
		//debug( "Doodad x: %d, y: %d, t: %d, f: %d\n", dx, dy, dt, df );
	}
}


void Game::Draw()
{
	DrawBackground();

	for ( int i=0; i<2; ++i )
	{
		int iFrame = g_oBackend.m_aoPlayers[i].m_iFrame;
		if ( 0 != iFrame )
		{
			g_oPlayerSelect.GetPlayerInfo(i).m_poPack->Draw( 
				ABS(iFrame)-1,
				g_oBackend.m_aoPlayers[i].m_iX,
				g_oBackend.m_aoPlayers[i].m_iY,
				iFrame<0 );
		}
	}
	
	if ( m_bDebug )
	{
		DrawPoly( "p1head", C_LIGHTRED );
		DrawPoly( "p1body", C_LIGHTGREEN );
		DrawPoly( "p1legs", C_LIGHTBLUE );
		DrawPoly( "p1hit", C_YELLOW );
		DrawPoly( "p2head", C_LIGHTRED );
		DrawPoly( "p2body", C_LIGHTGREEN );
		DrawPoly( "p2legs", C_LIGHTBLUE );
		DrawPoly( "p2hit", C_YELLOW );
	}

	DrawDoodads();
	DrawHitPointDisplay();

	if ( Ph_NORMAL == m_enGamePhase )
	{
		char s[100];
		sprintf( s, "%d", g_oBackend.m_iGameTime );
		DrawTextMSZ( s, inkFont, 320, 10, AlignHCenter, C_LIGHTCYAN, gamescreen );
	}
	else if ( Ph_START == m_enGamePhase )
	{
		char s[100];
		sprintf( s, "Round %d", m_iNumberOfRounds+1 );
		DrawTextMSZ( s, inkFont, 320, 200, AlignHCenter, C_WHITE, gamescreen );
	}
	else if ( Ph_REWIND == m_enGamePhase )
	{
		DrawTextMSZ( "REW", inkFont, 320, 10, AlignHCenter, C_WHITE, gamescreen );
	}
	else if ( Ph_SLOWFORWARD == m_enGamePhase )
	{
		DrawTextMSZ( "REPLAY", inkFont, 320, 10, AlignHCenter, C_WHITE, gamescreen );
	}
	else if ( Ph_REPLAY == m_enGamePhase )
	{
		DrawTextMSZ( "DEMO", inkFont, 320, 10, AlignHCenter, C_WHITE, gamescreen );
	}
	
	if ( oFpsCounter.m_iFps > 0 )
	{
		sge_BF_textoutf( gamescreen, fastFont, 2, 2, "%d fps", oFpsCounter.m_iFps );
	}
	
	SDL_Flip( gamescreen );
}



/***************************************************************************
                     GAME PROTECTED METHODS
***************************************************************************/



/**
This method reads and updates the game's variables. In replay mode,
this is done by parsing the replay string. Otherwise the perl
backend is advanced the given number of steps.

Whichever the case, the variables will be available in g_oBackend.
*/

void Game::Advance( int a_iNumFrames )
{
	if ( m_bIsReplay )
	{
		// Replay mode...

		m_iFrame += a_iNumFrames;
		if ( m_iFrame >= ((int)m_aReplayOffsets.size())-1 ) m_iFrame = m_aReplayOffsets.size() - 2;		
		if ( m_iFrame <= 0 ) m_iFrame = 0;
		std::string sFrameDesc = m_sReplayString.substr(
			m_aReplayOffsets[m_iFrame],
			m_aReplayOffsets[m_iFrame+1] - m_aReplayOffsets[m_iFrame] );
		
		g_oBackend.ReadFromString( sFrameDesc );
		return;
	}
	
	while ( a_iNumFrames > 0 )
	{
		-- a_iNumFrames;
		std::string sFrameDesc;
		g_oBackend.AdvancePerl();
		g_oBackend.ReadFromPerl();
		g_oBackend.PlaySounds();

		g_oBackend.WriteToString( sFrameDesc );
		m_sReplayString += sFrameDesc;
		m_sReplayString += '\n';
		m_aReplayOffsets.push_back( m_sReplayString.size() );
	}
}



/** ProcessEvents reads events from the SDL event system. 

Relevant key events are fed to the backend.
Esc brings up the menu.

Returns 1 on quit event, 0 otherwise.
*/

int Game::ProcessEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				g_oState.m_bQuitFlag = true;
				return 1;
			
			case SDL_KEYDOWN:
			{
				if ( event.key.keysym.sym == SDLK_ESCAPE )
					return 1;
				
				if ( Ph_NORMAL != m_enGamePhase )
					break;
				
				for (int i=0; i<2; i++)
				{
					for (int j=0; j<9; j++ )
					{
						if (g_oState.m_aiPlayerKeys[i][j] == event.key.keysym.sym)
						{
							g_oBackend.PerlEvalF( "KeyDown(%d,%d);", i, j );
						}
					}
				}
				break;
			}
			
			case SDL_KEYUP:
			{
				if ( Ph_NORMAL != m_enGamePhase )
					break;

				for (int i=0; i<2; i++)
				{
					for (int j=0; j<9; j++ )
					{
						if (g_oState.m_aiPlayerKeys[i][j] == event.key.keysym.sym)
						{
							g_oBackend.PerlEvalF( "KeyUp(%d,%d);", i, j );
						}
					}
				}
				break;
			}
		} // End of switch
	} // End of while SDL_PollEvent;
	
	return 0;
}



void Game::HurryUp()
{
	Audio->PlaySample( "aroooga.voc" );
	DrawGradientText( "HURRY UP!", titleFont, 200, gamescreen );
	SDL_Delay( 1000 );
	Audio->PlaySample( "machine_start.voc" );
}


void Game::TimeUp()
{
	DrawGradientText( "TIME IS UP!", titleFont, 200, gamescreen );
	SDL_Delay( 1000 );
}


void Game::InstantReplay( int a_iKoAt )
{
	int iCurrentFrame = m_aReplayOffsets.size() - 200;
	int iThisTick, iLastTick, iGameSpeed;
	
	m_enGamePhase = Ph_REWIND;
	iGameSpeed = 8;
	iThisTick = SDL_GetTicks() / iGameSpeed;
	iLastTick = iThisTick - 1;
	
	while ( iCurrentFrame < (int)m_aReplayOffsets.size() - 150 )
	{
		// 1. Wait for the next tick (on extremely fast machines..)
		while (iThisTick == iLastTick)
		{
			iThisTick = SDL_GetTicks() / iGameSpeed;
			if ( iThisTick==iLastTick ) SDL_Delay(1);
		}
		
		// 2. Advance as many ticks as necessary..
		
		int iNumTicks = iThisTick - iLastTick;
		if ( iNumTicks > 10 ) iNumTicks = 10;
		if ( iNumTicks < 0 ) iNumTicks = 0;
		iCurrentFrame += ( Ph_REWIND == m_enGamePhase ) ? -iNumTicks : +iNumTicks	;
		
		if ( Ph_REWIND == m_enGamePhase
			&& ( iCurrentFrame < a_iKoAt - 200 || iCurrentFrame <= 0 )
		)
		{
			m_enGamePhase = Ph_SLOWFORWARD;
			iGameSpeed = 16;
			SDL_Delay(500);
		}
		iLastTick = iThisTick;
		if ( iCurrentFrame < 0 ) iCurrentFrame = 0;

		m_iFrame = iCurrentFrame;		
		if ( m_iFrame >= ((int)m_aReplayOffsets.size())-1 ) m_iFrame = m_aReplayOffsets.size() - 2;		
		if ( m_iFrame <= 0 ) m_iFrame = 0;
		std::string sFrameDesc = m_sReplayString.substr(
			m_aReplayOffsets[m_iFrame],
			m_aReplayOffsets[m_iFrame+1] - m_aReplayOffsets[m_iFrame] );
		
		//debug( "PB: Frame %d ofs %d-%d; data: '%s'\n", m_iFrame,
		//	m_aReplayOffsets[m_iFrame], m_aReplayOffsets[m_iFrame+1], sFrameDesc.c_str() );
		
		g_oBackend.ReadFromString( sFrameDesc );
		
		if ( ProcessEvents() )
		{
			break;
		}
		oFpsCounter.Tick();
		
		// 3. Draw the next game screen..
		
		Draw();

		if ( g_oState.m_bQuitFlag )
		{
			break;
		}
	}
	
}


void Game::DoOneRound()
{
	m_enGamePhase = Ph_START;
	
	g_oBackend.PerlEvalF( "GameStart(%d);", g_oState.m_iHitPoints );
	
	int iKoFrame = -1;
	double dGameTime = 2 * 1000;
	int iThisTick, iLastTick, iGameSpeed;
	bool bHurryUp = false;
	
	iGameSpeed = 12;
	iThisTick = SDL_GetTicks() / iGameSpeed;
	iLastTick = iThisTick - 1;
	
	oFpsCounter.Reset();

	// 1. DO THE NORMAL GAME ROUND and HURRYUP
	
	while ( dGameTime > 0 )
	{
		
		// 1. Wait for the next tick (on extremely fast machines..)
		while (iThisTick == iLastTick)
		{
			iThisTick = SDL_GetTicks() / iGameSpeed;
			if ( iThisTick==iLastTick ) SDL_Delay(1);
		}
		
		// 2. Advance as many ticks as necessary..

		int iNumTicks = iThisTick - iLastTick;
		if ( iNumTicks > MAXFRAMESKIP ) iNumTicks = MAXFRAMESKIP;		
		Advance( iNumTicks );
		dGameTime -= iNumTicks * iGameSpeed;
		
		if ( Ph_START == m_enGamePhase )
		{
			if ( dGameTime < 0 )
			{
				m_enGamePhase = Ph_NORMAL;
				dGameTime = g_oState.m_iGameTime * 1000;
			}
		}
		else if ( Ph_NORMAL == m_enGamePhase )
		{
			if ( dGameTime < 10 * 1000 
				&& !bHurryUp )
			{
				bHurryUp = true;
				HurryUp();
				iGameSpeed = iGameSpeed * 3 / 4;
			}
			if ( g_oBackend.m_bKO )
			{
				m_enGamePhase = Ph_KO;
				dGameTime = 10 * 1000;
				iKoFrame = m_aReplayOffsets.size();
			}
			else if ( dGameTime < 0 )
			{
				m_enGamePhase = Ph_TIMEUP;
				TimeUp();
				break;
			}
		}
		
		g_oBackend.m_iGameTime = (int) ((dGameTime + 500.0) / 1000.0);
		iLastTick = iThisTick;
		
		if ( ProcessEvents() )
		{
			break;
		}
		
		oFpsCounter.Tick();
		
		// 3. Draw the next game screen..
		
		Draw();

		if ( g_oBackend.m_iGameOver || g_oState.m_bQuitFlag )
		{
			break;
		}
	}
	
	// 3. DO THE REPLAY (IF THERE WAS A KO)
	
	if ( iKoFrame>0 )
	{
		InstantReplay( iKoFrame );
	}
	
	// 4. END OF ROUND
	
	
	int p1h = g_oBackend.m_aoPlayers[0].m_iHitPoints;
	int p2h = g_oBackend.m_aoPlayers[1].m_iHitPoints;
	debug( "Game over; p1h = %d; p2h = %d\n", p1h, p2h );
	
	if ( p1h > p2h ) ++m_aiRoundsWonByPlayer[0];
	if ( p2h > p1h ) ++m_aiRoundsWonByPlayer[1];
	++m_iNumberOfRounds;
}



void Game::DoReplay( const char* a_pcReplayFile )
{
	std::ifstream oInput( a_pcReplayFile );
	int iPlayer1, iPlayer2;
	oInput >> iPlayer1 >> iPlayer2;
	
	std::string sLine;
	std::getline( oInput, sLine );
	g_oPlayerSelect.SetPlayer( 0, (FighterEnum) iPlayer1 );
	g_oPlayerSelect.SetPlayer( 1, (FighterEnum) iPlayer2 );
	
	int iThisTick, iLastTick, iGameSpeed;
	
	m_enGamePhase = Ph_REPLAY;
	iGameSpeed = 12;
	iThisTick = SDL_GetTicks() / iGameSpeed;
	iLastTick = iThisTick - 1;
	
	while ( !oInput.eof() )
	{
		// 1. Wait for the next tick (on extremely fast machines..)
		while (iThisTick == iLastTick)
		{
			iThisTick = SDL_GetTicks() / iGameSpeed;
			if ( iThisTick==iLastTick ) SDL_Delay(1);
		}
		
		// 2. Advance as many ticks as necessary..
		
		int iNumTicks = iThisTick - iLastTick;
		if ( iNumTicks > 5 ) iNumTicks = 5;
		
		for ( int i=0; i< iNumTicks; ++i )
		{
			std::getline( oInput, sLine );
		}
		if ( 0 == sLine.size() )
		{
			break;
		}
		
		iLastTick = iThisTick;
		
		g_oBackend.ReadFromString( sLine );
		
		if ( ProcessEvents() )
		{
			break;
		}
		oFpsCounter.Tick();
		
		// 3. Draw the next game screen..
		
		Draw();

		if ( g_oState.m_bQuitFlag )
		{
			break;
		}
	}
}



int DoGame( char* a_pcReplayFile, bool a_bIsReplay, bool a_bDebug )
{
	Game oGame( a_bIsReplay, a_bDebug );
	
	if ( a_bIsReplay )
	{
		if ( NULL == a_pcReplayFile )
		{
			return 0;
		}
		oGame.DoReplay( a_pcReplayFile );
		return 0;
	}
	else
	{
		int iRetval = oGame.Run();
		if ( NULL != a_pcReplayFile )
		{
			std::ofstream oOutput( a_pcReplayFile );
			oOutput << 
				g_oPlayerSelect.GetPlayerInfo(0).m_enFighter << ' ' << 
				g_oPlayerSelect.GetPlayerInfo(1).m_enFighter << '\n' << oGame.GetReplay();
		}
		return iRetval;
	}
}


