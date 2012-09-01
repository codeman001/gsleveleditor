#include <android/log.h>

#include "stdafx.h"
#include "CApplication.h"

extern "C" {

	bool g_appInit = true;
	bool g_appResume = false;
	CApplication 	g_myApp;	
	IrrlichtDevice* g_irrDevice = NULL;
	
	void appicationInitApp()
	{
		if ( g_appInit )
		{	
			// create irrlicht device
			#if defined(_IRR_COMPILE_WITH_OGLES1_)
			g_irrDevice = createDevice( irr::video::EDT_OGLES1, core::dimension2d<u32>(480, 800), 16, false, false, false, &g_myApp );
			#else
			g_irrDevice = createDevice( irr::video::EDT_OGLES2, core::dimension2d<u32>(480, 800), 16, false, false, false, &g_myApp );	
			#endif
			
			// init application
			g_myApp.initApplication( g_irrDevice );
		}
		else if ( g_appResume )
		{			
			// resume application
			g_appResume = false;			
		}
		
		g_appInit = false;
	}

	void appicationLoop()
	{
		if ( g_irrDevice->run() )
			g_myApp.mainLoop();
	}
	
	void appicationExitApp()
	{		
		g_myApp.destroyApplication();
		g_irrDevice->drop();
		__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so", "Native Application has exited");
	}

	void appicationPauseApp()
	{
	}

	void appicationResumeApp()
	{
		g_appResume = true;
	}

	void appicationResizeWindow(int w, int h)
	{		
	}

	void appicationTouchDown(int touchID, int x, int y)
	{	
	}

	void appicationTouchMove(int touchID, int x, int y)
	{		
	}

	void appicationTouchUp(int touchID, int x, int y)
	{		
	}

}
