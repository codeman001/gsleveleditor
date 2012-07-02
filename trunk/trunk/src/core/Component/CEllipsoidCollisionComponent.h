#ifndef _ELLIPSOILDCOLCOMPONENT_
#define _ELLIPSOILDCOLCOMPONENT_

#include "CGameObject.h"
#include "IObjectComponent.h"

class CEllipsoidCollisionComponent: public IObjectComponent
{
public:
	CEllipsoidCollisionComponent( CGameObject *pObj );

	virtual ~CEllipsoidCollisionComponent();

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