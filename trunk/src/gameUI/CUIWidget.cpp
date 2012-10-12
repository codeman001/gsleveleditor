#include "stdafx.h"
#include "CUIWidget.h"
#include "IView.h"

CUIWidget::CUIWidget()
{
	m_parent = NULL;
	m_controlID = -1;
	m_visible = true;
}

CUIWidget::CUIWidget(CUIWidget* parent)
{
	m_parent = NULL;	
	m_controlID = -1;
	m_visible = true;

	parent->addChild( this );
}

CUIWidget::~CUIWidget()
{
	// remove from parent
	remove();

	// destroy all child
	destroyChild();
}

// addChild
// add child widget
void CUIWidget::addChild( CUIWidget *child )
{
	child->remove();

	child->m_parent = this;
	m_childs.push_back( child );
}

// removeChild
// remove a child from childs list
void CUIWidget::removeChild( CUIWidget *child )
{
	std::vector<CUIWidget*>::iterator i = m_childs.begin(), end = m_childs.end();
	while ( i != end )
	{
		if ( (*i) == child )
		{
			m_childs.erase(i);
			return;
		}

		i++;
	}
}

// destroyChild
// destroy all child
void CUIWidget::destroyChild()
{
	std::vector<CUIWidget*>::iterator i = m_childs.begin(), end = m_childs.end();
	while ( i != end )
	{
		delete (*i);
		i++;
	}
	m_childs.clear();
}

// remove
// remove from parent
void CUIWidget::remove()
{
	if ( m_parent )
		m_parent->removeChild( this );
}

// update
// update per frame
void CUIWidget::update()
{
	IVideoDriver* driver = getIView()->getDriver();
	
	// update bouding rect
	if ( m_parent == NULL )	
		m_rect = driver->getViewPort();

	//update childs
	std::vector<CUIWidget*>::iterator i = m_childs.begin(), end = m_childs.end();
	while ( i != end )
	{
		(*i)->update();
		i++;
	}
}

// onTouchEvent
// update touch event
bool CUIWidget::onEvent(const SEvent& gameEvent)
{
	std::vector<CUIWidget*>::iterator i = m_childs.begin(), end = m_childs.end();
	while ( i != end )
	{
		// delivery touch event to all child
		if ( (*i)->onEvent(gameEvent) == true )
			return true;
		i++;
	}
	return false;
}

// getRectByFxObj (static function)
// get rect from flash obj
void CUIWidget::getRectByFxObj( CMenuFxObj fxObj, core::recti& rect )
{	
	int x,y,w,h;
    float fw, fh;
    
	// get size (pixel) on flash
    fxObj.getBound(&x, &y, &w, &h);

	// get ratio from flash to screen game
	if ( fxObj.getMenu() == NULL )
	{
		fw = 1.0f;
		fh = 1.0f;
	}
	else
		fxObj.getMenu()->getFxScaleRatio(&fw, &fh);
    
	// calc size
    float realX, realY, realW, realH;
    realX = (float)x / fw;
    realY = (float)y / fh;
    realW = (float)w / fw;
    realH = (float)h / fh;

	// result
	rect.UpperLeftCorner	= core::position2di((int)x,(int)y);
	rect.LowerRightCorner	= core::position2di((int)(x+w),(int)(y+h));
}