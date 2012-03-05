#ifndef _CANIMOBJECT_H
#define _CANIMOBJECT_H

#include "CGameObject.h"

class CAnimObject: public CGameObject
{
public:
	
protected:
	IAnimatedMeshSceneNode*	m_animNode;

public:
	CAnimObject();

	virtual ~CAnimObject();

	// updateObject
	// update object by frame
	virtual void updateObject();

	// loadFromFile
	// load anim object from file
	void loadFromFile( char *lpFilename );

	// setSpeed
	// set speed for anim (frame/sercond)
	inline void setSpeed( float speed )
	{
		if ( m_animNode )
			m_animNode->setAnimationSpeed( speed );
	}
};


#endif