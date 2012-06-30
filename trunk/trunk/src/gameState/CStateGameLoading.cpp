#include "stdafx.h"
#include "CStateGameLoading.h"
#include "CStateGameplay.h"
#include "CGameStateManager.h"

CStateGameLoading::CStateGameLoading()
	:CGameState( CGameState::GSStateGameLoading )
{
	m_loadingBar = NULL;
	m_beginLoading = false;

	CStateGameplay *gameplay = new CStateGameplay();
	
	m_levelLoad = gameplay->getLevel();		
	m_levelLoad->loadLevel( CGameLevel::getLevelFileToLoad() );

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
	if ( m_loadingBar )
		m_loadingBar->drop();

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
	if ( m_loadingBar && m_beginLoading )
	{
		bool loadFinish = m_levelLoad->loadStep();

		// show percent
		m_loadingBar->gotoFrame( m_levelLoad->getLoadingPercent(), false );

		if ( loadFinish )
		{			
			CMenuFxObj *menuObj = m_menuFx->getObj( getStateName(m_state) );
			if ( menuObj )
			{				
				menuObj->gotoFrame("hide", true);
				menuObj->drop();
			}
			m_beginLoading = false;			
		}

	}
}