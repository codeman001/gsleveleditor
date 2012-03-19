// uiEventCallback.h
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 15/12/2008
// Thong tin:
//		- uiEventCallback la doi tuong giup callback lai function trong 1 class


#ifndef _UIEVENTCALLBACK_
#define _UIEVENTCALLBACK_

#include "uiInclude.h"
#include "uiObject.h"

namespace uiClass
{
	
	// uiEventCallback
	// Doi tuong ho tro call member function cua class
	class DLLEXPORT uiEventCallback:public uiObject
	{
	protected:		

		// uiCallFunction: struct the hien 1 function va tham so di kem theo
		typedef void (*uiCallFunction) (LPVOID, uiObject*);
		uiCallFunction zm_pMethod;
		
		// Doi tuong chua member function can callback
		void*	zm_pObject;

		// Goi ham zm_pObject->function
		template<class T,void (T::*pMethod)(uiObject*)>
		static void callFunction(void* pObject, uiObject* objSender)
		{
			T *p = (T*) pObject;
			(p->*pMethod)( objSender );
		}
	public:
		// getObject
		// Tra ve UIOBJECT_EVENTCALL
		virtual DWORD getObject();

		uiEventCallback()
		{
			zm_pMethod = NULL;
			zm_pObject = NULL;
		}

		// setEvent
		// Thiet lap event 
		// Tham so: - Type Class
		//			- Dia chi function
		template<class T, void (T::*pTMethod)(uiObject*)>	
		void setEvent(T *pObject)
		{			
			zm_pObject = (void*)pObject;			
			zm_pMethod = &callFunction<T,pTMethod>;
		}

		// operator()
		// Goi function da thiet lap o ham setEvent
		// Tham so:	- Gia tri can gui toi function (Thong thuong NULL)
		void operator() (uiObject* objSender)
		{			
			if ( zm_pObject && zm_pObject )
				zm_pMethod( zm_pObject, objSender );
		}
		
	};

}

#endif
