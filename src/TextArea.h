/***************************************************************************
                          TextArea.h  -  description
                             -------------------
    begin                : Wed Jan 28 2004
    copyright            : (C) 2004 by upi
    email                : upi@feel
 ***************************************************************************/


#ifndef TEXTAREA_H
#define TEXTAREA_H

#include "SDL_video.h"

struct SDL_Surface;
struct _sge_TTFont;

#include <string>
#include <list>


typedef std::list<std::string> TStringList;
typedef std::list<int> TIntList;

class CTextArea
{
public:
	CTextArea( SDL_Surface* a_poScreen, _sge_TTFont* a_poFont, int a_x, int a_y, int a_w, int a_h );
	~CTextArea();

	void TintBackground( int a_iColor, int a_iAlpha );
	void AddString( const char* a_poText, int a_iColor );
	void Redraw();
	void Clear();
	void ScrollUp();
	void ScrollDown();
	
protected:
	SDL_Surface*		m_poScreen;
	SDL_Surface*		m_poBackground;
	_sge_TTFont*		m_poFont;
	SDL_Rect			m_oClipRect;
	int					x, y, w, h;
	
	int					m_iScrollOffset;
	TStringList			m_asRowTexts;
	TIntList			m_aiRowColors;
};


#endif // TEXTAREA_H
