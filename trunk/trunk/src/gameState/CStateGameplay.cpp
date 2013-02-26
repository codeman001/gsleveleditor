#include "stdafx.h"
#include "IView.h"
#include "gameEvent.h"

#include "CGameStateManager.h"
#include "CStateGameplay.h"
#include "CStateIGM.h"
#include "CStateMainMenu.h"

#include "CParticleComponent.h"
#include "core/TextureManager/CTextureManager.h"

#include "gameControl/CGameControl.h"

CStateGameplay::CStateGameplay()
	:CGameState(CGameState::GSStateGameplay)
{
	m_level = new CGameLevel();   
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
    
	// create dpad control
	m_dpad			= new CUIDPad("dpad", m_rootWidget, getStateObjFx().findObj("mcDPadTouch"));
	m_btnRunFast	= new CUIHoldButton("runfast", m_rootWidget, getStateObjFx().findObj("mcBtnRunFast"));
	m_btnShoot		= new CUIHoldButton("shoot", m_rootWidget, getStateObjFx().findObj("mcBtnShoot"));
	m_btnPause		= new CUIButton("pause", m_rootWidget, getStateObjFx().findObj("mcBtnPause"));

	m_btnRunFast->setVisible(false);
	m_btnShoot->setVisible(false);

#if defined (IOS) || defined (ANDROID)
    m_dpad->setEnable(true);
	m_btnRunFast->setVisible(true);
	m_btnShoot->setVisible(true);
#endif
    

	// init multiplayer
#ifdef HAS_MULTIPLAYER
	const char *isHost = CGameLevel::getLevelProperty("isHost");
	if ( strcmp(isHost, "true") == 0 )
		m_mpMgr = new CMultiplayerManager(true, false);
	else
    {
        const char *connectIP = CGameLevel::getLevelProperty("serverIP");
		short keyID = (short)( atoi(CGameLevel::getLevelProperty("keyID")) );
		
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
		if ( event.GameEvent.EventID == (s32)EvtButtonPress )
		{
			SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);

			if ( buttonEvent->data == m_btnRunFast )
                CGameControl::getInstance()->setRunFatMode(true);
			else if ( buttonEvent->data == m_btnShoot )
                CGameControl::getInstance()->setShootMode(true, false);
		}
		else if ( event.GameEvent.EventID == (s32)EvtButtonRelease )
		{
			SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);

			if ( buttonEvent->data == m_btnRunFast )
                CGameControl::getInstance()->setRunFatMode(false);
			else if ( buttonEvent->data == m_btnShoot )
                CGameControl::getInstance()->setShootMode(false, false);
			else if ( buttonEvent->data == m_btnPause )
				CGameStateMgr::getInstance()->pushState( new CStateIGM() );
		}
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