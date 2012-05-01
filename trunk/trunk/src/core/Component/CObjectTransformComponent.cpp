#include "stdafx.h"
#include "CObjectTransformComponent.h"

CObjectTransformComponent::CObjectTransformComponent( CGameObject *p )
	:IObjectComponent( p, IObjectComponent::ObjectTransform ),
	m_scale(1.0f, 1.0f, 1.0f)
{	 
}

CObjectTransformComponent::~CObjectTransformComponent()
{

}

// init
// run when init object
void CObjectTransformComponent::initComponent()
{
	m_gameObject->setRotation	( m_rotate );
	m_gameObject->setScale		( m_scale );
}

// update
// run when update per frame
void CObjectTransformComponent::updateComponent()
{
	
}

// saveData
// save data to serializable
void CObjectTransformComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	pObj->addFloat("rotateX",m_rotate.X, true);
	pObj->addFloat("rotateY",m_rotate.Y, true);
	pObj->addFloat("rotateZ",m_rotate.Z, true);
	
	pObj->addFloat("scaleX",m_scale.X, true);
	pObj->addFloat("scaleY",m_scale.Y, true);
	pObj->addFloat("scaleZ",m_scale.Z, true);
}

// loadData
// load data to serializable
void CObjectTransformComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	m_rotate.X = pObj->readFloat();
	m_rotate.Y = pObj->readFloat();
	m_rotate.Z = pObj->readFloat();

	m_scale.X = pObj->readFloat();
	m_scale.Y = pObj->readFloat();
	m_scale.Z = pObj->readFloat();
}