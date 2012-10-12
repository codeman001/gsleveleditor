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
	};

protected:
	CMenuFxObj	m_flashObj;
	ButtonState m_buttonState;

public:

	CUIButton( CUIWidget* parent, CMenuFxObj flashObj );
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
};

#endif