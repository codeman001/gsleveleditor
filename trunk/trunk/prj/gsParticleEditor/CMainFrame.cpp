#include "stdafx.h"
#include "CMainFrame.h"
#include "Resource.h"

CMainFrame::CMainFrame()
{	
}

CMainFrame::~CMainFrame()
{
}
	
// create
// Khoi tao cua so
int CMainFrame::create(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, LPWSTR lpName)
{
	int ret = uiForm::create( lpTitle, x, y, w, h, pParent, lpName );

	uiMenu *pMainMenu = ref<uiMenu> (new uiMenu());
	uiKeyAccelEntry e;
	
	// add file popup
	uiMenu *pFile =	pMainMenu->appendMenuPopup( L"&File" );
	
	uiMenuItem *pMenuItem = NULL;
	pMenuItem = pFile->appendMenuItem( L"&New effects\tCtrl+N" );
	pMenuItem->setEventOnClick<CMainFrame, &CMainFrame::onMenuNewEffects>( this );
	e.pushKey( FCONTROL, ('n'-'a' + 1) ,(WORD) pMenuItem->getID() );

	pMenuItem = pFile->appendMenuItem( L"&Open effects\tCtrl+O" );
	pMenuItem->setEventOnClick<CMainFrame, &CMainFrame::onMenuOpenEffects>( this );
	e.pushKey( FCONTROL, ('o'-'a' + 1) ,(WORD) pMenuItem->getID() );

	pMenuItem = pFile->appendMenuItem( L"&Save effects\tCtrl+S" );
	pMenuItem->setEventOnClick<CMainFrame, &CMainFrame::onMenuSaveEffects>( this );
	e.pushKey( FCONTROL, ('s'-'a' + 1) ,(WORD) pMenuItem->getID() );
	
	pFile->appendMenuItem();
	pMenuItem = pFile->appendMenuItem( L"E&xit\tCtrl+X" );
	pMenuItem->setEventOnClick<CMainFrame, &CMainFrame::onMenuExit>( this );
	e.pushKey( FCONTROL, ('x'-'a' + 1) ,(WORD) pMenuItem->getID() );	

	// add menu
	createAccelTable( &e );
	setMenu( pMainMenu );
	

	// add rebar
	uiRebar *pRebar = ref<uiRebar>( new uiRebar(L"RebarTool", 0,0, 300,30, this) );		

	// add tool bar
	uiToolbar *pToolbar = ref<uiToolbar>( new uiToolbar(L"Command", 0,0,0,0, 24,24, this ) );

	uiIcon iconAdd( MAKEINTRESOURCE(IDI_TOOLBARADD) );	
	uiIcon iconDel( MAKEINTRESOURCE(IDI_TOOLBARDEL) );

	DWORD iconAddIndex = pToolbar->pushImage( &iconAdd );	
	DWORD iconDelIndex = pToolbar->pushImage( &iconDel );
	
	SendMessageW( pToolbar->getHandle(), TB_SETBUTTONWIDTH, 0, 70);
	pToolbar->setTextRight(true);

	// add button
	m_addButton	= pToolbar->addButton(L"Add", iconAddIndex);
	m_addButton->setEventOnClicked<CMainFrame, &CMainFrame::onToolbarAdd>( this );
		
	// delete button
	m_delButton	= pToolbar->addButton(L"Delete", iconDelIndex);	
	m_delButton->setEventOnClicked<CMainFrame, &CMainFrame::onToolbarDel>( this );

	// setup band of toolbar
	uiRebarBand bandToolbar( pToolbar,L"");	
	bandToolbar.setWidth(200);	
	bandToolbar.setMinWidthHeight(0, 30);
	bandToolbar.enableGripper( true );
	bandToolbar.setBreakBand( true );
	
	// add band to rebar
	pRebar->addBand( &bandToolbar );
	pRebar->setDock( this, UIDOCK_TOP );	
	pRebar->showBandBorder( false );
	pRebar->showWindow(true);


	// add main view
	uiSplitContainer *split = ref<uiSplitContainer>( new uiSplitContainer(L"MainSplit", 0,0, 400, 400, this, 1, 2) );	
	uiSplitContainer *splitLeft = ref<uiSplitContainer>( new uiSplitContainer(L"LeftSplit", 0,0, 400, 400, split, 2, 1) );
	
	m_effectTreeWin		= ref<uiTreeView>( new uiTreeView(L"EffectTree", 0,0, 100, 100, true, splitLeft) );	
	m_effectTreeWin->changeWindowStyle( UISTYLE_CHILD );	

	m_effectPropertyWin	= ref<uiListProperty>( new uiListProperty(L"EffectProperty", 0,0, 100, 100, splitLeft, 2) ) ;
	m_effectPropertyWin->changeWindowStyle( UISTYLE_CHILD );	
	m_effectPropertyWin->showWindow( true );
		
	m_irrWin = ref<CIrrWindow>( new CIrrWindow(L"irrWindow", this) );

	// config split left
	splitLeft->setWindow( m_effectTreeWin, 0, 0 );
	splitLeft->setWindow( m_effectPropertyWin, 1, 0 );
	splitLeft->setRowSize( 0, h/2 );
	splitLeft->setRowSize( 1, h/2 );
	splitLeft->setExpanderSize( 5 );
	splitLeft->changeWindowStyle( UISTYLE_CHILD );
	splitLeft->showWindow( true );

	// config main split
	split->setWindow( splitLeft, 0, 0 );
	split->setWindow( m_irrWin,  0, 1 );
		
	split->setColSize( 0, 220 );
	split->setColSize( 1, w - 220 );	

	split->setExpanderSize( 5 );
	split->setDock( this, UIDOCK_FILL );
	split->changeWindowStyle( UISTYLE_CHILD );
	split->updateWindow();		
	
	split->showWindow( true );
	return ret;
}

