#include "stdafx.h"
#include "CUIButton.h"

CUIButton::CUIButton( CUIWidget* parent, CMenuFxObj flashObj )
	:CUIWidget(parent)
{
	m_flashObj = flashObj;
	getRectByFxObj( m_flashObj, m_rect );
}

CUIButton::~CUIButton()
{
}

// update
// update per frame
void CUIButton::update()
{
	CUIWidget::update();
	
	if ( m_visible == false && m_buttonState != ButtonInvisible )
	{
		// todo
		m_visible = ButtonInvisible;
	}
	else if ( m_visible == true && m_buttonState == ButtonInvisible )
	{
		// todo
		m_visible = ButtonNormal;
	}
}

// onTouchEvent
// update touch event
bool CUIButton::onEvent( const SEvent& gameEvent)
{
	// if childs handle this event
	if ( CUIWidget::onEvent(gameEvent) == true )
		return true;
	
	// not available
	if ( (m_controlID != -1 && gameEvent.EventControlID != 0) || 
		m_buttonState == ButtonDisable || 
		m_visible == false )
		return false;

	bool ret = false;

	// process button action
	if ( gameEvent.EventType == EET_MOUSE_INPUT_EVENT )
	{
		// check mouse hit this button
		core::position2di mousePos(gameEvent.MouseInput.X,gameEvent.MouseInput.Y);

		bool mouseOver = m_rect.isPointInside(mousePos);

		// when mouse out the button
		if ( mouseOver == false )
		{
			// mouse out
			if ( m_buttonState == ButtonFocus )
			{
				// todo ....				
			}
			m_buttonState = ButtonNormal;
		}
		else
		{
			if ( gameEvent.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
			{
				if ( m_buttonState == ButtonNormal )
				{
					// todo ....
					m_buttonState = ButtonFocus;
				}
			}
			else if ( gameEvent.MouseInput.Event == EMIE_MOUSE_MOVED )
			{
				// todo ...
			}
			else if ( gameEvent.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
			{			
				if ( m_buttonState == ButtonFocus )
				{
					// todo ....
					m_buttonState = ButtonNormal;
				}
			}

			// handle this event
			ret = true;
		}
	}

	return ret;
}