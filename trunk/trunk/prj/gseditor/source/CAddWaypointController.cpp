#include "stdafx.h"
#include "CAddWaypointController.h"

#include "IView.h"
#include "CGameObject.h"
#include "CZone.h"
#include "CObjTemplateFactory.h"

CAddWaypointController::CAddWaypointController()
{
	m_temp = new CWayPoint();
	m_temp->setVisible( false );
}

CAddWaypointController::~CAddWaypointController()
{
	delete m_temp;
}

void CAddWaypointController::initController()
{	
	m_temp->setVisible( false );	
}

void CAddWaypointController::cancelController()
{
	m_temp->setVisible( false );
}
			
void CAddWaypointController::onLMouseUp	(int x, int y)
{
}

void CAddWaypointController::onMouseMove(int x, int y)
{
	IView *pView = getIView();

	// move object
	core::line3df	ray = pView->getSelectRay();
	core::plane3df	plane( core::vector3df(0.0f, 0.0f, 0.0f), core::vector3df(0.0f, 1.0f, 0.0f) );
	
	// get position
	core::vector3df hit;
	bool b = plane.getIntersectionWithLine( ray.start, ray.getVector(), hit );
	
	if ( b )
	{
		if ( m_temp->isVisible() == false )
			m_temp->setVisible( true );
		
		if ( m_temp )
		{
			m_temp->updateObject();
			m_temp->setPosition( hit );			
		}
	}
}