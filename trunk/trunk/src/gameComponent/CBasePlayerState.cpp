//
//  CBasePlayerState.cpp
//  gameplay
//
//  Created by Pham Hong Duc on 1/27/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"
#include "gameState/CGameStateManager.h"
#include "CBasePlayerState.h"
#include "IView.h"


////////////////////////////////////////////////////////////
#define REAL_PRECISION	0.000001f
inline bool realIsZero(float x, float precision = REAL_PRECISION) 
{ 
	return fabs(x) < precision; 
};
inline bool realIsEqual(float a, float b, float precision = REAL_PRECISION) 
{ 
	return realIsZero(a-b, precision); 
};
////////////////////////////////////////////////////////////

CBasePlayerState::CBasePlayerState()
{
	// mesh time
	m_muzzleMeshTime = 0;

	m_gunMuzzle = NULL;
    m_gunLight  = NULL;
    m_gunLightComp = NULL;    

	m_owner = NULL;

	m_initPlayer = true;
}

CBasePlayerState::~CBasePlayerState()
{
    // delete gunlight & muzzle mesh
	delete m_gunLight;
	delete m_gunMuzzle;
}

void CBasePlayerState::update()
{
	if ( m_initPlayer == true )
	{
		initAnimationCallback();
		initPlayerObjects();

		m_initPlayer = false;
	}

	// update muzzle mesh time
	updateMuzzleMesh();

	// update gun muzzle & gun light
	m_gunLight->updateObject();
	m_gunMuzzle->updateObject();

	// set shadow
	CZone *zone = (CZone*)m_owner->getParent();
	CGameObject *shadowObj = zone->getStaticShadowManager();
	CShadowComponent* shadowComp = (CShadowComponent*)shadowObj->getComponent(IObjectComponent::Shadow);
	if ( shadowComp )
		shadowComp->setShadowNode( m_owner->getPosition(), core::vector3df(0,1,0) );
}


void CBasePlayerState::updateMuzzleMesh()
{
	if ( m_muzzleMeshTime > 0 )
	{
		m_muzzleMeshTime = m_muzzleMeshTime - getIView()->getTimeStep();
		if ( m_muzzleMeshTime <= 0.0f )
			m_muzzleMeshTime = 0.0f;

		// update muzzle mesh position
		CGameColladaSceneNode *gunTip = m_collada->getSceneNode("RightGunTip");
		core::matrix4 mat = gunTip->getAbsoluteTransformation();
		core::quaternion rot;
		rot.fromAngleAxis(core::degToRad(-90.0f), core::vector3df(0,1,0));
		mat *= rot.getMatrix();
        
        core::vector3df position(mat.getTranslation());
        core::vector3df rotation(mat.getRotationDegrees());
        
		m_gunMuzzle->setPosition(position);
		m_gunMuzzle->setRotation(rotation);
        
		m_gunMuzzle->setVisible(true);
	}
	else
	{
		m_gunMuzzle->setVisible(false);
	}
}

// getZoneBulletComponent
// get bullet manager
CBulletRayComponent* CBasePlayerState::getZoneBulletComponent()
{
    CZone *zone = (CZone*)m_owner->getParent();
    CGameObject *bulletMgr = (CGameObject*)zone->getBulletManager();
    
    CBulletRayComponent *bullet = (CBulletRayComponent*)bulletMgr->getComponent(CGameComponent::BulletRayComponent);
    return bullet;
}


void CBasePlayerState::init(CGameObject* gameObj)
{    
	m_owner = gameObj;


	/*
	m_animShootMachineGuns = "TP_Shoot_MachineGuns";
    	
	m_animRunForward	= "TP_RunFront";
	m_animRunBackward	= "TP_RunBack";
	m_animRunLeft		= "TP_RunLeft";
	m_animRunRight		= "TP_RunRight";

	
	// get all bone nodes
	std::vector<CGameColladaSceneNode*>	allNodes;
	allNodes.push_back( m_collada->getSceneNode("BoneRoot") );
	m_collada->getChildsOfSceneNode("BoneRoot", allNodes);
	
	// get up body nodes
	m_upBodyNodes.push_back( m_collada->getSceneNode("Bip01_Spine1-node") );
	m_collada->getChildsOfSceneNode("Bip01_Spine1-node", m_upBodyNodes);

	// find foot nodes
	std::vector<CGameColladaSceneNode*>::iterator i = allNodes.begin(), end = allNodes.end();
	while (i != end)
	{
		if ( std::find(m_upBodyNodes.begin(), m_upBodyNodes.end(), (*i)) == m_upBodyNodes.end() )
		{
			m_footNodes.push_back( (*i) );
		}
		i++;
	}

	// apply animation layer
	m_collada->setAnimLayer( m_footNodes,	0);
	m_collada->setAnimLayer( m_upBodyNodes, 1);	

	m_collada->enableAnimLayer(0, true);
	m_collada->enableAnimLayer(1, true);


	// connected animation layer
	m_collada->getSceneNode("Bip01_Spine1-node")->setConnectAnimLayer(true, false, false, false);
	*/
}

