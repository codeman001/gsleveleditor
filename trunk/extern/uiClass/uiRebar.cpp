// uiRebar.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 18/2/2009 
// Thong tin:
//

#include "uiRebar.h"
#include "uiInclude.h"
#include "uiApplication.h"
#include <windowsx.h>

using namespace uiClass;


/////////////////////////////////////////////////////////
//  rebarBand
/////////////////////////////////////////////////////////

uiRebarBand::uiRebarBand( uiWindow* p, LPWSTR lpText )
{		
	uiString::copy<WCHAR,WCHAR>( zm_Text, lpText );	
	zm_windows = p;

	zm_bGripper = true;
	zm_bBreak = true;

	
	memset( (void*) &zm_info, 0, sizeof( zm_info ) );	

	
}
		
uiRebarBand::~uiRebarBand()
{
	
}

// getObject
// Tra ve UIOBJECT
DWORD uiRebarBand::getObject()
{
	return UIOBJECT_REBARBAND;
}

// setWidthHeight
// Thiet lap chieu rong + cao cho Band
// Khi w hay h = 0 -> su dung chieu cao mac dinh
void uiRebarBand::setMinWidthHeight(DWORD w, DWORD h)
{
	zm_info.cxMinChild = w;
    zm_info.cyMinChild = h;
	
}

// setPos
// Thiet lap vi tri Band
void uiRebarBand::setWidth( DWORD w )
{
	zm_info.cx = w;
	//zm_info.cxIdeal = w;
}

// getInfoStruct
// Lay thong tin Band Info
REBARBANDINFOW* uiRebarBand::getInfoStruct()
{
	zm_info.cbSize = sizeof( REBARBANDINFO );
	zm_info.lpText = zm_Text;
	zm_info.hwndChild = zm_windows->getHandle();	

#if !defined(RBBS_USECHEVRON)
	#define RBBS_USECHEVRON				0x00000200 
#endif
	zm_info.fMask  =		  
          RBBIM_STYLE       // fStyle is valid.		
        | RBBIM_TEXT        // lpText is valid.
        | RBBIM_CHILD       // hwndChild is valid.
        | RBBIM_CHILDSIZE   // child size members are valid.
        | RBBIM_SIZE;       // cx is valid

	if ( zm_bBreak )
		zm_info.fStyle |= RBBS_BREAK;

	if ( zm_bGripper )
	{
		zm_info.fStyle |= RBBS_GRIPPERALWAYS;
		zm_info.fStyle &= ~RBBS_NOGRIPPER;
	}
	else
	{
		zm_info.fStyle |= RBBS_NOGRIPPER;
		zm_info.fStyle &= ~RBBS_GRIPPERALWAYS;
	}
	// Neu su dung CHEVRON thi phai bat RBN_CHEVRONPUSHED
	// Va popup Menu....

	return &zm_info;
}

// setBandWindow
// Thiet lap Window
void uiRebarBand::setBandWindow( uiWindow *pWin, LPWSTR lpText )
{
	uiString::copy<WCHAR,WCHAR>( zm_Text, lpText );	
	zm_windows = pWin;
}

// enableGripper
// Cho phep hien thi Gripper
void uiRebarBand::enableGripper( bool b )
{
	zm_bGripper = b;
}

// setBreakBand
// Cho phep xuong dong hay ko
void uiRebarBand::setBreakBand( bool b )
{
	zm_bBreak = b;
}





/////////////////////////////////////////////////////////
//  REBAR
/////////////////////////////////////////////////////////


uiRebar::uiRebar(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	this->create( lpTitle, x,y,w,h, pParent );

	zm_bResizeFather = false;
}

uiRebar::~uiRebar(void)
{

}
		
// getObject
// Tra ve UIOBJECT
DWORD uiRebar::getObject()
{
	return UIOBJECT_REBAR;
}

// create
// Khoi tao button
bool uiRebar::create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
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
	zm_hWnd = CreateWindowExW(WS_EX_TOOLWINDOW,
		L"ReBarWindow32",
		lpTitle,
		WS_VISIBLE | WS_BORDER | WS_CHILD | WS_CLIPCHILDREN |WS_CLIPSIBLINGS | 
			RBS_VARHEIGHT | RBS_BANDBORDERS |	CCS_NODIVIDER | CCS_NOPARENTALIGN,
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

// addBand
// Them 1 band vao Rebar
// addBand				
void uiRebar::addBand( uiRebarBand *pBand )
{
	static DWORD bandId = 0;
	
	SendMessage(this->getHandle(), RB_INSERTBANDW,	(WPARAM)-1,		(LPARAM)pBand->getInfoStruct() );	

	bandId++;
}

// notifyControl
// Xu ly notify
void uiRebar::notifyControl(LPARAM lParam)
{

// GNU Compiler ////////////////////////////////////////////////
#if !defined(RBN_FIRST)
	#define RBN_FIRST               (0U-831U)
#endif
	
#if !defined(RBN_BEGINDRAG)
	#define RBN_BEGINDRAG       (RBN_FIRST - 4)
#endif

#if !defined(RBN_ENDDRAG)
	#define RBN_ENDDRAG         (RBN_FIRST - 5)
#endif
////////////////////////////////////////////////////////////////

	UINT nMsg = ((LPNMHDR)lParam)->code;

	switch ( nMsg )
	{
	case RBN_BEGINDRAG:
		zm_bResizeFather = true;
		break;
	case RBN_ENDDRAG:
		zm_bResizeFather = false;
		break;
	}
}

// messageMap
// Phan loai message
LRESULT uiRebar::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{		
	// Hieu chinh size khi dang keo cac BAND tren rebar 
	// thi cap nhat kick thuoc cac cua so dang dock
	if ( uMsg == WM_SIZE && zm_bResizeFather )
		this->getParent()->updateDock();
	
	return uiControl::messageMap( hWnd, uMsg, wParam, lParam );
}

// showGrip
// Hien thi grip hay khong
void uiRebar::showBandBorder( bool b )
{
	if ( b ) 
		this->changeStyle( RBS_BANDBORDERS, 0 );
	else
		this->changeStyle( 0, RBS_BANDBORDERS );
}