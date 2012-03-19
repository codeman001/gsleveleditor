// uiButton.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 31/12/2008
// Thong tin:

#ifndef _UICHECKBOX_
#define _UICHECKBOX_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

namespace uiClass
{
	class DLLEXPORT uiCheckBox: public uiControl
	{
	protected:
		uiEventCallback	eventOnClicked;
		uiEventCallback	eventOnDbClicked;

		uiCheckBox(void);
	public:
		
		// Su kien OnClick
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnClicked(T* pObject)		{ eventOnClicked.setEvent<T,pTMethod>( pObject ); }
		
		// Su kien OnDbClick
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnDbClicked(T* pObject)	{ eventOnDbClicked.setEvent<T,pTMethod>( pObject );	}
	public:		
		virtual ~uiCheckBox(void);
		
		uiCheckBox(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);

		// create
		// Khoi tao uiCheckbox
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);

		// messageControlMap
		// Phan loai MSGs
		virtual void messageControlMap(UINT uMsg);
		
		// getObject
		// Tra ve UIOBJECT
		virtual DWORD getObject();

		// setCheck
		// Set gia tri cho checkbox
		void setCheck(bool b);

		// getCheck
		// Lay gia tri checkbox
		bool getCheck();
	};
}

#endif
