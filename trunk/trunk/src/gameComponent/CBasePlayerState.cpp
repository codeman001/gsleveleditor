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

CBasePlayerState::CBasePlayerState()
{
	m_owner = NULL;
	m_initPlayer = true;
}

CBasePlayerState::~CBasePlayerState()
{    
}

void CBasePlayerState::update()
{
	if ( m_initPlayer == true )
	{		
		initPlayerObjects();
		m_initPlayer = false;
	}	

	// set shadow
	CZone *zone = (CZone*)m_owner->getParent();
	CGameObject *shadowObj = zone->getStaticShadowManager();
	CShadowComponent* shadowComp = (CShadowComponent*)shadowObj->getComponent(IObjectComponent::Shadow);
	if ( shadowComp )
		shadowComp->setShadowNode( m_owner->getPosition(), core::vector3df(0,1,0) );
}


void CBasePlayerState::init(CGameObject* gameObj)
{    
	m_owner = gameObj;	
}

void CBasePlayerState::initPlayerObjects()
{

}

void CBasePlayerState::doNextState()
{		
    // sync at next frame
    CMultiplayerManager *mpMgr = CGameStateMgr::getInstance()->getCurrentState()->getMPManager();
    if ( mpMgr )
        mpMgr->setSyncData(true);
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


///////////////////////////////////////////////////////////////////////
// Player component end callback animation function
///////////////////////////////////////////////////////////////////////

// call back frame update on scenenode
void CBasePlayerState::_onUpdateFrameData( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int animLayer)
{
	std::map<CGameColladaSceneNode*, std::string>::iterator itBone = m_boneTransformCallback.find( (CGameColladaSceneNode*)node);
	if ( itBone != m_boneTransformCallback.end() )
	{	
//#if 0
		// implement in lua
		void *ret = CScriptManager::getInstance()->startFuncAndGetResult(
			itBone->second.c_str(),	// lua function
				"ss",				// param types
				m_luaObjName.c_str(),
				node->getName()
			);
		 
		if ( ret != NULL )
		{
			core::quaternion *q = (core::quaternion*)ret;

			// apply transform
			rotation = rotation*(*q);
		}
//#endif
	}
}

void CBasePlayerState::_onUpdateFrameDataChannel( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int channel, int animLayer)
{
}

void CBasePlayerState::_onUpdateFinishAbsolute( ISceneNode* node, core::matrix4& absoluteAnimationMatrix )
{
}