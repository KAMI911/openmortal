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
#include "Demo.h"






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


void DoMenu( bool a_bDrawBackground );


void Demo::OnMenu()
{
	::DoMenu( true );
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
		m_poFlyingChars->AddText( "OPENMORTAL CREDITS\n\n\n"
			"-- THE OPENMORTAL TEAM ARE --\n\n"
		  		"CODING - UPi\n"
				"MUSIC - Oedipus\n"
				"GRAPHICS - UPi\n"
			"\n-- CAST --\n\n"
				"Boxer - Zoli\n"
				"Cumi - As himself\n"
				"Descant - As himself\n"
				"Fureszes Orult - Ambrus\n"
				"Grizli - As himself\n"
				"Kinga - As herself\n"
				"Macy - As herself\n"
				"Misi - As himself\n"
				"Rising-san - Surba\n"
				"Sirpi - As himself\n"
				"Taka Ito - Bence\n"
				"Tokeletlen Katona - Dani\n"
				"Watasiwa Baka Janajo - Ulmar\n"
				"Black Dark Evil Mage - UPi\n"
			"\n-- HOSTING --\n\n"
				"sourceforge.net\n"
				"apocalypse.rulez.org\n"
				"freshmeat.net\n"
			"\nOpenMortal is Copyright 2003 of the OpenMortal Team\n"
			"Distributed under the GNU General Public Licence Version 2\n\n",
			FlyingChars::FC_AlignCenter, true );
		m_poFlyingChars->AddText( "Thanks to Midway for not harrassing us with legal stuff so far, "
			"even though we must surely violate at least 50 of their patents, international "
			"copyrights and registered trademarks.\n\n"
			"OpenMortal needs your help! If you can contribute music, graphics, improved code, "
			"additional characters, cash, beer, pizza or any other consumable, please mail us "
			"at upi@apocalypse.rulez.org! The same address is currently accepting comments and "
			"fanmail too (hint, hint!).\n\n",
			FlyingChars::FC_AlignJustify, true );
		m_poFlyingChars->AddText( "Be sure to check out other stuff from\n"
			"Apocalypse Production\n"
			"and\n"
			"Degec Entertainment\n\n",
			FlyingChars::FC_AlignCenter, true );
		
		m_poFlyingChars->AddText( "\n\n\n\n\n\n:)", FlyingChars::FC_AlignRight, true );
	}
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
		m_poFlyingChars->AddText( 
			"We, the Gods of the Evil Killer Black Antipathic Dim (witted) Fire Mages "
			"no longer tolerate the lack of evildoing.\n\n"
			"We send them out on a "
			"mission so diabolical, so evil that the world will never be the same "
			"again!\n\n"
			"We order our unworthy followers to "
			"\nDESTROY THE SATURDAY\n"
			"and plunge humanity into a dark age of 5 working days and 1 holiday "
			"per week... FOREVER!\n\n\n\n\n\n\n\n\n",
			FlyingChars::FC_AlignJustify, true );
	}
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
		m_poFlyingChars->AddText(
"Whenever EVIL looms on the horizon, the good guys are there to "
"save the day. Son Goku, the protector of Earth and Humanity "
"went to the rescue...\n\n"

"Only to become ROADKILL on his way to the Mortal Szombat "
"tournament! It was Cumi's first time behind the wheel, after all...\n\n\n\n\n\n\n\n\n",
			FlyingChars::FC_AlignJustify, true );
	}
};



class MainScreenDemo: public Demo
{
public:
	MainScreenDemo()
	{
		m_iTimeLeft = 100;
		
		m_poBackground = LoadBackground( "Mortal.png", 240 );
	}
	
	int Advance( int a_iNumFrames, bool a_bFlip )
	{
		m_iTimeLeft -= a_iNumFrames;
		Demo::Advance( a_iNumFrames, true );
		return ( m_iTimeLeft <= 0 ) ? 1 : 0;
	}
	
protected:
	int m_iTimeLeft;
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



void DoDemos()
{
	#define DoDemos_BREAKONEND \
		if ( g_oState.m_enGameMode != SState::IN_DEMO \
				|| g_oState.m_bQuitFlag ) \
			return;
	
	while (1)
	{
		DoDemos_BREAKONEND;
		{
			MainScreenDemo oDemo;
			oDemo.Run();
		}
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
		
	}
}
