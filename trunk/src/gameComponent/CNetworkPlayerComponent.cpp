#include "stdafx.h"
#include "CPlayerComponent.h"
#include "CNetworkPlayerComponent.h"
#include "CGameComponent.h"

#include "IView.h"

CNetworkPlayerComponent::CNetworkPlayerComponent(CGameObject* obj)
	:IObjectComponent(obj, CGameComponent::NetworkPlayerComponent)
{
}

CNetworkPlayerComponent::~CNetworkPlayerComponent()
{
}

// init
// run when init object
void CNetworkPlayerComponent::initComponent()
{
    m_collada = (CColladaMeshComponent*)m_gameObject->getComponent( IObjectComponent::ColladaMesh );
    
	init(m_gameObject);
}

// update
// run when update per frame
void CNetworkPlayerComponent::updateComponent()
{
    if ( m_collada == NULL )
        return;
    
    if ( m_gameObject->isNetworkController() == false )
        return;
    
	// update
	CBasePlayerState::update();

    updateState();
}

// saveData
// save data to serializable
void CNetworkPlayerComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( CGameComponent::s_compType[ m_componentID ] );
}

// loadData
// load data to serializable
void CNetworkPlayerComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
}


// unpackDataFromPlayerComponent
// sync mp gameplay
void CNetworkPlayerComponent::unpackDataFromPlayerComponent( CDataPacket* packet )
{
	// unpack player control
	m_playerMoveEvt.rotate		= packet->getFloat();
	m_playerMoveEvt.strength	= packet->getFloat();
	m_playerMoveEvt.run			= (packet->getByte() == 1);	

	m_playerCmdEvt.shoot		= (packet->getByte() == 1);
	m_playerCmdEvt.reload		= (packet->getByte() == 1);	
    
	// unpack player state
	m_subStateMP	= (CPlayerComponent::EPlayerSubState)packet->getByte();
	m_stateMP		= (CPlayerComponent::EPlayerState)packet->getByte();

	m_upbodySubStateMP	= (CPlayerComponent::EPlayerSubState)packet->getByte();
	m_upbodyStateMP		= (CPlayerComponent::EPlayerUpBodyState)packet->getByte();
    
    
	// unpack mp state
	unpackDataMPState(packet);
    unpackDataMPUpBodyState(packet);
    
    // change state
    if ( m_state != m_stateMP )
        setState(m_stateMP);
    
    // change upbody state
    if ( m_upbodyState != m_upbodyStateMP )
        setUpBodyState(m_upbodyStateMP);
}


void CNetworkPlayerComponent::unpackDataMPState(CDataPacket *packet)
{
	
}

void CNetworkPlayerComponent::unpackDataMPUpBodyState(CDataPacket *packet)
{
    
}

void CNetworkPlayerComponent::updateState()
{
    
}