/***************************************************************************
                          MortalNetwork.h  -  description
                             -------------------
    begin                : Sun Jan 25 2004
    copyright            : (C) 2004 by upi
    email                : upi@feel
 ***************************************************************************/


#ifndef MORTALNETWORK_H
#define MORTALNETWORK_H

#include "FighterEnum.h"


/** Mortal Network messages:


TYPICAL MESSAGE FLOW:

<connection is established>
Introduction messages are sent (version checking, usernames)

<players go to the character selection screen>
1. F <number> messages go both ways as players choose their characters.
2. R message goes in both direction when players have finished choosing.

<both sides go to the game screen>
In odd rounds,  the "server" if the "master" and the "client" is the "slave"
In even rounds, the "client" if the "master" and the "server" is the "slave"

Both the master and the slave send an S message to synchronize the game start.

The master sends G <text> messages to update the backend on the slave side.
The slave sends K <number> <bool> messages to communicate keystrokes to the master side.
The master sends O <number> <bool> message when the round is over.

Back to game start synchronization.

<both sides go to final judgement - may disconnect the game>

<both sides go back to the character selection screen>


OTHERS:

Msgs can be send on the character selection screen with M <text>.
The connection can be broken at any time. IsConnectionAlive() must be called
periodically.

The "server" is always appears as player 1, the "client" is always player 2.
However, they both use the "Player 1" keys.


SUMMARY OF MESSAGES:

I <version> <username> - Introduction sent both ways on connection.
G <text>			- Update on the game backend data.
K <number> <bool>	- Key # up/down
M <text>			- Incoming Msg text.
F <number>			- I have switched to fighter X.
R					- I have chosen a fighter.
S					- Ready for the next round (synch).
O <number> <bool>	- The round is over (who won, are there more rounds).

*/






class CMortalNetwork
{
public:
	
	static void Create();
	
	// Connection's lifecycle
	
	virtual bool		Start( const char* a_pcServerName ) = 0;	// Accept connection, or connect to given server
	virtual void		Stop() = 0;					// Disconnect
	virtual bool		IsConnectionAlive() = 0;	// Is the connection still good?
	virtual void		Update() = 0;				// Read network traffic. Might get disconnected...
	virtual const char*	GetLastError() = 0;
	virtual bool		IsMaster() = 0;				// Am I Master or Slave?
	
	// Msg related methods

	virtual const char*	GetRemoteUsername() = 0;	// This is the name that is passed upon connection.
	virtual void		SendMsg( const char* a_rsMsg ) = 0;	// Prompt the user for a line of chat text
	virtual bool		IsMsgAvailable() = 0;		// Returns true is a chatline has arrived
	virtual const char* GetMsg() = 0;				// The next chatline, or NULL if there are no more.
	
	// Charater Selection methods
	
	virtual bool		IsRemoteFighterAvailable( FighterEnum a_enFighter ) = 0;	// Does the other computer have fighter X installed?
	virtual FighterEnum	GetRemoteFighter() = 0;		// Returns the latest fighter chosen by the remote side.
	virtual bool		IsRemoteSideReady() = 0;	// The other player is finished choosing.
	
	virtual void		SendFighter( FighterEnum a_enFighter ) = 0;	// Let the other side know that I switched to fighter X.
	virtual void		SendReady() = 0;			// Let the other side know that I am ready.
	
	// Game methods

	virtual void		SynchStartRound() = 0;
	virtual void		SendGameData( const char* a_pcGameData ) = 0;
	virtual const char*	GetLatestGameData() = 0;
	virtual void		SendKeystroke( int a_iKey, bool a_bPressed ) = 0;
	virtual bool		GetKeystroke( int& a_riOutKey, bool a_rbPressed ) = 0;

	virtual void		SendRoundOver( int a_iWhoWon, bool a_bGameOver ) = 0;
	virtual bool		IsRoundOver( int& a_riOutWhoWon ) = 0;
	virtual bool		IsGameOver() = 0;
};


extern CMortalNetwork* g_poNetwork;


#endif // MORTALNETWORK_H
