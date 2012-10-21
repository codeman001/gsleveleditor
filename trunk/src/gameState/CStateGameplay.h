#ifndef STATE_GAMEPLAY_
#define STATE_GAMEPLAY_

#include "stdafx.h"
#include "CGameState.h"

class CStateGameplay: public CGameState
{
protected:	
	CGameLevel		*m_level;
    
#ifdef HAS_MULTIPLAYER
    float           m_syncGameInterval;
#endif
    
public:
	CStateGameplay();
	virtual ~CStateGameplay();

	virtual void onCreate();	
	virtual void onDestroy();
	virtual void onFsCommand( const char *command, const char *param );
	virtual void onUpdate();
	virtual void onRender();

    virtual void onEvent(const SEvent& event);    
    
	// getLevel
	// get 3d level
	CGameLevel* getLevel()
	{
		return m_level;
	}
};


#endif