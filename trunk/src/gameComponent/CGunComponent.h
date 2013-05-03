#ifndef _GUNCOMPONENT_H_
#define _GUNCOMPONENT_H_

#include "CGameComponent.h"
#include "CColladaMeshComponent.h"
#include "gameConfig/CGameConfig.h"

class CGunComponent: public IObjectComponent
{
protected:
	CGameConfig::SWeaponInfo	*m_info;
	
	enum EGunState
	{
		None = 0,
		Normal,
		Shoot,
		Reload
	};

	EGunState					m_state;
	EGunState					m_nextState;

	int							m_numBullets;
	int							m_bulletRate;
	
	int							m_currentBullets;

	CColladaMeshComponent		*m_collada;
	CGameColladaSceneNode		*m_muzzle;

	float						m_lastShootUpdate;
	float						m_muzzleTime;
	float						m_muzzleScale;

public:
	CGunComponent(CGameObject* obj, CGameConfig::SWeaponInfo* info);
	virtual ~CGunComponent();
    
    // init
	// run when init object
	virtual void initComponent();
    
	// update
	// run when update per frame
	virtual void updateComponent();
    
	// saveData
	// save data to serializable
	virtual void saveData( CSerializable* pObj );
    
	// loadData
	// load data to serializable
	virtual void loadData( CSerializable* pObj );

	// updateShoot
	// shoot
	void updateShoot();

	// updateMuzzle
	// update muzzle
	void updateMuzzle();

	// shoot
	// shoot bullet
	void shoot(bool b);

	// reload
	void reload(bool b);

	// needReload
	bool needReload();

	// getOwner
	// get owner of gun
	CGameObject* getOwner()
	{
		return m_gameObject->getParent();
	}
};

#endif