/***************************************************************************
                          MortalNetworkImpl.cpp  -  description
                             -------------------
    begin                : Sun Jan 25 2004
    copyright            : (C) 2004 by upi
    email                : upi@feel
 ***************************************************************************/

#include "MortalNetworkImpl.h"
#include "State.h"
#include "common.h"
#include "config.h"


#define MORTALNETWORKPORT 0x3A22


void MortalNetworkMessage( const char* format, ... );
bool MortalNetworkCheckKey();




CMortalNetwork* g_poNetwork = NULL;




void CMortalNetwork::Create()	// static
{
	if ( NULL == g_poNetwork )
	{
		g_poNetwork = new CMortalNetworkImpl;
	}
}



CMortalNetworkImpl::CMortalNetworkImpl()
{
	m_enState = NS_DISCONNECTED;
	m_bServer = false;
	m_bMaster = false;
	m_poSocket = NULL;

	m_enRemoteFighter = UNKNOWN;
	m_bRemoteReady = false;
	m_bRoundOver = false;
	m_iWhoWon = -1;
	m_bGameOver = false;
	
	if(SDLNet_Init()==-1)
	{
		m_bNetworkAvailable = false;
		m_sLastError = SDLNet_GetError();
		debug ( "Error opening SDLNet: %s\n", m_sLastError.c_str() );
	}

	m_bNetworkAvailable = true;
}


CMortalNetworkImpl::~CMortalNetworkImpl()
{
	Stop();
}


bool CMortalNetworkImpl::Start( const char* a_pcServerName )
{
#define RETURNNOERROR {								\
		debug( "%s\n", m_sLastError.c_str() );		\
		return false; }
#define RETURNWITHERROR {							\
		m_sLastError = SDLNet_GetError();			\
		debug( "%s\n", m_sLastError.c_str() );		\
		return false; }
#define RETURNWITHADDITIONALERROR {					\
		m_sLastError += SDLNet_GetError();			\
		debug( "%s\n", m_sLastError.c_str() );		\
		return false; }

	if ( !m_bNetworkAvailable )
	{
		return false;
	}
	
	debug( "CMortalNetworkImpl::Start( %s )\n", a_pcServerName ? a_pcServerName : "NULL" );
	IPaddress oAddress;

	if ( a_pcServerName )
	{
		MortalNetworkMessage( Translate("Resolving hostname...") );
	}

	int iResult = SDLNet_ResolveHost( &oAddress, a_pcServerName, MORTALNETWORKPORT );
	if ( iResult )
	{
		m_sLastError = Translate( "Couldn't resolve host." );
		RETURNNOERROR;
	}
	debug( "IP Address of server is 0x%x\n", oAddress.host );
	
	if ( a_pcServerName )
	{
		Uint32 ipaddr=SDL_SwapBE32(oAddress.host);
		MortalNetworkMessage("Connecting to %d.%d.%d.%d port %d",
			ipaddr>>24, (ipaddr>>16)&0xff, (ipaddr>>8)&0xff, ipaddr&0xff, oAddress.port);
	}
	
	if ( !a_pcServerName )
	{
		// SERVER-MODE CONNECTION
		
		m_poSocket = SDLNet_TCP_Open( &oAddress );
		
		// Wait for connection ...
		MortalNetworkMessage ( Translate("Waiting for connection... (press any key to abort)") );
		TCPsocket poClient;
		while ( 1 )
		{
			poClient = SDLNet_TCP_Accept( m_poSocket );
			if ( poClient ) break;
			
			if (MortalNetworkCheckKey()) break;;
			SDL_Delay( 100 );
		}
		
		SDLNet_TCP_Close( m_poSocket );
		
		if ( NULL == poClient )
		{
			m_sLastError = "No connection.";
			return false;
		}
		
		IPaddress* poRemoteAddress = SDLNet_TCP_GetPeerAddress(poClient);

		if ( !poRemoteAddress )
		{
			RETURNWITHERROR;
		}
		Uint32 ipaddr=SDL_SwapBE32(poRemoteAddress->host);
		MortalNetworkMessage("Accepted connection from %d.%d.%d.%d port %d",
			ipaddr>>24, (ipaddr>>16)&0xff, (ipaddr>>8)&0xff, ipaddr&0xff, oAddress.port);

		// Set the client socket as our socket, and drop the server socket.

		m_poSocket = poClient;
	}
	else
	{
		// CLIENT-MODE CONNECTION
		
		MortalNetworkMessage ( Translate("Waiting for connection... (press any key to abort)") );

		while (1)
		{
			m_poSocket = SDLNet_TCP_Open( &oAddress );
			if ( m_poSocket ) break;
			if ( MortalNetworkCheckKey() ) break;
			SDL_Delay( 100 );
		}
		if ( NULL == m_poSocket )
		{
			RETURNWITHERROR;
		}
	}

	// CONNECTION ESTABLISHED. SEND INTRO PACKETS

	MortalNetworkMessage( Translate("Connection established.") );
	
	struct SIntroPackage
	{
		char	cID;
		char	acVersion[10];
	} oIntroPackage, oRemotePackage;

	oIntroPackage.cID = 'I';
	strncpy( oIntroPackage.acVersion, VERSION, 10 );
	oIntroPackage.acVersion[9] = 0;

	debug( "Sending intro package... " );
	
	int iRetval = SDLNet_TCP_Send( m_poSocket, &oIntroPackage, sizeof( oIntroPackage ) );

	if ( iRetval < (int) sizeof( oIntroPackage ) )
	{
		RETURNWITHERROR;
	}
	
	iRetval = SDLNet_TCP_Recv( m_poSocket, &oRemotePackage, sizeof( oRemotePackage ) );
	if ( iRetval <= 0 )
	{
		RETURNWITHERROR;
	}
	if ( iRetval < (int) sizeof( oRemotePackage )
		|| oRemotePackage.cID != 'I'
		|| strncmp( oRemotePackage.acVersion, VERSION, 9 ) )
	{
		m_sLastError = Translate( "The remote side has a different version of OpenMortal running." );
		RETURNNOERROR;
	}
	
	MortalNetworkMessage( Translate("Life is good.") );
	
	m_enState = NS_CHARACTER_SELECTION;
	m_bServer = NULL == a_pcServerName;
	m_bMaster = m_bServer;
	m_sLastError = "";
	m_asMsgs.clear();
	m_enRemoteFighter = UNKNOWN;
	m_bRemoteReady = false;
	m_sLatestGameData = "";
	m_aiKeystrokes.clear();
	m_abKeystrokes.clear();
	
	m_poSocketSet = SDLNet_AllocSocketSet( 1 );
	SDLNet_TCP_AddSocket( m_poSocketSet, m_poSocket );	// Check for errors?
	
	while (!MortalNetworkCheckKey()) SDL_Delay( 100 );
	
	return true;
}


