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
	
	ISceneCollisionManager *collMan = getIView()->getSceneMgr()->getSceneCollisionManager();

	m_scaleState = 0;
	bool checkHitVector = true;

	while ( i != end )
	{
		CGameObject *pObj = (*i);

		// we need check to scale x, y, z or all 
		if ( checkHitVector )
		{
			if ( m_scaleState == 0   )
			{
				// hit on ox
				if ( pObj->isHittestObjectVector( x,y, 1 ) )
					m_scaleState = 1;
				// hit on oy
				else if ( pObj->isHittestObjectVector( x,y, 2 ) )				
					m_scaleState = 2;			
				// hit on oz
				else if ( pObj->isHittestObjectVector( x,y, 3 ) )
					m_scaleState = 3;
			}

			if ( m_scaleState != 0 )
			{
				checkHitVector = false;

				// get select ray
				core::line3df selectRay = getIView()->getSelectRay();

				// check hit test
				core::vector3df intersection;
				core::triangle3df hitTriangle;

				// check select
				ISceneNode *selectedSceneNode = collMan->getSceneNodeAndCollisionPointFromRay
					(
						selectRay,
						intersection,
						hitTriangle
					);
				
				// check hit test
				if ( selectedSceneNode )
				{
					m_scaleState = 0;
				}
			}
		}

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
			pObj->updateNodeScale();
			
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
			core::vector3df scaleVector = pObj->getScale();
			
			if ( m_scaleState == 0 )
				scaleVector = scaleVector * speed;
			else if ( m_scaleState == 1 )
				scaleVector.X = scaleVector.X * speed;
			else if ( m_scaleState == 2 )
				scaleVector.Y = scaleVector.Y * speed;
			else
				scaleVector.Z = scaleVector.Z * speed;

			float len = scaleVector.getLength();

			if ( len >= 0.5f )
				pObj->setScale( scaleVector );

			i++;
		}		
	}

}