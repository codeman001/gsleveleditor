#include "stdafx.h"
#include "CAddCameraController.h"
#include "IView.h"
#include "CZone.h"

CAddCameraController::CAddCameraController()
{
	m_temp = new CGameCamera();
	m_temp->setVisible( false );
}

CAddCameraController::~CAddCameraController()
{
	delete m_temp;
}

void CAddCameraController::initController()
{
	m_temp->setVisible( false );
}

void CAddCameraController::cancelController()
{
	m_temp->setVisible( false );
}
			
void CAddCameraController::onLMouseUp(int x, int y)
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
			CGameCamera *pObj =	pView->getCurrentZone()->createCamera();
			
			pObj->setPosition( hit );
			pObj->setVisible( true );

		}
		else
			pView->alertError( L"Can not create object because is too far" );
	}
	else
		pView->alertError( L"Can not create object because is too far" );
}

void CAddCameraController::onRMouseUp(int x, int y)
{
}

void CAddCameraController::onMouseMove(int x, int y)
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
