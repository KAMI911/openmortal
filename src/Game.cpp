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
#include "Background.h"
#include "common.h"
#include "gfx.h"
#include "Backend.h"
#include "RlePack.h"
#include "State.h"
#include "Game.h"
#include "Audio.h"
#include "MortalNetwork.h"


#include "MszPerl.h"


extern PerlInterpreter*	my_perl;


int Game::mg_iBackgroundNumber = 1;

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
	m_enInitialGameMode = g_oState.m_enGameMode;
	
	m_poBackground = new Background();
	m_poBackground->Load(mg_iBackgroundNumber++);
	if ( !m_poBackground->IsOK() )
	{
		m_poBackground->Load(1);
		mg_iBackgroundNumber = 1;
	}
	
	m_poDoodads = LoadBackground( "Doodads.png", 48, 64 );
	
	m_aiRoundsWonByPlayer[0] = m_aiRoundsWonByPlayer[1] = 0;
	m_iNumberOfRounds = 0;
}


Game::~Game()
{
	delete m_poBackground;
	m_poBackground = NULL;
	SDL_FreeSurface( m_poDoodads );
	m_poDoodads = NULL;
}


/** Runs a whole game, with two or three rounds.

\retval 0 if player 1 has won.
\retval 1 if player 2 has won.
\retval -1 if the game was a draw.
*/
int Game::Run()
{
	do
	{
		m_sReplayString = "";
		m_aReplayOffsets.clear();
		DoOneRound();
		
		if ( g_oState.m_bQuitFlag 
			|| m_enInitialGameMode != g_oState.m_enGameMode )
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


/** Returns the replay string of the last round.
*/
std::string& Game::GetReplay()
{
	return m_sReplayString;
}


/***************************************************************************
                     GAME DRAWING METHODS
***************************************************************************/


/** Draws the hitpoint bars that are displayed on the top of the screen.
Also draws the fighter names below the bars.

Input variables:
\li g_oBackend.m_aoPlayers[x].m_iHitPoints
\li g_oPlayerSelect.GetFighterName
*/
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
	
	sge_BF_textout( gamescreen, fastFont, g_oPlayerSelect.GetFighterName(0),
		230 - g_oPlayerSelect.GetFighterNameWidth(0), 38 );
	sge_BF_textout( gamescreen, fastFont, g_oPlayerSelect.GetFighterName(1),
		410, 38 );
}



/** Draws the background, using the m_poBackground object.
*/
void Game::DrawBackground()
{
	m_poBackground->Draw( g_oBackend.m_iBgX, g_oBackend.m_iBgY );
}


/** In debug mode, this method is used to draw the frame of the fighters.

\param a_sName The name of the polygon (in the perl namespace)
\param a_iColor The game color to draw the polygon with.
*/
void Game::DrawPoly( const char* a_pcName, int a_iColor )
{
	AV *poList;
	int n;

	poList = get_av( a_pcName, FALSE );
	if ( poList == NULL )
	{
		return;
	}

	n = av_len( poList ) + 1;

	if ( n< 2 )
	{
		return;
	}

	for ( int i=0; i<n; i += 2 )
	{
		int j = (i+2) % n;

		int x1 = SvIV( *av_fetch( poList, i, false) );
		int y1 = SvIV( *av_fetch( poList, i+1, false) );
		int x2 = SvIV( *av_fetch( poList, j, false) );
		int y2 = SvIV( *av_fetch( poList, j+1, false) );

		sge_Line( gamescreen, x1, y1, x2, y2, a_iColor ) ;
	}
}




/** Draws every doodad that is currently defined in the backend.
*/
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
		
		if ( roDoodad.m_iGfxOwner < 2 )
		{
			g_oPlayerSelect.GetPlayerInfo(roDoodad.m_iGfxOwner).m_poPack->Draw( 
				roDoodad.m_iFrame, roDoodad.m_iX, roDoodad.m_iY, roDoodad.m_iDir < 1 );
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


/** Draws the entire game screen:

\li First, the background.
\li The players.
\li The debug wireframes (if debugging is turned on)
\li The doodads.
\li The hitpoint display.
\li The gametime display.
\li The FPS display.
\li The "Round x" text during Ph_Start

Input:
\li m_enGamePhase
\li g_oBackend.m_iGameTime
\li m_iNumberOfRounds
\li oFpsCounter
*/
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
		sprintf( s, "%d", m_iGameTime );	// m_iGameTime is maintained by DoGame
		DrawTextMSZ( s, inkFont, 320, 10, AlignHCenter, C_LIGHTCYAN, gamescreen, false );
	}
	else if ( Ph_START == m_enGamePhase )
	{
		char s[100];
		const char* format = Translate( "Round %d" );
		sprintf( s, format, m_iNumberOfRounds+1 );
		DrawTextMSZ( s, inkFont, 320, 200, AlignHCenter, C_WHITE, gamescreen, false );
	}
	else if ( Ph_REWIND == m_enGamePhase )
	{
		DrawTextMSZ( "REW", inkFont, 320, 10, AlignHCenter, C_WHITE, gamescreen );
		sge_BF_textout( gamescreen, fastFont, Translate("Press F1 to skip..."), 230, 450 );
	}
	else if ( Ph_SLOWFORWARD == m_enGamePhase )
	{
		DrawTextMSZ( "REPLAY", inkFont, 320, 10, AlignHCenter, C_WHITE, gamescreen );
		sge_BF_textout( gamescreen, fastFont, Translate("Press F1 to skip..."), 230, 450 );
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



bool Game::IsNetworkGame()
{
	return SState::IN_NETWORK == g_oState.m_enGameMode;
}


/** Returns true if we control our own data, or false if the network supplies
us with game data. */
bool Game::IsMaster()
{
	return !IsNetworkGame() || g_poNetwork->IsMaster();
}


/**
This method reads and updates the game's variables. In replay mode,
this is done by parsing the replay string. Otherwise the perl
backend is advanced the given number of steps.

Whichever the case, the variables will be available in g_oBackend.

Only the backend-driven variables are modified, the GamePhase and
GameTime remain unchanged; these are up for DoOneRound and friends
to modify.
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

	if ( IsNetworkGame() )
	{
		bool bMaster = g_poNetwork->IsMaster();
		g_poNetwork->Update();
		if (!bMaster)
		{
			// We don't run our own backend, just pull the data from the network.
			const char* pcRemoteBackend = g_poNetwork->GetLatestGameData();
			g_oBackend.ReadFromString( pcRemoteBackend );
			return;
		}
	}
	
	static std::string sFrameDesc;
	
	while ( a_iNumFrames > 0 )
	{
		-- a_iNumFrames;
		g_oBackend.AdvancePerl();
		g_oBackend.ReadFromPerl();
		g_oBackend.PlaySounds();

		g_oBackend.WriteToString( sFrameDesc );
		m_sReplayString += sFrameDesc;
		m_sReplayString += '\n';
		m_aReplayOffsets.push_back( m_sReplayString.size() );
	}

	if ( IsNetworkGame() && sFrameDesc.size() )
	{
		g_poNetwork->SendGameData( sFrameDesc.c_str() );
		g_oBackend.PlaySounds();
	}

}



/** A helper method of ProcessEvents; it manages keypresses and releases of
players.
*/

void Game::HandleKey( int a_iPlayer, int a_iKey, bool a_bDown )
{
	if ( IsMaster() )
	{
		if ( IsNetworkGame() )
		{
			a_iPlayer = 0;
		}
		g_oBackend.PerlEvalF( a_bDown ? "KeyDown(%d,%d);" : "KeyUp(%d,%d);", a_iPlayer, a_iKey );
	}
	else
	{
		g_poNetwork->SendKeystroke( a_iKey, a_bDown );
	}
}


/** ProcessEvents reads events from the SDL event system. 

Relevant key events are fed to the backend.
Esc brings up the menu.

Returns 1 on quit event (e.g. if the current game or replay should be aborted), 0 otherwise.
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
				if ( event.key.keysym.sym == SDLK_ESCAPE /*&& !IsNetworkGame()*/ )
				{
					SState::TGameMode enMode = g_oState.m_enGameMode;
					::DoMenu( true );
					return g_oState.m_enGameMode == enMode ? 0 : 1;
				}
				if ( event.key.keysym.sym == SDLK_F1  /*&& !IsNetworkGame()*/ )
				{
					// shortcut: abort the current round. This shall is here
					// to ease testing, and will be removed from the final
					// version.
					return 1;
				}
				
				if ( Ph_NORMAL != m_enGamePhase &&
					Ph_REPLAY != m_enGamePhase )
					break;
				
				for (int i=0; i<2; i++)
				{
					for (int j=0; j<9; j++ )
					{
						if (g_oState.m_aiPlayerKeys[i][j] == event.key.keysym.sym)
						{
							if (g_oState.m_enGameMode == SState::IN_DEMO)
							{
								g_oState.m_enGameMode = SState::IN_MULTI;
								return 1;
							}
							HandleKey( i, j, true );
							//g_oBackend.PerlEvalF( "KeyDown(%d,%d);", i, j );
							return 0;
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
							HandleKey( i, j, false );
							g_oBackend.PerlEvalF( "KeyUp(%d,%d);", i, j );
							return 0;
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


/** This methods starts and runs the "instant replay" mode that is done
at the end of a game round. This means doing phases Ph_REWIND and Ph_SLOWFORWARD.

Rewind will go back in time 200 ticks before the parameter a_iKoAt.
*/
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

		if ( g_oState.m_bQuitFlag 
			|| SState::IN_DEMO == g_oState.m_enGameMode )
		{
			break;
		}
	}
	
}


/** This methods executes one round of gameplay.

The game progresses through phases Ph_START, Ph_NORMAL, and
Ph_KO. If a KO happened, it will invoke InstantReplay. At the end of
the round m_aiRoundsWonByPlayer[x] will be incremented depending on the
outcome. m_iNumberOfRounds will also increase by 1.
*/
void Game::DoOneRound()
{
	m_enGamePhase = Ph_START;
	
	g_oBackend.PerlEvalF( "GameStart(%d,%d);", g_oState.m_iHitPoints, m_bDebug );

	if ( IsNetworkGame() )
	{
		g_poNetwork->SynchStartRound();
	}

	int iKoFrame = -1;
	double dGameTime = 2 * 1000;	// Only for the "greeting phase", the real gametime will be set after.
	int iThisTick, iLastTick, iGameSpeed;
	bool bHurryUp = false;
	bool bReplayAfter = true;
	
	iGameSpeed = g_oState.m_iGameSpeed;
	iThisTick = SDL_GetTicks() / iGameSpeed;
	iLastTick = iThisTick - 1;
	
	oFpsCounter.Reset();
	
	// 1. DO THE NORMAL GAME ROUND (START, NORMAL, KO, TIMEUP)
	
	while ( dGameTime >= 0 )
	{
		if ( m_enInitialGameMode != g_oState.m_enGameMode )
		{
			return;
		}
		
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

		// 3. Check for state transitions and game time.
		// START -> NORMAL
		// NORMAL -> KO
		// NORMAL -> TIMEUP
		// bHurryUp flag can be set during NORMAL phase

		if ( !IsNetworkGame()
			|| (IsNetworkGame() && g_poNetwork->IsMaster()) )
		{
			if ( Ph_START == m_enGamePhase )		// Check for the end of the START phase
			{
				if ( dGameTime <= 0 )
				{
					m_enGamePhase = Ph_NORMAL;
					dGameTime = g_oState.m_iGameTime * 1000;
				}
			}
			else if ( Ph_NORMAL == m_enGamePhase )	// Check for the end of the NORMAL phase
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
				else if ( dGameTime <= 0 )
				{
					m_enGamePhase = Ph_TIMEUP;
					TimeUp();
					break;
				}
			}
			
			m_iGameTime = (int) ((dGameTime + 500.0) / 1000.0);
			
			if ( IsNetworkGame() )
			{
				g_poNetwork->SendGameTime( m_iGameTime, m_enGamePhase );
			}
		}
		else
		{
			m_iGameTime = g_poNetwork->GetGameTime();
			TGamePhaseEnum enOldPhase = m_enGamePhase;
			m_enGamePhase = (TGamePhaseEnum) g_poNetwork->GetGamePhase();
			dGameTime = 1000.0;	// ignored.
		}
		
		iLastTick = iThisTick;
		
		if ( ProcessEvents() || g_oState.m_bQuitFlag )
		{
			bReplayAfter = false;
			break;
		}

		if ( IsNetworkGame() && IsMaster() )
		{
			int iKey;
			bool bPressed;
			while ( g_poNetwork->GetKeystroke( iKey, bPressed ) )
			{
				debug( "Got GetKeystroke: %d, %d\n", iKey, bPressed );
				g_oBackend.PerlEvalF( bPressed ? "KeyDown(%d,%d);" : "KeyUp(%d,%d);", 1, iKey );
			}
		}
		
		oFpsCounter.Tick();
		
		// 3. Draw the next game screen..
		
		Draw();

		// 4. Check 'end of round' condition.

		if ( !IsMaster() )
		{
			if ( g_poNetwork->IsRoundOver() )
			{
				break;
			}
		}
		else if ( g_oBackend.m_iGameOver )
		{
			break;
		}
	}
	
	int p1h = g_oBackend.m_aoPlayers[0].m_iHitPoints;
	int p2h = g_oBackend.m_aoPlayers[1].m_iHitPoints;
	
	// 3. DO THE REPLAY (IF THERE WAS A KO)
	
	if ( iKoFrame>0 && bReplayAfter && !IsNetworkGame() )
	{
		InstantReplay( iKoFrame );
	}
	
	// 4. END OF ROUND
	
	debug( "Game over; p1h = %d; p2h = %d\n", p1h, p2h );
	
	if ( IsMaster() )
	{
		int iWhoWon = -1;
		if ( p1h > p2h ) { ++m_aiRoundsWonByPlayer[0]; iWhoWon = 0; }
		if ( p2h > p1h ) { ++m_aiRoundsWonByPlayer[1]; iWhoWon = 1; }
		
		if ( IsNetworkGame() )
		{
			g_poNetwork->SendRoundOver( iWhoWon, m_iNumberOfRounds > 0 );
		}
	}
	else
	{
		int iWhoWon = g_poNetwork->GetWhoWon();
		if ( iWhoWon>=0 )
		{
			++m_aiRoundsWonByPlayer[iWhoWon];
		}
	}
	
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






/** Public static function.

Other parts of OpenMortal need not include "Game.h" so long as they have
the definition of this method (defined in "common.h"). The method runs
a cycle of the game (either a normal game, or replay).

In replay mode, DoReplay() is called, and the replay file is required.
In normal mode, Run() is called. The replay file is recorded, if it is not NULL.
*/

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


