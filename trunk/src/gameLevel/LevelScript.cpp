#include "stdafx.h"
#include "LevelScript.h"
#include "CGameLevel.h"
#include "IView.h"

#include "CColladaMeshComponent.h"
#include "gameComponent/CPlayerComponent.h"

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
    unsigned long ptr = (unsigned long)p; 
	if ( p )
		lua_pushinteger( state, (lua_Integer)ptr );
	else
		lua_pushinteger( state, 0 );
	return 1;
}

// disableObject
// disable active object
// param: objectID
int disableObject(lua_State* state)
{
	lua_Integer objID = lua_tointeger(state,1);
	CGameObject* obj = (CGameObject*)objID;
	if ( obj )
		obj->setEnable(false);
	return 0;
}

// enableObject
// active object
// param: objectID
int enableObject(lua_State* state)
{
	lua_Integer objID = lua_tointeger(state,1);
	CGameObject* obj = (CGameObject*)objID;
	if ( obj )
		obj->setEnable(true);
	return 0;
}

// getObjectPosition
// get x,y,z of object
int getObjectPosition(lua_State* state)
{
	lua_Integer objID = lua_tointeger(state,1);
	CGameObject* obj = (CGameObject*)objID;
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
	lua_Integer objID	= lua_tointeger(state,1);
	float x		= (float)lua_tonumber(state,2);
	float y		= (float)lua_tonumber(state,3);
	float z		= (float)lua_tonumber(state,4);

	CGameObject* obj = (CGameObject*)objID;
	if ( obj )
	{
		core::vector3df newPos(x,y,z);
		obj->setPosition( newPos );
	}

	return 0;
}

// getObjectRotation
// get x,y,z of rotation vector
int getObjectRotation(lua_State* state)
{
	lua_Integer objID = lua_tointeger(state,1);
	CGameObject* obj = (CGameObject*)objID;
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
	lua_Integer objID	= lua_tointeger(state,1);
	float x		= (float)lua_tonumber(state,2);
	float y		= (float)lua_tonumber(state,3);
	float z		= (float)lua_tonumber(state,4);

	CGameObject* obj = (CGameObject*)objID;
	if ( obj )
	{
		core::vector3df newRot(x,y,z);
		obj->setRotation( newRot );
	}

	return 0;
}

// setObjectLookAtObject
// param: objectID, objectTargetID
int setObjectLookAtObject(lua_State* state)
{
	lua_Integer objID		= lua_tointeger(state,1);
	lua_Integer objTargetID	= lua_tointeger(state,2);

	CGameObject* obj		= (CGameObject*)objID;
	CGameObject* objTarget	= (CGameObject*)objTargetID;

	if ( obj && objTarget )
		obj->lookAt( objTarget );
	
	return 0;
}

// setObjectOrientation
// param: objectID, objectTargetID
int setObjectOrientation(lua_State* state)
{
	lua_Integer objID		= lua_tointeger(state,1);
	lua_Integer objTargetID	= lua_tointeger(state,2);

	CGameObject* obj		= (CGameObject*)objID;
	CGameObject* objTarget	= (CGameObject*)objTargetID;

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
	lua_Integer objID = lua_tointeger(state,1);
	lua_Integer wayID = lua_tointeger(state,2);	
	float speed = (float)lua_tonumber(state,3);
	int loop = lua_toboolean(state,4);

	CGameObject* obj = (CGameObject*)objID;
	CGameObject* way = (CGameObject*)wayID;
	if ( obj == NULL || way == NULL || way->getObjectType() != CGameObject::WaypointObject )
		return 0;

	obj->setAnimatorMoveToWayPoint( (CWayPoint*)way, speed, loop == 1 );
	return 0;
}