void CMainFrame::_OnPaint(uiGraphics * pG)
{
	uiForm::_OnPaint( pG );
}

void CMainFrame::_OnSize(uiSizeEvent sizeEvent, int nWidth, int nHeight)
{
	uiForm::_OnSize( sizeEvent, nWidth, nHeight );
}
	
bool CMainFrame::_OnKeyDown( uiKeyEvent keyEvent )
{
	return true;
}

bool CMainFrame::_OnKeyUp( uiKeyEvent keyEvent )
{
	return true;
}

void CMainFrame::_OnMouseWheel	( uiMouseEvent mouseEvent, int x, int y )
{
}

void CMainFrame::_OnIdle()
{
	m_irrWin->irrUpdate();
}

// registerWindow
// Dang ky cua so form
bool CMainFrame::registerWindow(LPWSTR lpNameApp, HINSTANCE hInst)
{
	WNDCLASSEXW wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = uiWindow::windowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;	
	wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject (LTGRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = lpNameApp;
	wndclass.hIconSm = LoadIcon (NULL, MAKEINTRESOURCE(IDI_GSPARTICLEEDITOR) );
	wndclass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_GSPARTICLEEDITOR));

	if (! RegisterClassExW(&wndclass))
		return false;

	return true;
}

// messageMap
// Phan tich su kien cho uiForm
LRESULT	CMainFrame::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return uiForm::messageMap(hWnd, uMsg, wParam, lParam);
}




void CMainFrame::onMenuNewEffects( uiObject *pSender )
{
}

void CMainFrame::onMenuOpenEffects( uiObject *pSender )
{
}

void CMainFrame::onMenuSaveEffects( uiObject *pSender )
{
}

void CMainFrame::onMenuExit( uiObject *pSender )
{
	uiApplication::exit();
}

void CMainFrame::onToolbarAdd( uiObject *pSender )
{
}

void CMainFrame::onToolbarDel( uiObject *pSender )
{
}