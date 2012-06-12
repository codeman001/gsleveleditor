// gsgameplay.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IView.h"

#include "swfUI/CGameUI.h"
#include "swfUI/CMenuFx.h"
#include "swfUI/CMenuFxObj.h"

IView* g_view = NULL;

IView* getIView()
{
	return g_view;
}

class CApplication: public IEventReceiver, IView
{	
public:
	long	m_lastUpdateTime;
	int		m_fps;

protected:
	gui::IGUIFont*	m_font;
public:

	virtual bool OnEvent(const SEvent& event)
	{
		vector<eventType>::iterator i = m_eventReceivers.begin(), end = m_eventReceivers.end();
		while ( i != end )
		{
			(*i).second->OnEvent( event );
			i++;
		}
		return true;
	}

	// getIView
	// for multi-inherit
	IView* getIView()
	{
		return (IView*) this;
	}

	void initApplication( IrrlichtDevice* device )
	{
		// init irrlicht device
		m_device	= device;
		m_driver	= device->getVideoDriver();
		m_smgr		= device->getSceneManager();

		m_lastUpdateTime =  m_device->getTimer()->getTime();
		m_fps = 0;

		m_device->setWindowCaption( STR_APP_TITLE );
		
		// get debug font
		m_font	= device->getGUIEnvironment()->getFont("data/bigfont.png");
		
		// init flash ui
		CGameUI::createGetInstance();
		CGameUI::getInstance()->openFlash("testflash","data/flashui/test.swf");	
		
		CMenuFx *fxMenu = CGameUI::getInstance()->getFlash("testflash");
		if ( fxMenu )
		{
			CMenuFxObj* fxObj =	fxMenu->getObj("fxMenu.movieButton");
		}
		registerEvent( "gameUIFlash", CGameUI::getInstance() );
	}

	void destroyApplication()
	{
		// uninit flash ui
		unRegisterEvent( CGameUI::getInstance() );
		CGameUI::releaseInstance();
	}

	void mainLoop()
	{		
		long now = m_device->getTimer()->getTime();
		f32 frameDeltaTime = (f32)(now - m_lastUpdateTime) / 1000.f; // Time in seconds
		m_lastUpdateTime = now;
		


		m_driver->beginScene(true, true, video::SColor(255,113,113,133));
		
		// draw 3d scene
		m_smgr->drawAll();
		

		// draw game flash ui
		core::recti viewport = m_driver->getViewPort();
		CGameUI::getInstance()->update( frameDeltaTime );		
		CGameUI::getInstance()->render( 0, 0, viewport.getWidth(), viewport.getHeight(), false );	

		// draw fps string
		int fps = m_driver->getFPS();
		core::stringw tmp(L"gsgameplay - ");
		tmp += m_driver->getName();

		tmp += L"\nfps: ";
		tmp += fps;
		
		tmp += L"\nPrimitive: ";
		tmp += m_driver->getPrimitiveCountDrawn();

		m_font->draw( tmp.c_str(), core::rect<s32>(10,10,300,100), video::SColor(255,255,255,255));			

		m_fps = fps;

		m_driver->endScene();
		
		Sleep(1);
	}

};

int _tmain(int argc, _TCHAR* argv[])
{
	CApplication myApp;
	g_view = myApp.getIView();

	IrrlichtDevice* device = createDevice( irr::video::EDT_OPENGL, core::dimension2d<u32>(960, 640), 16, false, false, false, &myApp );
	myApp.initApplication( device );

	while(device->run())
	{
		myApp.mainLoop();		
	}

	myApp.destroyApplication();
	device->drop();

	return 0;
}

