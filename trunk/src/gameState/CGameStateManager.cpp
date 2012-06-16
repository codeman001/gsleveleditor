#include "stdafx.h"
#include "CGameStateManager.h"

CGameStateMgr::CGameStateMgr()
{
}

CGameStateMgr::~CGameStateMgr()
{
	destroyAllState();
}

void CGameStateMgr::update(float timeStep)
{		
	// update current state
	int nState = m_stackState.size();
	if ( nState > 0 )
	{
		m_stackState[0]->update(timeStep);
	}

	// delete change, pop state
	nState = m_willDeleteState.size();
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
	int nState = m_stackState.size();
	if ( nState > 0 )
	{
		m_stackState[0]->render();
	}
}

// on resume from interupt
void CGameStateMgr::onResume()
{
	int nState = m_stackState.size();
	for ( int i = 0; i < nState; i++ )
	{
		m_stackState[i]->onResume();
	}
}

// on pause from interupt
void CGameStateMgr::onPause()
{
	int nState = m_stackState.size();
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
	int nState = m_stackState.size();
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
	int nState = m_stackState.size();
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
	return true;
}