//
//  CStateIGM.cpp
//  gameplay
//
//  Created by Pham Hong Duc on 2/23/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"
#include "IView.h"
#include "CStateIGM.h"
#include "CStateMainMenu.h"
#include "CGameStateManager.h"
#include "gameEvent.h"

const int k_btnResumeGame = 0;
const int k_btnMainMenu = 1;

CStateIGM::CStateIGM()
    :CGameState(GSStateIGM)
{
    m_menuChoice = -1;
}

CStateIGM::~CStateIGM()
{
    
}

void CStateIGM::onCreate()
{
    // show ui
	m_menuFx = CGameUI::getInstance()->getFlash("uiGameHud");
	
	// show mainmenu state
	showFxThisState();
    
    m_resumeButton = new CUIButton("btnResume", m_rootWidget, getStateObjFx().findObj("btnResume"));
    m_resumeButton->setText("txtLabel", "RESUME GAME");

    m_mainMenuButton = new CUIButton("btnMainMenu", m_rootWidget, getStateObjFx().findObj("btnMainMenu"));
    m_mainMenuButton->setText("txtLabel", "MAIN MENU");    
}

void CStateIGM::onDestroy()
{
    
}

void CStateIGM::onFsCommand( const char *command, const char *param )
{
    if ( strcmp("stateStatus", command) == 0 )
	{	
		if ( strcmp("show",param) == 0 && m_needInitState == true )
		{          
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
            if ( m_menuChoice == k_btnResumeGame )            
                CGameStateMgr::getInstance()->popState();
            else if ( m_menuChoice == k_btnMainMenu )
            {
                // todo quit game and joint to mainmenu
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
	}
}

void CStateIGM::onRender()
{
    // render parent
    CGameStateMgr::getInstance()->getStateBefore(this)->onRender();
}

void CStateIGM::onUpdate()
{
    CGameState::onUpdate();

	if ( m_menuChoice != -1 )
	{
		if ( m_resumeButton )
		{
			m_rootWidget->removeChild(m_resumeButton);
			delete m_resumeButton;
			m_resumeButton = NULL;
		}

		if ( m_mainMenuButton )
		{
			m_rootWidget->removeChild(m_mainMenuButton);
			delete m_mainMenuButton;
			m_mainMenuButton = NULL;
		}
	}
}

void CStateIGM::onEvent(const SEvent& event)
{
    if ( event.EventType == EET_GAME_EVENT )
    {
		if ( event.GameEvent.EventID == (s32)EvtButtonRelease )
        {
            SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);
            
            if ( buttonEvent->data == m_resumeButton )
            {
                m_menuChoice = k_btnResumeGame;
            }
            else if ( buttonEvent->data == m_mainMenuButton )
            {
                m_menuChoice = k_btnMainMenu;
            }           
            
            // animation state
            getStateObjFx().gotoFrame("hide", true);
        }        
    }
}

