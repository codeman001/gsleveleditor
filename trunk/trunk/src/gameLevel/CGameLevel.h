#ifndef _CGAMEL_LEVEL_
#define _CGAMEL_LEVEL_

#include "CGameObject.h"
#include "CGameCamera.h"
#include "CWayPoint.h"
#include "CTrigger.h"
#include "CZone.h"

#include "CComponentFactory.h"
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

	string			m_levelFile;
	vector<string>	m_listScriptFile;

protected:
	// compileGameScript
	// compile lua script
	void compileGameScript();
public:
	CGameLevel();
	virtual ~CGameLevel();

	// getCurrentLevel
	// get current level
	static CGameLevel* getCurrentLevel();

	// setCurrentLevel
	// set current level
	static void setCurrentLevel(CGameLevel* lv);

	// createZone
	// create a zone
	virtual CGameObject* createZone();

	// removeZone
	// remove zone
	virtual void removeZone(CGameObject* zone);

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

	// update
	// update per frame
	virtual void update();

	// render
	// render level per frame
	virtual void render();

	// setCamera
	// set current camera
	void setCamera( CGameCamera* cam );

};

#endif