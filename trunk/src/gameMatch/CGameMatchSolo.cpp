#include "stdafx.h"
#include "CGameMatchSolo.h"

CGameMatchSolo::CGameMatchSolo()
	:CBaseGameMatch(IGameMatch::SoloMode)
{	
}

CGameMatchSolo::~CGameMatchSolo()
{
}

// initUI
// init ui
void CGameMatchSolo::initUI(CMenuFxObj menufx, CUICanvas *rootWidget)
{
	CBaseGameMatch::initUI(menufx, rootWidget);

	// create dpad control
	m_dpad			= new CUIDPad("dpad", m_rootWidget, m_menufx.findObj("mcDPadTouch"));
	m_btnRunFast	= new CUIHoldButton("runfast", m_rootWidget, m_menufx.findObj("mcBtnRunFast"));
	m_btnShoot		= new CUIHoldButton("shoot", m_rootWidget, m_menufx.findObj("mcBtnShoot"));
	
	m_btnRunFast->setVisible(false);
	m_btnShoot->setVisible(false);

#if defined (IOS) || defined (ANDROID)
    m_dpad->setEnable(true);
	m_btnRunFast->setVisible(true);
	m_btnShoot->setVisible(true);
#endif

}

// update
// update match
void CGameMatchSolo::update()
{
	if ( m_player == NULL )
		m_player = spawn3rdPlayer();
	else
	{
		CBaseGameMatch::update();
	}
}

// onEvent
// process event
void CGameMatchSolo::onEvent(const SEvent& event)
{
	CBaseGameMatch::onEvent(event);

	if ( event.GameEvent.EventID == (s32)EvtButtonPress )
	{
		SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);

		if ( buttonEvent->data == m_btnRunFast )
		{
		}
		else if ( buttonEvent->data == m_btnShoot )
            CGameControl::getInstance()->setShootMode(true, false);
	}
	else if ( event.GameEvent.EventID == (s32)EvtButtonRelease )
	{
		SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);

		if ( buttonEvent->data == m_btnRunFast )
		{            
		}
		else if ( buttonEvent->data == m_btnShoot )
            CGameControl::getInstance()->setShootMode(false, false);			
	}

}