// uiListView.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 10/1/2009
// Thong tin:

#ifndef _UILISTVIEW_
#define _UILISTVIEW_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"
#include "uiAbstractList.h"
#include <vector>
#include <string>

using std::vector;
using std::wstring;

namespace uiClass
{
	class uiListViewColumn;
	class uiListViewRow;
	class uiListView;

	typedef vector< uiListViewRow* >	uiListViewRowSelected;

	// uiListViewColumn
	// Doi tuong the hien cot cua listview
	class DLLEXPORT uiListViewColumn: public uiObject
	{
	protected:
		WCHAR		zm_lpText[100];
		UINT		zm_nFlags;
		UINT		zm_nWidth;
		int			zm_nMFT;
		int			zm_nIcon;

		uiListView	*zm_pListView;
	public:
		// getObject
		// Lay gia tri UIOBJECT_
		virtual DWORD getObject();
		
		uiListViewColumn( LPWSTR lpText, UINT nWidth, bool isIcon, uiListView *pList);
		virtual ~uiListViewColumn();

		// setText
		// Thiet lap gia tri text
		void setText( LPWSTR lpText );

		// getText
		// Lay text
		void getText( LPWSTR lpText );
		
		// getText
		// Lay gia tri text
		LPWSTR getText();

		// setWidth
		// Thiet lap chieu rong
		void setWidth( UINT nWidth );

		// getWidth
		// Lay gia tri chieu rong
		UINT getWidth();

		// setAlgin
		// Thiet lap canh le cho title column
		// Tham so: align la const UILISTVIEW_MFT
		void setMFT( int nMFT );

		// getAlign
		// Tra ve UILISTVIEW_ALIGN
		int getMFT();

		// setIcon
		// Thiet lap icon 	
		void setIcon( int nIcon );

		// getFlags
		// Lay gia tri flags
		UINT getFlags();

		// getIcon
		// Lay icon hien hanh
		int getIcon();
		
		// update
		// Cap nhat thong tin column
		void update();
	};

	// uiRowItem
	// Du lieu tren 1 row
	class DLLEXPORT uiListViewRow: public uiAbstractListItem
	{
	protected:
		// Danh sach cac tham so string
		std::vector< std::wstring > zm_params;		
		bool						zm_bCBSelected;
		bool						zm_isSelected;
		
		DWORD		zm_nIndex;
		int			zm_nIconIndex;
	public:
		uiListViewRow( uiListView* lsview );		
		virtual ~uiListViewRow();

		// getObject
		// Lay gia tri Object
		DWORD getObject();

		// addParam
		// Tham 1 tham so
		void addParam( LPWSTR lpString );

		// getParam
		// Lay gia tri cua tham so
		bool getParam( UINT i, LPWSTR lpString );
		
		// getParamCount
		// So luong tham so
		UINT getParamCount();

		// setParam
		// Thiet lap tham so
		void setParam( UINT i, LPWSTR lpString );

		// clearParam
		// Xoa param
		void clearParam();
		
		// deleteParam
		// Xoa param thu i
		void deleteParam( UINT i );		

		// setIndex
		// Thiet lap vi tri index listview
		void setIndex( DWORD nIndex );

		// getIndex
		// Lay vi tri index trong listview
		DWORD getIndex();

		// setCheckBox
		// Thiet lap checkbox
		void setCheckBox( bool b );

		// getCheckBox
		// Lay gia tri checkbox
		bool getCheckBox();

		// getIconIndex
		// Lay icon hien hanh
		int getIconIndex();

		// setIconIndex
		// Thiet lap icon
		void setIconIndex(int nIndex);
	};


	// uiListView
	// Doi tuong listview
	class DLLEXPORT uiListView: public uiIconListControl
	{
	protected:		
		// Danh sach cac column
		vector<uiListViewColumn*>	zm_listColumn;			

		uiListView();	
		
		// Su kien
		uiEventCallback		eventOnClicked;
		uiEventCallback		eventOnRClicked;
		uiEventCallback		eventOnDbClicked;
		uiEventCallback		eventOnDbRClicked;
		uiEventCallback		eventOnRowChanged;
		uiEventCallback		eventOnColClicked;
		uiEventCallback		eventOnKeyDown;

		bool				zm_isHaveIcon;
	public:
		
		uiListView(LPCWSTR lpTitle, int x, int y, int w, int h, bool bIcon, uiWindow* pParent);
		virtual ~uiListView();
		
		// getObject
		// Tra ve const UI
		virtual DWORD getObject();

		// create
		// Khoi tao listView
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, bool bIcon, uiWindow* pParent);
		
		// messageControlMap
		// Phan loai su kien cho Control
		virtual void messageControlMap(UINT uMsg);	

		// notifyControl
		// Phan loai su kien cho Control
		virtual void notifyControl( LPARAM lParam );
		
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

		// Su kien OnRowChanged
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnRowChanged(T* pObject){	eventOnRowChanged.setEvent<T,pTMethod>( pObject );	}

		// Su kien OnColClicked
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnColClicked(T* pObject){	eventOnColClicked.setEvent<T,pTMethod>( pObject );	}

		// Su kien OnKeyDown
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnKeyDown(T* pObject){		eventOnKeyDown.setEvent<T,pTMethod>( pObject );	}


		// listAddColumn
		// Them 1 cot
		uiListViewColumn* addColumn( LPWSTR lpTitle, UINT nWidth );
		
		// listInsertColumn
		// Them 1 cot tai vi tri index
		uiListViewColumn* insertColumn( LPWSTR lpTitle, UINT nWidth, DWORD index );

		// addRow
		// Them 1 row
		uiListViewRow* addRow( LPWSTR lpTitle );

		// insertRow
		// Them 1 row tai vi tri nao do
		uiListViewRow* insertRow(DWORD nIndex, LPWSTR lpTitle );

		// updateColumn
		// Cap nhat lai column
		void updateColumn( uiListViewColumn *col );		

		// getColumn
		// Lay doi tuong column
		uiListViewColumn* getColumn( DWORD i );

		// getRow
		// Lay doi tuong row
		uiListViewRow* getRow( DWORD i );

		// getRowCount
		// Dem so luong dong
		DWORD getRowCount();

		// getColCount
		// Dem so luong cot
		DWORD getColCount();

		// deleteColumn
		// Xoa cot thu index
		void deleteColumn( DWORD nIndex );

		// deleteRow
		// Xoa dong thu index
		void deleteRow(DWORD nIndex );

		// deleteAllRow
		// Xoa tat ca cac hang
		void deleteAllRow();

		// selectedFullRow
		// Thiet lap select full het dong hay khong?
		void selectedFullRow(bool b);

		// selectedCheckbox
		// Thiet lap select bang checkbox hay khong
		void selectedCheckbox(bool b);

		// setStyleEx
		// Thay doi style
		void setStyleEx( DWORD exStyle, bool b );		

		// getRowSelect
		// Lay danh sach cac row duoc select
		void getRowSelected( uiListViewRowSelected* select );

		// getRowCheck
		// Lay danh sach cac row duoc check trong checkbox
		void getRowChecked( uiListViewRowSelected* select );

		// getRowSelectCount
		// Lay so dong duoc select
		DWORD getRowSelectedCount();

		// getCellRect
		// Lay kich thuoc 1 o
		void getCellRect( DWORD row, DWORD col, RECT *r );

		// update
		// Cap nhat item
		virtual void updateItem( uiAbstractListItem* pItem );

		// usingImageList
		// Cho phep su dung IMGLIST
		virtual void usingImageList( bool b );
	};
}

#endif
