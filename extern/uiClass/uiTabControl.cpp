// uiTabControl.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 23/2/2009 
// Thong tin:
//

#include "uiTabControl.h"
#include "uiInclude.h"
#include "uiApplication.h"
#include <windowsx.h>

using namespace uiClass;

// GNU Compiler ////////////////////////
#if !defined(TCN_FOCUSCHANGE)
	#define TCN_FOCUSCHANGE         (TCN_FIRST - 4)
#endif
////////////////////////////////////////

uiTabControl::uiTabControl(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	this->create( lpTitle, x,y,w,h, pParent );
}

uiTabControl::~uiTabControl()
{
	
}

// create
// Khoi tao button
bool uiTabControl::create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	assert( pParent );

	HINSTANCE hInst = pParent->getInstance();	
	HWND hParent;
	if (pParent == NULL)
		hParent = NULL;
	else
		hParent = pParent->getHandle();		
	
	zm_pParent = pParent;
	 
	this->zm_hInstance = hInst;	

#pragma warning(disable:4312)
	//Khoi tao cua so
	zm_hWnd = CreateWindowExW(
		0,
		L"SysTabControl32",
		lpTitle,
		WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_CLIPCHILDREN,
		x,y,w,h,
		hParent,
		(HMENU) uiApplication::controlRegister(this),
		hInst, NULL);
#pragma warning(default:4312)

	// Thay doi WND
	this->subClass();

	// Thiet lap quan he
	pParent->addChild(this);
	this->setParent(pParent);
	
	// Thay doi font chu
	SendMessage(zm_hWnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
               MAKELPARAM(false, 0));	

	return true;	
}

// getObject
// Tra ve doi tuong UIOBJECT_
DWORD uiTabControl::getObject()
{
	return UIOBJECT_TABCONTROL;
}

// notifyControl
// Phan loai su kien cho Control
void uiTabControl::notifyControl( LPARAM lParam )
{	
	UINT nMsg = ((LPNMHDR)lParam)->code;		

	switch ( nMsg )
	{
	case NM_DBLCLK:	
		eventOnDbClicked(this);
		break;
	case NM_CLICK:
		eventOnClicked(this);
		break;
	case NM_RCLICK:
		eventOnRClicked(this);
		break;
	case NM_RDBLCLK:
		eventOnRDbClicked(this);
		break;	
	case TCN_FOCUSCHANGE:
		eventOnFocusChanged(this);
		break;
	case TCN_SELCHANGE:		
		this->tabChange();
		eventOnTabChanged(this);
		break;
	case TCN_SELCHANGING:
		this->tabChanging();
		eventOnTabChanging(this);
		break;
	}

	uiControl::notifyControl(lParam);
}

// addTab
// Them 1 tab window moi
void uiTabControl::addTab( LPWSTR lpTabString, uiWindow* pWindow )
{
	// Lay Index hien tai cua Tab
	DWORD nItem = (DWORD) zm_listTabString.size();
	
	// Luu string vao array
	zm_listTabString.push_back( lpTabString );
	zm_listTabWindows.push_back( pWindow );

	if ( pWindow )
	{
		pWindow->setParent(this);
		
		pWindow->setLockLayout(this, UILOCKLAYOUT_LEFT| UILOCKLAYOUT_TOP| UILOCKLAYOUT_RIGHT| UILOCKLAYOUT_BOTTOM,
			4,28,4,4 );	
	}

	// Lay chuoi string
	LPWSTR string = (WCHAR*)zm_listTabString[nItem].data() ;

	TCITEMW item;
	memset( &item, 0, sizeof(item) );
	item.mask = TCIF_TEXT;	
	item.pszText = string;	

	// Them 1 item
	TabCtrl_InsertItem( this->getHandle(), nItem, &item );	

	if ( pWindow )
	{
		if ( nItem != 0 )
			pWindow->showWindow(false);
		else
			pWindow->showWindow(true);
	}

	this->selectTab(0);
}

