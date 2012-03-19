// uiRebarWindow.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 3/3/2009 
// Thong tin:
//

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"
#include "uiRebarWindow.h"

using namespace uiClass;

uiRebarWindow::uiRebarWindow(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
	:uiWindow(lpTitle, x,y,w,h, pParent)
{
	
}

uiRebarWindow::~uiRebarWindow()
{
	
}

LRESULT uiRebarWindow::messageMap( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( uMsg == WM_ERASEBKGND )
	{
		HDC hDC = (HDC) wParam;
		POINT p={0,0};
		MapWindowPoints( this->getHandle(),this->getParent()->getHandle(), &p, 1 );

		OffsetWindowOrgEx( hDC, p.x,p.y, &p );
		LRESULT lResult = SendMessage(this->getParent()->getHandle(), WM_ERASEBKGND,(WPARAM)hDC, 0L);
		SetWindowOrgEx( hDC, p.x, p.y, &p );
		return lResult;
	}
	
	if ( uMsg == WM_MOVE )
	{
		InvalidateRect( this->getHandle(), NULL, true );
	}

	return uiWindow::messageMap(hWnd, uMsg, wParam, lParam);
}