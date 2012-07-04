#include "stdafx.h"
#include "CTerrainComponent.h"

#include "CAnimMeshComponent.h"
#include "CStaticMeshComponent.h"
#include "CColladaMeshComponent.h"

#include "CZone.h"

#include "IView.h"

CTerrainComponent::CTerrainComponent( CGameObject *pObj )
	:IObjectComponent( pObj, (int)IObjectComponent::Terrain )
{
}

CTerrainComponent::~CTerrainComponent()
{
	// unRegister terrain
	CZone *pZone = (CZone*)	m_gameObject->getParent();
	if ( pZone )
		pZone->unRegisterTerrainObj( m_gameObject );
}

// init
// run when init object
void CTerrainComponent::initComponent()
{	
	IMesh *pMesh = NULL;
	ISceneNode		*node	= m_gameObject->getSceneNode();
	ISceneManager	*smgr	= getIView()->getSceneMgr();
	
	CZone *pZone = (CZone*)	m_gameObject->getParent();

	if ( node == NULL || pZone == NULL )
		return;

	m_listCollisionNode.clear();
	SMeshCollisionNode temp;

	if ( m_gameObject->getComponent( IObjectComponent::StaticMesh ) != NULL )
	{
		pMesh = ((IMeshSceneNode*) node)->getMesh();
		
		temp.mesh = pMesh;
		temp.type = 0;
		temp.node = node;

		m_listCollisionNode.push_back( temp );

	}
	else if ( m_gameObject->getComponent( IObjectComponent::AnimMesh ) != NULL )
	{
		pMesh = ((IAnimatedMeshSceneNode*) node)->getMesh();

		temp.mesh = pMesh;
		temp.type = 0;
		temp.node = node;

		m_listCollisionNode.push_back( temp );
	}
	else if ( m_gameObject->getComponent( IObjectComponent::ColladaMesh ) != NULL )
	{
		CColladaMeshComponent *comp = (CColladaMeshComponent*)m_gameObject->getComponent( IObjectComponent::ColladaMesh );
		CGameChildContainerSceneNode* colladaNode = comp->getColladaNode();
		
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

				if ( comp->isDefaultNode( sceneNode ) )
				{
					temp.type = 0;
					sceneNode->setTerrainNode( true );
					sceneNode->setHideTerrain( m_hideTerrain );
				}
				else
					temp.type = 1;
				
				m_listCollisionNode.push_back( temp );
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
	
	int nColNode = m_listCollisionNode.size();

	for ( int i = 0; i < nColNode; i++ )
	{
		if ( m_listCollisionNode[i].type == 0 )
		{			
			// add octree triangle
			ITriangleSelector* selector = smgr->createOctreeTriangleSelector( m_listCollisionNode[i].mesh, m_listCollisionNode[i].node );
			m_listCollisionNode[i].node->setTriangleSelector(selector);
			selector->drop();
		}
		else
		{
			// add 
			ITriangleSelector* selector = smgr->createTriangleSelectorFromBoundingBox( m_listCollisionNode[i].node );
			m_listCollisionNode[i].node->setTriangleSelector(selector);
			selector->drop();
		}		
	}

	// register terrain			
#ifdef GSEDITOR
	m_gameObject->getSceneNode()->setTriangleSelector(NULL);
#endif
	pZone->registerTerrainObj( m_gameObject );	
}

// updateComponent
// update object by frame
void CTerrainComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CTerrainComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );
	pObj->addBool("hideTerrain", m_hideTerrain, true);
}

// loadData
// load data to serializable
void CTerrainComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
	m_hideTerrain = pObj->readBool();
}

// getCollisionFromRay
// get collision from the ray
bool CTerrainComponent::getCollisionFromRay( core::line3df &ray, f32 &outBestDistanceSquared, core::vector3df &outBestCollisionPoint, core::triangle3df &outBestTriangle)
{
	int nColNode = m_listCollisionNode.size();

	for ( int i = 0; i < nColNode; i++ )
	{
		if ( checkCollisionFromNode( m_listCollisionNode[i].node, ray, outBestDistanceSquared, outBestCollisionPoint, outBestTriangle ) == true )
		{
			return true;
		}
	}

	return false;
}

// checkCollisionFromNode
// check collision from ray in node
bool CTerrainComponent::checkCollisionFromNode( ISceneNode* pNode, core::line3df &ray, f32 &outBestDistanceSquared, core::vector3df &outBestCollisionPoint, core::triangle3df &outBestTriangle)
{
	if ( pNode == NULL )
		return false;

	core::aabbox3df box = pNode->getTransformedBoundingBox();

	ITriangleSelector *selector = pNode->getTriangleSelector();
	if ( selector == NULL )
		return false;

	// if hit bouding box
	if ( box.intersectsWithLine( ray ) == true )
	{
		ISceneCollisionManager *colMgr = getIView()->getSceneMgr()->getSceneCollisionManager();
		
		core::vector3df		candidateCollisionPoint;
		core::triangle3df	candidateTriangle;

#ifdef GSGAMEPLAY
		ISceneNode			*hitNode = 0;
#else
		const ISceneNode	*hitNode = 0;
#endif

		// if hit on selector
		if ( colMgr->getCollisionPoint(ray, selector, candidateCollisionPoint, candidateTriangle, hitNode) == true )
		{			
			const f32 distanceSquared = (candidateCollisionPoint - ray.start).getLengthSQ();

			if(distanceSquared < outBestDistanceSquared)
			{
				outBestDistanceSquared = distanceSquared;
				
				outBestCollisionPoint = candidateCollisionPoint;
				outBestTriangle = candidateTriangle;

				const core::vector3df rayVector = ray.getVector().normalize();
				ray.end = ray.start + (rayVector * sqrtf(distanceSquared));
			}

			return true;
		}
	}
	
	return false;
}