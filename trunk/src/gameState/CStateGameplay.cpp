#include "stdafx.h"
#include "IView.h"
#include "gameEvent.h"
#include "CStateGameplay.h"
#include "gameControl/CGameControl.h"

CStateGameplay::CStateGameplay()
	:CGameState(CGameState::GSStateGameplay)
{
	m_level = new CGameLevel();    
}

CStateGameplay::~CStateGameplay()
{
	delete m_level;	
	CGameLevel::setCurrentLevel( NULL );
}

void CStateGameplay::onCreate()
{
	CGameLevel::setCurrentLevel( m_level );

	// show ui
	m_menuFx = CGameUI::getInstance()->getFlash("uiGameHud");
	
	setFxStateVisible( m_state, true );
	setFxAllStateVisible( m_state, false );
	
    // enable gamecontrol
    CGameControl::getInstance()->setEnable(true);
    
	// create dpad control
	m_dpad			= new CUIDPad("dpad", m_rootWidget, getStateObjFx().findObj("mcDPadTouch"));
	m_btnRunFast	= new CUIButton("runfast", m_rootWidget, getStateObjFx().findObj("mcBtnRunFast"));
	m_btnShoot		= new CUIButton("shoot", m_rootWidget, getStateObjFx().findObj("mcBtnShoot"));
	m_btnPause		= new CUIButton("pause", m_rootWidget, getStateObjFx().findObj("mcBtnPause"));

	m_btnRunFast->setVisible(false);
	m_btnShoot->setVisible(false);

#if defined (IOS) || defined (ANDROID)
    dpad->setEnable(true);
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
    
    m_syncGameInterval = 1000.0f/(float)MP_GAMEFPS_SYNC;
#endif
    
}

void CStateGameplay::onDestroy()
{
    // enable gamecontrol
    CGameControl::getInstance()->setEnable(false);
}

void CStateGameplay::onFsCommand( const char *command, const char *param )
{
}

void CStateGameplay::onUpdate()
{
	CGameState::onUpdate();

#ifdef HAS_MULTIPLAYER
	m_mpMgr->update();
    
    m_syncGameInterval = m_syncGameInterval - getIView()->getTimeStep();
    if ( m_syncGameInterval < 0 )
    {
        m_mpMgr->sendGameDataMessage(m_level);
        m_syncGameInterval = 1000.0f/(float)MP_GAMEFPS_SYNC;        
    }
#endif
	m_level->update();
}

void CStateGameplay::onRender()
{
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
		}
#endif
		if ( event.GameEvent.EventID == (s32)EvtButtonPress )
		{
			SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);

			if ( buttonEvent->data == m_btnRunFast )
            {
				m_dpad->setRunFastState(true);
            }
			else if ( buttonEvent->data == m_btnShoot )
            {
				m_dpad->setShootState(true);
            }
		}
		else if ( event.GameEvent.EventID == (s32)EvtButtonRelease )
		{
			SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);

			if ( buttonEvent->data == m_btnRunFast )
            {
				m_dpad->setRunFastState(false);
            }
			else if ( buttonEvent->data == m_btnShoot )
            {
				m_dpad->setShootState(false);
            }
			else if ( buttonEvent->data == m_btnPause )
			{
			}
		}
	}
}
