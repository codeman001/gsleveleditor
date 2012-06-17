#include "stdafx.h"
#include "CStateGameLoading.h"
#include "CGameStateManager.h"

CStateGameLoading::CStateGameLoading()
{
}

CStateGameLoading::~CStateGameLoading()
{
}

void CStateGameLoading::onCreate()
{
	m_menuFx = CGameUI::getInstance()->getFlash("uiGameMenu");
}