/***************************************************************************
                          Chooser.cpp  -  description
                             -------------------
    begin                : Tue Jan 27 2004
    copyright            : (C) 2004 by upi
    email                : upi@feel
 ***************************************************************************/


#include "Chooser.h"
#include "Backend.h"

#include "SDL_image.h"
#include "gfx.h"
#include "common.h"


CChooser g_oChooser;


CChooser::CChooser()
{
	m_iNumberOfFighters = -1;
}


CChooser::~CChooser()
{
	// Should free the portraits..
}

void CChooser::Init()
{
	if ( m_iNumberOfFighters >= 0 )
	{
		// Should only run once.
		return;
	}
	
	// 1. Query the list of fighters from PlayerSelect.
	
	m_iNumberOfFighters = g_oBackend.GetNumberOfFighters();
	if ( m_iNumberOfFighters > 100 )
	{
		m_iNumberOfFighters = 100;
	}
	
	char pcFilename[FILENAME_MAX+1];
	const char* s;
	int i;
	
	for ( i=0; i<m_iNumberOfFighters; ++i )
	{
		// Load the info of fighter #i
		FighterEnum enFighter = g_oBackend.GetFighterID( i );
		debug( "Fighter %d ID is %d\n", i, enFighter );
		m_aenFighters[i] = enFighter;
		
		// Load the portrait of fighter #i
		g_oBackend.PerlEvalF( "GetFighterStats(%d);", enFighter );
		s = g_oBackend.GetPerlString( "Portrait" );
		
		strcpy( pcFilename, DATADIR );
		strcat( pcFilename, "/characters/" );
		strcat( pcFilename, s );
		
		m_apoPortraits[i] = IMG_Load( pcFilename );
	}
	for ( i=m_iNumberOfFighters; i<100; ++i )
	{
		m_aenFighters[i] = UNKNOWN;
		m_apoPortraits[i] = NULL;
	}
	
	Resize( 158, 74, 483, 479 );
}


#define LINEWIDTH 5

void CChooser::Draw( SDL_Surface* a_poSurface )
{
	Init();
	
	// 1. CREATE A BLACK AND WHITE BUFFER
	
	SDL_Surface* poBuffer = SDL_CreateRGBSurface( SDL_SWSURFACE, x2 - x1, y2 - y1, 8, 0, 0, 0, 0 );
	
	SDL_Color aoColors[256];
	int i;
	for ( i=0; i<256; ++i )
	{
		aoColors[i].r = aoColors[i].g = aoColors[i].b = i;
	}
	SDL_SetColors( poBuffer, aoColors, 0, 256);
	SDL_SetColorKey( poBuffer, SDL_SRCCOLORKEY, 0 );
	
	// 2. START BLITTING THE CHARACTER PORTRAITS
	
	for ( i = 0; i<m_iNumberOfFighters; ++i )
	{
		// Blit portrait # i

		if ( NULL == m_apoPortraits[i] )
		{
			continue;
		}
		
		// Calculate the bounding rectangle of the character portrait
		
		int iCol = i % m_iCols;
		int iRow = i / m_iCols;
		int w = (x2-x1-LINEWIDTH) / m_iCols + 1;
		int h = (y2-y1-LINEWIDTH) / m_iRows + 1;
		int x = (x2-x1-LINEWIDTH) * iCol / m_iCols + LINEWIDTH;
		int y = (y2-y1-LINEWIDTH) * iRow / m_iRows + LINEWIDTH;

		SDL_Rect oSrc;
		oSrc.x = oSrc.y = 0;
		oSrc.w = w;
		oSrc.h = h;
		SDL_Rect oDst;
		oDst.x = x;
		oDst.y = y;

		debug( "Drawing portrait %d at %d:%d %dx%d\n", i, x, y, w, h );

		SDL_BlitSurface( m_apoPortraits[i], &oSrc, poBuffer, &oDst );
	}

	SDL_Rect oDst;
	oDst.x = x1;
	oDst.y = y1;
	SDL_BlitSurface( poBuffer, NULL, a_poSurface, &oDst );
}



void CChooser::Resize( int a_x1, int a_y1, int a_x2, int a_y2 )
{
	x1 = a_x1;
	y1 = a_y1;
	x2 = a_x2;
	y2 = a_y2;
	m_iRows = 1;
	m_iCols = 1;

	while ( m_iRows * m_iCols < m_iNumberOfFighters )
	{
		double dColDensity = double(x2-x1) / double(m_iCols);
		double dRowDensity = double(y2-y1) / double(m_iRows);

		if ( dRowDensity > dColDensity )
		{
			m_iRows ++;
		}
		else
		{
			m_iCols ++;
		}
	}
}

