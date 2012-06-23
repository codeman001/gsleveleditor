#include "stdafx.h"

#include "CStateInit.h"
#include "CStateMainMenu.h"
#include "CGameStateManager.h"

#include "IView.h"

CStateInit::CStateInit()
	:CGameState( CGameState::GSStateInit )
{
	m_loadFinish = false;
	m_logoTime = 2000.f;
}

CStateInit::~CStateInit()
{
}

void CStateInit::onCreate()
{	
	// init flash ui
	m_menuFx =	CGameUI::getInstance()->openFlash("uiGameInit", getIView()->getPath("data/flashui/uiGameInit.swf") );
}

void CStateInit::onDestroy()
{	
}

void CStateInit::onFsCommand(const char *command, const char *param)
{
	if ( strcmp( command, "animationStatus") == 0 && strcmp( param, "finish") == 0 )
	{
		// todo load menu state
		CMenuFx *menu =	CGameUI::getInstance()->openFlash("uiGameMenu", getIView()->getPath("data/flashui/uiGameMenu.swf"));
		
		// invisible all state
		for ( int state = (int)CGameState::GSStateMainMenu; state < (int)CGameState::StateCount; state++ )
		{
			// create flash mainmenu ui			
			CMenuFxObj *menuObj = menu->getObj( CGameState::getStateName( (CGameState::EGameState)state ) );
			if ( menuObj )
			{
				menuObj->setVisible( false );
				menuObj->drop();
			}			
		}

		// hide the menu
		menu->setVisible( false );
		
		// create main menu level
		CStateMainMenu* mainMenuState = new CStateMainMenu();

		// load all template
		CObjTemplateFactory::loadAllObjectTemplate();

		// load 3d level
		CGameLevel *level = mainMenuState->getLevel();
		level->loadLevel( getIView()->getPath( "data/level/levelMainMenu.lv" ) );			
		while ( level->loadStep() == false )
		{
		}

		// set gamestate
		m_mainMenuState = mainMenuState;

		// notify load finish
		m_loadFinish = true;
	}
	else if ( strcmp( command, "stateStatus" ) == 0 && strcmp( param, "close" ) == 0 )
	{
		// change state main menu
		m_menuFx->setVisible( false );
		CGameStateMgr::getInstance()->changeState( m_mainMenuState );
	}
}

void CStateInit::onUpdate()
{
	if ( m_loadFinish == true )
	{
		m_logoTime = m_logoTime - getIView()->getTimeStep();

		// wait logo time %d second
		if ( m_logoTime < 0 )
		{
			m_logoTime = 0.0f;
			m_loadFinish = false;

			// play state hide
			CMenuFxObj *obj = m_menuFx->findObj("gsGameInit");
			if ( obj )
			{
				obj->gotoFrame("hide", true);
				obj->drop();
			}
		}
	}
}