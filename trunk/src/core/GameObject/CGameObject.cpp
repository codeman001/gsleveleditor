#include "stdafx.h"
#include "CGameObject.h"
#include "CComponentFactory.h"
#include "IView.h"

long CGameObject::s_objectID = 1;

core::vector3df CGameObject::s_ox	= core::vector3df(1.0f, 0.0f, 0.0f);
core::vector3df CGameObject::s_oy	= core::vector3df(0.0f, 1.0f, 0.0f);
core::vector3df CGameObject::s_oz	= core::vector3df(0.0f, 0.0f, 1.0f);

const char* CGameObject::s_stringObjType[] =
{
	"GameObject",
	"TerrainObject",
	"ZoneObject",
	"Trigger",
	"Cinematic",
	"NumObject"
};

CGameObject::CGameObject()
{
	m_objectID		= -1;
	m_name			= L"noNameObj";
	m_objectTemplate= L"";
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
	m_lighting		= true;

	m_parent		= NULL;

	m_needSortComponent	= true;

#ifdef GSEDITOR
	m_treeItem		= NULL;
	m_uiVisible		= true;
#endif
}

CGameObject::~CGameObject()
{
	if ( m_node )
		m_node->drop();

	releaseAllComponent();
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
	pObj->addGroup	("Game object");

	pObj->addLong	("objectID",	m_objectID, true);
	pObj->addString	("objectType",	s_stringObjType[ (int)m_objectType ], true);

	char lpText[1024] = {0};
	uiString::convertUnicodeToUTF8( (unsigned short*)m_name.c_str(), lpText );
	pObj->addString	("objectName",	lpText, false);

	uiString::convertUnicodeToUTF8( (unsigned short*)m_objectTemplate.c_str(), lpText );
	pObj->addString	("objectTemplate",	lpText, true);

	pObj->addBool	("enable",		m_enable );
	pObj->addBool	("visible",		m_visible );
	pObj->addBool	("lighting",	m_lighting );

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
		iComp++;
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
	
	wchar_t lpText[1024] = {0};

	// read obj name	
	uiString::convertUTF8ToUnicode( pObj->readString(), (unsigned short*)lpText );
	setName( lpText );

	// read template	
	uiString::convertUTF8ToUnicode( pObj->readString(), (unsigned short*)lpText );
	m_objectTemplate = lpText;

	m_enable		= pObj->readBool();
	m_visible		= pObj->readBool();
	m_lighting		= pObj->readBool();

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

// getData
// get basic data to serializable
void CGameObject::getData( CSerializable* pObj )
{
	pObj->addGroup	("Game object");

	pObj->addLong	("objectID",	m_objectID, true);
	pObj->addString	("objectType",	s_stringObjType[ (int)m_objectType ], true);

	char lpText[1024] = {0};
	uiString::convertUnicodeToUTF8( (unsigned short*)m_name.c_str(), lpText );
	pObj->addString	("objectName",	lpText, false);

	uiString::convertUnicodeToUTF8( (unsigned short*)m_objectTemplate.c_str(), lpText );
	pObj->addString	("objectTemplate",	lpText, true);

	pObj->addBool	("enable",		m_enable );
	pObj->addBool	("visible",		m_visible );
	pObj->addBool	("lighting",	m_lighting);

	pObj->addFloat	("positionX",	m_position.X );
	pObj->addFloat	("positionY",	m_position.Y );
	pObj->addFloat	("positionZ",	m_position.Z );
	
	pObj->addFloat	("rotationX",	m_rotation.X );
	pObj->addFloat	("rotationY",	m_rotation.Y );
	pObj->addFloat	("rotationZ",	m_rotation.Z );

	pObj->addFloat	("scaleX",		m_scale.X );
	pObj->addFloat	("scaleY",		m_scale.Y );
	pObj->addFloat	("scaleZ",		m_scale.Z );
}

// updateData
// update data 
void CGameObject::updateData( CSerializable* pObj )
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
	
	wchar_t lpText[1024] = {0};

	// read obj name	
	uiString::convertUTF8ToUnicode( pObj->readString(), (unsigned short*)lpText );
	setName( lpText );

	// read template	
	uiString::convertUTF8ToUnicode( pObj->readString(), (unsigned short*)lpText );
	m_objectTemplate = lpText;

	m_enable		= pObj->readBool();
	m_visible		= pObj->readBool();
	m_lighting		= pObj->readBool();

	m_position.X	= pObj->readFloat();
	m_position.Y	= pObj->readFloat();
	m_position.Z	= pObj->readFloat();

	m_rotation.X	= pObj->readFloat();
	m_rotation.Y	= pObj->readFloat();
	m_rotation.Z	= pObj->readFloat();

	m_scale.X		= pObj->readFloat();
	m_scale.Y		= pObj->readFloat();
	m_scale.Z		= pObj->readFloat();
	
	setID( m_objectID );
	setEnable( m_enable );
	setVisible( m_visible );
	setLighting( m_lighting );

	updateRotation();

	updateNodePosition();
	updateNodeScale();
}

void CGameObject::setLighting( bool b )
{
	m_node->setMaterialFlag( video::EMF_LIGHTING, b );
	m_lighting = b;
}

// releaseAllComponent
// delete all component on object
void CGameObject::releaseAllComponent()
{
	ArrayComponentIter iComp = m_components.begin(), iEnd = m_components.end();
	while ( iComp != iEnd )
	{
		delete (*iComp);
		iComp++;
	}
	m_components.clear();
}

