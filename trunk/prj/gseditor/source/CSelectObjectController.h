#ifndef _SELECTOBJECT_CONTROLLER_
#define _SELECTOBJECT_CONTROLLER_

#include "stdafx.h"
#include "IController.h"
#include "CGameObject.h"

class CSelectObjectController: public IController
{
protected:
	bool		m_isRMouseDown;
	bool		m_isLMouseDown;
	
	int			m_beginX;
	int			m_beginY;

	bool		m_selectState;
	
	CGameObject			*m_hitOnObj;
	core::vector3df		m_hitPosition;
public:
	CSelectObjectController();

	virtual ~CSelectObjectController();
	
	virtual void render();

	virtual void initController();

	virtual void cancelController();

	virtual void onKeyUp		(int keyCode);	
	
	virtual void onLMouseDown	(int x, int y);

	virtual void onLMouseUp		(int x, int y);

	virtual void onRMouseDown	(int x, int y);

	virtual void onRMouseUp		(int x, int y);

	virtual void onMouseMove	(int x, int y);

	virtual void onMouseWhell	(bool scrollDown, int x, int y);

	// setStateForSelectObject
	// set state when mouse up
	virtual void setStateForSelectObject();
};


#endif