#ifndef _CCOMMS_H
#define _CCOMMS_H

#include "stdafx.h"
#include "CDataPacket.h"

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

class CMultiplayerManager;

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
    
    CMultiplayerManager *m_owner;
    
    struct SDataSend
    {
        void *data;
        int size;
        void *addr;
    };
        
    std::vector<SDataSend>  m_queueSendData;
    
public:
	CComms(CMultiplayerManager* owner, bool isServer, bool isOnline);
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
	
    // updateSendData
    // update per frame to send data
    bool updateSendData();
    
	// initDiscoveryWifi
	bool initDiscoveryWifi();

	// initServer
	// init server
	bool initServer();

	// sendDiscoveryPacket
	// send a packet to find server
	bool sendDiscoveryPacket(const void *data, int size);

    bool sendData(const void *data, int size, int id);
    bool sendData(const void *data, int size, const sockaddr_in& addr);
    
	// cleanSocket
	// clean up socket data
	void cleanSocket();
};

#endif

#endif