#ifndef _CPLAYERCOMPONENT_H_
#define _CPLAYERCOMPONENT_H_

#include "IObjectComponent.h"

#include "CColladaMeshComponent.h"
#include "CGameComponent.h"

#include "CLightObject.h"
#include "CGunLightComponent.h"

#include "CBulletRayComponent.h"
#include "CBasePlayerState.h"

class CWeaponComponent;
class CColladaMeshComponent;
class CNetworkPlayerComponent;

class CPlayerComponent: 
    public CBasePlayerState,
	public IObjectComponent,
	public IEventReceiver,
	public IGameAnimationCallback
{
public:	


protected:

	float					m_muzzleMeshTime;

	CGameObject*			m_gunMuzzle;

    CLightObject*           m_gunLight;
    CGunLightComponent*     m_gunLightComp;
    
	CGameObject*			m_bullet;
	CBulletRayComponent*	m_bulletRayComp;

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

	// packDataMultiplayer
    // pack data multiplayer
    virtual void packDataMultiplayer(CDataPacket *packet);
    
    // unPackDataMultiplayer
    // unpack data on multiplayer
    virtual void unpackDataMultiplayer(CDataPacket *packet);

public:

    
protected:

	// call back frame update on scenenode
	virtual void _onUpdateFrameData( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int animLayer);
	virtual void _onUpdateFrameDataChannel( ISceneNode* node, core::vector3df& pos, core::vector3df& scale, core::quaternion& rotation, int channel, int animLayer);
	virtual void _onUpdateFinishAbsolute( ISceneNode* node, core::matrix4& absoluteAnimationMatrix );
	

protected:
	void packDataMPState(CDataPacket *packet);	

	// updateState	
	void updateState();
	void updateMuzzleMesh();

	void updateStateIdle();	
	void updateStateTurn();
	void updateStateRunTurn();
	void updateStateRun();
	void updateStateRunFastTurn();    
	void updateStateRunFast();
    void updateStateRunToRunFast();
    void updateStateRunFastToRun();
    void updateStateStandAim();   	
    void updateStatePlayerRotate();
    
	// mp sync
	void packDataStateIdle(CDataPacket *packet);
	void packDataStateTurn(CDataPacket *packet);
	void packDataStateRunTurn(CDataPacket *packet);
	void packDataStateRun(CDataPacket *packet);
	void packDataStateRunFastTurn(CDataPacket *packet);
	void packDataStateRunFast(CDataPacket *packet);
    void packDataStateRunToRunFast(CDataPacket *packet);
    void packDataStateRunFastToRun(CDataPacket *packet);
    void packDataStateStandAim(CDataPacket *packet);
    void packDataStatePlayerRotate(CDataPacket *packet);    


    // updateUpperBody
    // update state aim, shoot, reload
    void updateUpperBody();
    void updateUpperBodyAim();
    void updateUpperBodyShoot();
    void updateUpperBodyReload();    
    void updateUpperBodyOffgun();
    void updateUpperBodyAimToOffgun();
    void updateUpperBodyOffgunToAim();
    void updateUpperBodyRunFast();

    

	// calcRunAnimationBlend
	// calc animation
	void calcRunAnimationBlend(float rot, float &forward, float &backward, float &left, float &right);

	void calcAimAnimationBlend(core::vector2df angle, float &up, float &down, float &left, float &right);	
	
	inline void showMuzzle(float time)
	{
		m_muzzleMeshTime = time;
	}

public:

	// setSpineRotation	
	void setSpineRotation( float r )
	{
		const float maxAngle = 110.0f;
		m_spineRotation = r;
		m_spineRotation = core::clamp<float>(m_spineRotation, -maxAngle, maxAngle);
	}

	// setSpineLookAt
	// rotate spine to look at a pos
	void setSpineLookAt( const core::vector3df& pos, float speed = 3.0f );

	// getCameraFrontVector
	// return camera front vector
	core::vector3df getCameraFrontVector();

    // getCameraRay
    // get camera view ray
    core::line3df getCameraRay();
    
	// getCollisionPoint
	// check collision
	core::vector3df getCollisionPoint( core::line3df ray );

    // getAimAngle
    // get angle of hand
    core::vector2df getAimAngle( const core::vector3df aimPoint );
    
};

#endif