#ifndef ADDTRIGGER_CONTROLLER
#define ADDTRIGGER_CONTROLLER

#include "stdafx.h"
#include "IController.h"
#include "CBaseAddController.h"
#include "CGameObject.h"
#include "CTrigger.h"

class CAddTriggerController: public CBaseAddController
{
protected:
	CTrigger	*m_temp;
	
public:
	CAddTriggerController();
	virtual ~CAddTriggerController();

	virtual void initController();

	virtual void cancelController();
			
	virtual void onLMouseUp		(int x, int y);

	virtual void onRMouseUp		(int x, int y);

	virtual void onMouseMove	(int x, int y);
};


#endif