void CMortalNetworkImpl::Stop()
{
	if ( NS_DISCONNECTED == m_enState )
	{
		return;
	}

	g_oState.m_enGameMode = SState::IN_DEMO;
	m_enState = NS_DISCONNECTED;
	SDLNet_FreeSocketSet( m_poSocketSet );
	SDLNet_TCP_Close( m_poSocket );
}


bool CMortalNetworkImpl::IsConnectionAlive()
{
	return ( NS_DISCONNECTED != m_enState );
}



#define DISCONNECTONCOMMUNICATIONERROR { 									\
		m_sLastError = Translate("Communication error. Disconnecting.");	\
		Stop();																\
		return; }
#define DISCONNECTWITH(A) { 												\
		m_sLastError = Translate("Communication error. Disconnecting.");	\
		Stop();																\
		return(A); }

#define CHECKCONNECTION if ( NS_DISCONNECTED == m_enState ) return;


void CMortalNetworkImpl::Update()
{
	CHECKCONNECTION;
	
	int iRetval = SDLNet_CheckSockets( m_poSocketSet, 0 );
	if ( iRetval <= 0 )
	{
		return;
	}
	
	char acBuffer[1024];
	iRetval = SDLNet_TCP_Recv( m_poSocket, acBuffer, 1024 );
	if ( iRetval <= 0 )
	{
		m_sLastError = SDLNet_GetError();
		Stop();
		return;
	}

	// OK, we've read it. Now let's see what it is.

	int iOffset = 0;

	while ( iOffset < iRetval )
	{
		debug( "Received stuff.. %c type, %d length, %d offset\n", acBuffer[iOffset], iRetval, iOffset );
	
		if ( NS_CHARACTER_SELECTION == m_enState )
		{
			switch ( acBuffer[iOffset] )
			{
			case 'M': iOffset += ReceiveMsg( acBuffer+iOffset, iRetval ); break;
			case 'F': iOffset += ReceiveFighter( acBuffer+iOffset, iRetval ); break;
			case 'R': iOffset += ReceiveReady( acBuffer+iOffset, iRetval ); break;
			case 'S': ++iOffset; break;
			default: DISCONNECTONCOMMUNICATIONERROR;
			}
		}
		else
		{
			switch ( acBuffer[iOffset] )
			{
			case 'M': iOffset += ReceiveMsg( acBuffer+iOffset, iRetval ); break;
			case 'G': iOffset += ReceiveGameData( acBuffer+iOffset, iRetval ); break;
			case 'K': iOffset += ReceiveKeystroke( acBuffer+iOffset, iRetval ); break;
			case 'O': iOffset += ReceiveRoundOver( acBuffer+iOffset, iRetval ); break;
			case 'S': ++iOffset; break;
			default: DISCONNECTONCOMMUNICATIONERROR;
			}
		}

		if ( !IsConnectionAlive() )
		{
			return;
		}
	}
}


