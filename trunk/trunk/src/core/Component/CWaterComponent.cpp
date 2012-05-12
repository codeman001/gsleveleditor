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
	
	pObj->addFloat("sizeX", m_sizeX, true);
	pObj->addFloat("sizeY", m_sizeY, true);
	pObj->addString("waterTexture", m_waterTexture.c_str(), true);
	pObj->addString("waterNormalTexture", m_waterNormalTexture.c_str(), true);
}

// loadData
// load data to serializable
void CWaterComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	m_sizeX		= pObj->readFloat();
	m_sizeY		= pObj->readFloat();
	m_waterTexture = pObj->readString();
	m_waterNormalTexture = pObj->readString();

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

	core::dimension2df tileSize(m_sizeX,m_sizeY);
	core::dimension2du tileCount(1,1);
	core::dimension2df countHill(1,1);
	core::dimension2df textureRepeat(1,1);
	
	ITexture *pTexNormal= driver->getTexture( getIView()->getPath( m_waterNormalTexture.c_str() ) );
	ITexture *pTexRef	= driver->getTexture( getIView()->getPath( m_waterTexture.c_str() ) );

	SMaterial mat;
	mat.TextureLayer[0].Texture = pTexNormal;
	mat.TextureLayer[1].Texture = pTexRef;

	mat.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
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