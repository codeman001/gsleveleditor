#include "stdafx.h"

#include "CViewWindow.h"
#include "CMainFrame.h"
#include "CGlobalInstance.h"

#include "IController.h"
#include "CDocument.h"

IView* g_view = NULL;

IView* getIView()
{
	return g_view;
}

CViewWindow::CViewWindow( WCHAR* lpString, uiWindow *p )
	:CBaseWindow( lpString, p )
{
	createIrrDevice();

	// init view
	g_view = this;
	
	// init controller
	CControllerManager::createGetInstance();

	// init document
	m_document = new CDocument();
	m_document->newDocument();
}
	
CViewWindow::~CViewWindow()
{
	if ( m_document )
	{
		delete m_document;
		m_document = NULL;
	}

	// destroy controller
	CControllerManager::releaseInstance();

	m_device->drop();
}

void CViewWindow::createIrrDevice()
{
	// create irr device
	SIrrlichtCreationParameters param;
	param.WindowId = (void*)getHandle();

	param.DriverType = video::EDT_OPENGL;	

	m_device	= createDeviceEx(param);
	m_driver	= m_device->getVideoDriver();
	m_smgr		= m_device->getSceneManager();
	
	// init opengl
	HDC HDc = GetDC( getHandle() );
	PIXELFORMATDESCRIPTOR pfd={0};
	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
	int pf = GetPixelFormat(HDc);	
	DescribePixelFormat(HDc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);	
	pfd.dwFlags |= PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.cDepthBits=16;
	pf = ChoosePixelFormat(HDc, &pfd);	
	SetPixelFormat(HDc, pf, &pfd);
	
	// share video data
	m_videoData.OpenGLWin32.HWnd = NULL;
	m_videoData.OpenGLWin32.HDc = HDc;
	m_videoData.OpenGLWin32.HRc = wglCreateContext(HDc);

	// share for multithread
	wglShareLists((HGLRC)m_driver->getExposedVideoData().OpenGLWin32.HRc, (HGLRC)m_videoData.OpenGLWin32.HRc);

}

void CViewWindow::_OnSize(	uiSizeEvent sizeEvent, int nWidth, int nHeight)
{
	if ( m_driver )
	{
		m_driver->OnResize( core::dimension2d<u32>(nWidth, nHeight) );
		if ( m_smgr->getActiveCamera() )
			m_smgr->getActiveCamera()->setAspectRatio( (f32) nWidth/nHeight );
	}
}

void CViewWindow::_OnPaint( uiGraphics *pG )
{	
}

void CViewWindow::_OnMouseMove	( uiMouseEvent mouseEvent, int x, int y )
{
	SEvent irrEvent;
	memset(&irrEvent, 0, sizeof(SEvent));

	irrEvent.EventType			= irr::EET_MOUSE_INPUT_EVENT;
	irrEvent.MouseInput.Event	= irr::EMIE_MOUSE_MOVED;
	irrEvent.MouseInput.X		= (short)x;
	irrEvent.MouseInput.Y		= (short)y;
	irrEvent.MouseInput.Shift	= mouseEvent.isShiftKeyDown();
	irrEvent.MouseInput.Control = mouseEvent.isCtrlKeyDown();
	irrEvent.MouseInput.Alt		= mouseEvent.isAltKeyDown();
	irrEvent.MouseInput.ButtonStates |= irr::EMBSM_LEFT;

	// post event
	m_device->postEventFromUser(irrEvent);

	// global mouse position
	m_mouseX = x;
	m_mouseY = y;

	// controller event
	CControllerManager::getInstance()->getCurrentController()->onMouseMove( x, y );
}

void CViewWindow::_OnMouseWheel	( uiMouseEvent mouseEvent, int x, int y )
{
	// controller event
	CControllerManager::getInstance()->getCurrentController()->onMouseWhell( mouseEvent.isWheelUp() == false, x, y );
}

