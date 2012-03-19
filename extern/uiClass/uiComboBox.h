// uiCombobox.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 2/1/2009
// Thong tin:

#ifndef _UICOMBOBOX_
#define _UICOMBOBOX_

#include "uiInclude.h"
#include "uiAbstractList.h"
#include "uiEvent.h"
#include "uiWindow.h"

namespace uiClass
{
	class uiComboBox;
	class uiComboBoxItem;

	// uiComboBoxItem
	// Doi tuong Item cua comboBox
	class DLLEXPORT uiComboBoxItem:public uiAbstractListItem
	{
	protected:
		WCHAR		zm_lpString[300];
		WCHAR		zm_lpValue[300];	
	public:
		uiComboBoxItem(uiComboBox *pCombobox);
		uiComboBoxItem(uiAbstractListControl *pListControl);

		virtual ~uiComboBoxItem();
			
		// setString
		// Thiet lap string cho ComboBoxItem
		void setString( LPWSTR lpString );

		// getString
		// Lay string
		void getString( LPWSTR lpString );

		// setValue 
		// Thiet lap gia tri cho Item
		void setValue (LPWSTR lpString );

		// getValue 
		// Lay gia tri
		void getValue (LPWSTR lpString );	

		// setItem
		// Thiet lap string, value
		void setItem( LPWSTR string, LPWSTR value );

	};

	// uiComboBox
	// Doi tuong ComboBox control
	class DLLEXPORT uiComboBox: public uiAbstractListControl
	{	
	protected:		
		uiEventCallback eventOnDbClicked;		
		uiEventCallback eventOnDropDown;
		uiEventCallback eventOnEditChange;
		uiEventCallback eventOnEditUpdate;
		uiEventCallback eventOnSelectChange;

		uiComboBox(void);
	public:
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnDbClicked(T* pObject)	{	eventOnDbClicked.setEvent<T,pTMethod>( pObject );	}			

		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnDropDown(T* pObject)		{	eventOnDropDown.setEvent<T,pTMethod>( pObject );	}
		
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnEditChange(T* pObject)	{	eventOnEditChange.setEvent<T,pTMethod>( pObject );	}
		
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnEditUpdate(T* pObject)	{	eventOnEditUpdate.setEvent<T,pTMethod>( pObject );	}
		
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnSelectChange(T* pObject)	{	eventOnSelectChange.setEvent<T,pTMethod>( pObject ); }		
		
	public:
		uiComboBox(LPCWSTR lpTitle, int x, int y, int w, int h, bool bIsDropList, uiWindow* pParent);
		virtual ~uiComboBox(void);
		
		// getObject
		// Tra ve UIOBJECT_
		DWORD getObject();

		// create
		// Khoi tao combobox
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, bool bIsDropList, uiWindow* pParent);		

		// messageControlMap
		// Phan loai message
		virtual void messageControlMap(UINT uMsg);

		// addString
		// Them 1 string vao ComboBox
		uiComboBoxItem*	addString( LPWSTR lpString );

		// addItem
		// Them 1 item bao gom string va value
		uiComboBoxItem*	addItem( LPWSTR lpString, LPWSTR lpValue );
		
		// insertItem
		// Chen 1 item bao gom string va value
		uiComboBoxItem*	insertItem( DWORD nIndex, LPWSTR lpString, LPWSTR lpValue);

		// insertString
		// Chen 1 string vao 1 vi tri bat ky
		uiComboBoxItem*	insertString( DWORD nIndex, LPWSTR lpString );

		// removeString
		// Xoa 1 string
		void removeItem( DWORD nIndex );

		// clearItem
		// Xoa toan bo cac item
		void clearItem();

		// getComboBoxText
		// Lay 1 chuoi o textbox
		void	getComboBoxText(DWORD nIndex,LPWSTR lpText);

		// getComboBoxValue
		// Lay gia tri value cua textbox
		void	getComboBoxValue(DWORD nIndex, LPWSTR lpValue);
		
		// getItem
		// Lay doi tuong uiComboBoxItem
		uiComboBoxItem* getItem(DWORD nIndex);

		// showDropDown
		// Show list
		void	showDropDown( bool b );

		// setItemData
		// Them data ke thua uiObject* vao comboBox
		void	setItemData( DWORD nIndex, uiObject* pData );	

		// getItemData
		// Lay du lieu data ke thua uiObject vao comboBox
		uiObject*	getItemData( DWORD nIndex);

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
		void selectItem(DWORD nIndex);

		// update
		// Cap nhat item
		virtual void updateItem( uiAbstractListItem* pItem );
		
	};
}

#endif
