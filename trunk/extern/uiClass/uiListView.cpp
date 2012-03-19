// uiListView.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 10/1/2009
// Thong tin:

#include "uiListView.h"
#include "uiInclude.h"
#include "uiString.h"
#include "uiApplication.h"

#include <algorithm>

using namespace uiClass;

////////////////////////////////////////////////////////////
/// uiListViewColumn
////////////////////////////////////////////////////////////

// getObject
// Lay gia tri UIOBJECT_
DWORD uiListViewColumn::getObject()
{
	return UIOBJECT_LISTVIEWCOL;
}
		
uiListViewColumn::uiListViewColumn( LPWSTR lpText, UINT nWidth, bool isIcon, uiListView *pList )
{
	uiString::copy<WCHAR,WCHAR>( zm_lpText, lpText );
	
#if !defined(LVCF_IMAGE)
#define LVCF_IMAGE 0x10
#endif

	zm_nFlags	= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	if ( isIcon )
		zm_nFlags |= LVCF_IMAGE;

	zm_nWidth	= nWidth;
	zm_nMFT		= LVCFMT_LEFT;	
	zm_pListView = pList;
	zm_nIcon	= -1;
}

uiListViewColumn::~uiListViewColumn()
{

}

// setText
// Thiet lap gia tri text
void uiListViewColumn::setText( LPWSTR lpText )
{
	uiString::copy<WCHAR,WCHAR>( zm_lpText, lpText );
}

// getText
// Lay text
void uiListViewColumn::getText( LPWSTR lpText )
{
	uiString::copy<WCHAR,WCHAR>( lpText,zm_lpText );
}

// getText
// Lay gia tri text
LPWSTR uiListViewColumn::getText()
{
	return zm_lpText;
}

// setWidth
// Thiet lap chieu rong
void uiListViewColumn::setWidth( UINT nWidth )
{
	zm_nWidth	= nWidth;
}

// getWidth
// Lay gia tri chieu rong
UINT uiListViewColumn::getWidth()
{
	return zm_nWidth;
}

// setAlgin
// Thiet lap canh le cho title column
// Tham so: align la const UILISTVIEW_MFT
void uiListViewColumn::setMFT( int nMFT )
{
	zm_nMFT		= nMFT;	
}

// getAlign
// Tra ve UILISTVIEW_MFT
int uiListViewColumn::getMFT()
{
	return zm_nMFT;
}

// setBitmap
// Thiet lap bitmap 
void uiListViewColumn::setIcon( int icon )
{
	zm_nIcon = icon;
}

// getBitmap
// Lay bitmap hien hanh
int uiListViewColumn::getIcon()
{
	return zm_nIcon;
}

// getFlags
// Lay gia tri flags
UINT uiListViewColumn::getFlags()
{
	return zm_nFlags;
}

void uiListViewColumn::update()
{
	zm_pListView->updateColumn( this );
}

////////////////////////////////////////////////////////////
/// uiListViewRow
////////////////////////////////////////////////////////////

uiListViewRow::uiListViewRow( uiListView* lsview )
:uiAbstractListItem( lsview )
{	
	zm_bCBSelected = false;
	zm_nIconIndex = -1;
}

uiListViewRow::~uiListViewRow()
{

}

// getObject
// Lay gia tri Object
DWORD uiListViewRow::getObject()
{
	return UIOBJECT_LISTVIEWROW;
}

// addParam
// Tham 1 tham so
void uiListViewRow::addParam( LPWSTR lpString )
{
	std::wstring s(lpString);
	zm_params.push_back( s );
}

void uiListViewRow::setParam( UINT i, LPWSTR lpString )
{	
	std::wstring s(lpString);	

	if ( i >=  (UINT) zm_params.size() )
		zm_params.push_back( s );
	else
		zm_params[i] = lpString;
}

// getParam
// Lay gia tri cua tham so
bool uiListViewRow::getParam( UINT i, LPWSTR lpString )
{
	if ( i >=  (UINT) zm_params.size() )
		return false;
	
	uiString::copy( lpString, zm_params[i].data() );
	return true;
}

