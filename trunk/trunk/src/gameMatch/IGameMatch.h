#ifndef _IGAMEMATCH_H
#define _IGAMEMATCH_H

#include "gameState/CGameState.h"
#include "gameEvent.h"

class IGameMatch
{
public:
	enum EGameMatchType
	{
		SoloMode = 0,
		TeamMode,
		TeamBombMode,
		ZombieSolo,
	};
	
	enum EMatchState
	{
		MatchBegin = 0,
		MatchPlaying,
		MatchEnd
	};

protected:
	EGameMatchType	m_matchType;
	EMatchState		m_matchState;

	CMenuFxObj		m_menufx;
	CUICanvas		*m_rootWidget;

public:
	
	IGameMatch(EGameMatchType type)
	{
		m_matchType = type;
		m_matchState = MatchBegin;
	}
	
	virtual ~IGameMatch()
	{
	}

	// initUI
	// init ui
	virtual void initUI(CMenuFxObj menufx, CUICanvas* rootWidget)
	{
		m_menufx		= menufx;
		m_rootWidget	= rootWidget;
	}

	// update
	// update match
	virtual void update() = 0;

	// onEvent
	// process event
	virtual void onEvent(const SEvent& event) = 0;
};

#endif