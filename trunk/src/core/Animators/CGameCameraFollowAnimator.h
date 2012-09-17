#ifndef _CAMERA_FOLLOW_ANIMATOR_
#define _CAMERA_FOLLOW_ANIMATOR_

#include "IEventReceiver.h"
#include "ISceneNodeAnimator.h"

#include "stdafx.h"

class CGameCameraFollowAnimator: public ISceneNodeAnimator
{
protected:
		float				m_radius;
		float				m_rotateSpeed;

		core::vector3df		m_rotate;
		ISceneNode			*m_followNode;
		gui::ICursorControl *m_cursorControl;

		bool				m_firstUpdate;
		s32					m_lastAnimationTime;

		bool				m_leftMousePress;
		core::position2d<f32> m_centerCursor, m_cursorPos;
    
		core::vector3df		m_currentPos;
		core::vector3df		m_currentTarget;
		core::vector3df		m_toPos;
		core::vector3df		m_toTarget;
public:
		//! Constructor
		CGameCameraFollowAnimator(	gui::ICursorControl* cursorControl, float radius, f32 rotateSpeed = 20.0f );

		//! Destructor
		virtual ~CGameCameraFollowAnimator();

		//! Event receive
		bool OnEvent(const SEvent& event);

		//! Animates a scene node.
		/** \param node Node to animate.
		\param timeMs Current time in milli seconds. */
		virtual void animateNode(ISceneNode* node, u32 timeMs);

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling this. */
		virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager=0);

		//! Returns true if this animator receives events.
		/** When attached to an active camera, this animator will be
		able to respond to events such as mouse and keyboard events. */
		virtual bool isEventReceiverEnabled() const
		{
			return true;
		}

		
		inline void setFollowNode( ISceneNode *node )
		{
			m_followNode = node;
		}
		
		inline void setRotateSpeed( float speed )
		{
			m_rotateSpeed = speed;
		}

		inline void setRadius( float r )
		{
			m_radius = r;
		}
protected:
	
	// checkCollide
	// check camera ray is collide with terrain
	bool checkCollide( const core::vector3df& target, const core::vector3df& pos, core::vector3df& outPos );

};

#endif