void CViewWindow::_OnLButtonDown	( uiMouseEvent mouseEvent, int x, int y )
{
	setCapture();

	SEvent irrEvent;
	memset(&irrEvent, 0, sizeof(SEvent));

	irrEvent.EventType			= irr::EET_MOUSE_INPUT_EVENT;
	irrEvent.MouseInput.Event	= irr::EMIE_LMOUSE_PRESSED_DOWN;
	irrEvent.MouseInput.X		= (short)x;
	irrEvent.MouseInput.Y		= (short)y;
	irrEvent.MouseInput.Shift	= mouseEvent.isShiftKeyDown();
	irrEvent.MouseInput.Control = mouseEvent.isCtrlKeyDown();
	irrEvent.MouseInput.Alt		= mouseEvent.isAltKeyDown();
	irrEvent.MouseInput.ButtonStates |= irr::EMBSM_LEFT;

	// post event
	m_device->postEventFromUser(irrEvent);

	// controller event
	CControllerManager::getInstance()->getCurrentController()->onLMouseDown( x, y );

	//  set focus
	uiApplication::getRoot()->setFocus();
}

void CViewWindow::_OnLButtonUp	( uiMouseEvent mouseEvent, int x, int y )
{
	releaseCapture();

	SEvent irrEvent;
	memset(&irrEvent, 0, sizeof(SEvent));

	irrEvent.EventType			= irr::EET_MOUSE_INPUT_EVENT;
	irrEvent.MouseInput.Event	= irr::EMIE_LMOUSE_LEFT_UP;
	irrEvent.MouseInput.X		= (short)x;
	irrEvent.MouseInput.Y		= (short)y;
	irrEvent.MouseInput.Shift	= mouseEvent.isShiftKeyDown();
	irrEvent.MouseInput.Control = mouseEvent.isCtrlKeyDown();
	irrEvent.MouseInput.Alt		= mouseEvent.isAltKeyDown();
	irrEvent.MouseInput.ButtonStates |= irr::EMBSM_LEFT;

	// post event
	m_device->postEventFromUser(irrEvent);

	// controller event
	CControllerManager::getInstance()->getCurrentController()->onLMouseUp( x, y );

	//  set focus
	uiApplication::getRoot()->setFocus();
}


void CViewWindow::_OnRButtonDown	( uiMouseEvent mouseEvent, int x, int y )
{
	setCapture();

	SEvent irrEvent;
	memset(&irrEvent, 0, sizeof(SEvent));

	irrEvent.EventType			= irr::EET_MOUSE_INPUT_EVENT;
	irrEvent.MouseInput.Event	= irr::EMIE_RMOUSE_PRESSED_DOWN;
	irrEvent.MouseInput.X		= (short)x;
	irrEvent.MouseInput.Y		= (short)y;
	irrEvent.MouseInput.Shift	= mouseEvent.isShiftKeyDown();
	irrEvent.MouseInput.Control = mouseEvent.isCtrlKeyDown();
	irrEvent.MouseInput.Alt		= mouseEvent.isAltKeyDown();
	irrEvent.MouseInput.ButtonStates |= irr::EMBSM_LEFT;

	// post event
	m_device->postEventFromUser(irrEvent);

	// controller event
	CControllerManager::getInstance()->getCurrentController()->onRMouseDown( x, y );

	//  set focus
	uiApplication::getRoot()->setFocus();
}

