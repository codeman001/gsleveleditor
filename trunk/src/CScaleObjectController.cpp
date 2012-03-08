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
}

void CScaleObjectController::onLMouseUp(int x, int y)
{
	m_isLMouseDown = false;
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
			pObj->setScale( scaleVector );

			i++;
		}
	}
}