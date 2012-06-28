#include "stdafx.h"
#include "LevelScript.h"
#include "CGameLevel.h"

#define getLevel()	CGameLevel::getCurrentLevel()

//////////////////////////////////////////////////////////
// GAME OBJECT FUNCTION IMPLEMENT
//////////////////////////////////////////////////////////

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

// getObjectPosition
// get x,y,z of object
int getObjectPosition(lua_State* state)
{
	int objID = lua_tointeger(state,1);
	CGameObject* obj = getLevel()->searchObject( objID );
	if ( obj )
	{
		core::vector3df pos = obj->getPosition();
		lua_pushnumber( state, pos.X );
		lua_pushnumber( state, pos.Y );
		lua_pushnumber( state, pos.Z );
		return 3;
	}

	lua_pushnumber( state, 0 );
	lua_pushnumber( state, 0 );
	lua_pushnumber( state, 0 );
	return 3;
}

// setObjectPosition
// set x,y,z of position
// param: objectID, x, y, z
int setObjectPosition(lua_State* state)
{
	int objID	= lua_tointeger(state,1);
	float x		= (float)lua_tonumber(state,2);
	float y		= (float)lua_tonumber(state,3);
	float z		= (float)lua_tonumber(state,4);

	CGameObject* obj = getLevel()->searchObject( objID );
	if ( obj )
		obj->setPosition( core::vector3df(x,y,z) );
	
	return 0;
}

// getObjectRotation
// get x,y,z of rotation vector
int getObjectRotation(lua_State* state)
{
	int objID = lua_tointeger(state,1);
	CGameObject* obj = getLevel()->searchObject( objID );
	if ( obj )
	{
		core::vector3df rot = obj->getRotation();
		lua_pushnumber( state, rot.X );
		lua_pushnumber( state, rot.Y );
		lua_pushnumber( state, rot.Z );
		return 3;
	}

	lua_pushnumber( state, 0 );
	lua_pushnumber( state, 0 );
	lua_pushnumber( state, 0 );
	return 3;
}

// setObjectRotation
// set x,y,z of rotation
// param: objectID, x, y, z
int setObjectRotation(lua_State* state)
{
	int objID	= lua_tointeger(state,1);
	float x		= (float)lua_tonumber(state,2);
	float y		= (float)lua_tonumber(state,3);
	float z		= (float)lua_tonumber(state,4);

	CGameObject* obj = getLevel()->searchObject( objID );
	if ( obj )
		obj->setRotation( core::vector3df(x,y,z) );
	
	return 0;
}

// setObjectLookAtObject
// param: objectID, objectTargetID
int setObjectLookAtObject(lua_State* state)
{
	int objID		= lua_tointeger(state,1);
	int objTargetID	= lua_tointeger(state,2);

	CGameObject* obj		= getLevel()->searchObject( objID );
	CGameObject* objTarget	= getLevel()->searchObject( objTargetID );

	if ( obj && objTarget )
		obj->lookAt( objTarget );
	
	return 0;
}

// setObjectOrientation
// param: objectID, objectTargetID
int setObjectOrientation(lua_State* state)
{
	int objID		= lua_tointeger(state,1);
	int objTargetID	= lua_tointeger(state,2);

	CGameObject* obj		= getLevel()->searchObject( objID );
	CGameObject* objTarget	= getLevel()->searchObject( objTargetID );

	if ( obj && objTarget )
	{
		obj->lookAt( objTarget );
		core::vector3df rot = objTarget->getPosition() - obj->getPosition();
		obj->setOrientation( rot );
	}
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


//////////////////////////////////////////////////////////
// CAMERA FUNCTION IMPLEMENT
//////////////////////////////////////////////////////////

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

// setCameraFarValue
// set farvalue for camera
int setCameraFarValue(lua_State* state)
{
	int camID = lua_tointeger(state,1);
	double farValue = lua_tonumber(state,2);

	CGameObject* obj = getLevel()->searchObject( camID );
	if ( obj->getObjectType() == CGameObject::CameraObject )
	{
		CGameCamera *cam = (CGameCamera*)obj;
		cam->setFarValue( (float)farValue );
	}
	return 0;
}

// setCameraLookAtObj
// set camera look at to object position
int setCameraLookAtObj(lua_State* state)
{
	int camID = lua_tointeger(state,1);
	int objID = lua_tointeger(state,2);

	CGameObject* cam = getLevel()->searchObject( camID );
	CGameObject* obj = getLevel()->searchObject( objID );

	if ( cam == NULL || obj == NULL || cam->getObjectType() != CGameObject::CameraObject )
		return 0;
	
	((CGameCamera*)cam)->setTargetObject( obj );
	return 0;
}


//////////////////////////////////////////////////////////
// SCENENODE FUNCTION IMPLEMENT
//////////////////////////////////////////////////////////

// setSceneNodePosition
// set a scenenode to position
// params: objectID, sceneNodeName, x,y,z
int setSceneNodePosition(lua_State* state)
{
	int objID	= lua_tointeger(state,1);
	const char* sceneNodeName = lua_tostring(state,2);
	float x		= (float)lua_tonumber(state,3);
	float y		= (float)lua_tonumber(state,4);
	float z		= (float)lua_tonumber(state,5);

	CGameObject* obj = getLevel()->searchObject( objID );
	if ( obj )
	{
		CColladaMeshComponent *comp = (CColladaMeshComponent*)obj->getComponent( IObjectComponent::ColladaMesh );
		if ( comp )
		{
			CGameColladaSceneNode* node = comp->getSceneNode(sceneNodeName);
			if ( node )
			{
				// set position
				node->setPosition( core::vector3df(x,y,z) );
			}
		}
	}

	return 0;
}


/////////////////////////////////////////////////////////////
// registerCFunction
// implement lua func by c++ language
void registerCFunction()
{
	// object function
	REGISTER_C_FUNCTION(getObjectByName);
	REGISTER_C_FUNCTION(disableObject);
	REGISTER_C_FUNCTION(enableObject);
	REGISTER_C_FUNCTION(getObjectPosition);
	REGISTER_C_FUNCTION(setObjectPosition);
	REGISTER_C_FUNCTION(getObjectRotation);
	REGISTER_C_FUNCTION(setObjectRotation);
	REGISTER_C_FUNCTION(setObjectLookAtObject);
	REGISTER_C_FUNCTION(setObjectOrientation);
	REGISTER_C_FUNCTION(setAnimatorMoveToWayPoint);

	// camera function
	REGISTER_C_FUNCTION(setLevelCamera);
	REGISTER_C_FUNCTION(setCameraFarValue);
	REGISTER_C_FUNCTION(setCameraLookAtObj);

	// scenenode function
	REGISTER_C_FUNCTION(setSceneNodePosition);
	
}
// end register
/////////////////////////////////////////////////////////////