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

	m_isTestPlayer = false;
}

CPlayerComponent::~CPlayerComponent()
{
	// unregister collide obj
	CGameObject *zone = (CGameObject*)m_gameObject->getParent();
	if ( zone && zone->getObjectType() == CGameObject::ZoneObject )	
		((CZone*)zone)->unRegisterCollideObj( m_gameObject);

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
	m_collada->loadScene(charInfo->model.c_str() );

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

	// init collision
	ISceneManager *smgr = getIView()->getSceneMgr();
	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox(m_gameObject->getSceneNode());
	m_gameObject->getSceneNode()->setTriangleSelector(selector);
	selector->drop();

	CGameObject *zone = (CGameObject*)m_gameObject->getParent();
	if ( zone && zone->getObjectType() == CGameObject::ZoneObject )	
		((CZone*)zone)->registerCollideObj( m_gameObject);
		
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
	if ( irrEvent.EventType == EET_GAME_EVENT && m_gameObject->isNetworkController() == false && m_isTestPlayer == false )
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