// clearParam
// Xoa param
void uiListViewRow::clearParam()
{
	zm_params.clear();
}
		
// deleteParam
// Xoa param thu i
void uiListViewRow::deleteParam( UINT i )
{
	if ( i >=  (UINT) zm_params.size() )
		return;

	std::vector< std::wstring >::iterator pos = zm_params.begin(); 
	for (UINT j = 0; j < i; j++ )
		pos++;
	
	zm_params.erase( pos );
}

// getParamCount
// So luong tham so
UINT uiListViewRow::getParamCount()
{	
	return (UINT) zm_params.size();
}

// setIndex
// Thiet lap vi tri index listview
void uiListViewRow::setIndex( DWORD nIndex )
{
	zm_nIndex = nIndex;
}

// getIndex
// Lay vi tri index trong listview
DWORD uiListViewRow::getIndex()
{
	return zm_nIndex;
}

// setCheckBox
// Thiet lap checkbox
void uiListViewRow::setCheckBox( bool b )
{
	zm_bCBSelected = b;
}

// getCheckBox
// Lay gia tri checkbox
bool uiListViewRow::getCheckBox()
{
	return zm_bCBSelected;
}

// getIconIndex
// Lay icon hien hanh
int uiListViewRow::getIconIndex()
{
	return zm_nIconIndex;
}

// setIconIndex
// Thiet lap icon
void uiListViewRow::setIconIndex(int nIndex)
{
	zm_nIconIndex = nIndex;
}

////////////////////////////////////////////////////////////
/// uiListView
////////////////////////////////////////////////////////////

// getObject
// Tra ve UIOBJECT_
DWORD uiListView::getObject()
{
	return UIOBJECT_LISTVIEW;
}

uiListView::uiListView(LPCWSTR lpTitle, int x, int y, int w, int h, bool bIcon, uiWindow* pParent)
{
	this->create( lpTitle, x,y,w,h, bIcon, pParent );
}

uiListView::~uiListView()
{
	// Giai phong data cac column
	vector< uiListViewColumn* >::iterator i;
	for (i = zm_listColumn.begin(); i != zm_listColumn.end(); i++ )	
		delete (*i);
	zm_listColumn.clear();
}


// create
// Khoi tao listView
bool uiListView::create(LPCWSTR lpTitle, int x, int y, int w, int h, bool bIcon, uiWindow* pParent)
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
	zm_hWnd = CreateWindowExW(WS_EX_CLIENTEDGE,
		L"SysListView32",
		lpTitle,
		WS_CHILD | WS_TABSTOP | WS_VISIBLE | LVS_ALIGNLEFT | LVS_REPORT,
		x,y,w,h,
		hParent,
		(HMENU) uiApplication::controlRegister(this) ,
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

	if ( bIcon )
	{
		ListView_SetImageList( this->getHandle(), this->getHandleLargeIcon(), LVSIL_NORMAL );
		ListView_SetImageList( this->getHandle(), this->getHandleSmallIcon(), LVSIL_SMALL );		
	}
	
	// ListView_SetExtendedListViewStyle(this->getHandle(), LVS_EX_INFOTIP|LVS_EX_LABELTIP);

	zm_isHaveIcon = bIcon;

	return true;
}

// messageControlMap
// Phan loai su kien cho Control
void uiListView::messageControlMap(UINT uMsg)
{
	switch (uMsg)
	{
	case BN_SETFOCUS:
		eventOnFocus	(this);		
		break;
	case BN_KILLFOCUS:
		eventOnLostFocus(this);
		break;
	}
}

