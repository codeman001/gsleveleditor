#ifndef _BASEADD_CONTROLLER_
#define _BASEADD_CONTROLLER_

#include "stdafx.h"
#include "IController.h"

#include "CGameObject.h"
#include "CZone.h"

class CBaseAddController: public IController
{
protected:

public:

	virtual bool getPickPosition( core::vector3df *pos );

};

#endif