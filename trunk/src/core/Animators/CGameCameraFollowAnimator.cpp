#include "stdafx.h"
#include "CGameCameraFollowAnimator.h"

#include "CTerrainComponent.h"
#include "gameLevel/CGameLevel.h"

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
		m_lastAnimationTime = timeMs;		

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

	// calc position
	pos = target + pos;

	// recalc target (look at right character)
	target = target - pos;
	mat.setRotationDegrees( core::vector3df(0, 10, 0) );
	mat.transformVect(target);
	target = pos + target;

	// need check camera collision with another object
	core::vector3df	colPos;
	if ( checkCollide(target, pos, colPos ) == true )
	{
		pos = colPos;
	}

	if ( m_firstUpdate )
	{
		m_currentPos	= pos;
		m_currentTarget	= target;
		m_toPos			= pos;
		m_toTarget		= target;

		camera->setPosition( pos );
		camera->setTarget( target );

		m_firstUpdate = false;
	}
	else
	{
		m_toPos			= pos;
		m_toTarget		= target;

		core::vector3df deltaPos	= m_toPos - m_currentPos;
		core::vector3df deltaTarget	= m_toTarget - m_currentTarget;

		if ( deltaPos.getLengthSQ() <= 50 )
			m_currentPos	= m_toPos;
		else
			m_currentPos	= m_currentPos		+ deltaPos * timeDiff * 0.01f;

		if ( deltaTarget.getLengthSQ() <= 50 )
			m_currentTarget	= m_toTarget;
		else
			m_currentTarget	= m_currentTarget	+ deltaTarget * timeDiff * 0.01f;

		camera->setPosition( m_currentPos );
		camera->setTarget( m_currentTarget );
	}
}

//! Creates a clone of this animator.
/** Please note that you will have to drop
(IReferenceCounted::drop()) the returned pointer after calling this. */
ISceneNodeAnimator* CGameCameraFollowAnimator::createClone(ISceneNode* node, ISceneManager* newManager)
{
	return NULL;
}

// checkCollide
// check camera ray is collide with terrain
bool CGameCameraFollowAnimator::checkCollide( const core::vector3df& target, const core::vector3df& pos, core::vector3df& outPos )
{	
	CGameLevel *level =	CGameLevel::getCurrentLevel();
	if ( level == NULL )
		return false;


	core::aabbox3df myBox(target, pos);	
	myBox.repair();

	core::line3df		ray( target, pos );
	core::triangle3df	colTri;
	core::vector3df		colPos;

	float	maxLength = ray.getLengthSQ();	
	bool	hasCol = false;
	float	minDistance = ray.getLength();

	// loop for all zone
	int nZone = level->getZoneCount();
	for ( int iZone = 0; iZone < nZone; iZone++ )
	{
		CZone *z = level->getZone( iZone );
		core::aabbox3df zoneBox = z->getSceneNode()->getTransformedBoundingBox();		
		
		if ( zoneBox.intersectsWithBox( myBox ) == true )
		{
			// loop all terrain object in zone
			ArrayGameObject& terrains = z->getTerrainList();
			ArrayGameObject::iterator itTerrain = terrains.begin(), itTerrainEnd = terrains.end();
			while ( itTerrain != itTerrainEnd )
			{
				CGameObject *objTerrain = ((CGameObject*) *itTerrain);

				// loop all scene node in a terrain object
				CTerrainComponent* com  = (CTerrainComponent*)objTerrain->getComponent( IObjectComponent::Terrain );
				if ( com )
				{
					if ( com->getCollisionFromRay( ray, maxLength, colPos, colTri ) == true )
					{
						float distance = colPos.getDistanceFrom(target);
						if ( distance < minDistance ) 
						{							
							minDistance = distance;
							hasCol = true;
							outPos = colPos;
						}
					}
				}
				
				itTerrain++;
			}
		}

	}

	return hasCol;
}