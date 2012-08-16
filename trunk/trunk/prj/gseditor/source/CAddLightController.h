#ifndef ADDLIGHT_CONTROLLER
#define ADDLIGHT_CONTROLLER

#include "stdafx.h"
#include "IController.h"
#include "CBaseAddController.h"
#include "CGameObject.h"
#include "CLightObject.h"

class CAddLightController: public CBaseAddController
{
protected:
	CLightObject	*m_temp;
	
public:
	CAddLightController();
	virtual ~CAddLightController();

	virtual void initController();

	virtual void cancelController();
			
	virtual void onLMouseUp		(int x, int y);

	virtual void onRMouseUp		(int x, int y);

	virtual void onMouseMove	(int x, int y);
};


#endif