#include "stdafx.h"
#include "CUIHoldButton.h"

CUIHoldButton::CUIHoldButton( const char *name, CUIWidget* parent, CMenuFxObj flashObj )
	:CUIWidget(name, parent, flashObj)
{
}

CUIHoldButton::~CUIHoldButton()
{
}

// update
// update per frame
void CUIHoldButton::update()
{
	CUIWidget::update();
}

// setVisible
// set visible a button
void CUIHoldButton::setVisible( bool b )
{
	m_visible = b;
	m_flashObj.setVisible(b);
}

// onTouchEvent
// update touch event
bool CUIHoldButton::onEvent( const SEvent& gameEvent)
{
	// if childs handle this event
    if ( CUIWidget::onEvent(gameEvent) == true )
        return true;
	
	// not available
	if ( (m_controlID != -1 && gameEvent.EventControlID != 0) || m_visible == false )
		return false;

	bool ret = false;

	return ret;
}