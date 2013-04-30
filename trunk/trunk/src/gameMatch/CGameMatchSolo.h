#ifndef _GAMEMATCH_SOLO_
#define _GAMEMATCH_SOLO_

#include "CBaseGameMatch.h"

class CGameMatchSolo: public CBaseGameMatch
{
protected:

	CUIDPad*		m_dpad;
	CUIHoldButton*	m_btnRunFast;
	CUIHoldButton*	m_btnShoot;
		
public:
	CGameMatchSolo();
	virtual ~CGameMatchSolo();

	// initUI
	// init ui
	virtual void initUI(CMenuFxObj menufx, CUICanvas *rootWidget);

	// update
	// update match
	virtual void update();

	// onEvent
	// process event
	virtual void onEvent(const SEvent& event);
};

#endif