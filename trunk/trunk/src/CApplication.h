#ifndef _CAPPLICATION_H_
#define _CAPPLICATION_H_

#include "stdafx.h"
#include "IView.h"

#include "CBinaryUtils.h"
#include "CDaeUtils.h"

#include "CParticleComponent.h"
#include "CColladaMeshComponent.h"

#include "gameState/CStateInit.h"
#include "gameState/CGameStateManager.h"
#include "gameLevel/CGameLevel.h"
#include "gameDebug/CGameDebug.h"
#include "gameConfig/CGameConfig.h"
#include "gameControl/CTouchManager.h"
#include "gameControl/CGameControl.h"
#include "gameString/CGameString.h"
#include "core/TextureManager/CTextureManager.h"

class CApplication: public IEventReceiver, IView
{	
public:
	long	m_lastUpdateTime;
	int		m_fps;
	
protected:
	bool			m_resizeWin;	
	CTouchManager	m_touchMgr;
protected:
	gui::IGUIFont*	m_font;
    int             m_width;
    int             m_height;
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

	// notifyResizeWin
	// notify change size of window
	void notifyResizeWin(int w, int h);

	// notifyETouchEvent
	// touch on device
	void notifyTouchEvent(CTouchManager::ETouchEvent touchEvent, int x, int y, int id);
    
    // notifyChangeOrientation
    // change orientation
    void notifyChangeOrientation( int id );
};


#endif