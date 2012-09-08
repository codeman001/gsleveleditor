#include "stdafx.h"
#include "CTouchManager.h"
#include "IView.h"

CTouchManager::CTouchManager()
{
	// reset to touchEvent
	for ( int i = 0; i < MAX_TOUCH; i++ )
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
	for ( int i = 0; i < MAX_TOUCH; i++ )
	{
		// set touch variable
		event.EventType = irr::EET_MOUSE_INPUT_EVENT;
		event.EventControlID = i;
		event.MouseInput.X = m_touch[i].x;
		event.MouseInput.Y = m_touch[i].y;
		event.MouseInput.Shift = false;
		event.MouseInput.Control = false;

        // touch 0 is UI touch
#if defined(IOS) || defined(ANDROID)
        if ( i == 0 )
        {
            device->getCursorControl()->setPosition(m_touch[0].x, m_touch[0].y);
        }
#endif

		if ( m_touch[i].touchEvent == CTouchManager::TouchDown )
		{			
			event.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
			device->postEventFromUser(event);
            
            //printf("touch down: %d %d\n", m_touch[i].x, m_touch[i].y);
		}
		else if ( m_touch[i].touchEvent == CTouchManager::TouchMove )
		{
			event.MouseInput.Event = irr::EMIE_MOUSE_MOVED;
			device->postEventFromUser(event);
		}
		else if ( m_touch[i].touchEvent == CTouchManager::TouchUp )
		{
			event.MouseInput.Event = irr::EMIE_LMOUSE_LEFT_UP;
			device->postEventFromUser(event);
		}
	}

	// reset to touchEvent
	for ( int i = 0; i < MAX_TOUCH; i++ )
	{
		if ( m_touch[i].touchEvent == CTouchManager::TouchUp )
		{
			m_touch[i].touchEvent	= CTouchManager::TouchNone;
			m_touch[i].x			= 0;
			m_touch[i].y			= 0;
			m_touchID[i]			= -1;
		}
	}
}

// getTouchID
// convert deviceID to 0, 1, 2....
int CTouchManager::getTouchID(int deviceID)
{
	int i = 0;

	// search deviceID
	for ( i = 0; i < MAX_TOUCH; i++ )
	{
		if ( m_touchID[i] == deviceID )
		{
			return i;
		}
	}
		
	// new touch ID
	for ( i = 0; i < MAX_TOUCH; i++ )
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
void CTouchManager::touchEvent(TouchEvent touchEvent, int x, int y, int id)
{
	int touchID = getTouchID( id );
	if ( touchID == -1 )
		return;
	
	m_touch[touchID].touchEvent	= touchEvent;
	m_touch[touchID].x			= x;
	m_touch[touchID].y			= y;
	m_touchID[touchID]			= id;
}