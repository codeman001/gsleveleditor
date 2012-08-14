#ifndef LIGHTING_COMPONENT
#define LIGHTING_COMPONENT

#include "IObjectComponent.h"
#include "CGameObject.h"

class CLightingComponent: public IObjectComponent
{
protected:
	
public:
	CLightingComponent(CGameObject *pObj);

	virtual ~CLightingComponent();	

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
};

#endif