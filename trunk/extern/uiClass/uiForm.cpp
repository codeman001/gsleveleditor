// uiForm.cpp.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 16/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"
#include "uiString.h"
#include "uiApplication.h"

using namespace uiClass;

// registerWindow
// Dang ky cua so form
bool uiForm::registerWindow(LPWSTR lpNameApp, HINSTANCE hInst)
{
	WNDCLASSEXW wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = uiWindow::windowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject (LTGRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = lpNameApp;
	wndclass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
	
	if (! RegisterClassExW(&wndclass))
		return false;

	return true;
}
			
// messageMap
// Phan tich su kien cho uiForm
LRESULT uiForm::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY)
	{
		PostQuitMessage(1);			
		return (LRESULT)1;
	}
	return uiWindow::messageMap(hWnd, uMsg, wParam, lParam);
}		

			
// uiForm
// Constructor khoi tao form
uiForm::uiForm(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, LPWSTR lpName)
{	
	this->create( lpTitle, x,y,w,h , pParent, lpName );
}

uiForm::~uiForm(void)
{
	if ( zm_pKeyAccel )
		delete zm_pKeyAccel;
}

uiForm::uiForm()
{
	zm_pKeyAccel = NULL;
}

// create
// Khoi tao cua so
int uiForm::create(LPWSTR lpTitle, int x, int y, int w, int h,uiWindow* pParent, LPWSTR lpName)
{
	zm_bAllowIdle = false;
	zm_pKeyAccel = NULL;

	HINSTANCE hInst = GetModuleHandle(0);

	// Dang ky cua so
	if ( !registerWindow(lpName, hInst) )
		return false;
	
	uiApplication::setAppName(lpName);

	// Thiet lap cua so cha
	HWND hParent;
	if (pParent == NULL)
		hParent = NULL;
	else
		hParent = pParent->getHandle();
	
	zm_pParent = pParent;
	this->zm_hInstance = hInst;

	// Gan dia chi this vao createStruct
	MDICREATESTRUCT MyStruct;
	memset(&MyStruct,0,sizeof(MDICREATESTRUCT));
	MyStruct.lParam = (LPARAM) this;
	

	// Khoi tao cua so
	zm_hWnd = CreateWindowExW(0,
		lpName,
		lpTitle,
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		x,y,w,h,
		hParent,
		0,hInst,&MyStruct);
	
	return (zm_hWnd != NULL) ;
}
			
// Idle 
void uiForm::enableOnIdle(bool b)
{
	zm_bAllowIdle = b;
}

// isEnableOnIdle	
bool uiForm::isEnableOnIdle()
{
	return zm_bAllowIdle;
}

// _OnIdle
// Su kien idle luon luon chay khi enableOnIdle.
void uiForm::_OnIdle(){ }

// getObject
// Tra ve const UIOBJECT_FORM
DWORD uiForm::getObject()
{
	return UIOBJECT_FORM;
}

// getKeyAccelTable
// Lay bang hotKey
uiKeyAccel* uiForm::getKeyAccelTable()
{
	return zm_pKeyAccel;
}

// createAccelTable
// Tao bang Accel
void uiForm::createAccelTable( uiKeyAccelEntry *p )
{
	if ( zm_pKeyAccel )
		delete zm_pKeyAccel;

	zm_pKeyAccel = new uiKeyAccel( p );
}

// setPositionCenterOfScreen
// Cho phep hien thi ngay chinh giua man hinh
void uiForm::setPositionCenterOfScreen()
{
	RECT rc, rw;
	GetWindowRect( GetDesktopWindow(), &rc );
	GetWindowRect( this->getHandle(), & rw );

	int dialogW = rw.right - rw.left;
	int dialogH = rw.bottom - rw.top;
	int dialogX = (rc.right - rc.left - dialogW)/2;
	int dialogY = (rc.bottom - rc.top - dialogH)/2;

	this->moveWindow( dialogX, dialogY, dialogW, dialogH );
}