/***************************************************************************
                         GameOver.cpp  -  description
                             -------------------
    begin                : Wed Aug 21 20:25:30 CEST 2003
    copyright            : (C) 2003 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include "SDL.h"
#include "gfx.h"
#include "Backend.h"
#include "common.h"
#include "State.h"
#include "RlePack.h"
#include "Audio.h"

#include <stdio.h>


void DoMenu( bool );


void GameOver( int a_iPlayerWon )
{
	SDL_Surface* poBackground = LoadBackground( "GameOver.png", 112 );
	DrawGradientText( "Final Judgement", titleFont, 20, poBackground );
	DrawTextMSZ( "Continue?", inkFont, 320, 100, AlignHCenter, C_LIGHTCYAN, poBackground );
	
	SDL_Surface* poFoot = LoadBackground( "Foot.png", 112 );
	
	SDL_BlitSurface( poBackground, NULL, gamescreen, NULL );
	
	char acString[100];
	sprintf( acString, "JudgementStart(%d);", a_iPlayerWon );
	PERLEVAL( acString );
	
	int thisTick, lastTick, firstTick, iGameSpeed;
	iGameSpeed = 14 ;
	firstTick = thisTick = SDL_GetTicks() / iGameSpeed;
	lastTick = thisTick - 1;
	
	int iTimeLeft = 8000 / iGameSpeed;
	int FOOTHEIGHT = poFoot->h;
	int GROUNDLEVEL = 440;
	int iFootY = -FOOTHEIGHT;
	bool bTimeUp = false;
	bool bKeyPressed = false;
	int iCounter = -1;
	
	SDL_Event event;

	while (1)
	{
		// 1. Wait for the next tick (on extremely fast machines..)
		while (1)
		{
			thisTick = SDL_GetTicks() / iGameSpeed;
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
		
		if ( thisTick - lastTick > 5 )
		{
			iTimeLeft -= 5;
		}
		else
		{
			iTimeLeft = iTimeLeft - thisTick + lastTick;
		}
		
		if ( iTimeLeft < 0 && !bTimeUp )
		{
			bTimeUp = true;
			Audio->FadeMusic( 1500 );
		}
		
		if ( bTimeUp )
		{
			iFootY += 12 * (thisTick - lastTick );
			if ( iFootY > GROUNDLEVEL - FOOTHEIGHT )
			{
				break;
			}
		}

		int iNumFrames = thisTick - lastTick;
		for ( int i=0; i<iNumFrames; ++i )
		{
			g_oBackend.AdvancePerl();
		};
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
						DoMenu( true );
						break;
					}

					for (int j=4; j<9; j++ )
					{
						if (g_oState.m_aiPlayerKeys[1-a_iPlayerWon][j] == event.key.keysym.sym)
						{
							bKeyPressed = true;
							break;
						}
					}
				}
				break;
			}	// switch statement
		}	// Polling events
		
		int p1x = SvIV(get_sv("p1x", FALSE));
		int p1y = SvIV(get_sv("p1y", FALSE)) - 15;
		int p1f = SvIV(get_sv("p1f", FALSE));
		int p2x = SvIV(get_sv("p2x", FALSE));
		int p2y = SvIV(get_sv("p2y", FALSE)) - 15;
		int p2f = SvIV(get_sv("p2f", FALSE));
		
		SDL_BlitSurface( poBackground, NULL, gamescreen, NULL );

		if ( a_iPlayerWon )
		{
			if (p2f) pack2->draw( ABS(p2f)-1, p2x, p2y, p2f<0 );
		}
		else
		{
			if (p1f) pack1->draw( ABS(p1f)-1, p1x, p1y, p1f<0 );
		}
		
		if ( bTimeUp )
		{
			SDL_Rect oRect;
			oRect.x = 0;
			oRect.y = iFootY + FOOTHEIGHT - 10;
			oRect.w = gamescreen->w;
			oRect.h = gamescreen->h - oRect.y;
			SDL_SetClipRect(gamescreen, &oRect);
		}
		if ( a_iPlayerWon )
		{
			if (p1f) pack1->draw( ABS(p1f)-1, p1x, p1y, p1f<0 );
		}
		else
		{
			if (p2f) pack2->draw( ABS(p2f)-1, p2x, p2y, p2f<0 );
		}
		SDL_SetClipRect( gamescreen, NULL );
		
		if ( !bTimeUp )
		{
			int iNewCounter = iTimeLeft * iGameSpeed / 1000;
			sprintf( acString, "%d", iNewCounter );
			DrawTextMSZ( acString, inkFont, 320, 130, AlignHCenter, C_LIGHTCYAN, gamescreen );
			if ( iNewCounter != iCounter )
			{
				Audio->PlaySample( "alarm.voc" );
				iCounter = iNewCounter;
			}
		}
		else
		{
			SDL_Rect oRect;
			oRect.x = 40;
			oRect.y = iFootY;
			SDL_BlitSurface( poFoot, NULL, gamescreen, &oRect );
		}
		
		SDL_Flip( gamescreen );
		
		if ( g_oState.m_bQuitFlag || bKeyPressed )
		{
			break;
		}
	}
	
	SDL_FillRect( gamescreen, NULL, C_BLACK );
	if ( bTimeUp )
	{
		SDL_UnlockSurface( gamescreen );
		DrawGradientText( "SPLAT!", titleFont, 220, gamescreen );
		Audio->PlaySample( "splat2.voc" );
		SDL_Flip( gamescreen );
		SDL_Delay( 1500 );
		g_oState.m_enGameMode = SState::IN_DEMO;
	}
	else
	{
		SDL_Flip( gamescreen );
	}

	SDL_FreeSurface( poBackground );
	return;
}


