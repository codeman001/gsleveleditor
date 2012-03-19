// uiApplication.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 18/12/2008
// Thong tin:

#include "uiApplication.h"
#include "uiInclude.h"
#include "uiString.h"
#include "uiKeyAccel.h"

#include <vector>
#include <algorithm>

using namespace uiClass;

// Khai bao global
WCHAR					uiApplication::g_lpAppname[400];
uiWindow				*uiApplication::g_rootWindow;
map<UINT,uiObject*>	*uiApplication::g_mapControl = NULL;
list<uiObject*>		*uiApplication::g_globalAlloc = NULL;
bool					uiApplication::g_processMsgOnAccel = false;

static bool g_bHaveName = false;

// setAppName
// Thiet lap ten cho ung dung
void uiApplication::setAppName( LPWSTR appName)
{
	uiString::copy<WCHAR,WCHAR>( g_lpAppname, appName );
	g_bHaveName = true;
}

// getAppName
// Thiet lap ten cho ung dung
void uiApplication::getAppName( LPWSTR appName)
{
	if (g_bHaveName == false )
		uiString::copy<WCHAR,WCHAR>( appName,L"uiClassApp" );
	else
		uiString::copy<WCHAR,WCHAR>( appName,g_lpAppname );
}

// getAppPath
// Tra lai duong dan cho ung dung
void uiApplication::getAppPath( LPWSTR appPath, int size )
{
	WCHAR *fullPath = new WCHAR[size];
	getFullAppPath( fullPath, size );
	uiString::getFolderPath<WCHAR, WCHAR> (fullPath, appPath);
	delete fullPath;
}
		
// getFullAppPath
// Tra lai duong dan + exe
void uiApplication::getFullAppPath( LPWSTR appPath, int size )
{
	HMODULE hMod = GetModuleHandle(NULL);
	GetModuleFileNameW( hMod, appPath, (DWORD) size );
}

// controlRegister	
// Dang ky ID cho control
UINT uiApplication::controlRegister	( uiControl *pControl )
{
	if ( g_mapControl == NULL )
		g_mapControl = new std::map<UINT,uiObject*>();

	static UINT dwControlID = UICONTROL_COMMON;
	dwControlID++;		

	(*g_mapControl)[dwControlID] = pControl;
	pControl->setID( dwControlID );

	return dwControlID;
}

// objectRegister
// Dang ky ID cho obj
void uiApplication::objectRegister	( UINT id, uiObject *pObject )
{
	if ( g_mapControl == NULL )
		g_mapControl = new std::map<UINT,uiObject*>();		

	(*g_mapControl)[id] = pObject;	
}

// getControlByID
// Lay Control bang ID
uiObject* uiApplication::getControlByID ( UINT nID )
{
	return (*g_mapControl)[nID];
}

// destroy
// Huy tai nguyen
void uiApplication::destroy()
{
	// Xoa control
	if ( g_mapControl != NULL )
	{
		g_mapControl->clear();
		delete g_mapControl;
	}
	
	// Huy toolTip
	uiToolTip::destroyTip();
	uiWindow::releaseAppListen();

	UIDEBUG_TRACE("Memory leak:\n");
	UIDEBUG_DUMPLEAK();
}

// run
// Chay ung dung
int uiApplication::run( uiForm* p)
{	

	// Gan dia chi cho cua so ROOT
	static bool r = false;
	if ( r == false )
	{		
		uiApplication::setRoot(p);
		r = true;
	}

	// Xu ly MSG
	MSG msg;	
	memset( (void*) &msg, 0, sizeof(MSG) );
	
	BOOL bMSG = true;		
	HWND hwndMain = p->getHandle();	

	while (bMSG)
	{		

		// Neu tat Idle (binh thuong)
		if ( !p->isEnableOnIdle() )
		{
			bMSG = GetMessageW(&msg, NULL ,0,0);

			// Lay bang phim tat
			uiKeyAccel *pAccel = p->getKeyAccelTable();

			// Neu co phim tat
			if ( pAccel )
			{

				if ( g_processMsgOnAccel == false )
				{
					// Giai ma phim tat
					if (!TranslateAccelerator( 
								hwndMain,				// handle to receiving window 
								pAccel->getHandle(),    // handle to active accelerator table 							
								&msg))					// message data 
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				else
				{
					TranslateAccelerator( hwndMain, pAccel->getHandle(), &msg ); 
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

			}
			// Nguoc lai
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);	
			}
		}
		// Bat Idle
		else
		{
			bMSG = (msg.message != WM_QUIT);
			if ( PeekMessage( &msg, NULL ,0,0, PM_REMOVE ) )
			{
				// Lay bang phim tat
				uiKeyAccel *pAccel = p->getKeyAccelTable();
				// Neu co phim tat
				if ( pAccel )
				{
					if ( g_processMsgOnAccel == false )
					{
						// Giai ma phim tat
						if (!TranslateAccelerator( 
									hwndMain,				// handle to receiving window 
									pAccel->getHandle(),    // handle to active accelerator table 
									&msg))					// message data 
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
					}
					else
					{
						TranslateAccelerator( hwndMain, pAccel->getHandle(), &msg ); 
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}

				}
				// Nguoc lai
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);	
				}
			}
			else
				p->_OnIdle();
		}
	}	
	return 1;
}

// getRoot
// Tra ve cua so form dau tien duoc chay
uiWindow *uiApplication::getRoot()
{
	return g_rootWindow;
}

// setRoot
// Thiet lap cua so goc cua ung dung
void uiApplication::setRoot( uiWindow *root )
{
	g_rootWindow = root;
}

// enableProcessMsgOnAccel
// cho phep xu ly msg khi nhan phim tat
void uiApplication::enableProcessMsgOnAccel( bool b )
{
	g_processMsgOnAccel = b;
}

void uiApplication::exit(int code)
{
	PostQuitMessage(code);
}