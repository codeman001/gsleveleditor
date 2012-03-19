// uiComboBox.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 31/12/2008
// Thong tin:

#include "uiComboBox.h"
#include "uiInclude.h"
#include "uiApplication.h"
#include "uiString.h"


#include <algorithm>
#include <windowsx.h>

using namespace uiClass;

////////////////////////////////////////
/// uiComboBoxItem
////////////////////////////////////////

uiComboBoxItem::uiComboBoxItem(uiComboBox *pCombobox)
	:uiAbstractListItem(pCombobox)
{
	uiString::copy<WCHAR,WCHAR>(zm_lpString,L"");
	uiString::copy<WCHAR,WCHAR>(zm_lpValue,L"");
}

uiComboBoxItem::uiComboBoxItem(uiAbstractListControl *pListControl)
	:uiAbstractListItem(pListControl)
{
	uiString::copy<WCHAR,WCHAR>(zm_lpString,L"");
	uiString::copy<WCHAR,WCHAR>(zm_lpValue,L"");
}

uiComboBoxItem::~uiComboBoxItem()
{

}
			
// setString
// Thiet lap string cho ComboBoxItem
void uiComboBoxItem::setString( LPWSTR lpString )
{
	uiString::copy<WCHAR,WCHAR>(zm_lpString,lpString);
	this->update();
}

// getString
// Lay string
void uiComboBoxItem::getString( LPWSTR lpString )
{
	uiString::copy<WCHAR,WCHAR>(lpString,zm_lpString);
}

// setValue 
// Thiet lap gia tri cho Item
void uiComboBoxItem::setValue (LPWSTR lpString )
{
	uiString::copy<WCHAR,WCHAR>(zm_lpValue,lpString);
}

// getValue 
// Lay gia tri
void uiComboBoxItem::getValue (LPWSTR lpString )
{
	uiString::copy<WCHAR,WCHAR>(lpString,zm_lpValue);
}

// setItem
// Thiet lap string, value
void uiComboBoxItem::setItem( LPWSTR string, LPWSTR value )
{
	uiString::copy<WCHAR,WCHAR>(zm_lpString,string);
	uiString::copy<WCHAR,WCHAR>(zm_lpValue,value);
}

////////////////////////////////////////
/// uiComboBox
////////////////////////////////////////

uiComboBox::uiComboBox()
{

}

uiComboBox::uiComboBox(LPCWSTR lpTitle, int x, int y, int w, int h, bool bIsDropList, uiWindow* pParent)
{
	// Khoi tao 1 comboBox
	this->create( lpTitle, x,y,w,h, bIsDropList, pParent );
}

uiComboBox::~uiComboBox(void)
{

}
		
// getObject
// Tra ve UIOBJECT_
DWORD uiComboBox::getObject()
{
	return UIOBJECT_COMBOBOX;
}

// create
// Khoi tao combobox
bool uiComboBox::create(LPCWSTR lpTitle, int x, int y, int w, int h, bool bIsDropList, uiWindow* pParent)
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

	DWORD dwStyle;
	if (bIsDropList)
		dwStyle = CBS_DROPDOWNLIST;
	else
		dwStyle = CBS_DROPDOWN;

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
	SendMessage(zm_hWnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
               MAKELPARAM(false, 0));	

	return true;
}		

// messageControlMap
// Phan loai message
void uiComboBox::messageControlMap(UINT uMsg)
{
	switch (uMsg)
	{
	case CBN_DBLCLK:
		eventOnDbClicked(this);
		break;	
	case CBN_DROPDOWN:
		eventOnDropDown(this);
		break;
	case CBN_EDITCHANGE:
		eventOnEditChange(this);
		break;
	case CBN_EDITUPDATE:
		eventOnEditUpdate(this);
		break;
	case CBN_SELCHANGE:
		eventOnSelectChange(this);
		break;
	case CBN_KILLFOCUS:
		eventOnFocus(this);
		break;
	case CBN_SETFOCUS:
		eventOnLostFocus(this);
		break;
	}	
}

// cmbAddString
// Them 1 string vao ComboBox
uiComboBoxItem* uiComboBox::addString( LPWSTR lpString )
{
	uiComboBoxItem *pItem = new uiComboBoxItem(this);
	pItem->setItem(lpString,L"");
	
	zm_listItem.push_back( pItem );

	ComboBox_AddString( this->getHandle(), lpString );
	return pItem;
}

// cmbInsertString
// Chen 1 string vao 1 vi tri bat ky
uiComboBoxItem* uiComboBox::insertString( DWORD nIndex, LPWSTR lpString )
{
	uiComboBoxItem *pItem = new uiComboBoxItem(this);
	pItem->setItem(lpString,L"");	
	
	this->insertAbstractItem( nIndex, pItem );

	ComboBox_InsertString( this->getHandle(), nIndex, lpString );

	return pItem;
}

