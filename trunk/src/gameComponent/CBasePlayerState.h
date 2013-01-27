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



class CBasePlayerState
{
public:
    enum EPlayerState
	{
		PlayerNone = 0,
		PlayerIdle,		
		PlayerTurn,
		PlayerRunTurn,
		PlayerRun,
        PlayerRunFastTurn,
		PlayerRunFast,
        PlayerRunToRunFast,
        PlayerRunFastToRun,
        PlayerStandAim,
        PlayerRotate,
		NumStateCount
	};
    
    enum EPlayerUpBodyState
    {
        PlayerUpBodyAim = 0,
        PlayerUpBodyShoot,
        PlayerUpBodyReload,
		PlayerUpBodyOffGun,
        PlayerUpBodyAimToOffGun,
        PlayerUpBodyOffGunToAim,
        PlayerUpBodyRunFast      
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
    
    
    float					m_runSpeed;
	float					m_runFastSpeed;
    
	bool					m_runCommand;
    bool                    m_gunOnCommand;
    
	SEventPlayerMove		m_playerMoveEvt;
    SEventPlayerCommand     m_playerCmdEvt;
    
	core::vector3df			m_controlRotate;
    
    
    int                     m_idleAnimationID;
	float					m_animCurrentTime;
    
    CColladaAnimation*		m_animationPackage;
	CColladaMeshComponent*	m_collada;
    
	std::vector<CGameColladaSceneNode*>	m_nodesHandsAndHead;
	std::vector<CGameColladaSceneNode*>	m_nodesChest;	
	std::vector<CGameColladaSceneNode*>	m_nodesLeftShoulder;
	std::vector<CGameColladaSceneNode*>	m_nodesRightShoulder;
    
	std::vector<CGameColladaSceneNode*>	m_nodesFoot;
	std::vector<CGameColladaSceneNode*>	m_nodesUpBody;
    
	CGameColladaSceneNode*				m_nodeNeck;
    
	// begin anim name declare
	std::vector<std::string>		m_animIdle;
    
	std::string						m_animRun;
	std::string						m_animRunForward;
	std::string						m_animRunBackward;
	std::string						m_animRunStrafeLeft;
	std::string						m_animRunStrafeRight;
	std::string						m_animRunNoGun;
    
	std::string						m_animGunOn;
	std::string						m_animGunOff;
	std::string						m_animGunReload;
    
	std::string						m_animAimDown;
	std::string						m_animAimUp;
	std::string						m_animAimLeft;
	std::string						m_animAimRight;
	std::string						m_animAimStraight;
    
	std::string						m_animShootLeft;
	std::string						m_animShootRight;
	std::string						m_animShootUp;
	std::string						m_animShootDown;
	std::string						m_animShootStraight;
	// end anim name declare	
    
	std::string				m_offGunAnimation;
	float					m_offGunFactor;
    
	float					m_aimFactor;
	bool					m_aimRotateCharacter;
    
    float                   m_upBodyRunFastFactor;
    
	float					m_runFactor;	
	float					m_runAccel;
	float					m_runToRunFastAccel;
	float					m_gunOnOffAccel;
    
	float					m_animForwardFactor;
	float					m_animBackwardFactor;
	float					m_animLeftFactor;
	float					m_animRightFactor;
    
	float					m_spineRotation;
    
	float					m_rootBlendRotation;
	float					m_spineBlendRotation;
    
	core::vector3df			m_runTurnVector;
	core::vector3df			m_runCurrentVector;
    
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
	void doNextState()
	{		
		// change next state
		m_lastState	= m_state;
		m_state		= m_nextState;
		m_subState	= SubStateInit;
		m_nextState = PlayerNone;
	}
    
    // setOffGunAnimation
	inline void setOffGunAnimation( std::string animName)
	{
		m_offGunAnimation = animName;
	}
    
	// setOffGunFactor
	inline void setOffGunFactor(float f)
	{
		m_offGunFactor = f;
	}
    
    // isFinishedAnim	
	bool isFinishedAnim( std::vector<CGameColladaSceneNode*>& nodes, int trackChannel = 0, int animLayer = 0);
    
	// stepAnimationTime	
	void stepAnimationTime();
    
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
    

};

#endif
