#include "stdafx.h"
#include "CGameGSCameraAnimators.h"

//! Constructor
CGameGSCameraAnimators::CGameGSCameraAnimators(	gui::ICursorControl* cursorControl,	f32 rotateSpeed,	f32 moveSpeed)
{
	m_cursorControl = cursorControl;
	m_rotateSpeed = rotateSpeed;
	m_moveSpeed = moveSpeed;

	m_mousePress = false;
	m_firstUpdate = true;
}

//! Destructor
CGameGSCameraAnimators::~CGameGSCameraAnimators()
{
}

//! Animates the scene node, currently only works on cameras
void CGameGSCameraAnimators::animateNode(ISceneNode* node, u32 timeMs)
{
	if (!node || node->getType() != ESNT_CAMERA)
		return;

	ICameraSceneNode* camera = static_cast<ICameraSceneNode*>(node);

	if (m_firstUpdate)
	{
		//m_centerCursor = m_cursorControl->getRelativePosition();
		//m_cursorPos = m_centerCursor;

		m_lastAnimationTime = timeMs;
		m_firstUpdate = false;
	}

	if(!camera->isInputReceiverEnabled())
		return;

	// get time
	f32 timeDiff = (f32) ( timeMs - m_lastAnimationTime );
	m_lastAnimationTime = timeMs;

	// update position
	core::vector3df pos = camera->getPosition();

	// Update rotation
	core::vector3df target = (camera->getTarget() - camera->getAbsolutePosition());
	core::vector3df relativeRotation = target.getHorizontalAngle();
	
	const float MaxVerticalAngle = 88;
	const int	MouseYDirection = 1;	

	if (m_cursorPos != m_centerCursor)
	{
		// rotate X
		relativeRotation.Y -= (m_centerCursor.X - m_cursorPos.X) * m_rotateSpeed * MouseYDirection;

		// rotate Y
		relativeRotation.X -= (m_centerCursor.Y - m_cursorPos.Y) * m_rotateSpeed;

		if (relativeRotation.X > MaxVerticalAngle*2 && relativeRotation.X < 360.0f-MaxVerticalAngle)
		{
			relativeRotation.X = 360.0f-MaxVerticalAngle;
		}
		else if (relativeRotation.X > MaxVerticalAngle && relativeRotation.X < 360.0f-MaxVerticalAngle)
		{
			relativeRotation.X = MaxVerticalAngle;
		}

		m_centerCursor = m_cursorControl->getRelativePosition();
		m_cursorPos = m_centerCursor;
	}
	
	// set target
	target.set(0,0, core::max_(1.f, pos.getLength()));
	core::vector3df movedir = target;

	core::matrix4 mat;
	mat.setRotationDegrees(core::vector3df(relativeRotation.X, relativeRotation.Y, 0));
	mat.transformVect(target);


	// set position
		


	// write translation
	camera->setPosition(pos);

	// write right target
	target += pos;
	camera->setTarget(target);
}

//! Event receiver
bool CGameGSCameraAnimators::OnEvent(const SEvent& evt)
{
	switch(evt.EventType)
	{
	case EET_KEY_INPUT_EVENT:		
		break;

	case EET_MOUSE_INPUT_EVENT:
		if (evt.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
		{
			m_mousePress = true;
			
			m_centerCursor = m_cursorControl->getRelativePosition();
			m_cursorPos = m_centerCursor;
		}
		else if ( evt.MouseInput.Event == EMIE_RMOUSE_LEFT_UP )
		{
			m_mousePress = false;
		}
		else if ( evt.MouseInput.Event == EMIE_MOUSE_MOVED )
		{
			if ( m_mousePress )
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

//! Returns the speed of movement in units per second
f32 CGameGSCameraAnimators::getMoveSpeed() const
{
	return m_moveSpeed;
}

//! Sets the speed of movement in units per second
void CGameGSCameraAnimators::setMoveSpeed(f32 moveSpeed)
{
	m_moveSpeed = moveSpeed;
}

//! Returns the rotation speed
f32 CGameGSCameraAnimators::getRotateSpeed() const
{
	return m_rotateSpeed;
}

//! Set the rotation speed
void CGameGSCameraAnimators::setRotateSpeed(f32 rotateSpeed)
{
	m_rotateSpeed = rotateSpeed;
}

//! Sets the keyboard mapping for this animator
//! \param keymap: an array of keyboard mappings, see SKeyMap
//! \param count: the size of the keyboard map array
void CGameGSCameraAnimators::setKeyMap(SKeyMap *map, u32 count)
{
}

//! Sets whether vertical movement should be allowed.
void CGameGSCameraAnimators::setVerticalMovement(bool allow)
{
}

//! Sets whether the Y axis of the mouse should be inverted.
/** If enabled then moving the mouse down will cause
the camera to look up. It is disabled by default. */
void CGameGSCameraAnimators::setInvertMouse(bool invert)
{
}

//! Creates a clone of this animator.
/** Please note that you will have to drop
(IReferenceCounted::drop()) the returned pointer once you're
done with it. */
ISceneNodeAnimator* CGameGSCameraAnimators::createClone(ISceneNode* node, ISceneManager* newManager)
{
	return NULL;
}

bool CGameGSCameraAnimators::isEventReceiverEnabled() const
{
	return true;
}