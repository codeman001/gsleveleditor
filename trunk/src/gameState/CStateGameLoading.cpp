#include "stdafx.h"
#include "CStateGameLoading.h"
#include "CStateGameplay.h"
#include "CGameStateManager.h"
#include "IView.h"

#include "CParticleComponent.h"
#include "CColladaMeshComponent.h"

#include "core/TextureManager/CTextureManager.h"

CStateGameLoading::CStateGameLoading(IGameMatch::EGameMatchType gameMatch)
	:CGameState( CGameState::GSStateGameLoading )
{	
	m_beginLoading = false;

	CStateGameplay *gameplay = new CStateGameplay(gameMatch);
	
	m_levelLoad = gameplay->getLevel();
	m_levelLoad->loadLevel( CGameLevel::getLevelProperty("levelLoad") );

    // release all main menu texture
	CColladaCache::freeData();
	CParticleCache::freeData();

    CTextureManager::getInstance()->removeAllTexture();
    
	m_nextState = gameplay;
}

CStateGameLoading::~CStateGameLoading()
{
	if ( m_nextState )
		delete m_nextState;
}

void CStateGameLoading::onCreate()
{
	m_menuFx = CGameUI::getInstance()->getFlash("uiGameMenu");	

	setFxStateVisible( m_state, true );
}

void CStateGameLoading::onDestroy()
{
	setFxStateVisible( m_state, false );
}

void CStateGameLoading::onFsCommand( const char *command, const char *param )
{
	if ( strcmp(command,"stateStatus") == 0 )
	{
		if ( strcmp(param,"state") == 0 )
		{
			m_loadingBar = m_menuFx->findObj("btnGameLoadingBar");	
			m_beginLoading = true;
		}
		else if ( strcmp(param,"close") == 0 )
		{			
			CGameStateMgr::getInstance()->changeState( m_nextState );
			m_nextState = NULL;
		}
	}
}

void CStateGameLoading::onUpdate()
{
	CGameState::onUpdate();

	if ( m_beginLoading )
	{
		bool loadFinish = m_levelLoad->loadStep();

		// show percent
		m_loadingBar.gotoFrame( m_levelLoad->getLoadingPercent(), false );

		if ( loadFinish )
		{			
			// load gameHud flash
			CMenuFx *menu = CGameUI::getInstance()->openFlash("uiGameHud", getIView()->getPath("data/flashui/uiGameHud.swf"));
			menu->setBackgroundTransparent(true);

			// invisible all state
			for ( int state = (int)CGameState::GSStateGameplay; state < (int)CGameState::StateCount; state++ )
			{
				// create flash mainmenu ui			
				CMenuFxObj menuObj = menu->getObj( CGameState::getStateName( (CGameState::EGameState)state ) );			
				menuObj.setVisible( false );									
			}

			// play animation
			CMenuFxObj menuObj = m_menuFx->getObj( getStateName(m_state) );				
			menuObj.gotoFrame("hide", true);			

			m_beginLoading = false;			
		}

	}
}