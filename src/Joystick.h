/***************************************************************************
                          Joystick.h  -  description
                             -------------------
    begin                : Sat Feb 14 2004
    copyright            : (C) 2004 by upi
    email                : upi@feel
 ***************************************************************************/


#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "SDL.h"

struct SMortalEvent;

/**
\ingroup Media
*/
class CJoystick
{
public:
	CJoystick();
	~CJoystick();

	int Init();
	bool TranslateEvent( const SDL_Event* a_poInEvent, SMortalEvent* a_poOutEvent );
	const char* GetJoystickName( int a_iJoystickNumber );

protected:

	int AxisWorkaround( const SDL_JoyAxisEvent& a_roEvent );

	enum {
		mg_iMaxJoysticks = 32,
	};

	int				m_iNumJoysticks;
	SDL_Joystick*	m_apoJoysticks[mg_iMaxJoysticks];
	const char*		m_apcJoystickNames[mg_iMaxJoysticks];
	bool			m_abWorkaround[mg_iMaxJoysticks];

	int				m_aiHorizontalAxis[mg_iMaxJoysticks];
	int				m_aiVerticalAxis[mg_iMaxJoysticks];
};


extern CJoystick g_oJoystick;

#endif // JOYSTICK_H
