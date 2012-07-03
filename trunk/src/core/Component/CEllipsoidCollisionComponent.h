#ifndef _ELLIPSOILDCOLCOMPONENT_
#define _ELLIPSOILDCOLCOMPONENT_

#include "CGameObject.h"
#include "IObjectComponent.h"

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

public:

	// isFalling
	// chek objet is falling
	bool isFalling()
	{
		return m_isFalling;
	}

protected:
	
	// collideTest
	// test collide on world
	void collideTest( ITriangleSelector *world );

};

#endif