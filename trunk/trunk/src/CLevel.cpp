#include "stdafx.h"
#include "CLevel.h"

#include "scenenode/CGameOxyzSceneNode.h"


CLevel::CLevel()
{
	init();
}

CLevel::~CLevel()
{	
}

void CLevel::init()
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	CGameOxyzSceneNode *oxyz = new CGameOxyzSceneNode( smgr->getRootSceneNode(), smgr, -1 );
	oxyz->drop();

	ICameraSceneNode *cam =	smgr->addCameraSceneNode(smgr->getRootSceneNode(), core::vector3df(-500, 500, -500), core::vector3df(0, 0, 0) );
	cam->setFOV( core::degToRad(60.0f) );	
}

void CLevel::update()
{
}

void CLevel::render()
{
}