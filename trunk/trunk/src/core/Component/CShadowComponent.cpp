#include "stdafx.h"
#include "CShadowComponent.h"

CShadowComponent::CShadowComponent(CGameObject *owner)
	:IObjectComponent( owner, IObjectComponent::Shadow )
{
	m_shadowCasting		= true;
	m_shadowReceiving	= false;
}
	
CShadowComponent::~CShadowComponent()
{
}

// init
// run when init object
void CShadowComponent::initComponent()
{
}

// update
// run when update per frame
void CShadowComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CShadowComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	pObj->addBool("shadowCasting",		m_shadowCasting, true);
	pObj->addBool("shadowReceiving",	m_shadowReceiving, true);
}

// loadData
// load data to serializable
void CShadowComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	m_shadowCasting		= pObj->readBool();
	m_shadowReceiving	= pObj->readBool();
}