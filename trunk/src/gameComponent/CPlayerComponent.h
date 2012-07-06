#ifndef _CPLAYERCOMPONENT_H_
#define _CPLAYERCOMPONENT_H_

#include "IObjectComponent.h"
#include "CColladaMeshComponent.h"
#include "CGameComponent.h"


class CPlayerComponent: 
	public IObjectComponent,
	public IEventReceiver
{
public:	
	enum EPlayerState
	{
		PlayerNone = 0,
		PlayerIdle,		
		PlayerWalk,
		PlayerRun,
		NumStateCount
	};

	enum EPlayerSubState
	{
		SubStateInit = 0,
		SubStateActive,
		SubStateEnd,
	};

	enum EPlayerStateRun
	{
		Run = 0,
		RunBack,
		RunLeft,
		RunRight
	};

protected:
	EPlayerSubState			m_subState;
	EPlayerState			m_state;
	EPlayerState			m_nextState;

	EPlayerStateRun			m_runState;
	EPlayerStateRun			m_lastRunState;

	float					m_runSpeed;
	float					m_runBackSpeed;
	float					m_runLeftSpeed;
	float					m_runRightSpeed;

	CColladaAnimation*		m_animationPackage;
	CColladaMeshComponent*	m_collada;

public:
	CPlayerComponent(CGameObject* obj);
	virtual ~CPlayerComponent();

	// init
	// run when init object
	virtual void initComponent();

	// update
	// run when update per frame
	virtual void updateComponent();

	// saveData
	// save data to serializable
	virtual void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	virtual void loadData( CSerializable* pObj );

	// OnEvent
	// cache event
	virtual bool OnEvent(const SEvent& irrEvent);
public:

	// setRunState
	// set run back, left, right...
	void setRunState( EPlayerStateRun state )
	{
		m_runState = state;
	}

	// setState
	// set state for player
	void setState( EPlayerState state )
	{
		if ( m_state == state )
			return;

		if ( m_state == PlayerNone )
		{
			m_state		= state;
			m_subState	= SubStateInit;
		}
		else
		{
			m_nextState = state;
			m_subState	= SubStateEnd;
		}
	}

protected:
	
	// updateState	
	void updateState();

	void updateStateIdle();
	void updateStateRun();

	// doNextState
	// change to next state
	void doNextState()
	{		
		// change next state
		m_state = m_nextState;
		m_subState = SubStateInit;				
		m_nextState = CPlayerComponent::PlayerNone;
	}
};

#endif