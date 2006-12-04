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


/**
\defgroup PlayerSelect Fighter selection
This module runs the fighter selection part of the game.
*/

#include "FighterEnum.h"
#include "common.h"

#include <string>
#include <vector>

class CRlePack;
struct SDL_Surface;
class CTextArea;
class CReadline;



/** PlayerInfo structure stores information about a PLAYER.

In OpenMortal, the term PLAYER refers to one of the two guys playing the
game, as opposed to FIGHTER, which refers to one of the many playable
characters.

PlayerInfo stores: the player's selected fighter, the fighters tint and
CRlePack. 
\ingroup PlayerSelect
*/

struct SPlayerInfo
{
	FighterEnum		m_enFighter;
	TintEnum		m_enTint;
	CRlePack*		m_poPack;
	std::string		m_sFighterName;

	std::vector<FighterEnum> m_aenTeam;
};



/** This class implements services that allows players to select their fighters.

It also stores info about which fighter is available, and
allows other parts of the program to programmatically assign a fighter
to a player, and set fighter tints (this is used by e.g. the "frozen"
effect.) 

This is the model part model-view-controller architecture of the player selection.


\ingroup PlayerSelect
*/

class CPlayerSelect
{
public:
	CPlayerSelect();
	
	const SPlayerInfo& GetPlayerInfo( int a_iPlayer );
	SPlayerInfo& EditPlayerInfo( int a_iPlayer );
	const char* GetFighterName( int a_iPlayer );
	int GetFighterNameWidth( int a_iPlayer );
	
	void DoPlayerSelect();
	void SetPlayer( int a_iPlayer, FighterEnum a_enFighter );
	void SetTint( int a_iPlayer, TintEnum a_enFighter );
	bool IsFighterAvailable( FighterEnum a_enFighter );
	bool IsLocalFighterAvailable( FighterEnum a_enFighter );
	bool IsFighterInTeam( FighterEnum a_enFighter );


protected:

	static CRlePack* LoadFighter( FighterEnum m_enFighter );

protected:
	SPlayerInfo	m_aoPlayers[MAXPLAYERS];
	int			m_aiFighterNameWidth[MAXPLAYERS];
};


extern CPlayerSelect g_oPlayerSelect;

#endif // PLAYERSELECT_H
