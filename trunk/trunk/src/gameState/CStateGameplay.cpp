#include "stdafx.h"
#include "IView.h"
#include "gameEvent.h"

#include "CGameStateManager.h"
#include "CStateIGM.h"
#include "CStateGameplay.h"
#include "CStateMainMenu.h"

#include "CParticleComponent.h"
#include "core/TextureManager/CTextureManager.h"

#include "gameMatch/CGameMatchSolo.h"

CStateGameplay::CStateGameplay(IGameMatch::EGameMatchType gameMatch)
	:CGameState(CGameState::GSStateGameplay)
{
	// create level
	m_level = new CGameLevel();

	// setting game match
	m_gameMatchType = gameMatch;
	switch(m_gameMatchType)
	{
	case IGameMatch::SoloMode:
		m_gameMatch = new CGameMatchSolo();
		break;
	default:
		m_gameMatch = NULL;
	}
}

CStateGameplay::~CStateGameplay()
{	
}

void CStateGameplay::onCreate()
{
	CGameLevel::setCurrentLevel( m_level );

	// show ui
	m_menuFx = CGameUI::getInstance()->getFlash("uiGameHud");
	
	// show mainmenu state
	showFxThisState();
	
    // enable gamecontrol
    CGameControl::getInstance()->setEnable(true);
    
	// init gameMatch
	if ( m_gameMatch )
		m_gameMatch->initUI(getStateObjFx(), m_rootWidget);
    

	// init multiplayer
#ifdef HAS_MULTIPLAYER
	const char *isHost = CGameLevel::getLevelProperty("IsHost");
	if ( strcmp(isHost, "true") == 0 )
		m_mpMgr = new CMultiplayerManager(true, false);
	else
    {
        const char *connectIP = CGameLevel::getLevelProperty("ServerIP");
		short keyID = (short)( atoi(CGameLevel::getLevelProperty("KeyID")) );
		
		m_mpMgr = new CMultiplayerManager(false, false, connectIP);
		m_mpMgr->setKeyID(keyID);
        m_mpMgr->sendReadyGameMessage();
    }
#endif
    
	// clear screen
	getIView()->clearScreenTime(100.0f);

}

void CStateGameplay::onDestroy()
{
	// tell network quit
	m_mpMgr->sendPlayerQuit();
	m_mpMgr->update();

    // hide fx state
	setFxStateVisible( m_state, false );
    
    // enable gamecontrol
    CGameControl::getInstance()->setEnable(false);
    
	// delete gamematch
	if ( m_gameMatch )
		delete m_gameMatch;
	m_gameMatch = NULL;

    // destroy level
    delete m_level;	
	m_level = NULL;
	CGameLevel::setCurrentLevel( NULL );
    
    // release all main menu texture
	CColladaCache::freeData();
	CParticleCache::freeData();
    
    CTextureManager::getInstance()->removeAllTexture();
}

void CStateGameplay::onFsCommand( const char *command, const char *param )
{
}

void CStateGameplay::onUpdate()
{
	CGameState::onUpdate();

#ifdef HAS_MULTIPLAYER
	m_mpMgr->update();
    
	// if call onDestroy first (if lost connection)
	if ( m_level == NULL )
		return;

    if ( m_mpMgr->needSyncData() )
    {
        m_mpMgr->sendGameDataMessage(m_level);
        m_mpMgr->setSyncData(false);
    }
#endif

	m_level->update();

	if ( m_gameMatch )
		m_gameMatch->update();
}

void CStateGameplay::onRender()
{
	if ( m_level )
		m_level->render();
}

void CStateGameplay::onEvent(const SEvent& event)
{
	if ( event.EventType == EET_GAME_EVENT )
	{
#ifdef HAS_MULTIPLAYER
		/////////////////////////////////////////
		// NETWORKING EVENT
		/////////////////////////////////////////

		if ( event.GameEvent.EventID == (s32)EvtNetworking )
		{
			SEventNetworking *networkEvent = ((SEventNetworking*)event.GameEvent.EventData);                
	        
			// todo unpack data
			if ( networkEvent->eventID == CMultiplayerManager::GameData )
			{
				CDataPacket *gamePacket = (CDataPacket*)networkEvent->data;

				int hostKeyID = (int)gamePacket->getShort();
				m_level->unpackDataMultiplayer(gamePacket, hostKeyID);
			}
			else if ( networkEvent->eventID == CMultiplayerManager::PlayerQuit )
			{
				CDataPacket *gamePacket = (CDataPacket*)networkEvent->data;

				int hostKeyID = (int)gamePacket->getShort();
				m_level->removeDisconectedObject(hostKeyID);

				// also quit game if host disconected
				checkDisconectedFromServer(hostKeyID);
			}

		}
		else if ( event.GameEvent.EventID == (s32)EvtNetworkDisconected )
		{
			SEventNetworkingDisconected *networkDisconected = ((SEventNetworkingDisconected*)event.GameEvent.EventData);
			
			// send player quit message to all client
			if ( m_mpMgr->isServer() )
				m_mpMgr->sendPlayerQuit(networkDisconected->hostKeyID);

			// remove disconected objects
			m_level->removeDisconectedObject(networkDisconected->hostKeyID);

			// also quit game if host disconected
			checkDisconectedFromServer(networkDisconected->hostKeyID);
		}
#endif

		// delivery ui event to game match implement
		if ( m_gameMatch )
			m_gameMatch->onEvent(event);		

	}
}

void CStateGameplay::checkDisconectedFromServer(int hostkeyID)
{
	// disconected from server
	if ( m_mpMgr->isHostServer(hostkeyID) == true )
	{
		// todo quit game and jump to mainmenu
		CGameStateMgr::getInstance()->popAllState();
	    
		CStateMainMenu *mainMenu = new CStateMainMenu();
		mainMenu->getLevel()->loadLevel( getIView()->getPath( "data/level/levelMainMenu.lv" ) );			
		while ( mainMenu->getLevel()->loadStep() == false )
		{
			// todo loading level
		}                
		CGameStateMgr::getInstance()->pushState(mainMenu);
	}
}