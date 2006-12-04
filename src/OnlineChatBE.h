//
// C++ Interface: onlinechatbe
//
// Description: 
//
//
// Author: upi <upi@apocalypse.rulez.org>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ONLINECHATBE_H
#define ONLINECHATBE_H


#include <string>


class IOnlineEventSink;
struct SOnlineChatBE_P;

/**
This is a backend class which implements communication services with the MortalNet backend.

The backend doesn't provide any frontend or interactivity and is meant to run independently in its own thread.

\author upi
\ingroup Network
*/
class IOnlineChatBE
{
public:
	enum ConnectionStateEnum
	{
		CS_Disconnected,					///< The backend is not connected to the server.
		CS_Connecting,						///< The backend is trying to connect to the server.
		CS_Connected,						///< Connected.
		CS_Disconnecting,
	};
	
	enum ClientModeEnum
	{
		CM_Chatroom,						///< The user is rotting away in the chatroom
		CM_Game,							///< The user is playing a game. He'll be back.
		CM_Away,							///< The user is not at all here, he just left his client running
		CM_WaitingForChallenge,				///< The user is not running the game, but is waiting for a challenge.
	};
	
	enum ChatEventEnum
	{
		CE_Nothing,
		CE_MyNick,							///< You are now known as...
		CE_Challenge,						///< Challenged by user X
		CE_Message,							///< Message from user X
		CE_Joins,							///< User X has joined MortalNet
		CE_Leaves,							///< User X has left MortalNet
		CE_NickChange,						///< User X is now known as Y
		CE_ServerMessage,					///< Miscellaneous info from server
	};
	
	struct UserInfo
	{
		std::string		m_sNick;			///< The name of the user
		ClientModeEnum	m_enClientMode;		///< The client mode of the user
		std::string		m_sHostName;		///< The hostname that is displayed for the user (reverse lookup by MortalNet)
		std::string		m_sHostAddress;		///< The host address string of the user (IPV4 or IPV6 number).
		
		UserInfo();
		UserInfo( const UserInfo& a_roRhs );
		UserInfo& operator=( const UserInfo& a_roRhs );
	};
	
	struct SChatEvent
	{
		ChatEventEnum	m_enEvent;
		UserInfo		m_sUser;
		std::string		m_sMessage;
	};
	
protected:
	
	// Lifecycle control
						IOnlineChatBE() {}
	virtual				~IOnlineChatBE() {}

public:
	static void			create();
	
	// Connection
	
	virtual void				connect( std::string a_sNick ) = 0;
	virtual void				disconnect() = 0;
	virtual ConnectionStateEnum	getConnectionState() const = 0;
	
	// Getting/setting my connection parameters
	
	virtual std::string			getMyNick() const = 0;
	virtual void				setMyNick( std::string a_sNick ) = 0;
	virtual ClientModeEnum		getMyClientMode() const = 0;
	virtual void				setMyClientMode( ClientModeEnum a_enClientMode ) = 0;
	virtual const UserInfo&		getMyUserInfo() const = 0;				///< Same as getUserInfo(0)
	
	// User control
	
	virtual int					getUserCount() const = 0;
	virtual const UserInfo&		getUserInfo( int a_iUserNumber ) const = 0;
	virtual ClientModeEnum		getClientMode() const = 0;
	virtual void				setClientMode( ClientModeEnum a_newMode ) = 0;
	
	// Event sinks
	
	virtual void				addEventSink( IOnlineEventSink* a_poSink ) = 0;
	virtual void				removeEventSink( IOnlineEventSink* a_poSink ) = 0;
	virtual void				removeAllEventSinks() = 0;
};


extern IOnlineChatBE*	g_poChatBE;



/** IBackendEventSink is an interface for receiving events from IOnlineChatBE.
Every backend event sink which is registered to g_poChatBE will be notified of
chat events as they occur. This is done with the receiveEvent() method.
*/

class IOnlineEventSink
{
public:
						IOnlineEventSink() {}
	virtual				~IOnlineEventSink() {}
	
	virtual void		receiveEvent( const IOnlineChatBE::SChatEvent& a_roEvent ) = 0;
	virtual void		connectionStateChange( IOnlineChatBE::ConnectionStateEnum a_enOldState, 
											   IOnlineChatBE::ConnectionStateEnum a_enNewState,
											   const std::string& a_rsMessage ) = 0;
};




#endif
