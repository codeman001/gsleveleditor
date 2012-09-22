#include "stdafx.h"
#include "CMultiplayerManager.h"

#ifdef HAS_MULTIPLAYER

CMultiplayerManager::CMultiplayerManager(bool isServer, bool isOnline)
{
	m_comm = new CComms(isServer,isOnline);

	m_isServer = isServer;
	m_isOnline = isOnline;
}

CMultiplayerManager::~CMultiplayerManager()
{
	delete m_comm; 
}


bool CMultiplayerManager::init()
{
	if ( m_isServer )
		return m_comm->initServer();

	return m_comm->initDiscoveryWifi();
}

// update
// update networking per frame
void CMultiplayerManager::update()
{
	m_comm->updateRevcData();
}

// sendDiscoveryPacket
// send a packet to find server
bool CMultiplayerManager::sendDiscoveryPacket()
{
	if ( m_isServer == true )
		return false;

	return m_comm->sendDiscoveryPacket();
}

#endif