// uiCheckbox.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 30/12/2008
// Thong tin:
//

#include "uiCheckbox.h"
#include "uiInclude.h"
#include "uiApplication.h"
#include <windowsx.h>

using namespace uiClass;

uiCheckBox::uiCheckBox(void)
{

}

uiCheckBox::~uiCheckBox(void)
{

}

uiCheckBox::uiCheckBox(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	this->create(lpTitle, x,y,w,h, pParent);
}

// create
// Khoi tao uiCheckbox
bool uiCheckBox::create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
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
		WS_CHILD | WS_TABSTOP | BS_CHECKBOX | WS_VISIBLE | BS_MULTILINE,
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
// Phan loai MSGs
void uiCheckBox::messageControlMap(UINT uMsg)
{
	switch (uMsg)
	{
	case BN_CLICKED:
		this->setCheck( !this->getCheck() );
		eventOnClicked(this);
		break;
	case BN_DBLCLK:
		eventOnDbClicked(this);
		break;
	case BN_SETFOCUS:
		eventOnFocus(this);
		break;
	case BN_KILLFOCUS:
		eventOnLostFocus(this);
		break;
	}
}
		
// getObject
// Tra ve UIOBJECT
DWORD uiCheckBox::getObject()
{
	return UIOBJECT_CHKBOX;
}

// chkSetCheck
// Set gia tri cho checkbox
void uiCheckBox::setCheck(bool b)
{
	Button_SetCheck( this->getHandle(), b);
}

// chkGetCheck
// Lay gia tri checkbox
bool uiCheckBox::getCheck()
{
	#pragma warning(disable:4800)
	return (bool) Button_GetCheck( this->getHandle() );
}