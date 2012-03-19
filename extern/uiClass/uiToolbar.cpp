// uiToolbar.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 18/2/2009 
// Thong tin:
//

#include "uiToolbar.h"
#include "uiInclude.h"
#include "uiApplication.h"
#include <windowsx.h>

using namespace uiClass;

///////////////////////////////////////////
// UITOOLBAR BUTTON
///////////////////////////////////////////

//  GNU Support /////////////////////////////////////////////

#if !defined(TBIF_TEXT)
	#define TBIF_TEXT               0x00000002
#endif

#if !defined(TBIF_STATE)
	#define TBIF_STATE              0x00000004
#endif

#if !defined(TBIF_STYLE)
	#define TBIF_STYLE              0x00000008
#endif

#if !defined(TBIF_LPARAM)
	#define TBIF_LPARAM             0x00000010
#endif

#if !defined(BTNS_DROPDOWN)
	#define BTNS_DROPDOWN			0x0008  // obsolete; use BTNS_DROPDOWN instead
#endif

#if !defined(BTNS_WHOLEDROPDOWN)
	#define BTNS_WHOLEDROPDOWN		0x0080          // draw drop-down arrow, but without split arrow section
#endif

#if !defined(TBIF_COMMAND)
	#define TBIF_COMMAND            0x00000020
#endif
	
#if !defined(TBSTYLE_FLAT)
	#define TBSTYLE_FLAT            0x0800
#endif

#if !defined(TB_SETIMAGELIST)
	#define TB_SETIMAGELIST         (WM_USER + 48)
#endif

#if !defined(CBRS_TOOLTIPS)
	#define CBRS_TOOLTIPS       0x0010L
#endif 

#if !defined(TBSTYLE_EX_DRAWDDARROWS)
	#define TBSTYLE_EX_DRAWDDARROWS 0x00000001
#endif

#if !defined(TB_SETEXTENDEDSTYLE)
	#define TB_SETEXTENDEDSTYLE     (WM_USER + 84)  // For TBSTYLE_EX_*
#endif

#if !defined(TB_ADDBUTTONSW)
	#define TB_ADDBUTTONSW          (WM_USER + 68)
#endif

#if !defined(TB_SETBUTTONINFOW)
	#define TB_SETBUTTONINFOW        (WM_USER + 64)
#endif

#if !defined(TB_GETBUTTONINFOW)
	#define TB_GETBUTTONINFOW        (WM_USER + 63)
#endif

#if !defined(TBSTYLE_LIST)
	#define TBSTYLE_LIST            0x1000
#endif

#if !defined(TB_INSERTBUTTONW)
	#define TB_INSERTBUTTONW        (WM_USER + 67)
#endif

#if !defined(TB_SETSTYLE)
	#define TB_SETSTYLE             (WM_USER + 56)
#endif

#if !defined(TB_GETSTYLE)
	#define TB_GETSTYLE             (WM_USER + 57)
#endif

//////////////////////////////////////////////////////////

uiToolbarButton::uiToolbarButton( uiToolbar *p, LPWSTR lpLabel, DWORD iImage )
{
	zm_pToolbar = p;
	memset( (void*) &zm_button, 0, sizeof( zm_button) );

	// Thiet lap Button
	zm_button.cbSize = sizeof( zm_button );
	zm_button.dwMask = TBIF_TEXT | TBIF_STYLE | TBIF_LPARAM | TBIF_STATE | TBIF_COMMAND;
	zm_button.fsState = TBSTATE_ENABLED;
	zm_button.fsStyle = TBSTYLE_BUTTON; //BTNS_DROPDOWN; //BTNS_WHOLEDROPDOWN;

	// Thiet lap LABEL
	uiString::copy<WCHAR,WCHAR>( zm_pLabel		, lpLabel );
	uiString::copy<WCHAR,WCHAR>( zm_pTooltip	, lpLabel );

	zm_button.pszText = zm_pLabel;
	zm_button.cchText = uiString::length<WCHAR>( zm_pLabel );

	// Thiet lap IMAGE
	zm_button.iImage = (int) iImage;
	zm_button.lParam = (DWORD_PTR) this;

	zm_pMenuPopup = NULL;
		
}

