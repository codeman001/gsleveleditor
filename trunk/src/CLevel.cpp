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

	CGameGSCameraAnimators* animator = new CGameGSCameraAnimators( getIView()->getDevice()->getCursorControl() );
	m_playerCamera->getCameraNode()->addAnimator( animator );
	animator->drop();

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


	// add terrain
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"data/terrain-heightmap.bmp",
		0,										// parent node
		-1,										// node id
		core::vector3df(0.f, 0.f, 0.f),			// position
		core::vector3df(0.f, 0.f, 0.f),			// rotation
		core::vector3df(40.f, 4.4f, 40.f),		// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		5,										// maxLOD
		scene::ETPS_17,							// patchSize
		4										// smoothFactor
		);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0,getIView()->getDriver()->getTexture("data/terrain-texture.jpg"));
	terrain->setMaterialTexture(1,getIView()->getDriver()->getTexture("data/detailmap3.jpg"));	
	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 20.0f);
}

void CLevel::update()
{
}

void CLevel::render()
{
}