// notifyControl
// Phan loai su kien cho Control
void uiListView::notifyControl( LPARAM lParam )
{
	LPNMLISTVIEW pnm = (LPNMLISTVIEW)lParam;
	UINT nMsg = ((LPNMHDR)lParam)->code;

	switch ( nMsg )
	{
	case NM_DBLCLK:
		eventOnDbClicked( this );
		break;
	case NM_CLICK:
		eventOnClicked( this );
		break;
	case NM_RCLICK:
		eventOnRClicked(this);
		break;
	case NM_RDBLCLK:
		eventOnDbRClicked(this);
		break;
	case LVN_COLUMNCLICK:
		eventOnColClicked( this );
		break;
	case LVN_KEYDOWN:
		eventOnKeyDown( this );
		break;
	case LVN_ITEMCHANGED:
		eventOnRowChanged( this );
		break;
	/*
	case LVN_GETINFOTIP:
	{
		LPNMLVGETINFOTIP pGetInfoTip = (LPNMLVGETINFOTIP)lParam;

        if (pGetInfoTip != NULL)
        {			
			uiString::copy<WCHAR,WCHAR>(pGetInfoTip->pszText, L"I am text");		
		}

	}
	*/
    break;

	}
	
	uiControl::notifyControl(lParam);
}

// listAddColumn
// Them 1 cot
uiListViewColumn* uiListView::addColumn( LPWSTR lpTitle, UINT nWidth )
{
	uiListViewColumn *p = new uiListViewColumn( lpTitle, nWidth, zm_isHaveIcon ,this );
	zm_listColumn.push_back( p );

	// Thiet lap column
	LVCOLUMNW lvc;
	lvc.mask	= p->getFlags();
	lvc.pszText = p->getText();	
	lvc.cx		= p->getWidth();
	lvc.fmt		= p->getMFT();

	ListView_InsertColumn( this->getHandle(), (DWORD)zm_listColumn.size() , &lvc );

	return p;
}


// addRow
// Tham 1 hang
uiListViewRow* uiListView::addRow( LPWSTR lpTitle )
{
	uiListViewRow *r = new uiListViewRow( this );
	r->addParam( lpTitle );
	zm_listItem.push_back( r );

	LVITEMW	lvi;
	lvi.iItem		= (int) zm_listItem.size();
	lvi.iImage		= (int) r->getIconIndex();	
	lvi.iSubItem	= 0;
	lvi.mask		= LVIF_IMAGE | LVIF_TEXT | LVIF_STATE;
	lvi.state		= 0;
	lvi.stateMask	= 0;
	lvi.pszText		= lpTitle;
	
	ListView_InsertItem( this->getHandle(), &lvi );	

	return r;
}

// insertRow
// Them 1 row tai vi tri nao do
uiListViewRow* uiListView::insertRow(DWORD nIndex, LPWSTR lpTitle )
{
	if ( nIndex > zm_listItem.size() )
		return NULL;

	uiListViewRow *r = new uiListViewRow( this );
	r->addParam( lpTitle );	
	
	insertAbstractItem( nIndex, r );

	LVITEMW	lvi;
	lvi.iItem		= nIndex;
	lvi.iImage		= (int) r->getIconIndex();
	lvi.iSubItem	= 0;
	lvi.mask		= LVIF_IMAGE | LVIF_TEXT | LVIF_STATE;
	lvi.state		= 0;
	lvi.stateMask	= 0;
	lvi.pszText		= lpTitle;

	ListView_InsertItem( this->getHandle(), &lvi );	

	return r;
}

// listInsertColumn
// Them 1 cot tai vi tri index
uiListViewColumn* uiListView::insertColumn( LPWSTR lpTitle, UINT nWidth, DWORD index )
{
	uiListViewColumn *p = new uiListViewColumn( lpTitle, nWidth,zm_isHaveIcon, this );


	vector< uiListViewColumn* >::iterator i;
	if ( index >= (DWORD)zm_listColumn.size() ) 
		i = zm_listColumn.end();
	else
		i = zm_listColumn.begin() + index;	

	zm_listColumn.insert( i, p );

	// Thiet lap column
	LVCOLUMNW lvc;
	lvc.mask	= p->getFlags();
	lvc.pszText = p->getText();	
	lvc.cx		= p->getWidth();
	lvc.fmt		= p->getMFT();

	ListView_InsertColumn( this->getHandle(), index , &lvc );

	return p;
}

