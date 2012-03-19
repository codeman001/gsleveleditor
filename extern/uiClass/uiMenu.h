// uiWindow.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 18/12/2008
// Thong tin:
//

#ifndef _UIMENU_
#define _UIMENU_

#include "uiInclude.h"
#include "uiString.h"
#include "uiObject.h"
#include "uiEventCallback.h"

using std::vector;
#pragma warning(disable: 4251)

namespace uiClass
{
	// Prototype ////////////////////////
	class uiMenuObject;
	class uiMenu;
	class uiMenuItem;
	class uiWindow;
	/////////////////////////////////////

#pragma region "uiMenuObject class"
	// uiMenuObject
	// Doi tuong menu abstract
	class DLLEXPORT uiMenuObject: public uiObject
	{
	protected:
		DWORD zm_MenuID;
		DWORD zm_MenuPos;
		
		WCHAR zm_lpLabel[300];
		DWORD zm_dwFlags;
		uiMenu* zm_pParent;			
		
		uiEventCallback		eventOnClicked;
		
		// createMenuID
		// Phat sinh 1 so ID cho menu
		DWORD createMenuID();
	public:
		// setPos
		// Cap nhat lai vi tri position
		void setPos(DWORD pos);

		// getPos
		// Lay vi tri position
		DWORD getPos();
		
		// getID
		// Lay ID cua menu
		DWORD getID();

		// getMenuLabel
		// Lay tieu de menu
		void getMenuLabel(LPWSTR lpLabel);
		
		// modifyLabel
		// Thay doi ten cua menu
		virtual void modifyLabel(LPWSTR lpLabel) = 0;
		
		// doEvent
		// Thuc thi su kien
		void doEvent();

		uiMenuObject();		
		virtual ~uiMenuObject();

		// setDisable 
		// Disable menu
		void setDisable ( bool b );
	};
#pragma endregion

#pragma region "uiMenu class"
	// uiMenu
	// Doi tuong menu
	class DLLEXPORT uiMenu: public uiMenuObject
	{
	protected:
		// Danh sach cac menu con
		vector<uiMenuObject*> zm_aChildMenu;

		// Hanlde menu
		HMENU	zm_hMenu;

	protected:
		// updateMenuItemPos
		// Cap nhat lai vi tri position cua menuItem (sau khi delete hay insert)
		void updateMenuItemPos();

		// insertMenuObject
		// Chen pMenu vao mang zm_aChildMenu
		bool insertMenuObject(uiMenuObject* pMenu, DWORD pos);

		// removeMenuObject
		// Loai bo 1 menu
		void removeMenuObject(DWORD pos);
	public:
		uiMenu();
		uiMenu(uiMenu *pParent, LPWSTR lpLabel, DWORD pos);

		virtual ~uiMenu();

		// getObject
		// Tra ve const UIOBJECT_MENU
		virtual DWORD getObject();

		// getHandle
		// Lay gia tri HMENU
		HMENU getHandle();

		// appendMenuItem
		// Khoi tao 1 menu item
		uiMenuItem	*appendMenuItem(LPWSTR lpLabel);

		// appendMenuItem
		// Khoi tao 1 menu dang Separator
		uiMenuItem	*appendMenuItem();

		// appendMenuPopup
		// Khoi tao 1 menu popup
		uiMenu		*appendMenuPopup(LPWSTR lpLabel);		

		// insertMenuItem
		// Chen 1 menu item vao 1 vi tri nao do
		uiMenuItem	*insertMenuItem(LPWSTR lpLabel, DWORD pos);

		// insertMenuSeparator
		// Chen 1 menu dang Separator vao 1 vi tri nao do
		uiMenuItem	*insertMenuItem(DWORD pos);

		// insertMenuPopup
		// Khoi tao 1 menu popup
		uiMenu		*insertMenuPopup(LPWSTR lpLabel, DWORD pos);

		// getItem
		// Lay doi tuong menuItem hoac menuPopup
		uiMenuObject* getItem(DWORD pos);

		// removeMenu
		// Loai bo 1 menu item
		void removeMenu(DWORD pos);

		// modifyLabel
		// Thay doi ten cua menu
		virtual void modifyLabel(LPWSTR lpLabel);

		// setCheck
		// Check menu item
		void setCheck(DWORD pos,bool bCheck);
		
		// getCheck
		// Kiem tra menu co check hay ko
		bool getCheck(DWORD pos);

	};
#pragma endregion	

#pragma region "uiMenuItem class"
	// uiMenuItem
	// Doi tuong uiMenuItem
	class DLLEXPORT uiMenuItem: public uiMenuObject
	{	
	public:		
		// Constructor khoi tao 1 menuItem
		uiMenuItem(uiMenu *pParent, LPWSTR lpLabel, DWORD pos);

		// Constructor khoi tao 1 menuSeparator
		uiMenuItem(uiMenu *pParent, DWORD pos);

		virtual ~uiMenuItem();	

		// getObject
		// Tra ve const UIOBJECT_MENUITEM
		virtual DWORD getObject();
		
		// modifyToSeparator
		// Chuyen menuItem thanh Separator
		void modifyToSeparator();
		
		// modifyLabel
		// Thay doi ten cua menuItem
		virtual void modifyLabel(LPWSTR lpLabel);
		
		// setCheck
		// Check menu item
		void setCheck(bool bCheck);
		
		// getCheck
		// Kiem tra menu co check hay ko
		bool getCheck();

		// setEventOnClick
		// Event se chay khi menu click
		template <class T,void (T::*pMethod)(uiObject*)>
		void setEventOnClick(T *pObject){ eventOnClicked.setEvent<T,pMethod> (pObject);	}
		
	};

#pragma endregion

#pragma region "uiMenuPopup class"
	
	// uiMenuPopup
	// Doi tuong menu Popup
	class DLLEXPORT uiMenuPopup: public uiMenu
	{
	public:
		uiMenuPopup();
		
		// getObject
		// Tra ve UIOBJ_
		virtual DWORD getObject();

		void popup(uiWindow *p, int x, int y, RECT *r = NULL );
	};
#pragma endregion

}

#endif
