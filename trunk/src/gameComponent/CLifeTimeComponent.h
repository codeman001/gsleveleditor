#ifndef _CLIFETIMECOMP_
#define _CLIFETIMECOMP_

#include "IObjectComponent.h"
#include "CGameComponent.h"
#include "CGameObject.h"

class CLifeTimeComponent: public IObjectComponent
{
protected:
    float           m_lifeTime;

public:  
	CLifeTimeComponent(CGameObject* obj);
	virtual ~CLifeTimeComponent();
    
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
        
    // setLifeTime
    // set light
    inline void setLifeTime(float t)
	{
		m_lifeTime = t;
	}

};

#endif