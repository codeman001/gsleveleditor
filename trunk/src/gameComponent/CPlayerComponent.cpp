#include "stdafx.h"
#include "IView.h"
#include "IRandomizer.h"

#include "CZone.h"
#include "CLightObject.h"

#include "CPlayerComponent.h"

#include "CParticleComponent.h"
#include "CColladaMeshComponent.h"
#include "CGunLightComponent.h"
#include "CShadowComponent.h"
#include "CNetworkPlayerComponent.h"

#include "gameLevel/CGameLevel.h"
#include "gameDebug/CGameDebug.h"

///////////////////////////////////////////////////////////////////////
// IObjectComponent overide implement
///////////////////////////////////////////////////////////////////////

CPlayerComponent::CPlayerComponent(CGameObject* obj)
	:IObjectComponent(obj, CGameComponent::PlayerComponent)
{	
	m_collada	= NULL;
    
	m_muzzleMeshTime = 0;
	m_gunMuzzle = NULL;
    m_gunLight  = NULL;
    m_gunLightComp = NULL;
    m_bullet = NULL;
	m_bulletRayComp = NULL;
}

CPlayerComponent::~CPlayerComponent()
{
	// unregister event
	getIView()->unRegisterEvent( this );

	// delete gunlight & muzzle mesh
	delete m_gunLight;
	delete m_gunMuzzle;
	delete m_bullet;
}

// init
// run when init object
void CPlayerComponent::initComponent()
{
    m_collada = (CColladaMeshComponent*)m_gameObject->getComponent( IObjectComponent::ColladaMesh );
    m_collada->setAnimationPackage( m_animationPackage );
    
    init(m_gameObject);    

    // apply animation callback
	CGameColladaSceneNode *spine = m_collada->getSceneNode("Spine");
	spine->setAnimationCallback(this);
	m_nodesChest.push_back( spine );
	
	spine = m_collada->getSceneNode("Spine1");
	spine->setAnimationCallback(this);	
	m_nodesChest.push_back( spine );
    
	spine = m_collada->getSceneNode("Spine2");
	spine->setAnimationCallback(this);
	m_nodesChest.push_back( spine );
    
	CGameColladaSceneNode *root = m_collada->getSceneNode("Reference");
	root->setAnimationCallback(this);
    
	m_nodeNeck = m_collada->getSceneNode("Neck");
	m_nodeNeck->setAnimationCallback(this);
    
    
    
	// register event
	getIView()->registerEvent("CPlayerComponent", this);	
    
    
    
    
    // create gunlight obj
	m_gunLight = new CLightObject(NULL);
    m_gunLight->setLightData(400.0f, 1.0f);
    m_gunLightComp = new CGunLightComponent(m_gunLight);
    m_gunLight->addComponent(m_gunLightComp);
	m_gunLight->setParent( m_gameObject );

	// create a gun muzzle mesh obj
	m_gunMuzzle = new CGameObject(NULL);
	CColladaMeshComponent *gunMesh = new CColladaMeshComponent(m_gunMuzzle);
	gunMesh->loadFromFile( getIView()->getPath("data/mesh/character/hero/muzzleFlash.scene"));
	m_gunMuzzle->addComponent(gunMesh);
	m_gunMuzzle->setParent(m_gameObject);

	// create bullet
	m_bullet = new CGameObject(NULL);
	m_bulletRayComp = new CBulletRayComponent(m_bullet);
	m_bullet->addComponent(m_bulletRayComp);
	m_bullet->setParent(m_gameObject);
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
    
	// only update state on current player	
	if ( m_gameObject->isNetworkController() == false )
		updateState();	

	// muzzle mesh update
	updateMuzzleMesh();

	// update gun muzzle & gun light
	m_gunLight->updateObject();
	m_gunMuzzle->updateObject();
	m_bullet->updateObject();

	// set shadow
	CZone *zone = (CZone*)m_gameObject->getParent();
	CGameObject *shadowObj = zone->getStaticShadowManager();
	CShadowComponent* shadowComp = (CShadowComponent*)shadowObj->getComponent(IObjectComponent::Shadow);
	if ( shadowComp )
		shadowComp->setShadowNode( m_gameObject->getPosition(), core::vector3df(0,1,0) );
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

// packDataMultiplayer
// pack data multiplayer
void CPlayerComponent::packDataMultiplayer(CDataPacket *packet)
{
	CNetworkPlayerComponent *comp = (CNetworkPlayerComponent*)m_gameObject->getComponent(CGameComponent::NetworkPlayerComponent);
	if ( comp == NULL )
		return;

	// packet player control
	packet->addFloat(m_playerMoveEvt.rotate);
	packet->addFloat(m_playerMoveEvt.strength);
	packet->addByte(m_playerMoveEvt.run);
	packet->addByte(m_playerMoveEvt.runFast);
	
	packet->addByte(m_playerCmdEvt.shoot);
	packet->addByte(m_playerCmdEvt.reload);

	// pack player state
	packet->addByte((unsigned char)m_subState);
	packet->addByte((unsigned char)m_state);

	packet->addByte((unsigned char)m_upbodySubState);
	packet->addByte((unsigned char)m_upbodyState);

	// pack mp state
	packDataMPState(packet);
}
    
// unPackDataMultiplayer
// unpack data on multiplayer
void CPlayerComponent::unpackDataMultiplayer(CDataPacket *packet)
{
	CNetworkPlayerComponent *comp = (CNetworkPlayerComponent*)m_gameObject->getComponent(CGameComponent::NetworkPlayerComponent);
	if ( comp )
		comp->unpackDataFromPlayerComponent(packet);
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
        else if ( irrEvent.GameEvent.EventID == EvtPlayerCommand )
        {
            m_playerCmdEvt = *((SEventPlayerCommand*)irrEvent.GameEvent.EventData);
            
            if ( m_playerCmdEvt.shoot || m_playerCmdEvt.reload )
                m_gunOnCommand = true;
            else
                m_gunOnCommand = false;
        }
	}

	return true;
}


