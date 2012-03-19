// uiWindowFunc.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 15/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"
#include "uiApplication.h"

using namespace uiClass;


// getObject
// Tra ve const UIOBJECT_WINDOW
DWORD uiWindow::getObject()
{
	return UIOBJECT_WINDOW;
}

// getHandle
// Lay gia tri HWND cua cua so
HWND uiWindow::getHandle()
{
	return this->zm_hWnd;
}

// getInstance
// Lay gia tri HINSTANCE
HINSTANCE uiWindow::getInstance()
{
	return this->zm_hInstance;
}

// gethParent
// Lay doi tuong cua so cha
uiWindow* uiWindow::getParent()
{
	return this->zm_pParent;
}

// changeStyle
// Thay doi kieu dang cua cua so
// Tham so:	- Style them vao
//			- Style loai bo
void uiWindow::changeStyle	(LONG lStyleAdd,LONG lStyleRemove)
{
	// Lay style hien tai
	LONG lStyle = GetWindowLong(this->getHandle() , GWL_STYLE);	

	// Them va loai bo style
	lStyle &= ~lStyleRemove;
	lStyle |= lStyleAdd;

	// Thiet lap style moi
	SetWindowLong(this->getHandle(), GWL_STYLE, lStyle);  

	UpdateWindow(this->getHandle());	
}

// changeStyleEx
// Thay doi kieu dang mot so ex mo rong cua cua so
// Tham so:	- Style them vao
//			- Style loai bo
void uiWindow::changeStyleEx(LONG lStyleAdd,LONG lStyleRemove)
{
	// Lay style hien tai
	LONG lStyle = GetWindowLong(this->getHandle() , GWL_EXSTYLE);	

	// Them va loai bo style
	lStyle &= ~lStyleRemove;
	lStyle |= lStyleAdd;

	// Thiet lap style moi
	SetWindowLong(this->getHandle(), GWL_EXSTYLE, lStyle);  

	UpdateWindow(this->getHandle());
}

// setParent
// Thay doi cua so cha
void uiWindow::setParent(uiWindow *pWin)
{
	HWND hWnd = NULL;
	
	if (pWin)
	{
		hWnd = pWin->getHandle();		
		zm_pParentInclude = pWin;
	}
	else
		zm_pParentInclude = NULL;

	// Thiet lap parent
	SetParent(this->getHandle(),hWnd);
}

// addChild
// Them mot cua so con
void uiWindow::addChild(uiWindow *pChild)
{
	assert(pChild);
	zm_aChild.push_back(pChild);
}

// removeChild
// Loai bo cua so con
void uiWindow::removeChild(uiWindow* pChild)
{
	assert(pChild);
	vector<uiWindow*>::iterator i;
	
	// Duyet danh sach vector va tim cua so pCHild can xoa
	for (i = zm_aChild.begin(); i != zm_aChild.end(); i++ )
	{
		if ( *i == pChild )
		{
			zm_aChild.erase(i);	
			return;
		}
	}
}

// invalidateRect
// Cap nhat cua so 			
void uiWindow::invalidateRect(RECT *r, bool bRePaint)
{
	InvalidateRect(this->getHandle(), r, bRePaint);
}
			
// validateRect
// Cap nhat cua so 
void uiWindow::validateRect(RECT *r)
{
	ValidateRect(this->getHandle(), r);
}
			
// setForegroundWindow
// Active cua so
void uiWindow::setForegroundWindow()
{
	SetForegroundWindow( this->getHandle() );
}

// enableWindow
// Bat chuc nang cua so
void uiWindow::enableWindow(bool b)
{
	EnableWindow( this->getHandle(), b);
}

// showWindow
// Hien/An cua so
void uiWindow::showWindow(bool b)
{
	if (b)
		ShowWindow( this->getHandle(), SW_SHOW );
	else
		ShowWindow( this->getHandle(), SW_HIDE );
}
			
// updateWindow
// Cap nhat cua so
void uiWindow::updateWindow()
{
	UpdateWindow( this->getHandle() );
}
			
// setWindowState
// Thay doi trang thai cua so theo hang so SW_ (WINAPI)
void uiWindow::setWindowState(UINT nState)
{
	ShowWindow( this->getHandle(), nState );
}

// setLeftTop
// Thiet lap vi tri cua so (so voi cua so cha)
void uiWindow::setLeftTop(int x, int y)
{
	int w,h;
	
	this->getWidthHeight(&w,&h);
	MoveWindow(this->getHandle(), x,y,w,h, true);
}

