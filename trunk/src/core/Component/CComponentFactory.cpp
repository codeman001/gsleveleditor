#include "stdafx.h"
#include "CComponentFactory.h"

#if defined(GSGAMEPLAY) || defined(GSEDITOR)
#include "CAnimMeshComponent.h"
#include "CStaticMeshComponent.h"
#include "CSkyboxComponent.h"
#include "CObjectTransformComponent.h"
#include "CObjectCollisionComponent.h"
#include "CShadowComponent.h"
#include "CBoxObjectComponent.h"
#include "CBillboardComponent.h"
#include "CTerrainComponent.h"
#include "CGrassComponent.h"
#include "CWaterComponent.h"
#endif 

#if defined(GSANIMATION) || defined(GSGAMEPLAY) || defined(GSEDITOR)
#include "CColladaMeshComponent.h"
#endif

#if defined(GSPARTICLE) || defined(GSGAMEPLAY) || defined(GSEDITOR)
#include "CParticleComponent.h"
#endif

#define	stringOfComponent( type )	IObjectComponent::s_compType[ (int)type ]

#ifdef GSEDITOR
// global template
vector<CSerializable> CComponentFactory::s_compTemplate;


// initComponentTemplate
// read component from template
void CComponentFactory::initComponentTemplate()
{
	s_compTemplate.clear();
	
	CSerializable *p = NULL;

	// add build in anim mesh
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::AnimMesh ];
	p->addGroup	(stringOfComponent(IObjectComponent::AnimMesh));
	p->addPath	("meshFile", "data/mesh/dwarf.x");
	p->addFloat	("animSpeed", 24.0f );

	// add build in static mesh
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::StaticMesh ];
	p->addGroup	(stringOfComponent(IObjectComponent::StaticMesh));
	p->addPath	("meshFile", "data/mesh/dwarf.x");

	// add skybox component
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Skybox ];
	p->addGroup	(stringOfComponent(IObjectComponent::Skybox));
	p->addPath	("skyTexture", "data/skydome2.jpg");		

	// add object transform
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::ObjectTransform ];
	p->addGroup	(stringOfComponent(IObjectComponent::ObjectTransform));	
	p->addFloat("rotateX", 0.0f);
	p->addFloat("rotateY", 0.0f);
	p->addFloat("rotateZ", 0.0f);
	p->addFloat("scaleX", 1.0f);
	p->addFloat("scaleY", 1.0f);
	p->addFloat("scaleZ", 1.0f);

	// add collision
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::ObjectCollision ];
	p->addGroup	(stringOfComponent(IObjectComponent::ObjectCollision));	
	p->addInt("collisionType", 0 );
	p->addInt("collisionData", 0 );

	// add shadow
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Shadow ];
	p->addGroup	(stringOfComponent(IObjectComponent::Shadow));	
	p->addBool("shadowCasting",		true);
	p->addBool("shadowReceiving",	false);

	// add box
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::BoxObject ];
	p->addGroup	(stringOfComponent(IObjectComponent::BoxObject));	
	p->addFloat("size", 100 );
	p->addPath("textureFile", "data/boxTexture.jpg");

	// add billboard
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Billboard ];
	p->addGroup(stringOfComponent(IObjectComponent::Billboard));
	p->addPath("textureFile", "data/particle/particlewhite.bmp");
	p->addFloat("billboardWidth", 50 );
	p->addFloat("billboardHeight", 50 );

	// add particle
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Particle ];
	p->addGroup(stringOfComponent(IObjectComponent::Particle));
	p->addPath( "particleXml", "data/particle/particle.xml" );

	// add collada mesh
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::ColladaMesh ];
	p->addGroup(stringOfComponent(IObjectComponent::ColladaMesh));
	p->addPath("meshFile", "data/mesh/marine_2_mesh.dae");
	p->addPath("animFile", "");
	p->addFloat("animSpeed", 24.0f);	
	p->addString("defaultNode", "" );

	// add terrain
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Terrain ];
	p->addGroup(stringOfComponent(IObjectComponent::Terrain));

	// add grass
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Grass ];
	p->addGroup(stringOfComponent(IObjectComponent::Grass));	
	p->addFloat("grassWidth",		100.0f, true);
	p->addFloat("grassHeight",		50.0f, true);
	p->addPath("grassTexture",		"data/grass.png", true);

	// add water
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Water ];
	p->addGroup(stringOfComponent(IObjectComponent::Water));
	p->addFloat("sizeX", 800.0f, true);
	p->addFloat("sizeY", 800.0f, true);
	p->addString("waterTexture", "data/water.tga", true);
	p->addString("waterNormalTexture", "data/waterNormal.tga", true);

	loadAllTemplate();
}

// isBuildInComponent
// return true, false
bool CComponentFactory::isBuildInComponent( CSerializable* p )
{
	const char *lpName = p->getAllRecord()->front().name;
	for ( int i = 0; i <= IObjectComponent::NumBuildInComponent; i++ )
	{
		if ( strcmp( lpName, stringOfComponent(i) ) == 0 )
			return true;
	}

	return false;
}

// freeData
// release all component template info
void CComponentFactory::freeData()
{
	s_compTemplate.clear();
}

