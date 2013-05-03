#ifndef _WEAPONCOMPONENT_
#define _WEAPONCOMPONENT_

#include "CGameComponent.h"
#include "gameConfig/CGameConfig.h"

class CWeaponComponent: public IObjectComponent
{
public:
	enum EWeaponType
	{
		Unknown = 0,
		Gun,
		NumWPType
	};

	struct SWeapon
	{
		CGameObject					*m_obj;
		CGameConfig::SWeaponInfo	*m_info;
		int							m_num;
		EWeaponType					m_type;

		SWeapon()
		{
			m_obj = NULL;
			m_info = NULL;
			m_num = 0;
			m_type = Unknown;
		}
	};

protected:
	std::vector<SWeapon>			m_weapons;
	SWeapon							*m_activeWeapon;

public:  
	CWeaponComponent(CGameObject* obj);
	virtual ~CWeaponComponent();
    
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

	// getWeapon
	// get weapon info
	SWeapon* getWeapon( const char* weaponName );

	// addWeaponToInventory
	// add to component
	void addWeaponToInventory( const char* weaponName );

	// setActiveWeapon
	// set weapon
	void setActiveWeapon( const char* weaponName );

	// updateActiveWeapon
	// update weapon
	void updateActiveWeapon();

	// shootActiveWeapon
	// shoot weapon
	void shootActiveWeapon(bool b);

	// reloadActiveWeapon
	// reload weapon
	void reloadActiveWeapon();

}; 

#endif