#ifndef STATE_GAMEPLAY_
#define STATE_GAMEPLAY_


#include "CGameState.h"

class CStateGameplay: public CGameState
{
protected:
	
	CGameLevel		*m_level;
public:
	CStateGameplay();
	virtual ~CStateGameplay();

	virtual void onCreate();	
	virtual void onDestroy();
	virtual void onFsCommand( const char *command, const char *param );
	virtual void onUpdate();
	virtual void onRender();

	// getLevel
	// get 3d level
	CGameLevel* getLevel()
	{
		return m_level;
	}
};


#endif