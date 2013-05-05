#include "stdafx.h"
#include "IView.h"
#include "CGunComponent.h"

#include "CZone.h"
#include "CBulletRayComponent.h"

#include "gameLevel/CGameLevel.h"


CGunComponent::CGunComponent(CGameObject* obj, CGameConfig::SWeaponInfo* info)
	:IObjectComponent(obj, CGameComponent::GunComponent)
{
	m_info = info;
	
	m_state = CGunComponent::Normal;
	m_nextState = CGunComponent::None;

	m_collada = NULL;
	m_muzzle = NULL;

	m_currentBullets = 0;
	m_numBullets = 0;
	m_bulletRate = 0;

	m_damage = 5.0f;
	m_damageThrowObj = false;
	m_autoAim = false;

	m_reloadTime = 5.0f;
	m_vibrateAngle = 0.0f;

	// parse info
	std::vector<CGameConfig::SParam>::iterator i = m_info->params.begin(), end = m_info->params.end();
	while ( i != end )
	{
		const char *value = i->value.c_str();

		// get param
		if ( i->name == "rateShoot" )
			m_bulletRate = atoi( value );
		else if ( i->name == "numBullet" )
			m_numBullets = atoi( value );
		else if ( i->name == "damage" )
			m_damage = (float)atof(value);
		else if ( i->name == "damageThrowObj" )
			m_damageThrowObj = (atoi(value) == 1);
		else if ( i->name == "autoAim" )
			m_autoAim = (atoi(value) == 1);
		else if ( i->name == "reloadTime" )
			m_reloadTime = (float)atof(value);
		else if ( i->name == "vibrateAngle" )	
			m_vibrateAngle = (float)atof(value);

		// next param
		i++;
	}

	m_currentBullets = m_numBullets;
}

CGunComponent::~CGunComponent()
{
}

// init
// run when init object
void CGunComponent::initComponent()
{
	m_collada	= (CColladaMeshComponent*)m_gameObject->getComponent(IObjectComponent::ColladaMesh);
	if ( m_collada == NULL )
		return;

	m_muzzle	= m_collada->getSceneNode(m_info->muzzle.c_str());	
}

// update
// run when update per frame
void CGunComponent::updateComponent()
{	
	switch ( m_state )
	{
	case Normal:
		{
			m_lastShootUpdate = 0.0f;
			m_muzzleTime = 0.0f;
		}
		break;
	case Shoot:
		updateShoot();
		break;
	case Reload:
		{
  			m_currentBullets = m_numBullets;
			m_lastShootUpdate = 0.0f;
			m_muzzleTime = 0.0f;
		}
		break;
	}
	
	// update to show/hide gun muzzle
	updateMuzzle();

	// change state
	if ( m_nextState != None )
	{
		m_state = m_nextState;
		m_nextState = None;
	}
}

// saveData
// save data to serializable
void CGunComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup(CGameComponent::s_compType[m_componentID]);
}


// updateShoot
// shoot
void CGunComponent::updateShoot()
{
	m_lastShootUpdate = m_lastShootUpdate + getIView()->getTimeStep();
	float timePerBullet = 60000.0f/m_bulletRate;
	
	int nBulletShoot = core::floor32(m_lastShootUpdate/timePerBullet);	
	
	if ( nBulletShoot > m_currentBullets )
		nBulletShoot = m_currentBullets;		

	if ( nBulletShoot > 0 )	
	{
		// todo shoot bullet
 		m_currentBullets = m_currentBullets - nBulletShoot;		

		// ....
		// ....
		CGameObject *owner = m_gameObject->getParent();
		if ( owner == NULL )
			return;

		CGameObject *zone =	owner->getParent();
		
		if ( zone == NULL || zone->getObjectType() != CGameObject::ZoneObject )
			return;

		CGameObject *bulletMgr = ((CZone*)zone)->getBulletManager();
		CBulletRayComponent *bulletRayComp = (CBulletRayComponent*)bulletMgr->getComponent(CGameComponent::BulletRayComponent);
		
		// get camera ray
		CGameCamera* cam = CGameLevel::getCurrentLevel()->getCamera();
		core::line3df ray;
		ray.start = cam->getPosition();
		
		core::vector3df baseDirection = (cam->getTarget() - ray.start).normalize();

		for (int i = 0; i < nBulletShoot; i++)
		{
			core::vector3df d = baseDirection;

			// rotate vibrate
			float n = (rand()%1000)<500?1.0f:-1.0f;
			float f = n*(rand()%1000)/1000.0f;
			d.rotateXYBy(f * m_vibrateAngle);

			n = (rand()%1000)<500?1.0f:-1.0f;
			f = n*(rand()%1000)/1000.0f;
			d.rotateYZBy(f * m_vibrateAngle);

			n = (rand()%1000)<500?1.0f:-1.0f;
			f = n*(rand()%1000)/1000.0f;
			d.rotateXZBy(f * m_vibrateAngle);			
			
			// finish calc ray
			ray.end = ray.start + d*50000.0f;

			// shoot bullet
			bulletRayComp->addBulletRay(ray, m_damage, m_damageThrowObj, m_gameObject, owner);
		}

		// ....

		// set shoot time
		m_lastShootUpdate = m_lastShootUpdate - nBulletShoot*timePerBullet;		
		
		// set muzzle
		m_muzzleTime = 100.0f;
		m_muzzleScale = 1.0f + ((rand()%999)/999.0f)*1.0f;
	}
	
	if ( m_currentBullets == 0 )
	{
		// todo no bullet :(
	}

}

// updateMuzzle
// update muzzle
void CGunComponent::updateMuzzle()
{
	bool show = true;

	m_muzzleTime = m_muzzleTime - getIView()->getTimeStep();
	
	if ( m_muzzleTime <= 0.0f )
	{
		m_muzzleTime = 0.0f;
		show = false;
	}

	if ( m_muzzle )
	{
		m_muzzle->setScale(core::vector3df(1,m_muzzleScale,m_muzzleScale));
		m_muzzle->setVisible(show);
	}

}


// loadData
// load data to serializable
void CGunComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
}

// shoot
// shoot bullet
void CGunComponent::shoot(bool b)
{
	if ( b == true )
		m_nextState = CGunComponent::Shoot;
	else
		m_nextState = CGunComponent::Normal;
}

// reload
void CGunComponent::reload(bool b)
{
	if ( b == true )
	{
		// reload the gun
		if ( m_currentBullets < m_numBullets )	
			m_nextState = CGunComponent::Reload;	
	}
	else
	{
		// reload finished
		m_nextState = CGunComponent::Normal;
	}
}

// needReload
bool CGunComponent::needReload()
{
	return (m_currentBullets == 0);
}