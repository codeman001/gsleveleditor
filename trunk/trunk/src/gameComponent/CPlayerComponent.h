#ifndef _CPLAYERCOMPONENT_H_
#define _CPLAYERCOMPONENT_H_

#include "IObjectComponent.h"
#include "CColladaMeshComponent.h"
#include "CGameComponent.h"

#include "gameEvent.h"

class CWeaponComponent;
class CColladaMeshComponent;

class CPlayerComponent: 
	public IObjectComponent,
	public IEventReceiver,
	public IGameAnimationCallback
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
		PlayerStandShooting,
        PlayerRunAim,
		NumStateCount
	};

    enum EPlayerUpBodyState
    {
        PlayerUpBodyAim = 0,
        PlayerUpBodyShoot,
        PlayerUpBodyReload,
		PlayerUpBodyOffGun
    };
    
	enum EPlayerSubState
	{
		SubStateInit = 0,
		SubStateActive,
		SubStateEnd,
	};

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
    bool					m_init;

	SEventPlayerMove		m_playerMoveEvt;
    SEventPlayerCommand     m_playerCmdEvt;
	core::vector3df			m_controlRotate;

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
	std::string				m_offGunAnimation1;
	float					m_offGunFactor;
	float					m_offGunFactor1;

	float					m_aimFactor;
	bool					m_aimRotateCharacter;

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

public:
	CPlayerComponent(CGameObject* obj);
	virtual ~CPlayerComponent();

	// init
	// run when init object
	virtual void initComponent();

	// update
	// run when update per frame
	virtual void updateComponent();

	// saveData
	// save data to serializable
	virtual void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	virtual void loadData( CSerializable* pObj );

	// OnEvent
	// cache event
	virtual bool OnEvent(const SEvent& irrEvent);
public:

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
    
protected:

	// call back frame update on scenenode
	virtual void _onUpdateFrameData( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int animLayer);
	virtual void _onUpdateFrameDataChannel( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int channel, int animLayer);
	virtual void _onUpdateFinishAbsolute( ISceneNode* node, core::matrix4& absoluteAnimationMatrix );
	

protected:
	// updateState	
	void updateState();

	void updateStateIdle();	
	void updateStateTurn();
	void updateStateRunTurn();
	void updateStateRun();
	void updateStateRunFastTurn();    
	void updateStateRunFast();
    void updateStateRunToRunFast();
    void updateStateRunFastToRun();
    void updateStateStandAim();   	
    void updateStateStandShooting();
    
    // updateUpperBody
    // update state aim, shoot, reload
    void updateUpperBody();
    void updateUpperBodyAim();
    void updateUpperBodyShoot();
    void updateUpperBodyReload();    
    void updateUpperBodyOffgun();

	// isFinishedAnim	
	bool isFinishedAnim( std::vector<CGameColladaSceneNode*>& nodes, int trackChannel = 0, int animLayer = 0);

	// stepAnimationTime	
	void stepAnimationTime();

	// calcRunAnimationBlend
	// calc animation
	void calcRunAnimationBlend(float rot, float &forward, float &backward, float &left, float &right);

	void calcAimAnimationBlend(core::vector2df angle, float &up, float &down, float &left, float &right);	

	// doNextState
	// change to next state
	void doNextState()
	{		
		// change next state
		m_lastState	= m_state;
		m_state		= m_nextState;
		m_subState	= SubStateInit;
		m_nextState = CPlayerComponent::PlayerNone;
	}	
	
public:

	// setSpineRotation	
	void setSpineRotation( float r )
	{
		const float maxAngle = 110.0f;
		m_spineRotation = r;
		m_spineRotation = core::clamp<float>(m_spineRotation, -maxAngle, maxAngle);
	}

	// setOffGunAnimation
	inline void setOffGunAnimation( std::string animName, std::string animName1 )
	{
		m_offGunAnimation = animName;
		m_offGunAnimation1 = animName1;
	}

	// setOffGunFactor
	inline void setOffGunFactor(float f, float f1)
	{
		m_offGunFactor = f;
		m_offGunFactor1 = f1;
	}

	// setSpineLookAt
	// rotate spine to look at a pos
	void setSpineLookAt( const core::vector3df& pos, float speed = 3.0f );

	// getCameraFrontVector
	// return camera front vector
	core::vector3df getCameraFrontVector();

	// turnToDir
	bool turnToDir(core::vector3df& dir, const core::vector3df& turnTo, float speed );

	// turnToDir
	// turn vector dir to turnTo
	core::vector3df interpolateTurnToDir( const core::vector3df& turnFrom, const core::vector3df& turnTo, float f );

	// getRatioWithAngle	
	float getRatioWithAngle( const core::vector3df& turnFrom, const core::vector3df& turnTo, float angle );
	float getAngle( const core::vector3df& v1, const core::vector3df& v2 );
	core::vector2df getAimAngle( const core::vector3df aimPoint );

	// fixAngle
	float fixAngle( float f );
    
    // getCameraRay
    // get camera view ray
    core::line3df getCameraRay();
    
	// getCollisionPoint
	// check collision
	core::vector3df getCollisionPoint( core::line3df ray );
};

#endif