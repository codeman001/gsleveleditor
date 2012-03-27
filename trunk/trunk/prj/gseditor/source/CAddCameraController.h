#ifndef _ADDCAMERA_CONTROLLER_
#define _ADDCAMERA_CONTROLLER_

#include "stdafx.h"
#include "IController.h"
#include "CGameObject.h"
#include "CGameCamera.h"

class CAddCameraController: public IController
{
protected:
	CGameCamera	*m_temp;
	
public:
	CAddCameraController();

	virtual ~CAddCameraController();

	virtual void initController();

	virtual void cancelController();
			
	virtual void onLMouseUp		(int x, int y);

	virtual void onRMouseUp		(int x, int y);

	virtual void onMouseMove	(int x, int y);
};

#endif