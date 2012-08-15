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

		m_gameObject->m_node = node;
		m_gameObject->m_node->setVisible( true );	
		m_gameObject->setLighting( false );

		// update position
		m_gameObject->updateNodePosition();
		m_gameObject->updateNodeRotation();		
#else
#endif
	}
	else
	{
		
	}
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
}

// loadData
// load data to serializable
void CLightingComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
}