//
//  Application.cpp
//  gameplay
//
//  Created by Pham Hong Duc on 9/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "stdafx.h"
#include "CApplication.h"

// get width, height of device
extern void getDeviceWidthHeight( int &width, int &height );

CApplication 	g_myApp;	
IrrlichtDevice* g_irrDevice = NULL;
	
void appicationInitApp()
{
    // add zip data
    const char *dataPak = NULL;
			
#ifdef USE_ZIPPACKAGE
    dataPak = getIView()->getPhysicPath("_common.zip");
#endif
       
    int width = 480;
    int height = 800;
    getDeviceWidthHeight( width, height );
    
    // create irrlicht device
#if defined(_IRR_COMPILE_WITH_OGLES1_)
    g_irrDevice = createDevice( irr::video::EDT_OGLES1, core::dimension2d<u32>(height, width ), 16, false, false, false, dataPak, &g_myApp );
#else
    g_irrDevice = createDevice( irr::video::EDT_OGLES2, core::dimension2d<u32>(height, width), 16, false, false, false, dataPak, &g_myApp );
#endif
			
    // init application
    g_myApp.initApplication( g_irrDevice );	
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
}
    
void appicationPauseApp()
{
}
    
void appicationResumeApp()
{
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
    
