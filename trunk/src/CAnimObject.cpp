#include "stdafx.h"
#include "CAnimObject.h"
#include "IView.h"

CAnimObject::CAnimObject()
{
	m_animNode = NULL;
	m_objectType = CGameObject::AnimObject;
}

CAnimObject::~CAnimObject()
{
}

// loadFromFile
// load anim object from file
void CAnimObject::loadFromFile( char *lpFilename )
{
	m_animeshFile = lpFilename;

	// release if mesh is loaded
	if ( m_node )
		destroyNode();

	ISceneManager *smgr = getIView()->getSceneMgr();
	
	// load mesh
	IAnimatedMesh *animMesh = smgr->getMesh( lpFilename );

	// create scene node
	m_animNode = new CGameAnimatedMeshSceneNode( this, animMesh, smgr->getRootSceneNode(), smgr );	
	m_animNode->setAnimationSpeed(20.f);	

	// set node
	m_node = m_animNode;

	// update position
	updateNodePosition();
	updateNodeRotation();
}

// updateObject
// update object by frame
void CAnimObject::updateObject()
{
	// get line ray
	/*
	scene::ISceneCollisionManager* collMan = getIView()->getSceneMgr()->getSceneCollisionManager();
	
	core::vector2di point( getIView()->m_mouseX, getIView()->m_mouseY );
	core::line3d<f32> ray = collMan->getRayFromScreenCoordinates( point, getIView()->getSceneMgr()->getActiveCamera() );
		
	// plane oxz
	core::plane3df plane( 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );

	core::vector3df hit;
	bool b = plane.getIntersectionWithLine( ray.start, ray.getVector(), hit );
	
	if ( b )
	{
		//setOrientation( hit );
		lookAt( hit );
	}
	*/
}

// saveData
// save data to serializable
void CAnimObject::saveData( CSerializable* pObj )
{
	CGameObject::saveData( pObj );

	// save mesh file
	pObj->addString("meshFile", m_animeshFile.c_str());
}

// loadData
// load data to serializable
void CAnimObject::loadData( CSerializable* pObj )
{
	CGameObject::loadData( pObj );

	// read mesh file
	char *string = pObj->readString();
	loadFromFile( string );
}