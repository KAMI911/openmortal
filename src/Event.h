/***************************************************************************
                          Event.h  -  description
                             -------------------
    begin                : Sat Feb 14 2004
    copyright            : (C) 2004 by upi
    email                : upi@feel
 ***************************************************************************/


#ifndef EVENT_H
#define EVENT_H


enum TMortalEventEnum
{
	Me_NOTHING,
	Me_QUIT,
	Me_MENU,
	Me_SKIP,
	Me_PLAYERKEYDOWN,
	Me_PLAYERKEYUP,
};


enum TMortalKeysEnum
{
	Mk_UP		= 0,
	Mk_DOWN		= 1,
	Mk_LEFT		= 2,
	Mk_RIGHT	= 3,
	Mk_BLOCK	= 4,
	Mk_LPUNCH	= 5,
	Mk_HPUNCH	= 6,
	Mk_LKICK	= 7,
	Mk_HKICK	= 8,
};

/**
\ingroup GameLogic
*/
struct SMortalEvent
{
	TMortalEventEnum	m_enType;
	int					m_iPlayer;
	int					m_iKey;
};

bool TranslateEvent( const SDL_Event* a_poInSDLEvent, SMortalEvent* a_poOutEvent );
bool MortalPollEvent( SMortalEvent& a_roOutEvent );
void MortalWaitEvent( SMortalEvent& a_roOutEvent );


#endif

