#ifndef _ELLIPSOILDCOLCOMPONENT_
#define _ELLIPSOILDCOLCOMPONENT_

#include "CGameObject.h"
#include "IObjectComponent.h"

class IEllipsoidCallback
{
public:
	virtual void _onUpdateEllipsoidBoundingBox(core::aabbox3df& box)
	{
	}
};

class CEllipsoidCollisionComponent: public IObjectComponent
{
protected:
	bool				m_isFirstUpdate;

	core::vector3df		m_lastPosition;
	core::vector3df		m_testPosition;
	core::vector3df		m_fallingVelocity;
	core::vector3df		m_translation;

	bool				m_isFalling;
	core::vector3df		m_gravity;
	core::vector3df		m_ellipsoidRadius;

	IEllipsoidCallback*	m_callback;
public:
	CEllipsoidCollisionComponent( CGameObject *pObj );

	virtual ~CEllipsoidCollisionComponent();

	// init
	// run when init object
	virtual void initComponent();

	// updateComponent
	// update object by frame
	virtual void updateComponent();

	// saveData
	// save data to serializable
	void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	void loadData( CSerializable* pObj );

	// isPhysicComponent
	// this is physic component
	virtual bool isPhysicComponent()
	{
		return true;
	}

	// initFromBBox
	// get box from bouding box of object
	void initFromBBox();

public:

	// isFalling
	// check objet is falling
	bool isFalling()
	{
		return m_isFalling;
	}

	// setElipsoidCallback
	// set call back
	void setElipsoidCallback(IEllipsoidCallback* callback)
	{
		m_callback = callback;
	}
protected:
	
	// collideTest
	// test collide on world
	void collideTest( ITriangleSelector *world );

};

#endif