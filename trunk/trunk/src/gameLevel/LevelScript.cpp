#include "stdafx.h"
#include "LevelScript.h"
#include "CGameLevel.h"

#define getLevel()	CGameLevel::getCurrentLevel()

// getObjectByName
// get object id by name
// param: objName
int getObjectByName(lua_State* state)
{
	const char* objName = lua_tostring(state,1);
	
	CGameObject* p = getLevel()->searchObject( objName );
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
	CGameObject* cam = getLevel()->searchObject( camID );
	if ( cam->getObjectType() == CGameObject::CameraObject )
		getLevel()->setCamera((CGameCamera*)cam);

	return 0;
}

// disableObject
// disable active object
// param: objectID
int disableObject(lua_State* state)
{
	int objID = lua_tointeger(state,1);
	CGameObject* obj = getLevel()->searchObject( objID );
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
	CGameObject* obj = getLevel()->searchObject( objID );
	if ( obj )
		obj->setEnable(true);
	return 0;
}

// setAnimatorMoveToWayPoint
// set anim move on waypoint
// param: objectID, waypointID, loop
int setAnimatorMoveToWayPoint(lua_State* state)
{
	int objID = lua_tointeger(state,1);
	int wayID = lua_tointeger(state,2);	
	float speed = (float)lua_tonumber(state,3);
	int loop = lua_toboolean(state,4);

	CGameObject* obj = getLevel()->searchObject( objID );
	CGameObject* way = getLevel()->searchObject( wayID );
	if ( obj == NULL || way == NULL || way->getObjectType() != CGameObject::WaypointObject )
		return 0;

	obj->setAnimatorMoveToWayPoint( (CWayPoint*)way, speed, loop == 1 );
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
	REGISTER_C_FUNCTION(setAnimatorMoveToWayPoint);	
}
// end register
/////////////////////////////////////////////////////////////