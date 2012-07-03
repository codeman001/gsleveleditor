#include "stdafx.h"
#include "IView.h"
#include "CStateGameplay.h"
#include "CGameCameraFollowAnimator.h"

CStateGameplay::CStateGameplay()
	:CGameState(CGameState::GSStateGameplay)
{
	m_level = new CGameLevel();
}

CStateGameplay::~CStateGameplay()
{
	delete m_level;	
	CGameLevel::setCurrentLevel( NULL );
}

void CStateGameplay::onCreate()
{
	ISceneManager *smgr = getIView()->getSceneMgr();
	CGameLevel::setCurrentLevel( m_level );
	
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNode();
	
	CGameObject *player = getLevel()->searchObject("player");
	// create anim
	CGameCameraFollowAnimator *anim = new CGameCameraFollowAnimator( getIView()->getDevice()->getCursorControl(), 400 );
	anim->setFollowNode( player->getSceneNode() );
	camera->addAnimator( anim );
	camera->setFarValue(10000);
	anim->drop();

	//CGameObject *col = getLevel()->searchObject("levelGameM1Col");
	//CColladaMeshComponent *comp = (CColladaMeshComponent*)col->getComponent( IObjectComponent::ColladaMesh );	
	//ISceneNode *node = comp->getDefaultNode(0);
	//ITriangleSelector *world = node->getTriangleSelector();

	//scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.0f,1.2f);
	//scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator( world, camera );
	//camera->addAnimator(anim);
	//camera->setFarValue(10000);
	//anim->drop();	
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