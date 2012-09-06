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

    
    class CAndroidCursor: public gui::ICursorControl
    {
    protected:
        core::position2d<f32>   m_pos;
        float                   m_invWidth;
        float                   m_invHeight;
    public:
        CIPhoneCursor()
        {
            m_invWidth = 1.0f;
            m_invHeight = 1.0f;
        }
        
        //! Changes the visible state of the mouse cursor.
		/** \param visible: The new visible state. If true, the cursor will be visible,
         if false, it will be invisible. */
		virtual void setVisible(bool visible)
        {
            
        }
        
		//! Returns if the cursor is currently visible.
		/** \return True if the cursor is visible, false if not. */
		virtual bool isVisible() const
        {
            return false;
        }
        
		//! Sets the new position of the cursor.
		/** The position must be
         between (0.0f, 0.0f) and (1.0f, 1.0f), where (0.0f, 0.0f) is
         the top left corner and (1.0f, 1.0f) is the bottom right corner of the
         render window.
         \param pos New position of the cursor. */
		virtual void setPosition(const core::position2d<f32> &pos)
        {
            m_pos = pos;
        }
        
		//! Sets the new position of the cursor.
		/** The position must be
         between (0.0f, 0.0f) and (1.0f, 1.0f), where (0.0f, 0.0f) is
         the top left corner and (1.0f, 1.0f) is the bottom right corner of the
         render window.
         \param x New x-coord of the cursor.
         \param y New x-coord of the cursor. */
		virtual void setPosition(f32 x, f32 y)
        {
            m_pos.X = x;
            m_pos.Y = y;             
        }
        
		//! Sets the new position of the cursor.
		/** \param pos: New position of the cursor. The coordinates are pixel units. */
		virtual void setPosition(const core::position2d<s32> &pos)
        {
            m_pos.X = (f32)pos.X;
            m_pos.Y = (f32)pos.Y;
        }
        
		//! Sets the new position of the cursor.
		/** \param x New x-coord of the cursor. The coordinates are pixel units.
         \param y New y-coord of the cursor. The coordinates are pixel units. */
		virtual void setPosition(s32 x, s32 y)
        {
            m_pos.X = (f32)x;
            m_pos.Y = (f32)y;            
        }
        
		//! Returns the current position of the mouse cursor.
		/** \return Returns the current position of the cursor. The returned position
         is the position of the mouse cursor in pixel units. */
		virtual const core::position2d<s32>& getPosition()
        {
            static core::position2di pos;
            pos.X = (s32)m_pos.X;
            pos.Y = (s32)m_pos.Y;
            return pos;
        }
        
		//! Returns the current position of the mouse cursor.
		/** \return Returns the current position of the cursor. The returned position
         is a value between (0.0f, 0.0f) and (1.0f, 1.0f), where (0.0f, 0.0f) is
         the top left corner and (1.0f, 1.0f) is the bottom right corner of the
         render window. */
		virtual core::position2d<f32> getRelativePosition()
        {
            return core::position2d<f32>(m_pos.X*m_invWidth, m_pos.Y*m_invHeight);
        }
        
		//! Sets an absolute reference rect for setting and retrieving the cursor position.
		/** If this rect is set, the cursor position is not being calculated relative to
         the rendering window but to this rect. You can set the rect pointer to 0 to disable
         this feature again. This feature is useful when rendering into parts of foreign windows
         for example in an editor.
         \param rect: A pointer to an reference rectangle or 0 to disable the reference rectangle.*/
		virtual void setReferenceRect(core::rect<s32>* rect)
        {
            m_invWidth = 1.0f/rect->getWidth();
            m_invHeight = 1.0f/rect->getHeight();
        }
        
    };

    
}

#endif

#endif