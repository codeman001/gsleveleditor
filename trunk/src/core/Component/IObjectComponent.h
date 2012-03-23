#ifndef _IOBJECTCOMPONENT_
#define _IOBJECTCOMPONENT_

class CGameObject;

#include "CSerializable.h"

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
		NumBuildInComponent = Shadow
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

};

#endif