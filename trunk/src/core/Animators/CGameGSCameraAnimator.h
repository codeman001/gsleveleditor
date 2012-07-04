#ifndef _GAMEGSCAMERAANIMATORS_
#define _GAMEGSCAMERAANIMATORS_

#include "stdafx.h"
#include "ISceneNodeAnimatorCameraFPS.h"

class CGameGSCameraAnimators : public ISceneNodeAnimatorCameraFPS
{
public:
		//! Constructor
		CGameGSCameraAnimators(	gui::ICursorControl* cursorControl,	f32 rotateSpeed = 2.0f,	f32 moveSpeed = 0.5f );

		//! Destructor
		virtual ~CGameGSCameraAnimators();

		//! Animates the scene node, currently only works on cameras
		virtual void animateNode(ISceneNode* node, u32 timeMs);

		//! Event receiver
		virtual bool OnEvent(const SEvent& event);

		//! Returns the speed of movement in units per second
		virtual f32 getMoveSpeed() const;

		//! Sets the speed of movement in units per second
		virtual void setMoveSpeed(f32 moveSpeed);

		//! Returns the rotation speed
		virtual f32 getRotateSpeed() const;

		//! Set the rotation speed
		virtual void setRotateSpeed(f32 rotateSpeed);
	
		//! Sets the keyboard mapping for this animator
		//! \param keymap: an array of keyboard mappings, see SKeyMap
		//! \param count: the size of the keyboard map array
		virtual void setKeyMap(SKeyMap *map, u32 count);

		//! Sets whether vertical movement should be allowed.
		virtual void setVerticalMovement(bool allow);

		//! Sets whether the Y axis of the mouse should be inverted.
		/** If enabled then moving the mouse down will cause
		the camera to look up. It is disabled by default. */
		virtual void setInvertMouse(bool invert);

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer once you're
		done with it. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0);

		virtual bool isEventReceiverEnabled() const;

		// enableMove
		// enable/disable move by right click
		virtual void enableMove( bool b );

protected:
		bool m_enableMove;

		f32	m_moveSpeed;
		f32 m_rotateSpeed;
		gui::ICursorControl *m_cursorControl;

		core::position2d<f32> m_centerCursor, m_cursorPos;

		bool	m_leftMousePress;
		bool	m_rightMousePress;
		bool	m_midMousePress;

		bool	m_mouseWhell;
		float	m_wheel;

		bool	m_firstUpdate;
		long	m_lastAnimationTime;
};

#endif