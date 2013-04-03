#include "stdafx.h"
#include "CTouchManager.h"
#include "CGameControl.h"
#include "IView.h"

CTouchManager::CTouchManager()
{
	// reset to touchEvent
	for ( int i = 0; i < MAX_MULTITOUCH; i++ )
	{
		m_touch[i].touchEvent	= CTouchManager::TouchNone;
		m_touch[i].x			= 0;
		m_touch[i].y			= 0;			
		m_touchID[i]			= -1;		
	}
}

CTouchManager::~CTouchManager()
{
}

// update
// reset touch & send event to irrlicht device
void CTouchManager::update()
{
	irr::SEvent		event;
	IrrlichtDevice *device = getIView()->getDevice();

	// notify event to irrlicht device
	for ( int i = 0; i < MAX_MULTITOUCH; i++ )
	{
		// set touch variable
		event.EventType = irr::EET_MOUSE_INPUT_EVENT;
		event.EventControlID = i;
		event.MouseInput.X = m_touch[i].x;
		event.MouseInput.Y = m_touch[i].y;
		event.MouseInput.Shift = false;
		event.MouseInput.Control = false;

		if ( m_touch[i].touchEvent == CTouchManager::TouchDown )
		{            
            m_touch[i].doTouchDown = false;
			event.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
			device->postEventFromUser(event);  
		}
		else if ( m_touch[i].touchEvent == CTouchManager::TouchMove )
		{
            if ( m_touch[i].doTouchDown == true )
            {
                // we fix if lost a touchdown event
                event.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
                m_touch[i].doTouchDown = false;
            }
            else
            {
                event.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
            }
			device->postEventFromUser(event);
		}
		else if ( m_touch[i].touchEvent == CTouchManager::TouchUp )
		{
			event.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
			device->postEventFromUser(event);
		}
	}

	// reset to touchEvent
	for ( int i = 0; i < MAX_MULTITOUCH; i++ )
	{
		if ( m_touch[i].touchEvent == CTouchManager::TouchUp )
		{
			m_touch[i].touchEvent	= CTouchManager::TouchNone;
			m_touch[i].x			= 0;
			m_touch[i].y			= 0;
			m_touchID[i]			= -1;
		}
        else if ( m_touch[i].touchEvent == CTouchManager::TouchDown )
        {
            // switch to touch move event for next update
            m_touch[i].touchEvent = CTouchManager::TouchMove;
        }
	}
}

// getTouchID
// convert deviceID to 0, 1, 2....
int CTouchManager::getTouchID(int deviceID)
{
	int i = 0;

	// search deviceID
	for ( i = 0; i < MAX_MULTITOUCH; i++ )
	{
		if ( m_touchID[i] == deviceID )
		{
			return i;
		}
	}
		
	// new touch ID
	for ( i = 0; i < MAX_MULTITOUCH; i++ )
	{
		if ( m_touchID[i] == -1 )
		{
			return i;
		}
	}

	return -1;
}

// touchEvent
// update touch event
void CTouchManager::touchEvent(ETouchEvent touchEvent, int x, int y, int id)
{
	int touchID = getTouchID( id );
	if ( touchID == -1 )
	{
		os::Printer::log("***** Error touchID!!! \n");
		return;
	}

	if ( touchEvent == CTouchManager::TouchDown )
	{
		// makesure we alway call TouchDown one time (double event on android)
		if (	m_touch[touchID].touchEvent == CTouchManager::TouchDown ||
				m_touch[touchID].touchEvent == CTouchManager::TouchMove )
		{
			touchEvent = CTouchManager::TouchMove;
		}
	}

	m_touch[touchID].touchEvent	= touchEvent;
	m_touch[touchID].x			= x;
	m_touch[touchID].y			= y;

    // make sure we alway call TouchDown one time (lost event if touch quickly)
    if ( touchEvent == CTouchManager::TouchDown )
        m_touch[touchID].doTouchDown = true;
    
	m_touchID[touchID]			= id;    
}