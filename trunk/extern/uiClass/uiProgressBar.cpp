// uiStatic.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 25/1/2009 (30 tet, ngay cuoi nam)
// Thong tin:

#include "uiProgressBar.h"
#include "uiInclude.h"
#include "uiApplication.h"
#include <windowsx.h>

using namespace uiClass;

uiProgressBar::uiProgressBar(LPCWSTR lpTitle, int x, int y, int w, int h, bool isMarque, bool isVertical, uiWindow* pParent)
{
	this->create( lpTitle, x,y,w,h, isMarque, isVertical,pParent );
}

uiProgressBar::~uiProgressBar(void)
{

}
		
// getObject
// Tra ve UIOBJECT
DWORD uiProgressBar::getObject()
{
	return UIOBJECT_PROGRESSBAR;
}

// create
// Khoi tao button
bool uiProgressBar::create(LPCWSTR lpTitle, int x, int y, int w, int h, bool isMarque, bool isVertical, uiWindow* pParent)
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
	
	DWORD style = 0;

#if !defined(PBS_MARQUEE)
	#define PBS_MARQUEE 0x08
#endif

	if ( isMarque == true )
		style |= PBS_MARQUEE;
	if ( isVertical == true )
		style |= PBS_VERTICAL;

#pragma warning(disable:4312)
	//Khoi tao cua so
	zm_hWnd = CreateWindowExW(0,
		L"msctls_progress32",
		lpTitle,
		WS_CHILD | WS_TABSTOP | WS_VISIBLE | style ,
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

// setMarque
// Cho Progress Bar chay
// Luu y: uiProgressBar phai duoc khoi tao isMarque = true
void uiProgressBar::setMarquee(bool b)
{
#if !defined(PBM_SETMARQUEE)
	#define PBM_SETMARQUEE 0x040A
#endif

	SendMessageW( this->getHandle(),	PBM_SETMARQUEE , (WPARAM) b, 0 );
}

// setRange
// Thiet lap pham vi cho Progress Bar
void uiProgressBar::setRange(int min, int max)
{
	SendMessageW( this->getHandle(), PBM_SETRANGE, 0, MAKELPARAM(min,max) );
}

// setPos
// Thiet lap vi tri
void uiProgressBar::setPos(int pos)
{
	SendMessageW( this->getHandle(), PBM_SETPOS, pos, 0 );
}

// getPos
// Tra ve vi tri hien tai
int uiProgressBar::getPos()
{
	return (int) SendMessageW( this->getHandle(), PBM_GETPOS, 0,0 );
}