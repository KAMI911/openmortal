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


typedef std::list<int> TIntList;
typedef std::list<std::string> TStringList;

 
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
	void		SendGameParams( int a_iGameSpeed, int a_iGameTime, int a_iHitPoints );

	// Game methods

	bool		SynchStartRound();
	void		SendGameData( const char* a_pcGameData );
	const char*	GetLatestGameData();
	
	void		SendKeystroke( int a_iKey, bool a_bPressed );
	bool		GetKeystroke( int& a_riOutKey, bool& a_rbPressed );

	void		SendGameTime( int a_iGameTime, int a_iGamePhase );
	int			GetGameTime();
	int			GetGamePhase();

	void		SendHurryup( int a_iHurryUpCode );
	int			GetHurryup() ;

	void		SendRoundOver( int a_iWhoWon, bool a_bGameOver );
	int			GetWhoWon();
	bool		IsRoundOver();
	bool		IsGameOver();

protected:
	void		SendRawData( char a_cID, const void* a_pData, int a_iLength );
	
	void		ReceiveMsg( void* a_pData, int a_iLength );
	void		ReceiveGameData( void* a_pData, int a_iLength );
	void		ReceiveKeystroke( void* a_pData, int a_iLength );
	void		ReceiveFighter( void* a_pData, int a_iLength );
	void		ReceiveReady( void* a_pData, int a_iLength );
	void		ReceiveRoundOver( void* a_pData, int a_iLength );
	void		ReceiveGameTime( void* a_pData, int a_iLength );
	void		ReceiveHurryup( void* a_pData, int a_iLength );
	void		ReceiveRemoteFighterAvailable( void* a_pData, int a_iLength );
	void		ReceiveRemoteFighterQuery( void* a_pData, int a_iLength );
	void		ReceiveGameParams( void* a_pData, int a_iLength );
	
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

	char					m_acIncomingBuffer[2048];
	int						m_iIncomingBufferSize;
	
	std::string				m_sLastError;
	
	TStringList				m_asMsgs;
	
	// GAME DATA
	
	TIntList				m_aiAvailableRemoteFighters;
	FighterEnum				m_enRemoteFighter;
	bool					m_bRemoteReady;
	struct SGameParams
	{
		Uint32					iGameTime;
		Uint32					iGameSpeed;
		Uint32					iHitPoints;
	}						m_oGameParams;
	
	std::string				m_sLatestGameData;
	TIntList				m_aiKeystrokes;
	TIntList				m_abKeystrokes;

	bool					m_bRoundOver;
	int						m_iWhoWon;
	bool					m_bGameOver;
	int						m_iGameTime;
	int						m_iGamePhase;
	int						m_iHurryupCode;
	
	// REMOTE QUERY RESPONSES
	
	bool					m_bSynchQueryResponse;
};


#endif // MORTALNETWORKIMPL_H

