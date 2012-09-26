#include "stdafx.h"
#include "CStateMainMenu.h"
#include "CStateGameLoading.h"
#include "CGameStateManager.h"

#include "IView.h"

const int k_btnCreateGame = 0;
const int k_btnJointGame = 1;

CStateMainMenu::CStateMainMenu()
	:CGameState( CGameState::GSStateMainMenu )
{
	m_menuChoice = -1;
	m_level = new CGameLevel();	
}

CStateMainMenu::~CStateMainMenu()
{
	delete m_level;	
	CGameLevel::setCurrentLevel( NULL );
}

void CStateMainMenu::onCreate()
{		
	m_menuFx->setVisible( true );
	m_menuFx->setBackgroundTransparent(true);

	// show mainmenu state
	setFxStateVisible( m_state, true );

	// register current level
	CGameLevel::setCurrentLevel( m_level );

#ifdef HAS_MULTIPLAYER
	// init client mp
	m_mpMgr = new CMultiplayerManager(false, false);
#endif
}

void CStateMainMenu::onDestroy()
{	
	setFxStateVisible( m_state, false );
}

void CStateMainMenu::onUpdate()
{
#ifdef HAS_MULTIPLAYER		
	const float constInterval = 2000.f;
	static float interval = constInterval;
	interval = interval - getIView()->getTimeStep();
	if ( interval < 0 )
	{
		// send discovery packet to find server
		m_mpMgr->sendDiscoveryPacket();
		interval = constInterval;
	}
    // remove host do not response in 3s
    m_mpMgr->removeAllDeviceNotResponse( 3000 );
	m_mpMgr->update();
    
    std::vector<CDeviceDetails*>    listServer;
    m_mpMgr->getAllActiveDevice(listServer);
    
    if ( listServer.size() == 0 )
    {
        // do not found server
    }
    else 
    {
        // founded sever
    }
#endif

	m_level->update();
}

void CStateMainMenu::onRender()
{
	m_level->render();
}

void CStateMainMenu::onFsCommand( const char *command, const char *param )
{
	if ( strcmp("stateStatus", command) == 0 && strcmp("close",param) == 0 )
	{		
		switch ( m_menuChoice )
		{
		case k_btnCreateGame:
			CGameLevel::setLevelProperty("levelLoad","data/level/levelGameM1.lv");
			CGameLevel::setLevelProperty("isHost","true");

			CGameStateMgr::getInstance()->changeState( new CStateGameLoading() );
			break;
		case k_btnJointGame:
			CGameLevel::setLevelProperty("levelLoad","data/level/levelGameM1.lv");
			CGameLevel::setLevelProperty("isHost","false");

			CGameStateMgr::getInstance()->changeState( new CStateGameLoading() );
			break;
		default:
			{
				printf("unknownState\n");
			}
			break;
		}
	}
	else if ( strcmp("buttonStatus",  command) == 0 )
	{
		if ( strcmp("createGame", param) == 0 )
			m_menuChoice = k_btnCreateGame;
		else if ( strcmp("jointGame", param) == 0 )
			m_menuChoice = k_btnJointGame;
	}
}