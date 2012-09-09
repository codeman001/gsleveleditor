#include "stdafx.h"
#include "IView.h"
#include "CStateGameplay.h"
#include "CGameCameraFollowAnimator.h"

#include "gameControl/CGameControl.h"

CStateGameplay::CStateGameplay()
	:CGameState(CGameState::GSStateGameplay)
{
	m_level = new CGameLevel();
}

CStateGameplay::~CStateGameplay()
{
	delete m_level;	
	CGameLevel::setCurrentLevel( NULL );
}

void CStateGameplay::onCreate()
{
	CGameLevel::setCurrentLevel( m_level );

	// show ui
	m_menuFx = CGameUI::getInstance()->getFlash("uiGameHud");
	setFxStateVisible( m_state, true );

	// hide dpad touch
	CMenuFxObj *dpadTouch = m_menuFx->findObj("mcDpadTouch");
	if ( dpadTouch )
	{
		dpadTouch->setVisible(false);
		dpadTouch->drop();
	}
    
	CMenuFxObj *dPadBase = m_menuFx->findObj("mcDpadBase");
	if ( dPadBase )
	{
		dPadBase->setVisible(false);
		dPadBase->drop();
	}	

    // enable gamecontrol
    CGameControl::getInstance()->setEnable(true);
    CGameControl::getInstance()->setNameFx("mcDpadTouch", "mcDpadBase", "mcDpadMove");
}

void CStateGameplay::onDestroy()
{
    // enable gamecontrol
    CGameControl::getInstance()->setEnable(false);
}

void CStateGameplay::onFsCommand( const char *command, const char *param )
{
}

void CStateGameplay::onUpdate()
{
	m_level->update();
}

void CStateGameplay::onRender()
{
	m_level->render();
}