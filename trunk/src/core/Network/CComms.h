#ifndef _CCOMMS_H
#define _CCOMMS_H

#include "stdafx.h"
#include "CDataPacket.h"

#ifdef HAS_MULTIPLAYER

#define MP_SERVER_PORT	"7564"
#define MP_CLIENT_PORT	"6564"

#define MP_APPLICATION_ID	1111
#define MP_DEVICES			16
#define MP_DATA_BUFFER		10240

#define MP_GAME_TIMEOUT   10000
#define MP_WAITCONNECT_TIMEOUT   10000

// CDeviceDetails
// object store device infomation
class CDeviceDetails
{
public:
	enum EDeviceState
	{
		stateUnknown = 0,
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
    unsigned int    m_lastTimeRespone;
    
	CDeviceDetails()
	{
		m_id = -1;
		m_deviceData = NULL;
		m_address = NULL;
		m_state = stateUnknown;
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

	// adddevice
	int addDevice( CDeviceDetails* dev );

    // getDevice
    inline CDeviceDetails* getDevice( int i )
    {
        return m_devices[i];
    }
    
	// remove device
	void removeDevice( CDeviceDetails* dev );
	void removeDevice( int id );

	// remove all device
	void removeAllDevice();
    void removeAllDeviceNotResponse( unsigned int time );
    
	// getDeviceIdFromAdress
	// return id device
	int getDeviceIdFromAdress( const void *addr );
    
    // updateDevices
    // update devices connected
    void updateDevices();
    
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
    bool initClient(const char *ipServer);
    
	// sendDiscoveryPacket
	// send a packet to find server
	bool sendDiscoveryPacket(const void *data, int size);

    bool sendData(const void *data, int size, int id);
    bool sendData(const void *data, int size, void* addr);
    bool sendDataToAll(const void *data, int size);
    
	// cleanSocket
	// clean up socket data
	void cleanSocket();
};

#endif

#endif