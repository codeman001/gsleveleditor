#ifndef _SNAKEBONE_H
#define _SNAKEBONE_H

#include "stdafx.h"

typedef list<core::vector3df>	listVector3df;

class CGameSnakeAnimator: public ISceneNodeAnimator
{
public:
	listVector3df			m_position;
	listVector3df			m_bone;

	float					m_snakeLength;
	float					m_snakeStep;	

	float					m_moveSpeed;
	core::vector3df			m_moveVec;	

	long					m_lastAnimationTime;

public:
	CGameSnakeAnimator();

	virtual ~CGameSnakeAnimator();

	void debugRender();

	// /////////////////////////////////////////////////////
	// ISceneNodeAnimator implement
	// /////////////////////////////////////////////////////

	bool getNextBone( core::vector3df *bone, float f );

	virtual void animateNode(ISceneNode* node, u32 timeMs);
			
	virtual bool OnEvent(const SEvent& gameEvent);

	virtual ISceneNodeAnimator* createClone(ISceneNode* node, ISceneManager* newManager = 0)
	{
		return NULL;
	}

	virtual bool isEventReceiverEnabled() const
	{
		return true;
	}	

	virtual bool hasFinished(void) const
	{
		return true;
	}

	inline listVector3df* getListPosition()
	{
		return &m_position;
	}
};

#endif