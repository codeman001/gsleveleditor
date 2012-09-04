#include "stdafx.h"

#include "CStateInit.h"
#include "CStateMainMenu.h"
#include "CStateGameLoading.h"
#include "CGameStateManager.h"

#include "IView.h"

//#define OPEN_GAMEPLAY	"data/level/levelGameM1.lv"

CStateInit::CStateInit()
	:CGameState( CGameState::GSStateInit )
{
	m_loadFinish = false;

#ifdef OPEN_GAMEPLAY
	m_logoTime = 0.f;
#else
	m_logoTime = 2000.f;
#endif

	m_mainMenuState = NULL;
}

CStateInit::~CStateInit()
{
	if ( m_mainMenuState )
		delete m_mainMenuState;
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

#ifdef USE_ZIPPACKAGE
		// add zip package
		io::IFileSystem *fileSystem = getIView()->getDevice()->getFileSystem();
		fileSystem->addZipFileArchive( getIView()->getPhysicPath("_scene.zip") );
		fileSystem->addZipFileArchive( getIView()->getPhysicPath("_texture.zip") );
#endif

		// load all template
		CObjTemplateFactory::loadAllObjectTemplate();

#ifndef OPEN_GAMEPLAY
		// load 3d level
		CStateMainMenu *mainMenuState = new CStateMainMenu();

		CGameLevel *level = mainMenuState->getLevel();
		level->loadLevel( getIView()->getPath( "data/level/levelMainMenu.lv" ) );			
		while ( level->loadStep() == false )
		{
		}

		// set gamestate
		// create main menu level		
		m_mainMenuState = mainMenuState;
#endif

		// notify load finish
		m_loadFinish = true;
	}
	else if ( strcmp( command, "stateStatus" ) == 0 && strcmp( param, "close" ) == 0 )
	{
		// change state main menu
		m_menuFx->setVisible( false );

#ifdef OPEN_GAMEPLAY
		m_menuFx = CGameUI::getInstance()->getFlash("uiGameMenu");	
		m_menuFx->setVisible( true );
		m_menuFx->setBackgroundTransparent( true );

		CGameLevel::setLevelLoad( OPEN_GAMEPLAY );
		CGameStateMgr::getInstance()->changeState( new CStateGameLoading() );
#else
		CGameStateMgr::getInstance()->changeState( m_mainMenuState );
		m_mainMenuState = NULL;
#endif
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