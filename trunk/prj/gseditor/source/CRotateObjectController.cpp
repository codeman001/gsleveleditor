#include "stdafx.h"
#include "CRotateObjectController.h"
#include "IView.h"
#include "IDoc.h"

void CRotateObjectController::initController()
{
	m_isLMouseDown = false;
}

void CRotateObjectController::cancelController()
{
}

void CRotateObjectController::onLMouseDown(int x, int y)
{
	m_isLMouseDown = true;

	m_beginX = x;
	m_beginY = y;

	ArrayGameObject *pListSelect = getIView()->getDocument()->getSelectObject();
	ArrayGameObjectIter i = pListSelect->begin(), end = pListSelect->end();
	while ( i != end )
	{
		CGameObject *pObj = (*i);
		pObj->saveTransform();
		i++;
	}
}

void CRotateObjectController::onLMouseUp(int x, int y)
{
	m_isLMouseDown = false;

	// set property for object
	ArrayGameObject *pListSelect = getIView()->getDocument()->getSelectObject();
	if ( pListSelect->size() > 0 )
	{
		getIView()->setObjectProperty( pListSelect->front() );
		
		CHistoryManager *pHistory = CHistoryManager::getInstance();
		pHistory->beginHistory();

		ArrayGameObjectIter iObj = pListSelect->begin(), iEnd = pListSelect->end();
		while ( iObj != iEnd )
		{
			CGameObject *pObj = (*iObj);				

			if ( pObj->isLock() == false )
			{
				// restore transform
				core::vector3df v =	pObj->getRotation();

				pObj->loadTransform();			
				pHistory->addHistoryBeginModifyObj( pObj );

				pObj->setRotation(v);
				pObj->updateNodeRotation();
				pObj->updateNodePosition();
				
				pHistory->addHistoryEndModifyObj( pObj );
			}

			iObj++;
		}

		pHistory->endHistory();

	}
}

void CRotateObjectController::onMouseMove(int x, int y)
{
	IView *pView = getIView();

	ArrayGameObject *pListSelect = pView->getDocument()->getSelectObject();

	float speed = 1.0f;
	float delta = (x - m_beginX) * speed;
	m_beginX = x;
	m_beginY = y;

	if ( m_isLMouseDown == true )
	{
		ArrayGameObjectIter i = pListSelect->begin(), end = pListSelect->end();
		while ( i != end )
		{
			CGameObject *pObj = (*i);
			
			if ( pObj->isLock() == false )
			{
				// rotate object
				pObj->setYaw( pObj->getYaw() - delta );
			}

			i++;
		}		
	}

}
