#ifndef _STATE_GAMELOADING_
#define _STATE_GAMELOADING_

#include "CGameState.h"

class CStateGameLoading: public CGameState
{
protected:
	CMenuFxObj		*m_loadingBar;
	CGameState		*m_nextState;
	CGameLevel		*m_levelLoad;

	bool			m_beginLoading;
public:
	CStateGameLoading();
	virtual ~CStateGameLoading();

	virtual void onCreate();	
	virtual void onDestroy();
	virtual void onFsCommand( const char *command, const char *param );
	virtual void onUpdate();
};

#endif