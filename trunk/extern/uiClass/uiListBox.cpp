// uiStatic.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 31/1/2009
// Thong tin:

#include "uiListBox.h"
#include "uiInclude.h"
#include "uiApplication.h"
#include <windowsx.h>

using namespace uiClass;

uiListBox::uiListBox()
{
}

uiListBox::uiListBox(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	this->create( lpTitle,x,y,w,h, pParent );
}

uiListBox::~uiListBox()
{

}

// getObject
// Tra ve UIOBJECT
DWORD uiListBox::getObject()
{
	return UIOBJECT_LISTBOX;
}

// create
// Khoi tao button
bool uiListBox::create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
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
	zm_hWnd = CreateWindowExW(0,
		L"listbox",
		lpTitle,
		WS_CHILD | WS_TABSTOP | WS_VISIBLE | LBS_STANDARD ,
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
void uiListBox::messageControlMap(UINT uMsg)
{		
	switch (uMsg)
	{
	case LBN_DBLCLK:
		eventOnDbClicked(this);
		break;	
	case LBN_SELCHANGE:
		eventOnSelectChange(this);
		break;
	case LBN_KILLFOCUS:
		eventOnFocus(this);
		break;
	case LBN_SETFOCUS:
		eventOnLostFocus(this);
		break;
	}	
}

// addString
// Them 1 string vao listBox
uiComboBoxItem*	uiListBox::addString( LPWSTR lpString )
{
	uiComboBoxItem *p = new uiComboBoxItem( this );
	
	p->setItem(lpString,L"");
	
	zm_listItem.push_back( p );

	ListBox_AddString( this->getHandle(), lpString );	

	return p;
}

// addItem
// Them 1 item bao gom string va value
uiComboBoxItem*	uiListBox::addItem( LPWSTR lpString, LPWSTR lpValue )
{
	uiComboBoxItem *p = new uiComboBoxItem( this );
	
	p->setItem(lpString,lpValue);
	
	zm_listItem.push_back( p );

	ListBox_AddString( this->getHandle(), lpString );	

	return p;
}

// insertItem
// Chen 1 item bao gom string va value
uiComboBoxItem*	uiListBox::insertItem( int nIndex, LPWSTR lpString, LPWSTR lpValue)
{
	uiComboBoxItem *pItem = new uiComboBoxItem(this);
	pItem->setItem(lpString,lpValue);	
	
	this->insertAbstractItem( nIndex, pItem );

	ListBox_InsertString( this->getHandle(), nIndex, lpString );

	return pItem;
}

// insertString
// Chen 1 string vao 1 vi tri bat ky
uiComboBoxItem*	uiListBox::insertString( int nIndex, LPWSTR lpString )
{
	uiComboBoxItem *pItem = new uiComboBoxItem(this);
	pItem->setItem(lpString,L"");	
	
	this->insertAbstractItem( nIndex, pItem );

	ListBox_InsertString( this->getHandle(), nIndex, lpString );

	return pItem;
}

// removeString
// Xoa 1 string
void uiListBox::removeItem( DWORD nIndex )
{
	ListBox_DeleteString( this->getHandle(), nIndex );
	this->removeAbstractItem(nIndex);
}

// getlistBoxText
// Lay 1 chuoi o textbox
void uiListBox::getListBoxText(DWORD nIndex,LPWSTR lpText)
{
	if ( nIndex < (DWORD)zm_listItem.size() )
	{
		uiComboBoxItem* p  = (uiComboBoxItem*) zm_listItem[nIndex];
		p->getString( lpText );
	}
}

// getlistBoxValue
// Lay gia tri value cua textbox
void uiListBox::getListBoxValue(DWORD nIndex, LPWSTR lpValue)
{
	if ( nIndex < (DWORD)zm_listItem.size() )
	{
		uiComboBoxItem* p  = (uiComboBoxItem*) zm_listItem[nIndex];
		p->getValue( lpValue );
	}
}
		
// getItem
// Lay doi tuong uiComboBoxItem
uiComboBoxItem* uiListBox::getItem(DWORD nIndex)
{
	if ( nIndex < (DWORD)zm_listItem.size() )
	{
		uiComboBoxItem* p  = (uiComboBoxItem*) zm_listItem[nIndex];
		return p;
	}
	
	return NULL;
}

// setItemData
// Them data ke thua uiObject* vao listBox
void uiListBox::setItemData( DWORD nIndex, uiObject* pData )
{

}	

// getItemData
// Lay du lieu data ke thua uiObject vao listBox
uiObject* uiListBox::getItemData( DWORD nIndex  )
{
	if ( nIndex < (DWORD)zm_listItem.size() )
		return zm_listItem[nIndex]->getData();			
	else
		return NULL;
}		

// itemCount
// Lay so luong Item
DWORD uiListBox::getItemCount()
{
	return ListBox_GetCount( this->getHandle() );
}

// getSelect
// Lay item dang lua chon
DWORD uiListBox::getSelectIndex()
{
	return ListBox_GetCurSel( this->getHandle() );
}
	
// getSelectItem
// Lay doi tuong getSelectItem
uiComboBoxItem* uiListBox::getSelectItem()
{
	return this->getItem( this->getSelectIndex() );
}
		
// getSelectString
// Lay gia tri text dang select
void uiListBox::getSelectString( LPWSTR string )
{
	uiComboBoxItem* p = this->getSelectItem();
	p->getString( string );
}

// getSelectValue
// Lay gia tri value dang select
void uiListBox::getSelectValue( LPWSTR value )
{
	uiComboBoxItem* p = this->getSelectItem();
	p->getValue( value );
}

// selectItem
// Lua chon item index
void uiListBox::selectItem(DWORD nIndex)
{
	ListBox_SetCurSel( this->getHandle(), nIndex );	
}

// update
// Cap nhat item
void uiListBox::updateItem( uiAbstractListItem* pItem )
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
			ListBox_DeleteString( this->getHandle(), j );
			ListBox_InsertString( this->getHandle(), j, lpString );

			return;
		}

		j++;
	}
}