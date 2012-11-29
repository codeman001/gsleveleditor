#ifndef _UIHUDCONTAINER_H_
#define _UIHUDCONTAINER_H_

#include "stdafx.h"
#include "CUIWidget.h"

class CUIHudContainer: public CUIWidget
{
protected:
	CMenuFxObj	m_aimFocus;
public:

	CUIHudContainer( const char *name, CUIWidget* parent, CMenuFxObj aimFocus );
	virtual ~CUIHudContainer();

	// update
	// update per frame
	virtual void update();

	// onTouchEvent
	// update touch event
	virtual bool onEvent( const SEvent& gameEvent);	
    
};

#endif