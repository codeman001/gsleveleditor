#ifndef _STATE_JOINGAME_
#define _STATE_JOINGAME_

#include "CGameState.h"

class CStateJoinGame: public CGameState, public IEventReceiver
{
protected:
	std::string		m_serverIP;

public:
	CStateJoinGame();
	virtual ~CStateJoinGame();

	virtual void onCreate();
	virtual void onDestroy();
	virtual void onFsCommand( const char *command, const char *param );	
	
	virtual void onRender();
	virtual void onUpdate();

    virtual bool OnEvent(const SEvent& event);
};

#endif