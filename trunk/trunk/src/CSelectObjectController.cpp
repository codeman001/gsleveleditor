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

		core::recti	rect( beginX, beginY, endX, endY );
		
		// draw select
		driver->draw2DRectangle( SColor(80, 0,255,0), rect );
		driver->draw2DRectangleOutline( rect, SColor(200, 0,255,0) );
	}

}