#ifndef _TERRAIN_COMPONENT_
#define _TERRAIN_COMPONENT_

#include "CGameObject.h"
#include "IObjectComponent.h"

class CTerrainComponent: public IObjectComponent
{
public:
	CTerrainComponent( CGameObject *pObj );

	virtual ~CTerrainComponent();

	// init
	// run when init object
	virtual void initComponent();

	// updateComponent
	// update object by frame
	virtual void updateComponent();

	// saveData
	// save data to serializable
	void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	void loadData( CSerializable* pObj );
};

#endif