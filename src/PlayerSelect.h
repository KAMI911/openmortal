/***************************************************************************
                          PlayerSelect.h  -  description
                             -------------------
    begin                : 2003-09-05
    copyright            : (C) 2003 by upi
    email                : upi@apocalypse.rulez.org
 ***************************************************************************/


#ifndef PLAYERSELECT_H
#define PLAYERSELECT_H

#define COLORSPERPLAYER		64
#define COLOROFFSETPLAYER1	112
#define COLOROFFSETPLAYER2	(COLOROFFSETPLAYER1+64)



#include "FighterEnum.h"
#include <string>

class RlePack;
struct SDL_Surface;

/** PlayerInfo structure stores information about a PLAYER.

In OpenMortal, the term PLAYER refers to one of the two guys playing the
game, as opposed to FIGHTER, which refers to one of the many playable
characters.

PlayerInfo stores: the player's selected fighter, the fighters tint and
RlePack. */

struct PlayerInfo
{
	FighterEnum		m_enFighter;
	TintEnum		m_enTint;
	RlePack*		m_poPack;
	std::string		m_sFighterName;
};



/** This class implements services that allows players to select their
fighters. It also stores info about which fighter is available, and
allows other parts of the program to programmatically assign a fighter
to a player, and set fighter tints (this is used by e.g. the "frozen"
effect.) */

class PlayerSelect
{
public:
	PlayerSelect();
	
	const PlayerInfo& GetPlayerInfo( int a_iPlayer );
	const char* GetFighterName( int a_iPlayer );
	int GetFighterNameWidth( int a_iPlayer );
	
	void DoPlayerSelect();
	void SetPlayer( int a_iPlayer, FighterEnum a_enFighter );
	void SetTint( int a_iPlayer, TintEnum a_enFighter );
	bool IsFighterAvailable( FighterEnum a_enFighter );

protected:

	void HandleKey( int a_iPlayer, int a_iKey );
	void DrawRect( int a_iPos, int a_iColor );
	void CheckPlayer( SDL_Surface* a_poBackground, int a_iRow, int a_iCol );
	static RlePack* LoadFighter( FighterEnum m_enFighter );

protected:
	PlayerInfo	m_aoPlayers[2];
	int			m_iP1, m_iP2;			// Chooser cells for player 1 and 2
	bool		m_bDone1, m_bDone2;		// Has player n chosen a player?
	int			m_aiFighterNameWidth[2];
	
};


extern PlayerSelect g_oPlayerSelect;

#endif // PLAYERSELECT_H
