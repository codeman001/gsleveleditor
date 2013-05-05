#include "stdafx.h"
#include "CGameObject.h"
#include "gameConfig/CGameConfig.h"

#include "CWeaponComponent.h"
#include "CGunComponent.h"

#include "gameDebug/CGameDebug.h"

const char *k_weaponTypeString[] = 
{
	"Unknown",
	"Gun",
    "NumWeaponType"
};

CWeaponComponent::CWeaponComponent(CGameObject* obj)
	:IObjectComponent(obj, CGameComponent::WeaponComponent)
{
	m_activeWeapon = NULL;
}

CWeaponComponent::~CWeaponComponent()
{
	std::vector<SWeapon>::iterator i = m_weapons.begin(), end = m_weapons.end();
	while (i != end)
	{
		if ( (*i).m_obj )
			delete (*i).m_obj;
		i++;
	}
	m_weapons.clear();
}
    
// init
// run when init object
void CWeaponComponent::initComponent()
{
	CEllipsoidCollisionComponent *ellipComp = (CEllipsoidCollisionComponent*)m_gameObject->getComponent(IObjectComponent::EllipsoidCollision);
	if ( ellipComp )
		ellipComp->setElipsoidCallback(this);
}

// update
// run when update per frame
void CWeaponComponent::updateComponent()
{
	std::vector<SWeapon>::iterator i = m_weapons.begin(), end = m_weapons.end();
	while (i != end)
	{
		if ( (*i).m_obj )
			(*i).m_obj->updateObject();

		i++;
	}
	
	updateActiveWeapon();
}

// updateActiveWeapon
// update weapon
void CWeaponComponent::updateActiveWeapon()
{
	if ( m_activeWeapon )
	{	
		CColladaMeshComponent* collada = (CColladaMeshComponent*)m_gameObject->getComponent(IObjectComponent::ColladaMesh);		
		CGameColladaSceneNode *snapNode = NULL;
		
		if ( m_activeWeapon->m_info->snap.empty() == false )
			snapNode = collada->getSceneNode( m_activeWeapon->m_info->snap.c_str() );

		// update position of gun
		if ( m_activeWeapon->m_type == Gun && snapNode )
		{
			core::matrix4 mat = snapNode->getAbsoluteTransformation();
			CGameObject *pObj = m_activeWeapon->m_obj;
			

			pObj->setVisible(true);
			pObj->setAbsoluteMatrix( mat );						
		}
	}
}

// shootActiveWeapon
// shoot weapon
void CWeaponComponent::shootActiveWeapon(bool b)
{
	if ( m_activeWeapon )
	{
		CGameObject *pObj = m_activeWeapon->m_obj;

		CGunComponent* gun = (CGunComponent*)pObj->getComponent(CGameComponent::GunComponent);
		if ( gun )
			gun->shoot(b);
	}
}

// reloadActiveWeapon
// reload weapon
void CWeaponComponent::reloadActiveWeapon(bool b)
{
	if ( m_activeWeapon )
	{
		CGameObject *pObj = m_activeWeapon->m_obj;

		CGunComponent* gun = (CGunComponent*)pObj->getComponent(CGameComponent::GunComponent);
		if ( gun )
			gun->reload(b);
	}
}

// needReloadActiveWeapon
// check to reload active weapon
bool CWeaponComponent::needReloadActiveWeapon()
{
	if ( m_activeWeapon )
	{
		CGameObject *pObj = m_activeWeapon->m_obj;

		CGunComponent* gun = (CGunComponent*)pObj->getComponent(CGameComponent::GunComponent);
		if ( gun )
			return gun->needReload();
	}
	return false;
}

// saveData
// save data to serializable
void CWeaponComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup(CGameComponent::s_compType[m_componentID]);
}

// loadData
// load data to serializable
void CWeaponComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
}

// getWeapon
// get weapon info
CWeaponComponent::SWeapon* CWeaponComponent::getWeapon( const char* weaponName )
{
	std::vector<SWeapon>::iterator i = m_weapons.begin(), end = m_weapons.end();
	while (i != end)
	{
		if ( (*i).m_info->name == weaponName )
			return &(*i);

		i++;
	}
	return NULL;
}

// addWeaponToInventory
// add to component
void CWeaponComponent::addWeaponToInventory( const char* weaponName )
{
	SWeapon* weapon = getWeapon(weaponName);

	if ( weapon == NULL )
	{
		CGameConfig::SWeaponInfo* wpInfo = CGameConfig::getInstance()->getWeaponByName(weaponName);
		if ( wpInfo )
		{
			CGameObject *wpObj = new CGameObject(NULL);
			
			// load model
			if ( wpInfo->model.empty() == false )
			{
				CColladaMeshComponent *collada = new CColladaMeshComponent(wpObj);
				collada->loadScene(wpInfo->model.c_str());
				wpObj->addComponent(collada);
				wpObj->initComponent();			
			}
			
			wpObj->setVisible(false);
			wpObj->setParent(m_gameObject);

			m_weapons.push_back( SWeapon() );
			SWeapon& w = m_weapons.back();

			w.m_obj = wpObj;
			w.m_num = 1;
			w.m_info = wpInfo;

			// find type of weapon
			for (int i = (int)Unknown; i < (int)NumWPType; i++)
			{
				if ( wpInfo->type == k_weaponTypeString[i] )
				{
					w.m_type = (EWeaponType)i;
					break;
				}
			}

			switch (w.m_type)
			{
			case Gun:
				{
					CGunComponent *gun = new CGunComponent(wpObj, wpInfo);
					wpObj->addComponent(gun);
				}
				break;
			}

			wpObj->initComponent();
		}
	}
	else
	{
		weapon->m_num++;
	}
}

// setActiveWeapon
// set weapon
void CWeaponComponent::setActiveWeapon( const char* weaponName )
{
	if ( m_activeWeapon )
		m_activeWeapon->m_obj->setVisible(false);

	m_activeWeapon = getWeapon(weaponName);
}

// _onUpdateEllipsoidBoundingBox
// update callback
void CWeaponComponent::_onUpdateEllipsoidBoundingBox(core::aabbox3df& box)
{
	if ( m_activeWeapon )
	{
		CGameObject *pObj = m_activeWeapon->m_obj;

		CColladaMeshComponent* collada = (CColladaMeshComponent*)m_gameObject->getComponent(IObjectComponent::ColladaMesh);		
		CGameColladaSceneNode *snapNode = NULL;
		
		if ( m_activeWeapon->m_info->snap.empty() == false )
		{
			// get node of position
			snapNode = collada->getSceneNode( m_activeWeapon->m_info->snap.c_str() );
			
			// transform box
			core::aabbox3df b = pObj->getSceneNode()->getBoundingBox();
			core::matrix4 mat;
			mat.setScale(core::vector3df(-1,1,1));
			mat.transformBoxEx(b);
			snapNode->LocalAbsoluteMatrix.transformBoxEx(b);

			// add box of weapon
			box.addInternalBox(b);

			/*
			core::matrix4 m = m_gameObject->getSceneNode()->getAbsoluteTransformation();

			// debug
			core::aabbox3df debugBox = box;
			m.transformBoxEx(debugBox);
			CGameDebug::getInstance()->addBoudingBox(debugBox, SColor(255,255,0,0));

			debugBox = b;
			m.transformBoxEx(debugBox);
			CGameDebug::getInstance()->addBoudingBox(debugBox, SColor(255,0,0,255));
			*/
		}
	}
}