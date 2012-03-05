#ifndef _CANIMOBJECT_H
#define _CANIMOBJECT_H

#include "CGameObject.h"

class CAnimObject: public CGameObject
{
public:
	
protected:
	
public:
	CAnimObject();

	virtual ~CAnimObject();

	// updateObject
	// update object by frame
	virtual void updateObject();

	// loadFromFile
	// load anim object from file
	void loadFromFile( char *lpFilename );
};


#endif