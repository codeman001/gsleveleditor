#ifndef _CAPPLICATION_H_
#define _CAPPLICATION_H_

#include "stdafx.h"
#include "IView.h"

#include "CBinaryUtils.h"

#include "gameState/CStateInit.h"
#include "gameState/CGameStateManager.h"
#include "gameLevel/CGameLevel.h"
#include "gameDebug/CGameDebug.h"

class CApplication: public IEventReceiver, IView
{	
public:
	long	m_lastUpdateTime;
	int		m_fps;

protected:
	gui::IGUIFont*	m_font;
public:
	CApplication();	

	virtual bool OnEvent(const SEvent& event);	

	// getIView
	// for multi-inherit
	IView* getIView()
	{
		return (IView*) this;
	}

	// initApplication
	// init app
	void initApplication( IrrlichtDevice* device );	

	// destroyApplication
	// destroy app
	void destroyApplication();	

	// mainLoop
	// loop game
	void mainLoop();	

};


#endif