uiToolbarButton::~uiToolbarButton()
{

}

// setButtonType
// Thay doi kieu cua button
// dwType la const UITOOLBARBUTTON_
void uiToolbarButton::setButtonType( DWORD dwType )
{
	switch ( dwType )
	{
	case UITOOLBARBUTTON_DROPDOWN:
		zm_button.fsStyle =	BTNS_DROPDOWN;
		break;
	case UITOOLBARBUTTON_WHOLEDROPDOWN:
		zm_button.fsStyle = BTNS_WHOLEDROPDOWN;
		break;
	case UITOOLBARBUTTON_SEPARATOR:
		zm_button.fsStyle = TBSTYLE_SEP;
		break;
	default:
		zm_button.fsStyle = TBSTYLE_BUTTON;
	}
	this->update();
}

// getObject
// Tra ve doi tuong UIOBJECT_
DWORD uiToolbarButton::getObject()
{
	return UIOBJECT_TOOLBARBUTTON;
}

// setLabel
// Thiet lap label cho button
void uiToolbarButton::setLabel( LPWSTR lpString )
{	
	uiString::copy<WCHAR,WCHAR>( zm_pLabel, lpString );
	zm_button.pszText = zm_pLabel;
	zm_button.cchText = uiString::length<WCHAR>( zm_pLabel );

	this->update();
}

// setTip
// Thiet lap string tooltip cho button
void uiToolbarButton::setTip( LPWSTR lpString )
{
	uiString::copy<WCHAR,WCHAR>( zm_pTooltip, lpString );
}

// getTip
// Lay toolTip
LPWSTR uiToolbarButton::getTip()
{
	return zm_pTooltip;
}

// getLabel
// Lay label cua Button
LPWSTR uiToolbarButton::getLabel()
{
	return zm_pLabel;
}

// setMenuPopup
// Thiet lap menu popup cho button
void uiToolbarButton::setMenuPopup( uiMenuPopup* pMenu )
{
	zm_pMenuPopup = pMenu;
}

// getMenuPopup
// Lay menu popup
uiMenuPopup* uiToolbarButton::getMenuPopup()
{
	return zm_pMenuPopup;
}

// setImage
// Thiet lap image cho button
void uiToolbarButton::setImage( DWORD nIndex )
{
	zm_button.iImage = (int) nIndex;
	this->update();
}

// getStruct
// Lay thong tin struct cua button
TBBUTTONINFOW* uiToolbarButton::getStruct()
{
	return &zm_button;
}

// setID
// Thiet lap ID cho button
void uiToolbarButton::setID( DWORD id )
{
	zm_idButton = id;
	zm_button.idCommand = (int) id;
}

// getID
// Lay id hien hanh
DWORD uiToolbarButton::getID()
{
	return zm_idButton;
}

// update
// Cap nhat trang thai button
void uiToolbarButton::update()
{
	// Thiet lap trang thai...
	SendMessageW( zm_pToolbar->getHandle(), TB_SETBUTTONINFOW, zm_idButton, (LPARAM) &zm_button );
}

// setCheck
// Thiet lap trang thai button
void uiToolbarButton::setCheck( bool check )
{	
	if ( check )
		zm_button.fsState	|= TBSTATE_CHECKED;
	else
		zm_button.fsState	&= ~TBSTATE_CHECKED;	

	this->update();
}


// getCheck
// Lay trang thay button
bool uiToolbarButton::isCheck()
{
	TBBUTTONINFOW info;
	memset(&info,0,sizeof(info));
	
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_STATE;
	SendMessageW( zm_pToolbar->getHandle(), TB_GETBUTTONINFOW, zm_idButton, (LPARAM) &info );

	return (info.fsState & TBSTATE_CHECKED) == TBSTATE_CHECKED ;
}

// enableButton
// Cho phep button hoat dong hay ko
void uiToolbarButton::enableButton( bool b )
{
	if ( b )
		zm_button.fsState	|= TBSTATE_ENABLED;
	else
		zm_button.fsState	&= ~TBSTATE_ENABLED;	

	this->update();
}

