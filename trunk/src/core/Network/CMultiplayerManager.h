#ifndef _CMPMANAGER_H_
#define _CMPMANAGER_H_

#include "stdafx.h"

#ifdef HAS_MULTIPLAYER

#include "CComms.h"
#include "CDataPacket.h"
#include "gameLevel/CGameLevel.h"

class CMultiplayerManager
{
public:
	enum EMPPacketType
	{
		Discovery = 0,
        ResponseDiscovery,
        Ping,
		JointGame,
        AcceptJoinGame,
        DeclineJoinGame,
        ReadyGame,
        GetName,
        ResponseName,
		Reconnect,
		PlayerQuit,
		Kick,
		Chat,
		GameData
	};
    
protected:
	bool	m_isServer;
	bool	m_isOnline;

	CComms	*m_comm;

    std::string m_name;
    
	short           m_keyID;
    CDataPacket*    m_gamePacket;
    
    bool            m_needSyncData;
    float           m_syncTime;
public:
	CMultiplayerManager(bool isServer, bool isOnline, const char *connectIP = NULL);
	virtual ~CMultiplayerManager();

    // setName
    // set name on device
    inline void setName( const char *name )
    {
        m_name = name;
    }

	// update
	// update networking per frame
	void update();

    // removeAllDeviceNotResponse
    // remove device do not response on (time) ms
    inline void removeAllDeviceNotResponse(int time)
    {
        m_comm->removeAllDeviceNotResponse(time);
    }
    
    // getDeviceName
    // get device infomation
    const char *getDeviceName( int devID );
    const char *getDeviceIp( int devID );
    const char *getDeviceIp( void *addr );
    
    // getAllActiveDevice
    // get all device
    void getAllActiveDevice( std::vector<CDeviceDetails*>& listDevices );
    
	// getKeyID
	// get key to connect
	inline short getKeyID()
	{
		return m_keyID;
	}

	// setKeyID
	// set server connect key
	inline void setKeyID(short id)
	{
		m_keyID = id;
	}

	// isServer
	// check if host is server
	inline bool isServer()
	{
		return m_isServer;
	}
    
    // needSyncData
    // return true if this client need send data to another
    inline bool needSyncData()
    {
        return m_needSyncData;
    }
    
    // setSyncData
    // tell the network to sync data
    void setSyncData(bool b)
    {
        m_needSyncData = b;
        m_syncTime = 1000.0f/(float)MP_GAMEFPS_SYNC;
    }
public:
    
	// sendDiscoveryPacket
	// send a packet to find server
	bool sendDiscoveryPacket();
    
    // sendJointGamePacket
    // send a packet to joint game
    bool sendJointGamePacket( void *addr );
    
    // sendPingMessage
    // send a ping packet to keep connection
    bool sendPingMessage();
    
    // sendReadyGameMessage
    // send a message begin game
    bool sendReadyGameMessage();
    
    // sendGetNameMessage
    // send get name
    bool sendGetNameMessage();
    
    // sendGameDataMessage
    // send game data
    bool sendGameDataMessage(CGameLevel *level);
    
	// sendPlayerQuit	
	bool sendPlayerQuit(short hostID);

    // onRevcData
	// process when revc data
	bool onRevcData( unsigned char *buffer, int size, int devID, void *addr );

protected:    
    
    // doMsgDiscovery
    // response discovery
    bool doMsgDiscovery( CDataPacket& packet, void *addr );

	// doMsgResponseDiscovery
    // response discovery
    bool doMsgResponseDiscovery( CDataPacket& packet, void *addr );
    
    // doMsgJointGame
    // response joint game to client
    bool doMsgJointGame( CDataPacket& packet, void* addr );
    
    // doMsgReadyGame
    bool doMsgClientReadyGame( CDataPacket& packet, void* addr );
    
    // doMsgGetName    
    bool doMsgGetName( CDataPacket& packet, void* addr, int devID );

    // doMsgResponseName    
    bool doMsgResponseName( CDataPacket& packet, void* addr, int devID );	
};

#endif

#endif