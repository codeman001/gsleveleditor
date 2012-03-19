// uiWindowEvent.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 15/12/2008
// Thong tin:

#include "uiWindow.h"
#include "uiInclude.h"
#include "uiString.h"
#include "uiApplication.h"

#include <iostream>

using std::vector;

using namespace uiClass;

// global event window
list<uiWindow*>* uiWindow::sm_globalEventWindow = NULL;

uiWindow::uiWindow()
{
	this->constructor();
}

// constructor
// Mot so khoi tao mac dinh ban dau cho uiWindow
void uiWindow::constructor()
{
	// Cua so
	zm_hWnd		= NULL;
	zm_pParent	= NULL;	

	zm_pParentInclude	= NULL;
	zm_nStyle			= UISTYLE_POPUP;			
	
	// Dock
	zm_pDockCtl = new uiDock( this );
	zm_pLockLayoutCtl = new uiLockLayout( this );

	zm_byteDock = 0;	
	zm_dwLockLayout = 0;

	// TabIndex
	static int s_index = 1;
	zm_nTabIndex	= s_index;
	s_index++;
	zm_tabController = NULL;


	zm_bInit		= false;

	// Margin = 0
	memset( (void*) &zm_margin, 0, sizeof(uiMargin) );
	memset( (void*) &zm_lockLayout, 0, sizeof(uiMargin) );

	zm_drawColor = uiColor::fromSystem( COLOR_3DFACE );
	zm_bReDraw = true;
}

uiWindow::uiWindow(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	this->constructor();

	// Khoi tao cua so
	this->create( lpTitle, x,y,w,h, pParent );	
}

uiWindow::~uiWindow(void)
{	
	delete zm_pDockCtl;
	delete zm_pLockLayoutCtl;
	DestroyWindow( this->getHandle() );
}


// Create khoi tao cua so
// Khoi tao cua so
bool uiWindow::create(LPWSTR lpTitle, int x, int y, int w, int h,uiWindow* pParent)
{
	// static int de tu dong phat sinh ten
	static int n=0;	
	
	// Khoi tao ten
	WCHAR lpName[200];
	WCHAR lpAppName[400];

	uiApplication::getAppName(lpAppName);
	uiString::format<WCHAR>( lpName, L"%s_win_%d",lpAppName,n);
	n++;
	
	// Instance cua ung dung
	HINSTANCE hInst = pParent->getInstance();

	// Dang ky cua so
	int r = this->registerWindow(lpName,hInst);
	assert(r);
	
	// Cua so cha
	HWND hParent;
	if (pParent == NULL)
		hParent = NULL;
	else
		hParent = pParent->getHandle();		
	
	zm_pParent = pParent;
		
	// Gia tri instance
	this->zm_hInstance = hInst;

	//Gan di chia (this) cua minh vao STRUCT
	MDICREATESTRUCT MyStruct;
	memset(&MyStruct,0,sizeof(MDICREATESTRUCT));
	MyStruct.lParam = (LPARAM) this;

	//Khoi tao cua so
	zm_hWnd = CreateWindowExW(0,
		lpName,
		lpTitle,
		WS_POPUP|WS_CLIPCHILDREN|WS_TABSTOP,
		x,y,w,h,
		hParent,
		0,hInst,&MyStruct);	
	assert(zm_hWnd);

	// Them chinh no vao cua so cha
	pParent->addChild(this);

	// Thiet lap cua so cha
	this->setParent(pParent);	

	return true;
}


// registerWindow
// Dang ky cua so
bool uiWindow::registerWindow(LPWSTR lpName, HINSTANCE hInst)
{
	WNDCLASSEXW wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = 0;
	wndclass.lpfnWndProc = uiWindow::windowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject (COLOR_WINDOW);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = lpName;
	wndclass.hIconSm = NULL;
	
	// Dang ky cua so
	if (! RegisterClassExW(&wndclass) )	
		return false;
	
	return true;
}
			
