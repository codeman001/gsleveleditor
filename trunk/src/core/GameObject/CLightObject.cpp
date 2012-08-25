#include "stdafx.h"
#include "IView.h"
#include "CZone.h"
#include "CGameBillboardSceneNode.h"
#include "CLightObject.h"

#pragma region LIGHTOBJECT

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

#ifdef GSEDITOR
	m_direction = m_directionObj->getPosition();
#endif
	pObj->addFloat("directionX",m_direction.X);	
	pObj->addFloat("directionY",m_direction.Y);
	pObj->addFloat("directionZ",m_direction.Z);

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
	m_direction.X = pObj->readFloat();
	m_direction.Y = pObj->readFloat();
	m_direction.Z = pObj->readFloat();
	CGameObject::loadData( pObj );

#ifdef GSEDITOR
	m_directionObj->setPosition( m_direction );
#endif

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
		
	setLightOrientation( ( m_direction - getPosition() ).normalize() );	
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

#ifdef GSEDITOR
	m_direction = m_directionObj->getPosition();
#endif

	pObj->addFloat("directionX",m_direction.X);	
	pObj->addFloat("directionY",m_direction.Y);
	pObj->addFloat("directionZ",m_direction.Z);

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
	m_direction.X = pObj->readFloat();
	m_direction.Y = pObj->readFloat();
	m_direction.Z = pObj->readFloat();

	CGameObject::updateData( pObj );

#ifdef GSEDITOR
	m_directionObj->setPosition( m_direction );
#endif

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
	
	setLightOrientation( ( m_direction - getPosition() ).normalize() );	
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
	sprintf(lpColor, "%02x%02x%02x", color.getRed(), color.getGreen(), color.getBlue() );
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

// setLightOrientation
// set light dir rotation
void CLightObject::setLightOrientation( const core::vector3df& v )
{
	core::vector3df front = v;
	core::vector3df up(0,1,0);

	front.normalize();

	core::vector3df right = up.crossProduct(front);
	up = front.crossProduct(right);

	f32 matData[16];

	matData[ 0] = right.X;
	matData[ 1] = right.Y;
	matData[ 2] = right.Z;
	matData[ 3] = 0.0f;

	matData[ 4] = up.X;
	matData[ 5] = up.Y;
	matData[ 6] = up.Z;
	matData[ 7] = 0.0f;

	matData[ 8] = front.X;
	matData[ 9] = front.Y;
	matData[10] = front.Z;
	matData[11] = 0.0f;

	matData[12] = 0.0f;
	matData[13] = 0.0f;
	matData[14] = 0.0f;
	matData[15] = 1.0f;

	core::matrix4 rotationMatrix;
	rotationMatrix.setM(matData);

	core::vector3df rotation = rotationMatrix.getRotationDegrees();
	m_lightSceneNode->setRotation( rotation );
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


#pragma endregion



#pragma region LIGHTMANAGER

CLightManager::CLightManager()
{
}

CLightManager::~CLightManager()
{
}

// OnPreRender
// This is called before the first scene node is rendered.
void CLightManager::OnPreRender(core::array<ILightSceneNode*> &lightList)
{
	m_listLightOnMap = &lightList;
}

// OnPostRender
// Called after the last scene node is rendered.
void CLightManager::OnPostRender(void)
{
	// turn on all light
	for(u32 i = 0, n = m_listLightOnMap->size(); i < n; i++)
		(*m_listLightOnMap)[i]->setVisible(true);
}

// begin a renderpass
void CLightManager::OnRenderPassPreRender(E_SCENE_NODE_RENDER_PASS renderPass)
{
	m_renderPass = renderPass;
}

// end a renderpass
void CLightManager::OnRenderPassPostRender(E_SCENE_NODE_RENDER_PASS renderPass)
{

}

// OnNodePreRender
// This is called before the specified scene node is rendered
void CLightManager::OnNodePreRender(ISceneNode* node)
{
	if ( scene::ESNRP_SOLID != m_renderPass )
		return;

	if ( node->isEnablePointLight() == false && node->isEnableDirectionLight() == false )
		return;
		
	// get node pos
	const core::vector3df nodePosition = node->getAbsolutePosition();

	// get light
	core::array<LightDistanceElement> sortingPointLightArray;
	core::array<LightDistanceElement> sortingDirectionLightArray;

	// init list light
	sortingPointLightArray.reallocate(m_listLightOnMap->size());
	sortingDirectionLightArray.reallocate(m_listLightOnMap->size());

	u32 i, n;
	for(i = 0, n = m_listLightOnMap->size(); i < n; ++i)
	{
		scene::ILightSceneNode* lightNode = (*m_listLightOnMap)[i];
		video::SLight &lightData = lightNode->getLightData();
		
		// turn off all light
		lightNode->setVisible( false );

		if ( lightData.Type == video::ELT_DIRECTIONAL && node->isEnableDirectionLight() )
		{
			f64 distance = lightNode->getAbsolutePosition().getDistanceFromSQ(nodePosition);
			sortingDirectionLightArray.push_back(LightDistanceElement(lightNode, distance));
		}
		else if ( lightData.Type == video::ELT_POINT && node->isEnablePointLight() )
		{
			f64 distance = lightNode->getAbsolutePosition().getDistanceFromSQ(nodePosition);
			sortingPointLightArray.push_back(LightDistanceElement(lightNode, distance));
		}				
	}

	sortingPointLightArray.sort();
	sortingDirectionLightArray.sort();

	// turn on avaliable point light
	for(i = 0, n = sortingPointLightArray.size(); i < n && i < 4; ++i)
		sortingPointLightArray[i].node->setVisible(true);
	
	// turn on avaliable direction light
	for(i = 0, n = sortingDirectionLightArray.size(); i < n && i < 2; ++i)
		sortingDirectionLightArray[i].node->setVisible(true);
}
	
// OnNodePostRender
// Called after the specified scene node is rendered
void CLightManager::OnNodePostRender(ISceneNode* node)
{
}

#pragma endregion