#include "stdafx.h"
#include "IView.h"
#include "CPlayerComponent.h"
#include "gameLevel/CGameLevel.h"


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
	m_runSpeed				= 6.0f;
	m_runBackSpeed			= 5.0f;

	m_bipSpineNode	= NULL;
	m_bipSpine1Node	= NULL;

	m_runRotation	= 0.0f;
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
	if ( m_collada )
	{
		m_collada->setAnimationPackage( m_animationPackage );
	
		m_bipSpineNode	= m_collada->getSceneNode("Bip01_Spine-node");
		m_bipSpine1Node = m_collada->getSceneNode("Bip01_Spine1-node");
	}
	// set basic state idle
	setState( CPlayerComponent::PlayerIdle );

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
		m_animationPackage = animFactory->loadAnimation("playerAnim", getIView()->getPath("data/mesh/character/marine/marine_2_anims.anim"));	
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
		}

		// update move
		m_runRotation = 0.0f;		
		bool run = false;

		// calc rotation
		if ( (m_keyActionBit & CPlayerComponent::KeyLeft) != 0 )
		{
			m_runRotation -= 90.0f;
			if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
				m_runRotation += 135.0f;
			else if ( (m_keyActionBit & CPlayerComponent::KeyUp) != 0 )
				m_runRotation += 45.0f;
			run = true;
		}
		else if ( (m_keyActionBit & CPlayerComponent::KeyRight) != 0 )
		{
			m_runRotation += 90.0f;
			if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
				m_runRotation -= 135.0f;
			else if ( (m_keyActionBit & CPlayerComponent::KeyUp) != 0 )
				m_runRotation -= 45.0f;
			run = true;
		}
		else if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
		{
			run = true;
		}
		else if ( (m_keyActionBit & CPlayerComponent::KeyUp) != 0 )
		{
			run = true;
		}

		if ( m_keyActionBit == CPlayerComponent::KeyNone )
			setState( CPlayerComponent::PlayerIdle );
		else
		{	
			if ( run )
			{
				setState( CPlayerComponent::PlayerRun );
			}
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
	case CPlayerComponent::PlayerRun:
		updateStateRun();
		break;
	default:
		break;
	}
}

void CPlayerComponent::updateStateIdle()
{
	if ( m_subState == SubStateInit )
	{
		m_collada->setAnimation("mp_idle");
		m_subState = SubStateActive;
	}
	else if ( m_subState == SubStateEnd )
	{				
		doNextState();
	}
	else
	{
		m_currentRunRot = 0.0f;
	}
}

void CPlayerComponent::updateStateRun()
{
	static int s_lastActionKey = 0;

	if ( m_subState == SubStateInit )
	{
		if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )		
			m_collada->setAnimation("mp_run_back");
		else
			m_collada->setAnimation("mp_run");

		s_lastActionKey = m_keyActionBit;
		m_subState = SubStateActive;

		// disable anim on bip spine
		m_bipSpineNode->setEnableAnim( true );
		m_bipSpine1Node->setEnableAnim( true );
	}
	else if ( m_subState == SubStateEnd )
	{	
		// enable anim on bip spine
		m_bipSpineNode->setEnableAnim( true );
		m_bipSpine1Node->setEnableAnim( true );

		// change state
		doNextState();
	}
	else
	{		
		// update run
		float diff = getIView()->getTimeStep() * 0.1f;

		// disable anim
		m_bipSpineNode->setEnableAnim( false );
		m_bipSpine1Node->setEnableAnim( false );

		// and update animation by manual
		m_bipSpineNode->updateAnimation();
		m_bipSpine1Node->updateAnimation();

		// -------------------------
		// begin rotate the bip
		
		// linear calc current rotation
		float rotSpeed = 3.0f  * diff;
		if ( m_currentRunRot < m_runRotation )
			m_currentRunRot = m_currentRunRot + rotSpeed;			
		else if ( m_currentRunRot > m_runRotation )
			m_currentRunRot = m_currentRunRot - rotSpeed;
		
		if ( fabs( m_currentRunRot - m_runRotation ) < rotSpeed )
			m_currentRunRot = m_runRotation;

		// rotate the legs to run vector
		core::vector3df rotateSpine = m_bipSpineNode->AnimationMatrix.getRotationDegrees();
		rotateSpine.X += m_currentRunRot;
		m_bipSpineNode->AnimationMatrix.setRotationDegrees( rotateSpine );

		// rotate the body to front vector
		rotateSpine = m_bipSpine1Node->AnimationMatrix.getRotationDegrees();
		rotateSpine.X -= m_currentRunRot;
		m_bipSpine1Node->AnimationMatrix.setRotationDegrees( rotateSpine );
		// end rotate the bip
		// -------------------------		
		
		// get camera front vector
		CGameCamera* cam = CGameLevel::getCurrentLevel()->getCamera();
		core::vector3df front = cam->getPosition() - cam->getTarget();
		front.Y = 0;
		front.normalize();
				
		core::vector3df objFront = m_gameObject->getFront();
		objFront.normalize();

		// current object position
		core::vector3df pos = m_gameObject->getPosition();			
		core::vector3df	moveFront = -front;

		// set rotation by camera
		float angle1 = (float)(core::vector2df( moveFront.X, moveFront.Z ).getAngleTrig());
		float angle2 = (float)(core::vector2df(  objFront.X,  objFront.Z ).getAngleTrig());
		
		// adjust to fix rotate too large
		if ( fabs(angle2 - angle1) > 180 )
		{
			if ( angle2 > angle1 )
				angle2 = angle2 - 360.0f;
			else
				angle1 = angle1 - 360.0f;
		}

		// linear calc current rotation
		rotSpeed = 5.0f  * diff;
		if ( angle2 < angle1 )
			angle2 = angle2 + rotSpeed;
		else if ( angle2 > angle1 )
			angle2 = angle2 - rotSpeed;		
		if ( fabs( angle2 - angle1 ) < rotSpeed )
			angle2 = angle1;
		
		// rotate object & front vector
		m_gameObject->setRotation(core::vector3df(0.0f, 90 - angle2, 0.0f));

		// get front
		front = m_gameObject->getFront();

		// rotation run vector
		core::matrix4 mat;
		mat.setRotationDegrees( core::vector3df(0, m_currentRunRot,0) );
		mat.rotateVect( front );

		if ( (m_keyActionBit & CPlayerComponent::KeyBack) != 0 )
		{
			// run back
			m_gameObject->setPosition( pos + front * m_runSpeed * diff );
		}
		else
		{
			// run forward
			m_gameObject->setPosition( pos - front * m_runBackSpeed * diff );
		}				

		// check if change action
		if ( s_lastActionKey != m_keyActionBit )
			m_subState = SubStateInit;

		s_lastActionKey = m_keyActionBit;
	}
}