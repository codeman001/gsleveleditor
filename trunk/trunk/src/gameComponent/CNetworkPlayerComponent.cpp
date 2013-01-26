#include "stdafx.h"
#include "CPlayerComponent.h"
#include "CNetworkPlayerComponent.h"
#include "CGameComponent.h"

#include "IView.h"

CNetworkPlayerComponent::CNetworkPlayerComponent(CGameObject* obj)
	:IObjectComponent(obj, CGameComponent::NetworkPlayerComponent)
{
    m_animCurrentTime = 0.0f;
}

CNetworkPlayerComponent::~CNetworkPlayerComponent()
{
}

// init
// run when init object
void CNetworkPlayerComponent::initComponent()
{
    m_playerComp = (CPlayerComponent*)m_gameObject->getComponent(CGameComponent::PlayerComponent);
    m_collada = (CColladaMeshComponent*)m_gameObject->getComponent(IObjectComponent::ColladaMesh);
    
    
    m_state = CPlayerComponent::PlayerIdle;
    m_subState = CPlayerComponent::SubStateInit;
}

// update
// run when update per frame
void CNetworkPlayerComponent::updateComponent()
{
    if ( m_playerComp == NULL || m_collada == NULL )
        return;
    
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

void CNetworkPlayerComponent::updateState()
{
    switch( m_state )
	{
        case CPlayerComponent::PlayerIdle:
            updateStateIdle();			
            break;
        case CPlayerComponent::PlayerTurn:
            updateStateTurn();			
            break;
        case CPlayerComponent::PlayerRunTurn:
            updateStateRunTurn();
            break;
        case CPlayerComponent::PlayerRun:
            updateStateRun();
            break;
        case CPlayerComponent::PlayerRunFastTurn:
            updateStateRunFastTurn();    
            break;
        case CPlayerComponent::PlayerRunFast:
            updateStateRunFast();
            break;
        case CPlayerComponent::PlayerRunToRunFast:
            updateStateRunToRunFast();
            break;
        case CPlayerComponent::PlayerRunFastToRun:
            updateStateRunFastToRun();
            break;
        case CPlayerComponent::PlayerStandAim:
            updateStateStandAim();			
            break;
        case CPlayerComponent::PlayerRotate:
            updateStatePlayerRotate();
            break;
	}
    
	// update body
	// updateUpperBody();
}


void CNetworkPlayerComponent::updateStateIdle()
{
    stepAnimationTime();
    
	if ( m_subState == CPlayerComponent::SubStateInit )
	{
        int r = 0;
        const char *anim = m_playerComp->m_animIdle[r].c_str();
        
		// change idle animation
        m_collada->setCrossFadeAnimation( anim, 0, 10.0f, false, 1 );
        m_animCurrentTime = m_collada->getCurrentAnimTimeLength();
        
		m_subState = CPlayerComponent::SubStateActive;
	}
	else if ( m_subState == CPlayerComponent::SubStateEnd )
	{
		doNextState();
	}
	else
	{
		// calc spine rotation
		//core::vector3df lookPos = m_gameObject->getPosition() + m_gameObject->getFront();
		//setSpineLookAt( lookPos, 1.0f );
        
		if ( m_runCommand )
			setState( CPlayerComponent::PlayerTurn );			
		else if ( m_gunOnCommand )
        {
			setState( CPlayerComponent::PlayerRotate );            
            m_gunOnCommand = false;
        }
        
        // reinit state
		if ( m_animCurrentTime <= 0 )
			m_subState = CPlayerComponent::SubStateInit;
	}
}

void CNetworkPlayerComponent::unpackDataStateIdle(CDataPacket *packet)
{
}



void CNetworkPlayerComponent::updateStateTurn()
{
}


void CNetworkPlayerComponent::unpackDataStateTurn(CDataPacket *packet)
{
}


void CNetworkPlayerComponent::updateStateRun()
{
}

void CNetworkPlayerComponent::unpackDataStateRun(CDataPacket *packet)
{
}




void CNetworkPlayerComponent::updateStateRunTurn()
{
}

void CNetworkPlayerComponent::unpackDataStateRunTurn(CDataPacket *packet)
{
}



void CNetworkPlayerComponent::updateStateRunFast()
{
}

void CNetworkPlayerComponent::unpackDataStateRunFast(CDataPacket *packet)
{
}




void CNetworkPlayerComponent::updateStateRunFastToRun()
{
}

void CNetworkPlayerComponent::unpackDataStateRunFastTurn(CDataPacket *packet)
{
}



void CNetworkPlayerComponent::updateStateRunToRunFast()
{
}

void CNetworkPlayerComponent::unpackDataStateRunToRunFast(CDataPacket *packet)
{
}



void CNetworkPlayerComponent::updateStateRunFastTurn()
{
}

void CNetworkPlayerComponent::unpackDataStateRunFastToRun(CDataPacket *packet)
{
}



void CNetworkPlayerComponent::updateStateStandAim()
{
}

void CNetworkPlayerComponent::unpackDataStateStandAim(CDataPacket *packet)
{
}



void CNetworkPlayerComponent::updateStatePlayerRotate()
{
}

void CNetworkPlayerComponent::unpackDataStatePlayerRotate(CDataPacket *packet)
{
}


// stepAnimationTime	
void CNetworkPlayerComponent::stepAnimationTime()
{
	float timeStep = getIView()->getTimeStep();
	m_animCurrentTime = m_animCurrentTime - timeStep;
	
	if ( m_animCurrentTime < 0 )
		m_animCurrentTime = 0;
}