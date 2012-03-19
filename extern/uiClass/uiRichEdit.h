// uiRichEdit.h
// LVT: Pham Hong Duc
// Ngay viet: 18/12/2011
// Thong tin:
// Class control rich edit giup phat trien cac editor

#ifndef _UIRICHEDIT_
#define _UIRICHEDIT_

#include "uiInclude.h"

#ifdef UICLASS_SUPPORT_RICHEDIT

#include "uiEvent.h"
#include "uiWindow.h"

namespace uiClass
{
	// uiRichEdit
	// Doi tuong edit control
	class DLLEXPORT uiRichEdit: public uiControl
	{	
	protected:
		uiEventCallback eventOnChange;
		uiEventCallback eventOnUpdate;	

		uiRichEdit();
	public:
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnChange(T* pObject)	{	eventOnChange.setEvent<T,pTMethod>( pObject );	}

		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnUpdate(T* pObject)	{	eventOnUpdate.setEvent<T,pTMethod>( pObject );	}
	public:		
		virtual ~uiRichEdit(void);

		// uiEdit
		// Khoi tao uiEdit		
		uiRichEdit(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);

		// create
		// Khoi tao uiEdit				
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);

		// messageControlMap
		// Phan loai Message
		virtual void messageControlMap(UINT uMsg);
	
		// getObject
		// Lay gia tri UIOBJECT_
		DWORD getObject();

		// getTextLong
		// Lay so luong text cua edit
		DWORD getTextLong();

		// getText
		// Lay text ra buffer
		DWORD getText( void *lpBuffer, int size );
	};

}

#endif	// UICLASS_SUPPORT_RICHEDIT

#endif