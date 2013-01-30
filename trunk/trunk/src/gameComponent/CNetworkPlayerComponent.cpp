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

    // check run command
    m_runCommand	= m_playerMoveEvt.run;    
    if ( m_playerCmdEvt.shoot || m_playerCmdEvt.reload )
        m_gunOnCommand = true;
    else
        m_gunOnCommand = false;
    
	// unpack mp state
	unpackDataMPState(packet);
    
    // change state
    if ( m_state != m_stateMP )
        setState(m_stateMP);
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
    static int s_currentAnimID = 0;
    
    stepAnimationTime();
    
	if ( m_subState == CPlayerComponent::SubStateInit )
	{
        s_currentAnimID = m_idleAnimationID;
        const char *anim = m_animIdle[m_idleAnimationID].c_str();
        
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
        // reinit state
		if ( m_animCurrentTime <= 0 || m_idleAnimationID != s_currentAnimID )
			m_subState = CPlayerComponent::SubStateInit;        
	}
}

void CNetworkPlayerComponent::unpackDataStateIdle(CDataPacket *packet)
{
    m_idleAnimationID = (int)packet->getByte();
}





void CNetworkPlayerComponent::updateStateTurn()
{
    static bool s_runFast = false;
    
	if ( m_subState == SubStateInit )
	{
		m_subState = SubStateActive;
		m_collada->setCrossFadeAnimation( m_animIdle[0].c_str() );
        
        // check runfast action fron control
        s_runFast = m_playerMoveEvt.runFast;
	}
	else if ( m_subState == SubStateEnd )
	{		
		doNextState();		
	}
	else
	{		
		core::vector3df v0, v1;
        
		// get vector rotate & speed
		v0 = m_gameObject->getFront();
		v1 = m_MPRotateVector;
		
        
		float rot = 0.0f;
		if ( m_runCommand )
        {
            // rotate to player control direction
			rot = m_playerMoveEvt.rotate;
        } 
        
        // rotate to run dir if runfast
        if ( s_runFast )
        {
            core::quaternion q;
            q.fromAngleAxis( core::degToRad(rot), core::vector3df(0,1,0) );
            q.getMatrix().rotateVect(v1);
            v1.normalize();
        }
        
		// step to turn camera vector
		bool turnFinish  = turnToDir( v0, v1, 6.0f );
        
		// rotate object
		m_gameObject->lookAt( m_gameObject->getPosition() + v0 );
	}
}


void CNetworkPlayerComponent::unpackDataStateTurn(CDataPacket *packet)
{
    m_MPRotateVector.Z = packet->getFloat();
    m_MPRotateVector.Y = packet->getFloat();
    m_MPRotateVector.Z = packet->getFloat();
}






