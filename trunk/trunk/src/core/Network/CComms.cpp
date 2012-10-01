#include "stdafx.h"
#include "CComms.h"

#ifdef HAS_MULTIPLAYER

#include "CMultiplayerManager.h"

CComms::CComms(CMultiplayerManager* owner, bool isServer, bool isOnline)
{
#if defined(WIN32)
	WSAStartup(MAKEWORD(2,2), &m_wsaData);
#endif

	for  (int i = 0; i < MP_DEVICES; i++ )
		m_devices[i] = NULL;

	m_dataSocket = INVALID_SOCKET;

	m_isOnline = isOnline;
	m_isServer = isServer;
    
    m_owner = owner;
}

CComms::~CComms()
{
	cleanSocket();

#if defined(WIN32)
	WSACleanup();
#endif
}

// cleanSocket
// clean up socket data
void CComms::cleanSocket()
{
	removeAllDevice();

	if ( m_dataSocket != INVALID_SOCKET )
	{
#ifdef WIN32
		closesocket( m_dataSocket );
#else
		close( m_dataSocket );
#endif
		m_dataSocket = INVALID_SOCKET;
	}
}


// findDeviceSlot
int CComms::findDeviceSlot()
{
	for ( int i = 0; i < MP_DEVICES; i++ )
	{
		if ( m_devices[i] == NULL )
			return i;
	}
	return -1;
}

// add device
int CComms::addDevice( CDeviceDetails* dev )
{
	int id = findDeviceSlot();
	if ( id == -1 )
		return -1;

	m_devices[id] = dev;
	m_devices[id]->m_id = id;
    m_devices[id]->m_lastTimeRespone = os::Timer::getRealTime();
    
	return id;
}

// remove device
void CComms::removeDevice( CDeviceDetails* dev )
{
	m_devices[ dev->m_id ] = NULL;
	delete dev;
}

void CComms::removeDevice( int id )
{
	if ( m_devices[ id ] )
		delete m_devices[ id ];
	m_devices[ id ] = NULL;
}

// remove all device
void CComms::removeAllDevice()
{
	for ( int i = 0; i < MP_DEVICES; i++ )
	{
		if ( m_devices[i] != NULL )
			delete m_devices[i];
		m_devices[i] = NULL;
	}
}

// removeAllDeviceNotResponse
// remove all device if not response
void CComms::removeAllDeviceNotResponse( unsigned int time )
{
    unsigned int currentTime = os::Timer::getRealTime();
    for ( int i = 0; i < MP_DEVICES; i++ )
	{
		if ( m_devices[i] != NULL && currentTime - m_devices[i]->m_lastTimeRespone > time )
        {
			delete m_devices[i];
            m_devices[i] = NULL;
            
            char string[512];
            sprintf(string, "- Network warning: remove device id: %d because donot response in %d ms",i, time);
            os::Printer::log(string);
        }
	}
}

// getIdFromAdress
// return id device
int CComms::getDeviceIdFromAdress( const void *addr )
{
	const int addrLength = sizeof(struct sockaddr_in);
    sockaddr_in *hostAddr = (sockaddr_in*)addr;

    
	for (int i = 0; i < MP_DEVICES; i++)
	{
		if( m_devices[i] )
        {
            sockaddr_in *devAddr = (sockaddr_in*)m_devices[i]->m_address;
            
            // compare port && address
            if ( devAddr->sin_port == hostAddr->sin_port && 
                memcmp(&(devAddr->sin_addr), &(hostAddr->sin_addr), sizeof(hostAddr->sin_addr)) == 0 )
            {
                return i;
            }
		}
	}

	return -1;
}

// updateDevices
// update devices connected
void CComms::updateDevices()
{
    unsigned int currentTime = os::Timer::getRealTime();
    
    for ( int j = 0; j < MP_DEVICES; j++ )
    {                
        if ( m_devices[j] != NULL )
        {
            unsigned int lastResponse = currentTime - m_devices[j]->m_lastTimeRespone;
            
            if ( m_devices[j]->m_state == CDeviceDetails::stateAskConnection && lastResponse > MP_WAITCONNECT_TIMEOUT )
            {
                delete m_devices[j];
                m_devices[j] = NULL;
                
                char string[512];
                sprintf(string, "- Network warning: remove device id %d because do not joint game", j);
                os::Printer::log(string);                
            }            
            else if ( m_devices[j]->m_state == CDeviceDetails::stateConnected && lastResponse > MP_GAMELAG_TIMEOUT )
            {
                m_devices[j]->m_state = CDeviceDetails::stateLag;
            }
            else if ( lastResponse > MP_GAME_TIMEOUT )
            {
                delete m_devices[j];
                m_devices[j] = NULL;
                
                char string[512];
                sprintf(string, "- Network warning: device id %d disconected", j);
                os::Printer::log(string);  
            }

        }
    }
}

