// gsanimation.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "gsanimation.h"
#include "CMainFrame.h"


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
	// todo open mesh & anim
	// lpCmdLine: "-open data/mesh/marine/marine_2_mesh.dae data/mesh/marine/marine_2_anims.dae"
	// lpCmdLine: "-open data/mesh/marine/marine_2_mesh.scene data/mesh/marine/marine_2_anims.anim"

	// todo export mesh
	// lpCmdLine: "-exportmesh data/mesh/marine/marine_2_mesh.dae data/mesh/marine/marine_2_mesh.scene"

	// todo export anim
	// lpCmdLine: "-exportanim data/mesh/marine/marine_2_anims.dae data/mesh/marine/marine_2_anims.anim"

	CMainFrame *pMainWnd = new CMainFrame(lpCmdLine);

	pMainWnd->create( STR_APP_TITLE, 50,50,1024,700,NULL, TEXT("gsParticleEditor") );	
			
	pMainWnd->setPositionCenterOfScreen();
	
	if ( pMainWnd->doExportMode() == false )
	{
		pMainWnd->setWindowState(SW_NORMAL);
		pMainWnd->showWindow(true);
		pMainWnd->updateWindow();	
		pMainWnd->setForegroundWindow();
		pMainWnd->setFocus();
	}
	else
	{
		pMainWnd->setWindowState(SW_MINIMIZE);
		pMainWnd->showWindow(true);
		pMainWnd->updateWindow();	
	}

	pMainWnd->enableOnIdle(true);

	uiApplication::enableProcessMsgOnAccel( true );
	uiApplication::run( pMainWnd );

	delete pMainWnd;
	uiApplication::destroy();

	return 1;
}