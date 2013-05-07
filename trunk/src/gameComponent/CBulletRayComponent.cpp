#include "stdafx.h"
#include "CBulletRayComponent.h"
#include "CParticleComponent.h"
#include "CLifeTimeComponent.h"

#include "CZone.h"
#include "gameLevel/CGameLevel.h"
#include "gameDebug/CGameDebug.h"

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
	m_gameObject->setSyncNetwork(true);

	std::vector<SBulletRay>::iterator it = m_rays.begin(), end = m_rays.end();
	while (it != end)
	{
		updateBulletCollision( (*it) );
		it++;
	}

	m_rays.clear();
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


// packDataMultiplayer
// pack data multiplayer
void CBulletRayComponent::packDataMultiplayer(CDataPacket *packet)
{
	int nray = m_raysSync.size();
	packet->addByte((unsigned char)nray);
	for (int i = 0; i < nray; i++)
	{
		
	}
	m_raysSync.clear();
}

// unPackDataMultiplayer
// unpack data on multiplayer
void CBulletRayComponent::unpackDataMultiplayer(CDataPacket *packet)
{
	int nray = (int)packet->getByte();
	for (int i = 0; i < nray; i++)
	{
		
	}
}


// updateBulletCollision
void CBulletRayComponent::updateBulletCollision(SBulletRay& bullet)
{
	core::vector3df     outPoint;
	core::triangle3df   outTri;
	
	CGameLevel *level =	CGameLevel::getCurrentLevel();

	ArrayGameObject objs;
	level->getListObjCollide(bullet.ray, objs, bullet.owner);

	if ( objs.size() > 0 )
	{
		ArrayGameObject::iterator iObj = objs.begin(), iEnd = objs.end();
		while ( iObj != iEnd )
		{
			CGameObject *obj = (CGameObject*)(*iObj);
			CColladaMeshComponent *collada = (CColladaMeshComponent*)obj->getComponent(IObjectComponent::ColladaMesh);
			
			if ( collada )
			{
				ISceneNode* root =  collada->getSceneNode("BoneRoot");
				
				std::queue<ISceneNode*> bones;
				core::list<ISceneNode*>::ConstIterator i = root->getChildren().begin(), 
					end = root->getChildren().end();
				
				while ( i != end )
				{
					bones.push((*i));
					i++;
				}
				
				std::vector<ISceneNode*> bonesHit;

				while ( bones.size() > 0 )
				{
					// get the first bone
					ISceneNode* b = bones.front();
					bones.pop();

					if ( b->getTransformedBoundingBox().intersectsWithLine(bullet.ray) )					
						bonesHit.push_back(b);

					// continue check the children
					i = b->getChildren().begin();
					end = b->getChildren().end();
					while ( i != end )
					{
						bones.push((*i));
						i++;
					}
				}

				if ( bonesHit.size() > 0 )
				{
					// implement from heal component

				}
			}
			iObj++;
		}
	}

	if ( level->checkTerrainCollide( bullet.ray, outPoint, outTri) == true )
	{
		// spawn particle
		createSpark( outPoint, "data/particle/bullet.xml", 5000.0f );
	}
}

// createSpark
CGameObject* CBulletRayComponent::createSpark( core::vector3df position, const char* xml, float lifeDelay )
{
	CGameObject *obj = m_gameObject->getParent();
	CGameObject *spark = NULL;

	if ( obj && obj->getObjectType() == CGameObject::ZoneObject )
	{
		// create object
		spark = ((CZone*)obj)->getParticleManager()->createEmptyObject();
		
		// add component
		CParticleComponent	*particle = new CParticleComponent(spark);
		particle->initParticle(xml);
		spark->addComponent(particle);

		CLifeTimeComponent *lifeTime = new CLifeTimeComponent(spark);
		lifeTime->setLifeTime(lifeDelay);
		spark->addComponent(lifeTime);
	
		spark->setPosition(position);
	}

	return spark;
}