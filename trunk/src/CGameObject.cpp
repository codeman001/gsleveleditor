#include "stdafx.h"
#include "CGameObject.h"

core::vector3df CGameObject::s_ox	= core::vector3df(1.0f, 0.0f, 0.0f);
core::vector3df CGameObject::s_oy	= core::vector3df(0.0f, 1.0f, 0.0f);
core::vector3df CGameObject::s_oz	= core::vector3df(0.0f, 0.0f, 1.0f);

CGameObject::CGameObject()
{
	m_objectID		= -1;
	m_objectType	= NullObject;
	
	m_position		= core::vector3df(0.0f, 0.0f, 0.0f );
	m_rotation		= core::vector3df(0.0f, 0.0f, 0.0f );
	m_scale			= core::vector3df(1.0f, 1.0f, 1.0f );

	m_right			= CGameObject::s_ox;
	m_front			= CGameObject::s_oz;
	m_up			= CGameObject::s_oy;

	m_node			= NULL;
	
	m_enable		= true;
	m_visible		= true;
}

CGameObject::~CGameObject()
{	
}

// setRotation
// set 3d rotation
void CGameObject::setRotation(core::vector3df& rot)
{
	m_rotation		=	rot;

	m_right			= CGameObject::s_ox;
	m_front			= CGameObject::s_oz;
	m_up			= CGameObject::s_oy;

	core::matrix4 rotationMatrix;
	rotationMatrix.setRotationDegrees(m_rotation);

	rotationMatrix.rotateVect(m_front);
	rotationMatrix.rotateVect(m_right);
	rotationMatrix.rotateVect(m_up);

	updateNodeRotation();
}

// setYaw
// rotation oy
void CGameObject::setYaw(float angle)
{
	m_rotation.Y = angle;
	updateRotation();
}
	
// setYaw
// rotation ox
void CGameObject::setPitch(float angle)
{
	m_rotation.X = angle;
	updateRotation();
}

// setYaw
// rotation oz
void CGameObject::setRoll(float angle)
{
	m_rotation.Z = angle;
	updateRotation();
}

// getQuaternion
// get rotation
void CGameObject::getQuaternion( core::quaternion* q )
{
	core::matrix4 mat;
	mat.setRotationDegrees(m_rotation);
		
	*q = core::quaternion(mat);
}
	
// setQuaternion
// set rotation
void CGameObject::setQuaternion( core::quaternion& q )
{
	const core::matrix4& rotationMatrix = q.getMatrix();

	m_rotation		=	rotationMatrix.getRotationDegrees();

	m_right			= CGameObject::s_ox;
	m_front			= CGameObject::s_oz;
	m_up			= CGameObject::s_oy;

	rotationMatrix.rotateVect(m_front);
	rotationMatrix.rotateVect(m_right);
	rotationMatrix.rotateVect(m_up);
	
	updateNodeRotation();
}

// setOrientation
// set rotation by front & up
void CGameObject::setOrientation(const core::vector3df& front, const core::vector3df& up)
{
	m_front	= front;
	m_front.normalize();

	m_right	= up.crossProduct(m_front);
	m_up	= m_front.crossProduct(m_right);

	f32 matData[16];

	matData[ 0] = m_right.X;
	matData[ 1] = m_right.Y;
	matData[ 2] = m_right.Z;
	matData[ 3] = 0.0f;

	matData[ 4] = m_up.X;
	matData[ 5] = m_up.Y;
	matData[ 6] = m_up.Z;
	matData[ 7] = 0.0f;

	matData[ 8] = m_front.X;
	matData[ 9] = m_front.Y;
	matData[10] = m_front.Z;
	matData[11] = 0.0f;

	matData[12] = 0.0f;
	matData[13] = 0.0f;
	matData[14] = 0.0f;
	matData[15] = 1.0f;

	core::matrix4 rotationMatrix;
	rotationMatrix.setM(matData);

	m_rotation = rotationMatrix.getRotationDegrees();
	updateNodeRotation();
}

// lookAt
// rotation object to the object
void CGameObject::lookAt(CGameObject* pObject)
{
	if (pObject != NULL)
		lookAt( pObject->getPosition() );
}

// lookAt
// rotation object to pos
void CGameObject::lookAt(const core::vector3df& pos)
{
	float angle = (float)(core::vector2df( pos.X - m_position.X, pos.Z - m_position.Z ).getAngleTrig());

	// rotation oz
	setRotation(core::vector3df(0.0f, 90-angle, 0.0f));
}

// updateRotation
void CGameObject::updateRotation()
{
	core::matrix4 mat;
	mat.setRotationDegrees(m_rotation);

	core::quaternion q(mat);

	m_right			=	q * CGameObject::s_ox;
	m_front			=	q * CGameObject::s_oz;
	m_up			=	q * CGameObject::s_oy;

	updateNodeRotation();
}

// updateNodeRotation
// update irr node rotation
void CGameObject::updateNodeRotation()
{
	if ( m_node )
	{
		m_node->setRotation( m_rotation );
	}
}

// updateNodePosition
// update irr node position
void CGameObject::updateNodePosition()
{
	if ( m_node )
	{
		m_node->setPosition( m_position );
	}
}

// destroyNode
// remove node on scene
void CGameObject::destroyNode()
{
	if ( m_node )
	{
		m_node->remove();
		m_node = NULL;
	}
}

// saveData
// save data to serializable
void CGameObject::saveData( CSerializable* pObj )
{
	const char* stringObjType[] = 
	{
		"NullObject",
		"ZoneObject",
		"CubeObject",
		"AnimObject"		
	};

	pObj->addRow("objectID",	&m_objectID, NSSerializable::longType, true);
	pObj->addRow("objectType",	(void*)stringObjType[ (int)m_objectType ], NSSerializable::stringType, true);
	
	pObj->addRow("enable",		&m_enable,	NSSerializable::boolType);
	pObj->addRow("visible",		&m_visible, NSSerializable::boolType);

	pObj->addRow("positionX",	&m_position.X, NSSerializable::floatType );
	pObj->addRow("positionY",	&m_position.Y, NSSerializable::floatType );
	pObj->addRow("positionZ",	&m_position.Z, NSSerializable::floatType );
	
	pObj->addRow("rotationX",	&m_rotation.X, NSSerializable::floatType );
	pObj->addRow("rotationY",	&m_rotation.Y, NSSerializable::floatType );
	pObj->addRow("rotationZ",	&m_rotation.Z, NSSerializable::floatType );

	pObj->addRow("scaleX",		&m_scale.X, NSSerializable::floatType );
	pObj->addRow("scaleY",		&m_scale.Y, NSSerializable::floatType );
	pObj->addRow("scaleZ",		&m_scale.Z, NSSerializable::floatType );		
}

// loadData
// load data to serializable
void CGameObject::loadData( CSerializable* pObj )
{
}