/***************************************************************************
                          PlayerSelect.cpp  -  description
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/

#include <stdio.h>

#include "PlayerSelect.h"
#include "PlayerSelectController.h"
 
#include "SDL.h"
#include "SDL_video.h"
#include "SDL_image.h"
#include "sge_primitives.h"
#include "sge_surface.h"
 
#include "common.h"
#include "Audio.h"
#include "sge_bm_text.h"
#include "gfx.h"
#include "RlePack.h"
#include "Backend.h"
#include "State.h"
#include "MortalNetwork.h"
#include "Chooser.h"
#include "sge_tt_text.h"
#include "TextArea.h"
#include "Event.h"


#ifndef NULL
#define NULL 0
#endif


/***************************************************************************
                     PUBLIC EXPORTED VARIABLES
***************************************************************************/


CPlayerSelect g_oPlayerSelect;








CPlayerSelect::CPlayerSelect()
{
	for ( int i=0; i<MAXPLAYERS; ++i )
	{
		m_aoPlayers[i].m_enFighter = UNKNOWN;
		m_aoPlayers[i].m_enTint = NO_TINT;
		m_aoPlayers[i].m_poPack = NULL;
	}
}



/*************************************************************************
							GENERAL PLAYER INFO
*************************************************************************/

/**
Returns the info of a given player. The most important piece in the info 
is the player's CRlePack which is used for drawing the player in the Game, 
the GameOver and the FighterInfo screens.
*/

const SPlayerInfo& CPlayerSelect::GetPlayerInfo( int a_iPlayer )
{
	return m_aoPlayers[ a_iPlayer ];
}


SPlayerInfo& CPlayerSelect::EditPlayerInfo( int a_iPlayer )
{
	return m_aoPlayers[ a_iPlayer ];
}


const char* CPlayerSelect::GetFighterName( int a_iPlayer )
{
	return m_aoPlayers[ a_iPlayer ].m_sFighterName.c_str();
}


int CPlayerSelect::GetFighterNameWidth( int a_iPlayer )
{
	return m_aiFighterNameWidth[ a_iPlayer ];
}

/**
Returns true if the given fighter can be used. 
Some fighters are just not in the data files yet, or not installed.
*/

bool CPlayerSelect::IsFighterAvailable( FighterEnum a_enFighter )
{
	if ( a_enFighter <= UNKNOWN )
	{
		return false;
	}
	
	bool bLocalAvailable = IsLocalFighterAvailable( a_enFighter );

	if ( SState::IN_NETWORK != g_oState.m_enGameMode || !bLocalAvailable )
	{
		return bLocalAvailable;
	}
	
	// Check the remote site
	return g_poNetwork->IsRemoteFighterAvailable( a_enFighter );
}


bool CPlayerSelect::IsLocalFighterAvailable( FighterEnum a_enFighter )
{
	if ( a_enFighter <= UNKNOWN )
	{
		return false;
	}

	g_oBackend.PerlEvalF("GetFighterStats(%d);", a_enFighter);
	const char* pcDatafile = g_oBackend.GetPerlString("Datafile");
	return pcDatafile && *pcDatafile;
}



bool CPlayerSelect::IsFighterInTeam( FighterEnum a_enFighter )
{
	std::vector<FighterEnum>::const_iterator it;
	for ( int i=0; i<g_oState.m_iNumPlayers; ++i )
	{
		std::vector<FighterEnum>& roTeam = m_aoPlayers[i].m_aenTeam;
		for ( it=roTeam.begin(); it!=roTeam.end(); ++it )
		{
			if ( a_enFighter == *it )
			{
				return true;
			}
		}
	}

	return false;
}



/** LoadFighter simply looks up the filename associated with the given
fighter, loads it, and returns the CRlePack.

\return The freshly loaded CRlePack, or NULL if it could not be loaded.
*/

CRlePack* CPlayerSelect::LoadFighter( FighterEnum m_enFighter )		// static
{
	char a_pcFilename[FILENAME_MAX+1];
	const char* s;

	g_oBackend.PerlEvalF( "GetFighterStats(%d);", m_enFighter );
	s = g_oBackend.GetPerlString( "Datafile" );

	strcpy( a_pcFilename, DATADIR );
	strcat( a_pcFilename, "/characters/" );
	strcat( a_pcFilename, s );

	CRlePack* pack = new CRlePack( a_pcFilename, COLORSPERPLAYER );
	if ( pack->Count() <= 0 )
	{
		debug( "Couldn't load CRlePack: '%s'\n", a_pcFilename );
		delete pack;
		return NULL;
	}

	return pack;
}



/** SetPlayer loads the given fighter for the given player.

The CRlePack is loaded first. If that succeeds, then the perl backend is
set too. The tint and palette of both players are set. */

void CPlayerSelect::SetPlayer( int a_iPlayer, FighterEnum a_enFighter )
{
	if ( m_aoPlayers[a_iPlayer].m_enFighter == a_enFighter )
	{
		if ( m_aoPlayers[a_iPlayer].m_poPack )
		{
			m_aoPlayers[a_iPlayer].m_poPack->ApplyPalette();
		}
		return;
	}
	if ( !IsFighterAvailable( a_enFighter ) )
	{
		return;
	}

	int iOffset = COLOROFFSETPLAYER1 + a_iPlayer*64;
	CRlePack* poPack = LoadFighter( a_enFighter );
	poPack->OffsetSprites( iOffset );

	if ( NULL == poPack )
	{
		debug( "SetPlayer(%d,%d): Couldn't load CRlePack\n", a_iPlayer, a_enFighter );
		return;
	}

	delete m_aoPlayers[a_iPlayer].m_poPack;
	m_aoPlayers[a_iPlayer].m_poPack = poPack;
	m_aoPlayers[a_iPlayer].m_enFighter = a_enFighter;

	g_oBackend.PerlEvalF( "SetPlayerNumber(%d,%d);", a_iPlayer, a_enFighter );
	m_aoPlayers[a_iPlayer].m_sFighterName = g_oBackend.GetPerlString( "PlayerName" );
	m_aiFighterNameWidth[a_iPlayer] = sge_BF_TextSize( fastFont, GetFighterName(a_iPlayer) ).w;

	TintEnum enTint = NO_TINT;

	//@ CLASHES WITH OTHER PLAYERS NEED TO BE HANDLED
	if ( m_aoPlayers[0].m_enFighter == m_aoPlayers[1].m_enFighter )
	{
		enTint = TintEnum( (rand() % 4) + 1 );
	}
	SetTint( 1, enTint );
	m_aoPlayers[a_iPlayer].m_poPack->ApplyPalette();
}



void CPlayerSelect::SetTint( int a_iPlayer, TintEnum a_enTint )
{
	m_aoPlayers[a_iPlayer].m_enTint = a_enTint;
	if ( m_aoPlayers[a_iPlayer].m_poPack )
	{
		m_aoPlayers[a_iPlayer].m_poPack->SetTint( a_enTint );
		m_aoPlayers[a_iPlayer].m_poPack->ApplyPalette();
	}
}


void CPlayerSelect::DoPlayerSelect()
{
	CPlayerSelectController oController( SState::IN_NETWORK == g_oState.m_enGameMode );
	oController.DoPlayerSelect();
}

