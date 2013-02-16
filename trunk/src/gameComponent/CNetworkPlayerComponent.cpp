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
	switch( m_stateMP )
	{
	case CBasePlayerState::PlayerIdle:
		unpackDataStateIdle(packet);			
		break;
	case CBasePlayerState::PlayerTurn:
		unpackDataStateTurn(packet);			
		break;
	case CBasePlayerState::PlayerRunTurn:
		unpackDataStateRunTurn(packet);
		break;
	case CBasePlayerState::PlayerRun:
		unpackDataStateRun(packet);
		break;
    case CBasePlayerState::PlayerRunFastTurn:
        unpackDataStateRunFastTurn(packet);    
        break;
	case CBasePlayerState::PlayerRunFast:
		unpackDataStateRunFast(packet);
		break;
    case CBasePlayerState::PlayerRunToRunFast:
        unpackDataStateRunToRunFast(packet);
        break;
    case CBasePlayerState::PlayerRunFastToRun:
        unpackDataStateRunFastToRun(packet);
        break;
    case CBasePlayerState::PlayerStandAim:
        unpackDataStateStandAim(packet);			
        break;
    case CBasePlayerState::PlayerRotate:
        unpackDataStatePlayerRotate(packet);
        break;
	}
}

void CNetworkPlayerComponent::unpackDataMPUpBodyState(CDataPacket *packet)
{
    switch (m_upbodyStateMP)
    {
        case CBasePlayerState::PlayerUpBodyAim:
            unpackDataUpperBodyAim(packet);
            break;
        case CBasePlayerState::PlayerUpBodyShoot:
            unpackDataUpperBodyShoot(packet);
            break;            
        case CBasePlayerState::PlayerUpBodyReload:
            unpackDataUpperBodyReload(packet);
            break;            
        case CBasePlayerState::PlayerUpBodyOffGun:
            unpackDataUpperBodyOffgun(packet);
            break;            
        case CBasePlayerState::PlayerUpBodyAimToOffGun:
            unpackDataUpperBodyAimToOffGun(packet);
            break;            
        case CBasePlayerState::PlayerUpBodyOffGunToAim:
            unpackDataUpperBodyOffGunToAim(packet);
            break;            
        case CBasePlayerState::PlayerUpBodyRunFast:
            unpackDataUpperBodyRunFast(packet);
            break;            
        default:
            break;
    }
}

