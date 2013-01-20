#ifndef _CBULLETRAYCOMP_
#define _CBULLETRAYCOMP_

#include "IObjectComponent.h"
#include "CGameComponent.h"
#include "CGameObject.h"

class CBulletRayComponent: public IObjectComponent
{
protected:
    
public:  
	CBulletRayComponent(CGameObject* obj);
	virtual ~CBulletRayComponent();
    
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