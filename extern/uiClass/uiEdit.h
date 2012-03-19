// uiEdit.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 2/1/2009
// Thong tin:

#ifndef _UIEDIT_
#define _UIEDIT_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

namespace uiClass
{
	// uiEdit
	// Doi tuong edit control
	class DLLEXPORT uiEdit: public uiControl
	{	
	protected:
		uiEventCallback eventOnChange;
		uiEventCallback eventOnUpdate;	

		uiEdit();
	public:
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnChange(T* pObject)	{	eventOnChange.setEvent<T,pTMethod>( pObject );	}

		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnUpdate(T* pObject)	{	eventOnUpdate.setEvent<T,pTMethod>( pObject );	}
	public:		
		virtual ~uiEdit(void);

		// uiEdit
		// Khoi tao uiEdit		
		uiEdit(LPCWSTR lpTitle, int x, int y, int w, int h, bool bPasswordEdit , uiWindow* pParent);

		// create
		// Khoi tao uiEdit				
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, bool bPasswordEdit , uiWindow* pParent);

		// messageControlMap
		// Phan loai Message
		virtual void messageControlMap(UINT uMsg);
	
		// getObject
		// Lay gia tri UIOBJECT_
		DWORD getObject();

		// emptyUndoBuffer
		// Xoa Undo
		void emptyUndoBuffer();

		// getLine
		// Lay du lieu o dong thu nLine
		int  getLine(int nLine, LPWSTR lpString, int charCount);

		// getLineCount
		// Dem so dong
		int	 getLineCount();

		// isModify
		// Kiem tra edit co thay doi hay ko
		bool isModify();

		// getText
		// Lay toan bo text
		void getText(LPWSTR lpText, int nCount);

		// getTextCount
		// Lay so ky tu hien tai
		int  getTextCount();

		void hideBalloonTip();

		// setNumberValueOnly
		// Chi cho phep nhap number
		void setNumberValueOnly( bool b );

		// setLimitText
		// Gioi han text
		void setLimitText(int nLimit);		

		// replaceSel
		// Thay the chuoi duoc lua chon bang lpString
		void replaceSel(LPWSTR lpString);

		// scroll
		// Dich chuyen con tro
		void scroll(int nLine, int nPos);

		// scrollCaretToBegin
		// Dich chuyen con tro toi vi tri dau tien
		void scrollCaretToBegin();

		// setPasswordChar
		// Thiet lap ky tu cho password
		void setPasswordChar( WCHAR wChar );

		// setReadOnly
		// ReadOnly Editbox
		void setReadOnly(bool b);

		// setSelect
		// Lua chon text
		void setSelect( int nStart,int nEnd);

		// setText
		// Thiet lap text cho Edit
		void setText( LPWSTR lpText );		

		// undo
		// Undo du lieu
		void undo();
	};
}

#endif

