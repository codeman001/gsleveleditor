#include "stdafx.h"
#include "CGameControl.h"

CGameControl::CGameControl()
{
    m_isEnable = false;
}

CGameControl::~CGameControl()
{

}

// update
// main loop update
void CGameControl::update()
{
    
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
        s32 mouseX = event.MouseInput.X;
        s32 mouseY = event.MouseInput.Y;
        
        if ( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
        {
            
        }
        else if ( event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
        {
            
        }
        else if ( event.MouseInput.Event == EMIE_MOUSE_MOVED )
        {
            
        }
    }
    return true;
}