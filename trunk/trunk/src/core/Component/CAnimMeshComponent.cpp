#include "stdafx.h"
#include "CAnimMeshComponent.h"
#include "IView.h"

CAnimMeshComponent::CAnimMeshComponent( CGameObject *pObj )
	:IObjectComponent( pObj, (int)IObjectComponent::AnimMesh )
{
	m_animNode = NULL;
	m_animSpeed = 24.0f;	
}

CAnimMeshComponent::~CAnimMeshComponent()
{
}

// loadFromFile
// load anim object from file
void CAnimMeshComponent::loadFromFile( char *lpFilename )
{
	if ( m_animeshFile == lpFilename )
		return;

	m_animeshFile = lpFilename;

	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	ISceneManager *smgr = getIView()->getSceneMgr();
	
	// load mesh
	IAnimatedMesh *animMesh = smgr->getMesh( lpFilename );
	if ( animMesh == NULL )
		return;

	// turn off lighting
	int numMesh = animMesh->getMeshBufferCount();
	for ( int i = 0; i < numMesh; i++ )
		animMesh->getMeshBuffer(i)->getMaterial().Lighting = m_gameObject->isLighting();
	
	// create scene node
	m_animNode = new CGameAnimatedMeshSceneNode( m_gameObject, animMesh, m_gameObject->getParentSceneNode(), smgr );	
	m_animNode->setAnimationSpeed( m_animSpeed );	

#ifdef GSEDITOR	
	// add collision
	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox(m_animNode);
	m_animNode->setTriangleSelector(selector);
	selector->drop();
#endif

	// set node
	m_gameObject->m_node = m_animNode;

	// set visibke
	m_gameObject->m_node->setVisible( true );	

	// update position
	m_gameObject->updateNodePosition();
	m_gameObject->updateNodeRotation();
}


// init
// run when init object
void CAnimMeshComponent::initComponent()
{

}

// updateObject
// update object by frame
void CAnimMeshComponent::updateComponent()
{
	
}

// saveData
// save data to serializable
void CAnimMeshComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	pObj->addPath("meshFile", m_animeshFile.c_str(), true);
	pObj->addFloat("animSpeed", m_animSpeed, true );
}

// loadData
// load data to serializable
void CAnimMeshComponent::loadData( CSerializable* pObj )
{		
	pObj->nextRecord();

	// read mesh file
	char *string = pObj->readString();
	loadFromFile( string );

	// read anim speed
	m_animSpeed = pObj->readFloat();
	if ( m_animNode )
		m_animNode->setAnimationSpeed( m_animSpeed );
}