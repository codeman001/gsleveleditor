#include "stdafx.h"
#include "CBoxObjectComponent.h"
#include "CGameBoxSceneNode.h"
#include "IView.h"

CBoxObjectComponent::CBoxObjectComponent(CGameObject *pObj )
	:IObjectComponent( pObj, IObjectComponent::BoxObject )
{

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
}

// loadData
// load data to serializable
void CBoxObjectComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	m_size			= pObj->readFloat();
	m_textureFile	= pObj->readString();

	createBox();
}