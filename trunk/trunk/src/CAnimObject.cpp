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
	//setOrientation( core::vector3df( 0.0f, 0.0f, 1.0f ) );
}