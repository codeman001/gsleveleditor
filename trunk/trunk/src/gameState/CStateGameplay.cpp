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