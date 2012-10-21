#include "stdafx.h"
#include "CStateJoinGame.h"
#include "gameEvent.h"
#include "IView.h"
#include "CGameStateManager.h"

CStateJoinGame::CStateJoinGame()
	:CGameState( CGameState::GSStateJoinGame )
{
}

CStateJoinGame::~CStateJoinGame()
{
}

void CStateJoinGame::onCreate()
{
	// show mainmenu state
	showFxThisState();

#ifdef HAS_MULTIPLAYER
	// init client mp
	m_mpMgr = new CMultiplayerManager(false, false);
#endif

	// add for test actionscript 2.0
	// begin test
	//CMenuFxObj listPanel = getStateObjFx().findObj("panelList");
	//for ( int i = 0; i < 4; i++ )
	//{
	//	char name[62];
	//	sprintf(name,"item%d",i);

	//	gameswf::as_value param(name);	
	//	listPanel.invokeASCallback("addListItem", &param, 1);

	//	CMenuFxObj list = listPanel.findObj(name);
	//	list.setPosition( 0, i*40 );
	//}
	
    CMenuFxObj txtPanel = getStateObjFx().findObj("txtPanel");
    txtPanel.setText("SEARCHING HOST...");
    
    CMenuFxObj txtHeader = getStateObjFx().findObj("txtHeader1");
    txtHeader.setText("Host name");
    
    txtHeader = getStateObjFx().findObj("txtHeader2");
    txtHeader.setText("Host IP");
    
    txtHeader = getStateObjFx().findObj("txtHeader3");
    txtHeader.setText("Level");
    
	CUIListview *list = new CUIListview("listHostName", m_rootWidget, getStateObjFx().findObj("panelList"));
    list->setRowHeight(30);
    
    for ( int i = 0; i < 10; i++ )
        list->addItem();
    
    m_btnMainMenu = new CUIButton("btnMainMenu", m_rootWidget, getStateObjFx().findObj("btnMainMenu"));
    m_btnMainMenu->setText("txtLabel", "Main Menu");
    
    m_btnJoinGame = new CUIButton("btnJoinGame", m_rootWidget, getStateObjFx().findObj("btnJoinGame"));
    m_btnJoinGame->setText("txtLabel", "Join Game");
}

void CStateJoinGame::onDestroy()
{
}

void CStateJoinGame::onFsCommand( const char *command, const char *param )
{
}
	
void CStateJoinGame::onRender()
{
    // render parent
    CGameStateMgr::getInstance()->getStateBefore(this)->onRender();
}

void CStateJoinGame::onUpdate()
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
    
	// get list servers founded
    std::vector<CDeviceDetails*>    listServer;
    m_mpMgr->getAllActiveDevice(listServer);
    
    if ( listServer.size() > 0 )
    {    
        
    }
#endif
}

void CStateJoinGame::onEvent(const SEvent& event)
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
                    m_serverIP = serverIP;            
            }
    #endif
        }
        else if ( event.GameEvent.EventID == (s32)EvtButtonRelease )
        {
            SEventButtonRelease *buttonEvent = ((SEventButtonRelease*)event.GameEvent.EventData);
            
            if ( buttonEvent->data == m_btnMainMenu )
            {
                CGameStateMgr::getInstance()->popState();
            }
        }
    }
}