#ifndef _WATER_COMPONENT_H_
#define _WATER_COMPONENT_H_


#include "CGameObject.h"
#include "IObjectComponent.h"
#include "CGameWaterSceneNode.h"

class CWaterComponent: public IObjectComponent
{	
protected:
	CGameWaterSceneNode		*m_waterNode;

public:
	CWaterComponent( CGameObject *pObj );

	virtual ~CWaterComponent();

	// init
	// run when init object
	virtual void initComponent();

	// updateComponent
	// update object by frame
	virtual void updateComponent();

	// initGrassNode
	// init grass
	void initWaterNode();
public:
	// saveData
	// save data to serializable
	void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	void loadData( CSerializable* pObj );
};

#endif