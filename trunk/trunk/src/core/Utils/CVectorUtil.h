#ifndef _VECTOR_UTILS_
#define _VECTOR_UTILS_

#include "stdafx.h"

class CVectorUtil
{
public:
	// turnToDir
	static bool turnToDir(core::vector3df& dir, const core::vector3df& turnTo, float speed );
    
	// turnToDir
	// turn vector dir to turnTo
	static core::vector3df interpolateTurnToDir( const core::vector3df& turnFrom, const core::vector3df& turnTo, float f );
   
	// getAngle
	// get angle of 2 vector
	static float getAngle( const core::vector3df& v1, const core::vector3df& v2 );	
    
	// fixAngle
	static float fixAngle( float f );
};

#endif