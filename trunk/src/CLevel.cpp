#include "stdafx.h"
#include "CLevel.h"

#include "CGameOxyzSceneNode.h"



CLevel::CLevel()
{
	init();
}

CLevel::~CLevel()
{	
	delete m_playerCamera;

	CObjTemplateFactory::freeData();
}

void CLevel::init()
{
	CObjTemplateFactory::initObjectTempalte();


	ISceneManager *smgr = getIView()->getSceneMgr();

	// add oxyz plance
	CGameOxyzSceneNode *oxyz = new CGameOxyzSceneNode( smgr->getRootSceneNode(), smgr, -1 );
	oxyz->drop();

	// setup camera
	m_playerCamera = new CGameCamera();
	m_playerCamera->setPosition( core::vector3df(-100, 100, -100) );
	m_playerCamera->setTarget( core::vector3df( 0, 0, 0) );
	getIView()->setActiveCamera( m_playerCamera );

	// spawn orge character	
	CGameObject *pOrge = CObjTemplateFactory::spawnObject( L"orge" );
	if ( pOrge )
	{
		pOrge->setPosition( core::vector3df( 10, 0, 10) );
		pOrge->setVisible( true );
	}
	
	// spawn skybox
	CGameObject *skyBox = CObjTemplateFactory::spawnObject( L"skybox" );
	if ( skyBox )
		skyBox->setVisible( true );

	ILightSceneNode *light = smgr->addLightSceneNode( smgr->getRootSceneNode() );
}

void CLevel::update()
{
}

void CLevel::render()
{
}