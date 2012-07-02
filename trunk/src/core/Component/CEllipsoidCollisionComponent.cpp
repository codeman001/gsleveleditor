#include "stdafx.h"
#include "CEllipsoidCollisionComponent.h"

CEllipsoidCollisionComponent::CEllipsoidCollisionComponent( CGameObject *pObj )
	:IObjectComponent( pObj, IObjectComponent::EllipsoidCollision )
{

}

CEllipsoidCollisionComponent::~CEllipsoidCollisionComponent()
{
}

// init
// run when init object
void CEllipsoidCollisionComponent::initComponent()
{
	//.. to do add animator here ..
}

// updateComponent
// update object by frame
void CEllipsoidCollisionComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CEllipsoidCollisionComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );
}

// loadData
// load data to serializable
void CEllipsoidCollisionComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
}