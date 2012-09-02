#include "CIrrDeviceAndroid.h"

#ifdef _IRR_COMPILE_WITH_ANDROID_DEVICE_

#include "IEventReceiver.h"
#include "irrList.h"
#include "os.h"

#include "CTimer.h"
#include "irrString.h"
#include "COSOperator.h"
#include "dimension2d.h"
#include "IGUISpriteBank.h"
#include "SExposedVideoData.h"


namespace irr
{
	namespace video
	{	
		#ifdef _IRR_COMPILE_WITH_OGLES1_
		IVideoDriver* createOGLES1Driver(const SIrrlichtCreationParameters& params, video::SExposedVideoData& data, io::IFileSystem* io);
		#endif

		#ifdef _IRR_COMPILE_WITH_OGLES2_
		IVideoDriver* createOGLES2Driver(const SIrrlichtCreationParameters& params, video::SExposedVideoData& data, io::IFileSystem* io);
		#endif
	}
} // end namespace irr

namespace irr
{

	//! constructor
	CIrrDeviceAndroid::CIrrDeviceAndroid(const SIrrlichtCreationParameters& params, const char* dataPak)
		: CIrrDeviceStub(params, dataPak)
	{
		// create driver
		createDriver();

		if (!VideoDriver)
			return;

		createGUIAndScene();
	}

	//! destructor
	CIrrDeviceAndroid::~CIrrDeviceAndroid()
	{
	}

	//! create the driver
	void CIrrDeviceAndroid::createDriver()
	{
		switch(CreationParams.DriverType)
		{	
		case video::EDT_OGLES1:
			#ifdef _IRR_COMPILE_WITH_OGLES1_
			{
				video::SExposedVideoData data;			

				VideoDriver = video::createOGLES1Driver(CreationParams, data, FileSystem);
				if (!VideoDriver)
				{
					os::Printer::log("Could not create OpenGL-ES1 driver.", ELL_ERROR);
				}
			}
			#else
			os::Printer::log("OpenGL-ES1 driver was not compiled in.", ELL_ERROR);
			#endif
			break;

		case video::EDT_OGLES2:
			#ifdef _IRR_COMPILE_WITH_OGLES2_
			{
				video::SExposedVideoData data;				

				VideoDriver = video::createOGLES2Driver(CreationParams, data, FileSystem);
				if (!VideoDriver)
				{
					os::Printer::log("Could not create OpenGL-ES2 driver.", ELL_ERROR);
				}
			}
			#else
			os::Printer::log("OpenGL-ES2 driver was not compiled in.", ELL_ERROR);
			#endif
			break;

		case video::EDT_NULL:
			// create null driver
			VideoDriver = video::createNullDriver(FileSystem, CreationParams.WindowSize);
			break;

		default:
			os::Printer::log("Unable to create video driver of unknown type.", ELL_ERROR);
			break;
		}
	}

	//! runs the device. Returns false if device wants to be deleted
	bool CIrrDeviceAndroid::run()
	{
		os::Timer::tick();
		return true;
	}

	//! Cause the device to temporarily pause execution and let other processes to run
	// This should bring down processor usage without major performance loss for Irrlicht
	void CIrrDeviceAndroid::yield()
	{
	}

	//! Pause execution and let other processes to run for a specified amount of time.
	void CIrrDeviceAndroid::sleep(u32 timeMs, bool pauseTimer)
	{
	}

	//! sets the caption of the window
	void CIrrDeviceAndroid::setWindowCaption(const wchar_t* text)
	{
		// todo later
	}

	//! returns if window is active. if not, nothing need to be drawn
	bool CIrrDeviceAndroid::isWindowActive() const
	{
		// In android it have no func
		return true;
	}

	//! returns if window has focus
	bool CIrrDeviceAndroid::isWindowFocused() const
	{
		// In android it have no func
		return true;
	}

	//! returns if window is minimized
	bool CIrrDeviceAndroid::isWindowMinimized() const
	{
		// In android it have no func
		return false;
	}

	//! presents a surface in the client area
	bool CIrrDeviceAndroid::present(video::IImage* surface, void* windowId, core::rect<s32>* src)
	{
		// get image from screen
		// in android we have no function
		return false;
	}

	//! notifies the device that it should close itself
	void CIrrDeviceAndroid::closeDevice()
	{
	}

	//! \return Returns a pointer to a list with all video modes
	//! supported by the gfx adapter.
	video::IVideoModeList* CIrrDeviceAndroid::getVideoModeList()
	{
		// In android it have no func
		return NULL;
	}

	//! Sets if the window should be resizable in windowed mode.
	void CIrrDeviceAndroid::setResizable(bool resize)
	{
		// enable resize windows
		// In android it have no func
	}

	//! Minimizes the window.
	void CIrrDeviceAndroid::minimizeWindow()
	{
		// In android it have no func
	}

	//! Maximizes the window.
	void CIrrDeviceAndroid::maximizeWindow()
	{
		// In android it have no func
	}

	//! Restores the window size.
	void CIrrDeviceAndroid::restoreWindow()
	{
		// In android it have no func
	}

	//! Activate any joysticks, and generate events for them.
	bool CIrrDeviceAndroid::activateJoysticks(core::array<SJoystickInfo> & joystickInfo)
	{
		return false;
	}

	//! Set the current Gamma Value for the Display
	bool CIrrDeviceAndroid::setGammaRamp( f32 red, f32 green, f32 blue, f32 brightness, f32 contrast )
	{
		return false;
	}

	//! Get the current Gamma Value for the Display
	bool CIrrDeviceAndroid::getGammaRamp( f32 &red, f32 &green, f32 &blue, f32 &brightness, f32 &contrast )
	{
		return false;
	}

	//! Remove all messages pending in the system message loop
	void CIrrDeviceAndroid::clearSystemMessages()
	{
		// In android it have no func
	}

}

#endif