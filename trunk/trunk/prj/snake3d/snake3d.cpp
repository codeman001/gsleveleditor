// snake3d.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "snake3d.h"

#include "IView.h"

#include "CLevel.h"

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


	CLevel	*m_level;

protected:
	ITexture*		m_image;
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
		m_device	= device;
		m_driver	= device->getVideoDriver();
		m_smgr		= device->getSceneManager();

		m_lastUpdateTime =  m_device->getTimer()->getTime();
		m_fps = 0;

		m_device->setWindowCaption( L"Snake3D" );

		// load image
		m_image = m_driver->getTexture("data/irrlichtlogo.png");
		m_font	= device->getGUIEnvironment()->getFont("data/bigfont.png");
	
		m_level = new CLevel();
	}

	void destroyApplication()
	{
		delete m_level;
	}

	void mainLoop()
	{		
		long now = m_device->getTimer()->getTime();
		f32 frameDeltaTime = (f32)(now - m_lastUpdateTime) / 1000.f; // Time in seconds
		m_lastUpdateTime = now;
		
		m_level->update();

		m_driver->beginScene(true, true, video::SColor(255,113,113,133));
		
		m_smgr->drawAll();

		m_level->render();

		// draw image
		m_driver->draw2DImage
			(
				m_image, 
				core::position2d<s32>(10,40),
				core::rect<s32>(0,0, m_image->getSize().Width, m_image->getSize().Height), 0,
				video::SColor(255,255,255,255), 
				true
			);

		int fps = m_driver->getFPS();

		// draw fps string		
		core::stringw tmp(L"Snake 3D - ");
		tmp += m_driver->getName();
		tmp += L" fps: ";
		tmp += fps;
			
		m_font->draw( tmp.c_str(), core::rect<s32>(10,10,300,50), video::SColor(255,255,255,255));			
		m_fps = fps;
		
		m_driver->endScene();
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

	myApp.destroyApplication();
	device->drop();

	return (int) 1;
}