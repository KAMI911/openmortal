/***************************************************************************
                          common.cpp  -  description
                             -------------------
    begin                : Fri Aug 24 2001
    copyright            : (C) 2001 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include <stdio.h>
#include <stdarg.h>

#include "common.h"
#include "State.h"
#include "Joystick.h"
#include "SDL/SDL.h"
#include "Event.h"


SDL_Surface* gamescreen = NULL;

void debug( const char* format, ... )
{
	va_list ap;
	va_start( ap, format );
	vfprintf( stderr, format, ap );
	va_end( ap );
}



bool FindPlayerKey( SDLKey a_enKey, int& a_riOutPlayer, int& a_riOutKey )
{
	for (int i=0; i<2; i++)
	{
		for (int j=0; j<9; j++ )
		{
			if (g_oState.m_aiPlayerKeys[i][j] == a_enKey )
			{
				a_riOutPlayer = i;
				a_riOutKey = j;
				return true;
			}
		}
	}
	return false;
}



bool TranslateEvent( const SDL_Event* a_poInEvent, SMortalEvent* a_poOutEvent )
{
	a_poOutEvent->m_enType = Me_NOTHING;
	
	switch ( a_poInEvent->type )
	{
	case SDL_QUIT:
		a_poOutEvent->m_enType = Me_QUIT;
		g_oState.m_bQuitFlag = true;
		return true;
	
	case SDL_KEYDOWN:
	{
		SDLKey enKey = a_poInEvent->key.keysym.sym;
		if ( enKey == SDLK_ESCAPE )
		{
			a_poOutEvent->m_enType = Me_MENU;
			return true;
		}
		if ( enKey == SDLK_F1 )
		{
			a_poOutEvent->m_enType = Me_SKIP;
			return true;
		}
		
		// Check the player keys
		int iPlayer;
		int iKey;
		bool bFound = FindPlayerKey( enKey, iPlayer, iKey );

		if ( bFound )
		{
			a_poOutEvent->m_enType = Me_PLAYERKEYDOWN;
			a_poOutEvent->m_iPlayer = iPlayer;
			a_poOutEvent->m_iKey = iKey;
			return true;
		}
		break;
	}
		
	case SDL_KEYUP:
	{
		SDLKey enKey = a_poInEvent->key.keysym.sym;
		int iPlayer;
		int iKey;
		bool bFound = FindPlayerKey( enKey, iPlayer, iKey );

		if ( bFound )
		{
			a_poOutEvent->m_enType = Me_PLAYERKEYUP;
			a_poOutEvent->m_iPlayer = iPlayer;
			a_poOutEvent->m_iKey = iKey;
			return true;
		}
		break;
	}

	case SDL_JOYAXISMOTION:
	case SDL_JOYBALLMOTION:
	case SDL_JOYHATMOTION:
	case SDL_JOYBUTTONDOWN:
	case SDL_JOYBUTTONUP:
	{
		return g_oJoystick.TranslateEvent( a_poInEvent, a_poOutEvent );
	}


	} // switch

	return false;
}

