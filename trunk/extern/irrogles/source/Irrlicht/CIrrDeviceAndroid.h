#ifndef __C_IRR_DEVICE_WINCE_H_INCLUDED__
#define __C_IRR_DEVICE_WINCE_H_INCLUDED__

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_ANDROID_DEVICE_

#include "CIrrDeviceStub.h"
#include "IrrlichtDevice.h"
#include "IImagePresenter.h"

namespace irr
{

	class CIrrDeviceAndroid:  public CIrrDeviceStub, public video::IImagePresenter
	{
	public:
			//! constructor
			CIrrDeviceAndroid(const SIrrlichtCreationParameters& params, const char* dataPak);

			//! destructor
			virtual ~CIrrDeviceAndroid();

			//! runs the device. Returns false if device wants to be deleted
			virtual bool run();

			//! Cause the device to temporarily pause execution and let other processes to run
			// This should bring down processor usage without major performance loss for Irrlicht
			virtual void yield();

			//! Pause execution and let other processes to run for a specified amount of time.
			virtual void sleep(u32 timeMs, bool pauseTimer);

			//! sets the caption of the window
			virtual void setWindowCaption(const wchar_t* text);

			//! returns if window is active. if not, nothing need to be drawn
			virtual bool isWindowActive() const;

			//! returns if window has focus
			virtual bool isWindowFocused() const;

			//! returns if window is minimized
			virtual bool isWindowMinimized() const;

			//! presents a surface in the client area
			virtual bool present(video::IImage* surface, void* windowId=0, core::rect<s32>* src=0);

			//! notifies the device that it should close itself
			virtual void closeDevice();

			//! \return Returns a pointer to a list with all video modes
			//! supported by the gfx adapter.
			video::IVideoModeList* getVideoModeList();		

			//! Sets if the window should be resizable in windowed mode.
			virtual void setResizable(bool resize=false);

			//! Minimizes the window.
			virtual void minimizeWindow();

			//! Maximizes the window.
			virtual void maximizeWindow();

			//! Restores the window size.
			virtual void restoreWindow();

			//! Activate any joysticks, and generate events for them.
			virtual bool activateJoysticks(core::array<SJoystickInfo> & joystickInfo);

			//! Set the current Gamma Value for the Display
			virtual bool setGammaRamp( f32 red, f32 green, f32 blue, f32 brightness, f32 contrast );

			//! Get the current Gamma Value for the Display
			virtual bool getGammaRamp( f32 &red, f32 &green, f32 &blue, f32 &brightness, f32 &contrast );

			//! Remove all messages pending in the system message loop
			virtual void clearSystemMessages();

			//! Get the device type
			virtual E_DEVICE_TYPE getType() const
			{
				return EIDT_ANDROID;
			}
	private:
			//! create the driver
			void createDriver();
	};

}

#endif

#endif