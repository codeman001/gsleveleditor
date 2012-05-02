#include "stdafx.h"
#include "CBaseAddController.h"

#include "IView.h"
#include "CDocument.h"

bool CBaseAddController::getPickPosition( core::vector3df *pos )
{
	IView *pView = getIView();

	// move object
	core::line3df	ray = pView->getSelectRay();

	// check hit on terrain
	CDocument *pDoc = (CDocument*) pView->getDocument();
	ArrayZone* zones = pDoc->getAllZone();
	
	ArrayZoneIter i = zones->begin(), end = zones->end();
	while ( i != end )
	{
		core::vector3df		colPoint;
		core::triangle3df	colTri;
		float maxDistance = 10000.0f;

		if ( (*i)->getTerrainCollision( ray, maxDistance, colPoint, colTri ) == true )
		{
			*pos = colPoint;
			return true;
		}
		i++;
	}

	core::plane3df	plane( core::vector3df(0.0f, 0.0f, 0.0f), core::vector3df(0.0f, 1.0f, 0.0f) );
	
	// get position
	core::vector3df hit;
	bool b = plane.getIntersectionWithLine( ray.start, ray.getVector(), hit );
	
	if ( b )
	{
		ICameraSceneNode *cam = pView->getSceneMgr()->getActiveCamera();

		core::vector3df camPos = cam->getAbsolutePosition();		
		core::line3df camRay( camPos, hit );

		if ( camRay.getLength() < 5000 )
		{
			*pos = hit;
			return true;
		}
	}

	return false;
}