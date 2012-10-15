#include "stdafx.h"
#include "CStateJoinGame.h"
#include "IView.h"

CStateJoinGame::CStateJoinGame()
	:CGameState( CGameState::GSStateJoinGame )
{
}

CStateJoinGame::~CStateJoinGame()
{
}

void CStateJoinGame::onCreate()
{
	// show mainmenu state
	showFxThisState();

    // register event for catch networking message
    getIView()->registerEvent("stateJoinGame", this);
}

void CStateJoinGame::onDestroy()
{
	getIView()->unRegisterEvent(this);
}

void CStateJoinGame::onFsCommand( const char *command, const char *param )
{
}
	
void CStateJoinGame::onRender()
{
}

void CStateJoinGame::onUpdate()
{
}

bool CStateJoinGame::OnEvent(const SEvent& event)
{
	return true;
}