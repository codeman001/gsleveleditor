#include "stdafx.h"
#include "CAddObjectController.h"

#include "IView.h"
#include "CGameObject.h"
#include "CZone.h"
#include "CObjTemplateFactory.h"


void CAddObjectController::initController()
{
	IView *pView = getIView();
	wchar_t *currentTemplate = pView->getCurrentObjectTemplate();
	
	CGameObject *p = CObjTemplateFactory::getGameObject( currentTemplate );
	if ( p )
		p->setVisible( true );

	// init first
	onMouseMove( pView->m_mouseX, pView->m_mouseY );
}

void CAddObjectController::cancelController()
{
	IView *pView = getIView();
	wchar_t *currentTemplate = pView->getCurrentObjectTemplate();
	
	CGameObject *p = CObjTemplateFactory::getGameObject( currentTemplate );
	if ( p )
		p->setVisible( false );
}

void CAddObjectController::onMouseMove(int x, int y)
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
		CGameObject *pObj = pView->getGameObjectOfCurrentTemplate();
		if ( pObj && pObj->isVisible() == false )
			pObj->setVisible( true );
		if ( pObj )
		{
			pObj->updateObject();
			pObj->setPosition( hit );			
		}
	}
}

void CAddObjectController::onLMouseUp(int x, int y)
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
			CGameObject *pObj =	pView->getCurrentZone()->spawnObject( pView->getCurrentObjectTemplate() );
			
			if ( pObj == NULL )
				pView->alertError( L"Can not create object because missing template" );
			else
			{
				pObj->setPosition( hit );
				pObj->setVisible( true );
			}
		}
		else
			pView->alertError( L"Can not create object because is too far" );
	}
	else
		pView->alertError( L"Can not create object because is too far" );
}