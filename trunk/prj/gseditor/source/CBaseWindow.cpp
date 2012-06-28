#include "stdafx.h"
#include "CBaseWindow.h"

CBaseWindow::CBaseWindow( WCHAR* lpString, uiWindow *pParent ):
	uiWindow(lpString, 0, 0, 100, 100, pParent)	
{
	this->changeWindowStyle( UISTYLE_CHILD );
	this->showWindow(true);

	m_theme = NULL;
}

CBaseWindow::~CBaseWindow()
{
	if ( m_theme )
		CloseThemeData( m_theme );
}

void CBaseWindow::_OnCreate()
{
	uiWindow::_OnCreate();
}

void CBaseWindow::drawTitleBackground(uiGraphics *pG, int x, int y, int w, int h, wchar_t* lpText)
{
	if ( m_theme == NULL )
		m_theme = OpenThemeData( this->getHandle(), TEXT("HEADER") );

	RECT rect = {x,y,w,h};
	int partID = HP_HEADERITEM ;
	int stateID = 0;

	DrawThemeBackground(
		m_theme,
		pG->getHandle(),
		partID, stateID,
		&rect, NULL
		);

	int textPadding = 10;

	uiFont font(14, L"tahoma");
	pG->setTextBkTransparent();
	pG->selectObject( &font );
	pG->drawTextInRect(textPadding,0, this->getWidth()-textPadding, h, lpText, DT_VCENTER|DT_LEFT|DT_SINGLELINE );
}