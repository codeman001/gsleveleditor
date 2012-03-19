// uiStatic.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 25/1/2009 (30 tet, ngay cuoi nam)
// Thong tin:
//

#include "uiStatic.h"
#include "uiInclude.h"
#include "uiApplication.h"
#include <windowsx.h>

using namespace uiClass;

uiStatic::uiStatic(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	this->create( lpTitle, x,y,w,h , pParent );
}

uiStatic::~uiStatic(void)
{

}
		
// getObject
// Tra ve UIOBJECT
DWORD uiStatic::getObject()
{
	return UIOBJECT_STATIC;
}

// create
// Khoi tao button
bool uiStatic::create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
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
	zm_hWnd = CreateWindowExW(0,
		L"Static",
		lpTitle,
		WS_CHILD | WS_TABSTOP | WS_VISIBLE ,
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