// updateColumn
// Cap nhat lai colum
void uiListView::updateColumn( uiListViewColumn *col )
{
	vector<uiListViewColumn*>::iterator i;
	int pos = 0;
	for ( i = zm_listColumn.begin(); i != zm_listColumn.end(); i++ )
	{
		if ( (*i) == col )
		{
			LVCOLUMNW lvc;
			lvc.mask	= col->getFlags();
			lvc.pszText = col->getText();	
			lvc.cx		= col->getWidth();
			lvc.fmt		= col->getMFT();
			
			ListView_SetColumn( this->getHandle(), pos, &lvc );
			return;
		}
		pos++;
	}
		
}

// update
// Cap nhat item
void uiListView::updateItem( uiAbstractListItem* pItem )
{
	uiListViewRow* row = (uiListViewRow*) pItem;

	uiAbstractArrayItem::iterator i;
	int pos = 0;
	for ( i = zm_listItem.begin(); i != zm_listItem.end(); i++ )
	{
		if ( (*i) == row )
		{
			WCHAR lpString[300];
			for (UINT j = 0; j < row->getParamCount() ; j++ )
			{
				row->getParam(j, lpString);

				LVITEMW	lvi;
				lvi.iItem		= pos;
				lvi.iImage		= ((uiListViewRow*)pItem)->getIconIndex();
				lvi.mask		= LVIF_IMAGE | LVIF_TEXT | LVIF_STATE;
				lvi.iSubItem	= j;				
				lvi.state		= 0;
				lvi.stateMask	= 0;
				lvi.pszText		= lpString;								
				
				ListView_SetItem( this->getHandle(), &lvi );
				ListView_SetCheckState( this->getHandle(), pos, row->getCheckBox() );

			}
			return;
		}
		pos++;
	}
}

// getColumn
// Lay doi tuong column
uiListViewColumn* uiListView::getColumn( DWORD i )
{
	if ( i >= (DWORD) zm_listColumn.size() )
		return NULL;

	return zm_listColumn[i];
}

// getRow
// Lay doi tuong row
uiListViewRow* uiListView::getRow( DWORD i )
{
	if ( i >= (DWORD) zm_listItem.size() )
		return NULL;
	
#pragma warning(disable:4800)
	((uiListViewRow*)zm_listItem[i])->setCheckBox( (bool)ListView_GetCheckState( this->getHandle(),  i ) );

	return (uiListViewRow*)zm_listItem[i];
}

// getRowCount
// Dem so luong dong
DWORD uiListView::getRowCount()
{
	return (DWORD) zm_listItem.size();
}

// getColCount
// Dem so luong cot
DWORD uiListView::getColCount()
{
	return (DWORD) zm_listColumn.size();
}

// deleteColumn
// Xoa cot thu index
void uiListView::deleteColumn( DWORD nIndex )
{
	if (nIndex >= zm_listColumn.size() )
		return;

	ListView_DeleteColumn( this->getHandle(), nIndex );
	
	vector<uiListViewColumn*>::iterator i = zm_listColumn.begin() + nIndex;
	delete (*i);
	zm_listColumn.erase( i );
}

// deleteRow
// Xoa dong thu index
void uiListView::deleteRow(DWORD nIndex )
{
	ListView_DeleteItem( this->getHandle(), nIndex );
	this->removeAbstractItem(nIndex);
}

// deleteAllRow
// Xoa tat ca cac hang
void uiListView::deleteAllRow()
{
	ListView_DeleteAllItems( this->getHandle() );
	
	uiAbstractArrayItem::iterator i = zm_listItem.begin();
	while ( i!= zm_listItem.end() )
	{
		delete (*i);
		i++;
	}

}

// setSelectedFullRow
// Thiet lap select full het dong hay khong?
void uiListView::selectedFullRow(bool b)
{	
#if !defined( LVS_EX_FULLROWSELECT )
	#define LVS_EX_FULLROWSELECT    0x00000020
#endif

	this->setStyleEx( LVS_EX_FULLROWSELECT, b );	
}

