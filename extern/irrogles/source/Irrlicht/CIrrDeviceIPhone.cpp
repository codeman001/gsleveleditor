// Copyright (C) 2002-2008 Nikolaus Gebhardt
// Copyright (C) 2008 Redshift Software, Inc.
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CIrrDeviceIPhone.h"

#ifdef _IRR_COMPILE_WITH_IPHONE_DEVICE_

#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <time.h>
#include "IEventReceiver.h"
#include "os.h"
#include "CTimer.h"
#include "irrString.h"
#include "Keycodes.h"
#include "COSOperator.h"
#include "CColorConverter.h"
#include "SIrrCreationParameters.h"
#include <CoreFoundation/CFRunLoop.h>
#include "SExposedVideoData.h"

//~ #include "irr/base/DeviceIPhone_Apple.h"

namespace irr
{
namespace video
{
	extern IVideoDriver* createOGLES1Driver(const SIrrlichtCreationParameters& params, video::SExposedVideoData& data, io::IFileSystem* io);
    
	extern IVideoDriver* createOGLES2Driver(const SIrrlichtCreationParameters& params, video::SExposedVideoData& data, io::IFileSystem* io);
}
}

namespace irr
{

//! constructor
CIrrDeviceIPhone::CIrrDeviceIPhone(const SIrrlichtCreationParameters& params, const char *packData)
	: CIrrDeviceStub(params, packData)
	, Close(false)
	, Closed(false)
	, WindowActive(false)
	, WindowMinimized(false)
{
	#ifdef _DEBUG
	setDebugName("CIrrDeviceIPhone");
	#endif	

	// print version, distribution etc.
	struct utsname info;
	uname(&info);
	Operator = new COSOperator((core::stringc("")
			+info.sysname+" "
			+info.release+" "
			+info.version+" "
			+info.machine).c_str());
	os::Printer::log(Operator->getOperationSystemVersion(), ELL_INFORMATION);

	// create driver
	createDriver();

	if (!VideoDriver)
		return;

	createGUIAndScene();
	
	WindowActive = true;
    
    static CIPhoneCursor s_cursor;
    core::rect<s32> winRect(0,0,params.WindowSize.Width, params.WindowSize.Height);
    s_cursor.setReferenceRect(&winRect);
    CursorControl = &s_cursor;
}



//! destructor
CIrrDeviceIPhone::~CIrrDeviceIPhone()
{
}


//! create the driver
void CIrrDeviceIPhone::createDriver()
{
	switch(CreationParams.DriverType)
	{
	case video::EDT_OGLES1:
	#ifdef _IRR_COMPILE_WITH_OGLES1_
		{
			video::SExposedVideoData data;
			VideoDriver = video::createOGLES1Driver( CreationParams, data, FileSystem );
		}
	#else
		os::Printer::log("No OpenGL-ES1 support compiled in.", ELL_ERROR);
	#endif
		break;
	case video::EDT_OGLES2:
	#ifdef _IRR_COMPILE_WITH_OGLES2_
		{
			video::SExposedVideoData data;
			VideoDriver = video::createOGLES2Driver( CreationParams, data, FileSystem );
		}
	#else
		os::Printer::log("No OpenGL-ES2 support compiled in.", ELL_ERROR);
	#endif
		break;
	case video::EDT_SOFTWARE:
	case video::EDT_BURNINGSVIDEO:
	case video::EDT_DIRECT3D8:
	case video::EDT_DIRECT3D9:
	case video::EDT_OPENGL:
		os::Printer::log("This driver is not available. Try OpenGL-ES1 renderer.",
			ELL_ERROR);
		break;

	case video::EDT_NULL:
		VideoDriver = video::createNullDriver(FileSystem, CreationParams.WindowSize);
		break;

	default:
		os::Printer::log("Unable to create video driver of unknown type.", ELL_ERROR);
		break;
	}
}



//! runs the device. Returns false if device wants to be deleted
bool CIrrDeviceIPhone::run()
{
	os::Timer::tick();
	yield();
	Closed = Close;
	return !Close;
}



//! Pause the current process for the minimum time allowed only to allow other processes to execute
void CIrrDeviceIPhone::yield()
{
	struct timespec ts = {0,0};
	nanosleep(&ts, NULL);
}



//! Pause execution and let other processes to run for a specified amount of time.
void CIrrDeviceIPhone::sleep(u32 timeMs, bool pauseTimer=false)
{
	bool wasStopped = Timer ? Timer->isStopped() : true;
	
	struct timespec ts;
	ts.tv_sec = (time_t) (timeMs / 1000);
	ts.tv_nsec = (long) (timeMs % 1000) * 1000000;

	if (pauseTimer && !wasStopped)
		Timer->stop();

	nanosleep(&ts, NULL);

	if (pauseTimer && !wasStopped)
		Timer->start();
}



//! sets the caption of the window
void CIrrDeviceIPhone::setWindowCaption(const wchar_t* text)
{
}



//! presents a surface in the client area
bool CIrrDeviceIPhone::present(video::IImage* image, void * windowId, core::rect<s32>* src)
{
	return false;
}



//! notifies the device that it should close itself
void CIrrDeviceIPhone::closeDevice()
{
	WindowActive = false;
	Close = true;	
}



//! returns if window is active. if not, nothing need to be drawn
bool CIrrDeviceIPhone::isWindowActive() const
{
	return WindowActive;
}


bool CIrrDeviceIPhone::isWindowFocused() const
{
	return isWindowActive();
}


bool CIrrDeviceIPhone::isWindowMinimized() const
{
	return !isWindowActive();
}


//! Sets if the window should be resizeable in windowed mode.
void CIrrDeviceIPhone::setResizable(bool resize)
{
}


//! Minimizes the window
void CIrrDeviceIPhone::minimizeWindow()
{
}


//! Maximizes the window if possible.
void CIrrDeviceIPhone::maximizeWindow()
{
}


//! Restore the window to normal size if possible.
void CIrrDeviceIPhone::restoreWindow()
{
}


void CIrrDeviceIPhone::setWindowActive(bool active)
{
	WindowActive = active;
}

} // end namespace

#endif // _IRR_COMPILE_WITH_IPHONE_DEVICE_

