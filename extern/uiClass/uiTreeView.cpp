// uiTreeView.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 7/2/2009
// Thong tin:

#include "uiTreeView.h"
#include "uiInclude.h"
#include "uiString.h"
#include "uiApplication.h"

#include <algorithm>

using namespace uiClass;


////////////////////////////////////
//// uiTreeViewItem
////////////////////////////////////

uiTreeViewItem::uiTreeViewItem(uiTreeView* pControl, uiTreeViewItem *father)
	:uiComboBoxItem( pControl )
{
	zm_control		= pControl;
	zm_father		= father;
	zm_bChildExpand = false;
	zm_nIconState	= -1;
	zm_nIcon		= -1;
	zm_hItem		= 0;
}

uiTreeViewItem::~uiTreeViewItem()
{
	uiListTreeViewItem::iterator i = zm_child.begin();
	while ( i != zm_child.end() )
	{
		delete (*i);
		i++;
	}
}

void uiTreeViewItem::setHandleTreeItem( HTREEITEM h )
{
	zm_hItem = h;
}

// destroyChild
// Loai bo tai nguyen cho item
void uiTreeViewItem::destroyChild( uiTreeViewItem* p )
{
	uiListTreeViewItem::iterator i = zm_child.begin();

	while ( i != zm_child.end() )
	{
		if ( (*i) == p )
		{
			TreeView_DeleteItem( this->zm_control->getHandle(),(*i)->getHandle() );
			zm_child.erase( i );
			delete p;
			return;
		}

		i++;
	}
	
}

// deleteAllChild
// Xoa tat ca cac child item cua no
void uiTreeViewItem::deleteAllChild()
{
	uiListTreeViewItem::iterator i = zm_child.begin();
	
	// Huy doi tuong
	while ( i != zm_child.end() )
	{
		TreeView_DeleteItem( this->zm_control->getHandle(),(*i)->getHandle() );
		delete (*i);
	}

	// Go bo danh sach
	zm_child.clear();
}

// expandChild
// Mo rong hoac thu nho node
void uiTreeViewItem::expandChild( bool b )
{	
	if ( b )
		TreeView_Expand( this->zm_control->getHandle(), getHandle(), TVE_EXPAND );
	else
		TreeView_Expand( this->zm_control->getHandle(), getHandle(), TVE_COLLAPSE );
}


// getChildCount
// So luong child
DWORD uiTreeViewItem::getChildCount()
{
	return (DWORD) zm_child.size();
}

// getChild
// Lay doi tuong Child Item
uiTreeViewItem* uiTreeViewItem::getChild( DWORD nIndex )
{
	if ( nIndex >= this->getChildCount() )
		return NULL;
	else
		return zm_child[nIndex];
}

// getHandle
// Lay handle HTREEITEM
HTREEITEM uiTreeViewItem::getHandle()
{
	return zm_hItem;
}

// getInsertStruct
// Lay struct cua treeviewItem
LPTV_INSERTSTRUCTW uiTreeViewItem::getInsertStruct()
{	

	memset( &zm_insertStruct, 0, sizeof( zm_insertStruct ) );

	// Neu la insert -> xac dinh vi tri chen
	// Nguoc lai la update
	if ( zm_hItem == 0 )
	{
		// Father	
		if ( zm_father == NULL )
		{
			zm_insertStruct.hInsertAfter = TVI_ROOT;
			zm_insertStruct.hParent = NULL;
		}
		else
		{
			zm_insertStruct.hInsertAfter = TVI_LAST;
			zm_insertStruct.hParent = zm_father->getHandle();
		}
	}

	// String & icon
	zm_insertStruct.item.pszText			= zm_lpString;
	zm_insertStruct.item.mask				= TVIF_TEXT;	

	if ( zm_nIcon != -1 )
	{
		zm_insertStruct.item.iImage			= zm_nIcon;		
		zm_insertStruct.item.mask			|= TVIF_IMAGE;
	}
	
	if ( zm_nIconState != -1 )
	{
		zm_insertStruct.item.iSelectedImage	= zm_nIconState;
		zm_insertStruct.item.mask			|= TVIF_SELECTEDIMAGE;
	}

	// Cho hien thi thi dau [+] hay ko?
	if ( zm_bChildExpand )
		zm_insertStruct.item.mask |= TVIF_CHILDREN;
	else
		zm_insertStruct.item.mask &= ~ TVIF_CHILDREN;

	if ( zm_hItem != 0 )
	{
		zm_insertStruct.item.hItem		= zm_hItem;
		zm_insertStruct.item.mask		|= TVIF_HANDLE;
	}
		
	return &zm_insertStruct;
}

