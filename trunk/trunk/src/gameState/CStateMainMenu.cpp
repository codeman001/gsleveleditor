#include "stdafx.h"
#include "CStateMainMenu.h"

CStateMainMenu::CStateMainMenu()
{
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
}