// getLeftTop
// Lay vi tri cua cua so (so voi cua so cha)
void uiWindow::getLeftTop(int* x,int* y)
{
	RECT size;	
	GetWindowRect(this->getHandle(),&size);		
	
	// Neu co cua so cha
	if ( zm_pParentInclude )
	{
		POINT point={0,0};	
		ClientToScreen( zm_pParentInclude->getHandle(), &point);	
		*x = size.left - point.x;
		*y = size.top - point.y;
	}
	// Ko co cua so cha
	else
	{
		*x = size.left;
		*y = size.top;
	}
}
			
// getTop
// Lay toa do Y cua cua so (so voi cua so cha)
int uiWindow::getTop()
{
	int x,y;
	this->getLeftTop(&x,&y);
	return y;	
}

// getLeft
// Lay toa do X cua cua so (so voi cua so cha)
int uiWindow::getLeft()
{
	int x,y;
	this->getLeftTop(&x,&y);
	return x;	
}

// moveWindow
// Di chuyen cua so
void uiWindow::moveWindow(int x, int y, int w, int h)
{
	MoveWindow( this->getHandle(), x,y,w,h, true );
}

// setWidthHeight
// Thiet lap chieu rong va chieu cua cua so
void uiWindow::setWidthHeight(int nWidth, int nHeight)
{
	int x,y;
	
	this->getLeftTop(&x,&y);
	MoveWindow(this->getHandle(), x,y,nWidth,nHeight, true);
}

// getWidthHeight
// Lay chieu rong va cao cua cua so
void uiWindow::getWidthHeight(int* nWidth, int* nHeight)
{
	RECT size;	
	GetWindowRect(this->getHandle(),&size);

	*nWidth =  size.right - size.left;
	*nHeight = size.bottom - size.top;
}

// getWidth
// Lay chieu rong
int uiWindow::getWidth()
{
	RECT size;	
	GetWindowRect(this->getHandle(),&size);
	return (size.right - size.left);
}

// getHeight
// Lay chieu cao
int uiWindow::getHeight()
{
	RECT size;	
	GetWindowRect(this->getHandle(),&size);	
	return (size.bottom - size.top);
}

// getPointToScreen
// Chuyen doi 1 point tu toa do cua so -> toa do man hinh
void uiWindow::getPointToScreen(int *x, int *y)
{
	POINT p = {*x,*y};
	ClientToScreen(this->getHandle(),&p);
	*x = p.x;
	*y = p.y;
}

// getPointToScreen
// Chuyen doi 1 point tu toa do man hinh -> toa do cua so
void uiWindow::getPointFromScreen(int *x, int *y)
{
	POINT p = {*x,*y};
	ScreenToClient(this->getHandle(),&p);
	*x = p.x;
	*y = p.y;
}

// setCapture
// Hook chuot khi no di chuyen ra ngoai cua so
void uiWindow::setCapture()
{
	SetCapture( this->getHandle() );
}

// releaseCapture
// unHook do ham setCapture()
void uiWindow::releaseCapture()
{
	ReleaseCapture();
}
			
// getClientWH
// Lay chieu rong + cao cua vung client
void uiWindow::getClientWH(int* nWidth, int* nHeight)
{	
	RECT size;	
	GetClientRect(this->getHandle(),&size);

	*nWidth =  size.right - size.left;
	*nHeight = size.bottom - size.top;
}

// setClientWH
// Thiet lap chieu rong + cao
void uiWindow::setClientWH(int w, int h)
{
	int nw,nh,cw,ch,cx,cy;	
	bool bMin = false;

	// Lay kich thuoc cua so that
	this->getWidthHeight(&nw,&nh);

	// Lay kich thuoc client
	this->getClientWH(&cw,&ch);
	
	// Neu cua so hien tai qua nho -> set size = 50 de lay duoc size client
	if (cw == 0) 
	{
		// Neu cua so co chieu rong = 0
		this->setWidthHeight( 50, this->getHeight() );
		bMin = true;
	}
	if (ch == 0)
	{
		// Neu cua so co chieu cao = 0
		this->setWidthHeight( this->getWidth(), 50 );
		bMin = true;
	}
	
	// Neu cua so qua nho (cw = 0 va cy = 0) -> lay lai kich thuoc
	if (bMin)
	{
		this->getWidthHeight(&nw,&nh);
		this->getClientWH(&cw,&ch);
	}

	// Tinh width va height moi
	cx = nw - cw + w;
	cy = nh - ch + h;


	// Thiet lap kich thuoc
	this->setWidthHeight( cx, cy);
}

// getClientWidth
// Lay chieu rong cua client
int uiWindow::getClientWidth()
{
	RECT size;	
	GetClientRect( this->getHandle(),&size);

	return (size.right - size.left);
}

