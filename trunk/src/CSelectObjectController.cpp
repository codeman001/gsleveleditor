#include "stdafx.h"
#include "CSelectObjectController.h"
#include "IView.h"
#include "IDoc.h"

CSelectObjectController::CSelectObjectController()
{
}

CSelectObjectController::~CSelectObjectController()
{
}

void CSelectObjectController::initController()
{
	m_isLMouseDown = false;
	m_isRMouseDown = false;
}

void CSelectObjectController::cancelController()
{
}

void CSelectObjectController::onKeyUp(int keyCode)
{
}

void CSelectObjectController::onLMouseDown(int x, int y)
{
	m_isLMouseDown = true;

	m_beginX = x;
	m_beginY = y;
}

void CSelectObjectController::onLMouseUp(int x, int y)
{
	m_isLMouseDown = false;

	int beginX = m_beginX;
	int beginY = m_beginY;
	int endX = x;
	int endY = y;

	if ( beginX > endX )
		core::swap<int>( beginX, endX );
	if ( beginY > endY )
		core::swap<int>( beginY, endY );

	core::recti	selectRect( beginX, beginY, endX, endY );

	IDoc *pDoc = getIView()->getDocument();
	
	// clear current select if not hold control
	bool isControlHold = GetAsyncKeyState( VK_CONTROL ) < 0;
	
	// clear the select if not hold control
	if ( isControlHold == false )
		pDoc->clearSelect();
	
	// detect select
	if ( selectRect.getWidth() > 5 && selectRect.getHeight() > 5 )
		pDoc->selectObject( beginX, beginY, selectRect.getWidth(), selectRect.getHeight(), isControlHold );
	else
		pDoc->selectObject( x, y, isControlHold );
	
	// update state for select object
	pDoc->setStateForSelectObject( CGameObject::Select );
	
}

void CSelectObjectController::onRMouseDown(int x, int y)
{
	m_isRMouseDown = true;
}

void CSelectObjectController::onRMouseUp(int x, int y)
{
	m_isRMouseDown = false;
}

void CSelectObjectController::onMouseWhell(bool scrollDown, int x, int y)
{
}

void CSelectObjectController::render()
{
	IVideoDriver *driver = getIView()->getDriver();
	
	if ( m_isLMouseDown )
	{
		int beginX = m_beginX;
		int beginY = m_beginY;
		int endX = getIView()->m_mouseX;
		int endY = getIView()->m_mouseY;

		if ( beginX > endX )
			core::swap<int>( beginX, endX );
		if ( beginY > endY )
			core::swap<int>( beginY, endY );

		core::recti	selectRect( beginX, beginY, endX, endY );
		
		// draw select
		driver->draw2DRectangle( SColor(80, 0,255,0), selectRect );
		driver->draw2DRectangleOutline( selectRect, SColor(200, 0,255,0) );
	}

}