// addObjectLod
// add lod data
// param: objectID, distance, node
int addObjectLod(lua_State* state)
{
	lua_Integer objID = lua_tointeger(state,1);
	float dis = (float)lua_tonumber(state,2);
	const char* node = lua_tostring(state,3);

	CGameObject* obj = (CGameObject*)objID;
	
	if ( obj )
	{
		// add on player comp
		CColladaMeshComponent* colladaComp = (CColladaMeshComponent*)obj->getComponent( IObjectComponent::ColladaMesh );
		if ( colladaComp )
			colladaComp->addLodData( dis, node );
	}

	return 0;
}

// clearObjectLod
// clear lod data, return default collada node
int clearObjectLod(lua_State* state)
{
	lua_Integer objID = lua_tointeger(state,1);
	CGameObject* obj = (CGameObject*)objID;
	
	// clear all lod data
	CColladaMeshComponent* colladaComp = (CColladaMeshComponent*)obj->getComponent( IObjectComponent::ColladaMesh );
	if ( colladaComp )
		colladaComp->clearLodData();

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
	lua_Integer camID = lua_tointeger(state,1);
	CGameObject* cam = (CGameObject*)camID;
	if ( cam && cam->getObjectType() == CGameObject::CameraObject )
		getLevel()->setCamera((CGameCamera*)cam);

	return 0;
}

// setCameraFarValue
// set farvalue for camera
int setCameraFarValue(lua_State* state)
{
	lua_Integer camID = lua_tointeger(state,1);
	double farValue = lua_tonumber(state,2);

	CGameObject* obj = (CGameObject*)camID;
	if ( obj && obj->getObjectType() == CGameObject::CameraObject )
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
	lua_Integer camID = lua_tointeger(state,1);
	lua_Integer objID = lua_tointeger(state,2);

	CGameObject* cam = (CGameObject*)camID;
	CGameObject* obj = (CGameObject*)objID;

	if ( cam == NULL || obj == NULL || cam->getObjectType() != CGameObject::CameraObject )
		return 0;
	
	((CGameCamera*)cam)->setTargetObject( obj );
	return 0;
}

// setCameraFollowObject
// set camera allway follow obj
int setCameraFollowObject(lua_State* state)
{
	lua_Integer camID = lua_tointeger(state,1);
	lua_Integer objID = lua_tointeger(state,2);
	float radius  = (float)lua_tonumber(state,3);

	CGameObject* cam = (CGameObject*)camID;
	CGameObject* obj = (CGameObject*)objID;

	if ( cam == NULL || obj == NULL || cam->getObjectType() != CGameObject::CameraObject )
		return 0;
	
	((CGameCamera*)cam)->setFollowObjectCamera( obj, radius );
	return 0;
}


