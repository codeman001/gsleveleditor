#ifndef _OBJECT_TRANSFORM_COMPONENT_
#define _OBJECT_TRANSFORM_COMPONENT_

#include "stdafx.h"
#include "CGameObject.h"
#include "IObjectComponent.h"

class CObjectTransformComponent: public IObjectComponent
{
protected:
	core::vector3df	m_rotate;
	core::vector3df	m_scale;

public:
	CObjectTransformComponent( CGameObject *p );
	virtual ~CObjectTransformComponent();

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

	// setScale
	// set scale the object
	inline void setScale(core::vector3df& v)
	{
		m_scale = v;
	}

	// setRotate
	// set rotate the object
	inline void setRotate(core::vector3df v)
	{
		m_rotate = v;
	}

};

#endif