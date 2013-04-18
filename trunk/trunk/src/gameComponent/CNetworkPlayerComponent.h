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
	void unpackDataMPState(CDataPacket *packet);    
        
protected:
    void updateState();
    
    
public:
  
};

#endif