#include "stdafx.h"
#include "CComms.h"

#ifdef HAS_MULTIPLAYER

CComms::CComms(bool isServer, bool isOnline)
{
#if defined(WIN32)
	WSAStartup(MAKEWORD(2,2), &m_wsaData);
#endif

	for  (int i = 0; i < MP_DEVICES; i++ )
		m_devices[i] = NULL;

	m_dataSocket = INVALID_SOCKET;

	m_isOnline = isOnline;
	m_isServer = isServer;
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

// getIdFromAdress
// return id device
int CComms::getDeviceIdFromAdress( void *addr )
{
	const int addrLength = sizeof(struct sockaddr_in);

	for (int i = 0; i < MP_DEVICES; i++)
	{
		if(memcmp(m_devices[i]->m_address, addr, addrLength) == 0)
		{
			return i;
		}
	}

	return -1;
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

		iResult = recvfrom(m_dataSocket, (char *)m_dataBuff, MP_DATA_BUFFER, 0,(struct sockaddr*)&addr,&addrLen);

		if( (iResult == SOCKET_ERROR) || (addrLen != sizeof(struct sockaddr)) )
		{
			int errClId = getDeviceIdFromAdress(&addr);

			if( errClId >= 0)
				removeDevice( errClId );

			// continue read data
			continue;
		}

		int clId = getDeviceIdFromAdress(&addr);

		if(clId == -1 && m_isServer)
		{
			onAcceptConnection( m_dataBuff, iResult, clId );

			// continue read data
			continue;
		}

		if ( onRevcData( m_dataBuff, iResult, clId) < 0 )
		{
			return false;
		}

	}
	return true;
}

// onRevcData
// process when revc data
bool CComms::onRevcData( unsigned char *buffer, int size, int devID )
{
	return true;
}

// onAcceptConnection
// accept connection
bool CComms::onAcceptConnection( unsigned char *buffer, int size, int devID )
{
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
	
	// add myself at slot 0
	CDeviceDetails* dd = new CDeviceDetails();
	dd->setAddress( result->ai_addr );
	addDevice(dd);
	return true;
}

// sendDiscoveryPacket
// send a packet to find server
bool CComms::sendDiscoveryPacket()
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
		return false;
	}

	const int kUIdApp = MP_APPLICATION_ID;

	// send broadcast
	if (sendto(m_dataSocket, (const char*)&kUIdApp, sizeof(kUIdApp), 0, (struct sockaddr *)&addrBroadcast, sizeof(addrBroadcast)) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}


#endif