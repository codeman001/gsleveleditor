#ifndef _CUIDPAD_H_
#define _CUIDPAD_H_

#include "CUIWidget.h"

class CUIDPad: public CUIWidget
{
protected:
	bool        m_mouseOver;

	CMenuFxObj	m_dpadBase;
	CMenuFxObj	m_dpadMove;

	core::position2di   m_dpadBasePos;
	core::position2di   m_dpadMovePos;
public:
	CUIDPad( const char *name, CUIWidget* parent, CMenuFxObj flashObj );
	virtual ~CUIDPad();

	// update
	// update per frame
	virtual void update();

	// onTouchEvent
	// update touch event
	virtual bool onEvent( const SEvent& gameEvent);
};


#endif