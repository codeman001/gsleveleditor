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

void addTestParticle()
{
	IrrlichtDevice *device = getIView()->getDevice();
	IVideoDriver *driver = getIView()->getDriver();
	ISceneManager *smgr = getIView()->getSceneMgr();

	Group* fireGroup = NULL;
	Group* smokeGroup = NULL;
	Emitter* smokeEmitter = NULL;
	System* particleSystem = NULL;

	float PI = core::PI;

	// Sets the update step
	System::setClampStep(true,0.1f);			// clamp the step to 100 ms
	System::useAdaptiveStep(0.001f,0.01f);		// use an adaptive step from 1ms to 10ms (1000fps to 100fps)


	// Renderers
	IRRQuadRenderer* fireRenderer = IRRQuadRenderer::create(  device );
	//fireRenderer->setScale(0.3f,0.3f);
	fireRenderer->setTexture(driver->getTexture("data/particle/fire2.bmp"));
	fireRenderer->setTexturingMode(TEXTURE_2D);
	fireRenderer->setBlending(BLENDING_ADD);
	fireRenderer->enableRenderingHint(DEPTH_WRITE,false);
	fireRenderer->setAtlasDimensions(2,2);

	IRRQuadRenderer* smokeRenderer = IRRQuadRenderer::create(device);
	//smokeRenderer->setScale(0.3f,0.3f);
	smokeRenderer->setTexture(driver->getTexture("data/particle/explosion.png"));
	smokeRenderer->setTexturingMode(TEXTURE_2D);
	smokeRenderer->setBlending(BLENDING_ALPHA);
	smokeRenderer->enableRenderingHint(DEPTH_WRITE,false);
	smokeRenderer->setAtlasDimensions(2,2);

	Model* fireModel = Model::create(FLAG_RED | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA | FLAG_SIZE | FLAG_ANGLE | FLAG_TEXTURE_INDEX,
		FLAG_RED | FLAG_GREEN | FLAG_ALPHA | FLAG_ANGLE,
		FLAG_RED | FLAG_GREEN | FLAG_TEXTURE_INDEX | FLAG_ANGLE,
		FLAG_SIZE);
	fireModel->setParam(PARAM_RED,0.8f,0.9f,0.8f,0.9f);
	fireModel->setParam(PARAM_GREEN,0.5f,0.6f,0.5f,0.6f);
	fireModel->setParam(PARAM_BLUE,0.3f);
	fireModel->setParam(PARAM_ALPHA,0.4f,0.0f);
	fireModel->setParam(PARAM_ANGLE,0.0f,2.0f * PI,0.0f,2.0f * PI);
	fireModel->setParam(PARAM_TEXTURE_INDEX,0.0f,4.0f);
	fireModel->setLifeTime(1.0f,1.5f);

	Interpolator* interpolator = fireModel->getInterpolator(PARAM_SIZE);
	interpolator->addEntry(0.5f,2.0f,5.0f);
	interpolator->addEntry(1.0f,0.0f);

	Model* smokeModel = Model::create(FLAG_RED | FLAG_GREEN | FLAG_BLUE | FLAG_ALPHA | FLAG_SIZE | FLAG_ANGLE | FLAG_TEXTURE_INDEX,
		FLAG_RED | FLAG_GREEN | FLAG_SIZE | FLAG_ANGLE,
		FLAG_TEXTURE_INDEX | FLAG_ANGLE,
		FLAG_ALPHA);
	smokeModel->setParam(PARAM_RED,0.3f,0.2f);
	smokeModel->setParam(PARAM_GREEN,0.25f,0.2f);
	smokeModel->setParam(PARAM_BLUE,0.2f);
	smokeModel->setParam(PARAM_ALPHA,0.2f,0.0f);
	smokeModel->setParam(PARAM_SIZE,5.0,10.0f);
	smokeModel->setParam(PARAM_TEXTURE_INDEX,0.0f,4.0f);
	smokeModel->setParam(PARAM_ANGLE,0.0f,2.0f * PI,0.0f,2.0f * PI);
	smokeModel->setLifeTime(5.0f,5.0f);

	interpolator = smokeModel->getInterpolator(PARAM_ALPHA);
	interpolator->addEntry(0.0f,0.0f);
	interpolator->addEntry(0.2f,0.2f);
	interpolator->addEntry(1.0f,0.0f);

	// Emitters
	// The emitters are arranged so that the fire looks realistic
	StraightEmitter* fireEmitter1 = StraightEmitter::create(Vector3D(0.0f,1.0f,0.0f));
	fireEmitter1->setZone(Sphere::create(Vector3D(0.0f,-1.0f,0.0f),0.5f));
	fireEmitter1->setFlow(40);
	fireEmitter1->setForce(1.0f,2.5f);

	StraightEmitter* fireEmitter2 = StraightEmitter::create(Vector3D(1.0f,0.6f,0.0f));
	fireEmitter2->setZone(Sphere::create(Vector3D(0.15f,-1.2f,0.075f),0.1f));
	fireEmitter2->setFlow(15);
	fireEmitter2->setForce(0.5f,1.5f);

	StraightEmitter* fireEmitter3 = StraightEmitter::create(Vector3D(-0.6f,0.8f,-0.8f));
	fireEmitter3->setZone(Sphere::create(Vector3D(-0.375f,-1.15f,-0.375f),0.3f));
	fireEmitter3->setFlow(15);
	fireEmitter3->setForce(0.5f,1.5f);

	StraightEmitter* fireEmitter4 = StraightEmitter::create(Vector3D(-0.8f,0.5f,0.2f));
	fireEmitter4->setZone(Sphere::create(Vector3D(-0.255f,-1.2f,0.225f),0.2f));
	fireEmitter4->setFlow(10);
	fireEmitter4->setForce(0.5f,1.5f);

	StraightEmitter* fireEmitter5 = StraightEmitter::create(Vector3D(0.1f,0.8f,-1.0f));
	fireEmitter5->setZone(Sphere::create(Vector3D(-0.075f,-1.2f,-0.3f),0.2f));
	fireEmitter5->setFlow(10);
	fireEmitter5->setForce(0.5f,1.5f);

	smokeEmitter = SphericEmitter::create(Vector3D(0.0f,1.0f,0.0f),0.0f,0.5f * PI);
	smokeEmitter->setZone(Sphere::create(Vector3D(),1.2f));
	smokeEmitter->setFlow(25);
	smokeEmitter->setForce(0.5f,1.0f);

	// Groups
	fireGroup = Group::create(fireModel,135);
	fireGroup->addEmitter(fireEmitter1);
	fireGroup->addEmitter(fireEmitter2);
	fireGroup->addEmitter(fireEmitter3);
	fireGroup->addEmitter(fireEmitter4);
	fireGroup->addEmitter(fireEmitter5);
	fireGroup->setRenderer(fireRenderer);
	fireGroup->setGravity(Vector3D(0.0f,3.0f,0.0f));
	fireGroup->enableAABBComputing(true);

	smokeGroup = Group::create(smokeModel,135);
	smokeGroup->addEmitter(smokeEmitter);
	smokeGroup->setRenderer(smokeRenderer);
	smokeGroup->setGravity(Vector3D(0.0f,0.4f,0.0f));
	smokeGroup->enableAABBComputing(true);
	
	// System
	particleSystem = IRRSystem::create(smgr->getRootSceneNode(),smgr);
	particleSystem->addGroup(smokeGroup);
	particleSystem->addGroup(fireGroup);
	particleSystem->enableAABBComputing(true);

	SPKFactory::getInstance().traceAll();
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
	
	addTestParticle();
}

void CLevel::update()
{
}

void CLevel::render()
{
}