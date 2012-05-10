#include "stdafx.h"
#include "CStaticMeshComponent.h"
#include "IView.h"

CStaticMeshComponent::CStaticMeshComponent( CGameObject *pObj )
	:IObjectComponent( pObj, (int)IObjectComponent::StaticMesh )
{
	m_meshNode = NULL;
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

// init
// run when init object
void CStaticMeshComponent::initComponent()
{
}

// updateComponent
// update object by frame
void CStaticMeshComponent::updateComponent()
{
}

// loadFromFile
// load anim object from file
void CStaticMeshComponent::loadFromFile( char *lpFilename )
{
	if ( m_meshFile == lpFilename )
		return;

	m_meshFile = lpFilename;

	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	ISceneManager *smgr = getIView()->getSceneMgr();
	
	// load mesh
	IAnimatedMesh *animMesh = smgr->getMesh( lpFilename );	
	
	// copy mesh
	IMeshManipulator *meshManipulator = smgr->getMeshManipulator();
	IMesh *staticMesh =	meshManipulator->createMeshCopy( animMesh->getMesh(0) );	

	// copy texture
	int numMesh = animMesh->getMeshBufferCount();
	for ( int i = 0; i < numMesh; i++ )
	{
		scene::IMeshBuffer* animBuffer		= animMesh->getMeshBuffer(i);
		scene::IMeshBuffer* staticBuffer	= staticMesh->getMeshBuffer(i);
		
		// copy material
		staticBuffer->getMaterial() = animBuffer->getMaterial();
		staticBuffer->getMaterial().Lighting = m_gameObject->isLighting();
		staticBuffer->setHardwareMappingHint( scene::EHM_STATIC );
	}	

	// create scene node
	m_meshNode = new CGameMeshSceneNode( m_gameObject, staticMesh, m_gameObject->getParentSceneNode(), smgr );	
	staticMesh->drop();

#ifdef GSEDITOR
	// add collision
	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox(m_meshNode);
	m_meshNode->setTriangleSelector(selector);
	selector->drop();
#endif

	// set node
	m_gameObject->m_node = m_meshNode;

	// set visibke
	m_gameObject->m_node->setVisible( true );	

	// update position
	m_gameObject->updateNodePosition();
	m_gameObject->updateNodeRotation();
}

// saveData
// save data to serializable
void CStaticMeshComponent::saveData( CSerializable* pObj )
{
	// save mesh file
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	// add path to mesh file
	pObj->addPath("meshFile", m_meshFile.c_str(), true);
}

// loadData
// load data to serializable
void CStaticMeshComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	// read mesh file
	char *string = pObj->readString();
	loadFromFile( string );
}