//
// C++ Interface: COnlineChatBEImpl
//
// Description: 
//
//
// Author: upi <upi@feel>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONLINECHATBEIMPL_H
#define CONLINECHATBEIMPL_H


#include "OnlineChatBE.h"
#include "SDL_net.h"
#include "SDL_thread.h"
#include <list>

/**
Implementation of the COnlineChatBE interface
\ingroup Network
\author upi
*/
class COnlineChatBEImpl: public IOnlineChatBE
{
public:
    COnlineChatBEImpl();

    ~COnlineChatBEImpl();


public:
	// Connection
	
	virtual void				connect( std::string a_sNick );
	virtual void				disconnect();
	virtual ConnectionStateEnum	getConnectionState() const;
	
	// Getting/setting my connection parameters
	
	virtual std::string			getMyNick() const;
	virtual void				setMyNick( std::string a_sNick );
	virtual ClientModeEnum		getMyClientMode() const;
	virtual void				setMyClientMode( ClientModeEnum a_enClientMode );
	virtual const UserInfo&		getMyUserInfo() const;				///< Same as getUserInfo(0)
	
	// User control
	
	virtual int					getUserCount() const;
	virtual const UserInfo&		getUserInfo( int a_iUserNumber ) const;
	virtual ClientModeEnum		getClientMode() const;
	virtual void				setClientMode( ClientModeEnum a_enNewMode );
	
	// Event sinks
	
	virtual void				addEventSink( IOnlineEventSink* a_poSink );
	virtual void				removeEventSink( IOnlineEventSink* a_poSink );
	virtual void				removeAllEventSinks();

protected:
	void						notifyEvent( const IOnlineChatBE::SChatEvent& a_roEvent );
	void						notifyConnectionState( IOnlineChatBE::ConnectionStateEnum a_enOldState, 
													   IOnlineChatBE::ConnectionStateEnum a_enNewState,
													   const std::string& a_rsMessage );
	void						notifyConnectionState( const std::string& a_rsMessage );
													   
	void						threadFunction();
	void						internalConnect();
	void						internalDisconnect();
	void						internalProcessMessage();
	void						sendNick();
	void						sendRawData( char a_cPrefix, const char* a_pcMessage );

protected:
	// typedef
	typedef std::list<IOnlineEventSink*> EventSinkList;
	typedef std::list<UserInfo*> UserInfoList;
	
	// Internal state
	ConnectionStateEnum			m_enConnectionState;		// The current state of the connection to the server
	ConnectionStateEnum			m_enNotifiedState;			// The last notified connection state
	ClientModeEnum				m_enClientMode;				// The current state of the client (e.g. "away")
	std::string					m_sMyNick;					// The last received nick from the server
	EventSinkList				m_apoSinks;					// Receivers of events
	UserInfoList				m_apoUsers;					// User list from the server
	
	// Connection to the server
	TCPsocket					m_poSocket;					// The TCP/IP network socket.
	SDLNet_SocketSet			m_poSocketSet;				// SDLNet construct for watching the socket.
	char						m_acIncomingBuffer[4096];	///< Received data goes here.
	int							m_iIncomingBufferSize;		///< How much of the buffer is filled?
	
	SDL_mutex*					m_poLock;
};

#endif
