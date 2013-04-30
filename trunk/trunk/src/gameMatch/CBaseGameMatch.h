#ifndef _BASEGAMEMATCH_H_
#define _BASEGAMEMATCH_H_

#include "IGameMatch.h"
#include "gameControl/CGameControl.h"

class CBaseGameMatch: public IGameMatch
{
protected:
	CUIButton*		m_btnPause;

	int				m_frameCount;
	bool			m_fixCamera;
	core::vector3df	m_staticTarget;

	CGameObject*	m_player;

public:
	CBaseGameMatch(IGameMatch::EGameMatchType type);
	virtual ~CBaseGameMatch();
	
	// initUI
	// init ui
	virtual void initUI(CMenuFxObj menufx, CUICanvas* rootWidget);

	// update
	// update match
	virtual void update();

	// onEvent
	// process event
	virtual void onEvent(const SEvent& event);

	// spawnPlayer
	// create a player
	CGameObject* spawn3rdPlayer();

	// updateCameraTarget	
	void updateCameraTarget();
};

#endif