#include "stdafx.h"
#include "IView.h"

#include "CObjectCollisionComponent.h"
#include "CAnimMeshComponent.h"
#include "CStaticMeshComponent.h"

CObjectCollisionComponent::CObjectCollisionComponent(CGameObject *owner)
	:IObjectComponent( owner, IObjectComponent::ObjectCollision )
{
	m_collisionType = CObjectCollisionComponent::BoudingBox;
	m_collisionData	= 0;
}
	
CObjectCollisionComponent::~CObjectCollisionComponent()
{
}

// init
// run when init object
void CObjectCollisionComponent::initComponent()
{
#ifndef GSEDITOR

	IMesh *pMesh = NULL;
	ISceneNode		*node	= m_gameObject->getSceneNode();
	ISceneManager	*smgr	= getIView()->getSceneMgr();

	if ( node == NULL )
		return;

	if ( m_gameObject->getComponent( IObjectComponent::StaticMesh ) != NULL )	
		pMesh = ((IMeshSceneNode*) node)->getMesh();
	else if ( m_gameObject->getComponent( IObjectComponent::AnimMesh ) != NULL )
		pMesh = ((IAnimatedMeshSceneNode*) node)->getMesh();

	if ( pMesh == NULL )
		return;
			
	switch ( m_collisionType )
	{
	case CObjectCollisionComponent::BoudingBox:
		{
			ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox(node);
			node->setTriangleSelector( selector );
			selector->drop();
		}
		break;
	case CObjectCollisionComponent::Triangle:
		{
			ITriangleSelector* selector = smgr->createTriangleSelector( pMesh, node );
			node->setTriangleSelector(selector);
			selector->drop();
		}
		break;
	case CObjectCollisionComponent::OctreeTriange:
		{
			ITriangleSelector* selector = smgr->createOctreeTriangleSelector( pMesh, node );
			node->setTriangleSelector(selector);
			selector->drop();
		}
		break;
	}	

#endif
}

// update
// run when update per frame
void CObjectCollisionComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CObjectCollisionComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	pObj->addInt("collisionType", (int)m_collisionType, true);
	pObj->addInt("collisionData", m_collisionData, true);
}

// loadData
// load data to serializable
void CObjectCollisionComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	// read collision type
	int type = pObj->readInt();	
	if ( type > CObjectCollisionComponent::OctreeTriange || type < 0 )
		m_collisionType = CObjectCollisionComponent::OctreeTriange;
	else
		m_collisionType = (CollisionType)type;

	// read collision data
	m_collisionData = pObj->readInt();
}