void CBasePlayerState::initPlayerObjects()
{
	// create gunlight obj
	m_gunLight = new CLightObject(NULL);
    m_gunLight->setLightData(400.0f, 1.0f);
    m_gunLightComp = new CGunLightComponent(m_gunLight);
    m_gunLight->addComponent(m_gunLightComp);
	m_gunLight->setParent(m_owner);

	// create a gun muzzle mesh obj
	m_gunMuzzle = new CGameObject(NULL);
	CColladaMeshComponent *gunMesh = new CColladaMeshComponent(m_gunMuzzle);
	gunMesh->loadFromFile( getIView()->getPath("data/mesh/character/hero/muzzleFlash.scene"));
	m_gunMuzzle->addComponent(gunMesh);
	m_gunMuzzle->setParent(m_owner);
}

// initAnimationCallback
// init callback func
void CBasePlayerState::initAnimationCallback()
{
	
}


void CBasePlayerState::doNextState()
{		
    // sync at next frame
    CMultiplayerManager *mpMgr = CGameStateMgr::getInstance()->getCurrentState()->getMPManager();
    if ( mpMgr )
        mpMgr->setSyncData(true);
}

// turnToDir
bool CBasePlayerState::turnToDir(core::vector3df& dir, const core::vector3df& turnTo, float speed )
{
	// rotate front vec
	float f = speed*0.1f*getIView()->getTimeStep();	
	
	if ( f >= fabsf(getAngle(dir, turnTo)) )
	{
		dir = turnTo;
        return true;
	}
    
    dir = interpolateTurnToDir( dir, turnTo, f );
    
    float rot = fabsf(getAngle(dir, turnTo));
    if ( rot <= speed )
    {
        dir = turnTo;
        return true;
    }
    
	return false;
}

// turnToDir
// turn vector dir to turnTo
core::vector3df CBasePlayerState::interpolateTurnToDir( const core::vector3df& turnFrom, const core::vector3df& turnTo, float f )
{
	// calc turn Direction
    core::vector3df normal;
    normal = turnTo.crossProduct(turnFrom);
    normal.normalize();
    
    core::quaternion q;
    q.fromAngleAxis( core::degToRad(f), normal);
    
	// rotate result vector
	core::vector3df dir = turnFrom;
	q.getMatrix().rotateVect( dir );
	dir.normalize();
    
	return dir;	
}

// getRatioWithAngle	
float CBasePlayerState::getRatioWithAngle( const core::vector3df& turnFrom, const core::vector3df& turnTo, float angle )
{
	float angleVec = turnTo.dotProduct( turnFrom );		
	angleVec = fixAngle(angleVec);
	angleVec = core::radToDeg( acosf(angleVec) );
    
	if ( realIsZero(angleVec) )
		return 1.0f;
    
	return angle/angleVec;
}

float CBasePlayerState::getAngle( const core::vector3df& v1, const core::vector3df& v2 )
{
	core::vector3df normal = v2.crossProduct(v1);	
	
	float angleVec = v2.dotProduct( v1 );
	
	angleVec = fixAngle(angleVec);
	angleVec = core::radToDeg( acosf(angleVec) );
    
	if ( normal.Y < 0 )
		angleVec = -angleVec;
	return angleVec;
}

// fixAngle
float CBasePlayerState::fixAngle( float f )
{
	if (realIsEqual(f, 1.f, 0.001f))
		f = 1.f;
	else if (realIsEqual(f, -1.f, 0.001f))
		f = -1.f;
	return f;
}

