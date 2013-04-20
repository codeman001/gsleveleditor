#include "stdafx.h"
#include "IView.h"

#include "CZone.h"
#include "CPlayerComponent.h"

#include "gameConfig/CGameConfig.h"
#include "gameLevel/CGameLevel.h"
#include "gameDebug/CGameDebug.h"

#include "CNetworkPlayerComponent.h"

///////////////////////////////////////////////////////////////////////
// IObjectComponent overide implement
///////////////////////////////////////////////////////////////////////

CPlayerComponent::CPlayerComponent(CGameObject* obj)
	:IObjectComponent(obj, CGameComponent::PlayerComponent)
{	
	m_collada	= NULL;
	m_init		= true;
}

CPlayerComponent::~CPlayerComponent()
{
	// unregister event
	getIView()->unRegisterEvent( this );
}

// init
// run when init object
void CPlayerComponent::initComponent()
{
    m_collada = (CColladaMeshComponent*)m_gameObject->getComponent( IObjectComponent::ColladaMesh );

	// load model by character id
	CGameConfig::SCharacterInfo *charInfo = CGameConfig::getInstance()->getCharacterInfo(m_charID);

	// load model
	m_collada->loadDae( charInfo->model.c_str() );

	// load animation
	CColladaAnimationFactory* animFactory = CColladaAnimationFactory::getInstance();
	m_animationPackage = animFactory->getAnimation( charInfo->name.c_str() );
	if ( m_animationPackage == NULL )
		m_animationPackage = animFactory->loadAnimation( charInfo->name.c_str(), getIView()->getPath(charInfo->anim));
    m_collada->setAnimationPackage( m_animationPackage );
    
	// apply lod
	for (int i = 0, n = (int)charInfo->lods.size(); i < n; i++)	
		m_collada->addLodData( charInfo->lods[i].distance, charInfo->lods[i].node.c_str() );	

    init(m_gameObject);

	// register event
	getIView()->registerEvent("CPlayerComponent", this);

	// init lua player component
	char luaObjName[64];
	sprintf(luaObjName, "CPlayerComp_%ld", m_gameObject->getID());
	m_luaObjName = luaObjName;
}

// update
// run when update per frame
void CPlayerComponent::updateComponent()
{
	// update gameplay	
	if ( m_collada == NULL )
		return;	    
    
	// only update state on current player	
	if ( m_gameObject->isNetworkController() == false )
	{
		// update
		CBasePlayerState::update();

		// update character action
		updateState();	

		// alway sync this object
		m_gameObject->setSyncNetwork(true);
	}
}

// saveData
// save data to serializable
void CPlayerComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( CGameComponent::s_compType[ m_componentID ] );

	pObj->addInt("charID", m_charID);
}

// loadData
// load data to serializable
void CPlayerComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
	
	// get char id
	m_charID = pObj->readInt();	
}

// packDataMultiplayer
// pack data multiplayer
void CPlayerComponent::packDataMultiplayer(CDataPacket *packet)
{
	CNetworkPlayerComponent *comp = (CNetworkPlayerComponent*)m_gameObject->getComponent(CGameComponent::NetworkPlayerComponent);
	if ( comp == NULL )
		return;

	// packet player control
	packet->addFloat(m_playerMoveEvt.rotate);
	packet->addFloat(m_playerMoveEvt.strength);
	packet->addByte(m_playerMoveEvt.run);	
	
	packet->addByte(m_playerCmdEvt.shoot);
	packet->addByte(m_playerCmdEvt.reload);

	// pack mp state
	packDataMPState(packet);
}
    
// unPackDataMultiplayer
// unpack data on multiplayer
void CPlayerComponent::unpackDataMultiplayer(CDataPacket *packet)
{
	CNetworkPlayerComponent *comp = (CNetworkPlayerComponent*)m_gameObject->getComponent(CGameComponent::NetworkPlayerComponent);
	if ( comp )
		comp->unpackDataFromPlayerComponent(packet);
}

// OnEvent
// cache event
bool CPlayerComponent::OnEvent(const SEvent& irrEvent)
{
	if ( irrEvent.EventType == EET_GAME_EVENT && m_gameObject->isNetworkController() == false )
	{
		if ( irrEvent.GameEvent.EventID == EvtPlayerMove )
		{
			m_playerMoveEvt = *((SEventPlayerMove*)irrEvent.GameEvent.EventData);			
		}
        else if ( irrEvent.GameEvent.EventID == EvtPlayerCommand )
        {
            m_playerCmdEvt = *((SEventPlayerCommand*)irrEvent.GameEvent.EventData);                        
        }

		// implement in lua
		CScriptManager::getInstance()->startFunc(
				"updatePlayerComponentInput",	// lua function
				"siddii",						// param types
				m_luaObjName.c_str(),
				m_playerMoveEvt.run,
				m_playerMoveEvt.strength,
				m_playerMoveEvt.rotate,
				(int)m_playerCmdEvt.shoot,
				(int)m_playerCmdEvt.reload
			);
	}

	return true;
}


