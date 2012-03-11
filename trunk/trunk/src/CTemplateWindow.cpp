#include "stdafx.h"
#include "CTemplateWindow.h"

CTemplateWindow::CTemplateWindow( WCHAR *lpString, uiWindow *p )
	:CAddEditWindow(lpString, p)
{
}

CTemplateWindow::~CTemplateWindow()
{
}
	
void CTemplateWindow::_OnPaint( uiGraphics *pG )
{
	uiWindow::_OnPaint( pG );
	drawTitleBackground(pG, 0,0, this->getWidth(), 30, TEXT("Game object template"));
}