// windowProc
// Xu ly cac su kien cho chuong trinh
LRESULT CALLBACK uiWindow::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#pragma warning(disable: 4311)
#pragma warning(disable: 4312)

	uiWindow *pWin = NULL;

	pWin = (uiWindow*) ( GetWindowLong(hWnd,GWL_USERDATA) );

	// Chuyen event cho cua so khac
	if ( sm_globalEventWindow )
	{
		list<uiWindow*>::iterator iWin = sm_globalEventWindow->begin(), end = sm_globalEventWindow->end();
		while ( iWin != end )
		{
			if ( (*iWin)->globalMessageMap( hWnd, uMsg, wParam, lParam ) == false )
			{
				// break all event to another window
				return DefWindowProc(hWnd,uMsg,wParam,lParam);
			}
			iWin++;
		}
	}

	// Neu cua so nay duoc khoi tao
	if (pWin){

		LRESULT r = pWin->messageMap(hWnd,uMsg,wParam,lParam);		
		
		if (r < 0)
			return DefWindowProc(hWnd,uMsg,wParam,lParam);
		else
			return r;
	}
	// Neu cua so nay chua duoc khoi tao
	else
	{		
		if (uMsg == WM_NCCREATE)
		{
			
			MDICREATESTRUCT *pMyStruct = (MDICREATESTRUCT*) ((LPCREATESTRUCT) lParam)->lpCreateParams;	
			pWin = (uiWindow*) (pMyStruct->lParam);
			
			// Dua pointer vao cua so
			SetWindowLong(hWnd,GWL_USERDATA, (long)(pWin));
			return DefWindowProc(hWnd, uMsg, wParam , lParam);
		}
		else
		{
			return DefWindowProc(hWnd, uMsg, wParam , lParam);			
		}
	
	}

#pragma warning(default: 4311)	
#pragma warning(default: 4312)	
}
			
