#include "stdafx.h"
#include "CScaleObjectController.h"
#include "IView.h"

void CScaleObjectController::initController()
{
	m_isLMouseDown = false;
}

void CScaleObjectController::cancelController()
{

}
	
void CScaleObjectController::onLMouseDown(int x, int y)
{
	m_isLMouseDown = true;

	ArrayGameObject *pListSelect = getIView()->getDocument()->getSelectObject();
	ArrayGameObjectIter i = pListSelect->begin(), end = pListSelect->end();
	while ( i != end )
	{
		CGameObject *pObj = (*i);
		pObj->saveTransform();
		i++;
	}
}

void CScaleObjectController::onLMouseUp(int x, int y)
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

			// restore transform
			core::vector3df v =	pObj->getScale();

			pObj->loadTransform();			
			pHistory->addHistoryBeginModifyObj( pObj );

			pObj->setScale(v);
			pObj->updateNodeRotation();
			pObj->updateNodePosition();
			
			pHistory->addHistoryEndModifyObj( pObj );

			iObj++;
		}

		pHistory->endHistory();
	}
}

void CScaleObjectController::onMouseMove(int x, int y)
{
	IView *pView = getIView();

	ArrayGameObject *pListSelect = pView->getDocument()->getSelectObject();

	int delta = (x - m_beginX);

	float speed = 1.0f;

	if ( delta < 0 )
		speed = 0.98f;
	else
		speed = 1.02f;

	m_beginX = x;
	m_beginY = y;

	if ( m_isLMouseDown == true )
	{
		ArrayGameObjectIter i = pListSelect->begin(), end = pListSelect->end();
		while ( i != end )
		{
			CGameObject *pObj = (*i);

			// rotate object
			core::vector3df scaleVector = pObj->getScale() * speed;
			float len = scaleVector.getLength();

			if ( len >= 0.5f )
				pObj->setScale( scaleVector );

			i++;
		}		
	}

}