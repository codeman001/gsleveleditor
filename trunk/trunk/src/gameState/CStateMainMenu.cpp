#include "stdafx.h"
#include "CStateMainMenu.h"

#include "CStateJoinGame.h"
#include "CStateGameLoading.h"
#include "CGameStateManager.h"

#include "gameEvent.h"
#include "IView.h"

#include "gameMatch/IGameMatch.h"

/*
txtPanelTitle
btnCreateGame
btnMainMenu
listZone
lstSelectLevel
itemListSelectLevel
 txtLevelName
 txtLevelMode
*/

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
}

void CStateMainMenu::onCreate()
{		
	m_menuFx->setVisible( true );
	m_menuFx->setBackgroundTransparent(true);

	// show mainmenu state
	showFxThisState();

	// register current level
	CGameLevel::setCurrentLevel( m_level );
}

void CStateMainMenu::onDestroy()
{	
    // hide fx state
	setFxStateVisible( m_state, false );
    
    delete m_level;	
	CGameLevel::setCurrentLevel( NULL );
}

void CStateMainMenu::onUpdate()
{
	CGameState::onUpdate();
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
            
            m_btnJoinGame  = new CUIButton("joinGame", m_rootWidget, getStateObjFx().findObj("btnJoinGame"));  
            m_btnJoinGame->setText("txtLabel",        "JOIN");
            m_btnJoinGame->setText("txtDescription",  "JOIN GAME ON LAN");

			m_btnOption = new CUIButton("option", m_rootWidget, getStateObjFx().findObj("btnOption"));
			m_btnOption->setText("txtLabel",        "OPTION");
            m_btnOption->setText("txtDescription",  "CUSTOMIZE YOUR GAME");

			m_btnAbout	= new CUIButton("about", m_rootWidget, getStateObjFx().findObj("btnAbout"));
			m_btnAbout->setText("txtLabel",        "ABOUT US");
            m_btnAbout->setText("txtDescription",  "DEVELOPED TEAM INFOMATION");

			m_needInitState = false;
			
			m_rootWidget->lockActionOnChilld(true);
		}
		else if ( strcmp("active",param) == 0 )
		{
			// todo
			// finish play show anim
			m_rootWidget->lockActionOnChilld(false);
		}
		else if ( strcmp("close",param) == 0 )
		{
            // finish hide animation            
			switch ( m_menuChoice )
			{
			case k_btnCreateGame:
				CGameLevel::setLevelProperty("levelLoad","data/level/levelGameM1.lv");
				CGameLevel::setLevelProperty("isHost","true");

				CGameStateMgr::getInstance()->changeState( new CStateGameLoading( IGameMatch::SoloMode ) );
				break;
			case k_btnJointGame:
				CGameStateMgr::getInstance()->pushState( new CStateJoinGame() );
				break;
			default:			
				break;
			}
		}
	}
}

void CStateMainMenu::onEvent(const SEvent& event)
{
    if ( event.EventType == EET_GAME_EVENT )
    {
		if ( event.GameEvent.EventID == (s32)EvtButtonRelease )
        {
            SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);
            
            if ( buttonEvent->data == m_btnCreateGame )
            {
                // press create game
                m_menuChoice = k_btnCreateGame;
                
                // animation state
                getStateObjFx().gotoFrame("hide", true);
            }
            else if ( buttonEvent->data == m_btnJoinGame )
            {
                // press joint game
				m_menuChoice = k_btnJointGame;
                
                // animation state
                getStateObjFx().gotoFrame("hide", true);
            }
            
        }
        
    }
}