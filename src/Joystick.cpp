/***************************************************************************
                          Joystick.cpp  -  description
                             -------------------
    begin                : Sat Feb 14 2004
    copyright            : (C) 2004 by upi
    email                : upi@feel
 ***************************************************************************/


#include "SDL.h"
#include "Event.h"
#include "common.h"
#include "Joystick.h"

#include <string.h>

CJoystick g_oJoystick;



CJoystick::CJoystick()
{
	m_iNumJoysticks = 0;
}


CJoystick::~CJoystick()
{
}


int InitJoystick()
{
	return g_oJoystick.Init();
}



int CJoystick::Init()
{
	m_iNumJoysticks = 0;
	int iResult = SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	if ( iResult < 0 )
	{
		debug( "CJoystick::Init() failed: %s\n", SDL_GetError() );
		return 0;
	}
	
	int iNumJoysticks = SDL_NumJoysticks();
	debug( "CJoystick::Init(): Number of joysticks is %d\n", iNumJoysticks );

	int i;
	for ( i=0; i<iNumJoysticks; ++i )
	{
		const char* poName = SDL_JoystickName(i);
		debug( "CJoystick::Init(): Joystick #%d is '%s'\n", i, poName );
		
		SDL_Joystick* poJoystick = SDL_JoystickOpen(i);
		if ( NULL == poJoystick )
		{
			continue;
		}
		
		m_apoJoysticks[m_iNumJoysticks] = poJoystick;
		m_apcJoystickNames[m_iNumJoysticks] = poName;
		m_abWorkaround[m_iNumJoysticks] = false;
		m_aiHorizontalAxis[m_iNumJoysticks] = 0;
		m_aiVerticalAxis[m_iNumJoysticks] = 0;
		
		debug( "CJoystick::Init(): Joystick #%d has %d axes, %d buttons, %d balls, %d hats\n",
			i,
			SDL_JoystickNumAxes( poJoystick ),
			SDL_JoystickNumButtons( poJoystick ),
			SDL_JoystickNumBalls( poJoystick ),
			SDL_JoystickNumHats( poJoystick ) );

		if ( strstr( poName, "SIGHT FIGHTER ACTION USB" ) )
		{
			m_abWorkaround[m_iNumJoysticks] = true;
			debug( "CJoystick::Init(): Infuriatingly stupid joystick detected.\n" );
		}
		++m_iNumJoysticks;
	}

	if ( m_iNumJoysticks )
	{
		SDL_JoystickEventState( SDL_ENABLE );
	}
	
	return m_iNumJoysticks;
}



const char* CJoystick::GetJoystickName( int a_iJoystickNumber )
{
	if ( a_iJoystickNumber >= m_iNumJoysticks
		|| a_iJoystickNumber < 0 )
	{
		return NULL;
	}
	
	return m_apcJoystickNames[a_iJoystickNumber];
}



bool CJoystick::TranslateEvent( const SDL_Event* a_poInEvent, SMortalEvent* a_poOutEvent )
{
	a_poOutEvent->m_enType = Me_NOTHING;
	
	switch ( a_poInEvent->type )
	{
	case SDL_JOYAXISMOTION:
	{
		int iJoyNumber	= a_poInEvent->jaxis.which;
		int iAxisNumber	= a_poInEvent->jaxis.axis;
		int iValue		= a_poInEvent->jaxis.value;
		
		if ( iAxisNumber > 1 )
		{
			// Only translate axis 0 and 1.
			return false;
		}
		
		if ( m_abWorkaround[iJoyNumber] )
		{
			iValue = AxisWorkaround( a_poInEvent->jaxis );
			debug( "Translated %d to %d\n", a_poInEvent->jaxis.value, iValue );
		}

		// Try the new value into motion.
		int& riOldValue = iAxisNumber ? m_aiVerticalAxis[iJoyNumber] : m_aiHorizontalAxis[iJoyNumber];
		
		if ( -1 == riOldValue )
		{
			if ( iValue < -3200 )
			{
				return false;
			}
			riOldValue = 0;
			a_poOutEvent->m_enType	= Me_PLAYERKEYUP;
			a_poOutEvent->m_iPlayer	= iJoyNumber;
			a_poOutEvent->m_iKey	= iAxisNumber ? Mk_UP : Mk_LEFT;
			return true;
		}
		
		if ( 0 == riOldValue )
		{
			if ( iValue < -6400 )
			{
				riOldValue = -1;
				a_poOutEvent->m_enType	= Me_PLAYERKEYDOWN;
				a_poOutEvent->m_iPlayer	= iJoyNumber;
				a_poOutEvent->m_iKey	= iAxisNumber ? Mk_UP : Mk_LEFT;
				return true;
			}
			if ( iValue > 6400 )
			{
				riOldValue = 1;
				a_poOutEvent->m_enType	= Me_PLAYERKEYDOWN;
				a_poOutEvent->m_iPlayer	= iJoyNumber;
				a_poOutEvent->m_iKey	= iAxisNumber ? Mk_DOWN : Mk_RIGHT;
				return true;
			}
			return false;
		}
		if ( 1 == riOldValue )
		{
			if ( iValue > 3200 )
			{
				return false;
			}
			riOldValue = 0;
			a_poOutEvent->m_enType	= Me_PLAYERKEYUP;
			a_poOutEvent->m_iPlayer	= iJoyNumber;
			a_poOutEvent->m_iKey	= iAxisNumber ?  Mk_DOWN : Mk_RIGHT;
			return true;
		}
		
		riOldValue = 0;
		return false;
	}
	
	case SDL_JOYBUTTONDOWN:
	case SDL_JOYBUTTONUP:
	{
		int iJoyNumber	= a_poInEvent->jbutton.which;
		int iButton		= a_poInEvent->jbutton.button;
		int iPressed	= a_poInEvent->jbutton.state == SDL_PRESSED;
		
		debug( "Button down: joy %d, button %d, state %d\n", iJoyNumber, iButton, iPressed );

		if ( iButton > 4 )
		{
			if ( iButton == SDL_JoystickNumButtons(m_apoJoysticks[iJoyNumber]) - 1
				&& iPressed )
			{
				a_poOutEvent->m_enType = Me_MENU;
				return true;
			}
			return false;
		}
		
		if ( iJoyNumber > 1 )
		{
			return false;
		}
		
		a_poOutEvent->m_enType	= iPressed ? Me_PLAYERKEYDOWN : Me_PLAYERKEYUP;
		a_poOutEvent->m_iPlayer	= iJoyNumber;
		a_poOutEvent->m_iKey	= iButton + 4;

		return true;
	}

	case SDL_JOYBALLMOTION:
		if( a_poInEvent->jball.ball == 0 )
		{
			// Nothing for now.
		}
		break;
	
	case SDL_JOYHATMOTION:
		if ( a_poInEvent->jhat.hat & SDL_HAT_UP )
		{
			// Nothing for now.
		}
		if ( a_poInEvent->jhat.hat & SDL_HAT_LEFT )
		{
			// Nothing for now.
		}
		if ( a_poInEvent->jhat.hat & SDL_HAT_RIGHTDOWN )
		{
			// Nothing for now.
		}
		break;
	
	} // end of switch statement

	return false;
}


int CJoystick::AxisWorkaround( const SDL_JoyAxisEvent& a_roEvent )
{
	if (   0 == a_roEvent.value ) return -32768;
	if ( 127 == a_roEvent.value ) return 0;
	if (  -1 == a_roEvent.value ) return 32768;

	return 0;
}



