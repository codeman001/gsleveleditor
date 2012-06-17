#include "stdafx.h"
#include "CStateMainMenu.h"
#include "CStateGameLoading.h"
#include "CGameStateManager.h"

const int k_btnPlayGame = 0;

CStateMainMenu::CStateMainMenu()
{
	m_menuChoice = -1;
}

CStateMainMenu::~CStateMainMenu()
{
}

void CStateMainMenu::onCreate()
{
	m_menuFx = CGameUI::getInstance()->getFlash("uiGameMenu");
	m_menuFx->setVisible( true );
}

void CStateMainMenu::onFsCommand( const char *command, const char *param )
{
	if ( strcmp("stateStatus", command) == 0 && strcmp("close",param) == 0 )
	{		
		switch ( m_menuChoice )
		{
		case k_btnPlayGame:
			CGameStateMgr::getInstance()->changeState( new CStateGameLoading() );			
			break;
		default:
			{
				printf("unknownState\n");
			}
			break;
		}
	}
	else if ( strcmp("buttonStatus",  command) == 0 )
	{
		if ( strcmp("playGame", param) == 0 )
			m_menuChoice = k_btnPlayGame;
	}
}