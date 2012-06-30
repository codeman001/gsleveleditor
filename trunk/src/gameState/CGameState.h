#ifndef _GAMESTATE_
#define _GAMESTATE_

#include "stdafx.h"
#include "gameLevel/CGameLevel.h"

#include "swfUI/CGameUI.h"
#include "swfUI/CMenuFx.h"
#include "swfUI/CMenuFxObj.h"

class CGameState
{
public:
	enum EGameState
	{
		GSStateInit = 0,
		GSStateMainMenu,
		GSStateGameLoading,
		GSStateGameplay,
		StateCount
	};

protected:
	CMenuFx		*m_menuFx;
	EGameState	m_state;

public:
	CGameState(EGameState state);
	virtual ~CGameState();

	// onCreate
	// on window create
	virtual void onCreate();

	// onDestroy
	// on window destroy
	virtual void onDestroy();

	// onActive	
	virtual void onActive();

	// onDeactive
	virtual void onDeactive();

	// updateWindow
	// Update UI
	virtual void onUpdate();

	// paintWindow
	// paint interface to screens
	virtual void onRender();

	// onFsCommand
	// flash notification
	virtual void onFsCommand( const char *command, const char *param );

	// onResume
	// on resume
	virtual void onResume();

	// onPause
	// on pause
	virtual void onPause();

	// setFxStateVisible
	// show/hide state on flash
	virtual void setFxStateVisible( EGameState state, bool b );

	// getStateName
	// get state name
	static const char* getStateName( EGameState state );
};

#endif