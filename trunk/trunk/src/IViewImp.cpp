#include "stdafx.h"
#include "IView.h"
#include "CGameCamera.h"

#if defined(GSEDITOR) || defined(GSANIMATION)
#include "CGameGSCameraAnimator.h"
#endif

#if defined(GSGAMEPLAY) && defined(WIN32)
	#define VISUALLEAKDETECTOR
#endif

#ifdef VISUALLEAKDETECTOR
	#include "vld.h"
#endif

IView::IView()
{
	m_device	= NULL;
	m_driver	= NULL;
	m_smgr		= NULL;
		
#ifdef GSEDITOR
	m_document		= NULL;
	m_contextObj	= NULL;
#endif

	m_mouseX = 0;
	m_mouseY = 0;

	m_camera = NULL;

	m_cameraAspect = 1.0f;
	m_timeStep = 1.0f;

#ifdef VISUALLEAKDETECTOR
	VLDEnable();
#endif
}

IView::~IView()
{	
	m_eventReceivers.clear();

#ifdef VISUALLEAKDETECTOR
	VLDDisable();
#else

#if !defined(GSGAMEPLAY) && defined(WIN32)
	UIDEBUG_TRACE("Debug leak memory: \n");
	UIDEBUG_DUMPLEAK();
#endif

#endif
}

// getSelectRay
// get 3d ray from camera to pointer
core::line3df IView::getSelectRay()
{
	ICameraSceneNode *camera = m_smgr->getActiveCamera();

	// if no camera
	if (  camera == NULL )
		return core::line3df();

	ISceneCollisionManager *collMan = m_smgr->getSceneCollisionManager();

	// get select ray
	core::line3df selectRay = collMan->getRayFromScreenCoordinates
		(	
			core::vector2di(m_mouseX, m_mouseY),
			camera
		);

	return selectRay;
}

bool IView::getScreenCoordinatesFrom3DPosition(const core::vector3df& pos3d, int *x, int *y)
{
	if ( m_device == NULL || m_driver == NULL || m_smgr == NULL )
	{
		*x = 0;
		*y = 0;
		return false;
	}

	ICameraSceneNode*	camera = m_smgr->getActiveCamera();

	if (camera == NULL)
		return false;

	const core::rect<s32>& viewPort = m_driver->getViewPort();
	core::dimension2d<s32> dim(viewPort.getWidth(), viewPort.getHeight());
	
	float scaleW = 1.0f;
	float scaleH = 1.0f;

	dim.Width	= (int)( (dim.Width  * (1.0f/scaleW))/ 2);
	dim.Height	= (int)( (dim.Height * (1.0f/scaleH))/ 2);
	
	core::matrix4 trans = camera->getProjectionMatrix();
	trans *= camera->getViewMatrix();

	f32 transformedPos[4] = { pos3d.X, pos3d.Y, pos3d.Z, 1.0f };

	trans.multiplyWith1x4Matrix(transformedPos);

	f32 zDiv = transformedPos[3] == 0.0f ? 1.0f : core::reciprocal(transformedPos[3]);

	if (zDiv < 0)
		zDiv = -zDiv;

	*x = dim.Width  + core::round32(dim.Width  * transformedPos[0] * zDiv);
	*y = dim.Height - core::round32(dim.Height * (transformedPos[1] * zDiv));

	return transformedPos[3] >= 0;
}

// setActiveCamera
// set active camera
void IView::setActiveCamera( CGameCamera* cam )
{
	m_camera = cam;
	
    if ( m_camera )
    {
        // active camera
        m_smgr->setActiveCamera( m_camera->getCameraNode() );	

        // adjust aspect
    #ifdef GSGAMEPLAY
        m_camera->getCameraNode()->setAspectRatio( m_cameraAspect );
    #endif
    }
}

// setCameraAspectRatio
// set camera aspect
void IView::setCameraAspectRatio(float f)
{
	m_cameraAspect = f;
	if ( m_camera )
		m_camera->getCameraNode()->setAspectRatio( f );
}

#if defined(GSEDITOR) || defined(GSANIMATION)
// enableFreeCamera
// disable or enable change camera view on editor
void IView::enableFreeCamera( bool b )
{
	if ( m_device == NULL || m_driver == NULL || m_smgr == NULL )
		return;

	ICameraSceneNode*	camera = m_smgr->getActiveCamera();

	if (camera == NULL)
		return;
	
	if ( camera->getAnimators().size() > 0 )
	{
		core::list<ISceneNodeAnimator*>::ConstIterator it = camera->getAnimators().begin();
		CGameGSCameraAnimators *camAnimator = (CGameGSCameraAnimators*) (*it);

		// disable rotate on right click
		camAnimator->enableMove( b );
	}
}
#endif

