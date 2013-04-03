#ifndef _UIBUTTON_H_
#define _UIBUTTON_H_

#include "stdafx.h"
#include "CUIWidget.h"

class CUIButton: public CUIWidget
{
public:
	enum EButtonState
	{
		ButtonNormal = 0,
		ButtonInvisible,
		ButtonDisable,
		ButtonFocus,
        ButtonRelease
	};
    
protected:
	EButtonState m_buttonState;
	EButtonState m_buttonLastState;
    bool        m_mouseOver;
    
public:

	CUIButton( const char *name, CUIWidget* parent, CMenuFxObj flashObj );
	virtual ~CUIButton();

	// update
	// update per frame
	virtual void update();

	// onETouchEvent
	// update touch event
	virtual bool onEvent( const SEvent& gameEvent);

	// getButtonState
	// set button state
	inline EButtonState getButtonState()
	{
		return m_buttonState;
	}

	// setButtonState
	// set button state
	void setButtonState(EButtonState state)
	{
		m_buttonState = state;
	}
    
	// setVisible
	// set visible a button
	virtual void setVisible( bool b );

};

#endif