#include "stdafx.h"
#include "CMultiplayerManager.h"
#include "gameEvent.h"
#include "IView.h"

#ifdef HAS_MULTIPLAYER

static unsigned short s_keyID = 0;

CMultiplayerManager::CMultiplayerManager(bool isServer, bool isOnline, const char *connectIP)
{
	m_comm = new CComms(this, isServer, isOnline);
    
    if ( isServer )
    {
        m_comm->initServer();
        
        // need request from server
        m_keyID = s_keyID++;        
    }
    else
    {
        if ( connectIP == NULL )
            m_comm->initDiscoveryWifi();
        else 
        {
            m_comm->initClient(connectIP);     
        }
        
        // need request from server
        m_keyID = -1;
    }
    
	m_isServer = isServer;
	m_isOnline = isOnline;
    
	// get default name
	char lpName[512];
	gethostname( lpName, 512 );
    m_name = lpName;

    m_gamePacket = new CDataPacket( 1024*1024 );
}

CMultiplayerManager::~CMultiplayerManager()
{
    delete m_gamePacket;
	delete m_comm; 
}

// update
// update networking per frame
void CMultiplayerManager::update()
{
	m_comm->updateRevcData();
	m_comm->updateSendData();
    m_comm->updateDevices();
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

// getDeviceName
// get device infomation
const char *CMultiplayerManager::getDeviceName( int devID )
{
    CDeviceDetails *dev = m_comm->getDevice(devID);
    if ( dev == NULL )
        return NULL;
    
    return dev->m_name.c_str();
}

// getDeviceIp
// get ip of device
const char *CMultiplayerManager::getDeviceIp( int devID )
{
    CDeviceDetails *dev = m_comm->getDevice(devID);
    if ( dev == NULL )
        return NULL;
    
    return inet_ntoa( ((sockaddr_in*)dev->m_address)->sin_addr );
}

const char *CMultiplayerManager::getDeviceIp( void *addr )
{
    return inet_ntoa( ((sockaddr_in*)addr)->sin_addr );    
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


// sendJointGamePacket
// send a packet to joint game
bool CMultiplayerManager::sendJointGamePacket( void *addr )
{
    if ( m_isServer == true )
		return false;
    
	CDataPacket packet(64);
	packet.addByte( (unsigned char) CMultiplayerManager::JointGame );
    packet.addShort( (unsigned short) m_name.length() );
    packet.addData( m_name.c_str(), (int)m_name.length() );    
	packet.packData();
    
	return m_comm->sendData( packet.getMessageData(), packet.getMessageSize(), addr );
}

// sendPingMessage
// send a ping packet to keep connection
bool CMultiplayerManager::sendPingMessage()
{
    CDataPacket packet(64);
	packet.addByte( (unsigned char) CMultiplayerManager::Ping );
	packet.packData();    
	return m_comm->sendData( packet.getMessageData(), packet.getMessageSize(), 0 );
}

// sendReadyGameMessage
bool CMultiplayerManager::sendReadyGameMessage()
{
    CDataPacket packet(64);
	packet.addByte( (unsigned char) CMultiplayerManager::ReadyGame );
	packet.addShort((unsigned short) m_keyID );
	packet.packData();
	return m_comm->sendData( packet.getMessageData(), packet.getMessageSize(), 0 );
}

// sendGetNameMessage
// send get name
bool CMultiplayerManager::sendGetNameMessage()
{
    CDataPacket packet(64);
	packet.addByte( (unsigned char) CMultiplayerManager::GetName );
	packet.packData();
	return m_comm->sendData( packet.getMessageData(), packet.getMessageSize(), 0 );
}

// sendGameDataMessage
// send game data
bool CMultiplayerManager::sendGameDataMessage(CGameLevel *level)
{
    m_gamePacket->resetPacket();
    m_gamePacket->addByte(CMultiplayerManager::GameData);
    m_gamePacket->addShort(m_keyID);
    level->packDataMultiplayer(m_gamePacket);    
    m_gamePacket->packData();    
    
    bool ret = false;
    if ( m_isServer )
        ret = m_comm->sendDataToAll(m_gamePacket->getMessageData(), m_gamePacket->getMessageSize());
    else
        ret = m_comm->sendData( m_gamePacket->getMessageData(), m_gamePacket->getMessageSize(), 0 );
    
    return ret;
}


// onRevcData
// process when revc data
bool CMultiplayerManager::onRevcData( unsigned char *buffer, int size, int devID, void *addr)
{    
    CDataPacket packet(buffer, size);
	if ( packet.checkData() == false )
	{
		const char *lpIp = getDeviceIp(addr);

        char string[512];
		sprintf(string,"- Network warning: %s send data error", lpIp);
        os::Printer::log(string);
        
		return false;
	}

    if ( devID == -1 )
    {
        unsigned char msgType = packet.getByte();
                
        // send event to revc
        static SEvent event;        
        static SEventNetworking eventNetwork;
        
        eventNetwork.eventID    = (int)msgType;
        eventNetwork.deviceID   = devID;
        eventNetwork.data       = &packet;
        
        event.EventType = EET_GAME_EVENT;
        event.GameEvent.EventID = (s32)EvtNetworking;
        event.GameEvent.EventData = &eventNetwork;
        
        getIView()->getDevice()->postEventFromUser( event );
        
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
            case CMultiplayerManager::JointGame:
                {
					if ( m_isServer == false )
						return false;
                    
                    // Response joint game
                    return doMsgJointGame(packet, addr);
                }
                break;
			case CMultiplayerManager::ReadyGame:
                {
                    if ( m_isServer == false )
                        return false;
                    
                    return doMsgClientReadyGame(packet, addr);
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
        unsigned char msgType = packet.getByte();
        
        // send event to revc
        static SEvent event;        
        static SEventNetworking eventNetwork;
        
        eventNetwork.eventID    = (int)msgType;
        eventNetwork.deviceID   = devID;
        eventNetwork.data       = &packet;
        
        event.EventType = EET_GAME_EVENT;
        event.GameEvent.EventID = (s32)EvtNetworking;
        event.GameEvent.EventData = &eventNetwork;
        
        getIView()->getDevice()->postEventFromUser( event );
        
        switch ( msgType ) 
        {            
			case CMultiplayerManager::AcceptJoinGame:
				{
					if ( m_isServer == true )
                        return false;

					// get key from server
					m_keyID = packet.getShort();
				}
				break;
            case CMultiplayerManager::DeclineJoinGame:                          
                {
                    // do nothing...
                }
                break;
            case CMultiplayerManager::GetName:
                {
                    return doMsgGetName(packet, addr, devID);
                }
                break;
            case CMultiplayerManager::ResponseName:
                {
                    return doMsgResponseName(packet, addr, devID);
                }
                break;                
            case CMultiplayerManager::Ping:
                {
                    // todo nothing
                }
                break; 
            case CMultiplayerManager::GameData:
                {
                    // forward data to all client
                    if ( m_isServer == true )
                        m_comm->sendDataToAll(buffer, size, devID);
                }
                break;
            default:
                {
                    return false;
                }
                break;
        };
    }
    
	return true;
}

// doMsgServerDiscovery
// response discovery
bool CMultiplayerManager::doMsgDiscovery( CDataPacket& packet, void *addr)
{
    int appID = packet.getInt();
    const char *lpIp = getDeviceIp(addr);

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
bool CMultiplayerManager::doMsgResponseDiscovery( CDataPacket& packet, void* addr )
{
    const char *lpIp = getDeviceIp(addr);

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
        dev->m_address = new sockaddr_in( *((sockaddr_in*)addr));
        m_comm->addDevice( dev );
    }
    
	return true;
}

// doMsgJointGame
// response joint game to client
bool CMultiplayerManager::doMsgJointGame( CDataPacket& packet, void* addr )
{
    const char *lpIp = getDeviceIp(addr);
    
    if ( m_comm->getDeviceIdFromAdress( &addr ) == -1 )
    {
        int id = m_comm->findDeviceSlot();
        CDataPacket response(64);

        if ( id == -1 )
            response.addByte( (unsigned char) CMultiplayerManager::DeclineJoinGame );
        else
        {
            response.addByte( (unsigned char) CMultiplayerManager::AcceptJoinGame );
            			
            // get name from packet
            char clientName[512] = {0};
            unsigned short lenName = packet.getShort();
            packet.getData(clientName, lenName);
			
			// add keyID
			response.addShort(s_keyID++);

            // add packet
            CDeviceDetails *dev = new CDeviceDetails();
            dev->m_name = clientName;
            dev->m_address = new sockaddr_in( *((sockaddr_in*)addr));
            dev->m_state = CDeviceDetails::stateAskConnection;
			dev->m_deviceData = (void*)((unsigned long)s_keyID - 1);

            m_comm->addDevice( dev );

            char string[512];
            sprintf(string,"- Network warning: client %s will joint game ...", lpIp);
            os::Printer::log(string);
        }
        
        response.packData();
        return m_comm->sendData( response.getMessageData(), response.getMessageSize(), addr);        
    }
    return false;
}

// doMsgReadyGame
bool CMultiplayerManager::doMsgClientReadyGame( CDataPacket& packet, void* addr )
{    
	unsigned short keyID = packet.getShort();
	sockaddr_in* address = (sockaddr_in*)addr;
	
	for ( int i = 0; i < MP_DEVICES; i++ )
	{
		CDeviceDetails* device = m_comm->getDevice(i);

		if ( device && device->m_state == CDeviceDetails::stateAskConnection )
		{
			sockaddr_in* deviceAddress = (sockaddr_in*)device->m_address;
			
			if ( (unsigned long)device->m_deviceData == (unsigned long) keyID && memcmp( &(address->sin_addr), &(deviceAddress->sin_addr), sizeof(deviceAddress->sin_addr) ) == 0 )
			{
				device->m_state = CDeviceDetails::stateConnected;
				
				// change client port
				delete device->m_address;
				device->m_address = new sockaddr_in( *address );

				char string[512];
				sprintf(string,"- Network warning: client id %d ready game ...", i);
				os::Printer::log(string);
				return true;
			}
		}
	}
   
	char string[512];
	sprintf(string,"- Network warning: client connect game game error with keyID: %d...", keyID);
	os::Printer::log(string);

    return false;
}

// doMsgGetName    
bool CMultiplayerManager::doMsgGetName( CDataPacket& packet, void* addr, int devID )
{
    CDataPacket response(64);
    response.addByte( (unsigned char) CMultiplayerManager::ResponseName );
    response.addShort( (unsigned short) m_name.length() );
    response.addData( m_name.c_str(), (int)m_name.length() );
	response.packData();
    
    return m_comm->sendData( response.getMessageData(), response.getMessageSize(), addr);
}

// doMsgResponseName    
bool CMultiplayerManager::doMsgResponseName( CDataPacket& packet, void* addr, int devID )
{
    // get name from packet
    char responseName[512] = {0};
    unsigned short lenName = packet.getShort();
    packet.getData(responseName, lenName);
    
    // set name on device
    m_comm->getDevice(devID)->m_name = responseName;
    return true;
}

#endif