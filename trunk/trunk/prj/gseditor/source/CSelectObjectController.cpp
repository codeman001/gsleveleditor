#include "stdafx.h"
#include "CSelectObjectController.h"
#include "IView.h"
#include "IDoc.h"

CSelectObjectController::CSelectObjectController()
{
}

CSelectObjectController::~CSelectObjectController()
{
}

void CSelectObjectController::initController()
{
	m_isLMouseDown = false;
	m_isRMouseDown = false;
	m_selectState = false;
	
	m_hitOnObj = NULL;
}

void CSelectObjectController::cancelController()
{
}

void CSelectObjectController::onKeyUp(int keyCode)
{
}

void CSelectObjectController::onLMouseDown(int x, int y)
{
	m_isLMouseDown = true;

	m_beginX = x;
	m_beginY = y;

	IDoc *pDoc = getIView()->getDocument();
	ISceneCollisionManager *collMan = getIView()->getSceneMgr()->getSceneCollisionManager();

	// check control hold
	bool isControlHold = GetAsyncKeyState( VK_CONTROL ) < 0;

	// clear select
	ArrayGameObject *currentSelect = pDoc->getSelectObject();	
	m_hitOnObj = NULL;

	// if the object is selected
	if ( currentSelect->size() >  0 && isControlHold == false )
	{
		m_selectState = true;
		
		// check all select object
		ArrayGameObjectIter it = currentSelect->begin(), end = currentSelect->end();
		
		// set move state
		m_moveState = 0;

		while ( it != end )
		{			
			CGameObject *pObj = (*it);

			// hit on ox
			if ( pObj->isHittestObjectVector( x,y, 1 ) )
				m_moveState = 1;
			// hit on oy
			else if ( pObj->isHittestObjectVector( x,y, 2 ) )				
				m_moveState = 2;			
			// hit on oz
			else if ( pObj->isHittestObjectVector( x,y, 3 ) )
				m_moveState = 3;			
		
			if ( m_moveState != 0 )
			{
				m_selectState = false;
				m_hitOnObj = pObj;
				m_hitPosition = pObj->getPosition();

				// ray
				core::line3df	ray = getIView()->getSelectRay();
				core::vector3df v	= m_hitOnObj->getUp();
				
				if ( m_moveState == 2 )
					v = m_hitOnObj->getRight();

				core::plane3df	plane( pObj->getPosition(), v );
				plane.getIntersectionWithLine( ray.start, ray.getVector(), m_hitPosition );
				break;
			}
		
			it++;
		}


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
			CGameObject *pObj =	pDoc->searchObject( selectedSceneNode->getID() );
			if ( pObj && pObj->getObjectState() != CGameObject::Normal )
			{
				m_selectState = false;

				m_hitOnObj = pObj;
				m_hitPosition = intersection;

				// move xyz
				m_moveState = 0;
			}			
		}
	
	}
	else
	{
		// no object is selected
		m_selectState = true;
	}

	// save all transform before move object
	if ( m_selectState == false )
	{
		ArrayGameObjectIter iObj = currentSelect->begin(), iEnd = currentSelect->end();
		while ( iObj != iEnd )
		{
			// save current transform
			(*iObj)->saveTransform();
			iObj++;
		}
	}

}

void CSelectObjectController::onLMouseUp(int x, int y)
{
	m_isLMouseDown = false;

	IDoc *pDoc = getIView()->getDocument();

	if ( m_selectState == true )
	{
		int beginX = m_beginX;
		int beginY = m_beginY;
		int endX = x;
		int endY = y;

		if ( beginX > endX )
			core::swap<int>( beginX, endX );
		if ( beginY > endY )
			core::swap<int>( beginY, endY );

		core::recti	selectRect( beginX, beginY, endX, endY );		
		
		// clear current select if not hold control
		bool isControlHold = GetAsyncKeyState( VK_CONTROL ) < 0;
		
		// clear the select if not hold control
		if ( isControlHold == false )
			pDoc->clearSelect();
		
		// detect select
		if ( selectRect.getWidth() > 5 && selectRect.getHeight() > 5 )
			pDoc->selectObject( beginX, beginY, selectRect.getWidth(), selectRect.getHeight(), isControlHold );
		else
			pDoc->selectObject( x, y, isControlHold );
		
		// set state for select object
		setStateForSelectObject();
	}

	// set property for object
	getIView()->setObjectProperty( pDoc->getFirstObjSelect() );

}

void CSelectObjectController::onRMouseDown(int x, int y)
{
	m_isRMouseDown = true;
}

void CSelectObjectController::onRMouseUp(int x, int y)
{
	m_isRMouseDown = false;
}

void CSelectObjectController::onMouseWhell(bool scrollDown, int x, int y)
{
}

void CSelectObjectController::onMouseMove(int x, int y)
{
	IView *pView = getIView();

	if ( m_selectState == false && m_isLMouseDown == true )
	{		
		ArrayGameObject *currentSelect = getIView()->getDocument()->getSelectObject();

		// move object
		core::line3df	ray = pView->getSelectRay();

		core::vector3df planeVector = m_hitOnObj->getUp();
		if ( m_moveState == 2 )
			planeVector = m_hitOnObj->getRight();		
		core::plane3df	plane( m_hitPosition, planeVector);
		

		// get position
		core::vector3df hit;		
		bool b = plane.getIntersectionWithLine( ray.start, ray.getVector(), hit );
		
		if ( b )
		{
			// restore transform
			m_hitOnObj->loadTransform();

			// calc offset
			core::vector3df offset = hit - m_hitPosition;

			ArrayGameObjectIter iObj = currentSelect->begin(), iEnd = currentSelect->end();
			while ( iObj != iEnd )
			{
				CGameObject *pObj = (*iObj);				

				// restore transform
				pObj->loadTransform();
				pObj->setPosition( pObj->getPosition() + offset );

				pObj->updateNodeRotation();
				pObj->updateNodePosition();

				iObj++;
			}
		}

	}
}

void CSelectObjectController::render()
{
	IVideoDriver *driver = getIView()->getDriver();
	
	// draw select rect
	if ( m_isLMouseDown && m_selectState == true )
	{
		int beginX = m_beginX;
		int beginY = m_beginY;
		int endX = getIView()->m_mouseX;
		int endY = getIView()->m_mouseY;

		if ( beginX > endX )
			core::swap<int>( beginX, endX );
		if ( beginY > endY )
			core::swap<int>( beginY, endY );

		core::recti	selectRect( beginX, beginY, endX, endY );
		
		// draw select
		driver->draw2DRectangle( SColor(80, 0,255,0), selectRect );
		driver->draw2DRectangleOutline( selectRect, SColor(200, 0,255,0) );
	}

}

// setStateForSelectObject
// set state when mouse up
void CSelectObjectController::setStateForSelectObject()
{
	// update state for select object
	getIView()->getDocument()->setStateForSelectObject( CGameObject::Move );
}