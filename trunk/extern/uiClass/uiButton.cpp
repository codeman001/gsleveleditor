// uiButton.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 30/12/2008
// Thong tin:
//

#include "uiButton.h"
#include "uiInclude.h"
#include "uiApplication.h"
#include <windowsx.h>

using namespace uiClass;

uiButton::uiButton(void)
{

}

uiButton::uiButton(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	// Khoi tao button
	this->create( lpTitle, x,y,w,h, pParent );
}

uiButton::~uiButton(void)
{

}

//getObject
// Tra ve UIOBJECT
DWORD uiButton::getObject()
{
	return UIOBJECT_BUTTON;
}


// create
// Khoi tao button
bool uiButton::create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
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
		L"Button",
		lpTitle,
		WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON | WS_VISIBLE | BS_MULTILINE,
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

// messageControlMap
// Phan loai msg cho button
void uiButton::messageControlMap(UINT uMsg)
{
	switch (uMsg)
	{
	case BN_CLICKED:
		eventOnClicked	(this);
		break;
	case BN_DBLCLK:
		eventOnDbClicked(this);
		break;
	case BN_SETFOCUS:
		eventOnFocus	(this);		
		break;
	case BN_KILLFOCUS:
		eventOnLostFocus(this);
		break;
	}
}

// btnSetState
// Thay doi trang thai up/down
void uiButton::setState(bool b)
{
	Button_SetState( this->getHandle(), b );
}
		
// btnGetState
// Lay trang thai
bool uiButton::getState()
{
#pragma warning(disable:4800)
	return (bool) Button_GetState( this->getHandle() );
}
	
// btnSetStyle
// Thay doi Stype button
//	- Tham so la hang so BS_ cua WINAPI
void uiButton::setStyle(DWORD dwBS)
{
	Button_SetStyle(this->getHandle(), dwBS, true);
}