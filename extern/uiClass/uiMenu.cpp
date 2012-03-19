// uiMenu.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 18/12/2008
// Thong tin:

#include "uiMenu.h"
#include "uiInclude.h"
#include "uiString.h"
#include "uiWindow.h"
#include "uiApplication.h"

using namespace uiClass;

///////////////////////////////////////////////////////////////////////////////
//// uiMenuObject
///////////////////////////////////////////////////////////////////////////////

uiMenuObject::uiMenuObject()
{
	uiString::copy<WCHAR,WCHAR>(zm_lpLabel,L"");
}

uiMenuObject::~uiMenuObject()
{

}

// createMenuID
// Phat sinh 1 so ID cho menu
DWORD uiMenuObject::createMenuID()
{
	// Thiet lap idMenu
	static DWORD dwMenuID = UICONTROL_MENU;
	
	// Dang ky ID
	uiApplication::objectRegister( dwMenuID, this );

	return dwMenuID++;
}

// setPos
// Cap nhat lai vi tri position
void uiMenuObject::setPos(DWORD pos)
{
	zm_MenuPos = pos;	
}

// getID
// Lay ID cua menu
DWORD uiMenuObject::getID()
{
	return zm_MenuID;
}

// getPos
// Lay vi tri position
DWORD uiMenuObject::getPos()
{
	return zm_MenuPos;
}

// getMenuLabel
// Lay tieu de menu
void uiMenuObject::getMenuLabel(LPWSTR lpLabel)
{
	uiString::copy<WCHAR,WCHAR>(lpLabel, zm_lpLabel);
}

// doEvent
// Thuc thi su kien
void uiMenuObject::doEvent()
{
	// Thuc thi event
	eventOnClicked( this );
}

// setDisable
// An menu
void uiMenuObject::setDisable ( bool b )
{
	if (zm_pParent == NULL)
		return;
	else
	{
		if ( b )
			zm_dwFlags = MF_GRAYED|MF_BYPOSITION;
		else
			zm_dwFlags = MF_ENABLED|MF_BYPOSITION;
		
		EnableMenuItem( zm_pParent->getHandle(), zm_MenuPos, zm_dwFlags );		
	}
}

///////////////////////////////////////////////////////////////////////////////
//// uiMenuPopup
///////////////////////////////////////////////////////////////////////////////

uiMenuPopup::uiMenuPopup()
{
	zm_pParent = NULL;

	// Khoi tao menu
	zm_hMenu = CreatePopupMenu();
}

// getObject
// Tra ve UIOBJ_
DWORD uiMenuPopup::getObject()
{
	return UIOBJECT_MENUPOPUP;
}

// popup
// Popup menu tai 1 vi tri nao do
void uiMenuPopup::popup(uiWindow *p, int x, int y, RECT *r) 
{
	HWND h = 0;
	if ( p )
		h = p->getHandle();

	TrackPopupMenu( this->getHandle(), TPM_LEFTALIGN|TPM_RIGHTBUTTON, x, y, 0, h, r);
}

///////////////////////////////////////////////////////////////////////////////
//// uiMenu
///////////////////////////////////////////////////////////////////////////////

// getObject
// Tra ve const UIOBJECT_MENU
DWORD uiMenu::getObject()
{
	return UIOBJECT_MENU;
}

uiMenu::uiMenu()
{
	zm_pParent = NULL;

	// Khoi tao menu
	zm_hMenu = CreateMenu();
}

uiMenu::uiMenu(uiMenu *pParent, LPWSTR lpLabel, DWORD pos)
{
	// Khoi tao menu
	zm_hMenu = CreateMenu();

	// Thiet lap label
	uiString::copy<WCHAR,WCHAR>(zm_lpLabel,lpLabel);
	
	zm_MenuPos = pos;
	zm_pParent = pParent;
	zm_dwFlags = MF_STRING|MF_POPUP;

	InsertMenuW( pParent->getHandle(), pos ,zm_dwFlags|MF_BYPOSITION, (UINT_PTR) zm_hMenu, lpLabel );
}

uiMenu::~uiMenu()
{
	vector<uiMenuObject*>::iterator i;
	uiMenuObject *p;
	for (i = zm_aChildMenu.begin(); i != zm_aChildMenu.end(); i++)
	{
		p = (*i);
		delete p;
	}
	zm_aChildMenu.clear();
	DestroyMenu( zm_hMenu );

}

// getHandle
// Lay gia tri HMENU
HMENU uiMenu::getHandle()
{
	return zm_hMenu;
}

// appendMenuItem
// Khoi tao 1 menuItem
uiMenuItem *uiMenu::appendMenuItem(LPWSTR lpLabel)
{
	// Khoi tao 1 menu
	uiMenuItem *pItem = new uiMenuItem(this, lpLabel, (DWORD) zm_aChildMenu.size() );
	zm_aChildMenu.push_back( pItem );
	
	return pItem;
}


