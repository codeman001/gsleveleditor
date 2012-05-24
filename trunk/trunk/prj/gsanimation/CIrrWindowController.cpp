#include "stdafx.h"
#include "CIrrWindowController.h"

#include "IView.h"

const int k_stateSelect = 0;
const int k_stateMove = 1;

CIrrWindowController::CIrrWindowController()
{
	m_controllerState = k_stateSelect;
	m_leftMouseDown = false;

	m_currentSelect = NULL;
	m_mouseDownX = 0;
	m_mouseDownY = 0;

	m_actionState = -1;
}

CIrrWindowController::~CIrrWindowController()
{
}

void CIrrWindowController::onLMouseDown(int x, int y)
{
	m_leftMouseDown = true;
	m_mouseDownX = x;
	m_mouseDownY = y;

	// reset variable	
	m_actionState = -1;

	// check select
	if ( m_currentSelect )
		m_actionState = m_currentSelect->getHitState( x,y );

	// disable free camera
	if ( m_actionState != -1 )
		getIView()->enableFreeCamera( false );
}

void CIrrWindowController::onLMouseUp(int x, int y)
{
	m_leftMouseDown = false;

	int dx = abs(x - m_mouseDownX);
	int dy = abs(y - m_mouseDownY);

	if ( dx <= 1 && dy <= 1 && m_actionState == -1 )
	{							
		m_currentSelect = NULL;
		m_controllerState = k_stateSelect;

		core::line3df selectRay = getIView()->getSelectRay();

		// check hit test
		core::vector3df intersection;
		core::triangle3df hitTriangle;

		// check select
		ISceneNode *selectedSceneNode = getIView()->getSceneMgr()->getSceneCollisionManager()->getSceneNodeAndCollisionPointFromRay
			(
				selectRay,
				intersection,
				hitTriangle
			);

		// check hit test
		if ( selectedSceneNode )
		{
			m_currentSelect = (CGameColladaSceneNode*)selectedSceneNode;
			m_controllerState = k_stateMove;		
		}

		// send event
		_onSelectNode( this );		
	}

	// reset free moving camera
	getIView()->enableFreeCamera( true );
	m_actionState = -1;
}

void CIrrWindowController::onRMouseDown(int x, int y)
{
}

void CIrrWindowController::onRMouseUp(int x, int y)
{
}

void CIrrWindowController::onMouseMove(int x, int y)
{
	getIView()->m_mouseX = x;
	getIView()->m_mouseY = y;

	// mouse drag
	if ( m_leftMouseDown && m_actionState != -1 && m_currentSelect )
	{
		// rotate oy
		if ( m_actionState == 0 )
		{
			core::vector3df rot = m_currentSelect->AnimationMatrix.getRotationDegrees();
			rot.Y += 1;
			m_currentSelect->AnimationMatrix.setRotationDegrees( rot );
		}
	}
}