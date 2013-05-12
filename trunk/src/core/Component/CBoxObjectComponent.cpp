#include "stdafx.h"
#include "CBoxObjectComponent.h"
#include "CGameBoxSceneNode.h"
#include "IView.h"

CBoxObjectComponent::CBoxObjectComponent(CGameObject *pObj )
	:IObjectComponent(pObj, IObjectComponent::BoxObject)
{
	m_colorA = 0xff;
	m_colorR = 0xff;
	m_colorG = 0xff;
	m_colorB = 0xff;
}

CBoxObjectComponent::~CBoxObjectComponent()
{
}

// init
// run when init object
void CBoxObjectComponent::initComponent()
{
}

// update
// run when update per frame
void CBoxObjectComponent::updateComponent()
{
}

// createBox
// create box node
void CBoxObjectComponent::createBox()
{
	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	ISceneManager *smgr = getIView()->getSceneMgr();
	IVideoDriver *driver = getIView()->getDriver();

	// create box
	ISceneNode *node = new CGameBoxSceneNode( m_gameObject, m_size, m_gameObject->getParentSceneNode(), smgr, (s32)m_gameObject->getID()	);
	
	if ( m_textureFile.size() > 0 )
		node->setMaterialTexture(0, driver->getTexture(m_textureFile.c_str()));

	node->setMaterialFlag( video::EMF_LIGHTING, m_gameObject->isLighting() );

#ifdef GSEDITOR
	video::SMaterial& mat = node->getMaterial(0);

	mat.DiffuseColor	= video::SColor(m_colorA, m_colorR,m_colorG,m_colorB);
	mat.AmbientColor	= video::SColor(m_colorA, m_colorR,m_colorG,m_colorB);
	mat.SpecularColor	= video::SColor(m_colorA, m_colorR,m_colorG,m_colorB);

	mat.setFlag( EMF_COLOR_MATERIAL, false );	
	mat.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
#endif

#ifdef GSEDITOR
	// add collision
	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox(node);
	node->setTriangleSelector(selector);
	selector->drop();
#endif
	
	// set visibke
	m_gameObject->m_node = node;
	m_gameObject->m_node->setVisible( true );	

	// update position
	m_gameObject->updateNodePosition();
	m_gameObject->updateNodeRotation();
}

// saveData
// save data to serializable
void CBoxObjectComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );
	
	pObj->addFloat("size", m_size, true );
	pObj->addPath("textureFile", m_textureFile.c_str(), true);	
	
	pObj->addInt("colorA", m_colorA, true);
	pObj->addInt("colorR", m_colorR, true);
	pObj->addInt("colorG", m_colorG, true);
	pObj->addInt("colorB", m_colorB, true);
}

// loadData
// load data to serializable
void CBoxObjectComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	m_size			= pObj->readFloat();
	m_textureFile	= pObj->readString();
	
	m_colorA		= pObj->readInt();
	m_colorR		= pObj->readInt();
	m_colorG		= pObj->readInt();
	m_colorB		= pObj->readInt();

	createBox();
}