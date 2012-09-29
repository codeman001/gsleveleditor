#ifndef _IOBJECTCOMPONENT_
#define _IOBJECTCOMPONENT_

class CGameObject;

#include "CSerializable.h"
#include "CDataPacket.h"

class IObjectComponent
{
public:
	enum ComponentType
	{
		AnimMesh,
		StaticMesh,
		Skybox,
		ObjectTransform,
		ObjectCollision,
		Shadow,
		BoxObject,
		Billboard,
		Particle,
		ColladaMesh,
		Terrain,
		Grass,
		Water,
		EllipsoidCollision,
		Lighting,
		ObjectProperty,
		NumBuildInComponent
	};

	static const char *s_compType[];
protected:
	int			m_componentID;
	CGameObject	*m_gameObject;

public:
	IObjectComponent(CGameObject *pObj, int compID);

	virtual ~IObjectComponent();

	inline int getComponent()
	{
		return m_componentID;
	}

	// init
	// run when init object
	virtual void initComponent()	= 0;

	// update
	// run when update per frame
	virtual void updateComponent()	= 0;

	// saveData
	// save data to serializable
	virtual void saveData( CSerializable* pObj ) = 0;

	// loadData
	// load data to serializable
	virtual void loadData( CSerializable* pObj ) = 0;

    // packDataMultiplayer
    // pack data multiplayer
    virtual void packDataMultiplayer(CDataPacket *packet)
    {
        
    }
    
    // unPackDataMultiplayer
    // unpack data on multiplayer
    virtual void unpackDataMultiplayer(CDataPacket *packet)
    {
        
    }
    
	// isPhysicComponent
	// check is physic component
	virtual bool isPhysicComponent()
	{
		return false;
	}


};

template <class T>
IObjectComponent* newComponent( CGameObject *pObj ) 
{
	return new T(pObj); 
}

// declare pointer function
typedef IObjectComponent* (*newComponentFn)( CGameObject *pObj );

// declare struct
struct SComponentCreation
{
	const char*		componentTypeName;
	newComponentFn	spawnFn;
};


#endif