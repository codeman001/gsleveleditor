#ifndef _STATE_GAMELOADING_
#define _STATE_GAMELOADING_

#include "CGameState.h"
#include "gameMatch/IGameMatch.h"

class CStateGameLoading: public CGameState
{
protected:	
	CGameState		*m_nextState;
	CGameLevel		*m_levelLoad;

	CMenuFxObj		m_loadingBar;

	bool			m_beginLoading;
public:
	CStateGameLoading(IGameMatch::EGameMatchType gameMatch);
	virtual ~CStateGameLoading();

	virtual void onCreate();	
	virtual void onDestroy();
	virtual void onFsCommand( const char *command, const char *param );
	virtual void onUpdate();
};

#endif