#include "stdafx.h"
#include "IView.h"
#include "CGameBillboardSceneNode.h"
#include "CLightingComponent.h"

CLightingComponent::CLightingComponent(CGameObject *pObj)
	:IObjectComponent( pObj, IObjectComponent::Lighting )
{

}

CLightingComponent::~CLightingComponent()
{
}

// init
// run when init object
void CLightingComponent::initComponent()
{
	ISceneNode *sceneNode =	m_gameObject->getSceneNode();		
	
	ISceneManager *smgr = getIView()->getSceneMgr();

	if ( sceneNode == NULL )
	{
#ifdef GSEDITOR
		ISceneNode *node = new CGameBillboardSceneNode
			( 
				m_gameObject, m_gameObject->getParentSceneNode(), 
				smgr, 
				m_gameObject->getID(), 
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
		m_gameObject->m_node = node;
		m_gameObject->m_node->setVisible( true );	
		m_gameObject->setLighting( false );

		// update position
		m_gameObject->updateNodePosition();
		m_gameObject->updateNodeRotation();		
#else
		ISceneNode *emtyNode = smgr->addEmptySceneNode( m_gameObject->getParentSceneNode() );
		emtyNode->grab();
		m_gameObject->m_node = emtyNode;

		// update position
		m_gameObject->updateNodePosition();
		m_gameObject->updateNodeRotation();	
#endif
	}
	
	// add light scenenode
	m_lightSceneNode = smgr->addLightSceneNode( m_gameObject->m_node );

	// setting light data
	video::SLight &light = m_lightSceneNode->getLightData();
	light.Type			= ELT_POINT;
	light.DiffuseColor	= m_diffuseColor;
	light.SpecularColor = m_specularColor;
	light.Radius		= m_radius;
	light.Attenuation.X	= 0.0f;
	light.Attenuation.Y	= 1.0f/(m_radius * m_strength);
	light.Attenuation.Z	= 0.0f;	

}

// update
// run when update per frame
void CLightingComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CLightingComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );
			
	std::string diffuseColor = getColorString(m_diffuseColor);
	std::string specularColor = getColorString(m_specularColor);

	pObj->addString("diffuseColor", diffuseColor.c_str(), true);
	pObj->addString("specularColor",specularColor.c_str(), true);
	pObj->addFloat("strength", m_strength, true);
	pObj->addFloat("radius", m_radius, true);
}

// loadData
// load data to serializable
void CLightingComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	std::string diffuseColor	= pObj->readString();
	std::string specularColor	= pObj->readString();
	m_strength = pObj->readFloat();
	m_radius = pObj->readFloat();

	m_diffuseColor = setColorString(diffuseColor);
	m_specularColor = setColorString(specularColor);

}

// getColorString
// get string hexa color name from SColor
std::string CLightingComponent::getColorString( const SColor& color )
{
	char lpColor[512];
	sprintf(lpColor, "%02x%02x%02x",color.getRed(), color.getGreen(), color.getBlue() );
	return std::string(lpColor);
}

// setColorString
// get SColor from string hexa name
SColor CLightingComponent::setColorString( const std::string& stringColor )
{
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