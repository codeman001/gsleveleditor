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

	m_irrWin = ref<CIrrWindow>( new CIrrWindow(L"irrWindow", this) );
	m_irrWin->changeWindowStyle( UISTYLE_CHILD );
	m_irrWin->setDock( this, UIDOCK_FILL );


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
	wndclass.hIconSm = LoadIcon (NULL, MAKEINTRESOURCE(IDI_GSANIMATION) );
	wndclass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_GSANIMATION));

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