// getFather
// Lay doi tuong father
uiTreeViewItem* uiTreeViewItem::getFather()
{
	return zm_father;
}

void uiTreeViewItem::setChildExpand( bool b )
{
	zm_bChildExpand = b;
}

// addChild 
// Them 1 children
uiTreeViewItem *uiTreeViewItem::addChild ( LPWSTR lpString, LPWSTR lpValue )
{
	uiTreeViewItem *p = new uiTreeViewItem ( zm_control, this );

	p->setString( lpString );
	p->setValue( lpValue );	
	
	zm_child.push_back( p );

	HTREEITEM hInsert = TreeView_InsertItem( zm_control->getHandle(), p->getInsertStruct() );	
	p->setHandleTreeItem( hInsert );	
	
	// Map item nay lai!
	uiMapTreeViewItem* map = zm_control->getMapItem();
	(*map)[hInsert] = p;

	return p;
}

// addChild 
// Them 1 children
uiTreeViewItem *uiTreeViewItem::addChild ( LPWSTR lpString )
{	
	return this->addChild(lpString,L"") ;
}

// setIconIndex
// Thiet lap icon 
void uiTreeViewItem::setIconIndex(int n)
{
	zm_nIcon = n;
}
		
// setIconStateIndex
// Thiet lap icon trong trang thai open
void uiTreeViewItem::setIconStateIndex(int n)
{
	zm_nIconState = n;
}

// getIconIndex
// Lay gia tri Icon
int uiTreeViewItem::getIconIndex()
{
	return zm_nIcon;
}

// getIconStateIndex
// Lay gia tri Icon trong trang thai open
int uiTreeViewItem::getIconStateIndex()
{
	return zm_nIconState;
}

void uiTreeViewItem::setString( LPWSTR lpString )
{
	uiString::copy<WCHAR,WCHAR>(zm_lpString,lpString);	
}






////////////////////////////////////
//// uiTreeView
////////////////////////////////////

uiTreeView::uiTreeView(LPCWSTR lpTitle, int x, int y, int w, int h, bool bIcon, uiWindow* pParent)
{
	this->create( lpTitle, x,y,w,h, bIcon, pParent );
}

uiTreeView::~uiTreeView()
{

}
		
// getObject
// Tra ve const UI
DWORD uiTreeView::getObject()
{
	return UIOBJECT_TREEVIEW;
}

// create
// Khoi tao listView
bool uiTreeView::create(LPCWSTR lpTitle, int x, int y, int w, int h, bool bIcon, uiWindow* pParent)
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
		L"SysTreeView32",
		lpTitle,
		WS_CHILD | WS_TABSTOP | WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
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
	
	// Thiet lap icon
	if ( bIcon )
	{
		TreeView_SetImageList( this->getHandle(), this->getHandleSmallIcon(), TVSIL_NORMAL ); 
		TreeView_SetImageList( this->getHandle(), this->getHandleSmallIcon(), TVSIL_STATE );		
	}

	return true;
}

// messageControlMap
// Phan loai su kien cho Control
void uiTreeView::messageControlMap(UINT uMsg)
{

}

// notifyControl
// Phan loai su kien cho Control
void uiTreeView::notifyControl( LPARAM lParam )
{
	LPNMTREEVIEWW pnm = (LPNMTREEVIEWW)lParam;
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
	
	case TVN_KEYDOWN:
		eventOnKeyDown( this );
		break;
	case TVN_ITEMEXPANDEDW:
		eventOnExpanded( this );
		break;
	case TVN_SELCHANGEDW:
		eventOnSelectChange( this );
		break;
	}
	
	uiControl::notifyControl(lParam);
}

