#include "stdafx.h"

#include "CGameSnakeSceneNode.h"
#include "CGameSnakeAnimator.h"
#include "IView.h"


CGameSnakeAnimator::CGameSnakeAnimator()
{	
	m_moveSpeed		= 0.1f;
	m_moveVec		= core::vector3df(1.0f, 0, 0);
	
	m_snakeLength	= 400.0f;
	m_snakeStep		= 100.0f;

	core::vector3df	tailPosition = core::vector3df(0, 0, 0);
	core::vector3df headPosition = tailPosition + m_snakeLength*m_moveVec;	
	
	m_position.push_back( headPosition );
	m_position.push_back( tailPosition );

	m_lastAnimationTime = -1;
}

CGameSnakeAnimator::~CGameSnakeAnimator()
{

}

bool CGameSnakeAnimator::getNextBone( core::vector3df *bone, float f )
{
	listVector3df::iterator i = --m_position.end(), begin = m_position.begin(), temp;

	float length = 0;		

	while ( 1 )
	{
		core::vector3df& v1 = (*i);
		temp = i; temp--;
		core::vector3df& v2 = (*temp);

		length = length + v2.getDistanceFrom( v1 );
		core::vector3df v = (v2 - v1).normalize();

		if ( length > f )
		{
			*bone = v1 + v * f;
			return true;
		}
		
		*bone = v2;

		i = temp;
		if ( i == begin )
			break;
	}

	return false;
}

void CGameSnakeAnimator::animateNode(ISceneNode* node, u32 timeMs)
{	
	long diff = timeMs - m_lastAnimationTime;
	if ( m_lastAnimationTime == -1 )
		diff = 10;
	m_lastAnimationTime = timeMs;


	// calc head
	core::vector3df& headPosition = m_position.front();
	core::vector3df move = m_moveSpeed * diff * m_moveVec;
	headPosition = headPosition + move;


	// calc tail!!!
	float length = m_snakeLength;
	core::vector3df *begin = NULL;
	listVector3df::iterator i = m_position.begin(), iEnd = m_position.end();
	while ( i != iEnd )
	{
		core::vector3df *end = &(*i);

		if ( begin != NULL )
		{
			float segLen = begin->getDistanceFrom(*end);
			if ( segLen > length )
			{
				float f = segLen - length;
				*end += (*begin - *end).normalize() * f;
				i++;
				break;
			}
			length = length - segLen;
		}

		begin = end;
		i++;
	}
	while ( i != iEnd )
		i = m_position.erase( i );	
	

	m_bone.clear();

	core::vector3df bone;
	float f = 0;
	float d = 0;
	
	while ( getNextBone( &bone, f ) == true )
	{
		m_bone.push_front( bone );
		f = f + m_snakeStep;
	}


	//m_bone.assign( m_position.begin(), m_position.end() );

	// need update mesh
	((CGameSnakeSceneNode*) node)->updateMesh( &m_bone );
}
	
bool CGameSnakeAnimator::OnEvent(const SEvent& gameEvent)
{	
	if ( gameEvent.EventType == EET_KEY_INPUT_EVENT )
	{		
		core::matrix4 mat;

		// on key up
		if ( gameEvent.KeyInput.PressedDown == false )
		{
			if ( gameEvent.KeyInput.Key == KEY_LEFT )
				mat.setRotationDegrees( core::vector3df(0.0, -20.0f, 0.0f) );
			else if ( gameEvent.KeyInput.Key == KEY_RIGHT )
				mat.setRotationDegrees( core::vector3df(0.0,  20.0f, 0.0f) );

			// add the mid
			m_position.push_front( m_position.front() );

			mat.rotateVect(m_moveVec);
		}		
	}
	return false;
}

void CGameSnakeAnimator::debugRender()
{
	IVideoDriver *driver = getIView()->getDriver();

	// set material
	SMaterial debug_mat;	
	debug_mat.Lighting = false;
	debug_mat.AntiAliasing=0;
	driver->setMaterial(debug_mat);

	// reset transform
	core::matrix4 mat;
	mat.makeIdentity();
	driver->setTransform(video::ETS_WORLD, mat);

	core::vector3df *begin = NULL;
	listVector3df::iterator i = m_position.begin(), end = m_position.end();
	while ( i != end )
	{
		core::vector3df *end = &(*i);

		if ( begin != NULL )
			driver->draw3DLine( *begin, *end );
		
		begin = end;
		i++;
	}

}