// register event
// register input event
void IView::registerEvent( std::string name, IEventReceiver *pEvent )
{
	std::vector<eventType>::iterator i = m_eventReceivers.begin(), end = m_eventReceivers.end();
	while ( i != end )
	{
		if ( (*i).second == pEvent )
			return;
		i++;
	}
	
	m_eventReceivers.push_back( eventType(name, pEvent) );
}

// unRegisterEvent
// unregister input event
void IView::unRegisterEvent( IEventReceiver *pEvent )
{
	std::vector<eventType>::iterator i = m_eventReceivers.begin(), end = m_eventReceivers.end();
	while ( i != end )
	{
		if ( (*i).second == pEvent )
		{
			m_eventReceivers.erase( i );
			return;
		}
		i++;
	}
}

// getPath
// get absolute path
wchar_t g_tempPathW[1024];
char	g_tempPathA[1024];

#if defined(IOS) || defined (MACOSX)
// get iphone resource path
extern void getResourcePath(char* cBuffer, int iLength);
#endif

char* IView::getPhysicPath(	const std::string& path )
{
#ifdef GSGAMEPLAY
	#if defined(ANDROID)
		// android
		uiString::copy<char,const char>( g_tempPathA, "/mnt/sdcard/gsgameplay/" );
		uiString::cat<char,const char>( g_tempPathA, path.c_str() );
	#elif defined(IOS) || defined (MACOSX)
		// iphone os
        char rootPath[512];
        getResourcePath(rootPath, 512);
        uiString::copy<char,const char>( g_tempPathA, rootPath );
        uiString::cat<char,const char>( g_tempPathA, "/" );
        uiString::cat<char,const char>( g_tempPathA, path.c_str() );
	#else
		uiString::copy<char,const char>( g_tempPathA, path.c_str() );
	#endif
#else
	uiString::copy<char,const char>( g_tempPathA, path.c_str() );
#endif
	return g_tempPathA;
}

char* IView::getPath( const std::string& path )
{
#ifdef GSGAMEPLAY		
	uiString::copy<char,const char>( g_tempPathA, path.c_str() );
#else	
	uiApplication::getAppPath( (LPWSTR)g_tempPathW, MAX_PATH );
	uiString::copy<char, wchar_t>( g_tempPathA, g_tempPathW  );
	uiString::cat<char,char>( g_tempPathA, "\\" );
	uiString::cat<char,const char>( g_tempPathA, path.c_str() );
#endif
	return g_tempPathA;
}

wchar_t* IView::getPathW( const std::string& path )
{	
#ifdef GSGAMEPLAY
	uiString::copy<wchar_t,const char>( g_tempPathW, path.c_str() );
#else
	uiApplication::getAppPath( (LPWSTR)g_tempPathW, MAX_PATH );
	uiString::cat<wchar_t,char>( g_tempPathW, "\\" );
	uiString::cat<wchar_t,const char>( g_tempPathW, path.c_str() );
#endif
	return g_tempPathW;
}

char* IView::getPath( const std::wstring& path )
{
#ifdef GSGAMEPLAY
	uiString::copy<char,const wchar_t>( g_tempPathA, path.c_str() );
#else
	uiApplication::getAppPath( (LPWSTR)g_tempPathW, MAX_PATH );
	uiString::copy<char, const wchar_t>( g_tempPathA, g_tempPathW  );
	uiString::cat<char,char>( g_tempPathA, "\\" );
	uiString::cat<char,const wchar_t>( g_tempPathA, path.c_str() );
#endif
	return g_tempPathA;
}

wchar_t* IView::getPathW( const std::wstring& path )
{
#ifdef GSGAMEPLAY
	uiString::copy<wchar_t,const wchar_t>( g_tempPathW, path.c_str() );
#else
	uiApplication::getAppPath( (LPWSTR)g_tempPathW, MAX_PATH );
	uiString::cat<wchar_t,char>( g_tempPathW, "\\" );
	uiString::cat<wchar_t,const wchar_t>( g_tempPathW, path.c_str() );
#endif
	return g_tempPathW;
}