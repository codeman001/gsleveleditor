//
//  CBasePlayerState.h
//  gameplay
//
//  Created by Pham Hong Duc on 1/27/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef _CBASEPLAYERSTATE_H
#define _CBASEPLAYERSTATE_H

#include "stdafx.h"
#include "gameEvent.h"
#include "CGameObject.h"
#include "CColladaMeshComponent.h"

#include "CLightObject.h"
#include "CGunLightComponent.h"
#include "CShadowComponent.h"

#include "CBulletRayComponent.h"

class CBasePlayerState:
	public IGameAnimationCallback
{

public:
    CBasePlayerState();
    virtual ~CBasePlayerState();
        
    void init(CGameObject* gameObj);

protected:    
	SEventPlayerMove		m_playerMoveEvt;
    SEventPlayerCommand     m_playerCmdEvt;    
    
    CColladaAnimation*		m_animationPackage;
	CColladaMeshComponent*	m_collada;

	bool					m_initPlayer;
	
	CGameObject*			m_owner;	
	std::string				m_luaObjName;

protected:
        
    // doNextState
	// change to next state
	void doNextState();	    
    
    // isFinishedAnim	
	bool isFinishedAnim( std::vector<CGameColladaSceneNode*>& nodes, int trackChannel = 0, int animLayer = 0);    	
        
public:
	// registerBoneTransformCallback
	// register 
	void registerBoneTransformCallback(CGameColladaSceneNode* bone, std::string& function)
	{
		m_boneTransformCallback[bone] = function;
	}

	// unRegisterBoneTransformCallback
	// unregister callback 
	void unRegisterBoneTransformCallback(CGameColladaSceneNode* bone)
	{
		std::map<CGameColladaSceneNode*, std::string>::iterator it;
		it = m_boneTransformCallback.find(bone);

		if ( it != m_boneTransformCallback.end() )		
			m_boneTransformCallback.erase(it);		
	}

protected:
 	

protected:
	void update();
	void initPlayerObjects();

	// call back frame update on scenenode
	virtual void _onUpdateFrameData( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int animLayer);
	virtual void _onUpdateFrameDataChannel( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int channel, int animLayer);
	virtual void _onUpdateFinishAbsolute( ISceneNode* node, core::matrix4& absoluteAnimationMatrix );
	
protected:
	std::map<CGameColladaSceneNode*, std::string>	m_boneTransformCallback;

};

#endif
