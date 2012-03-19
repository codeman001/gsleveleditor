#include "stdafx.h"
#include "CAddEditWindow.h"
#include "Resource.h"

/////////////////////////////////////////
// CComboList
/////////////////////////////////////////

CComboList::CComboList(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	HINSTANCE hInst = pParent->getInstance();	
	HWND hParent;
	if (pParent == NULL)
		hParent = NULL;
	else
		hParent = pParent->getHandle();		
	
	zm_pParent = pParent;
	 
	this->zm_hInstance = hInst;		

	DWORD dwStyle;
	dwStyle = CBS_SIMPLE|CBS_AUTOHSCROLL|CBS_DISABLENOSCROLL|WS_VSCROLL;

#pragma warning(disable:4312)
	//Khoi tao cua so
	zm_hWnd = CreateWindowExW(0,
		L"ComboBox",
		lpTitle,
		WS_CHILD | WS_TABSTOP | WS_VISIBLE | dwStyle,
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
	SendMessage(zm_hWnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(false, 0));

	// Turn off border of combolist
	HWND hEdit, hList;
	WCHAR   *wndClassName= L"Edit";

	hEdit = GetWindow(zm_hWnd, GW_CHILD);
		 
	GetClassName (hEdit, wndClassName, 10);
	if (!lstrcmp (wndClassName, L"Edit"))
		hList = GetWindow(hEdit, GW_HWNDNEXT);
	else
	{
		hList = hEdit;
		hEdit = GetWindow(hList, GW_HWNDNEXT);
	}
	
	LONG lStyle = GetWindowLong(hEdit, GWL_EXSTYLE);
	lStyle &= ~WS_EX_CLIENTEDGE;	
	SetWindowLong(hEdit, GWL_EXSTYLE, lStyle); 
	UpdateWindow(hEdit);	

}

CComboList::~CComboList(void)
{
}

// messageMap
// Phan loai su kien
// fix bug: CBS_SIMPLE ComboBox Repainting Problem
// ref: http://support.microsoft.com/kb/128110

LRESULT CComboList::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	#pragma warning(disable: 4312)
	
	if ( uMsg == WM_PAINT )
	{
		CallWindowProcW( (WNDPROC) zm_lOldWinProc,hWnd, uMsg, wParam, lParam);
		
		//This code obtains the handle to the edit control of the
		//combobox.
		
		HDC myDC;
		HPEN   hPen, hOldPen;
		HBRUSH hBrush;
		HBRUSH hOldBrush;

		//It can be any color. Here the area is painted white
		COLORREF myColor=RGB(255,255,255); 

		myDC = GetDC(hWnd);
		hBrush = CreateSolidBrush(myColor);
		hPen   = CreatePen (PS_SOLID, 1, myColor);
		hOldBrush = (HBRUSH)SelectObject(myDC, hBrush) ;
		hOldPen   = (HPEN)SelectObject(myDC, hPen);

		HWND hEdit, hList;
		RECT comboRect, editRect, listRect;
		WCHAR   *wndClassName= L"Edit";

		hEdit = GetWindow(zm_hWnd, GW_CHILD);
		 
		GetClassName (hEdit, wndClassName, 10);

		if (!lstrcmp (wndClassName, L"Edit"))
			hList = GetWindow(hEdit, GW_HWNDNEXT);
		else
		{
			hList = hEdit;
			hEdit = GetWindow(hList, GW_HWNDNEXT);
		}

		 //The dimensions of the Edit Control, ListBox control and
		 //the Combobox are calculated and then used
		 //as the base dimensions for the Rectangle() routine.

		GetClientRect (zm_hWnd, &comboRect);
		GetClientRect (hEdit, &editRect);
		GetClientRect (hList, &listRect);
		
		Rectangle (myDC,
					comboRect.left,
					editRect.bottom,
					comboRect.right-listRect.right,
					comboRect.bottom);
		//Also paint the gap, if any exists, between the bottom
		//of the listbox and the bottom of the ComboBox rectangle.
		Rectangle (myDC,
				comboRect.right-listRect.right,
				editRect.bottom +
				listRect.bottom,
				comboRect.right,
				comboRect.bottom);

		DeleteObject(SelectObject(myDC, hOldBrush)) ;
		DeleteObject(SelectObject(myDC, hOldPen)) ;
		ReleaseDC(hWnd, myDC);

		return 1;
	}
	else
	{
		// Thuc thi control mac dinh
		LRESULT r = CallWindowProcW( (WNDPROC) zm_lOldWinProc,hWnd, uMsg, wParam, lParam);

		// Thuc thi control duoc ke thua tu uiWindow
		uiWindow::messageMap(hWnd, uMsg, wParam, lParam);
		
		return r;
	}
	#pragma warning(default: 4312)	
}


