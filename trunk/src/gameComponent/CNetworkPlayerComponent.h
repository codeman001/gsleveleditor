#ifndef _CNETWORKPLAYERCOMPONENT_H_
#define _CNETWORKPLAYERCOMPONENT_H_

#include "IObjectComponent.h"
#include "CPlayerComponent.h"
#include "CBasePlayerState.h"

class CNetworkPlayerComponent: 
    public CBasePlayerState,
    public IObjectComponent
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

	CPlayerComponent::EPlayerSubState		m_subStateMP;
	CPlayerComponent::EPlayerState			m_stateMP;

	CPlayerComponent::EPlayerSubState		m_upbodySubStateMP;
	CPlayerComponent::EPlayerUpBodyState	m_upbodyStateMP;
    
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
    
};

#endif