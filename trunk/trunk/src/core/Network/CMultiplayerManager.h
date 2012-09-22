#ifndef _CMPMANAGER_H_
#define _CMPMANAGER_H_

#include "stdafx.h"
#include "CComms.h"

#ifdef HAS_MULTIPLAYER

class CMultiplayerManager
{
protected:
	bool	m_isServer;
	bool	m_isOnline;

	CComms	*m_comm;
public:
	CMultiplayerManager(bool isServer, bool isOnline);
	virtual ~CMultiplayerManager();

	// init
	// init discovery on client
	// init server
	bool init();

	// update
	// update networking per frame
	void update();

	// sendDiscoveryPacket
	// send a packet to find server
	bool sendDiscoveryPacket();
};

#endif

#endif