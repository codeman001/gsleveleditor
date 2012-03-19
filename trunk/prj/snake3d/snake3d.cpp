// snake3d.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "snake3d.h"

#include "IView.h"

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
public:

	virtual bool OnEvent(const SEvent& event)
	{
		return false;
	}

	// getIView
	// for multi-inherit
	IView* getIView()
	{
		return (IView*) this;
	}

	void initApplication( IrrlichtDevice* device )
	{
		m_device	= device;
		m_driver	= device->getVideoDriver();
		m_smgr		= device->getSceneManager();

		m_lastUpdateTime =  m_device->getTimer()->getTime();
		m_fps = 0;
	}

	void mainLoop()
	{		
		long now = m_device->getTimer()->getTime();
		f32 frameDeltaTime = (f32)(now - m_lastUpdateTime) / 1000.f; // Time in seconds
		m_lastUpdateTime = now;
		
		m_driver->beginScene(true, true, video::SColor(255,113,113,133));
		m_smgr->drawAll();
		m_driver->endScene();

		int fps = m_driver->getFPS();

		if (m_fps != fps)
		{
			core::stringw tmp(L"Snake 3D - Irrlicht Engine [");
			tmp += m_driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			m_device->setWindowCaption(tmp.c_str());
			m_fps = fps;
		}
	}

};

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{	
	CApplication myApp;
	g_view = myApp.getIView();

	IrrlichtDevice* device = createDevice( irr::video::EDT_OPENGL, core::dimension2d<u32>(960, 640), 16, false, false, false, &myApp );
	myApp.initApplication( device );

	while(device->run())
	{
		myApp.mainLoop();		
	}

	device->drop();

	UIDEBUG_TRACE("Debug leak memory: \n");
	UIDEBUG_DUMPLEAK();
	return (int) 1;
}