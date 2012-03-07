#ifndef _ROTATEOBJECT_CONTROLLER_
#define _ROTATEOBJECT_CONTROLLER_

#include "CSelectObjectController.h"

class CRotateObjectController: public IController
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