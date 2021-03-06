#ifndef _ADDOBJECT_CONTROLLER_
#define _ADDOBJECT_CONTROLLER_

#include "stdafx.h"
#include "IController.h"
#include "CBaseAddController.h"
#include "CGameObject.h"

class CAddObjectController: public CBaseAddController
{
protected:

public:

	virtual void initController();

	virtual void cancelController();
			
	virtual void onLMouseUp		(int x, int y);

	virtual void onMouseMove	(int x, int y);
};

#endif