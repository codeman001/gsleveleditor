#include "stdafx.h"

#include "CViewWindow.h"
#include "CMainFrame.h"
#include "CGlobalInstance.h"

IView* g_view = NULL;

//#define DEBUG_IRRWINDOW
//#define DEBUG_SHADERTEST
#define DEBUG_SHOWTERRAIN

#ifdef DEBUG_SHADERTEST
class MyShaderCallBack : public video::IShaderConstantSetCallBack
{
public:
	virtual void OnSetMaterial(const video::SMaterial& material)
	{
		
	}

	virtual void OnSetConstants(video::IMaterialRendererServices* services,	s32 userData)
	{
		video::IVideoDriver* driver = services->getVideoDriver();

		// set inverted world matrix
		// if we are using highlevel shaders (the user can select this when
		// starting the program), we must set the constants by name.
		core::matrix4 invWorld = getIView()->getDriver()->getTransform(video::ETS_WORLD);
		invWorld.makeInverse();
		services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);

		// set clip matrix
		core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		// set camera position
		core::vector3df pos = getIView()->getSceneMgr()->getActiveCamera()->getAbsolutePosition();
		services->setVertexShaderConstant("mLightPos", reinterpret_cast<f32*>(&pos), 3);

		// set light color
		video::SColorf col(0.0f,1.0f,1.0f,0.0f);
		services->setVertexShaderConstant("mLightColor",reinterpret_cast<f32*>(&col), 4);

		// set transposed world matrix
		core::matrix4 world = getIView()->getDriver()->getTransform(video::ETS_WORLD);
		world = world.getTransposed();
		services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);

		// set sampler
		services->setVertexShaderConstant("myTexture",  0);	// ActiveTextureARB	GL_TEXTURE0 + 0
		services->setVertexShaderConstant("myTexture1", 1);	// ActiveTextureARB	GL_TEXTURE0 + 1
	}
};
#endif

IView* getIView()
{
	return g_view;
}

CViewWindow::CViewWindow( WCHAR* lpString, uiWindow *p )
	:CBaseWindow( lpString, p )
{
	createIrrDevice();
	g_view = this;
}
	
CViewWindow::~CViewWindow()
{
	m_device->drop();
}

void CViewWindow::createIrrDevice()
{
	// create irr device
	SIrrlichtCreationParameters param;	

#ifdef DEBUG_IRRWINDOW
	param.WindowId = NULL;
#else
	param.WindowId = (void*)getHandle();
#endif	

	param.DriverType = video::EDT_OPENGL;	

	m_device	= createDeviceEx(param);
	m_driver	= m_device->getVideoDriver();
	m_smgr		= m_device->getSceneManager();
	
	

	// init opengl
#ifndef DEBUG_IRRWINDOW
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
#endif


#ifdef DEBUG_SHADERTEST
	video::IGPUProgrammingServices* gpu = m_driver->getGPUProgrammingServices();
	s32 newMaterialType = 0;

	if (gpu)
	{
		MyShaderCallBack* mc = new MyShaderCallBack();
		
		const char *psFileName = "media/opengl_multi.frag";
		const char *vsFileName = "media/opengl.vert";

		// create the shaders depending on if the user wanted high level
		// or low level shaders:
		// create material from high level shaders (hlsl or glsl)
		newMaterialType = gpu->addHighLevelShaderMaterialFromFiles(
				vsFileName, "vertexMain", video::EVST_VS_1_1,
				psFileName, "pixelMain", video::EPST_PS_1_1,
				mc, video::EMT_SOLID_2_LAYER);
		
		mc->drop();
	}

	// add scene
	scene::ICameraSceneNode* cam = m_smgr->addCameraSceneNode();
	cam->setTarget(core::vector3df(0,0,0));

	scene::ISceneNodeAnimator* anim = m_smgr->createFlyCircleAnimator(core::vector3df(0,15,0), 30.0f);
	cam->addAnimator(anim);
	anim->drop();

	scene::ISceneNode* cube = m_smgr->addCubeSceneNode(20);
	
	cube->setMaterialTexture(0, m_driver->getTexture("media/wall.bmp"));
	cube->setMaterialTexture(1, m_driver->getTexture("media/water.jpg"));
	cube->setMaterialFlag(video::EMF_LIGHTING, false);

	cube->setMaterialType((video::E_MATERIAL_TYPE)newMaterialType);


	m_smgr->addSkyBoxSceneNode
	(
		m_driver->getTexture("media/irrlicht2_up.jpg"),
		m_driver->getTexture("media/irrlicht2_dn.jpg"),
		m_driver->getTexture("media/irrlicht2_lf.jpg"),
		m_driver->getTexture("media/irrlicht2_rt.jpg"),
		m_driver->getTexture("media/irrlicht2_ft.jpg"),
		m_driver->getTexture("media/irrlicht2_bk.jpg")
	);
#endif

#ifdef DEBUG_SHOWTERRAIN
	// add camera
	scene::ICameraSceneNode* camera = m_smgr->addCameraSceneNodeMaya();

	camera->setPosition	(core::vector3df(2700*2,255*2,2600*2));
	camera->setTarget	(core::vector3df(2397*2,343*2,2700*2));
	camera->setFarValue(42000.0f);

	core::list<ISceneNodeAnimator*>::ConstIterator it = camera->getAnimators().begin();

	((ISceneNodeAnimatorCameraMaya*) (*it))->setZoomSpeed	( 400);
	((ISceneNodeAnimatorCameraMaya*) (*it))->setRotateSpeed	(-100);
	((ISceneNodeAnimatorCameraMaya*) (*it))->setMoveSpeed	( 400);

	// add terrain
	scene::ITerrainSceneNode* terrain = m_smgr->addTerrainSceneNode(
		"media/terrain-heightmap.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 0.f, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(40.f, 4.4f, 40.f),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		4										// smoothFactor
		);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0,m_driver->getTexture("media/terrain-texture.jpg"));
	terrain->setMaterialTexture(1,m_driver->getTexture("media/detailmap3.jpg"));	
	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 20.0f);

	// selector collision
	ITriangleSelector* selector = NULL;

	// add box
	scene::ISceneNode *pBox = m_smgr->addCubeSceneNode(40);
	pBox->setPosition( camera->getPosition() );
	pBox->setMaterialTexture( 0, m_driver->getTexture("media/stones.jpg") );
	pBox->setMaterialFlag( video::EMF_LIGHTING, false );	
	selector = m_smgr->createTriangleSelector( ((IMeshSceneNode*)pBox)->getMesh(), pBox );
	pBox->setTriangleSelector( selector );
	selector->drop();

	ISceneNode* arrow = m_smgr->addAnimatedMeshSceneNode
		(
			m_smgr->addArrowMesh("arrow", SColor(255, 255, 0, 0), SColor(255, 0, 255, 0)), 
			NULL
		);
	arrow->setMaterialFlag(video::EMF_LIGHTING, false);
	arrow->setPosition( camera->getPosition() + core::vector3df(0, 60,0) );
	arrow->setScale(core::vector3df(20,20,20));
	arrow->setRotation(core::vector3df(0,0,180));

	// add skybox
	m_driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);	
	scene::ISceneNode* skydome = m_smgr->addSkyDomeSceneNode(m_driver->getTexture("media/skydome.jpg"),16,8,0.95f,2.0f);
	m_driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
	

	// create octree
	selector = m_smgr->createTerrainTriangleSelector( terrain );
	terrain->setTriangleSelector( selector );
	selector->drop();
