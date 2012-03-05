#ifndef _CCUBEOBJECT_H_
#define _CCUBEOBJECT_H_

#include "CGameObject.h"

class CCubeObject: public CGameObject
{
protected:
	
public:
	CCubeObject( float size );

	virtual ~CCubeObject();

	// updateObject
	// update object by frame
	virtual void updateObject();
};

#endif