const char* CMortalNetworkImpl::GetLastError()
{
	return m_sLastError.c_str();
}


bool CMortalNetworkImpl::IsMaster()
{
	return m_bMaster;
}


/*
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

	std::list<std::string>	m_asMsgs;

	// GAME DATA

	FighterEnum				m_enRemoteFighter;
	bool					m_bRemoteReady;

	std::string				m_sLatestGameData;
	std::list<int>			m_iKeystrokes;
	std::list<int>			m_bKeystrokes;

	bool					m_bRoundOver;
	int						m_iWhoWon;
	bool					m_bGameOver;
*/


const char* CMortalNetworkImpl::GetRemoteUsername()
{
	return "upi";
}



/*************************************************************************
							MSG RELATED METHODS
*************************************************************************/


#define MAXSTRINGLENGTH 900
struct SMsgPackage
{
	char	cID;
	Uint16	iLength;
	char	acData[1024];
};

void CMortalNetworkImpl::InternalSendString( const char* a_pcText, char a_cID )
{
	CHECKCONNECTION;

	if ( NULL == a_pcText
		|| 0 == *a_pcText )
	{
		return;
	}

	int iLength = strlen( a_pcText );
	if ( iLength > MAXSTRINGLENGTH ) iLength = MAXSTRINGLENGTH;

	SMsgPackage oPackage;
	oPackage.cID = a_cID;
	oPackage.iLength = iLength;
	strncpy( oPackage.acData, a_pcText, iLength );
	oPackage.acData[iLength] = 0;

	int iPackageLength = iLength + sizeof(char) + sizeof(Uint16);

	int iRetval = SDLNet_TCP_Send( m_poSocket, &oPackage, iPackageLength );
	if ( iRetval < iPackageLength ) DISCONNECTONCOMMUNICATIONERROR;
}

char* CMortalNetworkImpl::InternalReceiveString( void* a_pData, int a_iLength, int& a_riOutLength )
{
	a_riOutLength = -1;
	
	// Verify data length vs package length
	
	SMsgPackage* pcPackage = (SMsgPackage*) a_pData;
	if ( a_iLength < (int) sizeof(char) + (int) sizeof(Uint16) + 1 )
	{
		DISCONNECTWITH(NULL);
	}
	
	a_riOutLength = sizeof(char) + sizeof(Uint16) + pcPackage->iLength;
	if ( pcPackage->iLength > MAXSTRINGLENGTH
		|| a_iLength < a_riOutLength )
	{
		DISCONNECTWITH(NULL);
	}
	
	pcPackage->acData[ pcPackage->iLength ] = 0;
	return pcPackage->acData;
}





void CMortalNetworkImpl::SendMsg( const char* a_pcMsg )
{
	InternalSendString( a_pcMsg, 'M' );
}

int CMortalNetworkImpl::ReceiveMsg( void* a_pData, int a_iLength )
{
	int iRetval;
	char* pcMsg = InternalReceiveString( a_pData, a_iLength, iRetval );

	if ( iRetval > 0 )
	{
		m_asMsgs.push_back( pcMsg );
	}

	return iRetval;
}


bool CMortalNetworkImpl::IsMsgAvailable()
{
	return m_asMsgs.size() > 0;
}

const char* CMortalNetworkImpl::GetMsg()
{
	static std::string sLastMsg;
	if ( IsMsgAvailable() )
	{
		sLastMsg = m_asMsgs.front();
		m_asMsgs.pop_front();
		return sLastMsg.c_str();
	}
	return NULL;
}




/*************************************************************************
				CHARACTER SELECTION RELATED METHODS
*************************************************************************/




bool CMortalNetworkImpl::IsRemoteFighterAvailable( FighterEnum a_enFighter )
{
	return true;
}




struct SFighterPackage
{
	char	cID;
	Uint32	iFighter;
};

void CMortalNetworkImpl::SendFighter( FighterEnum a_enFighter )
{
	CHECKCONNECTION;
	
	SFighterPackage oPackage;
	oPackage.cID = 'F';
	oPackage.iFighter = SDL_SwapBE32( a_enFighter );
	int iRetval = SDLNet_TCP_Send( m_poSocket, &oPackage, sizeof(oPackage) );
	if ( iRetval < (int) sizeof(oPackage) ) DISCONNECTONCOMMUNICATIONERROR;
}