// tabChange
// Su kien chay cung voi onSelChange
void uiTabControl::tabChange()
{
	int nSel = TabCtrl_GetCurSel(this->getHandle());

	uiWindow *pOldWin	= zm_listTabWindows[ zm_nCurrentTab ];
	uiWindow *pWin		= zm_listTabWindows[ nSel ];

	if ( pOldWin && pWin )
		pOldWin->showWindow(false);

	if ( pWin )
		pWin->showWindow(true);
}

// tabChanging
// Su kien chay cung voi onTabChanging;
void uiTabControl::tabChanging()
{
	zm_nCurrentTab = TabCtrl_GetCurSel(this->getHandle());
}

// selectTab 
// Lua chon tab
void uiTabControl::selectTab ( DWORD nIndex )
{
	TabCtrl_SetCurSel( this->getHandle(), nIndex );
}

// deleteTab
// Xoa tab
void uiTabControl::deleteTab ( DWORD nIndex )
{
	if ( zm_listTabString.size() == 0 )
		return;

	if ( nIndex >= (DWORD) zm_listTabString.size() )
		nIndex = (DWORD) (zm_listTabString.size() - 1);

	TabCtrl_DeleteItem( this->getHandle(), nIndex );
	zm_listTabString.erase	( zm_listTabString.begin() + nIndex );
	zm_listTabWindows.erase ( zm_listTabWindows.begin() + nIndex );
}
		
// getTabString
// Lay tieu de tab string
void uiTabControl::getTabString ( DWORD nIndex, LPWSTR lpString )
{
	if ( zm_listTabString.size() == 0 )
		return;

	if ( nIndex >= (DWORD) zm_listTabString.size() )
		nIndex = (DWORD) (zm_listTabString.size() - 1);
	uiString::copy<WCHAR,WCHAR>( lpString, (WCHAR*) zm_listTabString[nIndex].data() );
}

// setTabString 
// Thiet lap string cho tab
void uiTabControl::setTabString ( DWORD nIndex, LPWSTR lpString )
{
	if ( zm_listTabString.size() == 0 )
		return;

	if ( nIndex >= (DWORD) zm_listTabString.size() )
		nIndex = (DWORD) (zm_listTabString.size() - 1);

	zm_listTabString[nIndex] = lpString;

	// Lay chuoi string
	LPWSTR string = (WCHAR*)zm_listTabString[nIndex].data() ;

	TCITEMW item;
	memset( &item, 0, sizeof(item) );
	item.mask = TCIF_TEXT;	
	item.pszText = string;

	TabCtrl_SetItem( this->getHandle(), nIndex, &item );
}

// getSelectTab
// Lay tab dang duoc lua chon hien hanh
DWORD uiTabControl::getSelectTab()
{
	return (DWORD) TabCtrl_GetCurSel( this->getHandle() );
}

// deleteAllTab
// Xoa tat ca cac tab
void uiTabControl::deleteAllTab()
{
	// Xoa tat ca item
	TabCtrl_DeleteAllItems( this->getHandle() );
	
	// Xoa tat ca cac data
	zm_listTabString.clear();
	zm_listTabWindows.clear();
}

// insertTab
// Chen them 1 tab
void uiTabControl::insertTab(  DWORD nIndex, LPWSTR lpTabString, uiWindow* pWindow )
{
	if ( nIndex >= (DWORD) zm_listTabString.size() )
		nIndex = (DWORD) (zm_listTabString.size() - 1);	
	
	if ( zm_listTabString.size() == 0 )
		nIndex = 0;

	// Luu string vao array
	zm_listTabString.insert		(zm_listTabString.begin() + nIndex  ,lpTabString );
	zm_listTabWindows.insert	(zm_listTabWindows.begin() + nIndex ,pWindow );

	if ( pWindow )
	{
		pWindow->setParent(this);
	
		pWindow->setDock( this, UIDOCK_FILL );
		pWindow->setMargin(4,4, 28,4);
		pWindow->showWindow(true);	
	}

	// Lay chuoi string
	LPWSTR string = (WCHAR*)zm_listTabString[nIndex].data() ;

	TCITEMW item;
	memset( &item, 0, sizeof(item) );
	item.mask = TCIF_TEXT;	
	item.pszText = string;	

	// Them 1 item
	TabCtrl_InsertItem( this->getHandle(), nIndex, &item );

}