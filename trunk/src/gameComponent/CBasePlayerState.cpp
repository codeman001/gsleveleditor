//
//  CBasePlayerState.cpp
//  gameplay
//
//  Created by Pham Hong Duc on 1/27/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"
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
    // default state
	m_state			= PlayerNone;	
	m_lastState		= PlayerNone;
	m_nextState		= PlayerNone;
    
	
	// init run const
	m_runSpeed				= 0.3f;
	m_runFastSpeed			= 0.5f;
    
	m_runCommand	= false;			
    m_gunOnCommand  = false;
    
	m_runFactor = 0.0f;
	m_runAccel	= 0.002f;
	m_runToRunFastAccel = 0.003f;
    
    m_idleAnimationID       = 0;
	m_animCurrentTime		= 0.0f;	
    
	m_spineRotation = 0.0f;
	
    m_upBodyRunFastFactor = 0.0f;
	m_offGunFactor = 1.0f;
    
	m_aimFactor = 0.0f;
	m_aimRotateCharacter = false;
}

CBasePlayerState::~CBasePlayerState()
{
    
}

void CBasePlayerState::init(CGameObject* gameObj)
{    
    // foot nodes
	m_nodesFoot.push_back( m_collada->getSceneNode("Reference") );
	m_nodesFoot.push_back( m_collada->getSceneNode("LeftGun") );
	m_nodesFoot.push_back( m_collada->getSceneNode("Hips") );		
	m_nodesFoot.push_back( m_collada->getSceneNode("LeftUpLeg") );
	m_nodesFoot.push_back( m_collada->getSceneNode("RightUpLeg") );
	m_collada->getChildsOfSceneNode("LeftUpLeg",	m_nodesFoot);
	m_collada->getChildsOfSceneNode("RightUpLeg",	m_nodesFoot);
    
	// left & right hand with gun
	m_collada->getChildsOfSceneNode("LeftShoulder",		m_nodesLeftShoulder);
	m_nodesLeftShoulder.push_back( m_collada->getSceneNode("LeftGun") );
	m_nodesLeftShoulder.push_back( m_collada->getSceneNode("LeftGunTip") );	
    
	m_collada->getChildsOfSceneNode("RightShoulder",	m_nodesRightShoulder);
	m_collada->getChildsOfSceneNode("RightGun",			m_nodesRightShoulder);
	m_nodesRightShoulder.push_back( m_collada->getSceneNode("RightGun") );
	m_nodesRightShoulder.push_back( m_collada->getSceneNode("RightGunTip") );    
    
	// hand, head
	m_nodesHandsAndHead.push_back( m_collada->getSceneNode("RightGun") );		
	m_collada->getChildsOfSceneNode("Spine3",m_nodesHandsAndHead);
    
	// up body
	m_nodesUpBody = m_nodesHandsAndHead;
	m_nodesUpBody.push_back( m_collada->getSceneNode("Spine") );	
	m_nodesUpBody.push_back( m_collada->getSceneNode("Spine1") );    
	m_nodesUpBody.push_back( m_collada->getSceneNode("Spine2") );
	m_nodesUpBody.push_back( m_collada->getSceneNode("Spine3") );
    
	// connected animation layer
    m_collada->getSceneNode("Spine")->setConnectAnimLayer(true, false, false, false);
    m_collada->setNodeReferenceByAnimLayer(m_collada->getSceneNode("RightGun"), m_collada->getSceneNode("RightHand"));
    
    
	// decalre anim list
	m_animIdle.push_back( "Hero@Idle" );
	m_animIdle.push_back( "Hero@Idle1" );
	m_animIdle.push_back( "Hero@Idle2" );
    
	m_animRunForward	= "Hero@RunForward";
	m_animRunBackward	= "Hero@RunBackward";
	m_animRunStrafeLeft	= "Hero@RunStrafeLeft";
	m_animRunStrafeRight= "Hero@RunStrafeRight";
	m_animRunNoGun		= "Hero@RunForwardNoGun";
    
	m_animGunOn			= "Hero@GunOn";
	m_animGunOff		= "Hero@GunOff";	
	m_animGunReload		= "Hero@GunReload";
    
	m_animAimDown		= "Hero@AimDown";
	m_animAimUp			= "Hero@AimUp";
	m_animAimLeft		= "Hero@AimLeft";
	m_animAimRight		= "Hero@AimRight";
	m_animAimStraight	= "Hero@AimStraight";
    
	m_animShootLeft		= "Hero@ShootLeft";
	m_animShootRight	= "Hero@ShootRight";
	m_animShootUp		= "Hero@ShootUp";
	m_animShootDown		= "Hero@ShootDown";
	m_animShootStraight	= "Hero@ShootStraight";
    
    
	// set basic state idle
	m_collada->setAnimation( m_animIdle[0] );    
	setState( PlayerIdle );
    
	// enable anim layer 1
    m_collada->enableAnimLayer(1, true);
	m_collada->setAnimLayer( m_nodesUpBody, 1 );		
	
	setUpBodyState(PlayerUpBodyOffGun );
    setOffGunAnimation("Hero@Idle");
	setOffGunFactor(1.0f);
}


// stepAnimationTime	
void CBasePlayerState::stepAnimationTime()
{
	float timeStep = getIView()->getTimeStep();
	m_animCurrentTime = m_animCurrentTime - timeStep;
	
	if ( m_animCurrentTime < 0 )
		m_animCurrentTime = 0;
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
