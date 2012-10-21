#include "stdafx.h"
#include "CGameStateManager.h"

CGameStateMgr::CGameStateMgr()
{
}

CGameStateMgr::~CGameStateMgr()
{
	destroyAllState();
}

void CGameStateMgr::update()
{		
	// update current state
	int nState = (int)m_stackState.size();
	if ( nState > 0 )
	{
		m_stackState[0]->onUpdate();
	}

	// delete change, pop state
	nState = (int)m_willDeleteState.size();
	if ( nState > 0 )
	{
		for ( int i = 0; i < nState; i++ )
			delete m_willDeleteState[i];
		m_willDeleteState.clear();
	}
}

void CGameStateMgr::render()
{
	// render current state
	int nState = (int)m_stackState.size();
	if ( nState > 0 )
	{
		m_stackState[0]->onRender();
	}
}

// on resume from interupt
void CGameStateMgr::onResume()
{
	int nState = (int)m_stackState.size();
	for ( int i = 0; i < nState; i++ )
	{
		m_stackState[i]->onResume();
	}
}

// on pause from interupt
void CGameStateMgr::onPause()
{
	int nState = (int)m_stackState.size();
	for ( int i = 0; i < nState; i++ )
	{
		m_stackState[i]->onPause();
	}
}

// push a state
void CGameStateMgr::pushState( CGameState* state )
{
	if ( m_stackState.size() > 0 )
		m_stackState[0]->onDeactive();

    // clear font provider
    CGameUI::getInstance()->resetGlyphProvider();
    
	m_stackState.insert( m_stackState.begin(), state );

	// call on create
	state->onCreate();
}

// change current state
void CGameStateMgr::changeState( CGameState* state )
{
	if ( m_stackState.size() > 0 )
	{
		m_willDeleteState.push_back( m_stackState[0] );
		m_stackState[0]->onDestroy();

		m_stackState.erase( m_stackState.begin() );

        // clear font provider
        CGameUI::getInstance()->resetGlyphProvider();
        
		m_stackState.insert( m_stackState.begin(), state );
		state->onCreate();
	}
}

// pop a state
void CGameStateMgr::popState()
{
	if ( m_stackState.size() > 0 )
	{
		m_willDeleteState.push_back( m_stackState[0] );
		m_stackState[0]->onDestroy();

		m_stackState.erase( m_stackState.begin() );
		if ( m_stackState.size() > 0 )
			m_stackState[0]->onActive();

        // clear font provider
        CGameUI::getInstance()->resetGlyphProvider();        
	}
}

// get a current game state
CGameState* CGameStateMgr::getCurrentState()
{
	if ( m_stackState.size() == 0 )
		return NULL;

	return m_stackState[0];
}

// get a state before
CGameState* CGameStateMgr::getStateBefore(CGameState* state)
{
	int nState = (int)m_stackState.size();
	for ( int i = 0; i < nState; i++ )
	{
		if ( m_stackState[i] == state )
		{
			if ( i < nState - 1 )
				return m_stackState[i + 1];
			else
				return NULL;
		}
	}
	return NULL;
}

// destroyAllState
void CGameStateMgr::destroyAllState()
{
	int nState = (int)m_stackState.size();
	for ( int i = 0; i < nState; i++ )
	{
		m_stackState[i]->onDestroy();
		delete m_stackState[i];
	}
	m_stackState.clear();
}

// OnEvent
bool CGameStateMgr::OnEvent(const SEvent& irrEvent)
{
	int nState = (int)m_stackState.size();
	if ( nState > 0 )
	{
		// send event to root widget
		m_stackState[0]->getRootWidget()->onEvent( irrEvent );
        m_stackState[0]->onEvent( irrEvent );

		// send flash fx event
		if ( irrEvent.EventType == EET_FSCOMMAND_EVENT )	
			m_stackState[0]->onFsCommand( irrEvent.FSEvent.Command , irrEvent.FSEvent.Param );
	}
	return true;
}