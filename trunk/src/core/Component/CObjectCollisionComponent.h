#ifndef _OBJECT_COLLISION_COMPONENT_
#define _OBJECT_COLLISION_COMPONENT_

#include "IObjectComponent.h"
#include "CGameObject.h"

class CObjectCollisionComponent: public IObjectComponent
{
protected:
	struct SObjectCollisionParam
	{
		IMesh		*mesh;
		ISceneNode	*node;
	};
public:
	enum ECollisionType
	{
		BoudingBox = 0,
		Triangle,
		OctreeTriange
	};

	ECollisionType	m_collisionType;
	int				m_collisionData;

public:
	CObjectCollisionComponent(CGameObject *owner);
	
	virtual ~CObjectCollisionComponent();

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
};

#endif