// updateRevcData
// update per frame to revc data
bool CComms::updateRevcData()
{
	int iResult;

	fd_set setR;
	timeval tval = {0, 0};
	FD_ZERO(&setR);
	FD_SET(m_dataSocket, &setR);

	while( select(m_dataSocket+1, &setR, NULL, NULL, &tval) > 0 )
	{

		struct sockaddr_in addr;
		socklen_t addrLen  = sizeof( addr );

		iResult = recvfrom(m_dataSocket, (char *)m_dataBuff, MP_DATA_BUFFER, 0,(struct sockaddr*)&addr, &addrLen);
        
		if( (iResult == SOCKET_ERROR) || (addrLen != sizeof(struct sockaddr)) )
		{
			int errClId = getDeviceIdFromAdress(&addr);

			if( errClId >= 0)
				removeDevice( errClId );
            
			// continue read data
			continue;
		}
		
		int clId = getDeviceIdFromAdress(&addr);        
        //int port = ntohs( addr.sin_port );
        //char *ip = inet_ntoa( addr.sin_addr );
        
        // add lasttime response
        if ( clId >= 0 )
		{
			// update last time
            m_devices[clId]->m_lastTimeRespone = os::Timer::getRealTime();
			
			// change state if is lagging
			if ( m_devices[clId]->m_state == CDeviceDetails::stateLag )
				m_devices[clId]->m_state = CDeviceDetails::stateConnected;
		}

		if ( m_owner )
            m_owner->onRevcData( m_dataBuff, iResult, clId, &addr);
		
	}
	return true;
}


// updateSendData
// update per frame to send data
bool CComms::updateSendData()
{
    if ( m_queueSendData.size() == 0 )
        return true;
    
    timeval tval = {1, 0};
    
    fd_set setW;
    FD_ZERO(&setW);
    FD_SET(m_dataSocket, &setW);
    
    int iResult = select(m_dataSocket + 1, NULL, &setW, NULL, &tval);
    if (iResult == SOCKET_ERROR)
    {
        return false;
    }
    
    if (iResult == 0) //this means in 1 second no socket was free for writing
    {
        //if after 1 second the socket is not free, it means the network has a problem, so disconnect      
        return false;
    }
    
    int addrLen  = sizeof(struct sockaddr);
    
    for ( int i = 0, n = m_queueSendData.size(); i < n; i++ )
    {
        SDataSend &dataPak = m_queueSendData[i];
        
        if ( dataPak.addr == NULL )
        {
            // send data to all...
            for ( int j = 0; j < MP_DEVICES; j++ )
            {
                CDeviceDetails *dev = m_devices[j];                            
                
                if ( dev != NULL && (dev->m_state == CDeviceDetails::stateConnected || dev->m_state == CDeviceDetails::stateLag) && dataPak.idIgnore != j )
                {
                    // send data...
                    iResult = sendto(m_dataSocket, (const char *)dataPak.data, dataPak.size, 0,(struct sockaddr*)m_devices[j]->m_address, addrLen);		
                    if (iResult == SOCKET_ERROR)
                    {
                        removeDevice(j);

                        char string[512];
                        sprintf(string, "- Network warning: send data to device %d error", j);
                        os::Printer::log(string);                        
                    }
                }
            }
        }
        else
        {
            // send data...
            iResult = sendto(m_dataSocket, (const char *)dataPak.data, dataPak.size, 0,(struct sockaddr*)dataPak.addr, addrLen);		
            if (iResult == SOCKET_ERROR)
            {
                int devID = getDeviceIdFromAdress( dataPak.addr );

                if ( devID != -1 )
                {                                       
                    removeDevice(devID);                
                }
                
                char string[512];
                sprintf(string, "- Network warning: send data to device %d error", devID);
                os::Printer::log(string); 
            }
        }
                
        delete dataPak.data;
        
        if ( dataPak.addr != NULL )
            delete dataPak.addr;
        
    }

    m_queueSendData.clear();    
    return true;
}


