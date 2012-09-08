#include "stdafx.h"
#include "IView.h"
#include "CStateGameplay.h"
#include "CGameCameraFollowAnimator.h"

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
	ISceneManager *smgr = getIView()->getSceneMgr();
	CGameLevel::setCurrentLevel( m_level );

	// show ui
	m_menuFx = CGameUI::getInstance()->getFlash("uiGameHud");
	setFxStateVisible( m_state, true );

	// hide dpad touch
	CMenuFxObj *dpadTouch = m_menuFx->findObj("mcDpadTouch");
	if ( dpadTouch )
	{
		//int x = 0,y = 0,w = 0,h = 0;
		//dpadTouch->getBound(&x, &y, &w, &h);

		dpadTouch->setVisible(false);
		dpadTouch->drop();
	}
	
	// hidden touchzone
	

}

void CStateGameplay::onDestroy()
{
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