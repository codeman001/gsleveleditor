#include "stdafx.h"
#include "IView.h"
#include "CLifeTimeComponent.h"

CLifeTimeComponent::CLifeTimeComponent(CGameObject* obj)
	:IObjectComponent(obj, CGameComponent::LifeTimeComponent)
{
	
}

CLifeTimeComponent::~CLifeTimeComponent()
{
}
    
// init
// run when init object
void CLifeTimeComponent::initComponent()
{
}
    
// update
// run when update per frame
void CLifeTimeComponent::updateComponent()
{
	m_lifeTime = m_lifeTime - getIView()->getTimeStep();
	
	// release object
	if ( m_lifeTime <= 0 )
		m_gameObject->remove();
}
    
// saveData
// save data to serializable
void CLifeTimeComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( CGameComponent::s_compType[ m_componentID ] );
}
    
// loadData
// load data to serializable
void CLifeTimeComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
}