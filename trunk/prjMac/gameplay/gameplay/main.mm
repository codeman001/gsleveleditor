//
//  main.m
//  gameplay
//
//  Created by Pham Hong Duc on 9/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "irrlicht.h"

int main(int argc, char *argv[])
{	
    irr::IrrlichtDevice* device = NULL;
	
	// add zip data
	const char *dataPak = NULL;
    
	device = irr::createDevice( irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(800, 600), 16, false, false, false, dataPak, NULL );

    
	if ( device == NULL )
		return 1;
    
	// enable resize windows
	device->setResizable(true);
    
	// run application
	while(device->run())
	{
        device->getVideoDriver()->beginScene();
        device->getVideoDriver()->endScene();
	}
    
	device->drop();
    
	return 0;
}
