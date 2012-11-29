#include "stdafx.h"
#include "CUIHudContainer.h"
#include "gameEvent.h"
#include "IView.h"

CUIHudContainer::CUIHudContainer( const char *name, CUIWidget* parent, CMenuFxObj aimFocus )
	:CUIWidget(name, parent)
{
	m_aimFocus = aimFocus;
}

CUIHudContainer::~CUIHudContainer()
{
}

// update
// update per frame
void CUIHudContainer::update()
{	
}

// onTouchEvent
// update touch event
bool CUIHudContainer::onEvent( const SEvent& gameEvent)
{
	// if childs handle this event
    if ( CUIWidget::onEvent(gameEvent) == true )
        return true;

	return false;
}