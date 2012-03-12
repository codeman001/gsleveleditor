#include "stdafx.h"
#include "CComponentWindow.h"
#include "CComponentDialog.h"

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

void CComponentWindow::onAddButton()
{
	CComponentDialog dialog(L"Add, modify component", 0, 0, 500, 400, this);
	dialog.setPositionCenterOfScreen();
	
	uiApplication::getRoot()->doModal( &dialog );
}

void CComponentWindow::onModifyButton()
{
}

void CComponentWindow::onDelButton()
{
}