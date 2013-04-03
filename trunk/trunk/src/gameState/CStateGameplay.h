#ifndef STATE_GAMEPLAY_
#define STATE_GAMEPLAY_

#include "stdafx.h"
#include "CGameState.h"
#include "gameMatch/IGameMatch.h"

class CStateGameplay: public CGameState
{
protected:	
	CGameLevel		*m_level;	

	IGameMatch::EGameMatchType	m_gameMatchType;
	IGameMatch*					m_gameMatch;

public:
	CStateGameplay(IGameMatch::EGameMatchType gameMatch);
	virtual ~CStateGameplay();

	virtual void onCreate();	
	virtual void onDestroy();
	virtual void onFsCommand( const char *command, const char *param );
	virtual void onUpdate();
	virtual void onRender();

    virtual void onEvent(const SEvent& event);    
    
	void checkDisconectedFromServer(int hostkeyID);

	// getLevel
	// get 3d level
	CGameLevel* getLevel()
	{
		return m_level;
	}
};


#endif