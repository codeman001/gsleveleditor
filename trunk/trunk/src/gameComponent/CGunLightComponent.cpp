//
//  CGunLightComponent.cpp
//  gameplay
//
//  Created by Pham Hong Duc on 12/30/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"
#include "IView.h"
#include "CGunLightComponent.h"
#include "CLightObject.h"

CGunLightComponent::CGunLightComponent(CGameObject* obj)
    :IObjectComponent(obj, CGameComponent::GunLightComponent)
{
    
}

CGunLightComponent::~CGunLightComponent()
{
    
}

// init
// run when init object
void CGunLightComponent::initComponent()
{
    
}

// update
// run when update per frame
void CGunLightComponent::updateComponent()
{
    if (m_gameObject->getObjectType() == CGameObject::LightObject )
    {
        CLightObject *lightObj = (CLightObject*)m_gameObject;
        ILightSceneNode *lightNode = lightObj->getLightSceneNode();
        
        float timeStep = getIView()->getTimeStep();

        m_lighTime = m_lighTime - timeStep;
        if ( m_lighTime <= 0 )
        {
            lightNode->setVisible(false);
            m_lighTime = 0;
        }
        else 
        {
            lightNode->setVisible(true);
        }
    }
}

// saveData
// save data to serializable
void CGunLightComponent::saveData( CSerializable* pObj )
{
    pObj->addGroup( CGameComponent::s_compType[ m_componentID ] );
}

// loadData
// load data to serializable
void CGunLightComponent::loadData( CSerializable* pObj )
{
    pObj->nextRecord();
}