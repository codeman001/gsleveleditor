//
//  main.m
//  gameplay
//
//  Created by Pham Hong Duc on 9/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "stdafx.h"
#include "IView.h"
#include "CApplication.h"

int main(int argc, char *argv[])
{	
    CApplication myApp;
	
	IrrlichtDevice* device = NULL;
	
	// add zip data
	const char *dataPak = NULL;
    
#ifdef USE_ZIPPACKAGE
	dataPak = getIView()->getPhysicPath("_common.zip");
#endif

	device = createDevice( irr::video::EDT_OPENGL, core::dimension2d<u32>(800, 600), 32, false, false, false, dataPak, &myApp );

    
	if ( device == NULL )
		return 1;
    
	// set orient tation
	//device->getVideoDriver()->setOrientation( video::EOO_270 );
    
	myApp.initApplication( device );
    
	// enable resize windows
	device->setResizable(true);
    
	// run application
	while(device->run())
	{
        myApp.mainLoop();		
	}
    
	myApp.destroyApplication();
	device->drop();
    
	return 0;
}
