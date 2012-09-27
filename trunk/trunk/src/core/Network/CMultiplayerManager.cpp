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
    
	// get default name
	char lpName[512];
	gethostname( lpName, 512 );
    m_name = lpName;
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

// getAllActiveDevice
// get all device
void CMultiplayerManager::getAllActiveDevice( std::vector<CDeviceDetails*>& listDevices )
{
    listDevices.clear();
    
    for (int i = 0; i < MP_DEVICES; i++ )
    {
        CDeviceDetails *dev = m_comm->getDevice(i);
        if ( dev )
            listDevices.push_back(dev);
    }
}


// sendDiscoveryPacket
// send a packet to find server
bool CMultiplayerManager::sendDiscoveryPacket()
{
	if ( m_isServer == true )
		return false;

	CDataPacket packet(64);
	packet.addByte( (unsigned char) CMultiplayerManager::Discovery );
	packet.addInt( MP_APPLICATION_ID );
	packet.packData();

	return m_comm->sendDiscoveryPacket( packet.getMessageData(), packet.getMessageSize() );
}

// onRevcData
// process when revc data
bool CMultiplayerManager::onRevcData( unsigned char *buffer, int size, int devID, const sockaddr_in& addr)
{
    CDataPacket packet(buffer, size);
	if ( packet.checkData() == false )
	{
		char *lpIp = inet_ntoa(addr.sin_addr);

        char string[512];
		sprintf(string,"- Network warning: %s send data error", lpIp);
        os::Printer::log(string);
        
		return false;
	}

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
        char string[512];
        sprintf(string,"- Network warning: %s doMsgServerDiscovery false...", lpIp);
        os::Printer::log(string);
        
        return false;
	}

    CDataPacket response(64);
    response.addByte( (unsigned char) CMultiplayerManager::ResponseDiscovery );
    response.addShort( (unsigned short) m_name.length() );
    response.addData( m_name.c_str(), (int)m_name.length() );
	response.packData();

    return m_comm->sendData( response.getMessageData(), response.getMessageSize(), addr);
}

// doMsgResponseDiscovery
// response discovery
bool CMultiplayerManager::doMsgResponseDiscovery( CDataPacket& packet, const sockaddr_in& addr )
{
	char *lpIp = inet_ntoa(addr.sin_addr);

    char serverName[512] = {0};
	unsigned short lenName = packet.getShort();
    packet.getData(serverName, lenName);
    
    if ( m_comm->getDeviceIdFromAdress( &addr ) == -1 )
    {
        char string[512];
        sprintf(string,"- Network warning: detected server %s ...", lpIp);
        os::Printer::log(string);
        
        CDeviceDetails *dev = new CDeviceDetails();
        dev->m_name = serverName;
        dev->m_address = new sockaddr_in(addr);
        m_comm->addDevice( dev );
    }
    
	return true;
}

#endif