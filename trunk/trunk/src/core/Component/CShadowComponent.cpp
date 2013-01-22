#include "stdafx.h"
#include "CShadowComponent.h"
#include "CGameStaticShadowSceneNode.h"

CShadowComponent::CShadowComponent(CGameObject *owner)
	:IObjectComponent( owner, IObjectComponent::Shadow )
{
	
}
	
CShadowComponent::~CShadowComponent()
{
}

// init
// run when init object
void CShadowComponent::initComponent()
{
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();
	
	// new node
	m_gameObject->m_node = new CGameStaticShadowSceneNode(
			m_gameObject,
			m_gameObject->getParentSceneNode(), 
			getIView()->getSceneMgr(), 
			m_gameObject->getPosition(), 
			(s32)m_gameObject->getID() 
		);

#ifdef _IRR_COMPILE_WITH_OGLES2_
	ITexture *pShadowTex = getIView()->getDriver()->getTexture( getIView()->getPath( "data/texture/shadow.pvr" ) );
#else
	ITexture *pShadowTex = getIView()->getDriver()->getTexture( getIView()->getPath( "data/texture/shadow.tga" ) );
#endif

	m_gameObject->m_node->getMaterial(0).setTexture(0, pShadowTex);
	m_gameObject->m_node->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
}

// update
// run when update per frame
void CShadowComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CShadowComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );
}

// loadData
// load data to serializable
void CShadowComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
}