// initDiscoveryWifi
bool CComms::initDiscoveryWifi()
{
	if ( m_isServer )
		return false;

	int iResult;
	struct addrinfo *result = NULL, hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family		=	AF_INET;
	hints.ai_socktype	=	SOCK_DGRAM;
	hints.ai_protocol	=	IPPROTO_UDP;
	hints.ai_flags		=	AI_PASSIVE;

	iResult = getaddrinfo(NULL, MP_CLIENT_PORT, &hints, &result);
	if (iResult != 0)
	{
		return false;
	}

	m_dataSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_dataSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		return false;
	}
	
	int bOptVal = 1;	
	if (setsockopt(m_dataSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, sizeof(int)) == SOCKET_ERROR)
	{
		cleanSocket();
		return false;
	}

	freeaddrinfo(result);
	return true;
}

bool CComms::initClient(const char *ipServer)
{
    if ( m_isServer )
        return false;
    
    int iResult;
	struct addrinfo *result = NULL, hints;
    
	memset(&hints, 0, sizeof(hints));
	hints.ai_family		=	AF_INET;
	hints.ai_socktype	=	SOCK_DGRAM;
	hints.ai_protocol	=	IPPROTO_UDP;
	hints.ai_flags		=	AI_PASSIVE;
    
	iResult = getaddrinfo(NULL, MP_CLIENT_PORT, &hints, &result);
	if (iResult != 0)
	{
		return false;
	}
    
	m_dataSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    
	if (m_dataSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		return false;
	}
    
    freeaddrinfo(result);
    	
    sockaddr_in* connectAddress = new sockaddr_in();
    memset(connectAddress, 0, sizeof(sockaddr_in));
    
    connectAddress->sin_family      = AF_INET;
    connectAddress->sin_port        = htons(atoi(MP_SERVER_PORT));                    
    connectAddress->sin_addr.s_addr = inet_addr(ipServer);
	
    // add server at slot 0
    CDeviceDetails *device = new CDeviceDetails();
    device->m_state = CDeviceDetails::stateConnected;
    device->m_address = connectAddress;
    addDevice( device );
    
    return true;
}

// initServer
// init server
bool CComms::initServer()
{
	if ( m_isServer == false )
		return false;

	int iResult;
	struct addrinfo *result = NULL, hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family		=	AF_INET;
	hints.ai_socktype	=	SOCK_DGRAM;
	hints.ai_protocol	=	IPPROTO_UDP;
	hints.ai_flags		=	AI_PASSIVE;

	iResult = getaddrinfo(NULL, MP_SERVER_PORT, &hints, &result);
	if (iResult != 0)
	{
		return false;
	}

	m_dataSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_dataSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		return false;
	}

	iResult = bind(m_dataSocket, result->ai_addr, result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		cleanSocket();
		freeaddrinfo(result);
		return false;
	}
    
	return true;
}

// sendDiscoveryPacket
// send a packet to find server
bool CComms::sendDiscoveryPacket(const void *data, int size)
{
	if ( m_isServer )
		return false;


	struct sockaddr_in addrBroadcast;
		
	memset(&addrBroadcast, 0, sizeof(addrBroadcast));

	addrBroadcast.sin_family      = AF_INET;
	addrBroadcast.sin_port        = htons(atoi(MP_SERVER_PORT));

	if( m_isOnline == false )
	{
		// local multiplayer
		addrBroadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	}
	else
	{
		// online multiplayer
		// todo later
		return false;
	}

	// send broadcast
	if (sendto(m_dataSocket, (const char*)data, size, 0, (struct sockaddr *)&addrBroadcast, sizeof(addrBroadcast)) == SOCKET_ERROR)
	{
		return false;
	}

	printf("Send discovery packet: %d bytes \n", size);
	return true;
}

// sendData
// push data to send queue
bool CComms::sendData(const void *data, int size, int id)
{
    if ( id < 0 || id >= MP_DEVICES || m_devices[id] == NULL )
        return false;
    
    return sendData(data, size, m_devices[id]->m_address);
}

// sendData
// push data to send queue
bool CComms::sendData(const void *data, int size, void* addr)
{
    SDataSend dataPak;
    
    dataPak.data = new unsigned char[size];
    memcpy(dataPak.data, data, size);
    dataPak.size = size;
    dataPak.addr = new sockaddr_in( *((sockaddr_in*)addr) );
    
    m_queueSendData.push_back( dataPak );
    return true;
}

// sendDataToAll
// push data to send queue
bool CComms::sendDataToAll(const void *data, int size, int idIgnore)
{
    SDataSend dataPak;
    
    dataPak.data = new unsigned char[size];
    memcpy(dataPak.data, data, size);
    dataPak.size = size;
    dataPak.addr = NULL;
    dataPak.idIgnore = idIgnore;
    
    m_queueSendData.push_back( dataPak );
    return true; 
}

#endif