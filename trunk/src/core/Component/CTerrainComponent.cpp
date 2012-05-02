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

	if ( node == NULL )
		return;

	if ( m_gameObject->getComponent( IObjectComponent::StaticMesh ) != NULL )	
		pMesh = ((IMeshSceneNode*) node)->getMesh();
	else if ( m_gameObject->getComponent( IObjectComponent::AnimMesh ) != NULL )
		pMesh = ((IAnimatedMeshSceneNode*) node)->getMesh();
	else if ( m_gameObject->getComponent( IObjectComponent::ColladaMesh ) != NULL )
	{
		CColladaMeshComponent *comp = (CColladaMeshComponent*)m_gameObject->getComponent( IObjectComponent::ColladaMesh );
		CGameColladaSceneNode *node = comp->getDefaultNode();
		if ( node )
			pMesh = node->getMesh();
	}

	if ( pMesh == NULL )
		return;
			
	// add octree triangle
	ITriangleSelector* selector = smgr->createOctreeTriangleSelector( pMesh, node );
	node->setTriangleSelector(selector);
	selector->drop();
	
	// register terrain
	CZone *pZone = (CZone*)	m_gameObject->getParent();
	if ( pZone )
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
}

// loadData
// load data to serializable
void CTerrainComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
}

// getCollisionFromRay
// get collision from the ray
bool CTerrainComponent::getCollisionFromRay( core::line3df & ray, f32 &outBestDistanceSquared, core::vector3df &outBestCollisionPoint, core::triangle3df &outBestTriangle)
{
	ISceneNode *pNode = m_gameObject->getSceneNode();
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
		const ISceneNode *	hitNode = 0;

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