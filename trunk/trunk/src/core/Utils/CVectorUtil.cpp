#include "stdafx.h"
#include "CVectorUtil.h"

#include "IView.h"

// turnToDir
bool CVectorUtil::turnToDir(core::vector3df& dir, const core::vector3df& turnTo, float speed )
{
	// rotate front vec
	float f = speed*0.1f*getIView()->getTimeStep();	
	
	if ( f >= fabsf(getAngle(dir, turnTo)) )
	{
		dir = turnTo;
        return true;
	}
    
    dir = interpolateTurnToDir( dir, turnTo, f );
    
    float rot = fabsf(getAngle(dir, turnTo));
    if ( rot <= speed )
    {
        dir = turnTo;
        return true;
    }
    
	return false;
}

// turnToDir
// turn vector dir to turnTo
core::vector3df CVectorUtil::interpolateTurnToDir( const core::vector3df& turnFrom, const core::vector3df& turnTo, float f )
{
	// calc turn Direction
    core::vector3df normal;
    normal = turnTo.crossProduct(turnFrom);
    normal.normalize();
    
    core::quaternion q;
    q.fromAngleAxis( core::degToRad(f), normal);
    
	// rotate result vector
	core::vector3df dir = turnFrom;
	q.getMatrix().rotateVect( dir );
	dir.normalize();
    
	return dir;	
}

// get angle of 2 vector
float CVectorUtil::getAngle( const core::vector3df& v1, const core::vector3df& v2 )
{
	core::vector3df normal = v2.crossProduct(v1);	
	
	float angleVec = v2.dotProduct( v1 );
	
	angleVec = fixAngle(angleVec);
	angleVec = core::radToDeg( acosf(angleVec) );
    
	if ( normal.Y < 0 )
		angleVec = -angleVec;
	return angleVec;
}

////////////////////////////////////////////////////////////
#define REAL_PRECISION	0.000001f
inline bool realIsZero(float x, float precision = REAL_PRECISION) 
{ 
	return fabs(x) < precision; 
};
inline bool realIsEqual(float a, float b, float precision = REAL_PRECISION) 
{ 
	return realIsZero(a-b, precision); 
};

// fixAngle
float CVectorUtil::fixAngle( float f )
{
	if (realIsEqual(f, 1.f, 0.001f))
		f = 1.f;
	else if (realIsEqual(f, -1.f, 0.001f))
		f = -1.f;
	return f;
}