void CNetworkPlayerComponent::updateStateRun()
{
    if ( m_subState == SubStateInit )
	{
        // enable multi animation blending
		m_collada->enableAnimTrackChannel(0, true);
		m_collada->enableAnimTrackChannel(1, false);
		m_collada->enableAnimTrackChannel(2, true);
		m_collada->enableAnimTrackChannel(3, true);
		m_collada->enableAnimTrackChannel(4, true);
		m_collada->enableAnimTrackChannel(5, true);        
        
		m_runFactor = 0.0f;
        
		m_collada->setCrossFadeAnimation( m_animIdle[0].c_str(),0 );
		
		m_collada->setAnimation(m_animRunForward.c_str(),       2, true );
		m_collada->setAnimWeight(0.0f, 2);
        
		m_collada->setAnimation(m_animRunBackward.c_str(),      3, true );
		m_collada->setAnimWeight(0.0f, 3);
        
		m_collada->setAnimation(m_animRunStrafeRight.c_str(),   4, true );
		m_collada->setAnimWeight(0.0f, 4);
        
		m_collada->setAnimation(m_animRunStrafeLeft.c_str(),    5, true );
		m_collada->setAnimWeight(0.0f, 5);
        
        
        // set weight run anim = zero
		m_collada->setAnimWeight(1.0f, 0);
		m_collada->setAnimWeight(0.0f, 1);
        
		m_collada->setAnimWeight(0.0f, 2);
		m_collada->setAnimWeight(0.0f, 3);
		m_collada->setAnimWeight(0.0f, 4);
		m_collada->setAnimWeight(0.0f, 5);
        
		m_collada->synchronizedByTimeScale();	
        
		// default control rotate
		m_controlRotate = m_gameObject->getFront();
		
		// switch up body to aim state
        if ( m_upbodyState == CPlayerComponent::PlayerUpBodyOffGun )
            setUpBodyState(CPlayerComponent::PlayerUpBodyAim);
        
        // init virtual factor variable
        // it will sync from mp later
        m_animForwardFactor = 1.0f; 
        m_animBackwardFactor= 0.0f;
        m_animLeftFactor    = 0.0f;
        m_animRightFactor   = 0.0f;
        
        m_MPRunRotate         = 0.0f;
        
		m_subState = SubStateActive;
	}
	else if ( m_subState == SubStateEnd )
	{
		doNextState();
	}
	else
	{		
		core::vector3df v0, v1, runDir;
        
		// get vector rotate & speed
		v0 = m_gameObject->getFront();
		v1 = m_MPRotateVector;
		runDir = v0;		
        
		core::quaternion q;
		float rot = 0.0f;
        
		if ( m_runCommand )
        {
			rot = m_playerMoveEvt.rotate;		
            
            // rotate rundir
            q.fromAngleAxis( core::degToRad(rot), core::vector3df(0,1,0) );
            q.getMatrix().rotateVect(runDir);
            runDir.normalize();		
            
            // rotate step runDir
            turnToDir( m_controlRotate, runDir, 2.0f );
        }

        
		float step = m_runAccel*getIView()->getTimeStep();
        
		if ( m_runCommand == false )
		{
			m_runFactor = m_runFactor - step;
			if ( m_runFactor < 0.0f )
				m_runFactor = 0.0f;
            
			m_collada->setAnimWeight(1.0f - m_runFactor,							0);            
			m_collada->setAnimWeight(0.0f,											1);	
            
			m_collada->setAnimWeight(m_runFactor*m_animForwardFactor,	2);
			m_collada->setAnimWeight(m_runFactor*m_animBackwardFactor,	3);
			m_collada->setAnimWeight(m_runFactor*m_animLeftFactor,		4);
			m_collada->setAnimWeight(m_runFactor*m_animRightFactor,		5);
            
            m_collada->synchronizedByTimeScale();   
		}
		else
		{      
            // calc animation blending			
			calcRunAnimationBlend(m_MPRunRotate, 
                                  m_animForwardFactor,
                                  m_animBackwardFactor, 
                                  m_animLeftFactor, 
                                  m_animRightFactor);
            
			m_runFactor = m_runFactor + step;
			if ( m_runFactor > 1.0f )
				m_runFactor = 1.0f;
            
			m_collada->setAnimWeight(1.0f - m_runFactor, 0);
            m_collada->setAnimWeight(0.0f, 1);
            
			m_collada->setAnimWeight(m_runFactor*m_animForwardFactor,   2);
			m_collada->setAnimWeight(m_runFactor*m_animBackwardFactor,  3);
			m_collada->setAnimWeight(m_runFactor*m_animLeftFactor,      4);
			m_collada->setAnimWeight(m_runFactor*m_animRightFactor,     5);
            
            m_collada->synchronizedByTimeScale();                                
		}
        
        
		if ( m_runCommand )
		{
			// rotate character
            turnToDir( v0, v1, 2.0f );
			m_gameObject->lookAt( m_gameObject->getPosition() + v0 );
            
			// calc spine rotation
			//core::vector3df lookPos = m_gameObject->getPosition() + m_gameObject->getFront();
            //setSpineLookAt( lookPos, 1.0f );
		}
        
		// update run position
		float runSpeed = m_runSpeed * m_runFactor * getIView()->getTimeStep();
		core::vector3df newPos = m_gameObject->getPosition() + m_controlRotate * runSpeed;
		m_gameObject->setPosition( newPos );
        
	}
}

void CNetworkPlayerComponent::unpackDataStateRun(CDataPacket *packet)
{
    m_MPRotateVector.X = packet->getFloat();
    m_MPRotateVector.Y = packet->getFloat();
    m_MPRotateVector.Z = packet->getFloat();    
    
    m_runFactor         = packet->getFloat();    
    m_MPRunRotate       = packet->getFloat(); 
}