// appendMenuItem
uiMenuItem *uiMenu::appendMenuItem()
{
	// Khoi tao 1 menu
	uiMenuItem *pItem = new uiMenuItem(this, (DWORD) zm_aChildMenu.size() );
	zm_aChildMenu.push_back( pItem );
	
	return pItem;
}

// appendMenuPopup
// Khoi tao 1 menu popup
uiMenu *uiMenu::appendMenuPopup(LPWSTR lpLabel)
{
	uiMenu *pPopupMenu = new uiMenu(this, lpLabel, (DWORD) zm_aChildMenu.size() );
	zm_aChildMenu.push_back( pPopupMenu );
	
	return pPopupMenu;
}

// insertMenuItem
// Chen 1 menu item vao 1 vi tri nao do
uiMenuItem* uiMenu::insertMenuItem(LPWSTR lpLabel, DWORD pos)
{
	// Chen 1 menuItem vao vi tri pos
	uiMenuItem *pItem = new uiMenuItem(this, lpLabel, pos );
	this->insertMenuObject(pItem, pos);

	return pItem;
}

// insertMenuItem
// Chen 1 menu dang Separator vao 1 vi tri nao do
uiMenuItem* uiMenu::insertMenuItem(DWORD pos)
{
	// Chen 1 menuItem vao vi tri pos
	uiMenuItem *pItem = new uiMenuItem(this, pos );
	this->insertMenuObject(pItem, pos);

	return pItem;
}

// insertMenuPopup
// Khoi tao 1 menu popup
uiMenu* uiMenu::insertMenuPopup(LPWSTR lpLabel, DWORD pos)
{
	uiMenu *pPopupMenu = new uiMenu(this, lpLabel, pos );
	this->insertMenuObject(pPopupMenu, pos);
	
	return pPopupMenu;
}


// insertMenuObject
// Chen pMenu vao mang zm_aChildMenu
bool uiMenu::insertMenuObject(uiMenuObject* pMenu, DWORD pos)
{	
	DWORD posInsert = 0;
	vector<uiMenuObject*>::iterator i;
	
	if ( pos >= (DWORD) zm_aChildMenu.size() )	
		i = zm_aChildMenu.end();
	else
		i = zm_aChildMenu.begin() + pos;
	
	
	// Chen pMenu vao vi tri
	zm_aChildMenu.insert(i, pMenu);
	// Cap nhat thu tu menu
	this->updateMenuItemPos();

	return true;
}

// updateMenuItemPos
// Cap nhat lai vi tri position cua menuItem (sau khi delete hay insert)
void uiMenu::updateMenuItemPos()
{
	DWORD pos = 0;
	vector<uiMenuObject*>::iterator i = zm_aChildMenu.begin();
	
	// Cap nhat vi tri cho menuItem
	for (; i != zm_aChildMenu.end(); i++)
	{
		(*i)->setPos( pos );
		pos++;
	}

}

// getItem
// Lay doi tuong menuItem hoac menuPopup
uiMenuObject* uiMenu::getItem(DWORD pos)
{
	if ( pos >= (DWORD)zm_aChildMenu.size() )
		return NULL;

	return zm_aChildMenu[ pos ];
}

// removeMenu
// Loai bo 1 menu item
void uiMenu::removeMenu(DWORD pos)
{
	if ( pos >= (DWORD)zm_aChildMenu.size() )
		return;

	RemoveMenu( this->getHandle(), pos, MF_BYPOSITION );
	this->removeMenuObject( pos );
}

// removeMenuObject
// Loai bo 1 menu
void uiMenu::removeMenuObject(DWORD pos)
{		
	vector<uiMenuObject*>::iterator i;
	
	if ( pos >= (DWORD) zm_aChildMenu.size() )
		return;
	
	i = zm_aChildMenu.begin() + pos;
	
	zm_aChildMenu.erase(i);
	delete (*i);
		
}

// modifyLabel
// Thay doi ten cua menu
void uiMenu::modifyLabel(LPWSTR lpLabel)
{
	if (zm_pParent == NULL)
		return;
	else
	{
		zm_dwFlags = MF_STRING;
		uiString::copy<WCHAR,WCHAR>(zm_lpLabel, lpLabel);

		ModifyMenuW( zm_pParent->getHandle(), zm_MenuPos, MF_STRING|MF_BYPOSITION, zm_MenuID, lpLabel );
	}
}

// setCheck
// Check menu item
void uiMenu::setCheck(DWORD pos,bool bCheck)
{
	uiMenuObject *p = this->getItem(pos);
	
	if (p == NULL)
		return;

	if ( p->getObject() == UIOBJECT_MENUITEM )
		((uiMenuItem*)p)->setCheck(bCheck);
}
		
// getCheck
// Kiem tra menu co check hay ko
bool uiMenu::getCheck(DWORD pos)
{
	uiMenuObject *p = this->getItem(pos);
	
	if (p == NULL)
		return false;

	if ( p->getObject() == UIOBJECT_MENUITEM )	
		return ((uiMenuItem*)p)->getCheck();
	
	return false;
}
