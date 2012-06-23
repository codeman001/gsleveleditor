#include "stdafx.h"
#include "IView.h"

#include "CObjectCollisionComponent.h"
#include "CAnimMeshComponent.h"
#include "CStaticMeshComponent.h"
#include "CColladaMeshComponent.h"

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

	vector<SObjectCollisionParam>	listMesh;
	SObjectCollisionParam temp;

	if ( m_gameObject->getComponent( IObjectComponent::StaticMesh ) != NULL )
	{
		pMesh = ((IMeshSceneNode*) node)->getMesh();
		
		temp.mesh = pMesh;
		temp.node = node;
		listMesh.push_back( temp );
	}
	else if ( m_gameObject->getComponent( IObjectComponent::AnimMesh ) != NULL )
	{
		pMesh = ((IAnimatedMeshSceneNode*) node)->getMesh();
		
		temp.mesh = pMesh;
		temp.node = node;
		listMesh.push_back( temp );
	}
	else if ( m_gameObject->getComponent( IObjectComponent::ColladaMesh ) != NULL )
	{
		CColladaMeshComponent *comp = (CColladaMeshComponent*)m_gameObject->getComponent( IObjectComponent::ColladaMesh );		
		ISceneNode* colladaNode = comp->getColladaNode();

		std::queue<ISceneNode*>	listSceneNode;
		const core::list<ISceneNode*>* listChild = &colladaNode->getChildren();
		core::list<ISceneNode*>::ConstIterator it = listChild->begin(), end = listChild->end();
		while ( it != end )
		{
			listSceneNode.push( (*it) );
			it++;
		}

		while ( listSceneNode.size() )
		{
			CGameColladaSceneNode* sceneNode = (CGameColladaSceneNode*)listSceneNode.front();
			listSceneNode.pop();

			if ( sceneNode->getMesh() )
			{
				temp.mesh = sceneNode->getMesh();				
				temp.node = sceneNode;

				listMesh.push_back( temp );
			}

			const core::list<ISceneNode*>* listChild = &sceneNode->getChildren();
			it = listChild->begin();
			end = listChild->end();
			while ( it != end )
			{
				listSceneNode.push( (*it) );
				it++;
			}
		}		
	}

	for ( int i = 0; i < (int)listMesh.size(); i++ )
	{
		node = listMesh[i].node;
		pMesh = listMesh[i].mesh;

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
		m_collisionType = CObjectCollisionComponent::BoudingBox;
	else
		m_collisionType = (CollisionType)type;

	// read collision data
	m_collisionData = pObj->readInt();
}