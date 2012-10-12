//
//  CDpad.cpp
//  gameplay
//
//  Created by Pham Hong Duc on 9/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"
#include "CDpad.h"

#include "CGameControl.h"

#include "gameEvent.h"
#include "IView.h"

CDpad::CDpad()
{        
    m_controlID = -1;
	m_active = false;
}

CDpad::~CDpad()
{

}

// update
// main loop update
void CDpad::update()
{
    if ( m_controlID == -1 )
    {
        
        m_fxDpad.setVisible(false);
		m_fxDpadMove.setVisible(false);

		if ( m_active )
		{
			CGameControl::getInstance()->sendPlayerStopEvent();
			m_active = false;
		}
    }
    else 
    {
		m_active = true;

		core::position2di delta = m_touchPos - m_dpadPos;
		core::position2df moveVec((float)delta.X, (float)delta.Y);

        float fw, fh;
        CMenuFx *menuFx = CGameUI::getInstance()->getFlash("uiGameHud");
        menuFx->getFxScaleRatio( &fw, &fh );
        
		const float k_maxMove = 100.0f / fw;

		float length = moveVec.getLength();
		if ( length > k_maxMove )
			length = k_maxMove;

		// calc run angle
		moveVec.normalize();
		
		// run event
		float f = length/k_maxMove;
		if ( f >= 0.3f )
		{
			CGameControl::getInstance()->sendPlayerRunEvent( f, (float)moveVec.getAngle() - 90.0f, false );
		}
		else
		{
			CGameControl::getInstance()->sendPlayerStopEvent();
		}


		// recalc touch pos to render
		moveVec *= length;
		m_dpadMovePos = m_dpadPos + core::position2di((int)moveVec.X, (int)moveVec.Y);
       
		m_fxDpad.setVisible(true);
		m_fxDpad.setPosition( (int)(m_dpadPos.X * fw), (int)(m_dpadPos.Y * fh) );

		m_fxDpadMove.setVisible(true);
		m_fxDpadMove.setPosition( (int)(m_dpadMovePos.X * fw), (int)(m_dpadMovePos.Y * fh) );

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
    CMenuFx *menuFx = CGameUI::getInstance()->getFlash("uiGameHud");
    
    m_fxDpad = menuFx->findObj( (char*)name.c_str());
    m_fxDpadMove = menuFx->findObj( (char*)move.c_str()); 
}
