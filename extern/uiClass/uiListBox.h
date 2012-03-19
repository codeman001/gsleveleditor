// uiListBox.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 31/1/2009
// Thong tin:

#ifndef _UILISTBOX_
#define _UILISTBOX_

#include "uiAbstractList.h"
#include "uiComboBox.h"
#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

namespace uiClass
{	
	// uiListBox
	// Doi tuong Listbox
	class DLLEXPORT uiListBox: public uiAbstractListControl
	{
	protected:
		uiEventCallback eventOnDbClicked;		
		uiEventCallback eventOnSelectChange;
	public:		
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnDbClicked(T* pObject)	{	eventOnDbClicked.setEvent<T,pTMethod>( pObject );	}		
		
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnSelectChange(T* pObject)	{	eventOnSelectChange.setEvent<T,pTMethod>( pObject ); }

		uiListBox();
		uiListBox(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);
		virtual ~uiListBox();

		// getObject
		// Tra ve UIOBJECT
		virtual DWORD getObject();

		// create
		// Khoi tao button
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);		
		
		// messageControlMap
		// Phan loai message
		virtual void messageControlMap(UINT uMsg);

		// addString
		// Them 1 string vao listBox
		uiComboBoxItem*	addString( LPWSTR lpString );

		// addItem
		// Them 1 item bao gom string va value
		uiComboBoxItem*	addItem( LPWSTR lpString, LPWSTR lpValue );
		
		// insertItem
		// Chen 1 item bao gom string va value
		uiComboBoxItem*	insertItem( int nIndex, LPWSTR lpString, LPWSTR lpValue);

		// insertString
		// Chen 1 string vao 1 vi tri bat ky
		uiComboBoxItem*	insertString( int nIndex, LPWSTR lpString );
		
		// removeString
		// Xoa 1 string
		void	removeItem( DWORD nIndex );

		// getlistBoxText
		// Lay 1 chuoi o textbox
		void	getListBoxText(DWORD nIndex,LPWSTR lpText);

		// getlistBoxValue
		// Lay gia tri value cua textbox
		void	getListBoxValue(DWORD nIndex, LPWSTR lpValue);
		
		// getItem
		// Lay doi tuong uiComboBoxItem
		uiComboBoxItem* getItem(DWORD nIndex);
		
		// setItemData
		// Them data ke thua uiObject* vao listBox
		void	setItemData( DWORD nIndex, uiObject* pData );	

		// getItemData
		// Lay du lieu data ke thua uiObject vao listBox
		uiObject*	getItemData( DWORD nIndex );		

		// itemCount
		// Lay so luong Item
		DWORD		getItemCount();

		// getSelect
		// Lay item dang lua chon
		DWORD		getSelectIndex();
	
		// getSelectItem
		// Lay doi tuong getSelectItem
		uiComboBoxItem* getSelectItem();
		
		// getSelectString
		// Lay gia tri text dang select
		void getSelectString( LPWSTR string );

		// getSelectValue
		// Lay gia tri value dang select
		void getSelectValue( LPWSTR value );

		// selectItem
		// Lua chon item index
		void	selectItem(DWORD nIndex);

		// update
		// Cap nhat item
		virtual void updateItem( uiAbstractListItem* pItem );
	};
}

#endif
