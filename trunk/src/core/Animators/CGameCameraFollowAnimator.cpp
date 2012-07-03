#include "stdafx.h"
#include "CGameCameraFollowAnimator.h"


//! Constructor
CGameCameraFollowAnimator::CGameCameraFollowAnimator(	gui::ICursorControl* cursorControl, float radius, f32 rotateSpeed )
{
	m_radius = radius;
	m_rotateSpeed = rotateSpeed;

	m_firstUpdate = true;
	m_followNode = NULL;

	m_leftMousePress = false;
	m_cursorControl = cursorControl;
}

//! Destructor
CGameCameraFollowAnimator::~CGameCameraFollowAnimator()
{
}

//! Event receiver
bool CGameCameraFollowAnimator::OnEvent(const SEvent& evt)
{
	switch(evt.EventType)
	{
	case EET_KEY_INPUT_EVENT:		
		break;

	case EET_MOUSE_INPUT_EVENT:
		if (evt.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
		{
			m_leftMousePress = true;
			
			m_centerCursor = m_cursorControl->getRelativePosition();
			m_cursorPos = m_centerCursor;
		}
		else if ( evt.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
		{
			m_leftMousePress = false;
		}		
		else if ( evt.MouseInput.Event == EMIE_MOUSE_MOVED )
		{
			if ( m_leftMousePress )
			{
				m_cursorPos = m_cursorControl->getRelativePosition();
				return true;
			}
		}		
		break;
	default:
		break;
	}

	return false;

}

//! Animates a scene node.
/** \param node Node to animate.
\param timeMs Current time in milli seconds. */
void CGameCameraFollowAnimator::animateNode(ISceneNode* node, u32 timeMs)
{
	if (!node || node->getType() != ESNT_CAMERA || m_followNode == NULL)
		return;

	ICameraSceneNode *camera = (ICameraSceneNode*)node;
	
	if (m_firstUpdate)
	{
		m_lastAnimationTime = timeMs;
		m_firstUpdate = false;
	}

	if(!camera->isInputReceiverEnabled())
		return;

	// get time	
	f32 timeDiff = (f32) ( timeMs - m_lastAnimationTime );
	m_lastAnimationTime = timeMs;
	
	const float MaxVerticalAngle = 60;

	if ( m_cursorPos != m_centerCursor && m_leftMousePress )
	{
		// rotate X
		m_rotate.Y -= (m_centerCursor.X - m_cursorPos.X) * m_rotateSpeed * timeDiff;

		// rotate Y
		m_rotate.X += (m_centerCursor.Y - m_cursorPos.Y) * m_rotateSpeed * timeDiff;

		if ( m_rotate.X < -MaxVerticalAngle )
			m_rotate.X = -MaxVerticalAngle;
		if ( m_rotate.X > MaxVerticalAngle )
			m_rotate.X = MaxVerticalAngle;

		m_centerCursor = m_cursorControl->getRelativePosition();
		m_cursorPos = m_centerCursor;
	}	

	core::aabbox3df box = m_followNode->getTransformedBoundingBox();
	core::vector3df target = box.getCenter();	
	core::vector3df pos = core::vector3df(0,0, m_radius );

	core::matrix4 mat;
	mat.setRotationDegrees(core::vector3df(m_rotate.X, m_rotate.Y, 0));
	mat.transformVect(pos);

	pos = target + pos;

	camera->setPosition( pos );
	camera->setTarget( target );

}

//! Creates a clone of this animator.
/** Please note that you will have to drop
(IReferenceCounted::drop()) the returned pointer after calling this. */
ISceneNodeAnimator* CGameCameraFollowAnimator::createClone(ISceneNode* node, ISceneManager* newManager)
{
	return NULL;
}