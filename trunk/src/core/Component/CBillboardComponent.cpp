#include "stdafx.h"
#include "CBillboardComponent.h"
#include "CGameBillboardSceneNode.h"
#include "IView.h"

CBillboardComponent::CBillboardComponent(CGameObject *pObj)
	:IObjectComponent( pObj, IObjectComponent::Billboard )
{

}

CBillboardComponent::~CBillboardComponent()
{
}

// createBillboard
// create billboard scenenode
void CBillboardComponent::createBillboard()
{
	if ( m_textureFile.size() == 0 )
		return;

	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	ISceneManager *smgr = getIView()->getSceneMgr();
	IVideoDriver *driver = getIView()->getDriver();

	// create box
	ISceneNode *node = new CGameBillboardSceneNode( m_gameObject, smgr->getRootSceneNode(), smgr, m_gameObject->getID(), core::vector3df(0,0,0), m_billboardSize );
	
	ITexture *pTex = driver->getTexture( m_textureFile.c_str() );

#ifdef GSEDITOR
	if ( pTex == NULL )
	{
		WCHAR appPath[MAX_PATH];
		char  appPathA[MAX_PATH];

		uiApplication::getAppPath(appPath, MAX_PATH);
		uiString::copy<char, WCHAR>( appPathA, appPath  );
		
		std::string path = appPathA;
		path += "\\";
		path += m_textureFile;
		
		pTex = driver->getTexture(path.c_str());
	}
#endif

	node->setMaterialTexture(0, pTex );
	node->setMaterialFlag(	video::EMF_LIGHTING, false );
	node->setMaterialType(	video::EMT_TRANSPARENT_ADD_COLOR);

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

// init
// run when init object
void CBillboardComponent::initComponent()
{
}

// update
// run when update per frame
void CBillboardComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CBillboardComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );	
	pObj->addPath("textureFile", m_textureFile.c_str(), true);
	pObj->addFloat("billboardWidth", m_billboardSize.Width, true );
	pObj->addFloat("billboardHeight", m_billboardSize.Height, true );
}

// loadData
// load data to serializable
void CBillboardComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	// read mesh file
	m_textureFile = pObj->readString();	
	m_billboardSize.Width	= pObj->readFloat();
	m_billboardSize.Height	= pObj->readFloat();

	createBillboard();
}