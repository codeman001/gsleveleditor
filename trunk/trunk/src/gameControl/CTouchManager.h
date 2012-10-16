#ifndef _TOUCH_MGR_H
#define _TOUCH_MGR_H

#include "stdafx.h"

class CTouchManager
{
public:
	enum TouchEvent
	{
		TouchNone = 0,
		TouchDown,
		TouchMove,
		TouchUp,		
	};

	struct STouchStatus
	{
		TouchEvent	touchEvent;		
		int			x;
		int			y;
        bool        doTouchDown;
	};

protected:
	STouchStatus	m_touch		[ MAX_MULTITOUCH ];
	int				m_touchID	[ MAX_MULTITOUCH ];

public:
	CTouchManager();
	virtual ~CTouchManager();

	// update
	// reset touch & send event to irrlicht device
	void update();

	// getTouchID
	// convert deviceID to 0, 1, 2....
	int getTouchID(int deviceID);

	// touchEvent
	// update touch event
	void touchEvent(TouchEvent touchEvent, int x, int y, int id);
};

#endif