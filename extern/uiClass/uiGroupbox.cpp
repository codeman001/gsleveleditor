#include "uiGroupBox.h"
#include "uiInclude.h"
#include "uiApplication.h"

#include <windowsx.h>

using namespace uiClass;

uiGroupbox::uiGroupbox()
{

}

uiGroupbox::uiGroupbox(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	this->create( lpTitle, x,y,w,h, pParent );
}

uiGroupbox::~uiGroupbox(void)
{

}
		
// getObject
// Lay gia tri UIOBJECT
DWORD uiGroupbox::getObject()
{
	return UIOBJECT_GROUPBOX;
}

// create
// Khoi tao 1
bool uiGroupbox::create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
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
	zm_hWnd = CreateWindowExW( WS_EX_TRANSPARENT,
		L"Button",
		lpTitle,
		WS_CHILD | BS_GROUPBOX | WS_VISIBLE,
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

// Phan loai msg cho control
void uiGroupbox::messageControlMap(UINT uMsg)
{

}