/***************************************************************************
                          FlyingChars.h  -  description
                             -------------------
    begin                : Mon Aug 12 2003
    copyright            : (C) 2003 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/



#ifndef FLYINGCHARS_H
#define FLYINGCHARS_H


#include "sge_bm_text.h"
#include <vector>
#include <list>


/**
\class CFlyingChars
\brief Characters jumping onto the screen to form the text in demos

\ingroup Media
*/

class CFlyingChars
{

	struct SFlyingLetter
	{
		int m_iX, m_iY;
		int m_iSX, m_iSY;
		int m_iDX, m_iDY;
		int m_iDelay;
		int m_iTime;
		unsigned char m_cLetter;
	};

public:

	enum TextAlignmentEnum {
		FC_AlignLeft,
		FC_AlignRight,
		FC_AlignCenter,
		FC_AlignJustify,
	};

public:
	CFlyingChars( sge_bmpFont* a_poFont, const SDL_Rect& a_roRect, int a_iFontDisplacement = 0 );
	~CFlyingChars();
	
	void AddText( const char* a_pcText, TextAlignmentEnum a_enAlignment, bool bOneByOne );
	
	void Advance( int a_iNumFrames );
	void Draw();
	bool IsDone();
	int GetCount();
	
protected:
	void AddNextLine();
	int GetCharWidth( unsigned char a_cChar );
	void DequeueText();

protected:

	struct SEnqueuedText
	{
		const char*				m_pcText;
		TextAlignmentEnum		m_enAlignment;
	};
	std::list<SEnqueuedText>	m_oEnqueuedTexts;
	bool						m_bDone;

	bool						m_bScrolling;
	double						m_dScrollupRate;
	double						m_dScrollup;

	typedef std::vector<SFlyingLetter>	CFlyingLetterList;
	typedef CFlyingLetterList::iterator	CFlyingLetterIterator;

	sge_bmpFont*				m_poFont;
	int							m_iFontDisplacement;
	CFlyingLetterList			m_oLetters;
	int							m_iTimeToNextLine;

	SDL_Rect					m_oRect;	
	const unsigned char*		m_pcText;
	TextAlignmentEnum			m_enAlignment;
	int							m_iTextOffset;
	int							m_iLastLineY;
	int							m_iDelay;
};

#endif //  FLYINGCHARS_H
