#include "stdafx.h"
#include "CStateGameLoading.h"
#include "CGameStateManager.h"

CStateGameLoading::CStateGameLoading()
	:CGameState( CGameState::GSStateGameLoading )
{
	m_loadingBar = NULL;
	m_beginLoading = false;
}

CStateGameLoading::~CStateGameLoading()
{
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
	if ( strcmp(command,"stateStatus") == 0 && strcmp(param,"state") == 0 )
	{
		m_loadingBar = m_menuFx->findObj("btnGameLoadingBar");	
		m_beginLoading = true;
	}
}

void CStateGameLoading::onUpdate(float timeStep)
{
	if ( m_loadingBar && m_beginLoading )
	{
		// show percent
		// m_loadingBar->gotoFrame( 20, false );
	}
}