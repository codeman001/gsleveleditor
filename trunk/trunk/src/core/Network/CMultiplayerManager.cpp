#include "stdafx.h"
#include "CMultiplayerManager.h"

#ifdef HAS_MULTIPLAYER

CMultiplayerManager::CMultiplayerManager(bool isServer, bool isOnline)
{
	m_comm = new CComms(this, isServer, isOnline);
    
    if ( isServer )
        m_comm->initServer();
    else
        m_comm->initDiscoveryWifi();
    
	m_isServer = isServer;
	m_isOnline = isOnline;
    
    m_name = "noName";
}

CMultiplayerManager::~CMultiplayerManager()
{
	delete m_comm; 
}


bool CMultiplayerManager::reInit(bool isServer, bool isOnline)
{
    if ( m_comm )
        delete m_comm;
    
    m_isServer = isServer;
    m_isOnline = isOnline;
    
    m_comm = new CComms(this, isServer, isOnline);
    
	if ( isServer )
		return m_comm->initServer();

	return m_comm->initDiscoveryWifi();
}

// update
// update networking per frame
void CMultiplayerManager::update()
{
	m_comm->updateRevcData();
	m_comm->updateSendData();
}

// sendDiscoveryPacket
// send a packet to find server
bool CMultiplayerManager::sendDiscoveryPacket()
{
	if ( m_isServer == true )
		return false;

	CDataPacket packet(10);
	packet.addByte( (unsigned char) CMultiplayerManager::Discovery );
	packet.addInt( MP_APPLICATION_ID );

	return m_comm->sendDiscoveryPacket( packet.getMessageData(), packet.getMessageSize() );
}

// onRevcData
// process when revc data
bool CMultiplayerManager::onRevcData( unsigned char *buffer, int size, int devID, const sockaddr_in& addr)
{
    CDataPacket packet(buffer, size);
    
    if ( devID == -1 )
    {
        unsigned char msgType = packet.getByte();
        switch ( msgType ) 
        {
            case CMultiplayerManager::Discovery:
				{
					if ( m_isServer == false )
						return false;

					// Response to client
					return doMsgDiscovery(packet, addr);
				}
			case CMultiplayerManager::ResponseDiscovery:
				{
					if ( m_isServer == true )
						return false;
					
					// Add server
					return doMsgResponseDiscovery(packet, addr);
				}
            break;
            default:
				{
					return false;
				}
            break;
        }
    }
    else
    {
        // update data revc
    }
    
	return true;
}

// doMsgServerDiscovery
// response discovery
bool CMultiplayerManager::doMsgDiscovery( CDataPacket& packet, const sockaddr_in& addr)
{
    int appID = packet.getInt();
	char *lpIp = inet_ntoa(addr.sin_addr);

    if ( appID != MP_APPLICATION_ID )
	{
		printf("Network warning: %s doMsgServerDiscovery false...\n", lpIp);
        return false;
	}

	printf("- Network: doMsgDiscovery, response to client %s\n", lpIp);

    CDataPacket response(64);
    response.addByte( (unsigned char) CMultiplayerManager::ResponseDiscovery );
    response.addShort( (unsigned short) m_name.length() );
    response.addData( m_name.c_str(), (int)m_name.length() );
 
    return m_comm->sendData( response.getMessageData(), response.getMessageSize(), addr);
}

// doMsgResponseDiscovery
// response discovery
bool CMultiplayerManager::doMsgResponseDiscovery( CDataPacket& packet, const sockaddr_in& addr )
{
	char *lpIp = inet_ntoa(addr.sin_addr);
	printf("- Network: doMsgResponseDiscovery, add server %s\n", lpIp);
	

	return true;
}

#endif