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
}

void CRotateObjectController::onLMouseUp(int x, int y)
{
	m_isLMouseDown = false;
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

			// rotate object
			pObj->setYaw( pObj->getYaw() - delta );

			i++;
		}
	}

}
