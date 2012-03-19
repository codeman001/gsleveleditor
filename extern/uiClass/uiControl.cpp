// uiWindow.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 30/12/2008
// Thong tin:
//

#include "uiWindow.h"
#include "uiInclude.h"

using namespace uiClass;

uiControl::uiControl()
{
	zm_nID = 0;
}

uiControl::~uiControl()
{

}

// getObject
// Tra ve UIOBJECT
DWORD uiControl::getObject()
{
	return UIOBJECT_CONTROL;
}

// subClass
// Thiet lap subclass cho control de bat MSG
void uiControl::subClass()
{
	#pragma warning(disable: 4311)
	#pragma warning(disable: 4312)

	// WNDPROC mac dinh cua control
	zm_lOldWinProc = GetWindowLong(zm_hWnd, GWLP_WNDPROC);

	// Them userData la con tro this
	SetWindowLong(zm_hWnd, GWL_USERDATA,reinterpret_cast<long>(this) );

	// Thay doi WNDPROC cua control nay
	SetWindowLongPtr( zm_hWnd, GWLP_WNDPROC,(LONG) uiControl::windowProc );		

	#pragma warning(default: 4311)
	#pragma warning(default: 4312)
}

// windowProc
// Quan ly su kien va nam bat control se thuc thi su kien
LRESULT CALLBACK uiControl::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	#pragma warning(disable: 4311)
	#pragma warning(disable: 4312)
	
	// Chuyen event cho cua so khac
	if ( sm_globalEventWindow )
	{
		list<uiWindow*>::iterator iWin = sm_globalEventWindow->begin(), end = sm_globalEventWindow->end();
		while ( iWin != end )
		{
			if ( (*iWin)->globalMessageMap( hWnd, uMsg, wParam, lParam ) == false )
			{
				// break all event to another window
				return DefWindowProc(hWnd,uMsg,wParam,lParam);
			}
			iWin++;
		}
	}

	uiControl *pWin = NULL;

	// Lay userData tu control tu hWnd
	pWin = reinterpret_cast<uiControl*> (GetWindowLong(hWnd,GWL_USERDATA) );
	if (pWin)
	{
		// Goi messageMap
		LRESULT r = pWin->messageMap(hWnd,uMsg,wParam,lParam);			
		
		if (r < 0)
			return DefWindowProc(hWnd,uMsg,wParam,lParam);	
		else
			return r;
	}
	else	
		return DefWindowProc(hWnd, uMsg, wParam , lParam);	

	#pragma warning(default: 4311)	
	#pragma warning(default: 4312)	
}

// messageMap
// Phan loai su kien
LRESULT uiControl::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	#pragma warning(disable: 4312)
	
	// Thuc thi control mac dinh
	LRESULT r = CallWindowProcW( (WNDPROC) zm_lOldWinProc,hWnd, uMsg, wParam, lParam);

	// Thuc thi control duoc ke thua tu uiWindow
	uiWindow::messageMap(hWnd, uMsg, wParam, lParam);
	
	return r;
	#pragma warning(default: 4312)	
}

// messageControlMap
// Cac message danh cho control
void uiControl::messageControlMap(UINT uMsg)
{
	
}

// notifyControl
// Xu ly notify
void uiControl::notifyControl(LPARAM lParam)
{
	UINT nMsg = ((LPNMHDR)lParam)->code;

}

// setID
// Thiet lap ID 
void uiControl::setID( UINT nId )
{
	zm_nID = nId;
}

// getID
// Lay gia tri ID
UINT uiControl::getID()
{
	return zm_nID;
}