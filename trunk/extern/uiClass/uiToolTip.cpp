// uiToolTip.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 24/2/2009
// Thong tin:

#include "uiToolTip.h"
#include "uiInclude.h"
#include "uiString.h"
#include "uiApplication.h"

using namespace uiClass;

	
HWND uiToolTip::zm_hWnd = 0;
TOOLINFOW uiToolTip::zm_toolInfo;

// GNU Conpiler Support /////////////////////////////
#if !defined(TTS_BALLOON)
	#define TTS_BALLOON             0x40
#endif
#if !defined(TTS_CLOSE)
	#define TTS_CLOSE               0x80
#endif
#if !defined(TTF_TRACK)
	#define TTF_TRACK               0x0020
#endif
#if !defined(TTM_SETMAXTIPWIDTH)
	#define TTM_SETMAXTIPWIDTH      (WM_USER + 24)
#endif
#if !defined(TTM_SETTITLE)
	#define TTM_SETTITLE            TTM_SETTITLEW
#endif
#if !defined(TTM_TRACKPOSITION)
	#define TTM_TRACKPOSITION       (WM_USER + 18)  // lParam = dwPos
#endif
#if !defined(TTM_TRACKACTIVATE)
	#define TTM_TRACKACTIVATE       (WM_USER + 17)  // wParam = TRUE/FALSE start end  lparam = LPTOOLINFO
#endif

#if !defined(TTM_POP)
	#define TTM_POP                 (WM_USER + 28)
#endif

#if !defined(TTM_SETTITLEW)
	#define TTM_SETTITLEW           (WM_USER + 33)  // wParam = TTI_*, lParam = wchar* szTitle
#endif
////////////////////////////////////////////////////

// destroyTip
// Huy tooltip
void uiToolTip::destroyTip()
{
	if (IsWindow(zm_hWnd))
	{
		DestroyWindow( zm_hWnd );
		zm_hWnd = 0;
	}
}

// showTip 
// Hien thi ICON
void uiToolTip::showTip ( uiWindow *pWin, 
						LPWSTR lpCaption, LPWSTR lpString, DWORD dwIcon,
						bool bIsBalloon, bool bIsClose,
						int x, int y)
{
	if ( zm_hWnd != 0 )
		DestroyWindow( zm_hWnd );
	
	DWORD dwStyle = WS_POPUP | WS_VISIBLE | TTS_ALWAYSTIP | TTS_NOPREFIX;
	if (bIsBalloon)
		dwStyle |= TTS_BALLOON;
	if (bIsClose)
		dwStyle |= TTS_CLOSE;
	
	// Khoi tao cua so TOOLTIPS
	zm_hWnd = ::CreateWindowEx(0, TOOLTIPS_CLASS, 0, dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		pWin->getHandle(), 0, 0, 0);
	
	
	
	zm_toolInfo.cbSize   = sizeof(zm_toolInfo);		
	zm_toolInfo.uFlags   = TTF_TRACK;		
	zm_toolInfo.hwnd     = pWin->getHandle();
	zm_toolInfo.lpszText = lpString;
	zm_toolInfo.uId      = 0;
	zm_toolInfo.hinst    = pWin->getInstance();

	if (IsWindow(zm_hWnd))
	{
		SendMessage(zm_hWnd, TTM_SETMAXTIPWIDTH, 0, 500);						
		SendMessage(zm_hWnd, TTM_ADDTOOL, 0, (LPARAM) &zm_toolInfo);
		SendMessage(zm_hWnd, TTM_SETTITLE, (WPARAM) dwIcon, (LPARAM) lpCaption);
		SendMessage(zm_hWnd, TTM_TRACKPOSITION, (WPARAM)0, (LPARAM)MAKELONG(x, y));
		SendMessage(zm_hWnd, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&zm_toolInfo);
	}

}

void uiToolTip::showTip ( uiWindow *pWin, LPWSTR lpString, int x, int y)
{
	if ( zm_hWnd != 0 )
		DestroyWindow( zm_hWnd );
	
	DWORD dwStyle = WS_POPUP | WS_VISIBLE | TTS_ALWAYSTIP | TTS_NOPREFIX;
	
	// Khoi tao cua so TOOLTIPS
	zm_hWnd = ::CreateWindowEx(0, TOOLTIPS_CLASS, 0, dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		pWin->getHandle(), 0, 0, 0);
	
	
	zm_toolInfo.cbSize   = sizeof(zm_toolInfo);		
	zm_toolInfo.uFlags   = TTF_TRACK;		
	zm_toolInfo.hwnd     = pWin->getHandle();
	zm_toolInfo.lpszText = lpString;
	zm_toolInfo.uId      = 0;
	zm_toolInfo.hinst    = pWin->getInstance();

	if (IsWindow(zm_hWnd))
	{
		SendMessage(zm_hWnd, TTM_SETMAXTIPWIDTH, 0, 500);						
		SendMessage(zm_hWnd, TTM_ADDTOOL, 0, (LPARAM) &zm_toolInfo);	
		SendMessage(zm_hWnd, TTM_TRACKPOSITION, (WPARAM)0, (LPARAM)MAKELONG(x, y));
		SendMessage(zm_hWnd, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&zm_toolInfo);		
	}

}

// hideTip
// An hay tip
void uiToolTip::hideTip()		
{
	if (IsWindow(zm_hWnd))
	{
			SendMessage(zm_hWnd, TTM_POP, 0, 0);
	}
}

// setTipPos
// Thiet lap tipPos
void uiToolTip::setTipPos( int x, int y)
{
	if (IsWindow(zm_hWnd))
		SendMessage(zm_hWnd, TTM_TRACKPOSITION, (WPARAM)0, (LPARAM)MAKELONG(x, y));
}

// setTipString
// Thay doi string
void uiToolTip::setTipString( LPWSTR lpString )
{
	if (IsWindow(zm_hWnd))
	{
		zm_toolInfo.lpszText = lpString;
		SendMessage(zm_hWnd, TTM_SETTOOLINFO, 0, (LPARAM)&zm_toolInfo);
	}
}

void uiToolTip::setTipString( LPWSTR lpString, LPWSTR lpCaption, DWORD dwIcon )
{
	if (IsWindow(zm_hWnd))
	{
		SendMessage(zm_hWnd, TTM_SETTITLE, (WPARAM) dwIcon, (LPARAM) lpCaption);
		zm_toolInfo.lpszText = lpString;
		SendMessage(zm_hWnd, TTM_SETTOOLINFO, 0, (LPARAM)&zm_toolInfo);	
	}
}