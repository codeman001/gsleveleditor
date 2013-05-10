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
	
	if ( p == NULL )
	{				
		// create game object
		p = CObjTemplateFactory::spawnObject(currentTemplate );
		p->setObjectState( CGameObject::Move );
		CObjTemplateFactory::addTemplateObject( p );

		// reset template
		getIView()->setCurrentObjectTemplate(currentTemplate);
	}
	
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

	// get position
	core::vector3df hit;
	bool b = getPickPosition( &hit );
	
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
		
	// get position
	core::vector3df hit;
	bool b = getPickPosition( &hit );
	
	if ( b )
	{
		core::vector3df camPos = cam->getAbsolutePosition();		
		core::line3df camRay( camPos, hit );

		if ( camRay.getLength() < 5000 )
		{
			CGameObject *pObj =	pView->getCurrentZone()->createObject( pView->getCurrentObjectTemplate() );
			pView->getCurrentZone()->updateAddRemoveObj();

			if ( pObj == NULL )
				pView->alertError( L"Can not create object because missing template" );
			else
			{
				pObj->setPosition( hit );
				pObj->setVisible( true );

				// add history
				CHistoryManager::getInstance()->beginHistory();
				CHistoryManager::getInstance()->addHistoryCreateObj( pObj );
				CHistoryManager::getInstance()->endHistory();
			}
		}
		else
			pView->alertError( L"Can not create object because is too far" );
	}
	else
		pView->alertError( L"Can not create object because is too far" );
}