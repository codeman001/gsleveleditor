#ifndef _BASEGAMEMATCH_H_
#define _BASEGAMEMATCH_H_

#include "IGameMatch.h"
#include "gameControl/CGameControl.h"

class CBaseGameMatch: public IGameMatch
{
protected:
	CUIButton*		m_btnPause;

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

};

#endif