// isFinishedAnim	
bool CBasePlayerState::isFinishedAnim( std::vector<CGameColladaSceneNode*>& nodes, int trackChannel, int animLayer)
{
	for ( int i = 0, n = (int)nodes.size(); i < n; i++ )
	{		
		CGameAnimationTrack* track = nodes[i]->getAnimation(animLayer)->getTrack(trackChannel);
        
		if ( track->getTotalFrame() == 0 || track->isEnable() == false || track->isPause() == true )
			continue;
        
		if ( track->isEndTrack() == false || track->isCrossAnim() )
			return false;
	}
    
    
	return true;
}

// calcRunAnimationBlend
// calc animation
void CBasePlayerState::calcRunAnimationBlend(float rot, float &forward, float &backward, float &left, float &right)
{
	forward = 0.0f;
	backward = 0.0f;
	left = 0.0f;
	right = 0.0f;
    
	if ( -90.0f <= rot && rot <= 90.0f )
	{
		// move forward		
		backward = 0.0f;
		if ( rot <= 0.0f && rot <= 90.0f )
		{
			// right
			left = 0.0f;
            
			float fixAngle	= core::degToRad(fmodf(rot + 90.0f, 360.0f));
			float dForward	= fabsf(sinf(fixAngle));
            
			// we have sin2 + cos2 = 1
            // it mean dforward2 + dright2 = 1.0f
            forward = dForward*dForward;
			right   = 1.0f - forward;
		}
		else
		{
			// left
			right = 0.0f;
            
			float fixAngle	= core::degToRad(fmodf(rot + 90.0f, 360.0f));
			float dForward	= fabsf(sinf(fixAngle));
			
            forward = dForward*dForward;
            left = 1.0f - forward;
		}
	}
	else
	{
		// move back
		forward = 0.0f;
		if ( 90.0f <= rot && rot <= 180.0f )
		{
			// left
			right = 0.0f;
            
			float fixAngle	= core::degToRad(fmodf(rot + 90.0f, 360.0f));
			float dBackward	= fabsf(sinf(fixAngle));			
            
            backward    = dBackward*dBackward;
            left        = 1.0f - backward;
		}
		else
		{
			// right
			left = 0.0f;
            
			float fixAngle	= core::degToRad(fmodf(rot + 90.0f, 360.0f));
			float dBackward	= fabsf(sinf(fixAngle));
			
            backward    = dBackward*dBackward;
            right       = 1.0f - backward;
		}
	}
    
}

void CBasePlayerState::calcAimAnimationBlend(core::vector2df angle, float &up, float &down, float &left, float &right)
{
    
    const float maxAngleLR = 45.0f;
    const float maxangleUD = 45.0f;
    
    if ( angle.X > 0 )
    {
        // aim left
        if (angle.Y < 0 )
        {
            // aim left down
            left = fabsf(angle.X)/maxAngleLR;
            down = fabsf(angle.Y)/maxangleUD;
            
            right = 0.0f;
            up = 0.0f;
        }
        else 
        {
            // aim left up
            left    = fabsf(angle.X)/maxAngleLR;
            up      = fabsf(angle.Y)/maxangleUD;
            
            right = 0.0f;
            down = 0.0f;
        }
    }
    else 
    {
        // aim right
        if (angle.Y < 0 )
        {
            // aim right down
            right = fabsf(angle.X)/maxAngleLR;
            down = fabsf(angle.Y)/maxangleUD;
            
            left = 0.0f;
            up = 0.0f;
        }
        else 
        {
            // aim right up
            right = fabsf(angle.X)/maxAngleLR;
            up = fabsf(angle.Y)/maxangleUD;
            
            left = 0.0f;
            down = 0.0f;
        }
    }
}




///////////////////////////////////////////////////////////////////////
// Player component end callback animation function
///////////////////////////////////////////////////////////////////////

// call back frame update on scenenode
void CBasePlayerState::_onUpdateFrameData( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int animLayer)
{
}

void CBasePlayerState::_onUpdateFrameDataChannel( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int channel, int animLayer)
{
}

void CBasePlayerState::_onUpdateFinishAbsolute( ISceneNode* node, core::matrix4& absoluteAnimationMatrix )
{	
}