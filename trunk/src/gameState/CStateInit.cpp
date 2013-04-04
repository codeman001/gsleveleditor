#include "stdafx.h"

#include "CStateInit.h"
#include "CStateMainMenu.h"
#include "CStateGameLoading.h"
#include "CGameStateManager.h"

#include "IView.h"
#include "gameConfig/CGameConfig.h"

//#define OPEN_GAMEPLAY	"data/level/levelGameM1.lv"

CStateInit::CStateInit()
	:CGameState( CGameState::GSStateInit )
{
	m_loadMainMenu	= false;
}

CStateInit::~CStateInit()
{
}

void CStateInit::onCreate()
{		
}

void CStateInit::onDestroy()
{	
}

void CStateInit::onFsCommand(const char *command, const char *param)
{
}

void CStateInit::onRender()
{
	m_loadMainMenu = true;
}

void CStateInit::onUpdate()
{
	CGameState::onUpdate();

	if ( m_loadMainMenu )
	{
		// todo load menu state
		CMenuFx *menu =	CGameUI::getInstance()->openFlash("uiGameMenu", getIView()->getPath("data/flashui/uiGameMenu.swf"));
		
		// invisible all state
		for ( int state = (int)CGameState::GSStateMainMenu; state <= (int)CGameState::GSStateGameLoading; state++ )
		{
			// create flash mainmenu ui			
			CMenuFxObj menuObj = menu->getObj( CGameState::getStateName( (CGameState::EGameState)state ) );
			menuObj.gotoFrame( 0, false );
			menuObj.setVisible( false );			
		}

		// hide the menu
		menu->setVisible( false );	

#ifdef USE_ZIPPACKAGE
		// add zip package
		io::IFileSystem *fileSystem = getIView()->getDevice()->getFileSystem();
		fileSystem->addZipFileArchive( getIView()->getPhysicPath("_scene.zip") );
	#if defined (IOS)
		fileSystem->addZipFileArchive( getIView()->getPhysicPath("_texturePVR.zip") );
	#else
		fileSystem->addZipFileArchive( getIView()->getPhysicPath("_textureTGA.zip") );
	#endif
#endif
		// load game config
		CGameConfig::getInstance()->initConfig();

		// load all template
		CObjTemplateFactory::loadAllObjectTemplate();

#ifndef OPEN_GAMEPLAY
		// load 3d level
		CStateMainMenu *mainMenuState = new CStateMainMenu();

		CGameLevel *level = mainMenuState->getLevel();
		level->loadLevel( getIView()->getPath( "data/level/levelMainMenu.lv" ) );			
		while ( level->loadStep() == false )
		{
			// todo loading level
		}
#endif
	
#ifdef OPEN_GAMEPLAY
		m_menuFx = CGameUI::getInstance()->getFlash("uiGameMenu");	
		m_menuFx->setVisible( true );
		m_menuFx->setBackgroundTransparent( true );

		CGameLevel::setLevelLoad( OPEN_GAMEPLAY );
		CGameStateMgr::getInstance()->changeState( new CStateGameLoading() );
#else
		CGameStateMgr::getInstance()->changeState( mainMenuState );
#endif
	}

}