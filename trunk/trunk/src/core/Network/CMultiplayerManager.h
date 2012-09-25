#ifndef _CMPMANAGER_H_
#define _CMPMANAGER_H_

#include "stdafx.h"

#ifdef HAS_MULTIPLAYER

#include "CComms.h"
#include "CDataPacket.h"

class CMultiplayerManager
{
protected:
	bool	m_isServer;
	bool	m_isOnline;

	CComms	*m_comm;

    std::string m_name;
public:
	enum EMPPacketType
	{
		Discovery = 0,
        ResponseDiscovery,
		Connect,
		Reconnect,
		PlayerQuit,
		Kick,
		Chat,
		GamePackaget,
	};
public:
	CMultiplayerManager(bool isServer, bool isOnline);
	virtual ~CMultiplayerManager();

    // setName
    // set name on device
    inline void setName( const char *name )
    {
        m_name = name;
    }
    
	// reInit
	// init discovery on client
	// init server
	bool reInit(bool isServer, bool isOnline);

	// update
	// update networking per frame
	void update();

	// sendDiscoveryPacket
	// send a packet to find server
	bool sendDiscoveryPacket();
    
    // onRevcData
	// process when revc data
	bool onRevcData( unsigned char *buffer, int size, int devID, const sockaddr_in& addr );

    // doMsgServerDiscovery
    // response discovery
    bool doMsgServerDiscovery( CDataPacket& packet, const sockaddr_in& addr );
};

#endif

#endif