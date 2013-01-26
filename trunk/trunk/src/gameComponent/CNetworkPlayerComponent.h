#ifndef _CNETWORKPLAYERCOMPONENT_H_
#define _CNETWORKPLAYERCOMPONENT_H_

#include "IObjectComponent.h"
#include "CPlayerComponent.h"

class CNetworkPlayerComponent: public IObjectComponent
{
public:
	CNetworkPlayerComponent(CGameObject* obj);
	virtual ~CNetworkPlayerComponent();

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

	// unpackDataFromPlayerComponent
	// sync mp gameplay
	void unpackDataFromPlayerComponent( CDataPacket* packet );

protected:
    CPlayerComponent::EPlayerSubState		m_subState;
	CPlayerComponent::EPlayerState			m_state;
	CPlayerComponent::EPlayerState			m_nextState;
	CPlayerComponent::EPlayerState			m_lastState;
    
    CPlayerComponent::EPlayerSubState       m_upbodySubState;
    CPlayerComponent::EPlayerUpBodyState    m_upbodyState;
    
    bool					m_runCommand;
    bool                    m_gunOnCommand;
    
    float                   m_animCurrentTime;
    
	CPlayerComponent::EPlayerSubState		m_subStateMP;
	CPlayerComponent::EPlayerState			m_stateMP;

	CPlayerComponent::EPlayerSubState		m_upbodySubStateMP;
	CPlayerComponent::EPlayerUpBodyState	m_upbodyStateMP;

    
	SEventPlayerMove		m_playerMoveEvt;
    SEventPlayerCommand     m_playerCmdEvt;

    CPlayerComponent*       m_playerComp;
    CColladaMeshComponent*  m_collada;
    
protected:
	void unpackDataMPState(CDataPacket *packet);

	void unpackDataStateIdle(CDataPacket *packet);
	void unpackDataStateTurn(CDataPacket *packet);
	void unpackDataStateRunTurn(CDataPacket *packet);
	void unpackDataStateRun(CDataPacket *packet);
	void unpackDataStateRunFastTurn(CDataPacket *packet);
	void unpackDataStateRunFast(CDataPacket *packet);
	void unpackDataStateRunToRunFast(CDataPacket *packet);
	void unpackDataStateRunFastToRun(CDataPacket *packet);
	void unpackDataStateStandAim(CDataPacket *packet);
	void unpackDataStatePlayerRotate(CDataPacket *packet);
    
protected:
    void updateState();
    
	void updateStateIdle();
    void updateStateTurn();
	void updateStateRunTurn();
	void updateStateRun();
	void updateStateRunFastTurn();    
	void updateStateRunFast();
    void updateStateRunToRunFast();
    void updateStateRunFastToRun();
    void updateStateStandAim();   	
    void updateStatePlayerRotate();
    
public:
    // doNextState
	// change to next state
	void doNextState()
	{		
		// change next state
		m_lastState	= m_state;
		m_state		= m_nextState;
		m_subState	= CPlayerComponent::SubStateInit;
		m_nextState = CPlayerComponent::PlayerNone;
	}
    
    // setState
	// set state for player
	void setState( CPlayerComponent::EPlayerState state )
	{
		if ( m_state == state )
		{
			m_nextState = state;
			return;
		}
        
		if ( m_state == CPlayerComponent::PlayerNone )
		{
			m_state		= state;
			m_subState	= CPlayerComponent::SubStateInit;
		}
		else
		{
			m_nextState = state;
			m_subState	= CPlayerComponent::SubStateEnd;
		}
	}
    
    // stepAnimationTime	
	void stepAnimationTime();
};

#endif