#include "stdafx.h"
#include "CStateJoinGame.h"
#include "gameEvent.h"
#include "IView.h"

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

    // register event for catch networking message
    getIView()->registerEvent("stateJoinGame", this);

#ifdef HAS_MULTIPLAYER
	// init client mp
	m_mpMgr = new CMultiplayerManager(false, false);
#endif

	// add for test actionscript 2.0
	// begin test
	CMenuFxObj listPanel = getStateObjFx().findObj("panelList");
	for ( int i = 0; i < 4; i++ )
	{
		char name[62];
		sprintf(name,"item%d",i);

		gameswf::as_value param(name);	
		listPanel.invokeASCallback("addListItem", &param, 1);

		CMenuFxObj list = listPanel.findObj(name);
		list.setPosition( 0, i*40 );
	}
	// end test
}

void CStateJoinGame::onDestroy()
{
	getIView()->unRegisterEvent(this);
}

void CStateJoinGame::onFsCommand( const char *command, const char *param )
{
}
	
void CStateJoinGame::onRender()
{
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
    
    if ( listServer.size() == 0 )
    {
        // do not found hosts
    }
    else 
    {        
        // found hosts
    }
#endif
}

bool CStateJoinGame::OnEvent(const SEvent& event)
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

	return true;
}