// addItem
// Them 1 item bao gom string va value
uiComboBoxItem*	uiComboBox::addItem( LPWSTR lpString, LPWSTR lpValue )
{
	uiComboBoxItem *pItem = new uiComboBoxItem(this);
	pItem->setItem(lpString,lpValue);
	
	zm_listItem.push_back( pItem );

	ComboBox_AddString( this->getHandle(), lpString );
	return pItem;
}
		
// insertItem
// Chen 1 item bao gom string va value
uiComboBoxItem*	uiComboBox::insertItem( DWORD nIndex, LPWSTR lpString, LPWSTR lpValue)
{
	uiComboBoxItem *pItem = new uiComboBoxItem(this);
	pItem->setItem(lpString,lpValue);	
	
	this->insertAbstractItem( nIndex, pItem );

	ComboBox_InsertString( this->getHandle(), nIndex, lpString );

	return pItem;
}

// cmbRemoveString
// Xoa 1 string
void uiComboBox::removeItem( DWORD nIndex )
{
	ComboBox_DeleteString( this->getHandle(), nIndex );
	this->removeAbstractItem(nIndex);
}

// clearItem
// Xoa toan bo cac item
void uiComboBox::clearItem()
{
	while ( this->getItemCount() != 0 )
		this->removeItem( 0 );
}

// cmbGetComboBoxText
// Lay 1 chuoi o textbox
void uiComboBox::getComboBoxText(DWORD nIndex,LPWSTR lpText)
{
	if ( nIndex < (DWORD)zm_listItem.size() )
	{
		uiComboBoxItem* p  = (uiComboBoxItem*) zm_listItem[nIndex];
		p->getString( lpText );
	}
}

// getComboBoxValue
// Lay gia tri value cua textbox
void uiComboBox::getComboBoxValue(DWORD nIndex, LPWSTR lpValue)
{
	if ( nIndex < (DWORD)zm_listItem.size() )
	{
		uiComboBoxItem* p  = (uiComboBoxItem*) zm_listItem[nIndex];
		p->getValue( lpValue );		
	}
}

// getItem
// Lay doi tuong uiComboBoxItem
uiComboBoxItem* uiComboBox::getItem(DWORD nIndex)
{
	if ( nIndex < (DWORD)zm_listItem.size() )
	{
		uiComboBoxItem* p  = (uiComboBoxItem*) zm_listItem[nIndex];
		return p;
	}
	
	return NULL;
}

// cmbShowDropDown
// Show list
void uiComboBox::showDropDown( bool b )
{
	ComboBox_ShowDropdown( this->getHandle(), b);
}

// cmbSetItemData
// Them data ke thua uiObject* vao comboBox
void uiComboBox::setItemData( DWORD nIndex, uiObject* pData )
{
	if ( nIndex < (DWORD)zm_listItem.size() )
		zm_listItem[nIndex]->setData( pData );
}	

// cmbGetItemData
// Lay du lieu data ke thua uiObject vao comboBox
uiObject* uiComboBox::getItemData( DWORD nIndex  )
{
	if ( nIndex < (DWORD)zm_listItem.size() )
		return zm_listItem[nIndex]->getData();			
	else
		return NULL;
}		

// cmbItemCount
// Lay so luong Item
DWORD uiComboBox::getItemCount()
{
	return ComboBox_GetCount(this->getHandle());
}

// cmbGetSelect
// Lay item dang lua chon
DWORD uiComboBox::getSelectIndex()
{
	return ComboBox_GetCurSel( this->getHandle() );
}

// getSelectItem
// Lay doi tuong getSelectItem
uiComboBoxItem* uiComboBox::getSelectItem()
{
	return this->getItem( this->getSelectIndex() );
}

// getSelectString
// Lay gia tri text dang select
void uiComboBox::getSelectString( LPWSTR string )
{
	uiComboBoxItem* p = this->getSelectItem();
	if ( p )
		p->getString( string );
}

// getSelectValue
// Lay gia tri value dang select
void uiComboBox::getSelectValue( LPWSTR value )
{
	uiComboBoxItem* p = this->getSelectItem();
	if ( p )
		p->getValue( value );
}

// cmbSelectItem
// Lua chon item index
void uiComboBox::selectItem(DWORD nIndex)
{
	ComboBox_SetCurSel( this->getHandle(), nIndex );
}

// update
// Cap nhat item
void uiComboBox::updateItem( uiAbstractListItem* pItem )
{		
	DWORD j = 0;

	// Tim kiem vi tri item va insert
	uiAbstractArrayItem::iterator i = zm_listItem.begin();
	for (;i != zm_listItem.end(); i++)
	{
		if ( (*i) == pItem )
		{
			WCHAR lpString[300];

			// Lay string ...
			((uiComboBoxItem*)pItem)->getString( lpString );

			// xoa bo pItem ra khoi list			
			ComboBox_DeleteString( this->getHandle(), j );
			ComboBox_InsertString( this->getHandle(), j, lpString );

			return;
		}

		j++;
	}
}