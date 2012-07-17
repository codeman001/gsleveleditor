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
	m_keyActionBit	= CPlayerComponent::KeyNone;

	// init run const
	m_runSpeed				= 3.0f;
	m_runBackSpeed			= 2.0f;

	m_bipSpineNode	= NULL;
	m_bipSpine1Node	= NULL;
	m_gunDummyNode	= NULL;

	m_runRotation	= 0.0f;
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
	if ( m_collada )
	{
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
	}

	// idle anim list
	m_animIdle.push_back( "Hero@Idle" );
	m_animIdle.push_back( "Hero@Idle1" );	
	m_animIdleRun			= "Hero@IdleRun";
	m_animIdleRunForward	= "Hero@RunForward";
	m_animIdleRunBackward	= "Hero@RunBackward";
	m_animRunIdle			= "Hero@RunIdle";

	// set basic state idle
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
		m_runRotation = 0.0f;		
		m_runCommand = false;

		// calc rotation
		if ( (m_keyActionBit & CPlayerComponent::KeyLeft) != 0 )
		{
			m_runRotation -= 90.0f;
			if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
				m_runRotation += 135.0f;
			else if ( (m_keyActionBit & CPlayerComponent::KeyUp) != 0 )
				m_runRotation += 45.0f;
			m_runCommand = true;
		}
		else if ( (m_keyActionBit & CPlayerComponent::KeyRight) != 0 )
		{
			m_runRotation += 90.0f;
			if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
				m_runRotation -= 135.0f;
			else if ( (m_keyActionBit & CPlayerComponent::KeyUp) != 0 )
				m_runRotation -= 45.0f;
			m_runCommand = true;
		}
		else if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
		{
			m_runCommand = true;
		}
		else if ( (m_keyActionBit & CPlayerComponent::KeyUp) != 0 )
		{
			m_runCommand = true;
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
	case CPlayerComponent::PlayerIdleRun:
		updateStateIdleRun();
		break;
	case CPlayerComponent::PlayerRun:
		updateStateRun();
		break;
	case CPlayerComponent::PlayerRunIdle:
		updateStateRunIdle();
		break;
	default:
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

		m_collada->setAnimation( anim );
		m_animCurrentTime = m_collada->getCurrentAnim()->getRealTimeLength();

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
			setState( CPlayerComponent::PlayerIdleRun );

		if ( m_animCurrentTime <= 0 )
			m_subState = SubStateInit;
	}
}

void CPlayerComponent::updateStateIdleRun()
{
	stepAnimationTime();

	if ( m_subState == SubStateInit )
	{		
		m_collada->setAnimation( m_animIdleRun, false );		
		m_animCurrentTime = m_collada->getCurrentAnim()->getRealTimeLength();

		m_subState = SubStateActive;
	}
	else if ( m_subState == SubStateEnd )
	{		
		setState( CPlayerComponent::PlayerRun );
		doNextState();	
	}
	else
	{
		m_currentRunRot = 0.0f;

		// update run
		float diff = getIView()->getTimeStep() * 0.1f;

		// get front
		core::vector3df pos		= m_gameObject->getPosition();
		core::vector3df front	= m_gameObject->getFront();

		float animLength = m_collada->getCurrentAnim()->getRealTimeLength();
		float ratio = 1.0f - m_animCurrentTime/animLength;

		if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
		{
			// run back
			m_gameObject->setPosition( pos - front * m_runBackSpeed * ratio * diff );
		}
		else
		{
			// run forward
			m_gameObject->setPosition( pos + front * m_runSpeed * ratio * diff );
		}

		if ( m_animCurrentTime <= 0 )
			m_subState = SubStateEnd;
	}
}

void CPlayerComponent::updateStateRun()
{
	static int s_lastActionKey = 0;

	if ( m_subState == SubStateInit )
	{
		if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
			m_collada->setAnimation( m_animIdleRunBackward );
		else
			m_collada->setAnimation( m_animIdleRunForward );

		s_lastActionKey = m_keyActionBit;
		m_subState = SubStateActive;	
	}
	else if ( m_subState == SubStateEnd )
	{
		doNextState();
	}
	else
	{		
		// update run
		float diff = getIView()->getTimeStep() * 0.1f;
				
		// rotate object
		updateRotateObject();

		// update shot weapon
		// updateActionShotWeapon();

		// get front
		core::vector3df pos		= m_gameObject->getPosition();
		core::vector3df front	= m_gameObject->getFront();

		if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
		{
			// run back
			m_gameObject->setPosition( pos - front * m_runBackSpeed * diff );			
		}
		else
		{
			// run forward
			m_gameObject->setPosition( pos + front * m_runSpeed * diff );
		}

		if ( m_runCommand == false )
			setState( CPlayerComponent::PlayerRunIdle );

		// check to init run state if change action
		if ( s_lastActionKey != m_keyActionBit )
			m_subState = SubStateInit;

		s_lastActionKey = m_keyActionBit;
	}
}

