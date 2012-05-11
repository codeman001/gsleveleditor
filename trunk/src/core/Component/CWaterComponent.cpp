#include "stdafx.h"
#include "IView.h"
#include "CWaterComponent.h"

CWaterComponent::CWaterComponent( CGameObject *pObj )
	:IObjectComponent( pObj, (int)IObjectComponent::Water )
{
	m_waterNode = NULL;
}

CWaterComponent::~CWaterComponent()
{
}

// init
// run when init object
void CWaterComponent::initComponent()
{
}

// updateComponent
// update object by frame
void CWaterComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CWaterComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );
	
}

// loadData
// load data to serializable
void CWaterComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	initWaterNode();
}

// initGrassNode
// init grass
void CWaterComponent::initWaterNode()
{	
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();

	ISceneManager *smgr = getIView()->getSceneMgr();
	IVideoDriver *driver = getIView()->getDriver();

	core::dimension2df tileSize(80,80);	
	core::dimension2du tileCount(10,10);
	core::dimension2df countHill(1,1);
	core::dimension2df textureRepeat(1,1);

	char *waveNormal = "data/wavenormal.png";
	char *waveRef = "data/clouds.tga";	

	ITexture *pTex = driver->getTexture( waveNormal );
	ITexture *pTexRef = driver->getTexture( waveRef );

#ifdef GSEDITOR
	if ( pTex == NULL )
	{
		WCHAR appPath[MAX_PATH];
		char  appPathA[MAX_PATH];

		uiApplication::getAppPath(appPath, MAX_PATH);
		uiString::copy<char, WCHAR>( appPathA, appPath  );
		
		std::string path = appPathA;
		path += "\\";
		path += waveNormal;
		
		pTex = driver->getTexture(path.c_str());
	}

	if ( pTexRef == NULL )
	{
		WCHAR appPath[MAX_PATH];
		char  appPathA[MAX_PATH];

		uiApplication::getAppPath(appPath, MAX_PATH);
		uiString::copy<char, WCHAR>( appPathA, appPath  );
		
		std::string path = appPathA;
		path += "\\";
		path += waveRef;
		
		pTexRef = driver->getTexture(path.c_str());
	}
#endif

	SMaterial mat;
	mat.TextureLayer[0].Texture = pTex;
	mat.TextureLayer[1].Texture = pTexRef;

	mat.MaterialType = EMT_SOLID;
	mat.Lighting = false;

	IMesh *waterPlance = smgr->getGeometryCreator()->createHillPlaneMesh(tileSize, tileCount, &mat, 0.0f, countHill, textureRepeat );	
	waterPlance->setHardwareMappingHint( EHM_STATIC );

	// create node
	m_waterNode = new CGameWaterSceneNode( m_gameObject, waterPlance, m_gameObject->getParentSceneNode(), smgr );	

#ifdef GSEDITOR
	// add collision
	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox(m_waterNode);
	m_waterNode->setTriangleSelector(selector);
	selector->drop();
#endif
	
	// set visibke
	m_gameObject->m_node = m_waterNode;
	m_gameObject->m_node->setVisible( true );	
	m_gameObject->setLighting( false );

	// update position
	m_gameObject->updateNodePosition();
	m_gameObject->updateNodeRotation();

	waterPlance->drop();
}