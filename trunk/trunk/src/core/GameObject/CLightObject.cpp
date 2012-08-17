#include "stdafx.h"
#include "IView.h"
#include "CZone.h"
#include "CGameBillboardSceneNode.h"
#include "CLightObject.h"

CLightObject::CLightObject()
{
	m_objectType = CGameObject::LightObject;
	initLight();
}

CLightObject::CLightObject(CGameObject *parent)
	:CGameObject(parent)
{
	m_objectType = CGameObject::LightObject;		
	initLight();
}

CLightObject::~CLightObject()
{
#ifdef GSEDITOR
	if ( m_directionObj )
	{
		if ( m_directionObj->getParent() == NULL )
			delete m_directionObj;

		m_directionObj = NULL;
	}
#endif
}


void CLightObject::initLight()
{
	// default variable
	m_lightType	= 0;
	m_diffuseColor = SColor(255,255,255,255);
	m_specularColor = SColor(255,255,255,255);;
	m_strength = 1.0f;
	m_radius = 100.0f;

#ifdef GSEDITOR
	m_directionObj = NULL;
#endif

	// create light object
	ISceneManager *smgr = getIView()->getSceneMgr();

#ifdef GSEDITOR
	CGameBillboardSceneNode *node = new CGameBillboardSceneNode
			( 
				this, 
				this->getParentSceneNode(), 
				smgr, 
				-1,
				core::vector3df(0,0,0), 
				core::dimension2df(20.0f, 20.0f) 
			);
		
	node->setMaterialTexture(0, getIView()->getDriver()->getTexture( getIView()->getPath("data/editor/light.tga")) );
	node->setMaterialFlag(	video::EMF_LIGHTING, false );
	node->setMaterialType(	video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	
	// add collision
	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox(node);
	node->setTriangleSelector(selector);
	selector->drop();
					
	// update game object
	m_node = node;
	m_node->setVisible( true );	
	setLighting( false );

	// update position
	updateNodePosition();
	updateNodeRotation();	
	
#else
	ISceneNode *emtyNode = smgr->addEmptySceneNode( this->getParentSceneNode() );
	emtyNode->grab();
	m_node = emtyNode;

	// update position
	updateNodePosition();
	updateNodeRotation();
#endif

	// add light scenenode
	m_lightSceneNode = smgr->addLightSceneNode( m_node );

	// setting light data
	video::SLight &light = m_lightSceneNode->getLightData();
	light.Type = m_lightType == 0? ELT_POINT : ELT_DIRECTIONAL;
	light.DiffuseColor	= m_diffuseColor;
	light.SpecularColor = m_specularColor;
	light.Radius		= m_radius;
	light.Attenuation.X	= 0.0f;
	light.Attenuation.Y	= 1.0f/(m_radius * m_strength);
	light.Attenuation.Z	= 0.0f;


#ifdef GSEDITOR
	CZone *zone = (CZone*)m_parent;
	m_directionObj = new CGameObject( zone );	
	m_directionObj->setID( CGameObject::s_objectID++ );

	if ( zone )
		zone->addChild( m_directionObj );

	// add billboard
	CGameBillboardSceneNode *nodeDirection = new CGameBillboardSceneNode
		(
			m_directionObj, 
			m_directionObj->getParentSceneNode(),
			smgr, 
			m_directionObj->getID(), 
			core::vector3df(0,0,0), 
			core::dimension2df(20.0f, 20.0f) 
		);
	
	nodeDirection->setMaterialTexture(0, getIView()->getDriver()->getTexture( getIView()->getPath("data/editor/lightDirection.bmp")) );
	nodeDirection->setMaterialFlag(	video::EMF_LIGHTING, false );
	nodeDirection->setMaterialType(	video::EMT_TRANSPARENT_ADD_COLOR);
	
	// add collision
	ITriangleSelector *selectorDirection = smgr->createTriangleSelectorFromBoundingBox(nodeDirection);
	nodeDirection->setTriangleSelector(selectorDirection);
	selectorDirection->drop();

	m_directionObj->m_node = nodeDirection;
	m_directionObj->setVisible( false );
	m_directionObj->setLighting(false);
	m_directionObj->setDummyObject(true);
	m_directionObj->updateNodePosition();
	m_directionObj->updateNodeRotation();
#endif

}

// saveData
// save data to serializable ( use for save in game .sav )
void CLightObject::saveData( CSerializable* pObj )
{
	pObj->addGroup	( s_stringObjType[m_objectType] );
	
	std::string diffuseColor = getColorString(m_diffuseColor);
	std::string specularColor = getColorString(m_specularColor);

	pObj->addInt("lightingType", m_lightType);
	pObj->addString("diffuseColor", diffuseColor.c_str());
	pObj->addString("specularColor",specularColor.c_str());
	pObj->addFloat("strength", m_strength);
	pObj->addFloat("radius", m_radius);

	CGameObject::saveData( pObj );
}

// loadData
// load data to serializable ( use for load in game .sav )
void CLightObject::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
	
	m_lightType = pObj->readInt();
	std::string diffuseColor	= pObj->readString();
	std::string specularColor	= pObj->readString();
	m_strength = pObj->readFloat();
	m_radius = pObj->readFloat();

	m_diffuseColor = setColorString(diffuseColor);
	m_specularColor = setColorString(specularColor);

	// setting light data
	video::SLight &light = m_lightSceneNode->getLightData();
	light.Type = m_lightType == 0? ELT_POINT : ELT_DIRECTIONAL;
	light.DiffuseColor	= m_diffuseColor;
	light.SpecularColor = m_specularColor;
	light.Radius		= m_radius;
	light.Attenuation.X	= 0.0f;
	light.Attenuation.Y	= 1.0f/(m_radius * m_strength);
	light.Attenuation.Z	= 0.0f;

	CGameObject::loadData( pObj );
}

