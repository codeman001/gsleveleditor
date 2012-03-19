#include "stdafx.h"

#include "IView.h"
#include "CMainFrame.h"
#include "CGlobalInstance.h"

#include "resource.h"
#include "ids.h"


CMainFrame::CMainFrame()
{	
	m_pFramework = NULL;
	m_pApplication = NULL;

	m_mainSplitWnd = NULL;
}

CMainFrame::~CMainFrame()
{
	if(m_pFramework)
	{
		m_pFramework->Destroy();
		m_pFramework->Release();
		m_pFramework = NULL;
	}
}

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
	wndclass.hIconSm = LoadIcon (NULL, MAKEINTRESOURCE(IDI_GSEDITOR) );
	wndclass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_GSEDITOR));

	if (! RegisterClassExW(&wndclass))
		return false;

	return true;
}

#define ID_UNDO_COMMAND			28346
#define ID_REDO_COMMAND			28347
#define ID_CUT_COMMAND			28350
#define ID_PASTE_COMMAND		28351
#define ID_SELECTALL_COMMAND	28360

#define ID_NEW_COMMAND			28380
#define ID_OPEN_COMMAND			28381
#define ID_SAVE_COMMAND			28382
#define ID_EXPORT_COMMAND		28383

#define ID_SELECT_COMMAND		29000
#define ID_DELETE_COMMAND		29001
#define ID_ROTATE_COMMAND		29002
#define ID_SCALE_COMMAND		29003

#define ID_CAMERA_COMMAND		29004
#define ID_ADDOBJ_COMMAND		29005

LRESULT	CMainFrame::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch( uMsg )
	{
	case WM_COMMAND:
		{
			if ( GetFocus() != this->getHandle() )
				break;

			int id = LOWORD(wParam);
			switch ( id )
			{
			case ID_SELECT_COMMAND:
				CGlobalInstance::getInstance()->m_ribbonCommand->onSelectObjectCommand();
				break;
			case ID_DELETE_COMMAND:
				CGlobalInstance::getInstance()->m_ribbonCommand->onDeleteSelectObjectCommand();
				break;
			case ID_ROTATE_COMMAND:
				CGlobalInstance::getInstance()->m_ribbonCommand->onRotateObjectCommand();
				break;
			case ID_SCALE_COMMAND:
				CGlobalInstance::getInstance()->m_ribbonCommand->onScaleObjectCommand();
				break;

			case ID_CAMERA_COMMAND:
				CGlobalInstance::getInstance()->m_ribbonCommand->onCameraViewCommand();
				break;
			case ID_ADDOBJ_COMMAND:
				CGlobalInstance::getInstance()->m_ribbonCommand->onAddObjectCommand();
				break;

			case ID_NEW_COMMAND:
				CGlobalInstance::getInstance()->m_ribbonCommand->onNewDocumentCommand();
				break;
			case ID_SAVE_COMMAND:
				CGlobalInstance::getInstance()->m_ribbonCommand->onSaveDocumentCommand();
				break;
			case ID_OPEN_COMMAND:
				CGlobalInstance::getInstance()->m_ribbonCommand->onOpenDocumentCommand();
				break;
			}
		}
		break;
	};
	return uiForm::messageMap(hWnd, uMsg, wParam, lParam);
}

void CMainFrame::_OnIdle()
{
	IView *pView = getIView();
	if ( pView )
		pView->idleUpdateWindow();

	Sleep(1);
}

