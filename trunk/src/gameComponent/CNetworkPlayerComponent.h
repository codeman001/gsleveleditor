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
	CPlayerComponent::EPlayerSubState		m_subStateMP;
	CPlayerComponent::EPlayerState			m_stateMP;

	CPlayerComponent::EPlayerSubState		m_upbodySubStateMP;
	CPlayerComponent::EPlayerUpBodyState	m_upbodyStateMP;

	SEventPlayerMove		m_playerMoveEvt;
    SEventPlayerCommand     m_playerCmdEvt;

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

};

#endif