void CNetworkPlayerComponent::updateState()
{
    switch( m_state )
	{
        case CBasePlayerState::PlayerIdle:
            updateStateIdle();			
            break;
        case CBasePlayerState::PlayerTurn:
            updateStateTurn();			
            break;
        case CBasePlayerState::PlayerRunTurn:
            updateStateRunTurn();
            break;
        case CBasePlayerState::PlayerRun:
            updateStateRun();
            break;
        case CBasePlayerState::PlayerRunFastTurn:
            updateStateRunFastTurn();    
            break;
        case CBasePlayerState::PlayerRunFast:
            updateStateRunFast();
            break;
        case CBasePlayerState::PlayerRunToRunFast:
            updateStateRunToRunFast();
            break;
        case CBasePlayerState::PlayerRunFastToRun:
            updateStateRunFastToRun();
            break;
        case CBasePlayerState::PlayerStandAim:
            updateStateStandAim();			
            break;
        case CBasePlayerState::PlayerRotate:
            updateStatePlayerRotate();
            break;
	}
    
	// update body
    switch ( m_upbodyStateMP ) 
    {
        case CBasePlayerState::PlayerUpBodyAim:
            updateUpperBodyAim();
            break;
        case CBasePlayerState::PlayerUpBodyShoot:
            updateUpperBodyShoot();
            break;
        case CBasePlayerState::PlayerUpBodyReload:
            updateUpperBodyReload();
            break;
        case CBasePlayerState::PlayerUpBodyOffGun:
            updateUpperBodyOffgun();
            break;
        case CBasePlayerState::PlayerUpBodyAimToOffGun:
            updateUpperBodyAimToOffGun();
            break;
        case CBasePlayerState::PlayerUpBodyOffGunToAim:
            updateUpperBodyOffGunToAim();
            break;
        case CBasePlayerState::PlayerUpBodyRunFast:
            updateUpperBodyRunFast();
            break;            
    }
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

    
    // todo update
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

    // todo update
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
        turnToDir( v0, v1, 6.0f );
        
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

    // todo update
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
        bool runState = false;
        
		if ( m_nextState == CBasePlayerState::PlayerRun )
            runState = true;

        // change state
		doNextState();
        
        // do not need init run state
        if (runState )
            m_subState = SubStateActive;
    }

    // todo update
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
	if ( m_subState == SubStateInit )
	{
        m_collada->onlyEnableAnimTrackChannel(0);
		m_collada->enableAnimTrackChannel(0, true);
		m_collada->enableAnimTrackChannel(1, true);
        
		m_collada->setAnimation(m_animRunNoGun.c_str(), 1, true );
		
		m_collada->setAnimWeight(1.0f - m_runFactor, 0);
		m_collada->setAnimWeight(m_runFactor, 1);        
		m_collada->synchronizedByTimeScale();		
        
		m_subState = SubStateActive;
	}
	else if ( m_subState == SubStateEnd )
	{
		if ( m_nextState == CBasePlayerState::PlayerIdle )
		{
            // turn off multi anim track
			m_collada->setAnimWeight(1.0f, 0);
			m_collada->setAnimWeight(0.0f, 1);
            
			m_collada->setAnimSpeed(1.0f, 0);
			m_collada->onlyEnableAnimTrackChannel(0);
		}
		doNextState();
	}
    
    // todo update
	{
		float step = m_runAccel*getIView()->getTimeStep();
        
		if ( m_runCommand == false /*&& m_upbodyState == CBasePlayerState::PlayerUpBodyRunFast*/ )
		{            
            // calc spine rotation
            //core::vector3df lookPos = m_gameObject->getPosition() + m_gameObject->getFront();
            //setSpineLookAt( lookPos, 1.0f );
            
            m_runFactor = m_runFactor - step;
            
            // switch to idle state
            if ( m_runFactor < 0.0f )
            {
                m_runFactor = 0.0f;
            }
            
            m_collada->setAnimWeight(1.0f - m_runFactor, 0);
            m_collada->setAnimWeight(m_runFactor, 1);
            
            m_collada->synchronizedByTimeScale();
            
		}
		else
		{
			m_runFactor = m_runFactor + step;
			if ( m_runFactor > 1.0f )
				m_runFactor = 1.0f;
            
			m_collada->setAnimWeight(1.0f - m_runFactor, 0);
			m_collada->setAnimWeight(m_runFactor, 1);
            
			m_collada->synchronizedByTimeScale();
            
		}
        
		core::vector3df v0, v1;
        
		// get vector rotate & speed
		v0 = m_gameObject->getFront();
		v1 = m_MPRotateVector;
		
		core::quaternion q;
		float rot = 0.0f;
        
		if ( m_runCommand )		
			rot = m_playerMoveEvt.rotate;		
        
		q.fromAngleAxis( core::degToRad(rot), core::vector3df(0,1,0) );
		q.getMatrix().rotateVect(v1);
		v1.normalize();
		
		// do not need run turn state
		if ( fabs( getAngle(v0,v1) ) >= 150.0f && m_runCommand )
		{			
            // begin switch state run fast turn
			m_runTurnVector  = v1;
			m_runCurrentVector = m_gameObject->getFront();
		}
		else
		{                     
			// step to turn camera vector
			if ( m_runCommand )
			{
				// rotate character
                turnToDir( v0, v1, 2.0f );
				m_gameObject->lookAt( m_gameObject->getPosition() + v0 );
                
				// calc spine rotation
				if ( m_runFactor > 0.1f )
				{
					//core::vector3df lookPos = m_gameObject->getPosition() + getCameraFrontVector();
					//setSpineLookAt( lookPos );
				}
			}
			
			// update run position
			float runSpeed = m_runFastSpeed * m_runFactor * getIView()->getTimeStep();
			core::vector3df newPos = m_gameObject->getPosition() + v0 * runSpeed;
			m_gameObject->setPosition( newPos );
		}
	}    
}

void CNetworkPlayerComponent::unpackDataStateRunFast(CDataPacket *packet)
{
    m_MPRotateVector.X = packet->getFloat();
    m_MPRotateVector.Y = packet->getFloat();
    m_MPRotateVector.Z = packet->getFloat();    
}







