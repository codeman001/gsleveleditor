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
		ISceneNode *emtyNode = smgr->addEmptySceneNode( this->getParentSceneNode() );
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
	sprintf(lpColor, "%x%x%x%x", color.getAlpha(), color.getRed(), color.getGreen(), color.getBlue() );
	return std::string(lpColor);
}

// setColorString
// get SColor from string hexa name
SColor CLightingComponent::setColorString( const std::string& stringColor )
{
	std::string hexa = std::string("0x") + stringColor;
	irr::u32 color = 0;
	sscanf(hexa.c_str(),"%x", &color);
	return SColor (color);
}