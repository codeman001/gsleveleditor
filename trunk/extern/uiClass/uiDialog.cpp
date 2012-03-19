// uiForm.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 17/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"
#include "uiString.h"
#include "uiApplication.h"

using namespace uiClass;

uiDialog::uiDialog(LPWSTR lpTitle, int x, int y, int w, int h,uiWindow* pParent)	
{
	this->constructor(pParent);
	this->create(lpTitle, x,y,w,h, pParent);
}

// constructor
// Mot so khoi tao mac dinh ban dau
void uiDialog::constructor(uiWindow* pParent)
{
	
}

uiDialog::uiDialog()
{
	this->constructor(NULL);
}

uiDialog::~uiDialog(void)
{

}
		
// create
// Khoi tao dialog
bool uiDialog::create(LPWSTR lpTitle, int x, int y, int w, int h,uiWindow* pParent)
{
	WCHAR lpName[200];

	// Phat sinh ten tu dong	
	static int n=0;	
	WCHAR lpAppName[400];
	uiApplication::getAppName(lpAppName);
	uiString::format<WCHAR> (lpName, L"%s_Dialog_%d",lpAppName, n);
	n++;
	
	HINSTANCE hInst = pParent->getInstance();
	
	if ( !this->registerWindow(lpName, hInst) )
		return false;
	
	HWND hParent;
	if (pParent == NULL)
		hParent = NULL;
	else
		hParent = pParent->getHandle();
	
	zm_pParent = pParent;
	this->zm_hInstance = hInst;
	
	MDICREATESTRUCT MyStruct;
	memset(&MyStruct,0,sizeof(MDICREATESTRUCT));
	MyStruct.lParam = (LPARAM) this;	

	zm_hWnd = CreateWindowExW(
		WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
		lpName,
		lpTitle,
		WS_POPUP | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_SYSMENU,
		x,y,w,h,
		hParent,
		0,hInst,&MyStruct);

	return true;
}

// messageMap
// Phan tich su kien
LRESULT uiDialog::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CLOSE)
	{
		// enable cua so cha
		zm_pParent->enableWindow(true);
	}

	return uiForm::messageMap(hWnd, uMsg, wParam, lParam);
}

// getObject
// Tra ve const UIOBJECT_DIALOG
DWORD uiDialog::getObject()
{
	return UIOBJECT_DIALOG;
}