#ifndef _GAMESTATE_MGR_
#define _GAMESTATE_MGR_

#include "stdafx.h"
#include "CGameState.h"

class CGameStateMgr: 
	public uiSingleton<CGameStateMgr>,
	public IEventReceiver
{
protected:
	vector<CGameState*>	m_stackState;
	vector<CGameState*>	m_willDeleteState;
public:
	CGameStateMgr();
	virtual ~CGameStateMgr();

	// update loop
	void update();

	// render loop
	void render();

	// on resume from interupt
	void onResume();

	// on pause from interupt
	void onPause();

	// push a state
	void pushState( CGameState* state );

	// change current state
	void changeState( CGameState* state );

	// pop a state
	void popState();	

	// get a current game state
	CGameState* getCurrentState();

	// get a state before
	CGameState* getStateBefore(CGameState* state);

	// destroyAllState
	void destroyAllState();

	// OnEvent
	virtual bool OnEvent(const SEvent& irrEvent);

};

#endif