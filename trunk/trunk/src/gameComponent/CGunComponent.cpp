#include "stdafx.h"
#include "IView.h"
#include "CGunComponent.h"

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

	// parse info
	std::vector<CGameConfig::SParam>::iterator i = m_info->params.begin(), end = m_info->params.end();
	while ( i != end )
	{
		const char *value = i->value.c_str();

		if ( i->name == "rateShoot" )
			m_bulletRate = atoi( value );
		else if ( i->name == "numBullet" )
			m_numBullets = atoi( value );

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
			m_nextState = Normal;
			
			printf("Reload the gun\n");
			m_currentBullets = m_numBullets;
			m_lastShootUpdate = 0.0f;
			m_muzzleTime = 0.0f;
		}
		break;
	}
	
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
	float timePerBullet = 1000.0f/m_bulletRate;
	
	int nBulletShoot = core::round32(m_lastShootUpdate/timePerBullet);	
	
	if ( nBulletShoot > m_currentBullets )
		nBulletShoot = m_currentBullets;		

	if ( nBulletShoot > 0 )	
	{
		// todo shoot bullet
		m_currentBullets = m_currentBullets - nBulletShoot;
		printf("Bullets: %d\n", m_currentBullets);

		// ....
		// ....
		// ....

		// set shoot time
		m_lastShootUpdate = m_lastShootUpdate - nBulletShoot*timePerBullet;		
		
		// set muzzle
		m_muzzleTime = 1000.0f/24.0f;
		m_muzzleScale = 1.0f + ((rand()%999)/999.0f)*1.0f;
	}

	// need reload the gun
	if ( m_currentBullets == 0 )
	{
		m_nextState = Reload;
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
void CGunComponent::reload()
{
	if ( m_currentBullets < m_numBullets )
		m_nextState = CGunComponent::Reload;
}