/***************************************************************************
                          Chooser.h  -  description
                             -------------------
    begin                : Tue Jan 27 2004
    copyright            : (C) 2004 by upi
    email                : upi@feel
 ***************************************************************************/


#ifndef CHOOSER_H
#define CHOOSER_H



#include "FighterEnum.h"

struct SDL_Surface;


class CChooser
{
public:
	CChooser();
	~CChooser();

	void		Draw( SDL_Surface* a_poSurface );
	void		DrawRectangles();
	void		Resize( int x1, int y1, int x2, int y2 );
	FighterEnum	GetCurrentFighter( int a_iPlayer );
	int			GetCurrentPosition( int a_iPlayer );
	bool		GetDone( int a_iPlayer );

	void		SetDone( int a_iPlayer, bool a_bDone );
	void		SetCurrentFighter( int a_iPlayer );
	void		Move( int a_iPlayer, int a_iDirection );

protected:
	void		Init();

protected:
	FighterEnum	m_aenFighters[100];
	SDL_Surface* m_apoPortraits[100];
	bool		m_iPlayerDone[2];
	int			m_iPlayerX[2];
	int			m_iPlayerY[2];
	
	int			x1, y1, x2, y2;
	
	int			m_iNumberOfFighters;
	int			m_iRows;
	int			m_iCols;
	
};

extern CChooser g_oChooser;

#endif // CHOOSER_H
