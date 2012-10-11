#include "stdafx.h"
#include "IView.h"
#include "IRandomizer.h"

#include "CPlayerComponent.h"

#include "CColladaMeshComponent.h"
#include "CInventoryComponent.h"
#include "CWeaponComponent.h"

#include "gameLevel/CGameLevel.h"
#include "gameDebug/CGameDebug.h"

#define LOG_ANIM_TIME

#define REAL_PRECISION	0.000001f
inline bool realIsZero(float x, float precision = REAL_PRECISION) 
{ 
	return fabs(x) < precision; 
};
inline bool realIsEqual(float a, float b, float precision = REAL_PRECISION) 
{ 
	return realIsZero(a-b, precision); 
};

///////////////////////////////////////////////////////////////////////
// IObjectComponent overide implement
///////////////////////////////////////////////////////////////////////

CPlayerComponent::CPlayerComponent(CGameObject* obj)
	:IObjectComponent(obj, CGameComponent::PlayerComponent)
{
	// default state
	m_state			= CPlayerComponent::PlayerNone;	
	m_lastState		= CPlayerComponent::PlayerNone;
	m_nextState		= CPlayerComponent::PlayerNone;

	
	// init run const
	m_runSpeed				= 0.3f;
	m_runFastSpeed			= 0.5f;

	m_runCommand	= false;			

	m_runFactor = 0.0f;
	m_runAccel	= 0.002f;
	m_runToRunFastAccel = 0.003f;

	m_collada	= NULL;
	m_inventory = NULL;

	m_animShotCurrentTime	= 0.0f;
	m_animCurrentTime		= 0.0f;	

	m_spineRotation = 0.0f;
	m_lastRotation = 0.0f;
	
}

CPlayerComponent::~CPlayerComponent()
{
	// unregister event
	getIView()->unRegisterEvent( this );
}

// init
// run when init object
void CPlayerComponent::initComponent()
{	
	m_collada = (CColladaMeshComponent*) m_gameObject->getComponent( IObjectComponent::ColladaMesh );
	m_collada->setAnimationPackage( m_animationPackage );

	// foot nodes
	m_nodesFoot.push_back( m_collada->getSceneNode("Reference") );
	m_nodesFoot.push_back( m_collada->getSceneNode("LeftGun") );
	m_nodesFoot.push_back( m_collada->getSceneNode("RightGun") );
	m_nodesFoot.push_back( m_collada->getSceneNode("Hips") );		
	m_nodesFoot.push_back( m_collada->getSceneNode("LeftUpLeg") );
	m_nodesFoot.push_back( m_collada->getSceneNode("RightUpLeg") );
	m_collada->getChildsOfSceneNode("LeftUpLeg",	m_nodesFoot);
	m_collada->getChildsOfSceneNode("RightUpLeg",	m_nodesFoot);

	// left & right hand with gun
	m_collada->getChildsOfSceneNode("LeftShoulder",		m_nodesLeftShoulder);
	m_nodesLeftShoulder.push_back( m_collada->getSceneNode("LeftGun") );
	m_nodesLeftShoulder.push_back( m_collada->getSceneNode("LeftGunTip") );	

	m_collada->getChildsOfSceneNode("RightShoulder",	m_nodesRightShoulder);
	m_collada->getChildsOfSceneNode("RightGun",			m_nodesRightShoulder);
	m_nodesRightShoulder.push_back( m_collada->getSceneNode("RightGun") );
	m_nodesRightShoulder.push_back( m_collada->getSceneNode("RightGunTip") );

	// chest nodes
	CGameColladaSceneNode *spine = m_collada->getSceneNode("Spine");
	spine->setAnimationCallback(this);
	m_nodesChest.push_back( spine );
	
	spine = m_collada->getSceneNode("Spine1");
	spine->setAnimationCallback(this);	
	m_nodesChest.push_back( spine );

	spine = m_collada->getSceneNode("Spine2");
	spine->setAnimationCallback(this);
	m_nodesChest.push_back( spine );

	// set callback of root node
	CGameColladaSceneNode *root = m_collada->getSceneNode("Reference");
	root->setAnimationCallback(this);


	// hand, head
	m_nodesHandsAndHead.push_back( m_collada->getSceneNode("RightGun") );		
	m_collada->getChildsOfSceneNode("Spine3",m_nodesHandsAndHead);

	// neck
	m_nodeNeck = m_collada->getSceneNode("Neck");
	m_nodeNeck->setAnimationCallback(this);

	// decalre anim list
	m_animIdle.push_back( "Hero@Idle" );
	m_animIdle.push_back( "Hero@Idle1" );
	m_animIdle.push_back( "Hero@Idle2" );

	m_animRunForward	= "Hero@RunForward";
	m_animRunBackward	= "Hero@RunBackward";
	m_animRunStrafeLeft	= "Hero@RunStrafeLeft";
	m_animRunStrafeRight= "Hero@RunStrafeRight";
	m_animRunNoGun		= "Hero@RunForwardNoGun";

	m_animGunOn			= "Hero@GunOn";
	m_animGunOff		= "Hero@GunOff";	
	m_animGunReload		= "Hero@GunReload";

	m_animAimDown		= "Hero@AimDown";
	m_animAimUp			= "Hero@AimUp";
	m_animAimLeft		= "Hero@AimLeft";
	m_animAimRight		= "Hero@AimRight";
	m_animAimStraight	= "Hero@AimStraight";

	m_animShootLeft		= "Hero@ShootLeft";
	m_animShootRight	= "Hero@ShootRight";
	m_animShootUp		= "Hero@ShootUp";
	m_animShootStraight	= "Hero@ShootStraight";


	// set basic state idle
	m_collada->setAnimation( m_animIdle[0] );
	setState( CPlayerComponent::PlayerIdle );

	m_inventory	= (CInventoryComponent*)m_gameObject->getComponent( CGameComponent::InventoryComponent );

	// register event
	getIView()->registerEvent("CPlayerComponent", this);
}

