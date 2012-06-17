#ifndef _GAMESTATE_
#define _GAMESTATE_

#include "stdafx.h"

#include "swfUI/CGameUI.h"
#include "swfUI/CMenuFx.h"
#include "swfUI/CMenuFxObj.h"

class CGameState
{
protected:
	CMenuFx	*m_menuFx;
public:
	CGameState();
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
	virtual void onUpdate(float timeStep);

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
};

#endif