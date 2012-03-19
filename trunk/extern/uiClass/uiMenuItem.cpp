// uiMenuItem.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 18/12/2008
// Thong tin:

#include "uiMenu.h"
#include "uiInclude.h"
#include "uiString.h"

using namespace uiClass;

// Constructor khoi tao 1 menuSeparator
uiMenuItem::uiMenuItem(uiMenu *pParent, DWORD pos)
{
	zm_pParent = pParent;

	// Thiet lap label cho menu
	zm_dwFlags = MF_SEPARATOR;	
	
	zm_MenuID = this->createMenuID();

	// Vi tri menu
	zm_MenuPos = pos;
	
	// Them menu vao menu cha
	InsertMenuW( pParent->getHandle(), pos ,zm_dwFlags|MF_BYPOSITION, zm_MenuID, 0 );	
}

// Constructor khoi tao 1 menuItem
uiMenuItem::uiMenuItem(uiMenu *pParent, LPWSTR lpLabel, DWORD pos)
{
	zm_pParent = pParent;

	// Thiet lap label cho menu
	zm_dwFlags = MF_STRING;
	uiString::copy<WCHAR,WCHAR>(zm_lpLabel, lpLabel);
	
	zm_MenuID = this->createMenuID();

	// Vi tri menu
	zm_MenuPos = pos;
	
	// Them menu vao menu cha
	InsertMenuW( pParent->getHandle(), pos ,zm_dwFlags|MF_BYPOSITION, zm_MenuID, lpLabel );
}

uiMenuItem::~uiMenuItem()
{
	
}

// getObject
// Tra ve const UIOBJECT_MENUITEM
DWORD uiMenuItem::getObject()
{
	return UIOBJECT_MENUITEM;
}

// modifyToSeparator
// Chuyen menuItem thanh Separator
void uiMenuItem::modifyToSeparator()
{
	zm_dwFlags = MF_SEPARATOR;
	ModifyMenuW( zm_pParent->getHandle(), zm_MenuPos, MF_SEPARATOR|MF_BYPOSITION, 0, 0 );	
}

// modifyLabel
// Thay doi ten cua menuItem
void uiMenuItem::modifyLabel(LPWSTR lpLabel)
{
	zm_dwFlags = MF_STRING;
	uiString::copy<WCHAR,WCHAR>(zm_lpLabel, lpLabel);

	ModifyMenuW( zm_pParent->getHandle(), zm_MenuPos, MF_STRING|MF_BYPOSITION, zm_MenuID, lpLabel );
}

// setCheck
// Check menu item
void uiMenuItem::setCheck(bool bCheck)
{	
	zm_dwFlags &= ~MF_CHECKED;
	zm_dwFlags &= ~MF_UNCHECKED;

	if (bCheck == true)
		zm_dwFlags |= MF_CHECKED;		
	else
		zm_dwFlags |= MF_UNCHECKED;

	ModifyMenuW( zm_pParent->getHandle(), zm_MenuPos, zm_dwFlags|MF_BYPOSITION, zm_MenuID, zm_lpLabel );
}

// getCheck
// Kiem tra menu co check hay ko
bool uiMenuItem::getCheck()
{
	return ( (zm_dwFlags & MF_CHECKED) == MF_CHECKED );
}