// update
// run when update per frame
void CPlayerComponent::updateComponent()
{
	// update gameplay	
	if ( m_collada == NULL )
		return;	

    // alway sync this object
    m_gameObject->setSyncNetwork(true);
    
    // update player
	updateState();
	updateWeaponPosition();
}

// saveData
// save data to serializable
void CPlayerComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( CGameComponent::s_compType[ m_componentID ] );
}

// loadData
// load data to serializable
void CPlayerComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();


	// need load animation package
	CColladaAnimationFactory* animFactory = CColladaAnimationFactory::getInstance();
	m_animationPackage = animFactory->getAnimation("playerAnim");
	if ( m_animationPackage == NULL )
		m_animationPackage = animFactory->loadAnimation("playerAnim", getIView()->getPath("data/mesh/character/hero/hero.anim"));	
}


// OnEvent
// cache event
bool CPlayerComponent::OnEvent(const SEvent& irrEvent)
{
	if ( irrEvent.EventType == EET_GAME_EVENT && m_gameObject->isNetworkController() == false )
	{
		if ( irrEvent.GameEvent.EventID == EvtPlayerMove )
		{
			m_playerMoveEvt = *((SEventPlayerMove*)irrEvent.GameEvent.EventData);

			// check run command
			m_runCommand	= m_playerMoveEvt.run;
		}
	}

	return true;
}


///////////////////////////////////////////////////////////////////////
// Player component implement
///////////////////////////////////////////////////////////////////////

// updateState	
void CPlayerComponent::updateState()
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
	}
}

///////////////////////////////////////////////////////////////////////
// Player component update state
///////////////////////////////////////////////////////////////////////

void CPlayerComponent::updateStateIdle()
{
	stepAnimationTime();

	if ( m_subState == SubStateInit )
	{
        IrrlichtDevice* device = getIView()->getDevice();
        int r = device->getRandomizer()->rand() % (int)m_animIdle.size();		
        const char *anim = m_animIdle[r].c_str();

        m_collada->setCrossFadeAnimation( anim, 0, 10.0f, false );
        m_animCurrentTime = m_collada->getCurrentAnimTimeLength();
           
		m_subState = SubStateActive;
	}
	else if ( m_subState == SubStateEnd )
	{
		doNextState();		
	}
	else
	{
		// calc spine rotation
		core::vector3df lookPos = m_gameObject->getPosition() + m_gameObject->getFront();
		setSpineLookAt( lookPos, 1.0f );
	
		if ( m_runCommand )
		{	
			setState( CPlayerComponent::PlayerTurn );			
		}

        // reinit state
		if ( m_animCurrentTime <= 0 )
			m_subState = SubStateInit;
	}
}

void CPlayerComponent::updateStateTurn()
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
		v1 = getCameraFrontVector();
		
        
		float rot = 0.0f;
		if ( m_runCommand )
        {
            // rotate to player control direction
			rot = m_playerMoveEvt.rotate;
        }
        else
        {
            // user cancel run command
            setState( CPlayerComponent::PlayerIdle );
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
				
		if ( turnFinish )
		{
            if ( s_runFast )
                setState( CPlayerComponent::PlayerRunFast );                
            else
                setState( CPlayerComponent::PlayerRun );
		}
	}
}