// isEnable
// Kiem tra trang thai button
bool uiToolbarButton::isEnable()
{
	TBBUTTONINFOW info;
	memset(&info,0,sizeof(info));
	
	info.cbSize = sizeof(info);
	info.dwMask = TBIF_STATE;
	SendMessageW( zm_pToolbar->getHandle(), TB_GETBUTTONINFOW, zm_idButton, (LPARAM) &info );

	return (info.fsState & TBSTATE_ENABLED) == TBSTATE_ENABLED ;
}

// doEventClick
// Thuc thi su kien click
void uiToolbarButton::doEventClick()
{
	eventOnClicked(this);	
}

// doEventPopupMenu
// Thuc thi su kien menu push
void uiToolbarButton::doEventPopupMenu()
{
	eventOnPopupMenu(this);
}









///////////////////////////////////////////
// UITOOLBAR
///////////////////////////////////////////

uiToolbar::uiToolbar(LPCWSTR lpTitle, int x, int y, int w, int h, DWORD iconW, DWORD iconH, uiWindow* pParent)
{
	this->create( lpTitle, x,y,w,h, iconW, iconH, pParent );
}

uiToolbar::~uiToolbar(void)
{
	ImageList_Destroy( zm_listImage );
	
	// Huy toi tuong button
	vector< uiToolbarButton*>::iterator i = zm_listButton.begin();
	while ( i != zm_listButton.end() )
	{
		delete (*i);
		i++;
	}
	zm_listButton.clear();
	
}
		
// getObject
// Tra ve UIOBJECT
DWORD uiToolbar::getObject()
{
	return UIOBJECT_TOOLBAR;
}

// create
// Khoi tao button
bool uiToolbar::create(LPCWSTR lpTitle, int x, int y, int w, int h, DWORD iconW, DWORD iconH, uiWindow* pParent)
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
	zm_hWnd = CreateWindowExW(
		0,
		L"ToolbarWindow32",
		lpTitle,
		WS_VISIBLE | WS_CHILD | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | WS_CLIPCHILDREN |
		CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER |CCS_ADJUSTABLE ,
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
	
	// Thiet lap danh sach icon & image cho ToolBar
	zm_listImage = ImageList_Create( iconW,iconH, ILC_COLOR32|ILC_MASK, 5, 10 );
	SendMessage( this->getHandle() , TB_SETIMAGELIST, (WPARAM)0, (LPARAM)zm_listImage);	

	// Thiet lap styleEX cho toolbar
	// Cho phep ve dau mui ten

	// Cho phep hien thi dau TAM GIAC tren button
	DWORD dwExStyle = TBSTYLE_EX_DRAWDDARROWS;
	SendMessageW( this->getHandle(), TB_SETEXTENDEDSTYLE, 0, (LPARAM)dwExStyle);		

	return true;
}

// setButtonWH
// Thiet lap chieu rong + cao cho button
void uiToolbar::setButtonWH(DWORD w, DWORD h)
{		
	SendMessageW( this->getHandle(), TB_SETBUTTONSIZE, 0, MAKELONG(w, h));
}

// setImageWH
// Thiet lap chieu rong + cao cho image
void uiToolbar::setImageWH(DWORD w, DWORD h)
{
	SendMessageW( this->getHandle(), TB_SETBITMAPSIZE, 0, MAKELONG(w, h));
}

// pushImage
// Them 1 image button vao toolbar
DWORD uiToolbar::pushImage ( uiBitmap *p, uiColor mask )
{
	if ( p == NULL )	
		return 0;	
	else
		ImageList_AddMasked( zm_listImage, (HBITMAP) p->getSafeObject(), (COLORREF) mask.getColor() );

	return ImageList_GetImageCount( zm_listImage ) - 1;
}

// pushImage
// Them 1 image button vao toolbar
DWORD uiToolbar::pushImage ( uiBitmap *p )
{
	if ( p == NULL )	
		return 0;	
	else	
		ImageList_Add( zm_listImage, (HBITMAP) p->getSafeObject(), NULL );	

	return ImageList_GetImageCount( zm_listImage ) - 1;
}

// pushImage
// Them 1 image button vao toolbar
DWORD uiToolbar::pushImage ( uiIcon *p )
{
	if ( p == NULL )	
		return 0;	
	else
		ImageList_AddIcon ( zm_listImage,(HICON) p->getHandle() );	

	return ImageList_GetImageCount( zm_listImage ) - 1;
}

