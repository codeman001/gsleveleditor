#include "stdafx.h"

#include "CStateInit.h"
#include "CStateMainMenu.h"
#include "CGameStateManager.h"

CStateInit::CStateInit()
{
	m_loadFinish = false;
	m_logoTime = 3.0f;
}

CStateInit::~CStateInit()
{
}

void CStateInit::onCreate()
{	
	// init flash ui
	m_menuFx =	CGameUI::getInstance()->openFlash("uiGameInit","data/flashui/uiGameInit.swf");
}

void CStateInit::onFsCommand(const char *command, const char *param)
{
	printf("fsCommand: %s %s\n", command, param);

	if ( strcmp( command, "animationStatus") == 0 && strcmp( param, "finish") == 0 )
	{
		// todo load menu state
		CMenuFx *mainMenu =	CGameUI::getInstance()->openFlash("uiGameMenu","data/flashui/uiGameMenu.swf");
		mainMenu->setVisible( false );

		// notify load finish
		m_loadFinish = true;
	}
	else if ( strcmp( command, "stateStatus" ) == 0 && strcmp( param, "close" ) == 0 )
	{
		// change state main menu
		m_menuFx->setVisible( false );		
		CGameStateMgr::getInstance()->changeState( new CStateMainMenu() );
	}
}

void CStateInit::onUpdate(float timeStep)
{
	if ( m_loadFinish == true )
	{
		m_logoTime = m_logoTime - timeStep;

		// wait logo time %d second
		if ( m_logoTime < 0 )
		{
			m_logoTime = 0.0f;
			m_loadFinish = false;

			// play state hide
			CMenuFxObj *obj = m_menuFx->findObj("gsGameInit");
			if ( obj )
				obj->gotoFrame("hide", true);
		}
	}
}