#include "stdafx.h"
#include "LevelScript.h"
#include "CGameLevel.h"

// getObjectByName
// get object id by name
// param: objName
int getObjectByName(lua_State* state)
{
	const char* objName = lua_tostring(state,1);
	
	CGameObject* p = CGameLevel::getCurrentLevel()->searchObject( objName );
	if ( p )
		lua_pushinteger( state, p->getID() );
	else
		lua_pushinteger( state, -1 );
	return 1;
}

// setLevelCamera
// set active level camera
// param: cameraID
int setLevelCamera(lua_State* state)
{
	int camID = lua_tointeger(state,1);
	CGameObject* cam = CGameLevel::getCurrentLevel()->searchObject( camID );
	if ( cam->getObjectType() == CGameObject::CameraObject )
		CGameLevel::getCurrentLevel()->setCamera((CGameCamera*)cam);

	return 0;
}

// disableObject
// disable active object
// param: objectID
int disableObject(lua_State* state)
{
	int objID = lua_tointeger(state,1);
	CGameObject* obj = CGameLevel::getCurrentLevel()->searchObject( objID );
	if ( obj )
		obj->setEnable(false);
	return 0;
}

// enableObject
// active object
// param: objectID
int enableObject(lua_State* state)
{
	int objID = lua_tointeger(state,1);
	CGameObject* obj = CGameLevel::getCurrentLevel()->searchObject( objID );
	if ( obj )
		obj->setEnable(true);
	return 0;
}


/////////////////////////////////////////////////////////////
// registerCFunction
// implement lua func by c++ language
void registerCFunction()
{
	REGISTER_C_FUNCTION(getObjectByName);
	REGISTER_C_FUNCTION(setLevelCamera);
	REGISTER_C_FUNCTION(disableObject);
	REGISTER_C_FUNCTION(enableObject);
}
// end register
/////////////////////////////////////////////////////////////