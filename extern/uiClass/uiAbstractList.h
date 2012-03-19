// uiAbstractList.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 2/1/2009
// Thong tin:

#ifndef _UIABSTRACTLIST_
#define _UIABSTRACTLIST_

#include <vector>

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

namespace uiClass
{
	// class prototype ///////////////////////
	class uiAbstractListItem;
	class uiAbstractListControl;
	class uiIconListControl;
	typedef std::vector<uiAbstractListItem*>	uiAbstractArrayItem;
	//////////////////////////////////////////

	// uiAbstractListItem
	// Doi tuong item
	class DLLEXPORT uiAbstractListItem:public uiObject
	{
	protected:
		uiObject				*zm_pData;
		uiAbstractListControl	*zm_pListControl;
	public:
		uiAbstractListItem( uiAbstractListControl* p );
		virtual ~uiAbstractListItem();
		// setData
		// Thiet lap data user
		void setData(uiObject *p);

		// getData
		// Thiet lap data user
		uiObject* getData();

		// updateItem
		// Cap nhat item
		virtual void update();
	};

	// uiAbstractListControl
	// Doi tuong listcontrol
	class DLLEXPORT uiAbstractListControl:public uiControl
	{
	protected:
		uiAbstractArrayItem zm_listItem;
	protected:
		// insertAbstractItem
		// Them 1 phan tu vao mang
		virtual void insertAbstractItem(int nIndex, uiAbstractListItem* pItem );

		// removeAbstractItem
		// Xoa phan tu thu nIndex
		virtual void removeAbstractItem(int nIndex);
	public:

		virtual ~uiAbstractListControl();

		// updateItem
		// Cap nhat item
		virtual void updateItem( uiAbstractListItem* pItem ) = 0;
	};	

	// uiIconListControl
	// Control chua ICON cho Listview va TreeView...
	class DLLEXPORT uiIconListControl:public uiAbstractListControl
	{
	protected:
		HIMAGELIST	zm_hLargeIcon;
		HIMAGELIST	zm_hSmallIcon;
	public:
		uiIconListControl();
		virtual ~uiIconListControl();		
		
		// pushImage
		// Them 1 hinh anh vao list
		DWORD pushLargeImage( uiBitmap* pImage );
		DWORD pushLargeImage( uiIcon*	pImage );
		DWORD pushLargeImage( uiBitmap* pImage, uiColor mask );
		
		// removeImage
		// Xoa 1 image da push vao list
		void removeLargeImage( DWORD nIndex );
		
		// pushImage
		// Them 1 hinh anh vao list
		DWORD pushSmallImage( uiBitmap* pImage );
		DWORD pushSmallImage( uiIcon*	pImage );
		DWORD pushSmallImage( uiBitmap* pImage, uiColor mask  );
		
		// removeImage
		// Xoa 1 image da push vao list
		void removeSmallImage( DWORD nIndex );

		// removeAllSmall
		// Xoa toan bo small icon
		void removeAllSmall();

		// removeAllLarge
		// Xoa toan bo large icon
		void removeAllLarge();

		// getHandle
		// Lay handle ICON lon
		HIMAGELIST getHandleLargeIcon();

		// getHandleSmall
		// Lay handle ICON nho
		HIMAGELIST getHandleSmallIcon();

		// usingImageList
		// Cho phep su dung ImageList hay khong
		virtual void usingImageList( bool b ) = 0;
	};
}

#endif
