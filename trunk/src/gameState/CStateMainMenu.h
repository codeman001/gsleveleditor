#ifndef _STATE_MAINMENU_
#define _STATE_MAINMENU_

#include "CGameState.h"

class CStateMainMenu: public CGameState
{
public:
	CStateMainMenu();
	virtual ~CStateMainMenu();

	virtual void onCreate();
	virtual void onFsCommand( const char *command, const char *param );
};

#endif