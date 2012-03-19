// uiTabControl.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 23/2/2009 
// Thong tin:
//

#ifndef _UITABCONTROL_
#define _UITABCONTROL_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

using std::wstring;
using std::vector;

namespace uiClass
{	
	// uiTabControl
	// Doi tuong tabcontrol
	class DLLEXPORT uiTabControl: public uiControl
	{
	protected:
		vector<wstring>		zm_listTabString;
		vector<uiWindow*>	zm_listTabWindows;

		DWORD				zm_nCurrentTab;

		// event
		uiEventCallback		eventOnDbClicked;
		uiEventCallback		eventOnClicked;
		uiEventCallback		eventOnRClicked;
		uiEventCallback		eventOnRDbClicked;
		uiEventCallback		eventOnFocusChanged;
		uiEventCallback		eventOnTabChanged;
		uiEventCallback		eventOnTabChanging;
	protected:

		// tabChange
		// Su kien chay cung voi onTabChange
		virtual void tabChange();

		// tabChanging
		// Su kien chay cung voi onTabChanging;
		virtual void tabChanging();
	public:		
		// Su kien OnClick
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnDbClicked(T* pObject){	eventOnDbClicked.setEvent<T,pTMethod>( pObject );	}

		// Su kien OnClick
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnClicked(T* pObject)	{	eventOnClicked.setEvent<T,pTMethod>( pObject );	}
	
		// Su kien OnRClick
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnRClicked(T* pObject)	{	eventOnRClicked.setEvent<T,pTMethod>( pObject );	}

		// Su kien OnRDbClick
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnRDbClicked(T* pObject)	{	eventOnRDbClicked.setEvent<T,pTMethod>( pObject );	}

		// Su kien OnFocusChanged
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnFocusChanged(T* pObject)	{	eventOnFocusChanged.setEvent<T,pTMethod>( pObject );	}

		// Su kien OnTabChanged
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnTabChanged(T* pObject)	{	eventOnTabChanged.setEvent<T,pTMethod>( pObject );	}

		// Su kien OnTabChanging
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnTabChanging(T* pObject)	{	eventOnTabChanging.setEvent<T,pTMethod>( pObject );	}
	public:
		uiTabControl(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);
		virtual ~uiTabControl();

		// create
		// Khoi tao button
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);

		// getObject
		// Tra ve doi tuong UIOBJECT_
		virtual DWORD getObject();

		// notifyControl
		// Phan loai su kien cho Control
		virtual void notifyControl( LPARAM lParam );

		// addTab
		// Them 1 tab window moi
		void addTab( LPWSTR lpTabString, uiWindow* pWindow );

		// selectTab 
		// Lua chon tab
		void selectTab ( DWORD nIndex );
		
		// getSelectTab
		// Lay tab dang duoc lua chon hien hanh
		DWORD getSelectTab();	

		// deleteTab
		// Xoa tab
		void deleteTab ( DWORD nIndex );
		
		// getTabString
		// Lay tieu de tab string
		void getTabString ( DWORD nIndex, LPWSTR lpString );

		// setTabString 
		// Thiet lap string cho tab
		void setTabString ( DWORD nIndex, LPWSTR lpString );

		// deleteAllTab
		// Xoa tat ca cac tab
		void deleteAllTab();

		// insertTab
		// Chen them 1 tab
		void insertTab( DWORD nIndex, LPWSTR lpTabString, uiWindow* pWindow );
	};

};

#endif
