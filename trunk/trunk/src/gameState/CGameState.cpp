#include "stdafx.h"
#include "CGameState.h"

CGameState::CGameState( EGameState state )
{
	m_menuFx = NULL;
	m_state = state;

	// get game menu flash
	m_menuFx = CGameUI::getInstance()->getFlash("uiGameMenu");
}

CGameState::~CGameState()
{
}

// getStateName
// get state name
const char* CGameState::getStateName( EGameState state )
{
	static const char* s_stateName[] = {
		"stateInit",
		"stateMainMenu",
		"stateGameLoading",
		"stateGameplay",
		"stateCount"
	};

	return s_stateName[ (int)state ];	
}

// onCreate
// on window create
void CGameState::onCreate()
{
}

// onDestroy
// on window destroy
void CGameState::onDestroy()
{
}

// onActive	
void CGameState::onActive()
{
}

// onDeactive
void CGameState::onDeactive()
{
}


// onResume
// on resume
void CGameState::onResume()
{
}

// onPause
// on pause
void CGameState::onPause()
{
}

// onFsCommand
// flash notification
void CGameState::onFsCommand( const char *command, const char *param )
{
}

// updateWindow
// Update UI
void CGameState::onUpdate()
{
}

// paintWindow
// paint interface to screens
void CGameState::onRender()
{
}

// setFxStateVisible
// show/hide state on flash
void CGameState::setFxStateVisible( EGameState state, bool b )
{
	if ( m_menuFx )
	{
		CMenuFxObj *menuObj = m_menuFx->getObj( getStateName(state) );
		if ( menuObj )
		{
			menuObj->setVisible( b );
			menuObj->gotoFrame(1, true);
			menuObj->drop();
		}
	}
}