void CMainFrame::_OnCreate()
{
	AllocConsole();
	freopen("con","w",stdout);
	freopen("con","w",stderr);


	uiKeyAccelEntry e;	
	e.pushKey( FCONTROL, ('z'-'a' + 1) ,(WORD) ID_UNDO_COMMAND);		// control + z undo
	e.pushKey( FCONTROL, ('y'-'a' + 1) ,(WORD) ID_REDO_COMMAND);		// control + y redo	
	e.pushKey( FCONTROL, ('x'-'a' + 1) ,(WORD) ID_CUT_COMMAND);			// control + x cut
	e.pushKey( FCONTROL, ('v'-'a' + 1) ,(WORD) ID_PASTE_COMMAND);		// control + v paste
	e.pushKey( FCONTROL, ('a'-'a' + 1) ,(WORD) ID_SELECTALL_COMMAND);	// control + a select all

	e.pushKey( FCONTROL, ('n'-'a' + 1) ,(WORD) ID_NEW_COMMAND);			// control + n new
	e.pushKey( FCONTROL, ('o'-'a' + 1) ,(WORD) ID_OPEN_COMMAND);		// control + o open
	e.pushKey( FCONTROL, ('s'-'a' + 1) ,(WORD) ID_SAVE_COMMAND);		// control + s save

	e.pushKey( FVIRTKEY, VK_DELETE,		(WORD) ID_DELETE_COMMAND);		// delete
	e.pushKey( FVIRTKEY, 'S',			(WORD) ID_SELECT_COMMAND);		// select controller
	e.pushKey( FVIRTKEY, 'R',			(WORD) ID_ROTATE_COMMAND);		// rotate controller
	e.pushKey( FVIRTKEY, 'L',			(WORD) ID_SCALE_COMMAND);		// scale controller
	e.pushKey( FVIRTKEY, 'C',			(WORD) ID_CAMERA_COMMAND);		// camera view
	e.pushKey( FVIRTKEY, 'A',			(WORD) ID_ADDOBJ_COMMAND);		// add object controller

	createAccelTable( &e );


	HRESULT hr = ::CoCreateInstance(CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pFramework));

	if (FAILED(hr))
	{
		this->alert( TEXT("Can not init ribbon frameworks"), TEXT("Error") );
		uiApplication::exit(1);
		return;
	}
				
	// create application
	CRibbonApplication *pApplication;
	CRibbonApplication::CreateInstance(&pApplication, this->getHandle() );
	m_pApplication = pApplication;

	// init application
	hr = m_pFramework->Initialize(this->getHandle(), m_pApplication);
	if (FAILED(hr))
	{
		this->alert( TEXT("Can not init ribbon application"), TEXT("Error") );
		uiApplication::exit(1);
		m_pApplication->Release();
		return;
	}
			
	// load ui layout
	hr = m_pFramework->LoadUI(GetModuleHandle(NULL), L"APPLICATION_RIBBON");
	if (FAILED(hr))
	{
		this->alert( TEXT("Can not load ribbon ui"), TEXT("Error") );
		m_pApplication->Release();
		return;
	}
				
	// release ref on application
	m_pApplication->Release();	
	
	// store ref to global
	CGlobalInstance::getInstance()->m_ribbonFramework = m_pFramework;
	CGlobalInstance::getInstance()->m_mainFrame = this;
	
	// create status bar
	m_statusBarWnd = ref<uiStatusBar> ( new uiStatusBar(L"",0,0,0,0, this) );
	m_statusBarWnd->setDock(this, UIDOCK_BOTTOM );	
	int statusBarSize[] = {200, 200, 400, 1};
	m_statusBarWnd->createParts( 4, statusBarSize );
	m_statusBarWnd->showWindow(true);

	// create split window
	m_mainSplitWnd = ref<uiSplitContainer>( new uiSplitContainer(L"mainSplitWnd", 0,0,200,200, this, 1,3 ) );	
	m_mainSplitWnd->showWindow( true );	
	m_mainSplitWnd->setMargin(0,0,146,0);
	m_mainSplitWnd->setDock( this, UIDOCK_FILL );
	m_mainSplitWnd->changeWindowStyle( UISTYLE_CHILD );

	// create tab view
	uiTabControl *pTabView = ref<uiTabControl>( new uiTabControl(L"tabView", 0,0, 100, 100, m_mainSplitWnd) );

	m_viewWnd		= ref<CViewWindow>		( new CViewWindow(L"viewWnd", pTabView) );	
	m_leftSplitWnd	= ref<CLeftSplitWindow>	( new CLeftSplitWindow(L"leftSplitWnd", m_mainSplitWnd) );
	m_rightSplitWnd	= ref<CRightSplitWindow>( new CRightSplitWindow(L"rightSplitWnd", m_mainSplitWnd) );

	pTabView->setMargin( 0, 0, 5, 0 );
	pTabView->addTab( L"Game design mode", m_viewWnd );	
	pTabView->addTab( L"Game play mode", NULL );

	m_mainSplitWnd->setWindow( m_leftSplitWnd,	0,0 );
	m_mainSplitWnd->setWindow( pTabView,		0,1 );
	m_mainSplitWnd->setWindow( m_rightSplitWnd,	0,2 );

	// set size for propertyWnd
	m_mainSplitWnd->setColSize( 0, 220 );

	RECT rc;
	GetWindowRect(GetDesktopWindow(), &rc);
	int w = rc.right - rc.left;
	
	m_mainSplitWnd->setColSize( 1, w - 250 - 220 );
	m_mainSplitWnd->setColSize( 2, 250 );

	m_mainSplitWnd->setExpanderSize( 5 );
	m_mainSplitWnd->updateWindow();	

	CGlobalInstance::getInstance()->m_ribbonCommand->onCameraViewCommand();
}

void CMainFrame::_OnSize(uiSizeEvent sizeEvent, int nWidth, int nHeight)
{		
	if ( m_mainSplitWnd )
	{
		IUIRibbon* pRibbon = NULL;
		unsigned int height = 0;
		if (SUCCEEDED(m_pFramework->GetView(0, IID_PPV_ARGS(&pRibbon))))
		{
			pRibbon->GetHeight(&height);
			pRibbon->Release();
		}

		m_mainSplitWnd->setMargin(0,0,height,0);
		updateDock();
		m_ribbonUIHeight = height;
	}

	RECT rect;
    GetClientRect(this->getHandle(), &rect);
	rect.top = m_ribbonUIHeight;
	InvalidateRect(this->getHandle(), &rect, true);

	uiWindow::_OnSize( sizeEvent, nWidth, nHeight );
}

void CMainFrame::_OnPaint(uiGraphics * pG)
{
	uiWindow::_OnPaint( pG );
}

bool CMainFrame::_OnKeyDown( uiKeyEvent keyEvent )
{
	return uiWindow::_OnKeyDown( keyEvent);
}

bool CMainFrame::_OnKeyUp( uiKeyEvent keyEvent )
{
	return uiWindow::_OnKeyUp( keyEvent);
}

void CMainFrame::_OnMouseWheel	( uiMouseEvent mouseEvent, int x, int y )
{
	if ( GetFocus() == this->getHandle() )
		m_viewWnd->_OnMouseWheel( mouseEvent, x, y );
}