void CNetworkPlayerComponent::updateStateRunFastToRun()
{
    if ( m_subState == SubStateInit )
	{        
        // active state
		m_subState = SubStateActive;		
	}
	else if ( m_subState == SubStateEnd )
	{        
        // change state
		doNextState();
	}

    // update state
	{
        core::vector3df v0, v1, runDir;
        
		// get vector rotate & speed
		v0 = m_gameObject->getFront();
		v1 = m_MPRotateVector;
        runDir = v0;
        
		float step = m_runAccel*getIView()->getTimeStep()*1.0f;
        
        m_runFactor = m_runFactor - step;
        
        if ( m_runFactor < 0.0f )
        {
            m_runFactor = 0.0f;            
            setState( CBasePlayerState::PlayerRun );
        }        
        
        // rotate character to camera 
        // if rot 180 deg
        core::vector3df v = v0 + v1;
        if ( v.getLengthSQ() <= 0.1f )
            v1 = m_gameObject->getRight();
        
        turnToDir(runDir, v1, 6.0f);
        m_gameObject->lookAt(m_gameObject->getPosition() + runDir);
        
        // set anim weight
        m_collada->setAnimWeight(1.0f - m_runFactor,							0);            
        m_collada->setAnimWeight(m_runFactor,									1);        
        
        m_collada->synchronizedByTimeScale();   
        
		// update run position
		float runSpeed = m_runSpeed * m_runFactor * getIView()->getTimeStep();
		core::vector3df newPos = m_gameObject->getPosition() + m_gameObject->getFront() * runSpeed;
		m_gameObject->setPosition( newPos );
    }
}

void CNetworkPlayerComponent::unpackDataStateRunFastToRun(CDataPacket *packet)
{
    m_MPRotateVector.X = packet->getFloat();
    m_MPRotateVector.Y = packet->getFloat();    
    m_MPRotateVector.Z = packet->getFloat();
}







void CNetworkPlayerComponent::updateStateRunToRunFast()
{
    if ( m_subState == SubStateInit )
	{               
        // change state
		m_subState = SubStateActive;
        
	}
	else if ( m_subState == SubStateEnd )
	{	       
        // change state
		doNextState();     
	}

    
    // update state
	{		        
		float step = m_runAccel*getIView()->getTimeStep()*1.0f;        
        m_runFactor = m_runFactor - step;
        
        if ( m_runFactor < 0.0f )
        {
            // switch to run fast state
            m_runFactor = 0.0f;                        
        }    
        
        // set anim weight
        m_collada->setAnimWeight(1.0f - m_runFactor,							0);            
        m_collada->setAnimWeight(0.0f,											1);	
        
        m_collada->setAnimWeight(m_runFactor*m_animForwardFactor,	2);
        m_collada->setAnimWeight(m_runFactor*m_animBackwardFactor,	3);
        m_collada->setAnimWeight(m_runFactor*m_animLeftFactor,		4);
        m_collada->setAnimWeight(m_runFactor*m_animRightFactor,		5);
        
        m_collada->synchronizedByTimeScale();                    
        
        
		// update run position
		float runSpeed = m_runSpeed * m_runFactor * getIView()->getTimeStep();
		core::vector3df newPos = m_gameObject->getPosition() + m_controlRotate * runSpeed;
		m_gameObject->setPosition( newPos );
    }    
}

void CNetworkPlayerComponent::unpackDataStateRunToRunFast(CDataPacket *packet)
{
}






void CNetworkPlayerComponent::updateStateRunFastTurn()
{
	if ( m_subState == SubStateInit )
	{
		m_collada->enableAnimTrackChannel(0, true);
		m_collada->enableAnimTrackChannel(1, true);
        
		m_collada->setAnimWeight(1.0f - m_runFactor, 0);
		m_collada->setAnimWeight(m_runFactor, 1);
        
		m_collada->synchronizedByTimeScale();
        
		m_subState = SubStateActive;		
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
		v1 = m_runTurnVector;
		v1.normalize();
		
        // if rot 180 deg
        core::vector3df v = v0 + v1;
        if ( v.getLengthSQ() <= 0.1f )
            v1 = m_gameObject->getRight();
        
		// step to turn camera vector
        bool turn = turnToDir( v0, v1, 5.0f );
      
		// rotate object
		m_gameObject->lookAt( m_gameObject->getPosition() + v0 );
        
		// calc spine rotation
		//core::vector3df lookPos = m_gameObject->getPosition() + getCameraFrontVector();
		//setSpineLookAt( lookPos );
        
		// synch animation to idle
		float step = m_runAccel*getIView()->getTimeStep()*0.8f;
		m_runFactor = m_runFactor - step;        
        
        m_runFactor = core::clamp(m_runFactor, 0.0f, 1.0f);
        
		if ( m_runFactor <= 0.0f && turn == true )
		{
			m_runFactor = 0.0f;
			setState( CBasePlayerState::PlayerRunFast );
		}
		
		m_collada->setAnimWeight(1.0f - m_runFactor, 0);
		m_collada->setAnimWeight(m_runFactor, 1);
        
		m_collada->synchronizedByTimeScale();
		
		// update run position
		float runSpeed = m_runSpeed * m_runFactor * getIView()->getTimeStep();
		core::vector3df newPos = m_gameObject->getPosition() + m_runCurrentVector * runSpeed;
		m_gameObject->setPosition( newPos );
	}    
}

