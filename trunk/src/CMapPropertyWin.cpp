#include "stdafx.h"
#include "CMapPropertyWin.h"


CMapPropertyWindow::CMapPropertyWindow( WCHAR* lpString, uiWindow *p )
	:CBaseWindow(lpString, p)
{	
}

CMapPropertyWindow::~CMapPropertyWindow()
{	
}

void CMapPropertyWindow::_OnPaint( uiGraphics *pG )
{
	uiWindow::_OnPaint( pG );
	drawTitleBackground(pG, 0,0, this->getWidth(), 30, TEXT("Đối tượng") );	
}
