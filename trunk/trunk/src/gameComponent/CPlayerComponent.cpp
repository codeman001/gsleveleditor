#include "stdafx.h"
#include "IView.h"

#include "CZone.h"
#include "CPlayerComponent.h"

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
    m_collada->setAnimationPackage( m_animationPackage );
    
    init(m_gameObject);

	// register event
	getIView()->registerEvent("CPlayerComponent", this);
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

	// need load animation package
	CColladaAnimationFactory* animFactory = CColladaAnimationFactory::getInstance();
	m_animationPackage = animFactory->getAnimation("playerAnim");
	if ( m_animationPackage == NULL )
		m_animationPackage = animFactory->loadAnimation("playerAnim", getIView()->getPath("data/mesh/character/hero/hero.anim"));	
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

	// pack player state
	packet->addByte((unsigned char)m_subState);
	packet->addByte((unsigned char)m_state);

	packet->addByte((unsigned char)m_upbodySubState);
	packet->addByte((unsigned char)m_upbodyState);

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
	}

	return true;
}


void CPlayerComponent::packDataMPState(CDataPacket *packet)
{
    
}


///////////////////////////////////////////////////////////////////////
// Player component implement
///////////////////////////////////////////////////////////////////////


// updateState	
void CPlayerComponent::updateState()
{	
	
}

///////////////////////////////////////////////////////////////////////
// Player component update state
///////////////////////////////////////////////////////////////////////



//void CPlayerComponent::updateStateTEMPLATE()
//{
//	if ( m_subState == SubStateInit )
//	{        
//		m_subState = SubStateActive;		
//	}
//	else if ( m_subState == SubStateEnd )
//	{		
//		doNextState();		
//	}
//	else
//	{	
//  }
//}


//void CPlayerComponent::updateUpperBodyTEMPLATE()
//{
//    if ( m_upbodySubState == SubStateInit )
//    {        
//        m_upbodySubState = SubStateActive;		
//    }
//    else if ( m_upbodySubState == SubStateEnd )
//    {		
//        doNextState();		
//    }
//    else
//    {	
//    }    
//}


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
    ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 10000.0f;
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
