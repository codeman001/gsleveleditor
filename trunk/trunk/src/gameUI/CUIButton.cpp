#include "stdafx.h"
#include "CUIButton.h"
#include "gameEvent.h"
#include "IView.h"

CUIButton::CUIButton( const char *name, CUIWidget* parent, CMenuFxObj flashObj )
	:CUIWidget(name, parent)
{
	m_flashObj = flashObj;
    
    m_buttonState       = ButtonNormal;
    m_buttonLastState   = m_buttonState;
    m_mouseOver         = false;
}

CUIButton::~CUIButton()
{
}

// update
// update per frame
void CUIButton::update()
{
	CUIWidget::update();

    // update rect
	getRectByFxObj( m_flashObj, m_rect );
    
    // update text for button
    if ( m_buttonLastState != m_buttonState )
    {
        replaceTextOnLabel();
        m_buttonLastState = m_buttonState;
    }
    
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
    CUIWidget *root = getRootWidget();
    
	// process button action
	if ( gameEvent.EventType == EET_MOUSE_INPUT_EVENT && root->isLockActionOnChild() == false )
	{
		// check mouse hit this button
		core::position2di mousePos(gameEvent.MouseInput.X,gameEvent.MouseInput.Y);

        // check mouse over on button
		m_mouseOver = m_rect.isPointInside(mousePos);
        
		// when mouse out the button
		if ( m_mouseOver == false )
		{
			// mouse out
			if ( m_buttonState == ButtonFocus || m_buttonState == ButtonRelease )
                m_flashObj.gotoFrame("normal", true);

			m_buttonState = ButtonNormal;
		}
		else
		{
			if (gameEvent.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN || gameEvent.MouseInput.Event == EMIE_MOUSE_MOVED)
			{
				if ( m_buttonState == ButtonNormal )
				{
                    m_flashObj.gotoFrame("focus", true);
					m_buttonState = ButtonFocus;
				}
			}
			else if ( gameEvent.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
			{			
				if ( m_buttonState == ButtonFocus )
				{
                    // lock
                    root->lockActionOnChilld( true );
                    
                    // play anim
                    m_flashObj.gotoFrame("release", true);
					m_buttonState = ButtonRelease;
				}
			}

			// handle this event
			ret = true;
		}
	}
    else if ( gameEvent.EventType == EET_FSCOMMAND_EVENT )
    {
        const char *command = gameEvent.FSEvent.Command;
        const char *param   = gameEvent.FSEvent.Param;
        
        if ( strcmp("buttonStatus",  command) == 0 )
        {
            if ( strcmp("release", param) == 0 && m_buttonState == ButtonRelease )
            {                
                // unlock
                root->lockActionOnChilld( false );
                
                // hack for update text
                m_buttonLastState = ButtonRelease;
                if ( m_mouseOver )
                    m_buttonState = ButtonFocus;
                else
                    m_buttonState = ButtonNormal;
                
                // post press button event
                SEvent event;
                SEventButtonRelease button;
                
                event.EventType = EET_GAME_EVENT;
                event.GameEvent.EventID = (s32)EvtButtonRelease;
                
                button.buttonName = m_widgetName;
                button.data = this;
                event.GameEvent.EventData = &button;
                
                getIView()->getDevice()->postEventFromUser( event );
            }

			// update text
			replaceTextOnLabel();
        }       
    }

	return ret;
}