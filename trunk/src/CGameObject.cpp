#include "stdafx.h"
#include "CGameObject.h"
#include "CComponentFactory.h"
#include "IView.h"

core::vector3df CGameObject::s_ox	= core::vector3df(1.0f, 0.0f, 0.0f);
core::vector3df CGameObject::s_oy	= core::vector3df(0.0f, 1.0f, 0.0f);
core::vector3df CGameObject::s_oz	= core::vector3df(0.0f, 0.0f, 1.0f);

const char* CGameObject::s_stringObjType[] =
{
	"NullObject",
	"ZoneObject",
	"CubeObject",
	"AnimObject"
};

CGameObject::CGameObject()
{
	m_objectID		= -1;
	m_objectType	= GameObject;
	m_objectState	= Normal;

	m_position		= core::vector3df(0.0f, 0.0f, 0.0f );
	m_rotation		= core::vector3df(0.0f, 0.0f, 0.0f );
	m_scale			= core::vector3df(1.0f, 1.0f, 1.0f );

	m_right			= CGameObject::s_ox;
	m_front			= CGameObject::s_oz;
	m_up			= CGameObject::s_oy;

	m_node			= NULL;
	
	m_enable		= true;
	m_visible		= true;

	m_parent		= NULL;
}

CGameObject::~CGameObject()
{
	if ( m_node )
		m_node->drop();

	ArrayComponentIter iComp = m_components.begin(), iEnd = m_components.end();
	while ( iComp != iEnd )
	{
		delete (*iComp);
	}
	m_components.clear();
}

