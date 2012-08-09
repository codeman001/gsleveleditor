#include "stdafx.h"
#include "IView.h"
#include "IRandomizer.h"

#include "CPlayerComponent.h"

#include "CColladaMeshComponent.h"
#include "CInventoryComponent.h"
#include "CWeaponComponent.h"

#include "gameLevel/CGameLevel.h"

#define LOG_ANIM_TIME

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

	m_keyActionBit	= CPlayerComponent::KeyNone;

	// init run const
	m_runSpeed				= 3.0f;
	m_runBackSpeed			= 2.0f;
	m_runNoGunSpeed			= 5.0f;

	m_bipSpineNode	= NULL;
	m_bipSpine1Node	= NULL;
	m_gunDummyNode	= NULL;

	m_runRotation	= 0.0f;
	m_runUp			= false;
	m_runBack		= false;
	m_runLeft		= false;
	m_runRight		= false;
	m_runNoGun		= true;
	m_runCommand	= false;

	m_currentRunRot	= 0.0f;

	m_collada	= NULL;
	m_inventory = NULL;

	m_animShotCurrentTime	= 0.0f;
	m_animCurrentTime		= 0.0f;

	m_noGun = true;	
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
	m_gameObject->setLighting( false );

	m_collada = (CColladaMeshComponent*) m_gameObject->getComponent( IObjectComponent::ColladaMesh );
	m_collada->setAnimationPackage( m_animationPackage );

	// foot nodes
	m_nodesFoot.push_back( m_collada->getSceneNode("Reference") );
	m_nodesFoot.push_back( m_collada->getSceneNode("LeftGun") );
	m_nodesFoot.push_back( m_collada->getSceneNode("Hips") );		
	m_nodesFoot.push_back( m_collada->getSceneNode("LeftUpLeg") );
	m_nodesFoot.push_back( m_collada->getSceneNode("RightUpLeg") );
	m_collada->getChildsOfSceneNode("LeftUpLeg",	m_nodesFoot);
	m_collada->getChildsOfSceneNode("RightUpLeg",	m_nodesFoot);

	// chest nodes
	m_nodesChest.push_back( m_collada->getSceneNode("Spine") );
	m_nodesChest.push_back( m_collada->getSceneNode("Spine1") );
	m_nodesChest.push_back( m_collada->getSceneNode("Spine2") );

	// hand, head
	m_nodesHandsAndHead.push_back( m_collada->getSceneNode("RightGun") );		
	m_collada->getChildsOfSceneNode("Spine3",m_nodesHandsAndHead);

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
	// if key press
	if ( irrEvent.EventType == EET_KEY_INPUT_EVENT )
	{
		// key input
		EKEY_CODE key = irrEvent.KeyInput.Key;
		
		// run flag
		bool updateRunState = false;

		if ( irrEvent.KeyInput.PressedDown )
		{
			if (  key == irr::KEY_UP || key == irr::KEY_KEY_W )
				m_keyActionBit |= CPlayerComponent::KeyUp;
			else if (  key == irr::KEY_DOWN || key == irr::KEY_KEY_S )
				m_keyActionBit |= CPlayerComponent::KeyBack;
			else if (  key == irr::KEY_LEFT || key == irr::KEY_KEY_A )
				m_keyActionBit |= CPlayerComponent::KeyLeft;
			else if (  key == irr::KEY_RIGHT || key == irr::KEY_KEY_D )
				m_keyActionBit |= CPlayerComponent::KeyRight;
			else if (  key == irr::KEY_SPACE )
				m_keyActionBit |= CPlayerComponent::KeyFire;
		}		
		else
		{
			if (  key == irr::KEY_UP || key == irr::KEY_KEY_W )
				m_keyActionBit &= ~CPlayerComponent::KeyUp;
			else if (  key == irr::KEY_DOWN || key == irr::KEY_KEY_S )
				m_keyActionBit &= ~CPlayerComponent::KeyBack;
			else if (  key == irr::KEY_LEFT || key == irr::KEY_KEY_A )
				m_keyActionBit &= ~CPlayerComponent::KeyLeft;
			else if (  key == irr::KEY_RIGHT || key == irr::KEY_KEY_D )
				m_keyActionBit &= ~CPlayerComponent::KeyRight;
			else if (  key == irr::KEY_SPACE )
				m_keyActionBit &= ~CPlayerComponent::KeyFire;
		}

		// update move
		m_runUp			= false;
		m_runBack		= false;
		m_runLeft		= false;
		m_runRight		= false;

		// calc rotation
		if ( (m_keyActionBit & CPlayerComponent::KeyLeft) != 0 )
		{
			if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
				m_runBack = true;
			else if ( (m_keyActionBit & CPlayerComponent::KeyUp) != 0 )
				m_runUp = true;
			
			m_runLeft = true;
		}
		else if ( (m_keyActionBit & CPlayerComponent::KeyRight) != 0 )
		{
			if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
				m_runBack = true;
			else if ( (m_keyActionBit & CPlayerComponent::KeyUp) != 0 )
				m_runUp = true;
			m_runRight = true;
		}
		else if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
		{
			m_runBack = true;
		}
		else if ( (m_keyActionBit & CPlayerComponent::KeyUp) != 0 )
			m_runUp = true;

		m_runCommand = m_runUp || m_runBack || m_runLeft || m_runRight;
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
	case CPlayerComponent::PlayerRun:
		updateStateRun();		
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
		int r = device->getRandomizer()->rand() % m_animIdle.size();		
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
		m_currentRunRot = 0.0f;
	
		if ( m_runCommand )
		{			
			setState( CPlayerComponent::PlayerRun );
		}

		if ( m_animCurrentTime <= 0 )
			m_subState = SubStateInit;
	}
}


