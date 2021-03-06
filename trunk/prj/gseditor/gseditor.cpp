// toadoMapEditor.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "gseditor.h"
#include "CMainFrame.h"
#include "CGlobalInstance.h"

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        return FALSE;
    }

	CGlobalInstance::createGetInstance();

	CMainFrame *pMainWnd = new CMainFrame();

	pMainWnd->create( TEXT( STR_APP_TITLE ),50,50,1024,768,NULL, TEXT("gseditor") );	
	pMainWnd->setPositionCenterOfScreen();
	pMainWnd->setWindowState( SW_MAXIMIZE );
	pMainWnd->showWindow(true);		
	pMainWnd->updateWindow();
	pMainWnd->setForegroundWindow();
	pMainWnd->setFocus();
	pMainWnd->enableOnIdle(true);


	uiApplication::enableProcessMsgOnAccel( true );
	uiApplication::run( pMainWnd );
	delete pMainWnd;

	CoUninitialize();

	CGlobalInstance::releaseInstance();
	uiApplication::destroy();	

	return 0;
}