// getClientHeight
// Lay chieu cao cua client
int uiWindow::getClientHeight()
{
	RECT size;	
	GetClientRect( this->getHandle(),&size);

	return (size.bottom - size.top);
}


// setCaption
// Thiet lap tieu de
void uiWindow::setCaption(LPWSTR lpCaption)
{
	SetWindowTextW(this->getHandle(), lpCaption);
}

// getCaption
// Lay tieu de cua cua so
void uiWindow::getCaption(LPWSTR lpCaption)
{
	GetWindowTextW(this->getHandle(),lpCaption,200);
}

// setMargin
// Thiet lap le cho cua so
void uiWindow::setMargin(int l, int r, int t, int b)
{
	zm_margin.nLeft	= l;
	zm_margin.nRight	= r;
	zm_margin.nBottom = t;
	zm_margin.nTop	= b;
}

// getMargin
// Lay gia tri le da thiet lap
void uiWindow::getMargin(int *l, int *r, int *t, int *b)
{
	*l = zm_margin.nLeft;
	*r = zm_margin.nRight;
	*t = zm_margin.nBottom;
	*b = zm_margin.nTop;
}		

// showScrollBarVer
// Hien thi scrollBar doc
void uiWindow::showScrollBarVer(bool b)
{
	ShowScrollBar(this->getHandle(), SB_VERT, b);
}

// showScrollBarHorz
// Hien thi scrollBar ngang
void uiWindow::showScrollBarHorz(bool b)
{
	ShowScrollBar(this->getHandle(), SB_HORZ, b);
}

// showScrollBar
// Hien thi scrollBar
void uiWindow::showScrollBar(bool b)
{
	ShowScrollBar(this->getHandle(), SB_BOTH, b);
}


// setScrollRange
// Thiet lap mien gia tri cho scroll bar
// scroll = SB_HORZ
// scroll = SB_VERT
void uiWindow::setScrollRange( int scroll, int min, int max )
{
	SetScrollRange( this->getHandle(), scroll, min, max, true );
}

// getScrollRange
// Lay gia tri scroll
// scroll = SB_HORZ
// scroll = SB_VERT
void uiWindow::getScrollRange( int scroll, int *min, int *max )
{
	GetScrollRange( this->getHandle(), scroll, min, max );
}

// setScrollPos
// Thiet lap vi tri pos
void uiWindow::setScrollPos( int scroll, int pos )
{
	SetScrollPos( this->getHandle(), scroll, pos, true );
}

// getScrollPos
// Lay gia tri pos
void uiWindow::getScrollPos( int scroll, int *pos )
{
	*pos = GetScrollPos( this->getHandle(), scroll );
}

// enableScrollBar
// Bat tat su dung scroll
// scroll = SB_BOTH
// scroll = SB_HORZ
// scroll = SB_VERT
void uiWindow::enableScrollBar( int scroll, bool enable )
{
	if ( enable )
		EnableScrollBar( this->getHandle(), scroll, ESB_ENABLE_BOTH );
	else
		EnableScrollBar( this->getHandle(), scroll, ESB_DISABLE_BOTH );
}


// showMaximizeButton
// Hien thi nut Maximum
void uiWindow::showMaximizeButton(bool b)
{
	if (b)
		this->changeStyle(0,WS_MAXIMIZEBOX);		
	else
		this->changeStyle(WS_MAXIMIZEBOX,0);

	this->rePaint();
}
			
// showMinimumButton
// Hien thi nut Minimum
void uiWindow::showMinimumButton(bool b)
{
	if (b)
		this->changeStyle(0,WS_MINIMIZEBOX);		
	else
		this->changeStyle(WS_MINIMIZEBOX,0);

	this->rePaint();
}
			
// showCaption
// Hien thi thanh caption
void uiWindow::showCaption(bool b)
{
	if (b)
		this->changeStyle(0,WS_CAPTION);		
	else
		this->changeStyle(WS_CAPTION,0);

	this->rePaint();
}
			
// showBorder
// Hien thi border
void uiWindow::showBorder(bool b)
{
	if (b)
		this->changeStyle(0,WS_BORDER);		
	else
		this->changeStyle(WS_BORDER,0);

	this->rePaint();
}
			
