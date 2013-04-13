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

	CBasePlayerState::EPlayerSubState		m_subStateMP;
	CBasePlayerState::EPlayerState			m_stateMP;

	CBasePlayerState::EPlayerSubState		m_upbodySubStateMP;
	CBasePlayerState::EPlayerUpBodyState	m_upbodyStateMP;
    
protected:
	void unpackDataMPState(CDataPacket *packet);
    
    void unpackDataMPUpBodyState(CDataPacket *packet);
    
        
protected:
    void updateState();
    
    
public:
  
};

#endif