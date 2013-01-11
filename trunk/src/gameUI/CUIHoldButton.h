#ifndef _CUIHOLDBUTTON_H_
#define _CUIHOLDBUTTON_H_

#include "stdafx.h"
#include "CUIWidget.h"

class CUIHoldButton: public CUIWidget
{
protected:
	bool	m_holdState;
public:
	CUIHoldButton( const char *name, CUIWidget* parent, CMenuFxObj flashObj );
	virtual ~CUIHoldButton();

	// update
	// update per frame
	virtual void update();

	// onTouchEvent
	// update touch event
	virtual bool onEvent( const SEvent& gameEvent);

	// setVisible
	// set visible a button
	virtual void setVisible( bool b );
};

#endif