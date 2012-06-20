#ifndef _STATE_GAMELOADING_
#define _STATE_GAMELOADING_

#include "CGameState.h"

class CStateGameLoading: public CGameState
{
protected:	
public:
	CStateGameLoading();
	virtual ~CStateGameLoading();

	virtual void onCreate();
	virtual void onDestroy();
};

#endif