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
    enum EPlayerState
	{
		PlayerNone = 0,
		PlayerIdle,
		PlayerRun,        
        PlayerStandAim,        
		NumStateCount
	};
    
    enum EPlayerUpBodyState
    {
        PlayerUpBodyAim = 0,
        PlayerUpBodyShoot,
        PlayerUpBodyReload		
    };
    
	enum EPlayerSubState
	{
		SubStateInit = 0,
		SubStateActive,
		SubStateEnd,
	};

public:
    CBasePlayerState();
    virtual ~CBasePlayerState();
        
    void init(CGameObject* gameObj);
protected:
    EPlayerSubState			m_subState;
	EPlayerState			m_state;
	EPlayerState			m_nextState;
	EPlayerState			m_lastState;
    
    EPlayerSubState         m_upbodySubState;
    EPlayerUpBodyState      m_upbodyState;
        
    
	SEventPlayerMove		m_playerMoveEvt;
    SEventPlayerCommand     m_playerCmdEvt;    
    
    CColladaAnimation*		m_animationPackage;
	CColladaMeshComponent*	m_collada;
    	
	std::vector<CGameColladaSceneNode*>	m_nodesUpBody;    	
    
	// begin anim name declare
	std::string						m_animIdleAim;
    	
	std::string						m_animRunForward;
	std::string						m_animRunBackward;
	std::string						m_animRunLeft;
	std::string						m_animRunRight;	
    	
	// end anim name declare	
	float					m_runFactor;	
	float					m_runAccel;	
        
	float					m_animForwardFactor;
	float					m_animBackwardFactor;
	float					m_animLeftFactor;
	float					m_animRightFactor;    	
    
	float					m_rootBlendRotation;
	float					m_spineBlendRotation;
    
	core::vector3df			m_runTurnVector;
	core::vector3df			m_runCurrentVector;
    

	bool					m_initPlayer;
	float					m_muzzleMeshTime;
	
	CGameObject*			m_owner;
	CGameObject*			m_gunMuzzle;

    CLightObject*           m_gunLight;
    CGunLightComponent*     m_gunLightComp;  

protected:
    
	// setState
	// set state for player
	void setState( EPlayerState state )
	{
		if ( m_state == state )
		{
			m_nextState = state;
			return;
		}
        
		if ( m_state == PlayerNone )
		{
			m_state		= state;
			m_subState	= SubStateInit;
		}
		else
		{
			m_nextState = state;
			m_subState	= SubStateEnd;
		}
	}
    
    void setUpBodyState( EPlayerUpBodyState state )
    {
        m_upbodyState = state;
        m_upbodySubState = SubStateInit;
    }
    
    // doNextState
	// change to next state
	void doNextState();	    
    
    // isFinishedAnim	
	bool isFinishedAnim( std::vector<CGameColladaSceneNode*>& nodes, int trackChannel = 0, int animLayer = 0);    	
    
    // turnToDir
	bool turnToDir(core::vector3df& dir, const core::vector3df& turnTo, float speed );
    
	// turnToDir
	// turn vector dir to turnTo
	core::vector3df interpolateTurnToDir( const core::vector3df& turnFrom, const core::vector3df& turnTo, float f );
    
	// getRatioWithAngle	
	float getRatioWithAngle( const core::vector3df& turnFrom, const core::vector3df& turnTo, float angle );
    
	float getAngle( const core::vector3df& v1, const core::vector3df& v2 );	
    
	// fixAngle
	float fixAngle( float f );
    
    // calcRunAnimationBlend
	// calc animation
	void calcRunAnimationBlend(float rot, float &forward, float &backward, float &left, float &right);
    
	void calcAimAnimationBlend(core::vector2df angle, float &up, float &down, float &left, float &right);

	inline void showMuzzle(float time)
	{
		m_muzzleMeshTime = time;
	}

    // getZoneBulletComponent
    // get bullet manager
    CBulletRayComponent* getZoneBulletComponent();
    
protected:
	void update();
	void updateMuzzleMesh();

	void initAnimationCallback();
	void initPlayerObjects();

	// call back frame update on scenenode
	virtual void _onUpdateFrameData( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int animLayer);
	virtual void _onUpdateFrameDataChannel( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int channel, int animLayer);
	virtual void _onUpdateFinishAbsolute( ISceneNode* node, core::matrix4& absoluteAnimationMatrix );
	

};

#endif
