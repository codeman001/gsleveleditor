#include "stdafx.h"
#include "gameEvent.h"
#include "IView.h"
#include "CUIHoldButton.h"

CUIHoldButton::CUIHoldButton( const char *name, CUIWidget* parent, CMenuFxObj flashObj )
	:CUIWidget(name, parent, flashObj)
{
	m_holdState = false;
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
	if ( (m_controlID != -1 && m_controlID != gameEvent.EventControlID) || m_visible == false )
		return false;

	bool ret = false;

	CUIWidget *root = getRootWidget();
    
	// process button action
	if ( gameEvent.EventType == EET_MOUSE_INPUT_EVENT  )
	{
		// check mouse hit this button
		core::position2di mousePos(gameEvent.MouseInput.X,gameEvent.MouseInput.Y);

        // check mouse over on button
		bool mouseOver = m_rect.isPointInside(mousePos);
        
		// when mouse out the button
		if ( mouseOver == false )
		{
			if ( gameEvent.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
			{			
				if ( m_holdState == true )
				{                    
                    // play anim
                    m_flashObj.gotoFrame("release", true);
					m_holdState = false;


					// post release button event
					SEvent event;
					SEventButtonData button;
	                
					event.EventType = EET_GAME_EVENT;
					event.GameEvent.EventID = (s32)EvtButtonRelease;
	                
					button.buttonName = m_widgetName;
					button.data = this;
					event.GameEvent.EventData = &button;
	                
					getIView()->getDevice()->postEventFromUser( event );

					m_controlID = -1;
				}
			}
		}
		else
		{
			if (gameEvent.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
			{
				if ( m_holdState == false )
				{
                    m_flashObj.gotoFrame("focus", true);
					m_holdState = true;

					m_controlID = gameEvent.EventControlID;


					// post press button event				
					SEvent event;
					SEventButtonData button;
	                
					event.EventType = EET_GAME_EVENT;
					event.GameEvent.EventID = (s32)EvtButtonPress;
	                
					button.buttonName = m_widgetName;
					button.data = this;
					event.GameEvent.EventData = &button;
	                
					getIView()->getDevice()->postEventFromUser( event );
				}				
			}
			else if ( gameEvent.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
			{			
				if ( m_holdState == true )
				{                    
                    // play anim
                    m_flashObj.gotoFrame("release", true);
					m_holdState = false;


					// post release button event
					SEvent event;
					SEventButtonData button;
	                
					event.EventType = EET_GAME_EVENT;
					event.GameEvent.EventID = (s32)EvtButtonRelease;
	                
					button.buttonName = m_widgetName;
					button.data = this;
					event.GameEvent.EventData = &button;
	                
					getIView()->getDevice()->postEventFromUser( event );

					m_controlID = -1;
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
			// update text
			replaceTextOnLabel();
        }       
    }


	return ret;
}