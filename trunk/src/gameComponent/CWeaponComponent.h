#ifndef _CWEAPONCOMPONENT_H_
#define _CWEAPONCOMPONENT_H_

#include "IObjectComponent.h"
#include "CColladaMeshComponent.h"
#include "CGameComponent.h"

class CWeaponComponent: public IObjectComponent
{
public:
	enum EWeaponType
	{
		Unknown = 0,
		ShotGun,
		Count
	};
protected:
	EWeaponType	m_type;
	float		m_damage;
	float		m_rate;
	float		m_range;
	float		m_mobility;

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

public:
	// begin component property
	// ----------------------
	inline EWeaponType getWeaponType()
	{
		return m_type;
	}

	inline float getDamage()
	{
		return m_damage;
	}

	inline float getRate()
	{
		return m_rate;
	}

	inline float getRange()
	{
		return m_range;
	}

	inline float getMobility()
	{
		return m_mobility;
	}
	// end component property
	// ----------------------
};

#endif