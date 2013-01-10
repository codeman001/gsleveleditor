#ifndef _UIBUTTON_H_
#define _UIBUTTON_H_

#include "stdafx.h"
#include "CUIWidget.h"

class CUIButton: public CUIWidget
{
public:
	enum ButtonState
	{
		ButtonNormal = 0,
		ButtonInvisible,
		ButtonDisable,
		ButtonFocus,
        ButtonRelease
	};
    
protected:
	ButtonState m_buttonState;
	ButtonState m_buttonLastState;
    bool        m_mouseOver;
    
public:

	CUIButton( const char *name, CUIWidget* parent, CMenuFxObj flashObj );
	virtual ~CUIButton();

	// update
	// update per frame
	virtual void update();

	// onTouchEvent
	// update touch event
	virtual bool onEvent( const SEvent& gameEvent);

	// getButtonState
	// set button state
	inline ButtonState getButtonState()
	{
		return m_buttonState;
	}

	// setButtonState
	// set button state
	void setButtonState(ButtonState state)
	{
		m_buttonState = state;
	}
    
	// setVisible
	// set visible a button
	virtual void setVisible( bool b );

};

#endif