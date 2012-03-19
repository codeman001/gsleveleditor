// uiObject.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 18/12/2008
// Thong tin:
//

#ifndef _UIOBJECT_
#define _UIOBJECT_

#include "uiInclude.h"
#include <list>

using std::list;

namespace uiClass
{
#pragma warning(disable:4251)

	class DLLEXPORT uiObject
	{
	private:
		list<uiObject*>				*zm_refOject;
		list<uiObject*>::iterator	zm_thisObject;
		bool						zm_bNew;
	protected:
		void refClear();
	public:
		// getObject
		// Tra ve id de xac dinh doi tuong
		virtual DWORD getObject();
		
		uiObject::uiObject();		

		// destructor 
		// Nhiem vu pha huy cac memory da cap phat
		virtual ~uiObject();
		
		// ref
		// Giu dia chi doi tuong -> tu dong xoa khi ket thuc
		template<class T>
		T* ref(uiObject* pObject)
		{
			if ( zm_refOject == NULL )
				zm_refOject = new list<uiObject*>();

			zm_refOject->push_back(pObject);
			pObject->zm_thisObject	= --zm_refOject->end();
			pObject->zm_bNew		= true;
			return (T*) pObject;
		}
	
	};

	template <class T> class uiSingleton: public uiObject
	{
		private:
			uiSingleton(const uiSingleton&);
			uiSingleton& operator = (const uiSingleton&);
		protected:
			uiSingleton() {}
			virtual ~uiSingleton() { s_instance = 0; }
		public:
			static T* s_instance;

			static T* createGetInstance()
			{                    
				if (!s_instance)				
					s_instance = new T();
				
				return s_instance;
			};

			inline static T* getInstance()
			{
				return s_instance;
			}

			static void releaseInstance()
			{
				if (s_instance)
				{
					delete s_instance;					
				}
			}
	};
	
	template <class T>
	T* uiSingleton<T>::s_instance = 0;

}

#endif
