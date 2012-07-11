#include "stdafx.h"
#include "IView.h"
#include "CWeaponComponent.h"
#include "gameLevel/CGameLevel.h"


///////////////////////////////////////////////////////////////////////
// IObjectComponent overide implement
///////////////////////////////////////////////////////////////////////

CWeaponComponent::CWeaponComponent(CGameObject* obj)
	:IObjectComponent(obj, CGameComponent::WeaponComponent)
{
	m_type		= CWeaponComponent::ShotGun;
	m_damage	= 0.0f;
	m_rate		= 0.0f;
	m_range		= 0.0f;
	m_mobility	= 0.0f;
}

CWeaponComponent::~CWeaponComponent()
{
}

// init
// run when init object
void CWeaponComponent::initComponent()
{
}

// update
// run when update per frame
void CWeaponComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CWeaponComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( CGameComponent::s_compType[ m_componentID ] );
	pObj->addInt(	"weaponType",(int)m_type,true);
	pObj->addFloat( "damage",	m_damage, true );
	pObj->addFloat( "rate",		m_rate, true );
	pObj->addFloat( "range",	m_range, true );
	pObj->addFloat( "mobility", m_mobility, true );
}

// loadData
// load data to serializable
void CWeaponComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
	m_type		= (EWeaponType)pObj->readInt();
	m_damage	= pObj->readFloat();
	m_rate		= pObj->readFloat();
	m_range		= pObj->readFloat();
	m_mobility	= pObj->readFloat();
}