void CPlayerComponent::updateStateRun()
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
		m_collada->setAnimation(m_animRunBackward.c_str(),      3, true );
		m_collada->setAnimation(m_animRunStrafeRight.c_str(),   4, true );
		m_collada->setAnimation(m_animRunStrafeLeft.c_str(),    5, true );        
        
        // set weight run anim = zero
		m_collada->setAnimWeight(1.0f, 1);        
		m_collada->setAnimWeight(0.0f, 1);
		m_collada->setAnimWeight(0.0f, 2);
		m_collada->setAnimWeight(0.0f, 3);
		m_collada->setAnimWeight(0.0f, 4);
		m_collada->setAnimWeight(0.0f, 5);                

		m_collada->synchronizedByTimeScale();	

		// default control rotate
		m_controlRotate = m_gameObject->getFront();

		m_subState = SubStateActive;
	}
	else if ( m_subState == SubStateEnd )
	{
		if ( m_nextState == CPlayerComponent::PlayerIdle )
		{
            // turn off multi anim
			m_collada->setAnimWeight(1.0f, 0);
			m_collada->setAnimWeight(0.0f, 1);

			m_collada->setAnimSpeed(1.0f, 0);
			m_collada->onlyEnableAnimTrackChannel(0);
		}
		doNextState();
	}
	else
	{		
		core::vector3df v0, v1, runDir;

		// get vector rotate & speed
		v0 = m_gameObject->getFront();
		v1 = getCameraFrontVector();
		runDir = v0;		

		core::quaternion q;
		float rot = 0.0f;

		if ( m_runCommand )		
			rot = m_playerMoveEvt.rotate;		

		// rotate rundir
		q.fromAngleAxis( core::degToRad(rot), core::vector3df(0,1,0) );
		q.getMatrix().rotateVect(runDir);
		runDir.normalize();		
        
		// rotate step runDir
		turnToDir( m_controlRotate, runDir, 2.0f );
		
#ifdef WIN32            
		// debug line
		CGameDebug *debug = CGameDebug::getInstance();
		core::line3df line;
		line.start	= m_gameObject->getPosition();
		line.end	= m_gameObject->getPosition() + v1 * 400.0f;
		debug->addDrawLine(line, SColor(255,255,0,0) ); 

		line.end	= m_gameObject->getPosition() + runDir * 400.0f;
		debug->addDrawLine(line, SColor(255,0,255,0) );

		line.end	= m_gameObject->getPosition() + m_controlRotate * 400.0f;
		debug->addDrawLine(line, SColor(255,0,0,255) );
#endif    

		float step = m_runAccel*getIView()->getTimeStep();
        
		if ( m_runCommand == false )
		{
			m_runFactor = m_runFactor - step;
			if ( m_runFactor < 0.0f )
			{
				m_runFactor = 0.0f;
				setState( CPlayerComponent::PlayerIdle );
			}
            
			m_collada->setAnimWeight(1.0f - m_runFactor,				0);            
			m_collada->setAnimWeight(0.0f,								1);	
			m_collada->setAnimWeight(m_runFactor*m_animForwardFactor,   2);
			m_collada->setAnimWeight(m_runFactor*m_animBackwardFactor,  3);
			m_collada->setAnimWeight(m_runFactor*m_animLeftFactor,      4);
			m_collada->setAnimWeight(m_runFactor*m_animRightFactor,     5);
            
            m_collada->synchronizedByTimeScale();   
		}
		else
		{	
			// calc animation rotate
			m_controlRotate.normalize();		
			float realRot = -getAngle(m_controlRotate, v0);		

			// calc animation blending			
			calcRunAnimationBlend(realRot, 
				m_animForwardFactor, 
				m_animBackwardFactor, 
				m_animLeftFactor, 
				m_animRightFactor);

			m_runFactor = m_runFactor + step;
			if ( m_runFactor > 1.0f )
				m_runFactor = 1.0f;

			m_collada->setAnimWeight(1.0f - m_runFactor, 0);
			m_collada->setAnimWeight(0.0f,					1);
			m_collada->setAnimWeight(m_runFactor*m_animForwardFactor,   2);
			m_collada->setAnimWeight(m_runFactor*m_animBackwardFactor,  3);
			m_collada->setAnimWeight(m_runFactor*m_animLeftFactor,      4);
			m_collada->setAnimWeight(m_runFactor*m_animRightFactor,     5);
                        
            m_collada->synchronizedByTimeScale();
            
            if ( fabs(getAngle(m_controlRotate, runDir)) > 150.0f )
            {
                // need force turn
                setState(CPlayerComponent::PlayerRunTurn);
            }
            
            if ( m_playerMoveEvt.runFast && m_runFactor >= 1.0f )
            {
                // need change state run fast
                setState(CPlayerComponent::PlayerRunToRunFast);
            }
            
		}


		if ( m_runCommand )
		{
			// rotate character
            turnToDir( v0, v1, 2.0f );
			m_gameObject->lookAt( m_gameObject->getPosition() + v0 );

			// calc spine rotation
			core::vector3df lookPos = m_gameObject->getPosition() + m_gameObject->getFront();
			setSpineLookAt( lookPos, 1.0f );
		}

		// update run position
		float runSpeed = m_runSpeed * m_runFactor * getIView()->getTimeStep();
		core::vector3df newPos = m_gameObject->getPosition() + m_controlRotate * runSpeed;
		m_gameObject->setPosition( newPos );

	}
}

