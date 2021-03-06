#ifndef _STATE_JOINGAME_
#define _STATE_JOINGAME_

#include "CGameState.h"

class CStateJoinGame: public CGameState
{
protected:
	CUIListview		*m_lstServerList;
    CUIButton       *m_btnMainMenu;
    CUIButton       *m_btnJoinGame;

	bool			m_joinGame;
	std::string		m_server;
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