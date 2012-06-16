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
	CMenuFx *menu =	CGameUI::getInstance()->openFlash("uiGameInit","data/flashui/uiGameInit.swf");	
}

void CStateInit::onFsCommand(const char *command, const char *param)
{
	printf("fsCommand: %s %s\n", command, param);

	if ( strcmp( command, "animationStatus") == 0 && strcmp( param, "finish") == 0 )
	{
		// todo load another state		
	}
}