// uiWindow.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 18/12/2008
// Thong tin:
//

#ifndef _UIAPPLICATION_
#define _UIAPPLICATION_

#include "uiInclude.h"
#include "uiString.h"
#include "uiWindow.h"
#include "uiToolbar.h"
#include "uiTooltip.h"

#include <iostream>

using std::vector;
using std::map;
using std::list;

namespace uiClass
{
	class DLLEXPORT uiApplication
	{
	protected:
		static WCHAR				g_lpAppname[400];			
		static uiWindow				*g_rootWindow;
		static map<UINT,uiObject*>	*g_mapControl;
		static list<uiObject*>		*g_globalAlloc;
		static bool					g_processMsgOnAccel;
	public:		

		// setAppName
		// Thiet lap ten cho ung dung
		static void setAppName( LPWSTR appName);

		// getAppName
		// Thiet lap ten cho ung dung
		static void getAppName( LPWSTR appName);
		
		// getAppPath
		// Tra lai duong dan cho ung dung
		static void getAppPath( LPWSTR appPath, int size );
		
		// getFullAppPath
		// Tra lai duong dan + exe
		static void getFullAppPath( LPWSTR appPath, int size );

		// controlRegister	
		// Dang ky ID cho control
		static UINT controlRegister	( uiControl *pControl );
		
		// objectRegister
		// Dang ky ID cho obj
		static void objectRegister	( UINT id, uiObject *pObject );

		// getControlByID
		// Lay Control bang ID
		static uiObject* getControlByID( UINT nID );

		// destroy
		// Huy tai nguyen
		static void destroy();

		// run
		// Chay ung dung
		static int run( uiForm* p);

		// exit
		// Dong ung dung
		static void exit(int code = 1);

		// getRoot
		// Tra ve cua so form dau tien duoc chay
		static uiWindow *getRoot();

		// setRoot
		// Thiet lap cua so goc cua ung dung
		static void setRoot( uiWindow *root );

		// enableProcessMsgOnAccel
		// cho phep xu ly msg khi nhan phim tat
		static void enableProcessMsgOnAccel( bool b );

		// pushAllocOject
		// Luu object vao danh sach
		// static list<uiObject*>::reverse_iterator pushAllocObject( uiObject* p );
		
	};
}

#endif