void CPlayerComponent::updateStateRun()
{
	static float f = 0.0f;	
	
	if ( m_subState == SubStateInit )
	{
		m_subState = SubStateActive;		
		f = 0.0f;		
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
		
		bool turnFinish  = turnToDir( v0, v1, 4.0f );

		// rotate object
		m_gameObject->lookAt( m_gameObject->getPosition() + v0 );
		
		if ( turnFinish )
			setState( CPlayerComponent::PlayerIdle );
	}
}

///////////////////////////////////////////////////////////////////////
// Player component end update state function
///////////////////////////////////////////////////////////////////////

// stepAnimationTime	
void CPlayerComponent::stepAnimationTime()
{
	float timeStep = getIView()->getTimeStep();
	m_animCurrentTime = m_animCurrentTime - timeStep;
	m_animCrossfadeTime = m_animCrossfadeTime - timeStep;
	
	if ( m_animCurrentTime < 0 )
		m_animCurrentTime = 0;

	if ( m_animCrossfadeTime < 0 )
		m_animCrossfadeTime = 0;
}

// updateWeaponPosition
// update weapon
void CPlayerComponent::updateWeaponPosition()
{	
	if ( m_inventory && m_collada && m_gunDummyNode )
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
	if ( m_inventory && m_collada && m_gunDummyNode )
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
	speed = getRatioWithAngle(dir, turnTo, speed);

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
	angle = acos(angle);
	
	// interpolate	
	core::quaternion q, q1,q2;
	
	q1.fromAngleAxis( 0,		normal );
	q2.fromAngleAxis( angle,	normal );
	q.slerp( q1, q2, f );

	// rotate result vector
	core::vector3df dir = turnFrom;
	q.getMatrix().rotateVect( dir );
	return dir;	
}

// getRatioWithAngle	
float CPlayerComponent::getRatioWithAngle( const core::vector3df& turnFrom, const core::vector3df& turnTo, float angle )
{
	float angleVec = turnTo.dotProduct( turnFrom );
	angleVec = core::radToDeg( acos(angleVec) );	
	return angle/angleVec;
}