// setOpacity
// Thiet lap do trong suot
void uiWindow::setOpacity(BYTE nOpacity)
{
#ifndef WS_EX_LAYERED
	#define WS_EX_LAYERED 0x80000	
	#define LWA_COLORKEY 1 
	#define LWA_ALPHA 2 
#endif

	this->changeStyle(WS_EX_LAYERED,0);

	HMODULE user32dll = LoadLibraryW(L"user32.dll");
	
	// Lay function SetLayeredWindowAttributes trong user32dll
	typedef DWORD (WINAPI *PSLWA)(HWND, DWORD, BYTE, DWORD);
	PSLWA pFunc = (PSLWA)GetProcAddress(user32dll,"SetLayeredWindowAttributes");


	if (pFunc != NULL)
	{
		pFunc(this->getHandle(), RGB(255,255,255), nOpacity, LWA_ALPHA);
	}
	
	FreeLibrary(user32dll);
}

// setForcus
// Thiet lap active cua so nay
void uiWindow::setFocus()
{
	SetFocus( this->getHandle() );
}
			
// changeWindowStyle
// Thay doi kieu cua cua so (nStyle la hang so UISTYLE_)
void uiWindow::changeWindowStyle(int nStyle)
{
	// Cac style dinh nghia truoc
	LONG lStylePopup	= WS_POPUP;
	LONG lStyleNoResize = WS_CAPTION| WS_SYSMENU | WS_CLIPCHILDREN;
	LONG lStyleResize	= WS_OVERLAPPEDWINDOW;
	LONG lStyleTool		= WS_CAPTION | WS_THICKFRAME | WS_SYSMENU |WS_CLIPCHILDREN | WS_POPUP;
	LONG lStyleToolEx	= WS_EX_TOOLWINDOW|WS_EX_DLGMODALFRAME|WS_EX_NOPARENTNOTIFY;
	LONG lStyleChild	= WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	// Loai bo style
	switch (zm_nStyle)
	{
	case UISTYLE_POPUP:
		this->changeStyle(0,lStylePopup);
		break;
	case UISTYLE_NORESIZE:
		this->changeStyle(0,lStyleNoResize);
		break;
	case UISTYLE_RESIZE:
		this->changeStyle(0,lStyleResize);
		break;
	case UISTYLE_TOOL:
		this->changeStyle(0,lStyleTool);
		this->changeStyleEx(0,lStyleToolEx);
		break;
	case UISTYLE_CHILD:
		this->changeStyle(0,lStyleChild);
		break;
	}

	// Thay doi style
	zm_nStyle = nStyle;
	switch (nStyle)
	{
	case UISTYLE_POPUP:
		this->changeStyle(lStylePopup,0);
		break;
	case UISTYLE_NORESIZE:
		this->changeStyle(lStyleNoResize,0);
		break;
	case UISTYLE_RESIZE:		
		this->changeStyle(lStyleResize,0);
		break;
	case UISTYLE_TOOL:
		this->changeStyle(lStyleTool,0);
		this->changeStyleEx(lStyleToolEx,0);
		break;
	case UISTYLE_CHILD:
		this->changeStyle(lStyleChild,0);
		break;
	}

	this->setParent( zm_pParentInclude );
}
			
// closeWindow
// Dong cua so
void uiWindow::closeWindow()
{
	SendMessageA(this->getHandle(), WM_CLOSE,0,0);
}
			
// rePaint
// Ve lai cua so
void uiWindow::rePaint()
{	
	int w,h;
	this->getWidthHeight(&w,&h);
	this->setWidthHeight(w+1,h+1);
	this->setWidthHeight(w,h);
}


// setDock
// Dock vao cua so cha
// Tham so:	- Dia chi cua so cha
//			- Const Dock (UIDOCK_ )
void uiWindow::setDock(uiWindow* pParentWin, BYTE nDock)
{
	this->setParent( pParentWin );	
	pParentWin->zm_pDockCtl->dock(this);
	zm_byteDock = nDock;
}

// updateDock
// Cap nhat lai kich thuoc cac cua so da Dock
void uiWindow::updateDock()
{
	
	zm_pDockCtl->update();
}

// getDock
// Lay const UIDOCK
BYTE uiWindow::getDock()
{
	return zm_byteDock;
}
			
// unDock
// Thoat khoi dock cua so cha
void uiWindow::unDock()
{
	if ( zm_byteDock )
	{
		if ( zm_pParentInclude )
		{
			zm_pParentInclude->zm_pDockCtl->unDock( this );
			zm_byteDock = 0;
		}
	}
}

// bringTop
// Dem windows ra dang truoc tat cac cac cua so
void uiWindow::bringTop()
{
	BringWindowToTop( this->getHandle() );
}

// doModal
// Hien thi 1 Dialog
void uiWindow::doModal(uiDialog *pDialog)
{	
	assert(pDialog);	
	
	this->enableWindow(false);

	pDialog->showWindow(true);
	
	// Chay dialog!!
	uiApplication::run( pDialog );
	
	this->enableWindow(true);

	this->setForegroundWindow();
}

