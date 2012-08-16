#include "stdafx.h"
#include "IView.h"
#include "CAddLightController.h"

CAddLightController::CAddLightController()
{
	m_temp = new CLightObject();
	m_temp->setVisible( false );
}

CAddLightController::~CAddLightController()
{
	delete m_temp;
}

void CAddLightController::initController()
{
	m_temp->setVisible(false);
}

void CAddLightController::cancelController()
{
	m_temp->setVisible(false);
}
			
void CAddLightController::onLMouseUp(int x, int y)
{
	IView *pView = getIView();
	ICameraSceneNode *cam = pView->getSceneMgr()->getActiveCamera();
		
	// get position
	core::vector3df hit;
	bool b = getPickPosition( &hit );
	
	if ( b )
	{
		core::vector3df camPos = cam->getAbsolutePosition();		
		core::line3df camRay( camPos, hit );

		if ( camRay.getLength() < 5000 )
		{
			CLightObject *pObj =	pView->getCurrentZone()->createLight();
			
			pObj->setPosition( hit );
			pObj->setVisible( true );

			// add history
			CHistoryManager::getInstance()->beginHistory();
			CHistoryManager::getInstance()->addHistoryCreateObj( pObj );
			CHistoryManager::getInstance()->endHistory();

		}
		else
			pView->alertError( L"Can not create object because is too far" );
	}
	else
		pView->alertError( L"Can not create object because is too far" );
}

void CAddLightController::onRMouseUp(int x, int y)
{
}

void CAddLightController::onMouseMove(int x, int y)
{
	IView *pView = getIView();

	// get position
	core::vector3df hit;
	bool b = getPickPosition( &hit );
	
	if ( b )
	{
		if ( m_temp->isVisible() == false )
			m_temp->setVisible( true );
		
		if ( m_temp )
		{
			m_temp->updateObject();
			m_temp->setPosition( hit );
			m_temp->getDirectionObject()->setPosition( hit + core::vector3df(0,-1,0)*100.0f );
		}
	}
}