/////////////////////////////////////////
// CAddEditWindow
/////////////////////////////////////////

CAddEditWindow::CAddEditWindow( WCHAR* lpString, uiWindow *pParent )
	:CBaseWindow(lpString, pParent)
{
	uiRebar *pRebar = ref<uiRebar>( new uiRebar(L"RebarTool", 0,0, 300,30, this) );	
	
	uiToolbar *pToolbar = ref<uiToolbar>( new uiToolbar(L"Command", 0,0,0,0, 24,24, this ) );

	uiIcon iconAdd( MAKEINTRESOURCE(IDI_TOOLBARADD) );
	uiIcon iconMod( MAKEINTRESOURCE(IDI_TOOLBAREDIT) );
	uiIcon iconDel( MAKEINTRESOURCE(IDI_TOOLBARDELETE) );

	DWORD iconAddIndex = pToolbar->pushImage( &iconAdd );
	DWORD iconModIndex = pToolbar->pushImage( &iconMod );
	DWORD iconDelIndex = pToolbar->pushImage( &iconDel );

	pToolbar->setButtonWH(100,30);
	pToolbar->setTextRight(true);

	m_addButton	= pToolbar->addButton(L"Add", iconAddIndex);
	m_addButton->setEventOnClicked<CAddEditWindow, &CAddEditWindow::onToolbarCommand>( this );

	m_modifyButton = pToolbar->addButton(L"Modify", iconModIndex);
	m_modifyButton->setEventOnClicked<CAddEditWindow, &CAddEditWindow::onToolbarCommand>( this );

	m_deleteButton	= pToolbar->addButton(L"Delete", iconDelIndex);
	m_deleteButton->setEventOnClicked<CAddEditWindow, &CAddEditWindow::onToolbarCommand>( this );

	uiRebarBand bandToolbar( pToolbar,L"");	
	bandToolbar.setWidth(50);
	// set height for toolbar
	bandToolbar.setMinWidthHeight(0, 30);
	bandToolbar.enableGripper( false );
	bandToolbar.setBreakBand( true );
	
	pRebar->addBand( &bandToolbar );
	pRebar->setDock( this, UIDOCK_TOP );	
	pRebar->setMargin(0,0,30,0);
	pRebar->showBandBorder( false );
	pRebar->showWindow(true);

	// create combolist
	m_comboList = ref<CComboList>( new CComboList(L"", 0,0, 10,10, this) );
	m_comboList->setDock( this, UIDOCK_FILL );	
}

CAddEditWindow::~CAddEditWindow()
{
}

// onToolbarCommand
// event khi nhan toolbar
void CAddEditWindow::onToolbarCommand( uiObject *pObj )
{
	if ( pObj == m_addButton )
		onAddButton();
	else if ( pObj == m_modifyButton )
		onModifyButton();
	else if ( pObj == m_deleteButton )
		onDelButton();
}

// reloadList
// reload list of item
void CAddEditWindow::reloadList()
{
}

void CAddEditWindow::onAddButton()
{
}

void CAddEditWindow::onModifyButton()
{
}

void CAddEditWindow::onDelButton()
{
}