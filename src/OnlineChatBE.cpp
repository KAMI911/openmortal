//
// C++ Implementation: onlinechatbe
//
// Description: 
//
//
// Author: upi <upi@feel>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
/*
#include "OnlineChatBE.h"
#include "common.h"
#include "SDL_net.h"

class COnlineChatBEImpl: public IOnlineChatBE
{
protected:
	ConnectionStateEnum			m_enConnectionState;

public:
	// Lifecycle control
								COnlineChatBEImpl();
	virtual						~COnlineChatBEImpl() {}

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
	virtual void				setClientMode( ClientModeEnum a_newMode );
	
	// Event sinks
	
	virtual void				addEventSink( IOnlineEventSink* a_poSink );
	virtual void				removeEventSink( IOnlineEventSink* a_poSink );
	virtual void				removeAllEventSinks();
};

COnlineChatBEImpl::COnlineChatBEImpl()
{
	m_enConnectionState = CS_Disconnected;
}

void COnlineChatBEImpl::connect( std::string a_sNick )
{
	if ( m_enConnectionState != CS_Disconnected )
	{
		debug( "COnlineChatBEImpl::connect: Not in disconnected state!\n" );
		return;
	}
	m_enConnectionState = CS_Connecting;

	
}

void COnlineChatBEImpl::disconnect()
{
}
*/