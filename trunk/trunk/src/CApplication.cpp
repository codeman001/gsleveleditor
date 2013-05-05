#include "stdafx.h"
#include "CApplication.h"

#ifdef ANDROID
	#include <android/log.h>
#endif

//#define LIMIT_FPS	80.0f

IView* g_view = NULL;

IView* getIView()
{
	return g_view;
}

CApplication::CApplication()
{
	g_view = (IView*) this;
	m_resizeWin = true;
    m_width = 0;
    m_height = 0;
}

bool CApplication::OnEvent(const SEvent& event)
{
	// need copy to another array.
	// because the game will crash if registerEvent or unregister
	std::vector<eventType> eventWillProcess = m_eventReceivers;

	std::vector<eventType>::iterator i = eventWillProcess.begin(), end = eventWillProcess.end();
	while ( i != end )
	{
		bool sendEvent = false;

		// we need check
		std::vector<eventType>::iterator j = m_eventReceivers.begin(), jend = m_eventReceivers.end();
		while ( j != jend )
		{
			if ( (*i).second == (*j).second )
			{
				sendEvent = true;
				break;
			}
			j++;
		}

		// ok send event
		if ( sendEvent )
			(*i).second->OnEvent( event );

		i++;
	}

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

    m_width = viewport.getWidth();
    m_height = viewport.getHeight();
        
	// set light manager
	CLightManager *lightMgr = new CLightManager();
	m_smgr->setLightManager( lightMgr );
	lightMgr->drop();

	// get debug font
	m_font	= device->getGUIEnvironment()->getFont( getPath("data/font/smallFont.png") );

	// init instance for level
	CBinaryUtils::createGetInstance();
	CDaeUtils::createGetInstance();

	CScriptManager::createGetInstance();
	CColladaAnimationFactory::createGetInstance();
	CGameDebug::createGetInstance();
	CGameConfig::createGetInstance();

	// init instance for gamestate
	CGameStateMgr::createGetInstance();
	registerEvent( "gameStateMgr", CGameStateMgr::getInstance() );

	// init instance for flashui
	CGameUI::createGetInstance();
	registerEvent( "gameUIFlash", CGameUI::getInstance() );

    // init instance for gamecontrol
    CGameControl::createGetInstance();
    registerEvent( "gameControl", CGameControl::getInstance() );
    
    // init instance for texture manager
    CTextureManager::createGetInstance();
    
	// push gamestate
	CGameStateMgr::getInstance()->pushState( new CStateInit() );
}

void CApplication::destroyApplication()
{
	// drop event
	unRegisterEvent( CGameStateMgr::getInstance() );
	unRegisterEvent( CGameUI::getInstance() );
    unRegisterEvent( CGameControl::getInstance() );
    
	CColladaCache::freeData();
	CParticleCache::freeData();

	CObjTemplateFactory::freeData();
			
	CBinaryUtils::releaseInstance();
	CDaeUtils::releaseInstance();
	CGameStateMgr::releaseInstance();
	CScriptManager::releaseInstance();
	CColladaAnimationFactory::releaseInstance();
	CGameDebug::releaseInstance();
	CGameConfig::releaseInstance();
	CGameUI::releaseInstance();
    CGameControl::releaseInstance();
    CTextureManager::releaseInstance();
	CGameString::release();
}

void CApplication::mainLoop()
{
	m_device->getTimer()->tick();
	unsigned long now = m_device->getTimer()->getTime();
	m_timeStep = (f32)(now - m_lastUpdateTime);
	if ( m_timeStep == 0.0f )
		m_timeStep = 1.0f;
    
    const float maxTimeStep = 1000.0f/15.0f;
    if ( m_timeStep > maxTimeStep )
	{
		printf("Warning: Low FPS: %f tpf, %f fps \n", m_timeStep, 1000.0f/m_timeStep);
        m_timeStep = maxTimeStep;
	}

	m_lastUpdateTime = now;    
    
	// update camera aspect
    f32 fAspect = (f32)m_driver->getCurrentRenderTargetSize().Width / (f32)m_driver->getCurrentRenderTargetSize().Height;

	// setup orientation
	if ( m_driver->getOrientation() == video::EOO_0 )
		setCameraAspectRatio( fAspect );
	else
		setCameraAspectRatio( 1/fAspect );
	
	// update touch event
	m_touchMgr.update();

    // update gamecontrol
    if ( CGameControl::getInstance()->isEnable() )
        CGameControl::getInstance()->update();
    
	// clear debug
	CGameDebug::getInstance()->clearLines();
	CGameDebug::getInstance()->clearBoxs();
    CGameDebug::getInstance()->clear2DRects();
    
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

    // draw debug
    CGameDebug::getInstance()->draw2D();

	// clear screen
	if ( m_clearScreenTime > 0.0f )
	{
		// clear screen
		m_driver->beginScene(true, true, video::SColor(255,0,0,0));

		m_clearScreenTime = m_clearScreenTime - m_timeStep;
		if ( m_clearScreenTime < 0 )
			m_clearScreenTime = 0.0f;
	}

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
		m_device->sleep( (u32)sleepTime );
#else
	m_device->yield();
#endif				
}

// notifyResizeWin
// notify change size of window
void CApplication::notifyResizeWin(int w, int h)
{
    m_width = w;
    m_height = h;
    
	m_driver->OnResize(core::dimension2du((u32)w, (u32)h));
    core::rect<s32> winRect(0,0,w,h);
    m_device->getCursorControl()->setReferenceRect(&winRect);
	m_resizeWin = true;
}

// notifyETouchEvent
// touch on device
void CApplication::notifyTouchEvent(CTouchManager::ETouchEvent touchEvent, int x, int y, int id)
{
    E_ORIENTATION orientation = m_driver->getOrientation();
    if ( orientation == video::EOO_90 )
    {
        core::swap<int>(x,y);
        x = m_height - x;
    }
    else if ( orientation == video::EOO_270)
    {
        core::swap<int>(x,y);
        y = m_width - y;    
    }

	m_touchMgr.touchEvent( touchEvent, x, y, id );
}

void CApplication::notifyChangeOrientation( int id )
{
    if ( id == 1 )
    {
        // left
        m_driver->setOrientation(video::EOO_90);
    }
    else if ( id == 2 )
    {
        // right
        m_driver->setOrientation(video::EOO_270);
    }
    else
    {
        // portrail
        m_driver->setOrientation(video::EOO_90);
    }
}