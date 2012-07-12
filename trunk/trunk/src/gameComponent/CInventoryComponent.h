#ifndef _CINVETORYCOMPONENT_H_
#define _CINVETORYCOMPONENT_H_

#include "IObjectComponent.h"
#include "CColladaMeshComponent.h"
#include "CGameComponent.h"

class CInventoryComponent: public IObjectComponent
{
public:
	struct SInventoryItem
	{
		string			m_nameItem;
		CGameObject*	m_item;
		int				m_count;

		SInventoryItem()
		{
			m_item	= NULL;
			m_count = 0;
		}
	};

protected:	
		
	vector<SInventoryItem>			m_inventory;
	int								m_activeItemSlot;

public:
	CInventoryComponent(CGameObject* obj);
	virtual ~CInventoryComponent();

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
	
	// addItemToInventory
	// add a game item to inventory
	bool addItemToInventory( const char *objectTemplate );

	// getItemSlot
	// get index of item on inventory
	int getItemSlot( const char *objectTemplate );

	// setActiveItem
	// active item
	void setActiveItem( const char *objectTemplate );

	// getActiveItem
	// get active item
	SInventoryItem* getActiveItem()
	{
		if ( m_activeItemSlot < 0 )
			return NULL;
		return &m_inventory[m_activeItemSlot];
	}
};

#endif