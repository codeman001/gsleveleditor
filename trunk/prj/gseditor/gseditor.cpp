// toadoMapEditor.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "gseditor.h"
#include "CMainFrame.h"
#include "CGlobalInstance.h"

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

	CGlobalInstance::getInstance();

	CMainFrame *pMainWnd = new CMainFrame();

	pMainWnd->create( TEXT( STR_APP_TITLE ),50,50,1024,768,NULL, TEXT("gseditor") );	
	pMainWnd->setPositionCenterOfScreen();
	//pMainWnd->setWindowState( SW_MAXIMIZE );
	pMainWnd->showWindow(true);		
	pMainWnd->updateWindow();
	pMainWnd->setForegroundWindow();
	pMainWnd->setFocus();
	pMainWnd->enableOnIdle(true);


	uiApplication::enableProcessMsgOnAccel( true );
	uiApplication::run( pMainWnd );
	delete pMainWnd;

	CoUninitialize();

	CGlobalInstance::destroyInstance();
	uiApplication::destroy();	

	return 0;
}
