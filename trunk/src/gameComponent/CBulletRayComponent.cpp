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
	std::vector<core::line3df>::iterator it = m_rays.begin(), end = m_rays.end();
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

// updateBulletCollision
void CBulletRayComponent::updateBulletCollision(core::line3df& ray)
{
	// spawn particle
	createSpark( ray.end, "data/particle/bullet.xml", 5000.0f );
}

// createSpark
CGameObject* CBulletRayComponent::createSpark( core::vector3df position, const char* xml, float lifeDelay )
{
	CGameObject *obj = m_gameObject->getParent()->getParent();
	CGameObject *spark = NULL;

	if ( obj && obj->getObjectType() == CGameObject::ZoneObject )
	{
		// create object
		spark = ((CZone*)obj)->createEmptyObject();
		
		// add component
		CParticleComponent	*particle = new CParticleComponent(spark);
		particle->initParticle(xml);
		spark->addComponenet(particle);

		CLifeTimeComponent *lifeTime = new CLifeTimeComponent(spark);
		lifeTime->setLifeTime(lifeDelay);
		spark->addComponenet(lifeTime);
	
		spark->setPosition(position);
	}

	return spark;
}