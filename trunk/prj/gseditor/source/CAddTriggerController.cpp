#include "stdafx.h"
#include "CAddTriggerController.h"
#include "IView.h"
#include "CZone.h"

CAddTriggerController::CAddTriggerController()
{
	m_temp = new CTrigger();
	m_temp->setVisible( false );
}

CAddTriggerController::~CAddTriggerController()
{
	delete m_temp;
}

void CAddTriggerController::initController()
{
	m_temp->setVisible( false );
}

void CAddTriggerController::cancelController()
{
	m_temp->setVisible( false );
}
			
void CAddTriggerController::onLMouseUp	(int x, int y)
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
			CTrigger *pObj =	pView->getCurrentZone()->createTrigger();
			
			pObj->setPosition( hit );
			pObj->setVisible( true );

			// add history
			CHistoryManager* pHistory = CHistoryManager::createGetInstance();
			pHistory->beginHistory();
			pHistory->addHistoryCreateObj( pObj );			
			pHistory->endHistory();			
		}
		else
			pView->alertError( L"Can not create object because is too far" );
	}
	else
		pView->alertError( L"Can not create object because is too far" );
}

void CAddTriggerController::onRMouseUp (int x, int y)
{
}

void CAddTriggerController::onMouseMove	(int x, int y)
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