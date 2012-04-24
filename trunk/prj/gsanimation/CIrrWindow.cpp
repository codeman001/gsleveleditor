#include "stdafx.h"
#include "CIrrWindow.h"



CIrrWindow *g_irrView = NULL;

IView* getIView()
{
	if ( g_irrView )
	{
		return g_irrView->getIView();
	}
	return NULL;
}


CIrrWindow::CIrrWindow( WCHAR* lpString, uiWindow *pParent )
	:uiWindow( lpString, 0, 0, 100, 100, pParent )
{
	showWindow(true);
	createIrrDevice();
}

CIrrWindow::~CIrrWindow()
{
	destroyScene();

	m_device->drop();
}

void CIrrWindow::createIrrDevice()
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
	
	g_irrView = this;
	initScene();
}

void CIrrWindow::initScene()
{
	IVideoDriver* driver = m_driver;
	IrrlichtDevice* device = m_device;
	ISceneManager* smgr = m_smgr;

	// create design camera
	m_designCamera = new CGameCamera();
	m_designCamera->setName( L"Design camera" );
	m_designCamera->setTarget(core::vector3df(0,0,0));
	m_designCamera->setPosition( core::vector3df(-400, 200, -400) );	
	
	// add animator
	CGameGSCameraAnimators* camAnimator = new CGameGSCameraAnimators( device->getCursorControl() );
	m_designCamera->getCameraNode()->addAnimator( camAnimator );	
	camAnimator->drop();

	// set active camera
	getIView()->setActiveCamera( m_designCamera );



	// add oxyz plane node
	CGameOxyzSceneNode *oxyPlane = new CGameOxyzSceneNode( smgr->getRootSceneNode(), smgr, 1 );	
	oxyPlane->drop();

	// add light
	ISceneNode* pNode = smgr->addEmptySceneNode();

	// default light	
	ILightSceneNode *light = smgr->addLightSceneNode( pNode, core::vector3df(0, 0, 0) );
	light->setLightType( video::ELT_DIRECTIONAL );

	// add demo mesh
	m_animObject = new CGameObject();	

	CColladaMeshComponent *colladaComponent = new CColladaMeshComponent(m_animObject);
	m_animObject->m_components.push_back( colladaComponent );

}

void CIrrWindow::destroyScene()
{
	delete m_animObject;
	delete m_designCamera;
}

void CIrrWindow::irrUpdate()
{
	// draw document
	IVideoDriver* driver = m_driver;
	ISceneManager* smgr = m_smgr;

	// tick device
	m_device->getTimer()->tick();

	// update particle
	m_animObject->updateObject();

	// draw 3d scene
	driver->beginScene( true, true, 0, getIView()->getVideoData() );
		
	// draw all scene
	smgr->drawAll();	
	
	int fps = driver->getFPS();
	core::stringw text(L"fps: ");
	text += fps;
	text += L" frame/second";

	irr::gui::IGUIFont* font = smgr->getGUIEnvironment()->getBuiltInFont();
	font->draw( text.c_str(), core::recti( 10, 10, 200, 20), video::SColor(255, 255,0,0) );

	// end scene
	driver->endScene();
}

void CIrrWindow::_OnSize(uiSizeEvent sizeEvent, int nWidth, int nHeight)
{
	if ( m_driver && nWidth > 0 && nHeight > 0 )
	{
		m_driver->OnResize( core::dimension2d<u32>(nWidth, nHeight) );
		getIView()->setCameraAspectRatio( (f32) nWidth/nHeight );		
	}
}

void CIrrWindow::_OnMouseMove	( uiMouseEvent mouseEvent, int x, int y )
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
	// CControllerManager::getInstance()->getCurrentController()->onMouseMove( x, y );
}

void CIrrWindow::_OnMouseWheel	( uiMouseEvent mouseEvent, int x, int y )
{
	// controller event
	// CControllerManager::getInstance()->getCurrentController()->onMouseWhell( mouseEvent.isWheelUp() == false, x, y );

	SEvent irrEvent;
	memset(&irrEvent, 0, sizeof(SEvent));

	irrEvent.EventType			= irr::EET_MOUSE_INPUT_EVENT;
	irrEvent.MouseInput.Event	= irr::EMIE_MOUSE_WHEEL;
	irrEvent.MouseInput.X		= (short)x;
	irrEvent.MouseInput.Y		= (short)y;
	irrEvent.MouseInput.Shift	= mouseEvent.isShiftKeyDown();
	irrEvent.MouseInput.Control = mouseEvent.isCtrlKeyDown();
	irrEvent.MouseInput.Alt		= mouseEvent.isAltKeyDown();
	irrEvent.MouseInput.Wheel	= mouseEvent.isWheelUp()? -10.0f: 10.0f;

	// post event
	m_device->postEventFromUser(irrEvent);
}

void CIrrWindow::_OnLButtonDown	( uiMouseEvent mouseEvent, int x, int y )
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
	// CControllerManager::getInstance()->getCurrentController()->onLMouseDown( x, y );

	//  set focus
	uiApplication::getRoot()->setFocus();	
}

void CIrrWindow::_OnLButtonUp	( uiMouseEvent mouseEvent, int x, int y )
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
	// CControllerManager::getInstance()->getCurrentController()->onLMouseUp( x, y );

	//  set focus
	uiApplication::getRoot()->setFocus();
}


void CIrrWindow::_OnRButtonDown	( uiMouseEvent mouseEvent, int x, int y )
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
	// CControllerManager::getInstance()->getCurrentController()->onRMouseDown( x, y );

	//  set focus
	uiApplication::getRoot()->setFocus();
}

void CIrrWindow::_OnRButtonUp	( uiMouseEvent mouseEvent, int x, int y )
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
	// CControllerManager::getInstance()->getCurrentController()->onRMouseUp( x, y );

	//  set focus
	uiApplication::getRoot()->setFocus();
}
	
void CIrrWindow::_OnMButtonDown	( uiMouseEvent mouseEvent, int x, int y )
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

void CIrrWindow::_OnMButtonUp	( uiMouseEvent mouseEvent, int x, int y )
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