void CNetworkPlayerComponent::unpackDataStateRunFastTurn(CDataPacket *packet)
{
    m_runTurnVector.X = packet->getFloat();
    m_runTurnVector.Y = packet->getFloat();
    m_runTurnVector.Z = packet->getFloat();    
}




void CNetworkPlayerComponent::updateStateStandAim()
{
	if ( m_subState == SubStateInit )
    {		
		m_collada->setCrossFadeAnimation( m_animIdle[0].c_str(),0 );
		m_collada->setAnimWeight(1.0f, 0);
        		
		// setup player cmd state
		m_playerCmdEvt.shoot	= false;
		m_playerCmdEvt.reload	= false;

		// change state
        m_subState = SubStateActive;
    }
    else if ( m_subState == SubStateEnd )
    {		
        m_collada->setCrossFadeAnimation(m_animIdle[0].c_str(), 0, 10, true);
        doNextState();
    }
    
	// todo update
	{
		if ( m_aimRotateCharacter )
		{		
			// rotate main character		
			core::vector3df v0 = m_gameObject->getFront();
			core::vector3df aimPos = m_MPRotateVector;
			aimPos.Y = 0;
			aimPos.normalize();

			if ( turnToDir( v0, aimPos, 6.0f ) == true )
				m_aimRotateCharacter = false;

			m_gameObject->lookAt( m_gameObject->getPosition() + v0 );
		}		
    }
}