int CMortalNetworkImpl::ReceiveFighter( void* a_pcData, int a_iLength )
{
	SFighterPackage *poPackage = (SFighterPackage*) a_pcData;
	if ( a_iLength < (int) sizeof(SFighterPackage) ) DISCONNECTWITH(-1);
	
	m_enRemoteFighter = (FighterEnum) SDL_SwapBE32( poPackage->iFighter );
	debug( "ReceiveFighter: %d\n", m_enRemoteFighter );
	return sizeof( SFighterPackage );
}

FighterEnum CMortalNetworkImpl::GetRemoteFighter()
{
	return m_enRemoteFighter;
}




void CMortalNetworkImpl::SendReady()
{
	CHECKCONNECTION;
	
	char cReady = 'R';
	int iRetval = SDLNet_TCP_Send( m_poSocket, &cReady, sizeof(cReady) );
	if ( iRetval != sizeof(cReady) ) DISCONNECTONCOMMUNICATIONERROR;
}

int CMortalNetworkImpl::ReceiveReady( void* a_pData, int a_iLength )
{
	if ( a_iLength < (int) sizeof(char) ) DISCONNECTWITH(-1);
	m_bRemoteReady = true;
	return sizeof(char);
}

bool CMortalNetworkImpl::IsRemoteSideReady()
{
	return m_bRemoteReady;
}




/*************************************************************************
						GAME RELATED METHODS
*************************************************************************/





void CMortalNetworkImpl::SynchStartRound()
{
	m_bSynchQueryResponse = false;
	
	// run until both sides manage to get a SYNCH

	char cID = 'S';
	
	while ( !m_bSynchQueryResponse )
	{
		CHECKCONNECTION;
		int iRetval = SDLNet_TCP_Send( m_poSocket, &cID, 1 );
		if ( iRetval < 1 ) DISCONNECTONCOMMUNICATIONERROR;
		Update();
		SDL_Delay(200);
		if ( !IsConnectionAlive() ) break;
	}
}





void CMortalNetworkImpl::SendGameData( const char* a_pcGameData )
{
	InternalSendString( a_pcGameData, 'G' );
}

int CMortalNetworkImpl::ReceiveGameData( void* a_pData, int a_iLength )
{
	int iRetval;
	char* pcData = InternalReceiveString( a_pData, a_iLength, iRetval );

	if ( iRetval > 0 )
	{
		m_sLatestGameData = pcData;
	}

	return iRetval;
}

const char* CMortalNetworkImpl::GetLatestGameData()
{
	return m_sLatestGameData.c_str();
}




struct SKeystrokePackage
{
	char	cID;
	char	cKey;
	bool	bPressed;
};

void CMortalNetworkImpl::SendKeystroke( int a_iKey, bool a_bPressed )
{
	SKeystrokePackage oPackage;
	oPackage.cID = 'K';
	oPackage.cKey = a_iKey;
	oPackage.bPressed = a_bPressed;
	
	int iRetval = SDLNet_TCP_Send( m_poSocket, &oPackage, sizeof(oPackage) );
	if ( iRetval < (int)sizeof(oPackage) ) DISCONNECTONCOMMUNICATIONERROR;
}

int CMortalNetworkImpl::ReceiveKeystroke( void* a_pData, int a_iLength )
{
	if ( a_iLength < (int)sizeof(SKeystrokePackage) ) DISCONNECTWITH(-1);
	SKeystrokePackage* poPackage = (SKeystrokePackage*) a_pData;
	
	m_aiKeystrokes.push_back( poPackage->cKey );
	m_abKeystrokes.push_back( poPackage->bPressed );
	return sizeof(SKeystrokePackage);
}

bool CMortalNetworkImpl::GetKeystroke( int& a_riOutKey, bool a_rbOutPressed )
{
	if ( m_aiKeystrokes.size() == 0 )
	{
		return false;
	}
	a_riOutKey = m_aiKeystrokes.front();
	a_rbOutPressed = m_abKeystrokes.front();
	m_aiKeystrokes.pop_front();
	m_abKeystrokes.pop_front();
	return true;
}



void CMortalNetworkImpl::SendRoundOver( int a_iWhoWon, bool a_bGameOver )
{
}


int CMortalNetworkImpl::ReceiveRoundOver( void* a_pData, int a_iLength )
{
	return a_iLength;
}

bool CMortalNetworkImpl::IsRoundOver( int& a_riOutWhoWon )
{
	return false;
}

bool CMortalNetworkImpl::IsGameOver()
{
	return false;
}



