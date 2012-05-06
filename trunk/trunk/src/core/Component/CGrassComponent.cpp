#include "stdafx.h"
#include "CGrassComponent.h"
#include "IView.h"

CGrassComponent::CGrassComponent( CGameObject *pObj )
	:IObjectComponent( pObj, IObjectComponent::Grass )
{
}

CGrassComponent::~CGrassComponent()
{
}

// init
// run when init object
void CGrassComponent::initComponent()
{
}

// updateComponent
// update object by frame
void CGrassComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CGrassComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	pObj->addFloat("grassWidth",		m_width, true);
	pObj->addFloat("grassHeight",		m_height, true);
	pObj->addPath("grassTexture",		m_texture.c_str(), true);
}

// loadData
// load data to serializable
void CGrassComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	m_width		= pObj->readFloat();
	m_height	= pObj->readFloat();
	m_texture	= pObj->readString();

	initGrassNode();
}

// initGrassNode
// init grass
void CGrassComponent::initGrassNode()
{
	if ( m_texture.size() == 0 )
		return;

	// release if mesh is loaded
	if ( m_gameObject->m_node )
		m_gameObject->destroyNode();	

	/*
	1	0
	2	3
	*/

	m_indices[0] = 0;
	m_indices[1] = 2;
	m_indices[2] = 1;
	m_indices[3] = 0;
	m_indices[4] = 3;
	m_indices[5] = 2;

	m_vertices[0].Pos = core::vector3df(m_width*0.5f, m_height, 0);
	m_vertices[0].TCoords.set(1.0f, 0.0f);
	m_vertices[0].Color = SColor(255,255,255,255);

	m_vertices[1].Pos = core::vector3df(-m_width*0.5f, m_height, 0);
	m_vertices[1].TCoords.set(0.0f, 0.0f);
	m_vertices[1].Color = SColor(255,255,255,255);

	m_vertices[2].Pos = core::vector3df(-m_width*0.5f, 0, 0);
	m_vertices[2].TCoords.set(0.0f, 1.0f);
	m_vertices[2].Color = SColor(255,255,255,255);

	m_vertices[3].Pos = core::vector3df(m_width*0.5f, 0, 0);
	m_vertices[3].TCoords.set(1.0f, 1.0f);
	m_vertices[3].Color = SColor(255,255,255,255);

	SMesh *mesh = new SMesh();
	
	SMeshBuffer *meshBuffer = new SMeshBuffer();
	

	// make vertices	
	meshBuffer->Vertices.set_used(4);
	memcpy(meshBuffer->Vertices.pointer(), m_vertices, sizeof(S3DVertex) * 4);

	// make indices	
	meshBuffer->Indices.set_used(6);
	memcpy(meshBuffer->Indices.pointer(), m_indices, sizeof(u16) * 6);

	// set texture
	IVideoDriver *driver = getIView()->getDriver();
	ITexture *pTex = driver->getTexture( m_texture.c_str() );

#ifdef GSEDITOR
	if ( pTex == NULL )
	{
		WCHAR appPath[MAX_PATH];
		char  appPathA[MAX_PATH];

		uiApplication::getAppPath(appPath, MAX_PATH);
		uiString::copy<char, WCHAR>( appPathA, appPath  );
		
		std::string path = appPathA;
		path += "\\";
		path += m_texture;
		
		pTex = driver->getTexture(path.c_str());
	}
#endif

	meshBuffer->getMaterial().setTexture(0, pTex);
	meshBuffer->getMaterial().setFlag( video::EMF_LIGHTING, false );
	meshBuffer->getMaterial().MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;

	// add mesh buffer
	meshBuffer->recalculateBoundingBox();
	mesh->addMeshBuffer( meshBuffer );
	meshBuffer->drop();
	mesh->recalculateBoundingBox();

	m_grassNode = new CGameGrassSceneNode
		( 
			m_gameObject, 
			mesh, 
			m_gameObject->getParentSceneNode(), 
			getIView()->getSceneMgr(), 
			m_gameObject->getPosition(), 
			m_gameObject->getID() 
		);

	m_gameObject->m_node = m_grassNode;

#ifdef GSEDITOR
	// add collision
	ITriangleSelector *selector = getIView()->getSceneMgr()->createTriangleSelectorFromBoundingBox(m_grassNode);
	m_grassNode->setTriangleSelector(selector);
	selector->drop();
#endif

	// drop mesh
	mesh->drop();
}