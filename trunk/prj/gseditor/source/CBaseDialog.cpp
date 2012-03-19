#include "stdafx.h"
#include "CBaseDialog.h"

CBaseDialog::CBaseDialog(LPWSTR lpTitle, int x, int y, int w, int h,uiWindow* pParent)
	:uiDialog(lpTitle, x,y,w,h, pParent),
	m_titleFont(18, L"tahoma"),
	m_titleString(L"Need set title string!")
{
	// default width height
	m_headerHeight = 60;
	m_bottomHeight = 50;

	int buttonWidth = 90;
	int butonHeight = 25;

	uiButton *okButton = ref<uiButton>( new uiButton(L"OK", 0, 0, buttonWidth, butonHeight, this) );
	okButton->setLockLayout( this, UILOCKLAYOUT_BOTTOM|UILOCKLAYOUT_RIGHT, 0,0, 120, 12 );
	okButton->setEventOnClicked<CBaseDialog, &CBaseDialog::onButtonCommand>(this);
	okButton->showWindow( true );

	uiButton *cancelButton = ref<uiButton>( new uiButton(L"Cancel", 0, 0, buttonWidth, butonHeight, this) );
	cancelButton->setLockLayout( this, UILOCKLAYOUT_BOTTOM|UILOCKLAYOUT_RIGHT, 0,0, 20, 12 );
	cancelButton->setEventOnClicked<CBaseDialog, &CBaseDialog::onButtonCommand>(this);
	cancelButton->showWindow( true );

	m_okButton = okButton;
	m_cancelButton = cancelButton;
	
	enableOnIdle(true);
	updateLockLayout();
}

CBaseDialog::~CBaseDialog()
{
}

void CBaseDialog::_OnPaint( uiGraphics *pG )
{	 
	uiDialog::_OnPaint( pG );

	uiBrush bWhite( uiColor(0xffffff) );
	uiPen	penGrey( 1, PS_SOLID, uiColor(0xf0f0f0) );

	int h = getClientHeight();
	int w = getClientWidth();

	pG->drawFillRectangle(0, 0, w, h - m_bottomHeight, &bWhite);
	
	int paddingLeft = 55;
	int paddingRight = 20;
	int paddingTop  = 10;
	pG->selectObject( &m_titleFont );
	pG->setTextColor( uiColor(0x963300) );	// bgr
	pG->setTextBkTransparent( true );
	pG->drawTextInRect
		( 
			paddingLeft, paddingTop, w - paddingLeft - paddingRight, m_headerHeight - paddingTop, 
			m_titleString.c(), 
			DT_LEFT|DT_WORDBREAK 
		);

	pG->selectObject( &penGrey );
	pG->drawLine(0, m_headerHeight, w, m_headerHeight);
}

void CBaseDialog::_OnIdle()
{
	((uiForm*)uiApplication::getRoot())->_OnIdle();
}

// messageMap	
LRESULT CBaseDialog::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	((uiForm*)uiApplication::getRoot())->_OnIdle();
	return uiDialog::messageMap( hWnd, uMsg, wParam, lParam );
}

void CBaseDialog::onButtonCommand( uiObject *pSender )
{
	bool ret = false;

	if ( m_okButton == pSender )
	{
		ret = onOKButton();	
	}
	else if (m_cancelButton == pSender)
	{
		ret = onCancelButton();
	}

	if ( ret )
		closeWindow();
}