// messageMap
// Phan loai message
LRESULT uiWindow::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Toa do chuot Mouse 
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);	

	// Neu toa do am: x = 0xFF0F -> ~x = 0x00F0
	if ( (x & 0x8000) >> 15 == 0x1)
			x = - ((~x) & 0x0000FFFF);
	if ( (y & 0x8000) >> 15 == 0x1)
			y = - ((~y) & 0x0000FFFF);
	
	// Danh cho nhung su kien can tham so tra ve
	bool bBreak;
		
	PAINTSTRUCT PaintStruct;
	HDC hDC;	
	uiGraphics *pG;

	// Truoc su kien WM_NCCREATE thi gan hWnd
	if (this->zm_hWnd == NULL)
		this->zm_hWnd = hWnd;
	
	// Phan loai message
	switch ( uMsg )
	{

		//Cac message lien quan den chuot
		case WM_MOUSEMOVE:
			_OnMouseMove			(		uiMouseEvent( (UINT) wParam),x,y		);
			return (LRESULT) -1;

		case WM_MOUSELEAVE:
			_OnMouseLeave			(		uiMouseEvent( (UINT) wParam),x,y		);
			return (LRESULT) -1;

		case WM_MOUSEHOVER:
			_OnMouseHover			(		uiMouseEvent( (UINT) wParam),x,y		);
			return (LRESULT) -1;

		case WM_LBUTTONDBLCLK:
			_OnLButtonDbClick		(		uiMouseEvent( (UINT) wParam),x,y		);
			return (LRESULT) -1;

		case WM_LBUTTONDOWN:
			_OnLButtonDown			(		uiMouseEvent( (UINT) wParam),x,y		);
			return (LRESULT) -1;

		case WM_LBUTTONUP:
			_OnLButtonUp			(		uiMouseEvent( (UINT) wParam),x,y		);
			return (LRESULT) -1;

		case WM_MBUTTONDBLCLK:
			_OnMButtonDbClick		(		uiMouseEvent( (UINT) wParam),x,y		);
			return (LRESULT) -1;

		case WM_MBUTTONDOWN:
			_OnMButtonDown			(		uiMouseEvent( (UINT) wParam),x,y		);
			return (LRESULT) -1;

		case WM_MBUTTONUP:
			_OnMButtonUp			(		uiMouseEvent( (UINT) wParam),x,y		);
			return (LRESULT) -1;	

		case WM_RBUTTONDOWN:
			_OnRButtonDown			(		uiMouseEvent( (UINT) wParam),x,y		);
			return (LRESULT) -1;

		case WM_RBUTTONUP:
			_OnRButtonUp			(		uiMouseEvent( (UINT) wParam),x,y		);
			return (LRESULT) -1;

		case WM_RBUTTONDBLCLK:
			_OnRButtonDbClick		(		uiMouseEvent( (UINT) wParam),x,y		);
			return (LRESULT) -1;

		case WM_MOUSEWHEEL:			
			_OnMouseWheel			(		uiMouseEvent ( (UINT) wParam),x,y		);			
			return (LRESULT) -1;

		
		//Cac message lien quan den ban phim
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			bBreak = _OnKeyDown		(		uiKeyEvent( (UINT) wParam, (UINT) lParam) );
			if ( bBreak )
				break;
			else
				return 1;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			// Nhan phim Tab (chuyen focus)
			if ( wParam == VK_TAB )
			{
				// Neu da thiet lap danh sach Tab -> chuyen qua cua so tiep theo
				if ( zm_tabController )
					zm_tabController->next();
			}
			
			// Khoi tao su kien
			bBreak = _OnKeyUp		(		uiKeyEvent( (UINT) wParam, (UINT) lParam) );
			if ( bBreak )
				break;
			else
				return 1;

		case WM_CHAR:
			bBreak = _OnChar		(		uiCharEvent( (UINT) wParam, (UINT) lParam) );
			if ( bBreak )
				break;
			else
				return 1;
			break;

		case  WM_SIZE:			
			// Goi su kien _OnInit
			if (!zm_bInit){
				zm_bInit = true;
				_OnInit();				
			}					

			// Cap nhat cac cua so dock
			zm_pDockCtl->update();
			zm_pLockLayoutCtl->update();

			_OnSize(  uiSizeEvent( (int) wParam) , x,y );
			return (LRESULT) 1;
		case WM_CREATE:
			_OnCreate();			
			break;

		case WM_MOVE:			
			_OnMove(x,y);
			return (LRESULT) 1;

		case WM_CLOSE:
			if ( _OnClose() == true )			
				break;
			else
				return (LRESULT) 1;

		case WM_DESTROY:
			_OnDestroy();

			// Neu o cua so chinh -> thoat ung dung
			if ( this->zm_pParent == NULL)			
				PostQuitMessage (1) ;
			return (LRESULT) 1;		

		case WM_ERASEBKGND:			
			return (LRESULT) 1;

		case WM_PAINT:						
			hDC = BeginPaint(zm_hWnd, &PaintStruct);
			
			pG = new uiGraphics(hDC);			
			
			_OnPaint( pG );

			EndPaint( zm_hWnd, &PaintStruct );
			delete pG;

			return (LRESULT) 1;

		case WM_ACTIVATE:
			if ( zm_tabController )
				zm_tabController->setWinFocus( this );

			_OnSetFocus();
			return (LRESULT) 1;

		case WM_KILLFOCUS:
			_OnLostFocus();
			return (LRESULT) 1;
		
		case WM_NOTIFY:	
		{			
			UINT idControl = LOWORD(wParam);			
			uiObject *pControl = uiApplication::getControlByID( idControl );
			
			// Neu control da duoc register
			// Xu ly su kien cho control nay
			if ( pControl != NULL )
			{
				if (	pControl->getObject() != UIOBJECT_MENUITEM && 
						pControl->getObject() != UIOBJECT_TOOLBARBUTTON )
					((uiControl*)pControl)->notifyControl( lParam );
			}
			
// GNU Compiler ////////////////////////////////////////////
#if !defined(TBN_DROPDOWN)
	#define TBN_DROPDOWN            (TBN_FIRST - 10)
#endif

#if !defined(TB_GETRECT)
	#define TB_GETRECT              (WM_USER + 51) // wParam is the Cmd instead of index
#endif

#if !defined(TB_GETBUTTONINFOW)
	#define TB_GETBUTTONINFOW       (WM_USER + 63)
#endif

#if !defined(TBIF_LPARAM)
	#define TBIF_LPARAM             0x00000010			
#endif
////////////////////////////////////////////////////////////
						
			// Xu ly su kien khi icon on Toolbar Dropdown click...
			LPNMHDR pnmh = (LPNMHDR) lParam;
			if ( pnmh->code == TBN_DROPDOWN )
			{	
				// Lay ID cua button dang click
				UINT	iItem = (UINT)((LPNMTOOLBAR)lParam)->iItem;

				// Lay thong tin cua BUTTON
				TBBUTTONINFOW info;
				memset(&info,0,sizeof(info));				
				info.cbSize = sizeof(info);
				info.dwMask = TBIF_LPARAM;
				SendMessageW( pnmh->hwndFrom, TB_GETBUTTONINFOW, iItem, (LPARAM) &info );
			
				// Lay do tuong BUTTON
				uiToolbarButton* p = (uiToolbarButton*) info.lParam;								
				p->doEventPopupMenu();

				// Hien thi MENU POPUP
				uiMenuPopup *pMenuPopup = p->getMenuPopup();
				if ( pMenuPopup )
				{
					// Lay vung hinh chu nhat cua button do
					RECT rc;				
					::SendMessage(	pnmh->hwndFrom, 
									TB_GETRECT,
									iItem,
									(LPARAM)&rc);


					// Chuyen thanh toa do man hinh
					::MapWindowPoints(pnmh->hwndFrom, HWND_DESKTOP, (LPPOINT)&rc, 2);

					// Popup menu
					pMenuPopup->popup( this, rc.left, rc.bottom );
				}
			}
			
			// Display Tooltips
			if ( pnmh->code == TTN_GETDISPINFO )
			{
				LPNMTTDISPINFO pInfo = (LPNMTTDISPINFO)lParam;

				uiObject *p = uiApplication::getControlByID( (UINT)pInfo->hdr.idFrom );
				if ( p )
				{
#if !defined(TTM_SETMAXTIPWIDTH)
	#define TTM_SETMAXTIPWIDTH      (WM_USER + 24)
#endif

					// Thiet lap chieu dai toi da cho ToolTip
					SendMessage(pInfo->hdr.hwndFrom, TTM_SETMAXTIPWIDTH, 0, 500);	
					
					LPWSTR t=NULL;

					// Neu la button tren TOOLBAR
					if ( p->getObject() == UIOBJECT_TOOLBARBUTTON )
						t = ((uiToolbarButton*)p)->getTip();

					// Thiet lap toolTip cho control
					uiString::copy<WCHAR,WCHAR>( pInfo->lpszText, t);
				}			

			}									

			/////////////////////////////////////			

			// Goi control mac dinh
			_OnNotify((DWORD)wParam,(DWORD)lParam);
			
			break;
		}
		case WM_COMMAND:
		{			
			///////////////////////////////////////////////
			// Thuc thi su kien OnCommand mac dinh
			_OnCommand( (DWORD) lParam, (DWORD) wParam );		


			DWORD idItem		= LOWORD(wParam);
			UINT uControlMsg	= HIWORD(wParam);
			HWND hControl		= (HWND) lParam;
						

			///////////////////////////////////////////////
			// Control Event
			///////////////////////////////////////////////

			// Su kien voi Common Control
			if (hControl)
			{
				#pragma warning(disable:4312)				
				// Lay dia chi control
				uiControl* pControl = (uiControl*) GetWindowLong(hControl,GWL_USERDATA);
				
				// Neu control ton tai thi dinh nghia su kien cho control
				if (pControl)				
					pControl->messageControlMap( uControlMsg );																
				#pragma warning(default:4312)
			}
			
			// Kiem tra co phai la MENU hay TOOLBAR de thuc thi su kien
			// add here to fix on winXP
			uiObject* pObjectControl = uiApplication::getControlByID( idItem );
			if ( pObjectControl )
			{
				if ( pObjectControl->getObject() == UIOBJECT_MENUITEM )
				{
					uiMenuItem* pMenuItem = (uiMenuItem*) pObjectControl;
					pMenuItem->doEvent();
				}
				else if ( pObjectControl->getObject() == UIOBJECT_TOOLBARBUTTON )
				{
					uiToolbarButton* pToolbarButton = (uiToolbarButton*) pObjectControl;
					pToolbarButton->doEventClick();
				}
			}
			break;
		}
		case WM_DROPFILES:
		{
			HDROP hdrop = (HDROP) wParam;
			UINT  uNumFiles;
			WCHAR szNextFile [MAX_PATH];
			
			uiFileDrop fileDropEvent;

			// Get the # of files being dropped.
			uNumFiles = DragQueryFile ( hdrop, -1, NULL, 0 );
			for ( UINT uFile = 0; uFile < uNumFiles; uFile++ )
			{
				// Get the next filename from the HDROP info.
				if ( DragQueryFile ( hdrop, uFile, szNextFile, MAX_PATH ) > 0 )
				{
					fileDropEvent.addFile( szNextFile );		
				}
			}

			// Free up memory.
			DragFinish ( hdrop );

			this->_OnDropFile( &fileDropEvent );
			break;
		}
		case WM_MEASUREITEM:
		{
			LPMEASUREITEMSTRUCT lpmis; 
			lpmis = (LPMEASUREITEMSTRUCT) lParam; 
			
			DWORD idItem = lpmis->CtlID;
			uiControl *pControl = (uiControl*) uiApplication::getControlByID( idItem );
			
			if ( pControl != NULL )
			{
				pControl->_onMeasureItem( lpmis );
			}

			break;
		}
		case WM_DRAWITEM:
		{
			DRAWITEMSTRUCT * pdis;
			pdis = (DRAWITEMSTRUCT *)lParam;

			// NULL object
			DWORD idItem = pdis->CtlID;
			uiControl *pControl = (uiControl*) uiApplication::getControlByID( idItem );

			if ( pControl != NULL )
			{
				pControl->_onDrawItem( pdis );
			}

			return (LRESULT) 1;
		}
		case WM_HSCROLL:
		case WM_VSCROLL:		
		{
			int		nScrollCode = (int)LOWORD(wParam);
			short	nPos		= (short)HIWORD(wParam);
			
			SCROLLINFO si;
			si.cbSize = sizeof (si);
			si.fMask  = SIF_ALL;

			if ( uMsg == WS_HSCROLL )
				GetScrollInfo (hWnd, SB_HORZ, &si);
			else
				GetScrollInfo (hWnd, SB_VERT, &si);
			
			si.nPage = 1;

			switch ( nScrollCode )
			{
				 // User clicked the HOME keyboard key.
				case SB_TOP:
				//case SB_LEFT:
					si.nPos = si.nMin;
					break;
		              
				// User clicked the END keyboard key.
				case SB_BOTTOM:
				//case SB_RIGHT:
					si.nPos = si.nMax;
					break;
				 // User clicked the left arrow.
				case SB_LINELEFT: 
				//case SB_LINEUP:
					si.nPos -= 1;
					break;
		              
				// User clicked the right arrow.
				case SB_LINERIGHT:
				//case SB_LINEDOWN:
					si.nPos += 1;
					break;
		              
				// User clicked the scroll bar shaft left of the scroll box.
				case SB_PAGELEFT:
				//case SB_PAGEUP:
					si.nPos -= si.nPage;
					break;
		              
				// User clicked the scroll bar shaft right of the scroll box.
				case SB_PAGERIGHT:
				//case SB_PAGEDOWN:
					si.nPos += si.nPage;
					break;
		              
				// User dragged the scroll box.
				case SB_THUMBTRACK: 
					si.nPos = si.nTrackPos;
					break;
				case SB_THUMBPOSITION:
					si.nPos = si.nMin + nPos;
					break;
				default :
					break;
			};

			//printf("%d - %d - %d \n", si.nPos, si.nMin, si.nMax);
			si.fMask = SIF_POS;			
			if ( si.nPos < si.nMin )
				si.nPos = si.nMin;
			else if ( si.nPos > si.nMax )
				si.nPos = si.nMax;

			if ( uMsg == WS_HSCROLL )
				SetScrollInfo (hWnd, SB_HORZ, &si, FALSE);
			else
				SetScrollInfo (hWnd, SB_VERT, &si, FALSE);
			
			// create event
			uiScrollBarEvent scrollEvent( this, si.nPos, si.nMin, si.nMax );

			if ( uMsg == WS_HSCROLL )
				_OnHScroll( scrollEvent );
			else
				_OnVScroll( scrollEvent );				

			return (LRESULT) 1;
		}
	}

	return -1;
}