// setRotation
// set 3d rotation
void CGameObject::setRotation(core::vector3df& rot)
{
	m_rotation		=	rot;

	//fixRotationVector();

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

// fixRotationVector
// fix rotation angle from 0 -> 360
void CGameObject::fixRotationVector()
{

}

// updateRotation
void CGameObject::updateRotation()
{
	//fixRotationVector();

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

// updateNodeRotation
// update irr node rotation
void CGameObject::updateNodeRotation()
{
	if ( m_node )
	{	
		// flip ox
		core::vector3df rot(m_rotation);
		rot.Y += 180.f;

		m_node->setRotation( rot );
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

// updateNodeScale
// update irr node scale
void CGameObject::updateNodeScale()
{
	if ( m_node )
	{
		m_node->setScale( m_scale );
	}
}

// destroyNode
// remove node on scene
void CGameObject::destroyNode()
{
	if ( m_node )
	{
		m_node->drop();
		m_node->remove();
		m_node = NULL;
	}
}

// saveData
// save data to serializable
void CGameObject::saveData( CSerializable* pObj )
{
	pObj->addGroup	("Game object base");

	pObj->addLong	("objectID",	m_objectID, true);
	pObj->addString	("objectType",	s_stringObjType[ (int)m_objectType ], true);
	
	pObj->addBool	("enable",		m_enable );
	pObj->addBool	("visible",		m_visible );

	pObj->addFloat	("positionX",	m_position.X );
	pObj->addFloat	("positionY",	m_position.Y );
	pObj->addFloat	("positionZ",	m_position.Z );
	
	pObj->addFloat	("rotationX",	m_rotation.X );
	pObj->addFloat	("rotationY",	m_rotation.Y );
	pObj->addFloat	("rotationZ",	m_rotation.Z );

	pObj->addFloat	("scaleX",		m_scale.X );
	pObj->addFloat	("scaleY",		m_scale.Y );
	pObj->addFloat	("scaleZ",		m_scale.Z );

	// save component
	ArrayComponentIter iComp = m_components.begin(), iEnd = m_components.end();
	while ( iComp != iEnd )
	{
		(*iComp)->saveData( pObj );
	}

}

// loadData
// load data to serializable
void CGameObject::loadData( CSerializable* pObj )
{	
	pObj->nextRecord();

	m_objectID	= pObj->readLong();

	char *type = pObj->readString();
	for ( int i = 0; i < CGameObject::NumObject; i++ )
	{
		if ( strcmp(s_stringObjType[i], type) == 0 )
		{
			m_objectType = (CGameObject::ObjectType)i;
			break;
		}
	}

	m_enable		= pObj->readBool();
	m_visible		= pObj->readBool();

	m_position.X	= pObj->readFloat();
	m_position.Y	= pObj->readFloat();
	m_position.Z	= pObj->readFloat();

	m_rotation.X	= pObj->readFloat();
	m_rotation.Y	= pObj->readFloat();
	m_rotation.Z	= pObj->readFloat();

	m_scale.X		= pObj->readFloat();
	m_scale.Y		= pObj->readFloat();
	m_scale.Z		= pObj->readFloat();	
	
	
	IObjectComponent *pComp = NULL;
	pComp = CComponentFactory::loadComponent( this, pObj );

	while ( pComp ) 
	{
		// add component
		m_components.push_back( pComp );

		// continue load another component
		pComp = CComponentFactory::loadComponent( this, pObj );
	}
}

// saveTransform
// save all transform
void CGameObject::saveTransform()
{
	m_oldPosition	= m_position;
	m_oldRotation	= m_rotation;
	m_oldScale		= m_scale;
}

// loadTransform
// load all transform
void CGameObject::loadTransform()
{
	m_position		= m_oldPosition;
	m_rotation		= m_oldRotation;
	m_scale			= m_oldScale;		
}

// drawOXYZ
// draw oxyz
void CGameObject::drawFrontUpLeftVector()
{
	if ( m_node == NULL )
		return;

	IVideoDriver* driver = getIView()->getDriver();
	
	core::vector3df bBoxLength =	m_node->getBoundingBox().MaxEdge - m_node->getBoundingBox().MinEdge;

	float length = -1;	
	if ( length < 0 )
		length = bBoxLength.getLength();
	
	// set material
	SMaterial debug_mat;	
	debug_mat.Lighting = false;
	debug_mat.AntiAliasing=0;
	driver->setMaterial(debug_mat);

	// reset transform
	core::matrix4 mat;
	mat.makeIdentity();
	driver->setTransform(video::ETS_WORLD, mat);

	// draw up
	driver->draw3DLine( m_position, m_position + m_up * length,		SColor(255, 255, 0, 0) );

	// draw front
	driver->draw3DLine( m_position, m_position + m_front * length,	SColor(255, 0, 255, 0) );

	// draw right
	driver->draw3DLine( m_position, m_position + m_right * length,	SColor(255, 0, 0, 255) );

}

// drawRotationCircle
// draw circle around object
void CGameObject::drawCircleAroundObject()
{
	if ( m_node == NULL )
		return;

	IVideoDriver* driver = getIView()->getDriver();
	
	core::vector3df bBoxLength	=	m_node->getBoundingBox().MaxEdge - m_node->getBoundingBox().MinEdge;
	
	float radius = -1.0f;
	if ( radius <= 0 )
		radius = bBoxLength.getLength()/2;

	float height =	m_position.Y + m_node->getBoundingBox().getCenter().Y;

	// set material
	SMaterial debug_mat;	
	debug_mat.Lighting = false;
	debug_mat.AntiAliasing=0;
	driver->setMaterial(debug_mat);

	// reset transform
	core::matrix4 mat;
	mat.makeIdentity();
	driver->setTransform(video::ETS_WORLD, mat);

	// draw circle
	int		step = 30;
	float	rad = 0;
	float	radInc = core::PI * 2/step;
	
	core::vector3df	point1, point2;


	for ( int i = 0; i < step; i++ )
	{
		point1.Y = height;
		point1.X = radius * sin( rad );
		point1.Z = radius * cos( rad );

		rad = rad + radInc;

		point2.Y = height;
		point2.X = radius * sin( rad );
		point2.Z = radius * cos( rad );

		driver->draw3DLine
			( 
				m_position + point1,
				m_position + point2,
				SColor(255, 255, 255, 0)
			);
	}

}

// updateObject
// update object by frame...
void CGameObject::updateObject()
{
	ArrayComponentIter iComp = m_components.begin(), iEnd = m_components.end();
	while ( iComp != iEnd )
	{
		(*iComp)->updateComponent();
	}
}