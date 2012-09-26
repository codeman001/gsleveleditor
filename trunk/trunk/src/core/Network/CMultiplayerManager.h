#ifndef _CMPMANAGER_H_
#define _CMPMANAGER_H_

#include "stdafx.h"

#ifdef HAS_MULTIPLAYER

#include "CComms.h"
#include "CDataPacket.h"

class CMultiplayerManager
{
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
    
protected:
	bool	m_isServer;
	bool	m_isOnline;

	CComms	*m_comm;

    std::string m_name;
    
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

    // removeAllDeviceNotResponse
    // remove device do not response on (time) ms
    inline void removeAllDeviceNotResponse(int time)
    {
        m_comm->removeAllDeviceNotResponse(time);
    }
    
    // getAllActiveDevice
    // get all device
    void getAllActiveDevice( std::vector<CDeviceDetails*>& listDevices );
    
	// sendDiscoveryPacket
	// send a packet to find server
	bool sendDiscoveryPacket();
    
    // onRevcData
	// process when revc data
	bool onRevcData( unsigned char *buffer, int size, int devID, const sockaddr_in& addr );

    // doMsgDiscovery
    // response discovery
    bool doMsgDiscovery( CDataPacket& packet, const sockaddr_in& addr );

	// doMsgResponseDiscovery
    // response discovery
    bool doMsgResponseDiscovery( CDataPacket& packet, const sockaddr_in& addr );
    
};

#endif

#endif