// removeImage
// Xoa image
void uiToolbar::removeImage( DWORD index )
{
	ImageList_Remove(zm_listImage, index );
}

// addButton
// Them 1 button vao toolbar
uiToolbarButton* uiToolbar::addButton( LPWSTR lpLabel, DWORD iImage )
{	
	uiToolbarButton *pButton = new uiToolbarButton( this, lpLabel, iImage );
	
	// Dua button vao list
	zm_listButton.push_back( pButton );
		
	TBBUTTON b;
	memset( (void*) &b, 0, sizeof(b) );
	b.iBitmap = (int) iImage;
	b.fsState = TBSTATE_ENABLED;
	b.fsStyle = TBSTYLE_BUTTON;	
	b.idCommand = (int) this->createButtonID();

	// Them button
	SendMessageW( this->getHandle(), TB_ADDBUTTONSW, 1, (LPARAM) &b );

	// Luu ID button
	pButton->setID( b.idCommand );

	// Thiet lap thong tin bo sung cho button
	SendMessageW( this->getHandle(), TB_SETBUTTONINFOW, b.idCommand, (LPARAM) pButton->getStruct() );	
	
	// Dang ky ID cho object
	uiApplication::objectRegister( b.idCommand, pButton );

	return pButton;
}

// getButton
// Lay doi tuong button
uiToolbarButton* uiToolbar::getButton( DWORD index )
{
	if ( index >= (DWORD)zm_listButton.size() )
		return NULL;

	return zm_listButton[index];
}
		
// insertButton
// Them 1 button vao toolbar
uiToolbarButton* uiToolbar::insertButton( DWORD index, LPWSTR lpLabel, DWORD iImage )
{
	uiToolbarButton *pButton = new uiToolbarButton( this, lpLabel, iImage );
	
	// Dua button vao list
	if ( index >= (DWORD)zm_listButton.size() )
		zm_listButton.push_back( pButton );
	else
		zm_listButton.insert( zm_listButton.begin() + index, pButton );

	// Them button vao struct
	TBBUTTON b;
	memset( (void*) &b, 0, sizeof(b) );
	b.iBitmap = (int) iImage;
	b.fsState = TBSTATE_ENABLED;
	b.fsStyle = TBSTYLE_BUTTON;	
	b.idCommand = (int) this->createButtonID();

	// Them button
	SendMessageW( this->getHandle(), TB_INSERTBUTTONW, index, (LPARAM) &b );

	// Luu ID button
	pButton->setID( b.idCommand );

	// Thiet lap thong tin bo sung cho button
	SendMessageW( this->getHandle(), TB_SETBUTTONINFOW, b.idCommand, (LPARAM) pButton->getStruct() );	
	
	// Dang ky ID cho object
	uiApplication::objectRegister( b.idCommand, pButton );

	return pButton;
}

// deleteButton
// Xoa button tren toolbar
void uiToolbar::deleteButton( DWORD index )
{
	uiToolbarButton *p = this->getButton( index );
	if ( p )
	{
		// Xoa button
		SendMessageW( this->getHandle(), TB_DELETEBUTTON, 0, p->getID() );
	}
}

// addSeparator
// Them 1 button 1 khoang cach
uiToolbarButton* uiToolbar::addSeparator()
{
	uiToolbarButton* p = this->addButton(NULL,0);
	p->setButtonType( UITOOLBARBUTTON_SEPARATOR );
	return p;
}

// createButtonID
// Khoi tao ID cho button
DWORD uiToolbar::createButtonID()
{
	static DWORD dwButtonID = UICONTROL_TOOLBAR;	
	return dwButtonID++;
}

// setTextRight
// Thiet lap text tren phai cho toolbar (true/false)
void uiToolbar::setTextRight( bool b )
{
	DWORD style = (DWORD) SendMessageW( this->getHandle(), TB_GETSTYLE , 0, 0 ); 
	if ( b )
		style |= TBSTYLE_LIST;
	else
		style &= ~TBSTYLE_LIST;

	SendMessageW( this->getHandle(), TB_SETSTYLE, 0, (LPARAM) style );
}