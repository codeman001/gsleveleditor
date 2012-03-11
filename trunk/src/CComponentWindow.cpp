#include "stdafx.h"
#include "CComponentWindow.h"

CComponentWindow::CComponentWindow( WCHAR *lpString, uiWindow *pParent )
	:CAddEditWindow(lpString, pParent)
{
	
}

CComponentWindow::~CComponentWindow()
{
}

void CComponentWindow::_OnPaint( uiGraphics *pG )
{
	uiWindow::_OnPaint( pG );
	drawTitleBackground(pG, 0,0, this->getWidth(), 30, TEXT("Game component"));
}