// getData
// get basic data to serializable
void CLightObject::getData( CSerializable* pObj )
{
	pObj->addGroup	( s_stringObjType[m_objectType] );
	
	std::string diffuseColor = getColorString(m_diffuseColor);
	std::string specularColor = getColorString(m_specularColor);

	pObj->addInt("lightingType", m_lightType);
	pObj->addString("diffuseColor", diffuseColor.c_str());
	pObj->addString("specularColor",specularColor.c_str());
	pObj->addFloat("strength", m_strength);
	pObj->addFloat("radius", m_radius);

	CGameObject::getData( pObj );
}

// updateData
// update data
void CLightObject::updateData( CSerializable* pObj )
{
	pObj->nextRecord();
	
	m_lightType = pObj->readInt();
	std::string diffuseColor	= pObj->readString();
	std::string specularColor	= pObj->readString();
	m_strength = pObj->readFloat();
	m_radius = pObj->readFloat();

	m_diffuseColor = setColorString(diffuseColor);
	m_specularColor = setColorString(specularColor);


	// setting light data
	video::SLight &light = m_lightSceneNode->getLightData();
	light.Type = m_lightType == 0? ELT_POINT : ELT_DIRECTIONAL;
	light.DiffuseColor	= m_diffuseColor;
	light.SpecularColor = m_specularColor;
	light.Radius		= m_radius;
	light.Attenuation.X	= 0.0f;
	light.Attenuation.Y	= 1.0f/(m_radius * m_strength);
	light.Attenuation.Z	= 0.0f;


	CGameObject::updateData( pObj );
}

// updateObject
// update object by frame...
void CLightObject::updateObject()
{
#ifdef GSEDITOR	
	if ( m_directionObj )
	{
		if ( m_lightType != 0 )
			m_directionObj->setVisible( true );
		else
			m_directionObj->setVisible( false );
	}
#endif
	CGameObject::updateObject();
}


// getColorString
// get string hexa color name from SColor
std::string CLightObject::getColorString( const SColor& color )
{
	char lpColor[512];
	sprintf(lpColor, "%x%x%x%x", color.getAlpha(), color.getRed(), color.getGreen(), color.getBlue() );
	return std::string(lpColor);
}

// setColorString
// get SColor from string hexa name
SColor CLightObject::setColorString( const std::string& stringColor )
{
	unsigned int rgbColor = 0;
	unsigned int _r = 0, _g = 0, _b = 0;

	char strColor[512];
	uiString::copy<char, const char>(strColor, stringColor.c_str());

	uiString::toUpper<char>( strColor );
	int len = uiString::length<char>( strColor );
	
	for ( ;len < 6; len++)
		strColor[len] = '0';
	strColor[len] = 0;

	char rColor[5] = {0};
	rColor[0] = '0';
	rColor[1] = 'x';
	rColor[2] = strColor[0];
	rColor[3] = strColor[1];

	char gColor[5] = {0};
	gColor[0] = '0';
	gColor[1] = 'x';
	gColor[2] = strColor[2];
	gColor[3] = strColor[3];

	char bColor[5] = {0};
	bColor[0] = '0';
	bColor[1] = 'x';
	bColor[2] = strColor[4];
	bColor[3] = strColor[5];

	sscanf( rColor, "%X", &_r );
	sscanf( gColor, "%X", &_g );
	sscanf( bColor, "%X", &_b );
		
	return SColor(0xff, _r, _g, _b);
}

#ifdef GSEDITOR
// drawObject	
void CLightObject::drawObject()
{
	if ( m_visible && m_lightType != 0 )
	{
		IVideoDriver *driver = getIView()->getDriver();

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
		driver->draw3DLine( m_position, m_directionObj->getPosition(), SColor(255, 0, 255, 0) );
	}
}
#endif