// getRowSelect
// Lay danh sach cac row duoc select
void uiListView::getRowSelected( uiListViewRowSelected* select )
{
	// Xoa het danh sach select dang co
	select->clear();
	
	if ( this->getRowSelectedCount() == 0 )
		return;

	DWORD dwSelect = ListView_GetNextItem( this->getHandle(), 0, LVNI_SELECTED );		

	do
	{
		// Lay doi tuong row
		uiListViewRow *row = this->getRow(dwSelect);

		// Thiet lap vi tri index
		row->setIndex( dwSelect );

		// Luu vao danh sach
		select->push_back( row );

		// Tim row select tiep theo
		dwSelect = ListView_GetNextItem( this->getHandle(), dwSelect, LVNI_SELECTED );
	}
	while ( dwSelect != 0 );

}

// getRowChecked
// Lay danh sach cac row duoc check trong checkbox
void uiListView::getRowChecked( uiListViewRowSelected* select )
{
	select->clear();
	
	vector<uiAbstractListItem*>::iterator i = zm_listItem.begin();

	while ( i != zm_listItem.end() )
	{
		uiListViewRow *r = (uiListViewRow*) (*i);
		
		if ( r->getCheckBox() )
			select->push_back( r ) ;

		i++;
	}

}

// getRowSelectCount
// Lay so dong duoc select
DWORD uiListView::getRowSelectedCount()
{
	return ListView_GetSelectedCount( this->getHandle() );
}

// setSelectedCheckbox
// Thiet lap checkbox select
void uiListView::selectedCheckbox(bool b)
{
#if !defined(LVS_EX_CHECKBOXES)
	#define LVS_EX_CHECKBOXES       0x00000004
#endif

	this->setStyleEx( LVS_EX_CHECKBOXES, b );
}

// setStyleEx
// Thay doi style
void uiListView::setStyleEx( DWORD exStyle, bool b )
{	
#if !defined (LVM_SETEXTENDEDLISTVIEWSTYLE)
	#define LVM_SETEXTENDEDLISTVIEWSTYLE	(LVM_FIRST + 54)
#endif
#if !defined (LVM_GETEXTENDEDLISTVIEWSTYLE)
	#define LVM_GETEXTENDEDLISTVIEWSTYLE	(LVM_FIRST + 55)
#endif

	DWORD dwStyle = (DWORD)SendMessageW(this->getHandle(), LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	
	if ( b )
		dwStyle |= exStyle;
	else
		dwStyle &= ~exStyle;

	SendMessageW(this->getHandle(), LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
}

// getCellRect
// Lay kich thuoc 1 o
void uiListView::getCellRect( DWORD row, DWORD col, RECT *r )
{
#if !defined(LVM_GETSUBITEMRECT)
	#define LVM_GETSUBITEMRECT      (LVM_FIRST + 56)
#endif	
#if !defined(ListView_GetSubItemRect)
	#define ListView_GetSubItemRect(hwnd, iItem, iSubItem, code, prc) \
        (BOOL)SendMessageW((hwnd), LVM_GETSUBITEMRECT, (WPARAM)(int)(iItem), \
                ((prc) ? ((((LPRECT)(prc))->top = iSubItem), (((LPRECT)(prc))->left = code), (LPARAM)(prc)) : (LPARAM)(LPRECT)NULL))
#endif

	ListView_GetSubItemRect( this->getHandle(), row, col,LVIR_BOUNDS, r ); 	
}

// usingImageList
// Cho phep su dung ImageList hay khong
void uiListView::usingImageList( bool b )
{
	if ( b == true )
	{
		TreeView_SetImageList( this->getHandle(), this->getHandleLargeIcon(), LVSIL_NORMAL ); 
		TreeView_SetImageList( this->getHandle(), this->getHandleSmallIcon(), LVSIL_SMALL );
	}
	else
	{
		TreeView_SetImageList( this->getHandle(), NULL, LVSIL_NORMAL ); 
		TreeView_SetImageList( this->getHandle(), NULL, LVSIL_SMALL );
	}

}