// getCurrentCameraPosition
// get current camera pos
int getCurrentCameraPosition(lua_State* state)
{
	CGameCamera *cam = getLevel()->getCamera();

	if ( cam )
	{
		core::vector3df pos = cam->getPosition();
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

//////////////////////////////////////////////////////////
// SCENENODE FUNCTION IMPLEMENT
//////////////////////////////////////////////////////////

// setSceneNodePosition
// set a scenenode to position
// params: objectID, sceneNodeName, x,y,z
int setSceneNodePosition(lua_State* state)
{
	lua_Integer colladaID = lua_tointeger(state,1);
	const char* sceneNodeName = lua_tostring(state,2);

	float x		= (float)lua_tonumber(state,3);
	float y		= (float)lua_tonumber(state,4);
	float z		= (float)lua_tonumber(state,5);

	CColladaMeshComponent *comp = (CColladaMeshComponent*)colladaID;
	if ( comp )
	{
		CGameColladaSceneNode* node = comp->getSceneNode(sceneNodeName);
		if ( node )
		{
			// set position
			node->setPosition( core::vector3df(x,y,z) );
		}
	}

	return 0;
}


// setSceneNodeIsSkydome
// set sky box
int setSceneNodeIsSkydome(lua_State* state)
{
	lua_Integer colladaID	= lua_tointeger(state,1);
	const char* sceneNodeName = lua_tostring(state,2);	

	CColladaMeshComponent *comp = (CColladaMeshComponent*)colladaID;
	if ( comp )
	{
		CGameColladaSceneNode* node = comp->getSceneNode(sceneNodeName);
		if ( node )
		{
			// set position
			node->setSkydome( true );
		}
	}	

	return 0;
}

// setSceneNodeVisible
// set sky box
int setSceneNodeVisible(lua_State* state)
{
	lua_Integer colladaID	= lua_tointeger(state,1);
	const char* sceneNodeName = lua_tostring(state,2);
	bool b		= lua_toboolean(state,3) == 1;
	
	CColladaMeshComponent *comp = (CColladaMeshComponent*)colladaID;
	if ( comp )
	{
		CGameColladaSceneNode* node = comp->getSceneNode(sceneNodeName);
		if ( node )
		{
			// set position
			node->setVisible( b );
		}
	}	

	return 0;
}

// getColladaSceneNode
// get collada scenenode
int getColladaSceneNode(lua_State* state)
{
	lua_Integer colladaID = lua_tointeger(state,1);
	const char* sceneNodeName = lua_tostring(state,2);

	CColladaMeshComponent *collada = (CColladaMeshComponent*)colladaID;
	CGameColladaSceneNode *node = NULL;

	if ( collada )
		 node = collada->getSceneNode(sceneNodeName);	
	
	lua_pushnumber( state, (lua_Integer) node );
	return 1;
}

// getChildsOfColladaSceneNode
// get list child scene node
int getChildsOfColladaSceneNode(lua_State* state)
{
	lua_Integer colladaID = lua_tointeger(state,1);
	const char* sceneNodeName = lua_tostring(state,2);

	CColladaMeshComponent *collada = (CColladaMeshComponent*)colladaID;
	if ( collada )
	{
		std::vector<CGameColladaSceneNode*>	listChilds;
		collada->getChildsOfSceneNode(sceneNodeName, listChilds);
		
		int newTable = 0;

		// create lua array
		if ( listChilds.size() > 0 )
		{
			lua_createtable(state, (int)listChilds.size(), 0);
			newTable = lua_gettop(state);
		}	

		int idx = 1;
		std::vector<CGameColladaSceneNode*>::iterator i = listChilds.begin(), end = listChilds.end();
		while ( i != end )
		{
			// push variable
			lua_pushnumber(state, (lua_Integer) (*i) );			

			// set result to table
			// see http://pgl.yoyo.org/luai/i/lua_rawseti
			lua_rawseti (state, newTable, idx++);			

			i++;
		}
	}
	
	return 1;
}

// setSceneNodeIsJoinAnimLayer
// set scenenode as join anim
int setSceneNodeIsJoinAnimLayer(lua_State* state)
{
	lua_Integer colladaID = lua_tointeger(state, 1);
	const char* sceneNodeName = lua_tostring(state,2);
	int b = lua_toboolean(state,3);

	int x = lua_toboolean(state,4);
	int y = lua_toboolean(state,5);
	int z = lua_toboolean(state,6);

	CColladaMeshComponent *collada = (CColladaMeshComponent*)colladaID;
	if ( collada )	
		collada->getSceneNode(sceneNodeName)->setConnectAnimLayer(b == 1, x == 1, y == 1, z == 1);	
	
	return 0;
}

//////////////////////////////////////////////////////////
// LIGHTING FUNCTION IMPLEMENT
//////////////////////////////////////////////////////////

// setLevelAmbientLight
// set default lighting for level
int setLevelAmbientLight(lua_State* state)
{
	u32 r	= (u32)lua_tointeger(state,1);
	u32 g	= (u32)lua_tointeger(state,2);
	u32 b	= (u32)lua_tointeger(state,3);
	getIView()->getSceneMgr()->setAmbientLight( SColor(0xff, r,g,b) );
	return 0;
}


//////////////////////////////////////////////////////////
// COLLADA COMPONENT FUNCTION IMPLEMENT
//////////////////////////////////////////////////////////

// getObjectCollada
// param: obj
int getObjectCollada(lua_State* state)
{
	lua_Integer objectID = lua_tointeger(state,1);
	
	CGameObject* p = (CGameObject*)objectID;
	CColladaMeshComponent *collada = (CColladaMeshComponent*)p->getComponent(IObjectComponent::ColladaMesh);

	if ( collada )
		lua_pushinteger( state, (lua_Integer)collada );
	else
		lua_pushinteger( state, 0 );

	return 1;
}

// setColladaAnimationLayer
// set anim layer of list scenenode
int setColladaAnimationLayer(lua_State* state)
{	
	lua_Integer colladaID = lua_tointeger(state, 1);
	lua_Integer	layerID = lua_tointeger(state, 3);
	
	CColladaMeshComponent *collada = (CColladaMeshComponent*)colladaID;
	if ( collada == NULL )
		return 0;	

	int numSceneNode = lua_objlen(state, 2);
	for(int i = 1; i <= numSceneNode; i++) 
	{
		// push index of array
		lua_pushnumber(state, i);

		// push variable to stack
		// see http://pgl.yoyo.org/luai/i/lua_gettable
		// stack size = 4
		// 4	-1	index of array
		// 3	-2	layerID
		// 2	-3	tableOfSceneNode
		// 1	-4	colladaID
		lua_gettable(state, -3);
		
		// get nodeID on top of stack
		lua_Integer nodeID = lua_tointeger(state, -1);

		CGameColladaSceneNode *node = (CGameColladaSceneNode*)nodeID;
		if (node && node->getComponent() == collada)
		{
			node->setAnimLayer( (int)layerID );
		}

		// remove index of array
		lua_pop(state,1);
    }

	return 0;
}

// enableColladaAnimationLayer
// enable layer animation
int enableColladaAnimationLayer(lua_State* state)
{
	lua_Integer colladaID = lua_tointeger(state, 1);
	lua_Integer	layerID = lua_tointeger(state, 2);
	int b =	lua_toboolean(state,3);
	
	CColladaMeshComponent *collada = (CColladaMeshComponent*)colladaID;
	if ( collada == NULL )
		return 0;

	collada->enableAnimLayer( (int)layerID, b == 1 );
	return 0;
}

// setColladaAnimation
// set animation
int setColladaAnimation(lua_State* state)
{
	lua_Integer colladaID = lua_tointeger(state, 1);
	const char* animName = lua_tostring(state, 2);
	int track	= (int)lua_tointeger(state,3);
	int loop	= lua_toboolean(state,4);
	int layer	= (int)lua_tointeger(state,5);

	CColladaMeshComponent *collada = (CColladaMeshComponent*)colladaID;
	if ( collada )	
		collada->setAnimation(animName, track, loop == 1, layer);	

	return 0;
}

// getColladaCurrentAnimFrame
// get current animation frame
int getColladaCurrentAnimFrame(lua_State* state)
{
	lua_Integer colladaID = lua_tointeger(state, 1);
	int track	= (int)lua_tointeger(state,2);	
	int layer	= (int)lua_tointeger(state,3);

	CColladaMeshComponent *collada = (CColladaMeshComponent*)colladaID;
	if ( collada )	
	{
		float frame = collada->getCurrentFrame(track, layer);
		lua_pushnumber(state, frame);
	}
	return 1;
}

// setColladaPauseAnimAtFrame
// pause animation
int setColladaAnimFrame(lua_State* state)
{
	lua_Integer colladaID = lua_tointeger(state, 1);
	float frame = (float)lua_tonumber(state, 2);
	int track	= (int)lua_tointeger(state,3);	
	int layer	= (int)lua_tointeger(state,4);

	CColladaMeshComponent *collada = (CColladaMeshComponent*)colladaID;
	if ( collada )		
		collada->setCurrentFrame(frame, track, layer);

	return 0;
}

// setColladaPauseAnimAtFrame
// pause animation
int pauseColladaAnimAtFrame(lua_State* state)
{
	lua_Integer colladaID = lua_tointeger(state, 1);
	float frame = (float)lua_tonumber(state, 2);
	int track	= (int)lua_tointeger(state,3);	
	int layer	= (int)lua_tointeger(state,4);

	CColladaMeshComponent *collada = (CColladaMeshComponent*)colladaID;
	if ( collada )		
		collada->pauseAtFrame(frame, track, layer);

	return 0;
}

// resumeColladaAnim
// resume anim
int resumeColladaAnim(lua_State* state)
{
	lua_Integer colladaID = lua_tointeger(state, 1);	
	int track	= (int)lua_tointeger(state,2);	
	int layer	= (int)lua_tointeger(state,3);

	CColladaMeshComponent *collada = (CColladaMeshComponent*)colladaID;
	if ( collada )		
		collada->resumeAnim(track, layer);

	return 0;
}

//////////////////////////////////////////////////////////
// PLAYER COMPONENT FUNCTION IMPLEMENT
//////////////////////////////////////////////////////////

// getPlayerComponent
// return the player component
int getPlayerComponent(lua_State* state)
{
	lua_Integer objectID = lua_tointeger(state, 1);	
	CGameObject *obj = (CGameObject*)objectID;

	if ( obj )
	{
		CPlayerComponent *playerComp = (CPlayerComponent*)obj->getComponent(CGameComponent::PlayerComponent);
		lua_pushinteger( state, (lua_Integer)playerComp );
	}
	else
	{
		lua_pushinteger( state, 0 );
	}

	return 1;
}

// rotatePlayerToCameraFront
// rotate player to camera
int rotatePlayerToCameraFront(lua_State* state)
{
	lua_Integer playerID = lua_tointeger(state, 1);
	lua_Number rotStep = lua_tonumber(state,2);

	CPlayerComponent* playerComp = (CPlayerComponent*)playerID;

	bool finish = false;

	if ( playerComp )
		finish = playerComp->rotatePlayerToFront((float)rotStep);

	lua_pushinteger(state, (int)finish);
	return 1;
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
	REGISTER_C_FUNCTION(addObjectLod);
	REGISTER_C_FUNCTION(clearObjectLod);

	// camera function
	REGISTER_C_FUNCTION(setLevelCamera);
	REGISTER_C_FUNCTION(setCameraFarValue);
	REGISTER_C_FUNCTION(setCameraLookAtObj);
	REGISTER_C_FUNCTION(getCurrentCameraPosition);
	REGISTER_C_FUNCTION(setCameraFollowObject);

	// scenenode function
	REGISTER_C_FUNCTION(setSceneNodePosition);
	REGISTER_C_FUNCTION(setSceneNodeIsSkydome);	
	REGISTER_C_FUNCTION(setSceneNodeVisible);

	REGISTER_C_FUNCTION(getColladaSceneNode);
	REGISTER_C_FUNCTION(getChildsOfColladaSceneNode);
	REGISTER_C_FUNCTION(setSceneNodeIsJoinAnimLayer);

	// lighting function
	REGISTER_C_FUNCTION(setLevelAmbientLight);

	// collada function
	REGISTER_C_FUNCTION(getObjectCollada);
	REGISTER_C_FUNCTION(setColladaAnimationLayer);
	REGISTER_C_FUNCTION(enableColladaAnimationLayer);
	
	REGISTER_C_FUNCTION(setColladaAnimation);
	REGISTER_C_FUNCTION(getColladaCurrentAnimFrame);
	REGISTER_C_FUNCTION(setColladaAnimFrame);
	REGISTER_C_FUNCTION(pauseColladaAnimAtFrame);
	REGISTER_C_FUNCTION(resumeColladaAnim);

	// player function
	REGISTER_C_FUNCTION(getPlayerComponent);
	REGISTER_C_FUNCTION(rotatePlayerToCameraFront);
	
}
// end register
/////////////////////////////////////////////////////////////