void uiWindow::_OnMouseMove			(	uiMouseEvent MouseEvent,int x, int y	){ }
void uiWindow::_OnMouseLeave		(	uiMouseEvent MouseEvent,int x, int y	){ }
void uiWindow::_OnMouseHover		(	uiMouseEvent MouseEvent,int x, int y	){ }
			
void uiWindow::_OnLButtonDown		(	uiMouseEvent MouseEvent,int x, int y	){ }
void uiWindow::_OnLButtonUp			(	uiMouseEvent MouseEvent,int x, int y	){ }
void uiWindow::_OnLButtonDbClick	(	uiMouseEvent MouseEvent,int x, int y	){ }

void uiWindow::_OnMButtonDown		(	uiMouseEvent MouseEvent,int x, int y	){ }
void uiWindow::_OnMButtonUp			(	uiMouseEvent MouseEvent,int x, int y	){ }
void uiWindow::_OnMButtonDbClick	(	uiMouseEvent MouseEvent,int x, int y	){ }

void uiWindow::_OnRButtonDown		(	uiMouseEvent MouseEvent,int x, int y	){ }
void uiWindow::_OnRButtonUp			(	uiMouseEvent MouseEvent,int x, int y	){ }
void uiWindow::_OnRButtonDbClick	(	uiMouseEvent MouseEvent,int x, int y	){ }
			
