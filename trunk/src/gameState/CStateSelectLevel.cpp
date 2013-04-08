#include "stdafx.h"
#include "CStateSelectLevel.h"
#include "CStateGameLoading.h"
#include "CGameStateManager.h"
#include "gameConfig/CGameConfig.h"

CStateSelectLevel::CStateSelectLevel()
	:CGameState( CGameState::GSStateSelectLevel )
{
}

CStateSelectLevel::~CStateSelectLevel()
{
}
	
void CStateSelectLevel::onCreate()
{
	m_menuFx->setVisible( true );
	m_menuFx->setBackgroundTransparent(true);

	// show mainmenu state
	showFxThisState();

	// set title
	CMenuFxObj txtPanel = getStateObjFx().findObj("txtPanelTitle");
    txtPanel.setText("SELECT LEVEL");

	m_btnMainMenu	= new CUIButton("btnMainMenu",		getRootWidget(), getStateObjFx().findObj("btnMainMenu"));
	m_btnMainMenu->setText("txtLabel", "Main Menu");

	m_btnCreateGame = new CUIButton("btnCreateGame",	getRootWidget(), getStateObjFx().findObj("btnCreateGame"));
	m_btnCreateGame->setText("txtLabel", "Create Game");

	m_lstSelectLevel = new CUIListview("lstSelectLevel", getRootWidget(), getStateObjFx().findObj("lstSelectLevel"));
	m_lstSelectLevel->setRowHeight(30);


	CGameConfig* gameConfig = CGameConfig::getInstance();
	for ( int i = 0, n = gameConfig->getNumGameMatch(); i < n; i++)
	{
		CGameConfig::SGameMatch *match = gameConfig->getGameMatch(i);
		for ( int j = 0, n = match->levels.size(); j < n; j++ )
		{
			int itemID = m_lstSelectLevel->addItem();
			CUIListviewItem *listItem = m_lstSelectLevel->getItem(itemID);
			listItem->setUserData( &match->levels[j] );

			listItem->setText("txtLevelName", match->levels[j].name.c_str() );
			listItem->setText("txtLevelMode", IGameMatch::s_stringOfMatch[ (int)match->matchType ] );
		}
	}

	// default select
	if ( m_lstSelectLevel->getItemCount() > 0 )
		m_lstSelectLevel->setSelect(0);

	m_jumpToLoading = false;
}

void CStateSelectLevel::onDestroy()
{
	CGameState::onDestroy();
}

void CStateSelectLevel::onFsCommand( const char *command, const char *param )
{
}

void CStateSelectLevel::onRender()
{
	// render parent
    CGameStateMgr::getInstance()->getStateBefore(this)->onRender();
}

void CStateSelectLevel::onUpdate()
{
	CGameState::onUpdate();

	if ( m_jumpToLoading == true )
	{
		// hide fx flash
		setFxAllStateVisible(CGameState::StateCount, false);

		CGameStateMgr::getInstance()->popAllState();
		CGameStateMgr::getInstance()->pushState( new CStateGameLoading( IGameMatch::SoloMode ) );
	}
}

void CStateSelectLevel::onEvent(const SEvent& event)
{
	if ( event.GameEvent.EventID == (s32)EvtButtonRelease )
    {
        SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);
        
        if ( buttonEvent->data == m_btnMainMenu )
        {
            CGameStateMgr::getInstance()->popState();
        }
		else if ( buttonEvent->data == m_btnCreateGame )
		{
			for ( int i = 0; i < m_lstSelectLevel->getItemCount(); i++ )
			{
				if ( m_lstSelectLevel->getItem(i)->isSelected() )
				{
					CGameConfig::SLevelInfo *lvInfo = (CGameConfig::SLevelInfo*)m_lstSelectLevel->getItem(i)->getUserData();

					char levelPath[512] = {0};
					uiString::convertUnicodeToUTF8((unsigned short*)lvInfo->resource.c_str(), levelPath);

					CGameLevel::setLevelProperty("LevelLoad", levelPath);
					CGameLevel::setLevelProperty("IsHost","true");

					m_jumpToLoading = true;					
				}
			}
		}
    }
}