#ifndef _CZONE_H_
#define _CZONE_H_

#include "CContainerObject.h"
#include "CGameObject.h"
#include "CWayPoint.h"
#include "CGameCamera.h"
#include "CTrigger.h"
#include "CLightObject.h"

class CZone: public CContainerObject
{
protected:
	ArrayGameObject						m_terrains;
	ArrayGameObject						m_triggerObjects;

#if defined(GSGAMEPLAY)
	CContainerObject					*m_particleMgr;
	CGameObject							*m_shadowMgr;
#endif

public:
	CZone();
	virtual ~CZone();

	// updateObject
	// update object by frame...
	virtual void updateObject();

	// createObject
	// create a template object
	CGameObject* createObject( wchar_t* objTemplate );

	// createCamera
	// create a cameraObject
	CGameCamera* createCamera();

#if defined(GSEDITOR) || defined(GSGAMEPLAY)
	// createWaypoint
	// create a waypoint object
	CWayPoint* createWaypoint();

	// createTrigger
	// create a trigger object
	CTrigger* createTrigger();

	// createLight
	// create a light object
	CLightObject* createLight();
#endif

#ifdef GSEDITOR
	// setUIVisible
	// set visible on editor
	virtual void setUIVisible( bool b );
#endif

public:

	// saveData
	// save data
	virtual void saveData( CSerializable *pObj );

	// loadData
	// load data
	virtual void loadData( CSerializable *pObj );

	// getData
	// get data from zone
	virtual void getData( CSerializable* pObj );

	// updateData
	// update data
	virtual void updateData( CSerializable* pObj );

    // packDataMultiplayer
    // pack data multiplayer
    virtual void packDataMultiplayer(CDataPacket *packet);
    
    // unPackDataMultiplayer
    // unpack data on multiplayer
    virtual void unpackDataMultiplayer(CDataPacket *packet, int hostKeyId);

#if defined(GSEDITOR) || defined(GSGAMEPLAY)
	// registerTerrainObj
	// add obj to terrain list
	void registerTerrainObj( CGameObject *pObj );

	// unRegisterTerrainObj
	// remove obj from list terrain
	void unRegisterTerrainObj( CGameObject *pObj );

	// getTerrainCollision
	// check hit with a ray with a terrain
	bool getTerrainCollision( core::line3df & ray, f32 &outBestDistanceSquared, core::vector3df &outBestCollisionPoint, core::triangle3df &outBestTriangle);

	// getHeigthFromTerraint
	// get height of terrain at position
	bool getHeigthFromTerrain( core::vector3df &position, float *h, core::triangle3df *outTri = NULL );

	// getTerrainList
	// get all terrain object
	inline ArrayGameObject& getTerrainList()
	{
		return m_terrains;
	}
#endif

#ifdef GSGAMEPLAY
	// getParticleManager
	// return ps container
	inline CContainerObject* getParticleManager()
	{
		return m_particleMgr;
	}

	// getShadowManager
	// return shadow manager
	inline CGameObject* getStaticShadowManager()
	{
		return m_shadowMgr;
	}
#endif

};

typedef std::vector<CZone*>				ArrayZone;
typedef std::vector<CZone*>::iterator	ArrayZoneIter;

#endif