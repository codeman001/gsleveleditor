#include "stdafx.h"
#include "CTemplateWindow.h"
#include "CTemplateDialog.h"

CTemplateWindow::CTemplateWindow( WCHAR *lpString, uiWindow *p )
	:CAddEditWindow(lpString, p)
{
	reloadList();
}

CTemplateWindow::~CTemplateWindow()
{
}
	
void CTemplateWindow::_OnPaint( uiGraphics *pG )
{
	uiWindow::_OnPaint( pG );
	drawTitleBackground(pG, 0,0, this->getWidth(), 30, TEXT("Game object template"));
}

void CTemplateWindow::onAddButton()
{
	CTemplateDialog dialog(L"Add, modify object template", 0, 0, 500, 400, this);
	dialog.setPositionCenterOfScreen();
	
	uiApplication::getRoot()->doModal( &dialog );
}

void CTemplateWindow::onModifyButton()
{
}

void CTemplateWindow::onDelButton()
{
}

// reloadList
// reload list of item
void CTemplateWindow::reloadList()
{
}