void CNetworkPlayerComponent::unpackDataStateStandAim(CDataPacket *packet)
{
	m_MPRotateVector.X = packet->getFloat();
    m_MPRotateVector.Y = packet->getFloat();
    m_MPRotateVector.Z = packet->getFloat();
    m_aimRotateCharacter = (packet->getByte() == 1);
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
            setState(CBasePlayerState::PlayerStandAim);
            setUpBodyState(CBasePlayerState::PlayerUpBodyAim);
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






void CNetworkPlayerComponent::updateUpperBodyAim()
{
    if ( m_upbodySubState == SubStateInit )
    {
        // turn off all anim channel
        m_collada->onlyEnableAnimTrackChannel(0, 1);
        
		// base idle
        m_collada->setCrossFadeAnimationToLayer( m_animIdle[0].c_str(), 10.0f, true, 0, 0, 1, 0);
        
		// base aim
        m_collada->setAnimation(m_animAimStraight.c_str(),	1, true, 1 );        
        m_collada->setAnimWeight(0.0f, 1, 1);
        m_collada->enableAnimTrackChannel(1, true, 1);
        
        m_collada->setAnimation(m_animAimUp.c_str(),	2, true, 1 );
        m_collada->setAnimWeight(0.0f, 2, 1);
        m_collada->enableAnimTrackChannel(2, true, 1);
        
        m_collada->setAnimation(m_animAimDown.c_str(),	3, true, 1 );
        m_collada->setAnimWeight(0.0f, 3, 1);
        m_collada->enableAnimTrackChannel(3, true, 1);                       
        
        m_upbodySubState = SubStateActive;
    }
    else if ( m_upbodySubState == SubStateEnd )
    {
    }
    
    // todo update aim
    {
        float step = 0.005f*getIView()->getTimeStep();
        
        // inc aim factor
        m_aimFactor = m_aimFactor + step;
        
        // calc aim
        m_aimFactor = core::clamp<float>(m_aimFactor, 0.0f, 1.0f);
        
        // get up,down factor
		core::vector2df ret     = m_MPAimAngle;
        
		float wUp, wDown, wLeft, wRight, wStraight;
		calcAimAnimationBlend(ret, wUp, wDown, wLeft, wRight);		
        
        if ( wUp > 0 )
            wStraight = 1.0f - wUp;
        else 
            wStraight = 1.0f - wDown;
        
        // setup straight
        wStraight = core::clamp<float>(wStraight, 0.0f, 1.0f);
        
        
        // setup anim blend factor
        wStraight   = wStraight * m_aimFactor;
        wUp         = wUp * m_aimFactor;
        wDown       = wDown * m_aimFactor;        
		
		
		// rotate spine character        
        float spineAngle = m_MPSpineRotate;
        spineAngle = core::clamp<float>(spineAngle, -40.0f, 40.0f);        
		setSpineRotation(spineAngle);
        
        // blend anim up, down
        m_collada->setAnimWeight(1.0f - m_aimFactor, 0, 1);			// idle
        m_collada->setAnimWeight(wStraight, 1, 1);					// straight
		m_collada->setAnimWeight(wUp,	2, 1);						// up
		m_collada->setAnimWeight(wDown, 3, 1);						// down
        m_collada->synchronizedByTimeScale(1);
        
    }
}

void CNetworkPlayerComponent::unpackDataUpperBodyAim(CDataPacket *packet)
{
    m_MPAimAngle.X = packet->getFloat();
    m_MPAimAngle.Y = packet->getFloat();
    m_MPSpineRotate = packet->getFloat();
}




void CNetworkPlayerComponent::updateUpperBodyShoot()
{
    const float shootSpeed = 2.0f;      
	const float flashTime = 100.0f;
	static bool s_spawnBullet = true;

    if ( m_upbodySubState == SubStateInit )
    {
        // turn off all anim channel
        m_collada->onlyEnableAnimTrackChannel(1, 1);
        
		m_collada->setAnimation(m_animShootStraight.c_str(),1, false, 1);        
        m_collada->setAnimWeight(0.0f, 1, 1);
		m_collada->enableAnimTrackChannel(1, true, 1);
        
		m_collada->setAnimation(m_animShootUp.c_str(),		2, false, 1);
		m_collada->setAnimWeight(0.0f, 2, 1);
		m_collada->enableAnimTrackChannel(2, true, 1);
        
		m_collada->setAnimation(m_animShootDown.c_str(),	3, false, 1);
		m_collada->setAnimWeight(0.0f, 3, 1);
		m_collada->enableAnimTrackChannel(3, true, 1);
        
		// anim time
		m_animCurrentTime = m_collada->getCurrentAnimTimeLength()/shootSpeed;
        
		// setup player state
		m_playerCmdEvt.shoot	= false;
		m_playerCmdEvt.reload	= false;        
        
        
        CGameColladaSceneNode *gunTip = m_collada->getSceneNode("RightGunTip");
		
		// active gunlight
        core::vector3df gunPos = gunTip->getAbsolutePosition();
        m_gunLight->setPosition(gunPos);
        m_gunLightComp->setLightTime(flashTime);
        
		// show muzzle
		showMuzzle(flashTime);
		
		// spawn bullet
		s_spawnBullet = true;

        m_upbodySubState = SubStateActive;		
    }
    else if ( m_upbodySubState == SubStateEnd )
    {
    }
	    
    // todo update
	{
		stepAnimationTime();
		
		// finish animation shoot
		if ( m_animCurrentTime == 0.0f )
			setUpBodyState(CBasePlayerState::PlayerUpBodyAim);	
        
        
		if ( s_spawnBullet == true )
		{
			CGameColladaSceneNode *gunTip = m_collada->getSceneNode("RightGunTip");
			core::vector3df gunPos = gunTip->getAbsolutePosition();

			//core::line3df bulletRay;
			//bulletRay.start = gunPos;
			//bulletRay.end = colPos;
			//m_bulletRayComp->addBulletRay(bulletRay);
			
			s_spawnBullet = false;
		}

		// rotate spine character
		float angle = m_MPSpineRotate;
		angle = core::clamp<float>(angle, -40.0f, 40.0f);
		setSpineRotation(angle);


		// blend to up/down hand
		core::vector2df ret = m_MPAimAngle;
        
		float wUp, wDown, wLeft, wRight, wStraight;
		calcAimAnimationBlend(ret, wUp, wDown, wLeft, wRight);
		
        if ( wUp > 0 )
            wStraight = 1.0f - wUp;
        else 
            wStraight = 1.0f - wDown;
        
        // setup straight
        wStraight = core::clamp<float>(wStraight, 0.0f, 1.0f);		
        
        // blend anim up, down
        m_collada->setAnimWeight(0.0f,		0, 1);		// idle
        m_collada->setAnimWeight(wStraight, 1, 1);		// straight
		m_collada->setAnimWeight(wUp,		2, 1);		// up
		m_collada->setAnimWeight(wDown,		3, 1);		// down

		// speed up fire anim
        m_collada->synchronizedByTimeScale(1, shootSpeed);
	}
}

void CNetworkPlayerComponent::unpackDataUpperBodyShoot(CDataPacket *packet)
{
	m_MPAimAngle.X = packet->getFloat();
	m_MPAimAngle.Y = packet->getFloat();
	
	m_MPSpineRotate = packet->getFloat();
}




void CNetworkPlayerComponent::updateUpperBodyReload()
{
    
}

void CNetworkPlayerComponent::unpackDataUpperBodyReload(CDataPacket *packet)
{
    
}




void CNetworkPlayerComponent::updateUpperBodyOffgun()
{
    if ( m_upbodySubState == SubStateInit )
    {
		if ( m_offGunAnimation.length() > 0 )
		{
			m_collada->setAnimation(m_offGunAnimation, 0, true, 1);        
			m_collada->setAnimWeight(m_offGunFactor, 0, 1);
			m_collada->enableAnimTrackChannel(0, true, 1);
		}
		else
		{
			m_collada->enableAnimTrackChannel(0, false, 1);
		}
        
        m_upbodySubState = SubStateActive;		
    }
    else if ( m_upbodySubState == SubStateEnd )
    {
    }
    
    // todo update state
	{		
		core::vector2df ret = m_MPAimAngle;
        
		float wUp, wDown, wLeft, wRight, wStraight;
		calcAimAnimationBlend(ret, wUp, wDown, wLeft, wRight);
		
        if ( wUp > 0 )
            wStraight = 1.0f - wUp;
        else 
            wStraight = 1.0f - wDown;
        
        // setup straight
        wStraight = core::clamp<float>(wStraight, 0.0f, 1.0f);
		
		float inv	= 1.0f - m_offGunFactor;
		float f		= inv;
        
		// recalc weight
		wStraight	= wStraight*f;
		wUp			= wUp*f;
		wDown		= wDown*f;		
        
		// blending anim
        m_collada->setAnimWeight(m_offGunFactor,        0, 1);		// off gun anim
        m_collada->setAnimWeight(wStraight,				1, 1);		// straight
		m_collada->setAnimWeight(wUp,					2, 1);		// up
		m_collada->setAnimWeight(wDown,					3, 1);		// down
        
		m_collada->synchronizedByTimeScale(1);
        
		// revert the animation speed
		m_collada->setAnimSpeed(1.0f, 0, 1);                      
    }
}

void CNetworkPlayerComponent::unpackDataUpperBodyOffgun(CDataPacket *packet)
{
    m_MPAimAngle.X = packet->getFloat();
    m_MPAimAngle.Y = packet->getFloat();
}




void CNetworkPlayerComponent::updateUpperBodyAimToOffGun()
{
    if ( m_upbodySubState == SubStateInit )
    {        
        m_offGunFactor = 1.0f;
        
        m_collada->setAnimation(m_animGunOff, 0, true, 1);
        m_collada->setAnimWeight(1.0f, 0, 1);
        m_collada->setAnimSpeed(1.0f, 0, 1);
        m_collada->setAnimationLoop(false, 0, 1);
        m_collada->onlyEnableAnimTrackChannel(0, 1);
        
        m_upbodySubState = SubStateActive;
    }
    else if ( m_upbodySubState == SubStateEnd )
    {
        doNextState();
    }

    // todo update
    {
        // change state if finish animation
        if ( m_collada->isEndAnimation(0, 1) == true )
        {                                     
            m_upBodyRunFastFactor = 0.0f;
            m_offGunFactor = 1.0f;        
        }
    }
}

void CNetworkPlayerComponent::unpackDataUpperBodyAimToOffGun(CDataPacket *packet)
{
    
}



void CNetworkPlayerComponent::updateUpperBodyOffGunToAim()
{
    static int s_offgunState = 0;
    
    if ( m_upbodySubState == SubStateInit )
    {        
        s_offgunState = 0;
        m_upbodySubState = SubStateActive;
    }
    else if ( m_upbodySubState == SubStateEnd )
    {		
        doNextState();		
    }
    
    // todo update
    {        
        float step = 0.002f*getIView()->getTimeStep();    
        
        if ( s_offgunState == 0 )
        {
            m_upBodyRunFastFactor = m_upBodyRunFastFactor - step;
            
            // apply blend
            m_upBodyRunFastFactor = core::clamp<float>(m_upBodyRunFastFactor, 0.0f, 1.0f);    
            
            // apply anim weight
            m_collada->setAnimationLayerWeight(1, 1.0f - m_upBodyRunFastFactor);
            
            if ( m_upBodyRunFastFactor == 0.0f )
            {
                s_offgunState = 1;
                
                
                m_collada->onlyEnableAnimTrackChannel(0, 1);
                m_offGunFactor = 1.0f;
                
                m_collada->setAnimation(m_animGunOn, 0, true, 1);
                m_collada->setAnimWeight(m_offGunFactor, 0, 1);
                m_collada->setAnimationLoop(false, 0, 1);
                m_collada->enableAnimTrackChannel(0, true, 1);                
                
                m_collada->setAnimation(m_animShootStraight.c_str(),1, false, 1);        
                m_collada->setAnimWeight(0.0f, 1, 1);
                m_collada->enableAnimTrackChannel(1, true, 1);
                
                m_collada->setAnimation(m_animShootUp.c_str(),		2, false, 1);
                m_collada->setAnimWeight(0.0f, 2, 1);
                m_collada->enableAnimTrackChannel(2, true, 1);
                
                m_collada->setAnimation(m_animShootDown.c_str(),	3, false, 1);
                m_collada->setAnimWeight(0.0f, 3, 1);
                m_collada->enableAnimTrackChannel(3, true, 1);
                
                
            }
        }
        else
        {
            m_offGunFactor = m_offGunFactor - step;
            m_offGunFactor = core::clamp<float>(m_offGunFactor, 0.0f, 1.0f);
            
            core::vector2df ret = m_MPAimAngle;
            
            float wUp, wDown, wLeft, wRight, wStraight;
            calcAimAnimationBlend(ret, wUp, wDown, wLeft, wRight);
            
            if ( wUp > 0 )
                wStraight = 1.0f - wUp;
            else 
                wStraight = 1.0f - wDown;
            
            // setup straight
            wStraight = core::clamp<float>(wStraight, 0.0f, 1.0f);
            
            float inv	= 1.0f - m_offGunFactor;
            
            // recalc weight
            wStraight	= wStraight*inv;
            wUp			= wUp*inv;
            wDown		= wDown*inv;		
            
            // blending anim
            m_collada->setAnimWeight(m_offGunFactor,        0, 1);		// off gun anim
            m_collada->setAnimWeight(wStraight,				1, 1);		// straight
            m_collada->setAnimWeight(wUp,					2, 1);		// up
            m_collada->setAnimWeight(wDown,					3, 1);		// down
            
            m_collada->synchronizedByTimeScale(1);            
        }        
    }
}

void CNetworkPlayerComponent::unpackDataUpperBodyOffGunToAim(CDataPacket *packet)
{
    m_MPAimAngle.X = packet->getFloat();
    m_MPAimAngle.Y = packet->getFloat();
}


void CNetworkPlayerComponent::updateUpperBodyRunFast()
{
    if ( m_upbodySubState == SubStateInit )
    {        
        m_upbodySubState = SubStateActive;
    }
    else if ( m_upbodySubState == SubStateEnd )
    {
        doNextState();		
    }
    
    // todo update state
    {
        float step = 0.002f*getIView()->getTimeStep();
        
        if ( m_runCommand == true )
            m_upBodyRunFastFactor = m_upBodyRunFastFactor + step;
        else
            m_upBodyRunFastFactor = m_upBodyRunFastFactor - step;
        
        // apply blend
        m_upBodyRunFastFactor = core::clamp<float>(m_upBodyRunFastFactor, 0.0f, 1.0f);    
        
        // apply anim weight
        m_collada->setAnimationLayerWeight(1, 1.0f - m_upBodyRunFastFactor);
    }
}

void CNetworkPlayerComponent::unpackDataUpperBodyRunFast(CDataPacket *packet)
{
    
}
