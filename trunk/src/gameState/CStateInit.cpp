#include "stdafx.h"
#include "CStateInit.h"

CStateInit::CStateInit()
{
}

CStateInit::~CStateInit()
{
}

void CStateInit::onCreate()
{	
	// init flash ui		
	CGameUI::getInstance()->openFlash("testflash","data/flashui/test.swf");	
			
	CMenuFx *fxMenu = CGameUI::getInstance()->getFlash("testflash");
	if ( fxMenu )
	{
		CMenuFxObj* fxObj =	fxMenu->getObj("testState.label");
		if ( fxObj )
		{				
			fxObj->setText("Hong duc");
			fxObj->drop();
		}
	}
}