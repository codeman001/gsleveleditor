#include "stdafx.h"
#include "CStateMainMenu.h"
#include "CStateGameLoading.h"
#include "CGameStateManager.h"

#include "gameEvent.h"
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
	setFxAllStateVisible( m_state, false );

	// register current level
	CGameLevel::setCurrentLevel( m_level );

#ifdef HAS_MULTIPLAYER
	// init client mp
	m_mpMgr = new CMultiplayerManager(false, false);
#endif
    
    // register event for catch networking message
    getIView()->registerEvent("stateMainMenu", this);
}

void CStateMainMenu::onDestroy()
{	
    // hide fx state
	setFxStateVisible( m_state, false );
    
    // release event
    getIView()->unRegisterEvent(this);
}

void CStateMainMenu::onUpdate()
{
	CGameState::onUpdate();

#ifdef HAS_MULTIPLAYER		
	const float constInterval = 1000.f;
	static float interval = constInterval;
	interval = interval - getIView()->getTimeStep();
	if ( interval < 0 )
	{
		// send discovery packet to find server
		m_mpMgr->sendDiscoveryPacket();
		interval = constInterval;
	}
    // remove host do not response in 3s
    m_mpMgr->removeAllDeviceNotResponse( 2000 );
	m_mpMgr->update();
    
    std::vector<CDeviceDetails*>    listServer;
    m_mpMgr->getAllActiveDevice(listServer);
    
    if ( listServer.size() == 0 )
    {
        // do not found server		
		m_txtServerName.setText("find server...");
    }
    else 
    {        
		m_txtServerName.setText( listServer[0]->m_name.c_str() );
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
	if ( strcmp("stateStatus", command) == 0 )
	{	
		if ( strcmp("state",param) == 0 )
		{
			m_txtServerName = m_menuFx->findObj("txtServername");
			m_btnJointButton = m_menuFx->findObj("btnJoinGame");
			
			m_txtServerName.setText("find server...");
		}
		else if ( strcmp("close",param) == 0 )
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
                CGameLevel::setLevelProperty("serverIP", m_serverIP.c_str());

#ifdef HAS_MULTIPLAYER
				char keyID[512];
				sprintf(keyID,"%d", m_mpMgr->getKeyID());
				CGameLevel::setLevelProperty("keyID",keyID);
#endif

				CGameStateMgr::getInstance()->changeState( new CStateGameLoading() );
				break;
			default:
				{
					printf("unknownState\n");
				}
				break;
			}
		}
	}
	else if ( strcmp("buttonStatus",  command) == 0 )
	{
		if ( strcmp("createGame", param) == 0 )
			m_menuChoice = k_btnCreateGame;
		else if ( strcmp("jointGame", param) == 0 )
		{            
#ifdef HAS_MULTIPLAYER
			std::vector<CDeviceDetails*> listServer;
			m_mpMgr->getAllActiveDevice(listServer);

			if ( listServer.size() > 0 )
            {
                // send joint game to server
                m_mpMgr->sendJointGamePacket( listServer[0]->m_address );
            }
#endif
		}
	}
}

bool CStateMainMenu::OnEvent(const SEvent& event)
{
    if ( event.EventType == EET_GAME_EVENT && event.GameEvent.EventID == (s32)EvtNetworking )
    {
        SEventNetworking *networkEvent = ((SEventNetworking*)event.GameEvent.EventData);

#ifdef HAS_MULTIPLAYER
        if ( networkEvent->eventID == CMultiplayerManager::AcceptJointGame )
        {
            const char *serverIP = m_mpMgr->getDeviceIp( networkEvent->deviceID ) ;
            if ( serverIP )
            {                            
				m_serverIP = serverIP;

                m_btnJointButton.gotoFrame("release", true);
                m_menuChoice = k_btnJointGame;
            }
        }
#endif
        
    }
    return true;
}

