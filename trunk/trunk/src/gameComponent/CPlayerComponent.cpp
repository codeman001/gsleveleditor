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
	m_state = CPlayerComponent::PlayerNone;
	m_nextState = CPlayerComponent::PlayerNone;
	m_runState = CPlayerComponent::Run;


	// init run const
	m_runSpeed			= 6.0f;
	m_runBackSpeed		= 4.0f;
	m_runLeftSpeed		= 2.5f;
	m_runRightSpeed		= 2.5f;
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
		m_collada->setAnimationPackage( m_animationPackage );
	
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
			{
				m_runState = CPlayerComponent::Run;
				updateRunState = true;
			}
			else if (  key == irr::KEY_DOWN || key == irr::KEY_KEY_S )
			{
				m_runState = CPlayerComponent::RunBack;
				updateRunState = true;
			}
			else if (  key == irr::KEY_LEFT || key == irr::KEY_KEY_A )
			{
				m_runState = CPlayerComponent::RunLeft;
				updateRunState = true;
			}
			else if (  key == irr::KEY_RIGHT || key == irr::KEY_KEY_D )
			{
				m_runState = CPlayerComponent::RunRight;
				updateRunState = true;
			}


			// change state run
			if ( updateRunState == true )
			{
				// change to run state
				setState( CPlayerComponent::PlayerRun );				 
			}

		}		
		else
		{
			// change state idle
			setState( CPlayerComponent::PlayerIdle );
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
	}
}

void CPlayerComponent::updateStateRun()
{
	if ( m_subState == SubStateInit )
	{
		if ( m_runState == CPlayerComponent::RunBack )					
			m_collada->setAnimation("mp_run_back");
		else if ( m_runState == CPlayerComponent::RunLeft )
			m_collada->setAnimation("mp_strafe_left");
		else if ( m_runState == CPlayerComponent::RunRight )
			m_collada->setAnimation("mp_strafe_right");
		else
			m_collada->setAnimation("mp_run");

		m_subState = SubStateActive;
		m_lastRunState	= m_runState;
	}
	else if ( m_subState == SubStateEnd )
	{			
		m_runState	= CPlayerComponent::Run;		
		doNextState();
	}
	else
	{		
		if ( m_lastRunState != m_runState )
		{
			// we need init run animation
			// because we change run direction
			m_subState = SubStateInit;
		}
		else
		{
			// update run
			float diff = getIView()->getTimeStep() * 0.1f;
			
			// get front vector
			CGameCamera* cam = CGameLevel::getCurrentLevel()->getCamera();
			core::vector3df front = cam->getPosition() - cam->getTarget();
			front.Y = 0;
			front.normalize();


			// current object position
			core::vector3df pos = m_gameObject->getPosition();			

			if ( m_runState == CPlayerComponent::RunLeft )
			{
				m_gameObject->setPosition( pos + m_gameObject->getRight() * m_runLeftSpeed * diff );
			}
			else if ( m_runState == CPlayerComponent::RunRight )
			{
				m_gameObject->setPosition( pos - m_gameObject->getRight() * m_runRightSpeed * diff);
			}
			else if ( m_runState == CPlayerComponent::RunBack )
			{
				m_gameObject->setPosition( pos - front * m_runBackSpeed * diff );
				//m_gameObject->lookAt( pos + front );
			}
			else
			{
				m_gameObject->setPosition( pos + front * m_runSpeed * diff );
				m_gameObject->lookAt( pos + front );
			}

		}

		m_lastRunState = m_runState;
	}
}