/***************************************************************************
                         Demo.cpp  -  description
                             -------------------
    begin                : Wed Aug 16 22:18:47 CEST 2003
    copyright            : (C) 2003 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/
 

#include "FlyingChars.h"
#include "SDL.h"


#include "State.h"
#include "gfx.h"
#include "common.h"
#include "Backend.h"
#include "RlePack.h"
#include "FighterStats.h"	// #includes Demo.h
#include "config.h"





Demo::Demo()
{
	m_poFlyingChars = NULL;
	m_bAdvanceGame = false;
}


Demo::~Demo()
{
	delete m_poFlyingChars;
	m_poFlyingChars = NULL;
}


int Demo::Advance( int a_iNumFrames, bool a_bFlip )
{
	int iRetVal = 1;
	
	if ( a_iNumFrames > 5 )
		a_iNumFrames = 5;
		
	if ( m_poBackground )
	{
		SDL_BlitSurface( m_poBackground, NULL, gamescreen, NULL );
	}
	
	if ( m_poFlyingChars )
	{
		iRetVal &= AdvanceFlyingChars( a_iNumFrames );
		m_poFlyingChars->Draw();
	}
	if ( m_bAdvanceGame )
	{
		iRetVal &= AdvanceGame( a_iNumFrames );
		//@ DRAW GAME?
	}
	
	if ( a_bFlip )
	{
		SDL_Flip( gamescreen );
	}
	
	return iRetVal;
}


int Demo::AdvanceFlyingChars( int a_iNumFrames )
{
	m_poFlyingChars->Advance( a_iNumFrames );
	return ( m_poFlyingChars->IsDone() ? 1 : 0 );
}




int Demo::AdvanceGame( int a_iNumFrames )
{
	for ( int i=0; i<a_iNumFrames; ++i )
	{
		g_oBackend.AdvancePerl();
	}
	return 0;
}


int Demo::Run()
{
	SState::TGameMode enOriginalGameMode = g_oState.m_enGameMode;

	int thisTick, lastTick, firstTick, gameSpeed;
	SDL_Event event;
	gameSpeed = 12;

	if ( m_poBackground )
	{
		DrawTextMSZ( "Press Escape for the menu", impactFont, 10, 450, UseShadow, C_WHITE, m_poBackground );
	}
	
	thisTick = SDL_GetTicks() / gameSpeed;
	lastTick = thisTick - 1;
	firstTick = SDL_GetTicks() / gameSpeed;
	while ( 1 )
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
		
		// 2. Call ADVANCE.
		
		int iRetVal = Advance(thisTick-lastTick, true);
		lastTick = thisTick;
		
		if ( iRetVal )
		{
			return 0;
		}
		
		// 3. Handle events.

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					g_oState.m_bQuitFlag = true;
					break;
				case SDL_KEYDOWN:
					if ( event.key.keysym.sym == SDLK_ESCAPE )
					{
						OnMenu();
						break;
					}
					if ( event.key.keysym.sym == SDLK_F1 )
					{
						return 0;
					}
					for ( int i=0; i<2; ++i )
					{
						for ( int j=0; j<9; ++j )
						{
							if ( g_oState.m_aiPlayerKeys[i][j] == event.key.keysym.sym)
							{
								g_oState.m_enGameMode = SState::IN_MULTI;
							}
						}
					}
					break;
			} // switch
		} // while SDL_PollEvent

		if ( g_oState.m_enGameMode != enOriginalGameMode
			|| g_oState.m_bQuitFlag )
		{
			return 1;
		}		
		
	} // while 1;
}




void Demo::OnMenu()
{
	::DoMenu();
}




class CreditsDemo: public Demo
{
public:
	CreditsDemo()
	{
		m_poBackground = LoadBackground( "Credits.png", 240 );
		SDL_UnlockSurface( m_poBackground );
		DrawGradientText( "Credits", titleFont, 20, m_poBackground );
		SDL_Flip( m_poBackground );
		
		SDL_Rect oRect;
		oRect.x = 110; oRect.w = gamescreen->w - 220;
		oRect.y = 100; oRect.h = 350;
		
		m_poFlyingChars = new FlyingChars( creditsFont, oRect );
		
		m_sText1 = Translate( "CreditsText1" );
		m_sText2 = Translate( "CreditsText2" );
		m_sText3 = Translate( "CreditsText3" );
		
		m_poFlyingChars->AddText( m_sText1.c_str(), FlyingChars::FC_AlignCenter, true );
		m_poFlyingChars->AddText( m_sText2.c_str(), FlyingChars::FC_AlignJustify, true );
		m_poFlyingChars->AddText( m_sText3.c_str(), FlyingChars::FC_AlignCenter, true );
		
		m_poFlyingChars->AddText( "\n\n\n\n\n\n:)", FlyingChars::FC_AlignRight, true );
	}
protected:
	std::string m_sText1;
	std::string m_sText2;
	std::string m_sText3;
};



class Story1Demo: public Demo
{
public:
	Story1Demo()
	{
		m_poBackground = LoadBackground( "Story1.png", 240 );
		SDL_UnlockSurface( m_poBackground );
		
		SDL_Rect oRect;
		oRect.x = 50; oRect.w = gamescreen->w - 100;
		oRect.y = 50; oRect.h = gamescreen->h - 100;
		
		m_poFlyingChars = new FlyingChars( storyFont, oRect, -1 );
		m_sText1 = Translate( "Story1Text" );
		m_poFlyingChars->AddText( m_sText1.c_str(), FlyingChars::FC_AlignJustify, true );
	}
protected:
	std::string m_sText1;
};


class Story2Demo: public Demo
{
public:
	Story2Demo()
	{
		m_poBackground = LoadBackground( "Story2.png", 240 );
		SDL_UnlockSurface( m_poBackground );

		SDL_Rect oRect;
		oRect.x = 50; oRect.w = gamescreen->w - 100;
		oRect.y = 50; oRect.h = gamescreen->h - 100;

		m_poFlyingChars = new FlyingChars( storyFont, oRect, -1 );
		m_sText1 = Translate( "Story2Text" );
		m_poFlyingChars->AddText( m_sText1.c_str(), FlyingChars::FC_AlignJustify, true );
	}
protected:
	std::string m_sText1;
};



class MainScreenDemo: public Demo
{
public:
	MainScreenDemo()
	{
		i = 0;
		m_iTimeLeft = 50;
		m_poBackground = LoadBackground( "Mortal.png", 240 );
		DrawTextMSZ( VERSION, inkFont, 540, 430, UseShadow | AlignHCenter, C_WHITE, m_poBackground, false );
		std::string sStaffFilename = DATADIR;
		sStaffFilename += "/characters/STAFF.DAT";
		m_poPack = new RlePack( sStaffFilename.c_str(), 240 );
		m_poPack->ApplyPalette();
		SDL_BlitSurface( m_poBackground, NULL, gamescreen, NULL );
		SDL_Flip( gamescreen );

		int j, k, l;
		for ( j=0; j<14; ++j )
		{
			m_aiOrder[j] = j;
			m_bShown[j] = false;
		}
		for ( j=0; j<14; ++j )
		{
			k = rand() % 14;
			l = m_aiOrder[j]; m_aiOrder[j] = m_aiOrder[k]; m_aiOrder[k] = l;
		}
	}

	~MainScreenDemo()
	{
		delete m_poPack;
		m_poPack = NULL;
	}
	
	int Advance( int a_iNumFrames, bool a_bFlip )
	{
		static int x[14] = {
			0, 26, 67, 125, 159, 209,
			249, 289, 358, 397, 451, 489, 532, 161 };
		static int y[14] = {
			5, 4, 5, 5, 5, 7,
			4, 0, 7, 5, 5, 6, 5, 243 };
		
		m_iTimeLeft -= a_iNumFrames;

		if ( m_iTimeLeft <= 0
			&& i >= 14 )
		{
			return 1;
		}

		if ( m_iTimeLeft <= 0 )
		{
			m_bShown[ m_aiOrder[i] ] = true;
			for ( int j=0; j<=14; ++j )
			{
				if ( m_bShown[j] )
				{
					m_poPack->Draw( j, x[j], y[j], false );
				}
			}
			SDL_Flip( gamescreen );
			++i;
			m_iTimeLeft += 20;
			if ( i >= 14 )
			{
				m_iTimeLeft += 50;
			}
		}
		
		return 0;
	}
	
protected:
	RlePack* m_poPack;
	int m_iTimeLeft;
	int i;
	int m_aiOrder[14];
	bool m_bShown[14];
};




void DoReplayDemo()
{
	static int aiOrder[6] = {-1, -1, -1, -1, -1, -1};
	static int iNext = 0;
	
	if ( aiOrder[0]<0 )
	{
		// shuffle
		int i, j, k;
		for ( i=0; i<6; ++i ) aiOrder[i]=i;
		for ( i=0; i<6; ++i )
		{
			j = rand() % 6;
			k = aiOrder[i];
			aiOrder[i] = aiOrder[j];
			aiOrder[j] = k;
		}
		iNext = 0;
	}
	
	char acFilename[1024];
	sprintf( acFilename, DATADIR "/demo%d.om", aiOrder[iNext] );
//	DoGame( acFilename, true, false );
	
	iNext = ( iNext + 1 ) % 6;
}



static bool g_bFirstTime = true;


void DoDemos()
{
	#define DoDemos_BREAKONEND \
		if ( g_oState.m_enGameMode != SState::IN_DEMO \
				|| g_oState.m_bQuitFlag ) \
			return;

	if ( g_bFirstTime )
	{
		g_bFirstTime = false;
	}
	else
	{
		MainScreenDemo oDemo;
		oDemo.Run();
	}
	
	while (1)
	{
		DoDemos_BREAKONEND;
		DoReplayDemo();
		DoDemos_BREAKONEND;
		{
			Story1Demo oDemo;
			oDemo.Run();
		}
		DoDemos_BREAKONEND;
		DoReplayDemo();
		DoDemos_BREAKONEND;
		{
			FighterStatsDemo oDemo;
			oDemo.Run();
		}
		DoDemos_BREAKONEND;
		DoReplayDemo();
		DoDemos_BREAKONEND;
		{
			Story2Demo oDemo;
			oDemo.Run();
		}
		DoDemos_BREAKONEND;
		{
			FighterStatsDemo oDemo;
			oDemo.Run();
		}
		DoDemos_BREAKONEND;
		DoReplayDemo();
		DoDemos_BREAKONEND;
		{
			CreditsDemo oDemo;
			oDemo.Run();
		}
		DoDemos_BREAKONEND;
		{
			MainScreenDemo oDemo;
			oDemo.Run();
		}
		DoDemos_BREAKONEND;
		
	}
}
