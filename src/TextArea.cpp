/***************************************************************************
                          TextArea.cpp  -  description
                             -------------------
    begin                : Wed Jan 28 2004
    copyright            : (C) 2004 by upi
    email                : upi@feel
 ***************************************************************************/

#include "TextArea.h"
#include "sge_surface.h"
#include "sge_tt_text.h"
#include "gfx.h"
#include "common.h"


CTextArea::CTextArea( SDL_Surface* a_poScreen, _sge_TTFont* a_poFont, int a_x, int a_y, int a_w, int a_h )
{
	m_poScreen = a_poScreen;
	m_poFont = a_poFont;
	m_iScrollOffset = 0;

	m_oClipRect.x = x = a_x;
	m_oClipRect.y = y = a_y;
	m_oClipRect.w = w = a_w;
	m_oClipRect.h = h = a_h;
	
	m_poBackground = sge_copy_surface( a_poScreen, x, y, w, h );
}


CTextArea::~CTextArea()
{
	SDL_FreeSurface( m_poBackground );
	m_poBackground = NULL;
}


void CTextArea::Clear()
{
	m_aiRowColors.clear();
	m_asRowTexts.clear();
	m_iScrollOffset = 0;
}


void CTextArea::AddString( const char* a_poText, int a_iColor )
{
	m_asRowTexts.push_front( a_poText );
	m_aiRowColors.push_front( a_iColor );

	debug( "Added '%s', number of texts is %d\n", a_poText, m_asRowTexts.size() );
}


void CTextArea::ScrollUp()
{
	if ( m_iScrollOffset < (int) m_asRowTexts.size() - 1 )
	{
		++m_iScrollOffset;
		Redraw();
	}
}


void CTextArea::ScrollDown()
{
	if ( m_iScrollOffset > 0 )
	{
		--m_iScrollOffset;
		Redraw();
	}
}


void CTextArea::Redraw()
{
	TStringList::const_iterator itString = m_asRowTexts.begin();
	TIntList::const_iterator itColors = m_aiRowColors.begin();

	for ( int i=0; i<m_iScrollOffset; ++i )
	{
		++itString;
		++itColors;
	}

	SDL_Rect oOldClipRect;
	SDL_GetClipRect( m_poScreen, &oOldClipRect );
	SDL_SetClipRect( m_poScreen, &m_oClipRect );
	
	sge_Blit( m_poBackground, m_poScreen, 0, 0, x, y, w, h );
	
	for ( int yPos = y + h - sge_TTF_FontHeight( m_poFont );
		yPos >= y /*+ sge_TTF_FontAscent( m_poFont )*/;
		yPos -= sge_TTF_FontHeight( m_poFont ) )
	{
		if ( itColors == m_aiRowColors.end() )
		{
			break;
		}

		// Print the current text
		sge_tt_textout( m_poScreen, m_poFont, itString->c_str(), x, yPos + sge_TTF_FontAscent(m_poFont), *itColors, C_BLACK, 255 );
		++itString;
		++itColors;
	}

	sge_UpdateRect( m_poScreen, x, y, w, h );
	SDL_SetClipRect( m_poScreen, &oOldClipRect );
}

