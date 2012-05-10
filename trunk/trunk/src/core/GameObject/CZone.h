#ifndef _CZONE_H_
#define _CZONE_H_

#include "CGameObject.h"
#include "CWayPoint.h"
#include "CGameCamera.h"
#include "CTrigger.h"

class CZone: public CGameObject
{
protected:
	ArrayGameObject		m_childs;
	ArrayGameObject		m_terrains;

	bool				m_needSortObject;

protected:
	// addChild
	// add a game object to child list
	void addChild( CGameObject *p );

public:
	CZone();

	virtual ~CZone();

	// updateObject
	// update object by frame...
	virtual void updateObject();

	// sortObjectByID
	// sort object by id in list child
	void sortObjectByID();
	
	// setSortObject
	// tell the zone need sort object
	inline void setSortObject( bool b )
	{
		m_needSortObject = true;
	}

	// searchObject
	// search object by id
	CGameObject* searchObject( long objectID );

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
#endif

	// removeObject
	// remove object
	void removeObject( CGameObject *pObj );

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
#endif

};

typedef vector<CZone*>				ArrayZone;
typedef vector<CZone*>::iterator	ArrayZoneIter;

#endif