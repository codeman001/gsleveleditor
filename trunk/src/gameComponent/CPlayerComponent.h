#ifndef _CPLAYERCOMPONENT_H_
#define _CPLAYERCOMPONENT_H_

#include "IObjectComponent.h"

#include "CColladaMeshComponent.h"
#include "CGameComponent.h"

#include "CBasePlayerState.h"

class CWeaponComponent;
class CColladaMeshComponent;
class CNetworkPlayerComponent;

class CPlayerComponent: 
    public CBasePlayerState,
	public IObjectComponent,
	public IEventReceiver	
{
public:	


protected:	
	int		m_charID;
	bool	m_init;

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

	// packDataMultiplayer
    // pack data multiplayer
    virtual void packDataMultiplayer(CDataPacket *packet);
    
    // unPackDataMultiplayer
    // unpack data on multiplayer
    virtual void unpackDataMultiplayer(CDataPacket *packet);

public:
	inline void setCharacterID(int id)
	{
		m_charID = id;
	}

protected:
	void packDataMPState(CDataPacket *packet);	

	// updateState	
	void updateState();  

public:

	// getCameraFrontVector
	// return camera front vector
	core::vector3df getCameraFrontVector();

    // getCameraRay
    // get camera view ray
    core::line3df getCameraRay();
    
	// getCollisionPoint
	// check collision
	core::vector3df getCollisionPoint( core::line3df ray );

    // getAimAngle
    // get angle of hand
    core::vector2df getAimAngle( const core::vector3df aimPoint );
    
};

#endif