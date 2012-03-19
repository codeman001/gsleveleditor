// uiStatusBar.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 2/3/2009 
// Thong tin:
//
#include "uiStatusBar.h"
#include "uiInclude.h"
#include "uiApplication.h"
#include <windowsx.h>

using namespace uiClass;

#if !defined( SBT_TOOLTIPS )
	#define SBT_TOOLTIPS            0x0800
#endif

#if !defined( SB_SETICON )
	#define SB_SETICON              (WM_USER+15)
#endif

#if !defined(SB_SETTIPTEXTW)
	#define SB_SETTIPTEXTW          (WM_USER+17)
#endif

uiStatusBar::uiStatusBar(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	this->create( lpTitle, x,y,w,h, pParent );
}

uiStatusBar::~uiStatusBar(void)
{

}

// create
// Khoi tao status bar
bool uiStatusBar::create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
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
		L"msctls_statusbar32",
		lpTitle,
		WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | SBARS_SIZEGRIP | SBT_TOOLTIPS  ,
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
// Lay const doi tuong
DWORD uiStatusBar::getObject()
{
	return UIOBJECT_STATUSBAR;
}

// createParts
// Khoi tao cac thanh phan cua Part
void uiStatusBar::createParts( int nPart, int *size )
{	
	if ( zm_listWindow.size() > 0 )
		zm_listWindow.clear();

	for ( int i = 1; i < nPart; i++ )
	{
		size[i] += size[i-1];	
		zm_listWindow.push_back( NULL );
	}

	SendMessage(this->getHandle(), SB_SETPARTS, (WPARAM) nPart, (LPARAM) size);
}

// setIcon
// Thiet lap icon cho part
void uiStatusBar::setIcon( int part, uiIcon *pIcon )
{
	HICON icon;
	if ( pIcon )
		icon = pIcon->getHandle();
	else
		icon = NULL;

	SendMessage(this->getHandle(), SB_SETICON, (WPARAM) part, (LPARAM) icon);
}

// setText
// Thiet lap text cho part
void uiStatusBar::setText( int part, LPWSTR lpText )
{
	SendMessage(this->getHandle(), SB_SETTEXTW, (WPARAM) part, (LPARAM) lpText);
}

// setToolTip
// Thiet lap tooltip cho part
void uiStatusBar::setToolTip( int part, LPWSTR lpToolTip )
{
	SendMessage(this->getHandle(), SB_SETTIPTEXTW, (WPARAM) part, (LPARAM) lpToolTip);
}

// getRect
// Lay kich thuoc rect
void uiStatusBar::getRect( int part, LPRECT lpRect )
{
	SendMessage(this->getHandle(), SB_GETRECT, (WPARAM) part, (LPARAM) lpRect);
}

// setWindow
// Thiet lap cua so trong part
void uiStatusBar::setWindow( DWORD part, uiWindow *pWin )
{
	if ( part >= zm_listWindow.size() )
		return;
	
	zm_listWindow[ part ] = pWin;

	RECT rect;
	int l,r,t,b,x,y,width,height;
	this->getRect( part, &rect );
	pWin->getMargin( &l,&r,&t,&b );

	x = rect.left + l;
	y = rect.top + t;
	width = rect.right - rect.left - r - l;
	height = rect.bottom - rect.top - b - t;
			
	pWin->setParent( this );
	pWin->moveWindow(x,y,width,height);

}

// _OnSize
// Su kien onSize
void uiStatusBar::_OnSize( uiSizeEvent e, int w, int h )
{
	uiListWindows::iterator i = zm_listWindow.begin();
	DWORD part = 0;
	RECT rect;
	int l,r,t,b,x,y,width,height;

	while ( i != zm_listWindow.end() )
	{
		uiWindow *p = (*i);
		if ( p != NULL )
		{
			// Lay kich thuoc
			this->getRect( part, &rect );
			p->getMargin( &l,&r,&t,&b );

			x = rect.left + l;
			y = rect.top + t;
			width = rect.right - rect.left - r - l;
			height = rect.bottom - rect.top - b - t;
			
			p->setParent( this );
			p->moveWindow(x,y,width,height);
			
		}

		part++;
		i++;
	}

}