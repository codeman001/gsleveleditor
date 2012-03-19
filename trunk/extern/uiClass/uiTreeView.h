// uiListView.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 7/2/2009
// Thong tin:

#ifndef _UITREEVIEW_
#define _UITREEVIEW_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"
#include "uiAbstractList.h"
#include "uiComboBox.h"
#include <vector>
#include <string>

using std::vector;
using std::map;

namespace uiClass
{
	class uiTreeViewItem;
	class uiTreeView;

	typedef vector< uiTreeViewItem* >		uiListTreeViewItem;
	typedef map<HTREEITEM,uiTreeViewItem*>	uiMapTreeViewItem;

	// uiTreeViewItem
	// Doi tuong Item cho TreeView
	class DLLEXPORT uiTreeViewItem: public uiComboBoxItem
	{
	protected:
		uiListTreeViewItem	zm_child;
		uiTreeViewItem		*zm_father;
		uiTreeView			*zm_control;
		HTREEITEM			zm_hItem;
		bool				zm_bChildExpand;
		TV_INSERTSTRUCTW	zm_insertStruct;
		int					zm_nIcon;
		int					zm_nIconState;
	public:
		
		// destroyChild
		// Loai bo tai nguyen cho iteam
		void destroyChild( uiTreeViewItem* p );

		uiTreeViewItem(uiTreeView* pControl, uiTreeViewItem *father);
		virtual ~uiTreeViewItem();
		
		// getHandle
		// Lay handle TreeItem
		HTREEITEM getHandle();
		
		void setHandleTreeItem( HTREEITEM h );

		// getInsertStruct
		// Lay struct cua treeviewItem
		LPTV_INSERTSTRUCTW getInsertStruct();
		
		// setChildExpand
		// Cho phep hien dau [+] truoc item
		void setChildExpand( bool b );

		// addChild 
		// Them 1 children
		uiTreeViewItem *addChild ( LPWSTR lpString, LPWSTR lpValue );

		// addChild 
		// Them 1 children
		uiTreeViewItem *addChild ( LPWSTR lpString );
		
		// getChildCount
		// So luong child
		DWORD getChildCount();

		// getChild
		// Lay doi tuong Child Item
		uiTreeViewItem* getChild( DWORD nIndex );

		// setIconIndex
		// Thiet lap icon 
		void setIconIndex(int n);
		
		// setIconStateIndex
		// Thiet lap icon trong trang thai open
		void setIconStateIndex(int n);

		// getIconIndex
		// Lay gia tri Icon
		int getIconIndex();

		// getIconStateIndex
		// Lay gia tri Icon trong trang thai open
		int getIconStateIndex();

		// getFather
		// Lay doi tuong father
		uiTreeViewItem* getFather();

		// setString
		// Thiet lap string cho ComboBoxItem
		virtual void setString( LPWSTR lpString );

		// deleteAllChild
		// Xoa tat ca cac child item cua no
		void deleteAllChild();

		// expandChild
		// Mo rong hoac thu nho node
		void expandChild( bool b );
	};
	

	// uiTreeView
	// Doi tuong TreeView
	class DLLEXPORT uiTreeView: public uiIconListControl
	{
	protected:
		
		// Map item
		uiMapTreeViewItem zm_mapTree;

		// Su kien
		uiEventCallback		eventOnClicked;
		uiEventCallback		eventOnRClicked;
		uiEventCallback		eventOnDbClicked;
		uiEventCallback		eventOnDbRClicked;
		uiEventCallback		eventOnExpanded;
		uiEventCallback		eventOnSelectChange;
		uiEventCallback		eventOnKeyDown;

	public:
		uiTreeView(LPCWSTR lpTitle, int x, int y, int w, int h, bool bIcon, uiWindow* pParent);
		virtual ~uiTreeView();
		
		// messageControlMap
		// Phan loai su kien cho Control
		virtual void messageControlMap(UINT uMsg);	

		// notifyControl
		// Phan loai su kien cho Control
		virtual void notifyControl( LPARAM lParam );
		
		// getMapItem
		// Lay doi tuong std::map giua HTREEVIEW va uiTreViewItem
		uiMapTreeViewItem* getMapItem();
		
		// Su kien OnClick
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnClicked(T* pObject)	{	eventOnClicked.setEvent<T,pTMethod>( pObject );	}

		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnRClicked(T* pObject)	{	eventOnRClicked.setEvent<T,pTMethod>( pObject );	}
		
		// Su kien OnDbClick
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnDbClicked(T* pObject){	eventOnDbClicked.setEvent<T,pTMethod>( pObject );	}
		
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnDbRClicked(T* pObject){	eventOnDbRClicked.setEvent<T,pTMethod>( pObject );	}

		// Su kien eventOnExpanded
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnExpanded(T* pObject){	eventOnExpanded.setEvent<T,pTMethod>( pObject );	}		

		// Su kien OnKeyDown
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnKeyDown(T* pObject){		eventOnKeyDown.setEvent<T,pTMethod>( pObject );	}

		// Su kien OnKeyDown
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnSelectChange(T* pObject){ eventOnSelectChange.setEvent<T,pTMethod>( pObject );	}


		// getObject
		// Tra ve const UI
		virtual DWORD getObject();

		// create
		// Khoi tao listView
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, bool bIcon, uiWindow* pParent);

		// update
		// Cap nhat item
		virtual void updateItem( uiAbstractListItem* pItem );

		// addItem
		// Them 1 item vao TreeView
		uiTreeViewItem* addItem( LPWSTR lpString, LPWSTR lpValue );

		// addItem
		// Them 1 item vao TreeView
		uiTreeViewItem* addItem( LPWSTR lpString);
		
		// getHisTest
		// Lay doi tuong duoc click chuot
		uiTreeViewItem* getHisTest( int x, int y );

		// deleteItem
		// Xoa treeview Item
		void deleteItem( uiTreeViewItem* p );

		// deleteAllItem
		// Xoa tat ca cac item
		void deleteAllItem();

		// usingImageList
		// Cho phep su dung ImageList hay khong
		virtual void usingImageList( bool b );
				
		// getChildCount
		// So luong child
		DWORD getChildCount();

		// getChild
		// Lay doi tuong Child Item
		uiTreeViewItem* getChild( DWORD nIndex );

		// getItemSelected
		// Lay danh sach cac item dang select
		void getItemSelected( uiListTreeViewItem* list );

		// selectItem
		// Lua chon item
		void selectItem( uiTreeViewItem *item );
	};
}

#endif
