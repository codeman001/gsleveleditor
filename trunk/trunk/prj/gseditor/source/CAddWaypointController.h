#ifndef _ADDWAYPOINT_CONTROLLER_
#define _ADDWAYPOINT_CONTROLLER_

#include "stdafx.h"
#include "IController.h"
#include "CBaseAddController.h"
#include "CGameObject.h"
#include "CWaypoint.h"

class CAddWaypointController: public CBaseAddController
{
protected:
	CWayPoint	*m_temp;

	CWayPoint	*m_connect;
public:
	CAddWaypointController();
	virtual ~CAddWaypointController();

	virtual void initController();

	virtual void cancelController();
			
	virtual void onLMouseUp		(int x, int y);

	virtual void onRMouseUp		(int x, int y);

	virtual void onMouseMove	(int x, int y);
};

#endif