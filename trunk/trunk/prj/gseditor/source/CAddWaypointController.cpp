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
	m_connect = NULL;
}

void CAddWaypointController::cancelController()
{
	m_temp->setVisible( false );
	m_connect = NULL;
}
			
void CAddWaypointController::onLMouseUp	(int x, int y)
{
	IView *pView = getIView();
	ICameraSceneNode *cam = pView->getSceneMgr()->getActiveCamera();

	// move object
	core::line3df	ray = pView->getSelectRay();
	core::plane3df	plane( core::vector3df(0.0f, 0.0f, 0.0f), core::vector3df(0.0f, 1.0f, 0.0f) );
	
	// get position
	core::vector3df hit;
	bool b = plane.getIntersectionWithLine( ray.start, ray.getVector(), hit );
	
	if ( b )
	{
		core::vector3df camPos = cam->getAbsolutePosition();		
		core::line3df camRay( camPos, hit );

		if ( camRay.getLength() < 5000 )
		{
			CWayPoint *pObj =	pView->getCurrentZone()->spawnWaypoint();
			
			pObj->setPosition( hit );
			pObj->setVisible( true );

			if ( m_connect )
			{
				m_connect->setNext( pObj );
				pObj->setBack( m_connect );
			}
			
			m_connect = pObj;

		}
		else
			pView->alertError( L"Can not create object because is too far" );
	}
	else
		pView->alertError( L"Can not create object because is too far" );
}

void CAddWaypointController::onRMouseUp(int x, int y)
{
	m_connect = NULL;
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