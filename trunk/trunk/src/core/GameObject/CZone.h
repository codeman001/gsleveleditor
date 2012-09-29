#ifndef _CZONE_H_
#define _CZONE_H_

#include "CGameObject.h"
#include "CWayPoint.h"
#include "CGameCamera.h"
#include "CTrigger.h"
#include "CLightObject.h"

class CZone: public CGameObject
{
protected:
	ArrayGameObject						m_childs;
	ArrayGameObject						m_terrains;

	bool								m_needSortObject;

#ifdef GSGAMEPLAY
	core::map<std::wstring, CGameObject*>	m_objectByName;
#endif

public:
	CZone();

	virtual ~CZone();

	// updateObject
	// update object by frame...
	virtual void updateObject();

	// sortObjectByID
	// sort object by id in list child
	void sortObjectByID();
	
	// getNumberObjects
	// get num object
	int getNumberObjects();

	// setSortObject
	// tell the zone need sort object
	inline void setSortObject( bool b )
	{
		m_needSortObject = true;
	}

	// searchObject
	// search object by id
	CGameObject* searchObject( long objectID );

#ifdef GSGAMEPLAY
	CGameObject* searchObject( const wchar_t *objectName );

	// registerObjectName
	// register object name for search object by name
	void registerObjectName( CGameObject* obj );
#endif

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

	// removeObject
	// remove object
	void removeObject( CGameObject *pObj );

	// addChild
	// add a game object to child list
	void addChild( CGameObject *p );

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
    virtual void unpackDataMultiplayer(CDataPacket *packet);
    
	// getChilds
	// get list child
	inline ArrayGameObject* getChilds()
	{
		return &m_childs;
	}

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

};

typedef std::vector<CZone*>				ArrayZone;
typedef std::vector<CZone*>::iterator	ArrayZoneIter;

#endif