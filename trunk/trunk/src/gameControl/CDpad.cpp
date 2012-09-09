//
//  CDpad.cpp
//  gameplay
//
//  Created by Pham Hong Duc on 9/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"
#include "CDpad.h"

CDpad::CDpad()
{
    m_fxDpad = NULL;
    m_fxDpadMove = NULL;
    
    m_controlID = -1;
}

CDpad::~CDpad()
{
    if ( m_fxDpad )
        m_fxDpad->drop();
    if ( m_fxDpadMove )
        m_fxDpadMove->drop();
}

// update
// main loop update
void CDpad::update()
{
    if ( m_controlID == -1 )
    {
        // need hide dpad
        if ( m_fxDpad )
            m_fxDpad->setVisible(false);
    }
    else 
    {
        m_fxDpad->setVisible(true);
        m_fxDpad->setPosition( m_dpadPos.X, m_dpadPos.Y );
    }
}

// setVisible
// show joystick on screen
void CDpad::setVisible( bool b )
{
    m_visible = b;
}

// setDpadPos
// set position of joystick
void CDpad::setDpadPos( int x, int y )
{
    m_dpadPos.X = x;
    m_dpadPos.Y = y;
    
    m_touchPos.X = x;
    m_touchPos.Y = y;
}

// setTouchPos
// set position of touch
void CDpad::setTouchPos( int x, int y )
{
    m_touchPos.X = x;
    m_touchPos.Y = y;
}

// setDpadFxName
void CDpad::setDpadFxName( const std::string& name, const std::string& move )
{
    if ( m_fxDpad )
        m_fxDpad->drop();
    if ( m_fxDpadMove )
        m_fxDpadMove->drop();
    
    CMenuFx *menuFx = CGameUI::getInstance()->getFlash("uiGameHud");
    
    m_fxDpad = menuFx->findObj( (char*)name.c_str());
    m_fxDpadMove = menuFx->findObj( (char*)move.c_str()); 
}