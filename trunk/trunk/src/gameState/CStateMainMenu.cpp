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
    
    // ui button
    m_btnCreateGame = NULL;
    m_btnJoinGame = NULL;
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
	showFxThisState();

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
        // do not found hosts
    }
    else 
    {        
        // found hosts
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
		if ( strcmp("show",param) == 0 && m_needInitState == true )
		{
            // finish show animation            
            m_btnCreateGame = new CUIButton("createGame", m_rootWidget, getStateObjFx().findObj("btnCreateGame"));
            m_btnCreateGame->setText("txtLabel",        "CREATE GAME");
            m_btnCreateGame->setText("txtDescription",  "HOST A GAME SERVER");
            
            m_btnJoinGame  = new CUIButton("jointGame", m_rootWidget, getStateObjFx().findObj("btnJoinGame"));  
            m_btnJoinGame->setText("txtLabel",        "JOIN");
            m_btnJoinGame->setText("txtDescription",  "JOIN GAME ON LAN");

			m_needInitState = false;
		}
		else if ( strcmp("active",param) == 0 )
		{
			// todo
			// finish play show anim
		}
		else if ( strcmp("close",param) == 0 )
		{
            // finish hide animation            
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
				break;
			}
		}
	}
}

bool CStateMainMenu::OnEvent(const SEvent& event)
{
    if ( event.EventType == EET_GAME_EVENT )
    {
        if ( event.GameEvent.EventID == (s32)EvtNetworking )
        {
            SEventNetworking *networkEvent = ((SEventNetworking*)event.GameEvent.EventData);

#ifdef HAS_MULTIPLAYER
			// check to joint game msg from server
            if ( networkEvent->eventID == CMultiplayerManager::AcceptJoinGame )
            {
                const char *serverIP = m_mpMgr->getDeviceIp( networkEvent->deviceID ) ;
                if ( serverIP )
                {                            
                    m_serverIP = serverIP;
                    m_menuChoice = k_btnJointGame;
                }
            }
#endif

        }
        else if ( event.GameEvent.EventID == (s32)EvtButtonPress )
        {
            SEventButtonPress *buttonEvent = ((SEventButtonPress*)event.GameEvent.EventData);

            if ( buttonEvent->data == m_btnCreateGame )
            {
                // press create game
                m_menuChoice = k_btnCreateGame;
            }
            else if ( buttonEvent->data == m_btnJoinGame )
            {
                // press joint game
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
            
            // animation state
            getStateObjFx().gotoFrame("hide", true);
        }
        
    }
    return true;
}

