#include "stdafx.h"
#include "CEllipsoidCollisionComponent.h"
#include "IView.h"

#ifdef GSGAMEPLAY
#include "gameLevel/CGameLevel.h"
#include "CTerrainComponent.h"
#endif

CEllipsoidCollisionComponent::CEllipsoidCollisionComponent( CGameObject *pObj )
	:IObjectComponent( pObj, IObjectComponent::EllipsoidCollision )
{
	m_isFirstUpdate		= true;
	m_isFalling			= true;
	
	m_gravity			= core::vector3df(0,-100,0);
	
	m_ellipsoidRadius	= core::vector3df(50,90,80);	// right, up, front
	m_translation		= core::vector3df(0,-90,0);
}

CEllipsoidCollisionComponent::~CEllipsoidCollisionComponent()
{
}

// init
// run when init object
void CEllipsoidCollisionComponent::initComponent()
{

}

// updateComponent
// update object by frame
void CEllipsoidCollisionComponent::updateComponent()
{
#ifdef GSGAMEPLAY
	CGameLevel *level =	CGameLevel::getCurrentLevel();
	if ( level == NULL )
	{
		m_isFirstUpdate = true;
		return;
	}

	ISceneNode *node = m_gameObject->getSceneNode();
	if ( node == NULL )
	{
		m_isFirstUpdate = true;
		return;
	}

	if ( m_isFirstUpdate == true )
	{
		m_lastPosition = m_gameObject->getPosition();
		m_isFirstUpdate = false;
		return;
	}	

	// get position need goto
	m_testPosition = m_gameObject->getPosition();
		
	// calc gravity	
	float diff = getIView()->getTimeStep();
	
	m_fallingVelocity += m_gravity * (f32)diff * 0.001f;

	// my bouding box
	core::aabbox3df myBox = node->getTransformedBoundingBox();

	int nZone = level->getZoneCount();

	// loop for all zone
	for ( int iZone = 0; iZone < nZone; iZone++ )
	{
		CZone *z = level->getZone( iZone );
		core::aabbox3df zoneBox = z->getSceneNode()->getTransformedBoundingBox();		
		
		if ( zoneBox.intersectsWithBox( myBox ) == true )
		{
			// loop all terrain object in zone
			ArrayGameObject& terrains = z->getTerrainList();
			ArrayGameObject::iterator itTerrain = terrains.begin(), itTerrainEnd = terrains.end();
			while ( itTerrain != itTerrainEnd )
			{
				CGameObject *objTerrain = ((CGameObject*) *itTerrain);

				// loop all scene node in a terrain object
				CTerrainComponent* com  = (CTerrainComponent*)objTerrain->getComponent( IObjectComponent::Terrain );
				if ( com )
				{
					int numNode = com->getNumTerrainNode();
					for ( int i = 0; i < numNode; i++ )
					{
						SMeshCollisionNode& node = com->getMeshCollisionNode(i);
						ITriangleSelector *tri = node.node->getTriangleSelector();
						if ( tri )
						{
							collideTest( tri );
						}
					}
				}
				
				itTerrain++;
			}
		}

	}

	// set new position
	m_gameObject->setPosition( m_testPosition );

	// save last position
	m_lastPosition = m_testPosition;
#endif
}

// collideTest
// test collide on world
void CEllipsoidCollisionComponent::collideTest( ITriangleSelector *world )
{
#ifdef GSGAMEPLAY
	core::vector3df vel = m_testPosition - m_lastPosition;

	core::triangle3df	collisionTriangle;
	core::vector3df		collisionPoint;
	core::vector3df		collisionResultPosition;
	ISceneNode			*collisionNode = 0;	
	
	bool f = false;
	f32 slidingSpeed = 0.0005f;

	// detect collision
	collisionResultPosition  = getIView()->getSceneMgr()->getSceneCollisionManager()->getCollisionResultPosition
		(
			world, m_lastPosition - m_translation,
			m_ellipsoidRadius, 
			vel, 
			collisionTriangle,	// out triangle
			collisionPoint,		// out point
			f,
			collisionNode,		// out node
			slidingSpeed,
			m_fallingVelocity
		);
		
	collisionResultPosition += m_translation;

	if (f == false)
	{
		// has ground position
		m_isFalling = false;
		m_fallingVelocity.set(0, 0, 0);
	}
	else
	{
		m_isFalling = true;
	}

	m_testPosition = collisionResultPosition;
#endif
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