void uiWindow::_OnMouseWheel		(	uiMouseEvent MouseEvent,int x, int y	){ }




bool uiWindow::_OnKeyDown	(	uiKeyEvent	 KeyEvent	){ return true; }
bool uiWindow::_OnKeyUp		(	uiKeyEvent	 KeyEvent	){ return true; }
bool uiWindow::_OnChar		(	uiCharEvent  CharEvent	){ return true; }


void uiWindow::_OnCreate()	{}
void uiWindow::_OnInit()	{}

void uiWindow::_OnPaint		(	uiGraphics * pG ) 
{	
	if ( zm_bReDraw )
	{
		// To mau 3DFace (tu system) cho window
		uiPen p(1,PS_NULL,uiColor::fromDWORD(0));
		uiBrush b( zm_drawColor );

		pG->selectObject(&p);
		pG->drawFillRectangle( 0,0, this->getClientWidth(), this->getClientHeight(), &b);

		p.create(1, PS_SOLID, uiColor::fromDWORD(0) );
		pG->selectObject(&p);
	}
}

void uiWindow::_OnSize		(	uiSizeEvent sizeEvent, int nWidth, int nHeight)	{ }
void uiWindow::_OnMove		(	int x, int y ) { }
bool uiWindow::_OnClose		()	{ return true; }
void uiWindow::_OnDestroy	()	{ }		
void uiWindow::_OnSetFocus	()	{ }
void uiWindow::_OnLostFocus	()	{ }
		
