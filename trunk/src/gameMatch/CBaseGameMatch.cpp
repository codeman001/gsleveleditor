#include "stdafx.h"
#include "CBaseGameMatch.h"

#include "gameState/CGameStateManager.h"
#include "gameState/CStateIGM.h"


CBaseGameMatch::CBaseGameMatch(IGameMatch::EGameMatchType type)
	:IGameMatch(type)
{
}

CBaseGameMatch::~CBaseGameMatch()
{
}

// initUI
// init ui
void CBaseGameMatch::initUI(CMenuFxObj menufx, CUICanvas* rootWidget)
{
	IGameMatch::initUI(menufx, rootWidget);

	// create button
	m_btnPause = new CUIButton("pause", m_rootWidget, m_menufx.findObj("mcBtnPause"));
}


// update
// update match
void CBaseGameMatch::update()
{
}

// onEvent
// process event
void CBaseGameMatch::onEvent(const SEvent& event)
{
	if ( event.GameEvent.EventID == (s32)EvtButtonPress )
	{
		SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);			
	}
	else if ( event.GameEvent.EventID == (s32)EvtButtonRelease )
	{
		SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);
		if ( buttonEvent->data == m_btnPause )
			CGameStateMgr::getInstance()->pushState( new CStateIGM() );
	}
}