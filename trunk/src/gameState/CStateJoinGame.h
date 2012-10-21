#ifndef _STATE_JOINGAME_
#define _STATE_JOINGAME_

#include "CGameState.h"

class CStateJoinGame: public CGameState
{
protected:
	std::string		m_serverIP;

    CUIButton       *m_btnMainMenu;
    CUIButton       *m_btnJoinGame;
public:
	CStateJoinGame();
	virtual ~CStateJoinGame();

	virtual void onCreate();
	virtual void onDestroy();
	virtual void onFsCommand( const char *command, const char *param );	
	
	virtual void onRender();
	virtual void onUpdate();

    virtual void onEvent(const SEvent& event);
};

#endif