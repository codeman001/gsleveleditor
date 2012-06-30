#include "stdafx.h"
#include "IView.h"
#include "CStateGameplay.h"

CStateGameplay::CStateGameplay()
	:CGameState(CGameState::GSStateGameplay)
{
	m_level = new CGameLevel();
}

CStateGameplay::~CStateGameplay()
{
	delete m_level;
}

void CStateGameplay::onCreate()
{
	ISceneManager *smgr = getIView()->getSceneMgr();

	CGameObject *col = getLevel()->searchObject("levelGameM1Col");

	CColladaMeshComponent *comp = (CColladaMeshComponent*)col->getComponent( IObjectComponent::ColladaMesh );	
	ISceneNode *node = comp->getDefaultNode(0);
	ITriangleSelector *world = node->getTriangleSelector();

	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.0f,1.2f);
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator( world, camera );
	camera->addAnimator(anim);
	camera->setFarValue(10000);
	anim->drop();
}

void CStateGameplay::onDestroy()
{
}

void CStateGameplay::onFsCommand( const char *command, const char *param )
{
}

void CStateGameplay::onUpdate()
{
	m_level->update();
}

void CStateGameplay::onRender()
{
	m_level->render();
}