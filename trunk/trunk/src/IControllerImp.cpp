#include "stdafx.h"
#include "IController.h"

#include "CSelectObjectController.h"

// CControllerManager
// 
CControllerManager::CControllerManager()
{	
	int numController = (int)IController::NumController;

	m_controller = new IController* [ numController ];
	for ( int i = 0; i < numController; i++ )
		m_controller[i] = NULL;

	m_controller[ (int)IController::NullController ]	= new IController();
	m_controller[ (int)IController::SelectObject ]		= new CSelectObjectController();
	m_controller[ (int)IController::MoveObject ]		= new IController();
	m_controller[ (int)IController::RotateObject ]		= new IController();
	m_controller[ (int)IController::ScaleObject ]		= new IController();

	// default is null controller
	m_currentController = m_controller[ (int)IController::NullController ];
}

CControllerManager::~CControllerManager()
{
	int numController = (int)IController::NumController;	
	for ( int i = 0; i < numController; i++ )
	{
		if ( m_controller[i] )
			delete m_controller[i];
	}
	delete m_controller;
}

void CControllerManager::setController( IController::IControllerType controller )
{
	// cancel old controller
	if ( 
			m_currentController && 
			m_currentController != m_controller[ (int) controller ]
		)
	{
		m_currentController->cancelController();		
	}

	// set new controller
	m_currentController = m_controller[ (int) controller ];
	if ( m_currentController )
		m_currentController->initController();	
}

void CControllerManager::onKeyUp(int keyCode)
{
	if ( m_currentController )
		m_currentController->onKeyUp( keyCode );
}

void CControllerManager::onKeyDown(int keyCode)
{
	if ( m_currentController )
		m_currentController->onKeyDown( keyCode );
}

void CControllerManager::onMouseMove(int x, int y)
{
	if ( m_currentController )
		m_currentController->onMouseMove( x, y );
}

void CControllerManager::onLMouseUp(int x, int y)
{
	if ( m_currentController )
		m_currentController->onLMouseUp( x, y );
}

void CControllerManager::onRMouseUp(int x, int y)
{
	if ( m_currentController )
		m_currentController->onRMouseUp( x, y );
}

void CControllerManager::onLMouseDown(int x, int y)
{
	if ( m_currentController )
		m_currentController->onLMouseDown( x, y );
}

void CControllerManager::onRMouseDown(int x, int y)
{
	if ( m_currentController )
		m_currentController->onRMouseDown( x, y );
}

void CControllerManager::onMouseWhell(bool scrollDown, int x, int y)
{
	if ( m_currentController )
		m_currentController->onMouseWhell( scrollDown, x, y );
}

void CControllerManager::onMenuCommand( int idCmd )
{
	if ( m_currentController )
		m_currentController->onMenuCommand( idCmd );
}