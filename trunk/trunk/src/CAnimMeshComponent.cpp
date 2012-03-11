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
	m_animeshFile = lpFilename;

	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	ISceneManager *smgr = getIView()->getSceneMgr();
	
	// load mesh
	IAnimatedMesh *animMesh = smgr->getMesh( lpFilename );

	// create scene node
	m_animNode = new CGameAnimatedMeshSceneNode( m_gameObject, animMesh, smgr->getRootSceneNode(), smgr );	
	m_animNode->setAnimationSpeed( m_animSpeed );	

#ifdef GSEDITOR
	// add collision
	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox(m_animNode);
	m_animNode->setTriangleSelector(selector);
	selector->drop();
#endif

	// set node
	m_gameObject->m_node = m_animNode;

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

	pObj->addPath("meshFile", m_animeshFile.c_str());
	pObj->addFloat("animSpeed", m_animSpeed );
}

// loadData
// load data to serializable
void CAnimMeshComponent::loadData( CSerializable* pObj )
{		
	pObj->nextRecord();

	// read mesh file
	char *string = pObj->readString();
	loadFromFile( string );
}