#ifndef _CGAMEL_LEVEL_
#define _CGAMEL_LEVEL_

#include "CGameObject.h"
#include "CGameCamera.h"
#include "CWayPoint.h"
#include "CTrigger.h"
#include "CZone.h"

#include "CComponentFactory.h"

#include "CColladaMeshComponent.h"
#include "gameComponent/CGameComponent.h"

#include "CObjTemplateFactory.h"

#include "script/CScriptManager.h"
using namespace NSScriptManager;

class CGameLevel
{
protected:
	ArrayZone		m_zones;

	CGameCamera	*	m_activeCamera;
	CZone*			m_activeZone;

	// for load level	
	char*			m_loadLevelBuffer;
	char*			m_loadPos;
	CZone*			m_loadZone;

	std::string					m_levelFile;
	std::vector<std::string>	m_listScriptFile;
	int				m_numObjects;
	int				m_numObjectsLoaded;
    
    std::map<SNetworkObjID, long>   m_mapNetworkObjID;    
protected:
	// addScriptFile
	// add file script to compile
	void addScriptFile(const std::string& path);

	// compileGameScript
	// compile lua script
	void compileGameScript();
		
public:
	CGameLevel();
	virtual ~CGameLevel();

	// getCurrentLevel
	// get current level
	static CGameLevel* getCurrentLevel();

	// setLevelProperty
	// set level tag property
	static void setLevelProperty(const char *lpProperty, const char *lpValue);

	// getLevelProperty
	// get proprety tag
	static const char* getLevelProperty(const char *lpProperty);

	// setCurrentLevel
	// set current level
	static void setCurrentLevel(CGameLevel* lv);

	// createZone
	// create a zone
	virtual CGameObject* createZone();

	// removeZone
	// remove zone
	virtual void removeZone(CGameObject* zone);

	// getZoneCount
	// return number of zone
	int getZoneCount()
	{
		return (int)m_zones.size();
	}

	// getZone
	// get zone
	CZone* getZone(int i)
	{
		return m_zones[i];
	}

	// removeAllZone
	// remove all zone on level
	virtual void releaseLevel();
	
	// searchObjByID
	// search object with id
	virtual CGameObject* searchObject( long id );

	// searchObject
	// search object with name
	virtual CGameObject* searchObject( const char* name );

	// loadLevel
	// load all object from file .lv
	void loadLevel( const char *lpLevel );

	// loadStep
	// load level step
	bool loadStep(int nStep = 1);
	
	// getLoadingPercent
	// get percent of load level
	int getLoadingPercent();

	// update
	// update per frame
	virtual void update();

    // packDataMultiplayer
    // pack data multiplayer
    virtual void packDataMultiplayer(CDataPacket *packet);
    
    // unPackDataMultiplayer
    // unpack data on multiplayer
    virtual void unpackDataMultiplayer(CDataPacket *packet, int hostKeyId);
    
    // removeDisconectedObject
    // remove all object at host is disconected
    virtual void removeDisconectedObject( int hostKeyId );
    
	// render
	// render level per frame
	virtual void render();
    
    // getNetworkObjID
    // reference network objectid to my game objectid
    long getNetworkObjID( SNetworkObjID& networkID );    
    void registerNetworkObjID( SNetworkObjID& networkID, long objectID );    
    void unRegisterNetworkObjID( SNetworkObjID& networkID );
    
	// setCamera
	// set current camera
	void setCamera( CGameCamera* cam );

	// getCamera
	// get current camera
	CGameCamera* getCamera();

	// checkTerrainCollide
	// check terrain collide
	bool checkTerrainCollide( core::line3df& ray, core::vector3df& outPos, core::triangle3df& outTri );

};

#endif