void CViewWindow::_OnRButtonUp	( uiMouseEvent mouseEvent, int x, int y )
{
	releaseCapture();

	SEvent irrEvent;
	memset(&irrEvent, 0, sizeof(SEvent));

	irrEvent.EventType			= irr::EET_MOUSE_INPUT_EVENT;
	irrEvent.MouseInput.Event	= irr::EMIE_RMOUSE_LEFT_UP;
	irrEvent.MouseInput.X		= (short)x;
	irrEvent.MouseInput.Y		= (short)y;
	irrEvent.MouseInput.Shift	= mouseEvent.isShiftKeyDown();
	irrEvent.MouseInput.Control = mouseEvent.isCtrlKeyDown();
	irrEvent.MouseInput.Alt		= mouseEvent.isAltKeyDown();
	irrEvent.MouseInput.ButtonStates |= irr::EMBSM_LEFT;

	// post event
	m_device->postEventFromUser(irrEvent);

	// controller event
	CControllerManager::getInstance()->getCurrentController()->onRMouseUp( x, y );

	//  set focus
	uiApplication::getRoot()->setFocus();
}
	
void CViewWindow::_OnMButtonDown	( uiMouseEvent mouseEvent, int x, int y )
{
	setCapture();

	SEvent irrEvent;
	memset(&irrEvent, 0, sizeof(SEvent));

	irrEvent.EventType			= irr::EET_MOUSE_INPUT_EVENT;
	irrEvent.MouseInput.Event	= irr::EMIE_MMOUSE_PRESSED_DOWN;
	irrEvent.MouseInput.X		= (short)x;
	irrEvent.MouseInput.Y		= (short)y;
	irrEvent.MouseInput.Shift	= mouseEvent.isShiftKeyDown();
	irrEvent.MouseInput.Control = mouseEvent.isCtrlKeyDown();
	irrEvent.MouseInput.Alt		= mouseEvent.isAltKeyDown();
	irrEvent.MouseInput.ButtonStates |= irr::EMBSM_LEFT;

	// post event
	m_device->postEventFromUser(irrEvent);

	//  set focus
	uiApplication::getRoot()->setFocus();
}

void CViewWindow::_OnMButtonUp	( uiMouseEvent mouseEvent, int x, int y )
{
	releaseCapture();

	SEvent irrEvent;
	memset(&irrEvent, 0, sizeof(SEvent));

	irrEvent.EventType			= irr::EET_MOUSE_INPUT_EVENT;
	irrEvent.MouseInput.Event	= irr::EMIE_MMOUSE_LEFT_UP;
	irrEvent.MouseInput.X		= (short)x;
	irrEvent.MouseInput.Y		= (short)y;
	irrEvent.MouseInput.Shift	= mouseEvent.isShiftKeyDown();
	irrEvent.MouseInput.Control = mouseEvent.isCtrlKeyDown();
	irrEvent.MouseInput.Alt		= mouseEvent.isAltKeyDown();
	irrEvent.MouseInput.ButtonStates |= irr::EMBSM_LEFT;

	// post event
	m_device->postEventFromUser(irrEvent);

	//  set focus
	uiApplication::getRoot()->setFocus();
}

void CViewWindow::idleUpdateWindow()
{	
	// update document
	if ( m_document && m_device )
	{
		// update document
		m_document->updateDocument();
		
		// draw document
		m_document->drawDocument();
	}
}

void CViewWindow::alertError( wchar_t *lpString )
{
	this->alert( lpString, L"Warning!", MB_OK | MB_ICONEXCLAMATION );
}

bool CViewWindow::yesNoQuestion( wchar_t *lpString )
{
	int ret = MessageBoxW( this->getHandle(), lpString, L"Warning!", MB_YESNO | MB_ICONQUESTION );

	if ( ret == IDYES )
		return true;

	return false;
}

void CViewWindow::setStatusText( int part, wchar_t *lpString )
{
	CMainFrame* pFrame = (CMainFrame*)CGlobalInstance::getInstance()->m_mainFrame;
	pFrame->getStatusBar()->setText( part, lpString );	
}

// setObjectProperty
// set object property on listview window
void CViewWindow::setObjectProperty(CGameObject *pObj)
{
	CMainFrame* pFrame = (CMainFrame*)CGlobalInstance::getInstance()->m_mainFrame;
	pFrame->getObjPropertyWnd()->setObject( pObj );
}