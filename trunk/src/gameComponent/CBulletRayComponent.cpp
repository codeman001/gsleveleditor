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


// packDataMultiplayer
// pack data multiplayer
void CBulletRayComponent::packDataMultiplayer(CDataPacket *packet)
{
	int nray = m_raysSync.size();
	packet->addByte((unsigned char)nray);
	for (int i = 0; i < nray; i++)
	{
		packet->addFloat(m_raysSync[i].start.X);
		packet->addFloat(m_raysSync[i].start.Y);
		packet->addFloat(m_raysSync[i].start.Z);

		packet->addFloat(m_raysSync[i].end.X);
		packet->addFloat(m_raysSync[i].end.Y);
		packet->addFloat(m_raysSync[i].end.Z);
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
		core::line3df ray;
		ray.start.X = packet->getFloat();
		ray.start.Y = packet->getFloat();
		ray.start.Z = packet->getFloat();

		ray.end.X = packet->getFloat();
		ray.end.Y = packet->getFloat();
		ray.end.Z = packet->getFloat();

		m_rays.push_back(ray);
	}
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