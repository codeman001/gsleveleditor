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
    
    bool        m_isEnable;
public:
	CUIDPad( const char *name, CUIWidget* parent, CMenuFxObj flashObj );
	virtual ~CUIDPad();

	// update
	// update per frame
	virtual void update();

	// onETouchEvent
	// update touch event
	virtual bool onEvent( const SEvent& gameEvent);
    
    // setEnable
    // enable this control
    inline void setEnable(bool b)
    {
        m_isEnable = b;
    }	
};


#endif