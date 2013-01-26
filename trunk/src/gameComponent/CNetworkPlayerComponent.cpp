#include "stdafx.h"
#include "CPlayerComponent.h"
#include "CNetworkPlayerComponent.h"
#include "CGameComponent.h"

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
}

// update
// run when update per frame
void CNetworkPlayerComponent::updateComponent()
{
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
	m_playerMoveEvt.runFast		= (packet->getByte() == 1);

	m_playerCmdEvt.shoot		= (packet->getByte() == 1);
	m_playerCmdEvt.reload		= (packet->getByte() == 1);	

	// unpack player state
	m_subStateMP	= (CPlayerComponent::EPlayerSubState)packet->getByte();
	m_stateMP		= (CPlayerComponent::EPlayerState)packet->getByte();

	m_upbodySubStateMP	= (CPlayerComponent::EPlayerSubState)packet->getByte();
	m_upbodyStateMP		= (CPlayerComponent::EPlayerUpBodyState)packet->getByte();

	// unpack mp state
	unpackDataMPState(packet);
}


void CNetworkPlayerComponent::unpackDataMPState(CDataPacket *packet)
{
	switch( m_stateMP )
	{
	case CPlayerComponent::PlayerIdle:
		unpackDataStateIdle(packet);			
		break;
	case CPlayerComponent::PlayerTurn:
		unpackDataStateTurn(packet);			
		break;
	case CPlayerComponent::PlayerRunTurn:
		unpackDataStateRunTurn(packet);
		break;
	case CPlayerComponent::PlayerRun:
		unpackDataStateRun(packet);
		break;
    case CPlayerComponent::PlayerRunFastTurn:
        unpackDataStateRunFastTurn(packet);    
        break;
	case CPlayerComponent::PlayerRunFast:
		unpackDataStateRunFast(packet);
		break;
    case CPlayerComponent::PlayerRunToRunFast:
        unpackDataStateRunToRunFast(packet);
        break;
    case CPlayerComponent::PlayerRunFastToRun:
        unpackDataStateRunFastToRun(packet);
        break;
    case CPlayerComponent::PlayerStandAim:
        unpackDataStateStandAim(packet);			
        break;
    case CPlayerComponent::PlayerRotate:
        unpackDataStatePlayerRotate(packet);
        break;
	}
}

void CNetworkPlayerComponent::unpackDataStateIdle(CDataPacket *packet)
{
}

void CNetworkPlayerComponent::unpackDataStateTurn(CDataPacket *packet)
{
}

void CNetworkPlayerComponent::unpackDataStateRun(CDataPacket *packet)
{
}

void CNetworkPlayerComponent::unpackDataStateRunTurn(CDataPacket *packet)
{
}

void CNetworkPlayerComponent::unpackDataStateRunFast(CDataPacket *packet)
{
}

void CNetworkPlayerComponent::unpackDataStateRunFastTurn(CDataPacket *packet)
{
}

void CNetworkPlayerComponent::unpackDataStateRunToRunFast(CDataPacket *packet)
{
}

void CNetworkPlayerComponent::unpackDataStateRunFastToRun(CDataPacket *packet)
{
}

void CNetworkPlayerComponent::unpackDataStateStandAim(CDataPacket *packet)
{
}

void CNetworkPlayerComponent::unpackDataStatePlayerRotate(CDataPacket *packet)
{
}
