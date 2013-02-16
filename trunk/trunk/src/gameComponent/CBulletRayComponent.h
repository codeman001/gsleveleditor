#ifndef _CBULLETRAYCOMP_
#define _CBULLETRAYCOMP_

#include "IObjectComponent.h"
#include "CGameComponent.h"
#include "CGameObject.h"

class CBulletRayComponent: public IObjectComponent
{
protected:
	std::vector<core::line3df>		m_rays;
	std::vector<core::line3df>		m_raysSync;    
	float							m_damage;

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
	void addBulletRay( const core::line3df& ray )
	{
		m_rays.push_back(ray);
        m_raysSync.push_back(ray);
	}

	// setBulletDamage
	// set damage
	inline void setBulletDamage(float f)
	{
		m_damage = f;
	}

    // packDataMultiplayer
    // pack data multiplayer
    virtual void packDataMultiplayer(CDataPacket *packet);
    
    // unPackDataMultiplayer
    // unpack data on multiplayer
    virtual void unpackDataMultiplayer(CDataPacket *packet);
    
protected:
	
	// updateBulletCollision
	void updateBulletCollision(core::line3df& ray);

	// createSpark
	CGameObject* createSpark( core::vector3df position, const char* xml, float lifeDelay );
};

#endif