void uiWindow::_OnCommand	( DWORD dwCommand, DWORD lParam )	{ }
void uiWindow::_OnNotify	( DWORD dwNotify,  DWORD lParam )	{ }

void uiWindow::_OnDropFile	( uiFileDrop* fileDrop )			{ }

void uiWindow::_onMeasureItem( MEASUREITEMSTRUCT* lpItemStruct ){ }

void uiWindow::_onDrawItem( DRAWITEMSTRUCT* lpDrawItemStruct ){ }

void uiWindow::_OnHScroll	( uiScrollBarEvent scrollEvent )	{ }

void uiWindow::_OnVScroll	( uiScrollBarEvent scrollEvent )	{ }

// static func listenThisAppEvent
void uiWindow::listenThisAppEvent( uiWindow *p )
{	
	if ( sm_globalEventWindow )
		sm_globalEventWindow->push_back( p );
	else
	{
		initAppListen();
		sm_globalEventWindow->push_back( p );
	}
}

// static func unListenThisAppEvent
void uiWindow::unListenThisAppEvent( uiWindow *p )
{
	if ( sm_globalEventWindow )
		sm_globalEventWindow->remove( p );
}

void uiWindow::initAppListen()
{
	if ( sm_globalEventWindow == NULL )
		sm_globalEventWindow = new list<uiWindow*>();
}

void uiWindow::releaseAppListen()
{
	if ( sm_globalEventWindow )
		delete sm_globalEventWindow;
}