// setMenu
// Thiet lap menu cho cua so
void uiWindow::setMenu( uiMenu *pMenu )
{
	assert(pMenu);
	
	SetMenu( this->getHandle(), pMenu->getHandle() );	
}

// isEnable
// Kiem tra cua so co enable hay khong
bool uiWindow::isEnable()
{
#pragma warning (disable:4800)
	return (bool)IsWindowEnabled( this->getHandle() );
}

// isShow
// Kiem tra cua so Hiden hay Show
bool uiWindow::isShow()
{
#pragma warning (disable:4800)
	return (bool)IsWindowVisible( this->getHandle() );
}

// setTabIndex
// Thiet lap vi tri tab 
//		- i < 0: Control khong forus khi nhan Tab
void uiWindow::setTabIndex( int i )
{
	zm_nTabIndex = i;
}
			
// getTabIndex
// Lay gia tri TabIndex
int uiWindow::getTabIndex()
{
	return zm_nTabIndex;
}

// setIcon
// Thiet lap icon cho cua so
void uiWindow::setIcon(uiIcon *pIcon, bool bLargeIcon)
{
	SendMessage( this->getHandle(), WM_SETICON, bLargeIcon?ICON_BIG:ICON_SMALL, (LPARAM) pIcon->getHandle() );
}

// setCursor
// Thiet lap cursor cho cua so
void uiWindow::setCursor( uiCursor *pCursor)
{
	#pragma warning(disable: 4311)
	SetClassLong(this->getHandle(),GCL_HCURSOR, (LONG) pCursor->getHandle() );
	#pragma warning(default: 4311)
}

// setFont
// Thiet lap font chu cho cua so
void uiWindow::setFont( uiFont *font )
{
	SendMessage(zm_hWnd, WM_SETFONT, (WPARAM) font->getSafeObject(),
               MAKELPARAM(false, 0));
}

// alert
// Hien thi 1 thong bao msgBox
void uiWindow::alert( LPWSTR strMessage, LPWSTR caption, DWORD dwFlags )
{
	MessageBoxW( this->getHandle(), strMessage, caption, dwFlags );
}

// question
// Hien thi 1 thong bao yes no
bool uiWindow::question( LPWSTR lpString, LPWSTR caption, DWORD dwFlags )
{
	int ret = MessageBoxW( this->getHandle(), lpString, caption, MB_YESNO | MB_ICONQUESTION | dwFlags );

	if ( ret == IDYES )
		return true;

	return false;
}

// setLockLayout
// Thiet lap lockLayout
void uiWindow::setLockLayout( uiWindow* pWin, DWORD dwLock, uiMargin m )
{
	zm_pParentInclude = pWin;
	pWin->zm_pLockLayoutCtl->lockLayout( this );

	zm_lockLayout = m;
	zm_dwLockLayout = dwLock;
}

void uiWindow::setLockLayout( uiWindow* pWin, DWORD dwLock, int l, int t, int r, int b )
{
	zm_pParentInclude = pWin;
	pWin->zm_pLockLayoutCtl->lockLayout( this );

	zm_dwLockLayout = dwLock;
	zm_lockLayout.nLeft = l;
	zm_lockLayout.nTop = t;
	zm_lockLayout.nRight = r;
	zm_lockLayout.nBottom = b;
}

// unLockLayout
// Huy bo layout da lock
void uiWindow::unLockLayout()
{
	zm_pParentInclude->zm_pLockLayoutCtl->unlockLayout( this );
}

// getLockLayout
// Lay trang thai lockLayout
DWORD uiWindow::getLockLayout( uiMargin *m )
{
	*m = zm_lockLayout;
	return zm_dwLockLayout;
}

// updateLockLayout
// Cap nhat LockLayout
void uiWindow::updateLockLayout()
{
	zm_pLockLayoutCtl->update();
}

// enableReDraw
// Cho phep ve lai hay ko 
void uiWindow::enableReDraw( bool b )
{
	zm_bReDraw = b;
}

// setReDrawColor
// Thiet lap mau sau cho nen windows
void uiWindow::setReDrawColor( uiColor b )
{
	zm_drawColor = b;
}

// setTabController
// Thiet lap doi tuong tabController
void uiWindow::setTabController( uiTabController* pTabController )
{
	zm_tabController = pTabController;
}

// getTabController
// Lay doi tuong TabController
uiTabController* uiWindow::getTabController()
{
	return zm_tabController;
}