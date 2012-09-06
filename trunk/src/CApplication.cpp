#include "stdafx.h"
#include "CApplication.h"

#ifdef ANDROID
	#include <android/log.h>
#endif

//#define LIMIT_FPS	30.0f

IView* g_view = NULL;

IView* getIView()
{
	return g_view;
}

CApplication::CApplication()
{
	g_view = (IView*) this;
	m_resizeWin = true;
}

bool CApplication::OnEvent(const SEvent& event)
{
	std::vector<eventType>::iterator i = m_eventReceivers.begin(), end = m_eventReceivers.end();
	while ( i != end )
	{
		(*i).second->OnEvent( event );
		i++;
	}

	// camera event
	if ( m_smgr && m_smgr->getActiveCamera() )
		m_smgr->getActiveCamera()->OnEvent( event );

	if ( event.EventType == EET_LOG_TEXT_EVENT )
	{
#ifdef ANDROID
		__android_log_print(ANDROID_LOG_INFO, "libgsgameplay.so" , event.LogEvent.Text );
#else
		printf("%s\n", event.LogEvent.Text);
#endif
	}

	return true;
}


void CApplication::initApplication( IrrlichtDevice* device )
{
	// init irrlicht device
	m_device	= device;
	m_driver	= device->getVideoDriver();
	m_smgr		= device->getSceneManager();
	m_fileSystem = device->getSceneManager()->getFileSystem();
	
	m_lastUpdateTime =  m_device->getTimer()->getTime();
	m_fps = 0;

	m_device->setWindowCaption( STR_APP_TITLE );
	
	core::recti viewport = m_driver->getViewPort();
	setCameraAspectRatio( viewport.getWidth()/(float)viewport.getHeight() );

	// set light manager
	CLightManager *lightMgr = new CLightManager();
	m_smgr->setLightManager( lightMgr );
	lightMgr->drop();

	// get debug font
	m_font	= device->getGUIEnvironment()->getFont( getPath("data/font/smallFont.png") );

	// init instance for level
	CBinaryUtils::createGetInstance();
	CScriptManager::createGetInstance();
	CColladaAnimationFactory::createGetInstance();
	CGameDebug::createGetInstance();

	// init instance for gamestate
	CGameStateMgr::createGetInstance();
	registerEvent( "gameStateMgr", CGameStateMgr::getInstance() );

	// init instance for flashui
	CGameUI::createGetInstance();
	registerEvent( "gameUIFlash", CGameUI::getInstance() );

	// push gamestate
	CGameStateMgr::getInstance()->pushState( new CStateInit() );
}

void CApplication::destroyApplication()
{
	// drop event
	unRegisterEvent( CGameStateMgr::getInstance() );
	unRegisterEvent( CGameUI::getInstance() );

	CColladaCache::freeData();		
	CObjTemplateFactory::freeData();
			
	CBinaryUtils::releaseInstance();
	CGameStateMgr::releaseInstance();
	CScriptManager::releaseInstance();
	CColladaAnimationFactory::releaseInstance();
	CGameDebug::releaseInstance();
	CGameUI::releaseInstance();
}

void CApplication::mainLoop()
{
	long now = m_device->getTimer()->getTime();
	m_timeStep = (f32)(now - m_lastUpdateTime);
	if ( m_timeStep == 0.0f )
		m_timeStep = 1.0f;
	m_lastUpdateTime = now;
	
	// update camera aspect
#ifndef WIN32
	if ( m_resizeWin )
#endif
	{
		CGameCamera* activeCam = getActiveCamera();
		if ( activeCam )
		{ 
			f32 fAspect = (f32)m_driver->getCurrentRenderTargetSize().Width / (f32)m_driver->getCurrentRenderTargetSize().Height;
			activeCam->getCameraNode()->setAspectRatio( fAspect );

			m_resizeWin = false;
		}		
	}
	
	// update touch event
	m_touchMgr.update();

	// clear debug
	CGameDebug::getInstance()->clearLines();
	CGameDebug::getInstance()->clearBoxs();

	// update state
	CGameUI::getInstance()->update();
	CGameStateMgr::getInstance()->update();

	// clear screen
	m_driver->beginScene(true, true, video::SColor(255,0,0,0));
	
	// render on 3d gamestate
	CGameStateMgr::getInstance()->render();

	// draw game 2d flash ui		
	core::recti viewport = m_driver->getViewPort();
	CGameUI::getInstance()->render( 0, 0, viewport.getWidth(), viewport.getHeight(), false );	

	// draw debug fps string
	int fps = m_driver->getFPS();
	
	core::stringw tmp(L"Irrlicht engine ");
	tmp += m_device->getVersion();
	tmp += L"\n";
	tmp += m_driver->getName();

	tmp += L"\nFps: ";
	tmp += fps;
	
	tmp += L"\nPrimitive: ";
	tmp += m_driver->getPrimitiveCountDrawn();

	tmp += L", texBind: ";
	tmp += m_driver->getBindTextureCount();

	tmp += L", numTextureLoaded: ";
	tmp += m_driver->getTextureCount();

	if ( m_font )
		m_font->draw( tmp.c_str(), core::rect<s32>(10,10,300,100), video::SColor(255,0,255,0));			

	m_fps = fps;
	m_driver->endScene();

	// get current time on timer
	m_device->getTimer()->tick();

#ifdef LIMIT_FPS		
	float limitTime = (long)(1000.0f /LIMIT_FPS);
	long  frameTime = m_device->getTimer()->getTime() - m_lastUpdateTime;
	long sleepTime = (long)limitTime - frameTime;
	if ( sleepTime > 0 )
		m_device->sleep( sleepTime );
#else
	m_device->yield();
#endif				
}

// notifyResizeWin
// notify change size of window
void CApplication::notifyResizeWin(int w, int h)
{
	m_driver->OnResize(irr::core::dimension2du((u32)w, (u32)h));
	m_resizeWin = true;
}

// notifyTouchEvent
// touch on device
void CApplication::notifyTouchEvent(CTouchManager::TouchEvent touchEvent, int x, int y, int id)
{
	m_touchMgr.touchEvent( touchEvent, x, y, id );
}