// initComponent
// create a component on object
void CGameObject::createComponent( CSerializable* componentData )
{
	IObjectComponent *pComp = NULL;

	int pos = componentData->getCursorRecord();
	
	pComp = CComponentFactory::loadComponent( this, componentData );

	while ( pComp )
	{
		// add component
		m_components.push_back( pComp );

		// continue load another component
		pComp = CComponentFactory::loadComponent( this, componentData );
	}

	componentData->setCursorRecord( pos );	
}

// initComponent
// create a component on object
void CGameObject::initComponent()
{
	// sort component
	sortComponentById();

	// call init all component
	ArrayComponentIter i = m_components.begin(), end = m_components.end();
	while ( i != end )
	{
		(*i)->initComponent();
		i++;
	}
}

// sortComponentById
// sort array component
void CGameObject::sortComponentById()
{
	int len = (int)m_components.size();
	
	IObjectComponent *obj;
		
	for (int i = 1; i <= len - 1; i++)
	{
		obj = m_components[i];
		
		int j = i - 1;
		int done = 0;

		do
		{
			if ( m_components[j]->getComponent() > obj->getComponent() )
			{
				m_components[j + 1] = m_components[j];

				j = j - 1;
				if ( j < 0 )
					done = 1;
			}
			else
				done = 1;
		}
		while (done == 0);
		
		m_components[j + 1] = obj;		
	}

	m_needSortComponent = false;
}

// getComponent
// get component
IObjectComponent *CGameObject::getComponent( int componentID )
{
	if ( m_needSortComponent == true )
		sortComponentById();

	int first = 0, last = m_components.size() - 1;
	int mid = 0;
	
	while (first <= last) 
	{
		mid = (first + last) / 2;
		
		if ( componentID > m_components[mid]->getComponent() )
			first = mid + 1;
		else if ( componentID < m_components[mid]->getComponent() )
			last = mid - 1;
		else		
			return m_components[mid];
   }

	return NULL;

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

#ifdef GSEDITOR

// isLineHit
// mouse x, y is hit on line x1y1 - x2y2
bool CGameObject::isLineHit( int X1, int Y1, int X2, int Y2, int X, int Y )
{
	float x1 = (float)X1;
	float y1 = (float)Y1;

	float x2 = (float)X2;
	float y2 = (float)Y2;

	float xHit = (float)X;
	float yHit = (float)Y;
	
	const float constSelect = 5.0f;

	if ( x1 > x2 )
	{
		if ( xHit < (x2 - constSelect) || xHit > (x1 + constSelect) )
			return false;
	}
	else
	{
		if ( xHit < (x1 - constSelect) || xHit > (x2 + constSelect) )
			return false;
	}
	
	if ( y1 > y2 )
	{
		if ( yHit < (y2 - constSelect) || yHit > (y1 + constSelect) )
			return false;
	}
	else
	{
		if ( yHit < (y1 - constSelect) || yHit > (y2 + constSelect) )
			return false;
	}


	core::vector2df v( x2 - x1, y2 - y1 );
	core::vector2df w( xHit - x1, yHit - y1 );

	#define dot(u,v)   ((float)u.dotProduct(v))
	#define norm(v)    sqrtf(dot(v,v))
			
	float c1 = dot(w,v);
	if ( c1 <= 0 )
	{
		core::vector2df r( xHit - x1, yHit - y1 );
		float distance = norm(r);
		
		if ( distance <= constSelect )
			return true;
	}

	float c2 = dot(v,v);	
	
	if ( c2 <= c1 )
	{
		core::vector2df r( xHit - x2, yHit - y2 );
		float distance = norm(r);
		
		if ( distance <= constSelect )
			return true;
	}

	float b = c1 / c2;
		
	core::vector2df Pb( x1 + b*v.X, y1 + b*v.Y );

	// Khoang cach giua point va edge
	core::vector2df r( xHit - Pb.X, yHit - Pb.Y );
	double distance = norm(r);				

	#undef dot
	#undef norm
	
	if ( distance <= constSelect )
		return true;

	return false;
}

// isHittestObjectVector
// typeVector:	1 ox, 2 oy, 3, oz
bool CGameObject::isHittestObjectVector( int x, int y, int typeVector )
{
	if ( m_node == NULL )
		return false;

	IView *pView = getIView();

	IVideoDriver* driver = pView->getDriver();	
	core::vector3df bBoxLength = m_node->getBoundingBox().MaxEdge - m_node->getBoundingBox().MinEdge;
	float length = bBoxLength.getLength() * 1.5f;

	core::vector3df	ox = m_position + m_right	* length;
	core::vector3df	oy = m_position + m_up		* length;
	core::vector3df	oz = m_position + m_front	* length;

	int x1, y1, x2, y2;
	pView->getScreenCoordinatesFrom3DPosition( m_position, &x1, &y1 );

	if ( typeVector == 1 )
		pView->getScreenCoordinatesFrom3DPosition( ox, &x2, &y2 );
	else if ( typeVector == 2 )
		pView->getScreenCoordinatesFrom3DPosition( oy, &x2, &y2 );
	else if ( typeVector == 3 )
		pView->getScreenCoordinatesFrom3DPosition( oz, &x2, &y2 );
	else
		return false;
	
	return isLineHit( x1, y1, x2, y2, x, y );
}

#endif

// drawOXYZ
// draw oxyz
void CGameObject::drawFrontUpLeftVector()
{
	if ( m_node == NULL )
		return;

	IVideoDriver* driver = getIView()->getDriver();
	
	core::vector3df bBoxLength =	m_node->getBoundingBox().MaxEdge - m_node->getBoundingBox().MinEdge;

	float length = bBoxLength.getLength() * 1.5f;
	
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
		iComp++;
	}
}