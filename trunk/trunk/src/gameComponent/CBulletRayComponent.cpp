#include "stdafx.h"
#include "CBulletRayComponent.h"

CBulletRayComponent::CBulletRayComponent(CGameObject* obj)
	:IObjectComponent(obj, CGameComponent::BulletRayComponent)
{
}

CBulletRayComponent::~CBulletRayComponent()
{
}
    
// init
// run when init object
void CBulletRayComponent::initComponent()
{
}
    
// update
// run when update per frame
void CBulletRayComponent::updateComponent()
{
}
    
// saveData
// save data to serializable
void CBulletRayComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( CGameComponent::s_compType[ m_componentID ] );
}
    
// loadData
// load data to serializable
void CBulletRayComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
}