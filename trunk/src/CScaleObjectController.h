#ifndef _SCALEOBJECT_CONTROLLER_
#define _SCALEOBJECT_CONTROLLER_

#include "stdafx.h"
#include "IController.h"
#include "CGameObject.h"

class CScaleObjectController: public IController
{
protected:
	bool	m_isLMouseDown;

	int		m_beginX;
	int		m_beginY;

	float	m_oldYaw;
public:
	virtual void initController();

	virtual void cancelController();
	
	virtual void onLMouseDown	(int x, int y);
	
	virtual void onLMouseUp		(int x, int y);

	virtual void onMouseMove	(int x, int y);	
};


#endif