void CPlayerComponent::packDataMPState(CDataPacket *packet)
{
	switch( m_state )
	{
	case CPlayerComponent::PlayerIdle:
		packDataStateIdle(packet);			
		break;
	case CPlayerComponent::PlayerTurn:
		packDataStateTurn(packet);			
		break;
	case CPlayerComponent::PlayerRunTurn:
		packDataStateRunTurn(packet);
		break;
	case CPlayerComponent::PlayerRun:
		packDataStateRun(packet);
		break;
    case CPlayerComponent::PlayerRunFastTurn:
        packDataStateRunFastTurn(packet);    
        break;
	case CPlayerComponent::PlayerRunFast:
		packDataStateRunFast(packet);
		break;
    case CPlayerComponent::PlayerRunToRunFast:
        packDataStateRunToRunFast(packet);
        break;
    case CPlayerComponent::PlayerRunFastToRun:
        packDataStateRunFastToRun(packet);
        break;
    case CPlayerComponent::PlayerStandAim:
        packDataStateStandAim(packet);			
        break;
    case CPlayerComponent::PlayerRotate:
        packDataStatePlayerRotate(packet);
        break;
	}
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
    case CPlayerComponent::PlayerStandAim:
        updateStateStandAim();			
        break;
    case CPlayerComponent::PlayerRotate:
        updateStatePlayerRotate();
        break;
	}

	// update body
	updateUpperBody();
}


// updateMuzzleMesh
void CPlayerComponent::updateMuzzleMesh()
{
	if ( m_muzzleMeshTime > 0 )
	{
		m_muzzleMeshTime = m_muzzleMeshTime - getIView()->getTimeStep();
		if ( m_muzzleMeshTime <= 0.0f )
			m_muzzleMeshTime = 0.0f;

		// update muzzle mesh position
		CGameColladaSceneNode *gunTip = m_collada->getSceneNode("RightGunTip");
		core::matrix4 mat = gunTip->getAbsoluteTransformation();
		core::quaternion rot;
		rot.fromAngleAxis(core::degToRad(90.0f), core::vector3df(0,1,0));
		mat *= rot.getMatrix();
        
        core::vector3df position(mat.getTranslation());
        core::vector3df rotation(mat.getRotationDegrees());
        
		m_gunMuzzle->setPosition(position);
		m_gunMuzzle->setRotation(rotation);
        
		m_gunMuzzle->setVisible(true);
	}
	else
	{
		m_gunMuzzle->setVisible(false);
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
        m_idleAnimationID = r;
        const char *anim = m_animIdle[r].c_str();

		// change idle animation
        m_collada->setCrossFadeAnimation( anim, 0, 10.0f, false, 1 );
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
			m_subState = SubStateInit;
	}
}

