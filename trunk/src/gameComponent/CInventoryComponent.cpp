#include "stdafx.h"
#include "IView.h"
#include "CInventoryComponent.h"
#include "CZone.h"
#include "gameLevel/CGameLevel.h"

CInventoryComponent::CInventoryComponent(CGameObject* obj)
	:IObjectComponent( obj, CGameComponent::InventoryComponent )
{
	m_activeItemSlot = -1;
}

CInventoryComponent::~CInventoryComponent()
{
}

// init
// run when init object
void CInventoryComponent::initComponent()
{
}

// update
// run when update per frame
void CInventoryComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CInventoryComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( CGameComponent::s_compType[ m_componentID ] );
}

// loadData
// load data to serializable
void CInventoryComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
}


// addObjectToInventory
// add a obj to inventory
bool CInventoryComponent::addItemToInventory( const char *objectTemplate )
{
	int slot = getItemSlot( objectTemplate );
	if ( slot > 0 )
	{
		m_inventory[slot].m_count++;
		return true;
	}
	else
	{
		CZone* zone = (CZone*)m_gameObject->getParent();
		
		wchar_t objectTemplateW[512];
		uiString::convertUTF8ToUnicode( objectTemplate, (unsigned short*) objectTemplateW );

		CGameObject* obj = zone->createObject( objectTemplateW );
		if ( obj )
		{
			obj->setVisible( false );
			obj->setEnable( false );

			CInventoryComponent::SInventoryItem item;
			item.m_nameItem = objectTemplate;
			item.m_item		= obj;
			item.m_count	= 0;

			m_inventory.push_back( item );			
			return false;
		}

	}

	return false;
}

// getItemSlot
// get index of item on inventory
int CInventoryComponent::getItemSlot( const char *objectTemplate )
{
	for ( int i = 0, n = (int)m_inventory.size(); i < n; i++ )
	{
		if ( m_inventory[i].m_nameItem == objectTemplate )
		{
			return i;
		}
	}
	return -1;
}

// setActiveItem
// active item
void CInventoryComponent::setActiveItem( const char *objectTemplate )
{
	m_activeItemSlot = getItemSlot(objectTemplate);
}