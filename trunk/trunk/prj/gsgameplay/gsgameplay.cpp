// gsgameplay.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
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

protected:
	
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
		m_device	= device;
		m_driver	= device->getVideoDriver();
		m_smgr		= device->getSceneManager();

		m_lastUpdateTime =  m_device->getTimer()->getTime();
		m_fps = 0;

		m_device->setWindowCaption( STR_APP_TITLE );
	}

	void destroyApplication()
	{
	}

	void mainLoop()
	{		
		long now = m_device->getTimer()->getTime();
		f32 frameDeltaTime = (f32)(now - m_lastUpdateTime) / 1000.f; // Time in seconds
		m_lastUpdateTime = now;
		


		m_driver->beginScene(true, true, video::SColor(255,113,113,133));
		
		// draw all scene
		m_smgr->drawAll();
		

		int fps = m_driver->getFPS();

		// draw fps string		
		core::stringw tmp(L"Snake 3D - ");
		tmp += m_driver->getName();
		tmp += L" fps: ";
		tmp += fps;
			
		//m_font->draw( tmp.c_str(), core::rect<s32>(10,10,300,50), video::SColor(255,255,255,255));			

		m_fps = fps;
		
		m_driver->endScene();
	}

};

int _tmain(int argc, _TCHAR* argv[])
{
	CApplication myApp;
	g_view = myApp.getIView();

	IrrlichtDevice* device = createDevice( irr::video::EDT_OGLES1, core::dimension2d<u32>(960, 640), 16, false, false, false, &myApp );
	myApp.initApplication( device );

	while(device->run())
	{
		myApp.mainLoop();		
	}

	myApp.destroyApplication();
	device->drop();

	return 0;
}