void CPlayerComponent::updateStateRunTurn()
{
    static float s_runTurnFactor = 0.0f;
    static float s_controlRotate = 0.0f;
    float invRun = 0.0f;
    
    if ( m_subState == SubStateInit )
	{
        s_runTurnFactor = 0.0f;
        s_controlRotate = m_playerMoveEvt.rotate;
        
		m_subState = SubStateActive;        
    }
	else if ( m_subState == SubStateEnd )
    {
        bool runState = false;
        
        if ( m_nextState == CPlayerComponent::PlayerRun )
            runState = true;

        // change state
		doNextState();
        
        // do not need init run state
        if (runState )
            m_subState = SubStateActive;
    }
    else
    {
        float step = m_runAccel*getIView()->getTimeStep();
        
        if ( m_runCommand == false )
        {
            m_runFactor = m_runFactor - step;
            if ( m_runFactor <= 0 )
                setState( CPlayerComponent::PlayerIdle );
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
		v1 = getCameraFrontVector();
        
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
            
            // change state
            setState(CPlayerComponent::PlayerRun);
        }
        
    }
}

void CPlayerComponent::updateStateRunFast()
{	
	if ( m_subState == SubStateInit )
	{
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
		if ( m_nextState == CPlayerComponent::PlayerIdle )
		{
			m_collada->setAnimWeight(1.0f, 0);
			m_collada->setAnimWeight(0.0f, 1);

			m_collada->setAnimSpeed(1.0f, 0);
			m_collada->onlyEnableAnimTrackChannel(0);
		}
		doNextState();
	}
	else
	{
		float step = m_runAccel*getIView()->getTimeStep();

		if ( m_runCommand == false )
		{
			// calc spine rotation
			core::vector3df lookPos = m_gameObject->getPosition() + m_gameObject->getFront();
			setSpineLookAt( lookPos, 1.0f );

			m_runFactor = m_runFactor - step;
			if ( m_runFactor < 0.0f )
			{
				m_runFactor = 0.0f;
				setState( CPlayerComponent::PlayerIdle );
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
            
            // change to run state
            if ( m_playerMoveEvt.runFast == false )
            {
                setState( CPlayerComponent::PlayerRunFastToRun );
            }
            
		}
        
		core::vector3df v0, v1;

		// get vector rotate & speed
		v0 = m_gameObject->getFront();
		v1 = getCameraFrontVector();
		
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
			setState( CPlayerComponent::PlayerRunFastTurn );			
			m_runTurnVector  = v1;
			m_runCurrentVector = m_gameObject->getFront();
		}
		else
		{
			m_lastRotation = rot;

#ifdef WIN32            
			// debug line
			CGameDebug *debug = CGameDebug::getInstance();
			core::line3df line;
			line.start	= m_gameObject->getPosition();
			line.end	= m_gameObject->getPosition() + v1 * 400.0f;
			debug->addDrawLine(line, SColor(255,255,0,0) ); 
			line.end	= m_gameObject->getPosition() + v0 * 400.0f;
			debug->addDrawLine(line, SColor(255,0,255,0) );
#endif
            
			// step to turn camera vector
			if ( m_runCommand )
			{
				// rotate character
                turnToDir( v0, v1, 2.0f );
				m_gameObject->lookAt( m_gameObject->getPosition() + v0 );

				// calc spine rotation
				if ( m_runFactor > 0.1f )
				{
					core::vector3df lookPos = m_gameObject->getPosition() + getCameraFrontVector();					
					setSpineLookAt( lookPos );
				}
			}
			
			// update run position
			float runSpeed = m_runFastSpeed * m_runFactor * getIView()->getTimeStep();
			core::vector3df newPos = m_gameObject->getPosition() + v0 * runSpeed;
			m_gameObject->setPosition( newPos );
		}
	}
}