// addItem
// Them 1 item vao TreeView
uiTreeViewItem* uiTreeView::addItem( LPWSTR lpString, LPWSTR lpValue )
{
	uiTreeViewItem *p = new uiTreeViewItem(this, NULL);	
	zm_listItem.push_back ( p );
	
	p->setString( lpString );	
	p->setValue( lpValue );

	// Them item vao control	
	HTREEITEM hInsert = TreeView_InsertItem( this->getHandle(), p->getInsertStruct() );
	p->setHandleTreeItem( hInsert );
	
	// Map item nay!
	zm_mapTree[hInsert] = p;

	return p;
}

// addItem
// Them 1 item vao TreeView
uiTreeViewItem* uiTreeView::addItem( LPWSTR lpString)
{	
	return this->addItem(lpString,L"" ) ;
}

// getHisTest
// Lay doi tuong duoc click chuot
uiTreeViewItem* uiTreeView::getHisTest( int x, int y )
{
	TVHITTESTINFO hitTestInfo;
	hitTestInfo.pt.x = x;
	hitTestInfo.pt.y = y;

	HTREEITEM item = TreeView_HitTest( this->getHandle(), &hitTestInfo );
	
	return zm_mapTree[item];
}

// update
// Cap nhat item
void uiTreeView::updateItem( uiAbstractListItem* pItem )
{
	uiTreeViewItem* treeItem = (uiTreeViewItem*) pItem;
	TreeView_SetItem( this->getHandle(), & (treeItem->getInsertStruct()->item) );	
}

// usingImageList
// Cho phep su dung ImageList hay khong
void uiTreeView::usingImageList( bool b )
{
	if ( b == true )
	{
		TreeView_SetImageList( this->getHandle(), this->getHandleSmallIcon(), TVSIL_NORMAL ); 
		TreeView_SetImageList( this->getHandle(), this->getHandleSmallIcon(), TVSIL_STATE );
	}
	else
	{
		TreeView_SetImageList( this->getHandle(), NULL, TVSIL_NORMAL ); 
		TreeView_SetImageList( this->getHandle(), NULL, TVSIL_STATE );
	}

}

// deleteItem
// Xoa treeview Item
void uiTreeView::deleteItem( uiTreeViewItem* p )
{	
	TreeView_DeleteItem( this->getHandle(), p->getHandle() );

	// Root Item
	if ( p->getFather() == NULL )
	{
		// Tim vi tri xoa
		uiAbstractArrayItem::iterator i = zm_listItem.begin();
		while ( i != zm_listItem.end() )
		{
			// Thay duoc vi tri xoa
			if ( (*i) == p )
			{
				// Xoa doi tuong Item
				zm_listItem.erase( i );
				delete p;
				return;
			}

			i++;
		}
		
	}
	// Leaf Item
	else
		p->getFather()->destroyChild( p );
}

// getChildCount
// So luong child
DWORD uiTreeView::getChildCount()
{
	return (DWORD) zm_listItem.size();
}

// getMapItem
// Lay doi tuong std::map giua HTREEVIEW va uiTreViewItem
uiMapTreeViewItem* uiTreeView::getMapItem()
{
	return &zm_mapTree;
}

// getChild
// Lay doi tuong Child Item
uiTreeViewItem* uiTreeView::getChild( DWORD nIndex )
{
	if ( nIndex >= this->getChildCount() )
		return NULL;
	else
		return (uiTreeViewItem*) zm_listItem[nIndex];
}

// getItemSelected
// Lay danh sach cac item dang select
void uiTreeView::getItemSelected( uiListTreeViewItem* list )
{
	list->clear();
	HTREEITEM hItem = TreeView_GetNextItem( this->getHandle(), 0, TVGN_CARET );

	while ( hItem != 0 )
	{
		uiTreeViewItem *p = zm_mapTree[ hItem ];
		if ( p != NULL )		
			list->push_back( p );		

		HTREEITEM old = hItem;
		hItem = TreeView_GetNextItem( this->getHandle(), hItem, TVGN_CARET );

		if ( hItem == old )
			hItem = 0;
	}

}

// selectItem
// Lua chon item
void uiTreeView::selectItem( uiTreeViewItem *item )
{
	TreeView_Select( this->getHandle(), item->getHandle(), TVM_SELECTITEM );
}

// deleteAllItem
// Xoa tat ca cac item
void uiTreeView::deleteAllItem()
{
	TreeView_DeleteAllItems( this->getHandle() );
}