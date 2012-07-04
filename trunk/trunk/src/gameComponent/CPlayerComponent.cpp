#include "stdafx.h"
#include "CPlayerComponent.h"

CPlayerComponent::CPlayerComponent(CGameObject* obj)
	:IObjectComponent(obj, CGameComponent::PlayerComponent)
{
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