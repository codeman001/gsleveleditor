#ifndef _CGAMEL_LEVEL_
#define _CGAMEL_LEVEL_

#include "CGameObject.h"
#include "CGameCamera.h"
#include "CZone.h"

#include "CComponentFactory.h"
#include "CObjTemplateFactory.h"

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

public:
	CGameLevel();
	virtual ~CGameLevel();

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