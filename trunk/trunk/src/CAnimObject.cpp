#include "stdafx.h"
#include "CAnimObject.h"
#include "IView.h"

CAnimObject::CAnimObject()
{
}

CAnimObject::~CAnimObject()
{
}

// loadFromFile
// load anim object from file
void CAnimObject::loadFromFile( char *lpFilename )
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	scene::IAnimatedMeshSceneNode* animNode = smgr->addAnimatedMeshSceneNode( smgr->getMesh( lpFilename ) );
	animNode->setAnimationSpeed(20.f);
	
	m_node = animNode;
	updateNodePosition();
	updateNodeRotation();
}

// updateObject
// update object by frame
void CAnimObject::updateObject()
{
	// get line ray	
	scene::ISceneCollisionManager* collMan = getIView()->getSceneMgr()->getSceneCollisionManager();
	
	core::vector2di point( getIView()->m_mouseX, getIView()->m_mouseY );
	core::line3d<f32> ray = collMan->getRayFromScreenCoordinates( point, getIView()->getSceneMgr()->getActiveCamera() );
		
	// plane oxz
	core::plane3df plane( 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );

	core::vector3df hit;
	bool b = plane.getIntersectionWithLine( ray.start, ray.getVector(), hit );
	
	if ( b )
	{
		setOrientation( hit );
		//lookAt( hit );
	}
}