#endif
	
}

void CViewWindow::_OnCreate()
{
}

void CViewWindow::_OnSize(	uiSizeEvent sizeEvent, int nWidth, int nHeight)
{
#ifndef DEBUG_IRRWINDOW
	// set viewport
	if ( m_driver )
	{
		m_driver->OnResize( core::dimension2d<u32>(nWidth, nHeight) );
		if ( m_smgr->getActiveCamera() )
			m_smgr->getActiveCamera()->setAspectRatio( (f32) nWidth/nHeight );
	}
#endif
}

void CViewWindow::_OnPaint( uiGraphics *pG )
{
	requestPaintWindow();
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
}

void CViewWindow::_OnMouseWheel	( uiMouseEvent mouseEvent, int x, int y )
{	
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

#ifdef DEBUG_SHOWTERRAIN
	// check hit test
	scene::ISceneNode* highlightedSceneNode = 0;
	scene::ISceneCollisionManager* collMan = m_smgr->getSceneCollisionManager();

	core::vector2di point(x, y);	
	core::line3d<f32> ray = collMan->getRayFromScreenCoordinates( point, m_smgr->getActiveCamera() );

	//scene::ICameraSceneNode* camera = m_smgr->getActiveCamera();
	//core::line3d<f32> ray;
	//ray.start = camera->getPosition();
	//ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 9000.0f;
	
	core::vector3df		intersection;	
	core::triangle3df	hitTriangle;

	scene::ISceneNode * selectedSceneNode = collMan->getSceneNodeAndCollisionPointFromRay
		(
					ray,
					intersection,
					hitTriangle
		);

	if ( selectedSceneNode )
	{
		if ( selectedSceneNode->isDebugDataVisible() == 0 )
			selectedSceneNode->setDebugDataVisible( scene::EDS_BBOX );
		else
			selectedSceneNode->setDebugDataVisible( 0 );

		printf("Hit test on obj id\n");
	}
#endif

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
}

void CViewWindow::idleUpdateWindow()
{	
	if ( m_device )
	{
		beginDraw();
		drawDocument();
		endDraw();
	}
}

void CViewWindow::beginDraw()
{
}

void CViewWindow::drawDocument()
{	
	m_device->getTimer()->tick();

#ifdef DEBUG_IRRWINDOW	
	m_driver->beginScene(true, true, 0);
#else
	m_driver->beginScene(true, true, 0, m_videoData);
#endif

	m_smgr->drawAll();
	m_driver->endScene();

}

void CViewWindow::endDraw()
{
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
	//CMainFrame* pFrame = (CMainFrame*)CGlobalInstance::getInstance()->m_mainFrame;
	//pFrame->m_statusBar->setText( part, lpString );
}