void CPlayerComponent::packDataMPState(CDataPacket *packet)
{
    
}


// updateState	
void CPlayerComponent::updateState()
{	
	if ( m_init )
	{
		// call lua func
		CScriptManager::getInstance()->startFunc("createPlayerComponent","sl", m_luaObjName.c_str(), (unsigned long)m_gameObject);
		m_init = false;
	}

	// call update in lua func
	CScriptManager::getInstance()->startFunc("updatePlayerComponent","sd", m_luaObjName.c_str(), getIView()->getTimeStep() );
}


// rotatePlayerToFront
// rotate player
bool CPlayerComponent::rotatePlayerToFront(float step)
{
	core::line3df	ray		= getCameraRay();
	core::vector3df	colPos	= getCollisionPoint(ray);
			
	// rotate main character		
	core::vector3df v0 = m_gameObject->getFront();
	core::vector3df aimPos = colPos - m_gameObject->getPosition();
	aimPos.Y = 0;
	aimPos.normalize();

	bool finish = false;
	if ( turnToDir( v0, aimPos, 6.0f ) == true )
		finish = true;

	m_gameObject->lookAt( m_gameObject->getPosition() + v0 );
	return finish;
}


// getCameraFrontVector
// return camera front vector
core::vector3df CPlayerComponent::getCameraFrontVector()
{
	CGameCamera* cam = CGameLevel::getCurrentLevel()->getCamera();
	core::vector3df front = cam->getTarget() - cam->getPosition();
	front.Y = 0;
	front.normalize();	

	return front;
}

core::vector2df CPlayerComponent::getAimAngle( const core::vector3df aimPoint )
{
	core::vector2df ret;

	CGameColladaSceneNode *spine = m_collada->getSceneNode("Neck");	//RightGunTip
	core::vector3df spinePoint = spine->getAbsolutePosition();

	core::vector3df aimRay	= aimPoint - spinePoint;
	core::vector3df base	= m_gameObject->getFront();

	//core::line3df line;
	//line.start = spinePoint;
	//line.end = aimPoint;
	//CGameDebug::getInstance()->addDrawLine(line, SColor(255,255,255,255));

	aimRay.normalize();

	core::plane3df plane;
	
	// projection to right plane
	core::vector3df outRight;
	plane.setPlane( spinePoint, m_gameObject->getRight() );
	plane.getIntersectionWithLine( aimPoint, m_gameObject->getRight(), outRight );

	// projection to top plane
	core::vector3df outTop;
	plane.setPlane( spinePoint, m_gameObject->getUp() );
	plane.getIntersectionWithLine( aimPoint, m_gameObject->getUp(), outTop );

	// calc result
	core::vector3df aimRayX	= outTop - spinePoint;
	aimRayX.normalize();
	ret.X = getAngle(aimRayX, base);

	core::vector3df aimRayY	= outRight - spinePoint;
	aimRayY.normalize();
	ret.Y = fabsf(getAngle(aimRayY, base));
	if ( outRight.Y < spinePoint.Y )
		ret.Y = -ret.Y;

	ret.X = core::clamp<float>(ret.X, -45.0f, 45.0f);
	ret.Y = core::clamp<float>(ret.Y, -45.0f, 45.0f);

	return ret;
}

// getCameraRay
// get camera view ray
core::line3df CPlayerComponent::getCameraRay()
{
    core::line3d<f32> ray;
    ICameraSceneNode *camera = getIView()->getSceneMgr()->getActiveCamera();
    ray.start = camera->getPosition();	
    ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 50000.0f;

	// return the ray length: 500m
    return ray;
}

// getCollisionPoint
// check collision
core::vector3df CPlayerComponent::getCollisionPoint( core::line3df ray )
{
	core::vector3df ret;
	
	core::vector3df     outPoint;
	core::triangle3df   outTri;

	CGameLevel *level =	CGameLevel::getCurrentLevel();
	if ( level->checkTerrainCollide(ray, outPoint, outTri) == true )
	{
		ret = outPoint;
	}
	else
	{
		ret = ray.end;
	}
	 
	return ret;
}
