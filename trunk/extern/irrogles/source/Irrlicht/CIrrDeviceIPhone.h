// Copyright (C) 2002-2008 Nikolaus Gebhardt
// Copyright (C) 2008 Redshift Software, Inc.
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_IRR_DEVICE_IPHONE_H_INCLUDED__
#define __C_IRR_DEVICE_IPHONE_H_INCLUDED__

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_IPHONE_DEVICE_

#ifdef __cplusplus
extern "C"
#else
extern
#endif

//void irr_device_iphone_create(struct MIrrIPhoneDevice * dev);

#ifdef __cplusplus

#include "CIrrDeviceStub.h"
#include "IrrlichtDevice.h"
#include "IImagePresenter.h"

//~ #include <video/ogles1.h>
#include <CoreFoundation/CFRunLoop.h>

namespace irr
{

	class CIrrDeviceIPhone : public CIrrDeviceStub, public video::IImagePresenter
	{
    protected:
        
	public:
		typedef void * NativeWindowType;
        
		//! constructor
		CIrrDeviceIPhone(const SIrrlichtCreationParameters& params, const char *packData);

		//! destructor
		virtual ~CIrrDeviceIPhone();

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

		//! Checks if the Irrlicht window has focus
		virtual bool isWindowFocused() const;

		//! Checks if the Irrlicht window is minimized
		virtual bool isWindowMinimized() const;

		//! presents a surface in the client area
		virtual bool present(video::IImage* surface, void * windowId = 0, core::rect<s32>* src=0 );

		//! notifies the device that it should close itself
		virtual void closeDevice();

		//! Sets if the window should be resizeable in windowed mode.
		virtual void setResizable(bool resize=false);
		
		//! Minimizes the window
		virtual void minimizeWindow();
		
		//! Maximizes the window if possible.
		virtual void maximizeWindow();

		//! Restore the window to normal size if possible.
		virtual void restoreWindow();

		//! De/activates the window. When not active no rendering takes place.
		virtual void setWindowActive(bool);

		//! Get the type of the device.
		/** This allows the user to check which windowing system is currently being
		used. */
		virtual E_DEVICE_TYPE getType() const
		{
			return EIDT_OSX;
		}

	private:		
	
		volatile bool Close;
		volatile bool Closed;
		volatile bool WindowActive;
		volatile bool WindowMinimized;

		//! Create the driver.
		void createDriver();		
				
	};
    
    
    class CIPhoneCursor: public gui::ICursorControl
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
    

} // end namespace irr

#endif // __cplusplus

#endif // _IRR_COMPILE_WITH_IPHONE_DEVICE_

#endif // __C_IRR_DEVICE_IPHONE_H_INCLUDED__

