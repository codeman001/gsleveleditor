#ifndef _CCOMMS_H
#define _CCOMMS_H

#include "stdafx.h"

#ifdef HAS_MULTIPLAYER

#define MP_SERVER_PORT	"7564"
#define MP_CLIENT_PORT	"6564"

#define MP_APPLICATION_ID	123981
#define MP_DEVICES			16
#define MP_DATA_BUFFER		10240

// CDeviceDetails
// object store device infomation
class CDeviceDetails
{
public:
	enum EDeviceState
	{
		stateNotConnect = 0,
		stateAskConnection,
		stateConnected,
		stateWaitReconect,
	};
public:
	std::string		m_name;
	int				m_id;
	void*			m_deviceData;
	void*			m_address;
	EDeviceState	m_state;

	CDeviceDetails()
	{
		m_id = -1;
		m_deviceData = NULL;
		m_address = NULL;
		m_state = stateAskConnection;
	}

	void setAddress(sockaddr *addr)
	{
		m_address = new sockaddr( *addr );
	}

	virtual ~CDeviceDetails()
	{
		if ( m_address )
			delete m_address;
	}
};

// CComms
// object will send/revc data to networks
class CComms
{
protected:

#if defined(WIN32)
	WSADATA m_wsaData;
#endif

	SOCKET	m_dataSocket;

	bool	m_isServer;
	bool	m_isOnline;

	CDeviceDetails*		m_devices[MP_DEVICES];

	unsigned char		m_dataBuff[MP_DATA_BUFFER];
public:
	CComms(bool isServer, bool isOnline);
	virtual ~CComms();

	// findDeviceSlot
	int findDeviceSlot();

	// add device
	int addDevice( CDeviceDetails* dev );

	// remove device
	void removeDevice( CDeviceDetails* dev );
	void removeDevice( int id );

	// remove all device
	void removeAllDevice();

	// getDeviceIdFromAdress
	// return id device
	int getDeviceIdFromAdress( void *addr );

	// updateRevcData
	// update per frame to revc data
	bool updateRevcData();

	// acceptConnection
	// accept connection
	bool onAcceptConnection( unsigned char *buffer, int size, int devID );

	// onRevcData
	// process when revc data
	bool onRevcData( unsigned char *buffer, int size, int devID );

	// initDiscoveryWifi
	bool initDiscoveryWifi();

	// initServer
	// init server
	bool initServer();

	// sendDiscoveryPacket
	// send a packet to find server
	bool sendDiscoveryPacket();

	// cleanSocket
	// clean up socket data
	void cleanSocket();
};

#endif

#endif