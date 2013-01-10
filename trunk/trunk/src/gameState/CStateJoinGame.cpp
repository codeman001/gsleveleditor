#include "stdafx.h"
#include "CStateJoinGame.h"
#include "CStateGameLoading.h"
#include "gameEvent.h"
#include "IView.h"
#include "CGameStateManager.h"

CStateJoinGame::CStateJoinGame()
	:CGameState( CGameState::GSStateJoinGame )
{
	m_joinGame = false;
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
	
    CMenuFxObj txtPanel = getStateObjFx().findObj("txtPanel");
    txtPanel.setText("SEARCHING HOST...");
    
    CMenuFxObj txtHeader = getStateObjFx().findObj("txtHeader1");
    txtHeader.setText("Host name");
    
    txtHeader = getStateObjFx().findObj("txtHeader2");
    txtHeader.setText("Host IP");
    
    txtHeader = getStateObjFx().findObj("txtHeader3");
    txtHeader.setText("Level");
    
	m_lstServerList = new CUIListview("listHostName", m_rootWidget, getStateObjFx().findObj("panelList"));
    m_lstServerList->setRowHeight(30);
    
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
    	
	// add server to listview
	int numServer = listServer.size();
	for ( int i = 0; i < numServer; i++ )
    {    
		CDeviceDetails* dev = listServer[i];

		if ( m_lstServerList->getItemWithUserData(dev) == NULL )
		{
			int itemID = m_lstServerList->addItem();
			CUIListviewItem *item =	m_lstServerList->getItem(itemID);
			item->setUserData(dev);

			item->setText("txtItem1", dev->m_name.c_str() );
			item->setText("txtItem2", m_mpMgr->getDeviceIp(dev->m_id));
			item->setText("txtItem3", "");
		}
    }

	// remove disconected server on listview
	for ( int i = 0; i < m_lstServerList->getItemCount(); i++ )
	{
		void *dev = m_lstServerList->getItem(i)->getUserData();

		// check server
		bool needRemove = true;

		for (int j = 0; j < numServer; j++ )
		{
			if ( listServer[j] == dev )
			{
				needRemove = false;
				break;
			}
		}

		// need remove on listview
		if ( needRemove )
		{
			m_lstServerList->removeItem(i);
			i--;
		}
	}

	if ( m_joinGame )
	{
		CGameLevel::setLevelProperty("levelLoad","data/level/levelGameM1.lv");
		CGameLevel::setLevelProperty("isHost","false");				
		CGameLevel::setLevelProperty("serverIP", m_server.c_str());

		char keyID[512];
		sprintf(keyID,"%d", m_mpMgr->getKeyID());
		CGameLevel::setLevelProperty("keyID", keyID);

		// hide fx flash
		setFxAllStateVisible(CGameState::StateCount, false);

		CGameStateMgr::getInstance()->popAllState();
		CGameStateMgr::getInstance()->pushState( new CStateGameLoading() );

		m_joinGame = false;
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
				m_server = serverIP;				
				m_joinGame = true;				
            }
    #endif
        }
        else if ( event.GameEvent.EventID == (s32)EvtButtonRelease )
        {
            SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);
            
            if ( buttonEvent->data == m_btnMainMenu )
            {
                CGameStateMgr::getInstance()->popState();
            }
			else if ( buttonEvent->data == m_btnJoinGame )
			{
				for ( int i = 0; i < m_lstServerList->getItemCount(); i++ )
				{
					if ( m_lstServerList->getItem(i)->isSelected() )
					{
#ifdef HAS_MULTIPLAYER
						CDeviceDetails* dev = (CDeviceDetails*)m_lstServerList->getItem(i)->getUserData();
						m_mpMgr->sendJointGamePacket( dev->m_address );
						break;
#endif
					}
				}
			}
        }
    }
}