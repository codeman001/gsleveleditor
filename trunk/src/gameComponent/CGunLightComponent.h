//
//  File.h
//  gameplay
//
//  Created by Pham Hong Duc on 12/30/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _CGUNLIGHTCOMP_
#define _CGUNLIGHTCOMP_

#include "IObjectComponent.h"
#include "CGameComponent.h"
#include "CGameObject.h"
#include "CLightObject.h"

class CGunLightComponent: public IObjectComponent
{
protected:
    float           m_lighTime;
    CLightObject    *m_lightObj;
public:  
	CGunLightComponent(CGameObject* obj);
	virtual ~CGunLightComponent();
    
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
        
    // setLightTime
    // set light
    void setLightTime(float t);
    
};

#endif
