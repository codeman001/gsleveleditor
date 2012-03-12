#include "stdafx.h"
#include "CBaseDialog.h"

CBaseDialog::CBaseDialog(LPWSTR lpTitle, int x, int y, int w, int h,uiWindow* pParent)
	:uiDialog(lpTitle, x,y,w,h, pParent),
	m_titleFont(14, L"tahoma"),
	m_titleString(L"Need set title string!")
{
	// default width height
	m_headerHeight = 235;
	m_bottomHeight = 70;
}

CBaseDialog::~CBaseDialog()
{
}

void CBaseDialog::_onPaint( uiGraphics *pG )
{	 
	uiDialog::_OnPaint( pG );

	uiBrush bWhite( uiColor(0xffffff) );
	uiPen	penGrey( 1, PS_SOLID, uiColor(0xf0f0f0) );

	int h = getClientHeight();
	int w = getClientWidth();

	pG->drawFillRectangle(0, 0, w, h - m_bottomHeight, &bWhite);
	
	pG->selectObject( &penGrey );
	pG->drawLine(0, m_headerHeight, w, m_headerHeight);
}

void CBaseDialog::onButtonCommand( uiObject *pSender )
{
}