// mp sync
void CPlayerComponent::packDataStateIdle(CDataPacket *packet)
{
    packet->addByte((unsigned char)m_idleAnimationID);
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
		return;
	}
	
	core::vector3df v0, v1;

	// get vector rotate & speed
	v0 = m_gameObject->getFront();
	v1 = getCameraFrontVector();
    
    // sync multiplayer
    m_MPRotateVector = v1;
    
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
		if ( m_runCommand )
		{
			if ( s_runFast )
				setState( CPlayerComponent::PlayerRunFast );
			else
				setState( CPlayerComponent::PlayerRun );
		}		
		else
		{
			setState( CPlayerComponent::PlayerIdle );
		}
	}
}

void CPlayerComponent::packDataStateTurn(CDataPacket *packet)
{
    packet->addFloat(m_MPRotateVector.X);
    packet->addFloat(m_MPRotateVector.Y);
    packet->addFloat(m_MPRotateVector.Z);    
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
		v1 = getCameraFrontVector();
		runDir = v0;		

        // sync multiplayer
        m_MPRotateVector = v1;
        
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
/*        
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
*/
		float step = m_runAccel*getIView()->getTimeStep();
        
		if ( m_runCommand == false )
		{
			m_runFactor = m_runFactor - step;
			if ( m_runFactor < 0.0f )
			{
				m_runFactor = 0.0f;
				setState( CPlayerComponent::PlayerStandAim );
			}
            
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
			// calc animation rotate
			m_controlRotate.normalize();		
			float realRot = -getAngle(m_controlRotate, v0);		
            
            // mp sync
            m_MPRunRotate = realRot;
            
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
            m_collada->setAnimWeight(0.0f, 1);
            		
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
			//core::vector3df lookPos = m_gameObject->getPosition() + m_gameObject->getFront();
            //setSpineLookAt( lookPos, 1.0f );
		}

		// update run position
		float runSpeed = m_runSpeed * m_runFactor * getIView()->getTimeStep();
		core::vector3df newPos = m_gameObject->getPosition() + m_controlRotate * runSpeed;
		m_gameObject->setPosition( newPos );

	}       
}

void CPlayerComponent::packDataStateRun(CDataPacket *packet)
{
    packet->addFloat(m_MPRotateVector.X);
    packet->addFloat(m_MPRotateVector.Y);
    packet->addFloat(m_MPRotateVector.Z);
    
    packet->addFloat(m_runFactor);
    packet->addFloat(m_MPRunRotate);
}

// updateStateRunTurn
// turn if rot > 150
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
            {
                m_runFactor = 0;
                setState( CPlayerComponent::PlayerIdle );
            }
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
        
        // sync mp
        m_MPRotateVector = v1;
        
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
        
        m_MPRunTurnFactor   = s_runTurnFactor;
        m_MPRunRotate       = s_controlRotate;
    }
}

void CPlayerComponent::packDataStateRunTurn(CDataPacket *packet)
{
    packet->addFloat(m_MPRotateVector.X);
    packet->addFloat(m_MPRotateVector.Y);
    packet->addFloat(m_MPRotateVector.Z);    
    
    packet->addFloat(m_MPRunTurnFactor);
    packet->addFloat(m_MPRunRotate);
}




