// gsgameplay.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IView.h"
#include "CApplication.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CApplication myApp;
	
	IrrlichtDevice* device = NULL;
	
#if defined(_IRR_COMPILE_WITH_OPENGL_)
	device = createDevice( irr::video::EDT_OPENGL, core::dimension2d<u32>(1024, 768), 16, false, false, false, &myApp );
#elif defined(_IRR_COMPILE_WITH_OGLES1_)
	device = createDevice( irr::video::EDT_OGLES1, core::dimension2d<u32>(1024, 768), 16, false, false, false, &myApp );
#else
	device = createDevice( irr::video::EDT_OGLES2, core::dimension2d<u32>(1024, 768), 16, false, false, false, &myApp );	
#endif

	if ( device == NULL )
		return 1;

	myApp.initApplication( device );

	while(device->run())
	{
		myApp.mainLoop();		
	}

	myApp.destroyApplication();
	device->drop();

	return 0;
}

