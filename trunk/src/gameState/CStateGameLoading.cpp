#include "stdafx.h"
#include "CStateGameLoading.h"
#include "CGameStateManager.h"

CStateGameLoading::CStateGameLoading()
	:CGameState( CGameState::GSStateGameLoading )
{
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
	setFxStateVisible( m_state, false );
}