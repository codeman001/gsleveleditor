#ifndef _GAMESTATE_
#define _GAMESTATE_

#include "stdafx.h"
#include "gameLevel/CGameLevel.h"

#ifdef HAS_MULTIPLAYER
#include "CMultiplayerManager.h"
#endif

#include "swfUI/CGameUI.h"
#include "swfUI/CMenuFx.h"
#include "swfUI/CMenuFxObj.h"

#include "gameUI/CUIWidget.h"

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
	CUIWidget	*m_rootWidget;
	EGameState	m_state;

#ifdef HAS_MULTIPLAYER
	CMultiplayerManager	*m_mpMgr;
#endif

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
	virtual void setFxAllStateVisible( EGameState ignoreState, bool b );

	// getStateName
	// get state name
	static const char* getStateName( EGameState state );

	// getRootWidget
	// return the root control
	inline CUIWidget* getRootWidget()
	{
		return m_rootWidget;
	}

	// getMenuFx
	// return menu fx
	inline CMenuFx* getMenuFx()
	{
		return m_menuFx;
	}
};

#endif