void CNetworkPlayerComponent::updateStateRunTurn()
{
    static float s_runTurnFactor = 0.0f;
    static float s_controlRotate = 0.0f;
    float invRun = 0.0f;
    
    s_runTurnFactor = m_MPRunTurnFactor;
    s_controlRotate = m_MPRunRotate;
    
    if ( m_subState == SubStateInit )
	{
        s_runTurnFactor = 0.0f;
        s_controlRotate = m_playerMoveEvt.rotate;
        
		m_subState = SubStateActive;        
    }
	else if ( m_subState == SubStateEnd )
    {
        // change state
		doNextState();        
    }
    else
    {
        float step = m_runAccel*getIView()->getTimeStep();
        
        if ( m_runCommand == false )
        {
            m_runFactor = m_runFactor - step;
            if ( m_runFactor <= 0 )
                m_runFactor = 0;
        }
        else 
        {
            s_runTurnFactor = s_runTurnFactor + step;
            invRun = 1.0f - s_runTurnFactor;
            
            m_runFactor = m_runFactor + step;
            if ( m_runFactor > 1.0f )
                m_runFactor = 1.0f;
        }
        
        core::vector3df v0, v1;
        
		// get vector rotate & speed
		v0 = m_gameObject->getFront();
		v1 = m_MPRotateVector;
        
        core::quaternion q;
        
		// rotate rundir
		q.fromAngleAxis( core::degToRad(s_controlRotate), core::vector3df(0,1,0) );
		q.getMatrix().rotateVect(v0);
		v0.normalize();
        
        // calc control rotate
        m_controlRotate = v0;        
        float animForward, animBackward, animLeft, animRight;
        
        // calc future animation blending			
        calcRunAnimationBlend(s_controlRotate,
                              animForward, 
                              animBackward, 
                              animLeft, 
                              animRight);
        
        // calc present animation
        animForward     = invRun*m_animForwardFactor    + s_runTurnFactor*animForward;
        animBackward    = invRun*m_animBackwardFactor   + s_runTurnFactor*animBackward;
        animLeft        = invRun*m_animLeftFactor       + s_runTurnFactor*animLeft;
        animRight       = invRun*m_animRightFactor      + s_runTurnFactor*animRight;
        
        m_collada->setAnimWeight(1.0f - m_runFactor,  0);
        m_collada->setAnimWeight(0.0f,	1);    
        
		m_collada->setAnimWeight(m_runFactor*animForward,   2);
        m_collada->setAnimWeight(m_runFactor*animBackward,  3);
        m_collada->setAnimWeight(m_runFactor*animLeft,      4);
        m_collada->setAnimWeight(m_runFactor*animRight,     5);
        
        m_collada->synchronizedByTimeScale();        
        
        // update player position
        float runSpeed = m_runSpeed * m_runFactor * s_runTurnFactor * getIView()->getTimeStep();
		core::vector3df newPos = m_gameObject->getPosition() + m_controlRotate * runSpeed;
		m_gameObject->setPosition( newPos );
        
        // turn finish
        if ( s_runTurnFactor > 1.0f )
        {
            s_runTurnFactor = 1.0f;
            
            m_animForwardFactor = animForward;
            m_animBackwardFactor = animBackward;
            m_animLeftFactor = animLeft;
            m_animRightFactor = animRight;        
        }        
    }
}

void CNetworkPlayerComponent::unpackDataStateRunTurn(CDataPacket *packet)
{
    m_MPRotateVector.X = packet->getFloat();
    m_MPRotateVector.Y = packet->getFloat();
    m_MPRotateVector.Z = packet->getFloat();    

    m_MPRunTurnFactor   = packet->getFloat();
    m_MPRunRotate       = packet->getFloat();    
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
    if ( m_subState == CPlayerComponent::SubStateInit )
    {        
        m_subState = CPlayerComponent::SubStateActive;		
    }
    else if ( m_subState == CPlayerComponent::SubStateEnd )
    {		
        doNextState();		
    }
    else
    {
        core::vector3df v0 = m_gameObject->getFront();
        core::vector3df aimPos = m_MPRotateVector;
        aimPos.Y = 0;
        aimPos.normalize();
        
        if ( turnToDir( v0, aimPos, 6.0f ) == true )
        {
            setState(CPlayerComponent::PlayerStandAim);
            setUpBodyState(CPlayerComponent::PlayerUpBodyAim);
        }
        m_gameObject->lookAt( m_gameObject->getPosition() + v0 );
        
    } 
}

void CNetworkPlayerComponent::unpackDataStatePlayerRotate(CDataPacket *packet)
{
    m_MPRotateVector.X = packet->getFloat();
    m_MPRotateVector.Y = packet->getFloat();
    m_MPRotateVector.Z = packet->getFloat();
}