void CPlayerComponent::updateStateRunFastTurn()
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
		v1 = m_runCurrentVector + m_runTurnVector;
		v1.normalize();
		
		// step to turn camera vector
        turnToDir( v0, v1, 1.0f );

		// rotate object
		m_gameObject->lookAt( m_gameObject->getPosition() + v0 );

		// calc spine rotation
		core::vector3df lookPos = m_gameObject->getPosition() + getCameraFrontVector();
		setSpineLookAt( lookPos );


		// synch animation to idle
		float step = m_runAccel*getIView()->getTimeStep()*0.8f;
		m_runFactor = m_runFactor - step;
		if ( m_runFactor < 0.0f )
		{
			m_runFactor = 0.0f;
			setState( CPlayerComponent::PlayerRunFast );
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

void CPlayerComponent::updateStateRunToRunFast()
{
    static float s_runFastFactor = 0.0f;
    float invRun = 1.0f - s_runFastFactor;
    
    if ( m_subState == SubStateInit )
	{        
        s_runFastFactor = 0.0f;
        m_runFactor = 1.0f;
        
        m_collada->enableAnimTrackChannel(1, true);
        m_collada->setAnimation(m_animRunNoGun.c_str(), 1, true );
        
		m_subState = SubStateActive;		
	}
	else if ( m_subState == SubStateEnd )
	{	
        bool runFastState = false;
        if ( m_nextState == CPlayerComponent::PlayerRunFast )        
            runFastState = true;

        // change state
		doNextState();
        
        // do not need init run fast
        if ( runFastState )
            m_subState = SubStateActive;
	}
	else
	{	
        float step = m_runToRunFastAccel*getIView()->getTimeStep();
        
        if ( m_runCommand == false )
		{
			m_runFactor = m_runFactor - step;
			if ( m_runFactor < 0.0f )
			{
				m_runFactor = 0.0f;
				setState( CPlayerComponent::PlayerIdle );
			}                   
		}
        else
        {
            m_runFactor = m_runFactor + step;
            if ( m_runFactor > 1.0f )
                m_runFactor = 1.0f;
            
            s_runFastFactor = s_runFastFactor + step;
			if ( s_runFastFactor >= 1.0f )
				s_runFastFactor = 1.0f;

            invRun = 1.0f - s_runFastFactor;
            
            if ( s_runFastFactor >= 1.0f )
            {            
                m_collada->enableAnimTrackChannel(0, true);
                m_collada->enableAnimTrackChannel(1, true);
                
				// rotate object to control dir
				m_gameObject->lookAt( m_gameObject->getPosition() + m_controlRotate );
								
				float spineRotation = getAngle( m_gameObject->getFront(), getCameraFrontVector());
				setSpineRotation(spineRotation);

				// if run back (we do not rotate spine)
				if ( fabsf(m_playerMoveEvt.rotate) >= 150.0f )
					setSpineRotation(0.0f);

				// run fast state
                setState(CPlayerComponent::PlayerRunFast);
            }
        }

		// calc rotate anim (see callback function: _onUpdateFrameDataChannel)
		m_spineBlendRotation = getAngle( m_controlRotate, m_gameObject->getFront() );
		m_rootBlendRotation	 = -m_spineBlendRotation;
		
		// if run back (we do not rotate spine)
		if ( fabsf(m_playerMoveEvt.rotate) >= 150.0f )
			m_spineBlendRotation = 0.0f;

		const float maxSpineAngle = 110.0f;
		m_spineBlendRotation = core::clamp<float>(m_spineBlendRotation, -maxSpineAngle, maxSpineAngle);


		// calc animation blending
        float runFast       = s_runFastFactor*m_runFactor;
        float runForward    = invRun*m_animForwardFactor*m_runFactor;
        float runBackward   = invRun*m_animBackwardFactor*m_runFactor;
        float runLeft       = invRun*m_animLeftFactor*m_runFactor;
        float runRight      = invRun*m_animRightFactor*m_runFactor;
        
        m_collada->setAnimWeight(1.0f - m_runFactor,   0);
        m_collada->setAnimWeight(runFast,       1);
        m_collada->setAnimWeight(runForward,    2);
        m_collada->setAnimWeight(runBackward,   3);
        m_collada->setAnimWeight(runLeft,       4);
        m_collada->setAnimWeight(runRight,      5);
        
        m_collada->synchronizedByTimeScale();
        
        // update run position
		float deltaSpeed = (m_runFastSpeed - m_runSpeed)*s_runFastFactor;
		float runSpeed = (m_runSpeed + deltaSpeed)*m_runFactor* getIView()->getTimeStep();
		core::vector3df newPos = m_gameObject->getPosition() + m_controlRotate * runSpeed;
		m_gameObject->setPosition( newPos );        
    }

}

void CPlayerComponent::updateStateRunFastToRun()
{
	static float s_runFastToRunFactor = 0.0f;
	static float s_runRotate = 0.0f;

    if ( m_subState == SubStateInit )
	{
		// enable multi animation blending
		m_collada->enableAnimTrackChannel(0, true);
		m_collada->enableAnimTrackChannel(1, true);
		m_collada->enableAnimTrackChannel(2, true);
		m_collada->enableAnimTrackChannel(3, true);
		m_collada->enableAnimTrackChannel(4, true);
		m_collada->enableAnimTrackChannel(5, true);

		m_collada->setCrossFadeAnimation( m_animIdle[0].c_str(),0 );
		m_collada->setAnimation(m_animRunForward.c_str(),       2, true );
		m_collada->setAnimation(m_animRunBackward.c_str(),      3, true );
		m_collada->setAnimation(m_animRunStrafeRight.c_str(),   4, true );
		m_collada->setAnimation(m_animRunStrafeLeft.c_str(),    5, true );        
        
        // set weight run anim = zero
		m_collada->setAnimWeight(1.0f - m_runFactor, 0);        
		m_collada->setAnimWeight(m_runFactor, 1);
		m_collada->setAnimWeight(0.0f, 2);
		m_collada->setAnimWeight(0.0f, 3);
		m_collada->setAnimWeight(0.0f, 4);
		m_collada->setAnimWeight(0.0f, 5);
        
		m_collada->synchronizedByTimeScale();	

		s_runFastToRunFactor = 0.0f;
		s_runRotate = m_playerMoveEvt.rotate;

        core::vector3df v0, v1, runDir;
                
		// get vector rotate & speed
		v0 = m_gameObject->getFront();
		v1 = getCameraFrontVector();
        runDir = v1;
        
        // calc blend (see function callback
        m_spineBlendRotation = getAngle(v0, v1);
        m_rootBlendRotation = -m_spineBlendRotation;
        const float maxSpineAngle = 110.0f;
		m_spineBlendRotation = core::clamp<float>(m_spineBlendRotation, -maxSpineAngle, maxSpineAngle);
        
        // rotate obj to camera
        m_gameObject->lookAt( m_gameObject->getPosition() + getCameraFrontVector() );
        
		// rotate rundir
		core::quaternion q;        
		q.fromAngleAxis( core::degToRad(s_runRotate), core::vector3df(0,1,0) );
		q.getMatrix().rotateVect(runDir);
		runDir.normalize();		
                
		// fix rotate
		m_controlRotate = runDir;
        setSpineRotation( 0.0f );
        
        // calc animation blending			
		calcRunAnimationBlend(s_runRotate,
                              m_animForwardFactor, 
                              m_animBackwardFactor, 
                              m_animLeftFactor, 
                              m_animRightFactor);        
        
        // active state
		m_subState = SubStateActive;		
	}
	else if ( m_subState == SubStateEnd )
	{
		bool runState = false;
		if ( m_nextState == CPlayerComponent::PlayerRun )        
            runState = true;

        // change state
		doNextState();
        
        // do not need init run
        if ( runState )
            m_subState = SubStateActive;
	}
	else
	{
		float step = m_runToRunFastAccel*getIView()->getTimeStep();
        
        if ( m_runCommand == false )
		{
			m_runFactor = m_runFactor - step;
			if ( m_runFactor < 0.0f )
			{
				m_runFactor = 0.0f;
				setState( CPlayerComponent::PlayerIdle );
			}                   
		}
        else
        {
            m_runFactor = m_runFactor + step;
            if ( m_runFactor > 1.0f )
                m_runFactor = 1.0f;
            
            s_runFastToRunFactor = s_runFastToRunFactor + step;

			if ( s_runFastToRunFactor >= 1.0f )
            {
				s_runFastToRunFactor = 1.0f;
				setState(CPlayerComponent::PlayerRun);
            }
        }

        float runFast       = (1.0f - s_runFastToRunFactor)*m_runFactor;
        float runForward    = s_runFastToRunFactor*m_animForwardFactor*m_runFactor;
        float runBackward   = s_runFastToRunFactor*m_animBackwardFactor*m_runFactor;
        float runLeft       = s_runFastToRunFactor*m_animLeftFactor*m_runFactor;
        float runRight      = s_runFastToRunFactor*m_animRightFactor*m_runFactor;        
        
        
        m_collada->setAnimWeight(1.0f - m_runFactor,   0);
        m_collada->setAnimWeight(runFast,       1);
        m_collada->setAnimWeight(runForward,    2);
        m_collada->setAnimWeight(runBackward,   3);
        m_collada->setAnimWeight(runLeft,       4);
        m_collada->setAnimWeight(runRight,      5);
        
        m_collada->synchronizedByTimeScale();
        
        
        // update run position
		float deltaSpeed = (m_runFastSpeed - m_runSpeed)*s_runFastToRunFactor;
		float runSpeed = (m_runFastSpeed - deltaSpeed)* m_runFactor * getIView()->getTimeStep();
		core::vector3df newPos = m_gameObject->getPosition() + m_controlRotate*runSpeed;
		m_gameObject->setPosition( newPos );
    }    
}


//void CPlayerComponent::updateStateTEMPLATE()
//{
//    if ( m_subState == SubStateInit )
//	{        
//		m_subState = SubStateActive;		
//	}
//	else if ( m_subState == SubStateEnd )
//	{		
//		doNextState();		
//	}
//	else
//	{	
//  }
//}

///////////////////////////////////////////////////////////////////////
// Player component end update state function
///////////////////////////////////////////////////////////////////////


// _onUpdateFrameData
// call back frame update on scenenode
void CPlayerComponent::_onUpdateFrameData( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation )
{
    // todo modify rotation, position of anim    
	const core::vector3df rotAxis = core::vector3df(0,0,1);

	const float maxSpine = 70.0f;
	float spineAngle = core::clamp<float>(m_spineRotation, -maxSpine, maxSpine);
	float neckAngle = m_spineRotation - spineAngle;
	float r = spineAngle/3.0f;
	
	if ( node == m_nodesChest[0] )
	{
		core::quaternion q;
		q.fromAngleAxis( core::degToRad(r), rotAxis  );
		rotation = rotation * q;
	}
	else if ( node == m_nodesChest[1] )
	{
		core::quaternion q;
		q.fromAngleAxis( core::degToRad(r), rotAxis );
		rotation = rotation * q;
	}
	else if ( node == m_nodesChest[2] )
	{
		core::quaternion q;
		q.fromAngleAxis( core::degToRad(r), rotAxis );
		rotation = rotation * q;
	}
	else if ( node == m_nodeNeck )
	{
		core::quaternion q;
		core::vector3df neckAxis = core::vector3df(0.0f,-0.8f,1.0f);
		neckAxis.normalize();
		q.fromAngleAxis( core::degToRad( neckAngle ), neckAxis );
		rotation = rotation * q;
	}
}

void CPlayerComponent::_onUpdateFrameDataChannel( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int channel )
{
	if ( (m_state == CPlayerComponent::PlayerRunToRunFast || m_state == CPlayerComponent::PlayerRunFastToRun )&& channel == 1 )
	{
		ISceneNode *root = m_collada->getSceneNode("Reference");

		
		// todo modify rotation, position of anim    
		const core::vector3df rotAxis = core::vector3df(0,0,1);

		const float maxSpine = 70.0f;
		float spineAngle = core::clamp<float>(m_spineBlendRotation, -maxSpine, maxSpine);
		float neckAngle = m_spineBlendRotation - spineAngle;
		float r = spineAngle/3.0f;

		// we need rotate the animation channel
		if ( node == root )
		{
			core::quaternion q;
			const core::vector3df rotAxis = core::vector3df(0,1,0);

			q.fromAngleAxis( core::degToRad( m_rootBlendRotation ), rotAxis );
			rotation = rotation * q;
		}	
		else if ( node == m_nodesChest[0] )
		{
			core::quaternion q;
			q.fromAngleAxis( core::degToRad(r), rotAxis  );
			rotation = rotation * q;
		}
		else if ( node == m_nodesChest[1] )
		{
			core::quaternion q;
			q.fromAngleAxis( core::degToRad(r), rotAxis );
			rotation = rotation * q;
		}
		else if ( node == m_nodesChest[2] )
		{
			core::quaternion q;
			q.fromAngleAxis( core::degToRad(r), rotAxis );
			rotation = rotation * q;
		}
		else if ( node == m_nodeNeck )
		{
			core::quaternion q;
			core::vector3df neckAxis = core::vector3df(0.0f,-0.8f,1.0f);
			neckAxis.normalize();
			q.fromAngleAxis( core::degToRad( neckAngle ), neckAxis );
			rotation = rotation * q;
		}
	}

}


///////////////////////////////////////////////////////////////////////
// Player component end callback animation function
///////////////////////////////////////////////////////////////////////


// stepAnimationTime	
void CPlayerComponent::stepAnimationTime()
{
	float timeStep = getIView()->getTimeStep();
	m_animCurrentTime = m_animCurrentTime - timeStep;
	
	if ( m_animCurrentTime < 0 )
		m_animCurrentTime = 0;
}

// calcRunAnimationBlend
// calc animation
void CPlayerComponent::calcRunAnimationBlend(float rot, float &forward, float &backward, float &left, float &right)
{
	forward = 0.0f;
	backward = 0.0f;
	left = 0.0f;
	right = 0.0f;
			
	if ( -90.0f <= rot && rot <= 90.0f )
	{
		// move forward		
		backward = 0.0f;
		if ( rot <= 0.0f && rot <= 90.0f )
		{
			// right
			left = 0.0f;

			float fixAngle	= core::degToRad(fmodf(rot + 90.0f, 360.0f));
			float dForward	= fabs(sinf(fixAngle));
   
			// we have sin2 + cos2 = 1
            // it mean dforward2 + dright2 = 1.0f
            forward = dForward*dForward;
			right   = 1.0f - forward;
		}
		else
		{
			// left
			right = 0.0f;

			float fixAngle	= core::degToRad(fmodf(rot + 90.0f, 360.0f));
			float dForward	= fabs(sinf(fixAngle));
			
            forward = dForward*dForward;
            left = 1.0f - forward;
		}
	}
	else
	{
		// move back
		forward = 0.0f;
		if ( 90.0f <= rot && rot <= 180.0f )
		{
			// left
			right = 0.0f;

			float fixAngle	= core::degToRad(fmodf(rot + 90.0f, 360.0f));
			float dBackward	= fabs(sinf(fixAngle));			

            backward    = dBackward*dBackward;
            left        = 1.0f - backward;
		}
		else
		{
			// right
			left = 0.0f;

			float fixAngle	= core::degToRad(fmodf(rot + 90.0f, 360.0f));
			float dBackward	= fabs(sinf(fixAngle));
			
            backward    = dBackward*dBackward;
            right       = 1.0f - backward;
		}
	}
    
}

// isFinishedAnim	
bool CPlayerComponent::isFinishedAnim( std::vector<CGameColladaSceneNode*>& nodes, int trackChannel )
{
	for ( int i = 0, n = (int)nodes.size(); i < n; i++ )
	{		
		CGameAnimationTrack* track = nodes[i]->getAnimation()->getTrack(trackChannel);

		if ( track->getTotalFrame() == 0 || track->isEnable() == false || track->isPause() == true )
			continue;

		if ( track->isEndTrack() == false || track->isCrossAnim() )
			return false;
	}


	return true;
}

// updateWeaponPosition
// update weapon
void CPlayerComponent::updateWeaponPosition()
{	
	if ( m_inventory && m_collada )
	{
		CInventoryComponent::SInventoryItem* item = m_inventory->getActiveItem();
		if ( item && item->m_item )
		{
			CWeaponComponent* weapon = (CWeaponComponent*)item->m_item->getComponent( CGameComponent::WeaponComponent );
			if ( weapon )
			{
				if ( weapon->getWeaponType() == CWeaponComponent::ShotGun )
				{					
				}		
			}
		}	// if has active item
	}	// if has inventory
}

// getCurrentWeapon
// get weapon
CWeaponComponent* CPlayerComponent::getCurrentWeapon()
{
	if ( m_inventory && m_collada )
	{
		CInventoryComponent::SInventoryItem* item = m_inventory->getActiveItem();
		if ( item && item->m_item )
		{
			CWeaponComponent* weapon = (CWeaponComponent*)item->m_item->getComponent( CGameComponent::WeaponComponent );
			return weapon;			
		}
	}
	return NULL;
}

// setSpineLookAt
// rotate spine to look at a pos
void CPlayerComponent::setSpineLookAt( const core::vector3df& pos, float speed )
{
	core::vector3df lookAt = pos - m_gameObject->getPosition();
	lookAt.normalize();

	float a = getAngle( m_gameObject->getFront(), lookAt );
	
	speed = speed * getIView()->getTimeStep() * 0.1f;
	if ( m_spineRotation < a )
		m_spineRotation = m_spineRotation+speed;
	else if ( m_spineRotation > a )
		m_spineRotation = m_spineRotation-speed;

	if ( fabs(m_spineRotation-a) <= speed )
		m_spineRotation = a;

	setSpineRotation(m_spineRotation);
}


// getCameraFrontVector
// return camera front vector
core::vector3df CPlayerComponent::getCameraFrontVector()
{
	CGameCamera* cam = CGameLevel::getCurrentLevel()->getCamera();
	core::vector3df front = cam->getTarget() - cam->getPosition();
	front.Y = 0;
	front.normalize();
	return front;
}

// turnToDir
bool CPlayerComponent::turnToDir(core::vector3df& dir, const core::vector3df& turnTo, float speed )
{
	// get speed need
	speed = getRatioWithAngle(dir, turnTo, speed);

	// hack if speed is too small
	if ( speed < 0.02f )
		speed = 0.02f;

	// rotate front vec
	float f = speed*0.1f*getIView()->getTimeStep();	
	if ( f >= 1.0f )
	{		
		dir = turnTo;
		return true;
	}
	
	dir = interpolateTurnToDir( dir, turnTo, f );
	return false;
}

// turnToDir
// turn vector dir to turnTo
core::vector3df CPlayerComponent::interpolateTurnToDir( const core::vector3df& turnFrom, const core::vector3df& turnTo, float f )
{
	// calc turn Direction
	core::vector3df normal;
	normal = turnTo.crossProduct(turnFrom);
	normal.normalize();

	// fix f factor
	f = core::clamp( f, 0.0f, 1.0f );

	// calc angle
	float angle = turnTo.dotProduct( turnFrom );
	angle = fixAngle(angle);
	angle = acos(angle);
	
	// interpolate	
	core::quaternion q, q1,q2;
	
	q1.fromAngleAxis( 0,		normal );
	q2.fromAngleAxis( angle,	normal );
	q.slerp( q1, q2, f );

	// rotate result vector
	core::vector3df dir = turnFrom;
	q.getMatrix().rotateVect( dir );
	dir.normalize();
	return dir;	
}

// getRatioWithAngle	
float CPlayerComponent::getRatioWithAngle( const core::vector3df& turnFrom, const core::vector3df& turnTo, float angle )
{
	float angleVec = turnTo.dotProduct( turnFrom );		
	angleVec = fixAngle(angleVec);
	angleVec = core::radToDeg( acos(angleVec) );
	
	if ( realIsZero(angleVec) )
		return 1.0f;

	return angle/angleVec;
}

float CPlayerComponent::getAngle( const core::vector3df& v1, const core::vector3df& v2 )
{
	core::vector3df normal = v2.crossProduct(v1);	
	
	float angleVec = v2.dotProduct( v1 );
	
	angleVec = fixAngle(angleVec);
	angleVec = core::radToDeg( acos(angleVec) );

	if ( normal.Y < 0 )
		angleVec = -angleVec;
	return angleVec;
}

// fixAngle
float CPlayerComponent::fixAngle( float f )
{
	if (realIsEqual(f, 1.f, 0.001f))
		f = 1.f;
	else if (realIsEqual(f, -1.f, 0.001f))
		f = -1.f;
	return f;
}