void CPlayerComponent::updateStateRunFast()
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
		if ( m_nextState == CPlayerComponent::PlayerIdle )
		{
            // turn off multi anim track
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
        
		if ( m_runCommand == false && m_upbodyState == CPlayerComponent::PlayerUpBodyRunFast )
		{
            
            // calc spine rotation
            //core::vector3df lookPos = m_gameObject->getPosition() + m_gameObject->getFront();
            //setSpineLookAt( lookPos, 1.0f );
            
            m_runFactor = m_runFactor - step;

            if ( m_runFactor < 0.0f )
            {
                m_runFactor = 0.0f;
                
                setUpBodyState(CPlayerComponent::PlayerUpBodyOffGunToAim);
                setState( CPlayerComponent::PlayerRotate );
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
            // only change when ready run fast state
            if ( m_playerMoveEvt.runFast == false )
            {    
                setUpBodyState(CPlayerComponent::PlayerUpBodyOffGunToAim);
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
/*
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
*/            
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

void CPlayerComponent::packDataStateRunFast(CDataPacket *packet)
{
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
		v1 = m_runTurnVector;
		v1.normalize();
		
        // if rot 180 deg
        core::vector3df v = v0 + v1;
        if ( v.getLengthSQ() <= 0.1f )
            v1 = m_gameObject->getRight();
        
		// step to turn camera vector
        bool turn = turnToDir( v0, v1, 5.0f );
/*
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
*/
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

void CPlayerComponent::packDataStateRunFastTurn(CDataPacket *packet)
{
}


void CPlayerComponent::updateStateRunToRunFast()
{   
    if ( m_subState == SubStateInit )
	{               
        // change state
		m_subState = SubStateActive;
        
        // upper state
        setUpBodyState( CPlayerComponent::PlayerUpBodyAimToOffGun );
	}
	else if ( m_subState == SubStateEnd )
	{	       
        // change state
		doNextState();     
	}
	else
	{		        
		float step = m_runAccel*getIView()->getTimeStep()*1.0f;        
        m_runFactor = m_runFactor - step;
                
        if ( m_runFactor < 0.0f )
        {
            m_runFactor = 0.0f;            
            setState( CPlayerComponent::PlayerRunFast );
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

void CPlayerComponent::packDataStateRunToRunFast(CDataPacket *packet)
{
}

void CPlayerComponent::updateStateRunFastToRun()
{
    if ( m_subState == SubStateInit )
	{		
        setUpBodyState(CPlayerComponent::PlayerUpBodyOffGunToAim);
        
        // active state
		m_subState = SubStateActive;		
	}
	else if ( m_subState == SubStateEnd )
	{        
        // change state
		doNextState();
	}
	else
	{
        core::vector3df v0, v1, runDir;
        
		// get vector rotate & speed
		v0 = m_gameObject->getFront();
		v1 = getCameraFrontVector();
        runDir = v0;
        
		float step = m_runAccel*getIView()->getTimeStep()*1.0f;
        
        m_runFactor = m_runFactor - step;
        
        if ( m_runFactor < 0.0f )
        {
            m_runFactor = 0.0f;            
            setState( CPlayerComponent::PlayerRun );
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

void CPlayerComponent::packDataStateRunFastToRun(CDataPacket *packet)
{
}

void CPlayerComponent::updateStateStandAim()
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
			core::line3df	ray		= getCameraRay();
			core::vector3df	colPos	= getCollisionPoint(ray);                		        		 
					
			// rotate main character		
			core::vector3df v0 = m_gameObject->getFront();
			core::vector3df aimPos = colPos - m_gameObject->getPosition();
			aimPos.Y = 0;
			aimPos.normalize();

			if ( turnToDir( v0, aimPos, 6.0f ) == true )
				m_aimRotateCharacter = false;

			m_gameObject->lookAt( m_gameObject->getPosition() + v0 );
		}		

		// change state un
        if ( m_runCommand )
            setState(CPlayerComponent::PlayerRun);		
    }
}

void CPlayerComponent::packDataStateStandAim(CDataPacket *packet)
{
	packet->addFloat(m_MPRotateVector.X);
    packet->addFloat(m_MPRotateVector.Y);
    packet->addFloat(m_MPRotateVector.Z);
}

void CPlayerComponent::updateStatePlayerRotate()
{
    if ( m_subState == SubStateInit )
    {        
        m_subState = SubStateActive;		
    }
    else if ( m_subState == SubStateEnd )
    {		
        doNextState();		
    }
    else
    {
        core::line3df	ray		= getCameraRay();
        core::vector3df	colPos	= getCollisionPoint(ray);                		        		 
        
        // rotate main character		
        core::vector3df v0 = m_gameObject->getFront();
        core::vector3df aimPos = colPos - m_gameObject->getPosition();
        aimPos.Y = 0;
        aimPos.normalize();
        
        // set vector for packmultiplayer
        m_MPRotateVector = aimPos;
        
        if ( turnToDir( v0, aimPos, 6.0f ) == true )
        {
            setState(CPlayerComponent::PlayerStandAim);
            setUpBodyState(CPlayerComponent::PlayerUpBodyAim);
        }
        m_gameObject->lookAt( m_gameObject->getPosition() + v0 );
    }    
}

void CPlayerComponent::packDataStatePlayerRotate(CDataPacket *packet)
{
    packet->addFloat(m_MPRotateVector.X);
    packet->addFloat(m_MPRotateVector.Y);
    packet->addFloat(m_MPRotateVector.Z);
}


//void CPlayerComponent::updateStateTEMPLATE()
//{
//	if ( m_subState == SubStateInit )
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


// updateUpperBody
// update state aim, shoot, reload
void CPlayerComponent::updateUpperBody()
{
    if ( m_upbodyState == CPlayerComponent::PlayerUpBodyAim )
    {
        updateUpperBodyAim();
    }
    else if ( m_upbodyState == CPlayerComponent::PlayerUpBodyShoot )
    {
        updateUpperBodyShoot();
    }
    else if ( m_upbodyState == CPlayerComponent::PlayerUpBodyReload )
    {
        updateUpperBodyReload();
    }
	else if ( m_upbodyState == CPlayerComponent::PlayerUpBodyOffGun )
	{
		updateUpperBodyOffgun();
	}
    else if ( m_upbodyState == CPlayerComponent::PlayerUpBodyAimToOffGun )
    {
        updateUpperBodyAimToOffgun();
    }
    else if ( m_upbodyState == CPlayerComponent::PlayerUpBodyOffGunToAim )
    {
        updateUpperBodyOffgunToAim();
    }
    else if ( m_upbodyState == CPlayerComponent::PlayerUpBodyRunFast )
    {
        updateUpperBodyRunFast();
    }
}

void CPlayerComponent::updateUpperBodyAim()
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
        
        core::line3df	ray		= getCameraRay();
		core::vector3df	colPos	= getCollisionPoint(ray);
        
		core::vector2df ret = getAimAngle(colPos);
        
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
		core::vector3df v0 = m_gameObject->getFront();
		core::vector3df aimPos = colPos - m_gameObject->getPosition();
		aimPos.Y = 0;
		aimPos.normalize();
		float angle = getAngle(v0,aimPos);
		if ( fabsf(angle) > 40.0f )
			m_aimRotateCharacter = true;

		angle = core::clamp<float>(angle, -40.0f, 40.0f);
		setSpineRotation(angle);
        
        // blend anim up, down
        m_collada->setAnimWeight(1.0f - m_aimFactor, 0, 1);			// idle
        m_collada->setAnimWeight(wStraight, 1, 1);					// straight
		m_collada->setAnimWeight(wUp,	2, 1);						// up
		m_collada->setAnimWeight(wDown, 3, 1);						// down
        m_collada->synchronizedByTimeScale(1);    
        
		if ( m_gunOnCommand && m_aimFactor == 1.0f )
		{
			if ( m_playerCmdEvt.shoot == true )
			{
				// todo shoot
                setUpBodyState( CPlayerComponent::PlayerUpBodyShoot );
			}
			else if ( m_playerCmdEvt.reload == true )
			{
				// todo reload
				setUpBodyState( CPlayerComponent::PlayerUpBodyReload );
			}
		}        
    }
}

void CPlayerComponent::updateUpperBodyShoot()
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
		{
			if ( m_playerCmdEvt.shoot == false )
			{
				// change to aim state if end shooting
                setUpBodyState(CPlayerComponent::PlayerUpBodyAim);
			}
			else
			{
				// continue shoot
                m_animCurrentTime = m_collada->getCurrentAnimTimeLength()/shootSpeed;
                
				// reset frame
				m_collada->setCurrentFrame(0, 1, 1);
				m_collada->setCurrentFrame(0, 2, 1);
				m_collada->setCurrentFrame(0, 3, 1);

				CGameColladaSceneNode *gunTip = m_collada->getSceneNode("RightGunTip");
        
				// active gunlight
				core::vector3df gunPos = gunTip->getAbsolutePosition();
				m_gunLight->setPosition(gunPos);
				m_gunLightComp->setLightTime(100.0f);

				// show muzzle
				showMuzzle(flashTime);

                // setup player state
                m_playerCmdEvt.shoot	= false;
                m_playerCmdEvt.reload	= false;

				// spawn bullet
				s_spawnBullet = true;
                
			}
		}
        
        core::line3df	ray		= getCameraRay();
		core::vector3df	colPos	= getCollisionPoint(ray);
        
		if ( s_spawnBullet == true )
		{
			CGameColladaSceneNode *gunTip = m_collada->getSceneNode("RightGunTip");
			core::vector3df gunPos = gunTip->getAbsolutePosition();

			core::line3df bulletRay;
			bulletRay.start = gunPos;
			bulletRay.end = colPos;

			m_bulletRayComp->addBulletRay(bulletRay);
			s_spawnBullet = false;
		}

		// rotate spine character
		core::vector3df v0 = m_gameObject->getFront();
		core::vector3df aimPos = colPos - m_gameObject->getPosition();
		aimPos.Y = 0;
		aimPos.normalize();
		float angle = getAngle(v0,aimPos);
		if ( fabsf(angle) > 40.0f )
			m_aimRotateCharacter = true;

		angle = core::clamp<float>(angle, -40.0f, 40.0f);
		setSpineRotation(angle);


		// blend to up/down hand
		core::vector2df ret = getAimAngle(colPos);
        
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

void CPlayerComponent::updateUpperBodyReload()
{
    if ( m_upbodySubState == SubStateInit )
    {        
        m_upbodySubState = SubStateActive;		
    }
    else if ( m_upbodySubState == SubStateEnd )
    {
    }
    else
    {	
    }    
}

void CPlayerComponent::updateUpperBodyOffgun()
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
    else
	{
		core::line3df	ray		= getCameraRay();
		core::vector3df	colPos	= getCollisionPoint(ray);
        
		core::vector2df ret = getAimAngle(colPos);
        
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

void CPlayerComponent::updateUpperBodyAimToOffgun()
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
    else
    {
        // change state if finish animation
        if ( m_collada->isEndAnimation(0, 1) == true )
        {                                     
            m_upBodyRunFastFactor = 0.0f;
            m_offGunFactor = 1.0f;
            
            // run fast animation
            setUpBodyState(CPlayerComponent::PlayerUpBodyRunFast);
        }
    }
}

void CPlayerComponent::updateUpperBodyRunFast()
{
    if ( m_upbodySubState == SubStateInit )
    {        
        m_upbodySubState = SubStateActive;
    }
    else if ( m_upbodySubState == SubStateEnd )
    {		
        doNextState();		
    }
    else
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

void CPlayerComponent::updateUpperBodyOffgunToAim()
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
    else
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
            
            
            core::line3df	ray		= getCameraRay();
            core::vector3df	colPos	= getCollisionPoint(ray);
            
            core::vector2df ret = getAimAngle(colPos);
            
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
            
            // change state
            if ( m_offGunFactor == 0.0f )
                setUpBodyState( CPlayerComponent::PlayerUpBodyAim );            
        }
        
    }
}

//void CPlayerComponent::updateUpperBodyTEMPLATE()
//{
//    if ( m_upbodySubState == SubStateInit )
//    {        
//        m_upbodySubState = SubStateActive;		
//    }
//    else if ( m_upbodySubState == SubStateEnd )
//    {		
//        doNextState();		
//    }
//    else
//    {	
//    }    
//}

///////////////////////////////////////////////////////////////////////
// Player component end update state function
///////////////////////////////////////////////////////////////////////

// _onUpdateFrameDataChannel
// call when finish get frame on a channel
void CPlayerComponent::_onUpdateFrameDataChannel( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int channel, int animLayer)
{

}


// _onUpdateFrameData
// call when finish blending many channel
void CPlayerComponent::_onUpdateFrameData( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int animLayer )
{    
	if ( animLayer != 0 && m_state == CPlayerComponent::PlayerIdle )
	{
		 // todo modify rotation, position of anim
		const core::vector3df rotAxis = core::vector3df(0,0,1);

		const float maxSpine = 70.0f;
		float spineAngle = core::clamp<float>(m_spineRotation, -maxSpine, maxSpine);
		float neckAngle = m_spineRotation - spineAngle;
		float r = spineAngle/3.0f;

        if ( r > 0 || neckAngle > 0 )
        {
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
	}  
}

// _onUpdateFinishAbsolute
// call when finish calc skin animation
void CPlayerComponent::_onUpdateFinishAbsolute( ISceneNode* node, core::matrix4& absoluteAnimationMatrix )
{
	if ( m_state != CPlayerComponent::PlayerIdle )
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
            absoluteAnimationMatrix *= q.getMatrix();
        }
        else if ( node == m_nodesChest[1] )
        {
            core::quaternion q;
            q.fromAngleAxis( core::degToRad(r), rotAxis );
            absoluteAnimationMatrix *= q.getMatrix();
        }
        else if ( node == m_nodesChest[2] )
        {
            core::quaternion q;
            q.fromAngleAxis( core::degToRad(r), rotAxis );
            absoluteAnimationMatrix *= q.getMatrix();
        }			
        else if ( node == m_nodeNeck )
        {
            core::quaternion q;
            core::vector3df neckAxis = core::vector3df(0.0f,-0.8f,1.0f);
            neckAxis.normalize();
            q.fromAngleAxis( core::degToRad( neckAngle ), neckAxis );
            absoluteAnimationMatrix *= q.getMatrix();
        }
        
	}
}


///////////////////////////////////////////////////////////////////////
// Player component end callback animation function
///////////////////////////////////////////////////////////////////////

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

core::vector2df CPlayerComponent::getAimAngle( const core::vector3df aimPoint )
{
	core::vector2df ret;

	CGameColladaSceneNode *spine = m_collada->getSceneNode("Neck");	//RightGunTip
	core::vector3df spinePoint = spine->getAbsolutePosition();

	core::vector3df aimRay	= aimPoint - spinePoint;
	core::vector3df base	= m_gameObject->getFront();

	//core::line3df line;
	//line.start = spinePoint;
	//line.end = aimPoint;
	//CGameDebug::getInstance()->addDrawLine(line, SColor(255,255,255,255));

	aimRay.normalize();

	core::plane3df plane;
	
	// projection to right plane
	core::vector3df outRight;
	plane.setPlane( spinePoint, m_gameObject->getRight() );
	plane.getIntersectionWithLine( aimPoint, m_gameObject->getRight(), outRight );

	// projection to top plane
	core::vector3df outTop;
	plane.setPlane( spinePoint, m_gameObject->getUp() );
	plane.getIntersectionWithLine( aimPoint, m_gameObject->getUp(), outTop );

	// calc result
	core::vector3df aimRayX	= outTop - spinePoint;
	aimRayX.normalize();
	ret.X = getAngle(aimRayX, base);

	core::vector3df aimRayY	= outRight - spinePoint;
	aimRayY.normalize();
	ret.Y = fabsf(getAngle(aimRayY, base));
	if ( outRight.Y < spinePoint.Y )
		ret.Y = -ret.Y;

	ret.X = core::clamp<float>(ret.X, -45.0f, 45.0f);
	ret.Y = core::clamp<float>(ret.Y, -45.0f, 45.0f);

	return ret;
}

// getCameraRay
// get camera view ray
core::line3df CPlayerComponent::getCameraRay()
{
    core::line3d<f32> ray;
    ICameraSceneNode *camera = getIView()->getSceneMgr()->getActiveCamera();
    ray.start = camera->getPosition();
    ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;
    return ray;
}

// getCollisionPoint
// check collision
core::vector3df CPlayerComponent::getCollisionPoint( core::line3df ray )
{
	core::vector3df ret;
	
	core::vector3df     outPoint;
	core::triangle3df   outTri;

	CGameLevel *level =	CGameLevel::getCurrentLevel();
	if ( level->checkTerrainCollide(ray, outPoint, outTri) == true )
	{
		ret = outPoint;
	}
	else
	{
		ret = ray.end;
	}
	 
	return ret;
}
