#ifndef _CPLAYERCOMPONENT_H_
#define _CPLAYERCOMPONENT_H_

#include "IObjectComponent.h"
#include "CColladaMeshComponent.h"
#include "CGameComponent.h"

#define SHIFTBIT(x)	(1<<x)

class CWeaponComponent;
class CInventoryComponent;
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
		PlayerRunBackward,
		PlayerRunLeft,
		PlayerRunRight,

		NumStateCount
	};

	enum EPlayerSubState
	{
		SubStateInit = 0,
		SubStateActive,
		SubStateEnd,
	};

	enum EKeyActionBit
	{
		KeyNone		= 0,
		KeyUp		= SHIFTBIT(1),
		KeyLeft		= SHIFTBIT(2),
		KeyRight	= SHIFTBIT(3),
		KeyBack		= SHIFTBIT(4),
		KeyFire		= SHIFTBIT(5)
	};

protected:
	EPlayerSubState			m_subState;
	EPlayerState			m_state;
	EPlayerState			m_nextState;
	EPlayerState			m_lastState;

	float					m_runSpeed;
	float					m_runNoGunSpeed;
	float					m_runBackSpeed;
	
	bool					m_runUp;
	bool					m_runBack;
	bool					m_runLeft;
	bool					m_runRight;
	bool					m_runCommand;

	bool					m_runNoGun;

	float					m_animShotCurrentTime;
	float					m_animCurrentTime;
	float					m_animCrossfadeTime;

	CColladaAnimation*		m_animationPackage;
	CColladaMeshComponent*	m_collada;
	CInventoryComponent*	m_inventory;

	CGameColladaSceneNode*	m_bipRootNode;
	CGameColladaSceneNode*	m_bipSpineNode;
	CGameColladaSceneNode*	m_bipSpine1Node;
	CGameColladaSceneNode*	m_gunDummyNode;


	vector<CGameColladaSceneNode*>	m_nodesHandsAndHead;
	vector<CGameColladaSceneNode*>	m_nodesChest;
	vector<CGameColladaSceneNode*>	m_nodesFoot;

	// begin anim name declare
	vector<std::string>				m_animIdle;

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
	std::string						m_animShootStraight;
	// end anim name declare

	int						m_keyActionBit;

	bool					m_noGun;

	float					m_runFactor;
	float					m_runAccel;

	float					m_spineRotation;
	float					m_lastRotation;

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

protected:

	// call back frame update on scenenode
	virtual void _onUpdateFrameData( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation );

protected:
	// updateState	
	void updateState();

	void updateStateIdle();	
	void updateStateTurn();
	void updateStateRunTurn();
	void updateStateRun();
	void updateStateRunBackward();
	void updateStateRunLeft();
	void updateStateRunRight();

	// stepAnimationTime	
	void stepAnimationTime();

	// updateActionShotWeapon
	// shoot weapon action
	void updateActionShotWeapon();

	// updateWeaponPosition
	// update weapon
	void updateWeaponPosition();	

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

	// getCurrentWeapon
	// get weapon
	CWeaponComponent* getCurrentWeapon();

public:

	// setSpineRotation	
	void setSpineRotation( float r )
	{
		const float maxAngle = 70.0f;
		m_spineRotation = r;
		if ( m_spineRotation > maxAngle )
			m_spineRotation = maxAngle;
		if ( m_spineRotation < -maxAngle )
			m_spineRotation = -maxAngle;
	}

	// setSpineLookAt
	// rotate spine to look at a pos
	void setSpineLookAt( const core::vector3df& pos, float speed = 2.0f );

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

	// fixAngle
	float fixAngle( float f );
};

#endif