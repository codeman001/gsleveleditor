// uiThread.h
// Developer: Pham Hong Duc
// Ngay viet: 15/05/2009

// Ghi chu: uiClass library chua ho tro thread
// Bo sung cho thu vien sau

#ifndef _UITREAD_
#define _UITREAD_

#include "uiInclude.h"
#include "uiEventCallback.h"

namespace uiClass
{
	
	// uiThreadObj
	// Doi tuong truu tuong cho thread
	class DLLEXPORT uiThread: public uiObject
	{
	protected:
		uiEventCallback	eventThreadFunc;
	public:
		// Event thread
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventThreadFunc(T* pObject)	{	eventThreadFunc.setEvent<T,pTMethod>( pObject );	}
	public:
		uiThread ();
		virtual ~uiThread ();

		// kill 
		// Huy thread
		void kill ();		

		// beginThread
		// Bat dau chay thread
		void beginThread();

	protected:				

		virtual void run ();

		static DWORD WINAPI ThreadEntry (void *pArg);

		int             zm_isDying;	
		HANDLE			zm_handle;
	    DWORD			zm_tId;

		// isDie
		// Die thread
		bool isDie();

		// WaitForDeath
		// Cho doi
		void waitForDeath();
		
		// getHandle
		// Lay handle cua thread
		HANDLE getHandle();

	};

	// uiMutex
	// Doi tuong mutex
	class DLLEXPORT uiMutex: public uiObject
	{
	public:
		uiMutex ();
		~uiMutex ();
	
		// lock 
		// Khoa thread lai
		void lock ();
		
		// release
		// Mo khoa thread de tiep tuc tien trinh
		void release ();

	protected:
		CRITICAL_SECTION zm_critSection;
	};


};


#endif