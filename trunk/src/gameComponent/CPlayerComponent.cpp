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
	m_state = CPlayerComponent::PlayerIdle;
}

CPlayerComponent::~CPlayerComponent()
{
}

// init
// run when init object
void CPlayerComponent::initComponent()
{
}

// update
// run when update per frame
void CPlayerComponent::updateComponent()
{
	// update gameplay
	CColladaMeshComponent* collada = (CColladaMeshComponent*) m_gameObject->getComponent( IObjectComponent::ColladaMesh );
	if ( collada == NULL )
		return;

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
}





///////////////////////////////////////////////////////////////////////
// Player component implement
///////////////////////////////////////////////////////////////////////