// saveAllTemplate
// save all template info
void CComponentFactory::saveAllTemplate()
{
	wchar_t path[MAX_PATH] = {0};
	wchar_t fileName[MAX_PATH] = {0};

	uiApplication::getAppPath( (LPWSTR)path, MAX_PATH );
	swprintf( fileName, MAX_PATH, L"%s\\componentTemplate.css", path );

	std::ofstream file( fileName );
	
	int n = s_compTemplate.size();
	for ( int i = 0; i < n; i++ )
	{
		CSerializable *p = &s_compTemplate[i];
		if ( isBuildInComponent(p) == false )
		{
			// write to file
			p->saveData( file );
		}
	}

	file.close();

}

// loadAllTemplate
// load all template info
void CComponentFactory::loadAllTemplate()
{
	wchar_t path[MAX_PATH] = {0};
	wchar_t fileName[MAX_PATH] = {0};

	uiApplication::getAppPath( (LPWSTR)path, MAX_PATH );
	swprintf( fileName, MAX_PATH, L"%s\\componentTemplate.css", path );

	std::ifstream file( fileName );
	if ( file.is_open() )
	{
		file.seekg (0, ios::end);
		unsigned long length = file.tellg();
		file.seekg (0, ios::beg);
		
		char *lpBuffer = new char[length];
		memset( lpBuffer, 0, length );

		file.read(lpBuffer,length);
		file.close();	

		CSerializable s;
		char *p = lpBuffer;
		
		while ( *p != NULL )
		{
			if ( s.readData(p) == true )
				s_compTemplate.push_back( s );
			else
				break;
		}

		delete lpBuffer;
	}
	file.close();
}

// addComponent
// add component
bool CComponentFactory::addComponent( CSerializable* p )
{
	int numComponent = (int)s_compTemplate.size();
	const char *lpName = p->getAllRecord()->front().name;

	for ( int i = 0; i < numComponent; i++ )
	{
		CSerializable *pSerializable = &s_compTemplate[i];
		if ( strcmp( lpName, pSerializable->getAllRecord()->front().name) == 0 )
			return false;
	}
	
	s_compTemplate.push_back( *p );
	return true;
}

// removeComponent
// remove component
bool CComponentFactory::removeComponent( CSerializable* p )
{
	int numComponent = (int)s_compTemplate.size();
	const char *lpName = p->getAllRecord()->front().name;

	for ( int i = 0; i < numComponent; i++ )
	{
		CSerializable *pSerializable = &s_compTemplate[i];

		if ( strcmp(lpName, pSerializable->getAllRecord()->front().name) == 0 )
		{
			s_compTemplate.erase( s_compTemplate.begin() + i );
			return true;
		}
	}
	
	return false;
}

// getComponent
// get component data
CSerializable* CComponentFactory::getComponent( char *lpName )
{
	int numComponent = (int)s_compTemplate.size();

	for ( int i = 0; i < numComponent; i++ )
	{
		CSerializable *pSerializable = &s_compTemplate[i];

		if ( strcmp(lpName, pSerializable->getAllRecord()->front().name) == 0 )
		{
			return pSerializable;
		}
	}

	return NULL;
}

#endif

// loadComponent
// create component
IObjectComponent*	CComponentFactory::loadComponent( CGameObject *pObj, CSerializable *data )
{	
	int cursor = data->getCursorRecord();
	char *lpComponentName = data->readString();
	data->setCursorRecord(cursor);
	
	if ( lpComponentName == NULL )
		return NULL;

	IObjectComponent* pComp = NULL;

#if defined(GSGAMEPLAY) || defined(GSEDITOR)
	if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::AnimMesh) ) == 0 )	
		pComp = new CAnimMeshComponent( pObj );
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::StaticMesh ) ) == 0 )	
		pComp = new CStaticMeshComponent( pObj );	
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::Skybox ) ) == 0 )	
		pComp = new CSkyboxComponent( pObj );	
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::ObjectTransform ) ) == 0 )
		pComp = new CObjectTransformComponent( pObj );
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::ObjectCollision ) ) == 0 )
		pComp = new CObjectCollisionComponent( pObj );
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::Shadow ) ) == 0 )
		pComp = new CShadowComponent( pObj );
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::BoxObject ) ) == 0 )
		pComp = new CBoxObjectComponent( pObj );
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::Billboard ) ) == 0 )
		pComp = new CBillboardComponent( pObj );
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::Terrain ) ) == 0 )
		pComp = new CTerrainComponent( pObj );
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::Grass ) ) == 0 )
		pComp = new CGrassComponent( pObj );
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::Water ) ) == 0 )
		pComp = new CWaterComponent( pObj );	
#endif
#if defined(GSANIMATION) || defined(GSGAMEPLAY) || defined(GSEDITOR)
	if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::ColladaMesh ) ) == 0 )
		pComp = new CColladaMeshComponent( pObj );	
#endif
#if defined(GSPARTICLE) || defined(GSGAMEPLAY) || defined(GSEDITOR)
	if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::Particle ) ) == 0 )
		pComp = new CParticleComponent( pObj );	
#endif

	if ( pComp )
		pComp->loadData( data );

	// check not build in component
	return pComp;
}