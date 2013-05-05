#ifndef _CBULLETRAYCOMP_
#define _CBULLETRAYCOMP_

#include "IObjectComponent.h"
#include "CGameComponent.h"
#include "CGameObject.h"

class CBulletRayComponent: public IObjectComponent
{
public:
	struct SBulletRay
	{
		core::line3df	ray;
		float			damage;
		bool			throwObj;
		CGameObject*	weapon;
		CGameObject*	owner;

		SBulletRay()
		{
			damage = 5.0f;
			throwObj = false;
			weapon = NULL;
			owner = NULL;
		}
	};

protected:
	std::vector<SBulletRay>		m_rays;
	std::vector<SBulletRay>		m_raysSync;

public:
	CBulletRayComponent(CGameObject* obj);
	virtual ~CBulletRayComponent();
    
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
    
	// addBulletRay
	// add a ray
	void addBulletRay( const core::line3df& ray, float damage, bool throwObj, CGameObject *weapon, CGameObject *owner )
	{
		SBulletRay r;
		r.ray = ray;
		r.damage = damage;
		r.throwObj = throwObj;
		r.weapon = weapon;
		r.owner = owner;

		m_rays.push_back(r);
        m_raysSync.push_back(r);
	}

    // packDataMultiplayer
    // pack data multiplayer
    virtual void packDataMultiplayer(CDataPacket *packet);
    
    // unPackDataMultiplayer
    // unpack data on multiplayer
    virtual void unpackDataMultiplayer(CDataPacket *packet);
    
protected:
	
	// updateBulletCollision
	void updateBulletCollision(SBulletRay& bullet);

	// createSpark
	CGameObject* createSpark( core::vector3df position, const char* xml, float lifeDelay );
};

#endif