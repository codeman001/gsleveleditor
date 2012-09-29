#include "stdafx.h"
#include "IView.h"
#include "gameEvent.h"
#include "CStateGameplay.h"
#include "CGameCameraFollowAnimator.h"

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

	// hide dpad touch
	CMenuFxObj dpadTouch = m_menuFx->findObj("mcDpadTouch");
	dpadTouch.setVisible(false);

    
	CMenuFxObj dPadBase = m_menuFx->findObj("mcDpadBase");
	dPadBase.setVisible(false);	

    // enable gamecontrol
    CGameControl::getInstance()->setEnable(true);
    CGameControl::getInstance()->setNameFx("mcDpadTouch", "mcDpadBase", "mcDpadMove");
	
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

    // register event for catch networking message
    getIView()->registerEvent("stateGameplay", this);
}

void CStateGameplay::onDestroy()
{
    // enable gamecontrol
    CGameControl::getInstance()->setEnable(false);
    
    getIView()->unRegisterEvent(this);
}

void CStateGameplay::onFsCommand( const char *command, const char *param )
{
}

void CStateGameplay::onUpdate()
{
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

bool CStateGameplay::OnEvent(const SEvent& event)
{
    if ( event.EventType == EET_GAME_EVENT && event.GameEvent.EventID == (s32)EvtNetworking )
    {
        SEventNetworking *networkEvent = ((SEventNetworking*)event.GameEvent.EventData);                
        
#ifdef HAS_MULTIPLAYER
        // todo unpack data
        if ( networkEvent->eventID == CMultiplayerManager::GameData )
        {
            CDataPacket *gamePacket = (CDataPacket*)networkEvent->data;

            int hostKeyID = (int)gamePacket->getShort();
            m_level->unpackDataMultiplayer(gamePacket, hostKeyID);
        }
#endif
        
    }    
    return true;
}
