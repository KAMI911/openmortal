/***************************************************************************
                          MortalNetworkImpl.h  -  description
                             -------------------
    begin                : Sun Jan 25 2004
    copyright            : (C) 2004 by upi
    email                : upi@feel
 ***************************************************************************/

#ifndef MORTALNETWORKIMPL_H
#define MORTALNETWORKIMPL_H


#include "MortalNetwork.h"
#include "SDL_net.h"
#include <string>
#include <list>

 
class CMortalNetworkImpl: public CMortalNetwork
{
public:
	CMortalNetworkImpl();
	virtual ~CMortalNetworkImpl();
	
	// Connection's lifecycle

	bool		Start( const char* a_pcServerName );
	void		Stop();					// Disconnect
	bool		IsConnectionAlive();	// Is the connection still good?
	void		Update();				// Read network traffic. Might get disconnected...
	const char*	GetLastError();
	bool		IsMaster();				// Am I Master or Slave?

	// Msg related methods

	const char*	GetRemoteUsername();	// This is the name that is passed upon connection.
	void		SendMsg( const char* a_pcMsg );	// Prompt the user for a line of chat text
	bool		IsMsgAvailable();		// Returns true is a chatline has arrived
	const char* GetMsg();				// The next chatline, or NULL if there are no more.

	// Charater Selection methods

	bool		IsRemoteFighterAvailable( FighterEnum a_enFighter );	// Does the other computer have fighter X installed?
	FighterEnum	GetRemoteFighter();		// Returns the latest fighter chosen by the remote side.
	bool		IsRemoteSideReady();	// The other player is finished choosing.

	void		SendFighter( FighterEnum a_enFighter );	// Let the other side know that I switched to fighter X.
	void		SendReady();			// Let the other side know that I am ready.

	// Game methods

	void		SynchStartRound();
	void		SendGameData( const char* a_pcGameData );
	const char*	GetLatestGameData();
	void		SendKeystroke( int a_iKey, bool a_bPressed );
	bool		GetKeystroke( int& a_riOutKey, bool a_rbPressed );

	void		SendRoundOver( int a_iWhoWon, bool a_bGameOver );
	bool		IsRoundOver( int& a_riOutWhoWon );
	bool		IsGameOver();

protected:
	void		InternalSendString( const char* a_pcText, char a_cID );
	char*		InternalReceiveString( void* a_pData, int a_iLength, int& a_riOutLength );
	
	int			ReceiveMsg( void* a_pData, int a_iLength );
	int			ReceiveGameData( void* a_pData, int a_iLength );
	int			ReceiveKeystroke( void* a_pData, int a_iLength );
	int			ReceiveFighter( void* a_pData, int a_iLength );
	int			ReceiveReady( void* a_pData, int a_iLength );
	int			ReceiveRoundOver( void* a_pData, int a_iLength );

protected:
	bool		m_bNetworkAvailable;
	
	enum TNetworkState
	{
		NS_DISCONNECTED,
		NS_CHARACTER_SELECTION,
		NS_IN_GAME,
	};

	TNetworkState			m_enState;
	bool					m_bServer;
	bool					m_bMaster;
	TCPsocket				m_poSocket;
	SDLNet_SocketSet		m_poSocketSet;
	
	std::string				m_sLastError;
	
	std::list<std::string>	m_asMsgs;
	
	// GAME DATA
	
	FighterEnum				m_enRemoteFighter;
	bool					m_bRemoteReady;
	
	std::string				m_sLatestGameData;
	std::list<int>			m_aiKeystrokes;
	std::list<int>			m_abKeystrokes;

	bool					m_bRoundOver;
	int						m_iWhoWon;
	bool					m_bGameOver;
	
	// REMOTE QUERY RESPONSES
	
	bool					m_bSynchQueryResponse;
};


#endif // MORTALNETWORKIMPL_H

