#ifndef _TOUCH_MGR_H
#define _TOUCH_MGR_H

#include "stdafx.h"

#define MAX_TOUCH	4

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
	};

protected:
	STouchStatus	m_touch		[ MAX_TOUCH ];
	int				m_touchID	[ MAX_TOUCH ];

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