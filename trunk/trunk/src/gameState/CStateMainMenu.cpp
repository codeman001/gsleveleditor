#include "stdafx.h"
#include "CStateMainMenu.h"
#include "CStateGameLoading.h"
#include "CGameStateManager.h"

const int k_btnPlayGame = 0;

CStateMainMenu::CStateMainMenu()
	:CGameState( CGameState::GSStateMainMenu )
{
	m_menuChoice = -1;
	m_level = new CGameLevel();	
}

CStateMainMenu::~CStateMainMenu()
{
	delete m_level;
}

void CStateMainMenu::onCreate()
{		
	m_menuFx->setVisible( true );	
	m_menuFx->setBackgroundTransparent(true);

	// show mainmenu state
	setFxStateVisible( m_state, true );

	CGameCamera* cam = (CGameCamera*)m_level->searchObject(3);
	if ( cam )
		m_level->setCamera( cam );
}

void CStateMainMenu::onDestroy()
{
	setFxStateVisible( m_state, false );
}

void CStateMainMenu::onUpdate()
{
	m_level->update();
}

void CStateMainMenu::onRender()
{
	m_level->render();
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