void CPlayerComponent::updateStateRunIdle()
{
	stepAnimationTime();

	if ( m_subState == SubStateInit )
	{		
		m_collada->setAnimation( m_animRunIdle, false );		
		m_animCurrentTime = m_collada->getCurrentAnim()->getRealTimeLength();

		m_subState = SubStateActive;
	}
	else if ( m_subState == SubStateEnd )
	{		
		setState( CPlayerComponent::PlayerIdle );
		doNextState();	
	}
	else
	{
		m_currentRunRot = 0.0f;

		// update run
		float diff = getIView()->getTimeStep() * 0.1f;

		// get front
		core::vector3df pos		= m_gameObject->getPosition();
		core::vector3df front	= m_gameObject->getFront();

		float animLength = m_collada->getCurrentAnim()->getRealTimeLength();
		float ratio = m_animCurrentTime/animLength;

		if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
		{
			// run back
			m_gameObject->setPosition( pos - front * m_runBackSpeed * ratio * diff );
		}
		else
		{
			// run forward
			m_gameObject->setPosition( pos + front * m_runSpeed * ratio * diff );
		}

		if ( m_animCurrentTime <= 0 )
			m_subState = SubStateEnd;
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
	if ( m_animCurrentTime < 0 )
		m_animCurrentTime = 0;
}

// updateActiveFire
// update shot weapon action
void CPlayerComponent::updateActionShotWeapon()
{	
/*
	CWeaponComponent *weapon = getCurrentWeapon();
	if ( weapon )
	{
		if ( weapon->getWeaponType() == CWeaponComponent::ShotGun )
		{
			// update shoot gun
			if (	(m_keyActionBit & CPlayerComponent::KeyFire ) != 0 &&
					( 
						m_animShotCurrentTime == m_animShotTotalTime || 
						m_animShotTotalTime == 0 
					)
				)
			{
				// begin state
				m_collada->setAnimation("idle_attack_shotgun_fire", m_handAndHeadNodes);
				m_collada->getCurrentAnim()->loop = false;
				m_animShotTotalTime		= m_collada->getCurrentAnim()->duration * ( 1000.0f/m_gunDummyNode->getFPS());
				m_animShotCurrentTime	= 0.0f;
			}
			else
			{				
   				if ( m_animShotCurrentTime >= m_animShotTotalTime )
				{
					m_animShotCurrentTime = m_animShotTotalTime;

					if ( (m_keyActionBit & CPlayerComponent::KeyFire) == 0 )
					{
						m_collada->setAnimation("idle_attack_shotgun", m_handAndHeadNodes);
					}
				}
				else
				{
					m_animShotCurrentTime += getIView()->getTimeStep();
				}
			}
		}
	}
*/
}

// updateRotateObject
// rotate the object to camera front
void CPlayerComponent::updateRotateObject()
{
	// get camera front vector
	CGameCamera* cam = CGameLevel::getCurrentLevel()->getCamera();
	core::vector3df front = cam->getPosition() - cam->getTarget();
	front.Y = 0;
	front.normalize();

	core::vector3df objFront = m_gameObject->getFront();

	// current object position	
	core::vector3df	moveFront = -front;

	// set rotation by camera
	float angle1 = (float)(core::vector2df( moveFront.X, moveFront.Z ).getAngleTrig() - m_runRotation);
	float angle2 = (float)(core::vector2df(  objFront.X,  objFront.Z ).getAngleTrig());
	
	// find shortest arc
	if ( fabs(angle2 - angle1) > 180 )
	{
		if ( angle2 > angle1 )
			angle2 = angle2 - 360.0f;
		else
			angle1 = angle1 - 360.0f;
	}

	// linear calc current rotation
	float diff = getIView()->getTimeStep() * 0.1f;
	float rotSpeed = 2.0f  * diff;

	if ( angle2 < angle1 )
		angle2 = angle2 + rotSpeed;
	else if ( angle2 > angle1 )
		angle2 = angle2 - rotSpeed;		
	if ( fabs( angle2 - angle1 ) < rotSpeed )
		angle2 = angle1;
	
	// rotate object & front vector
	m_gameObject->setRotation(core::vector3df(0.0f, 90 - angle2, 0.0f));
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
