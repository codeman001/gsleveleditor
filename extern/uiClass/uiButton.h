// uiButton.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 30/12/2008
// Thong tin:
//

#ifndef _UIBUTTON_
#define _UIBUTTON_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

namespace uiClass
{
	class DLLEXPORT uiButton: public uiControl
	{			
	protected:
		// Su kien cho button
		uiEventCallback	eventOnClicked;
		uiEventCallback	eventOnDbClicked;

		uiButton(void);
	public:		
		uiButton(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);
		virtual ~uiButton(void);
		
		// getObject
		// Tra ve UIOBJECT
		virtual DWORD getObject();

		// create
		// Khoi tao button
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);

		// messageControlMap
		// Phan loai msg cho button
		virtual void messageControlMap(UINT uMsg);

		// setState
		// Thay doi trang thai up/down
		void setState(bool b);
		
		// getState
		// Lay trang thai
		bool getState();
	
		// setStyle
		// Thay doi Stype button
		//	- Tham so la hang so BS_ cua WINAPI
		void setStyle(DWORD dwBS);		
		
		// Su kien OnClick
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnClicked(T* pObject)	{	eventOnClicked.setEvent<T,pTMethod>( pObject );	}
		
		// Su kien OnDbClick
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnDbClicked(T* pObject){	eventOnClicked.setEvent<T,pTMethod>( pObject );	}

	};
}

#endif
