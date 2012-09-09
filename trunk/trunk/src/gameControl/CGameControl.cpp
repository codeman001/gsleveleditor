#include "stdafx.h"
#include "CGameControl.h"

CGameControl::CGameControl()
{
    m_isEnable = false;
}

CGameControl::~CGameControl()
{

}

// setNameFx
// set name of flash obj
void CGameControl::setNameFx( const std::string& nameDpadTouch, const std::string& nameDPad,const std::string& nameDpadMove )
{
    m_fxnameDpadTouch = nameDpadTouch;
    m_fxnameDpad = nameDPad;
    m_fxnameDpadMove = nameDpadMove;
    
    // flash gamehud
    CMenuFx *menuFx = CGameUI::getInstance()->getFlash("uiGameHud");
    
    // get touch touch dpad
    CMenuFxObj *dpadTouch = menuFx->findObj((char*) nameDpadTouch.c_str());
    if ( dpadTouch )
    {
        int x,y,w,h;
        dpadTouch->getBound(&x, &y, &w, &h);
        
        m_touchDpad.UpperLeftCorner = core::vector2di(x,y);
        m_touchDpad.LowerRightCorner = core::vector2di(x+w, y+h);
        
        dpadTouch->drop();
    }
    
    // set fx for dpad
    m_moveDpad.setDpadFxName(nameDPad, nameDpadMove);
}

// update
// main loop update
void CGameControl::update()
{
    m_moveDpad.update();
}

// OnEvent
// IEventReceiver implement
bool CGameControl::OnEvent(const SEvent& event)
{
    if ( m_isEnable == false )
        return true;
    
    if ( event.EventType == EET_MOUSE_INPUT_EVENT )
    {
        s32 controlID = event.EventControlID;
        core::vector2di mousePos(event.MouseInput.X, event.MouseInput.Y);
        
        if ( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
        {
            // touch on dpad
            if ( m_touchDpad.isPointInside( mousePos ) == true )
            {
                if ( m_moveDpad.getControlID() == -1 )
                {
                    m_moveDpad.setControlID( controlID );
                    m_moveDpad.setDpadPos( event.MouseInput.X, event.MouseInput.Y );
                }
            }
            
        }
        else if ( event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
        {
            // release dpad move
            if ( m_moveDpad.getControlID() == controlID )
                m_moveDpad.setControlID( -1 );

        }
        else if ( event.MouseInput.Event == EMIE_MOUSE_MOVED )
        {
            // move dpad
            if ( m_moveDpad.getControlID() == controlID )
                m_moveDpad.setTouchPos( event.MouseInput.X, event.MouseInput.Y );
        }
    }
    return true;
}