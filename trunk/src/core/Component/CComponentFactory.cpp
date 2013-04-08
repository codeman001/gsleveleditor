#include "stdafx.h"
#include "CComponentFactory.h"

#include "IView.h"

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
#include "CEllipsoidCollisionComponent.h"
#include "CLightingComponent.h"
#include "CObjectPropertyComponent.h"
#endif 

#ifdef GSGAMEPLAY
#include "gameComponent/CGameComponent.h"
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
std::vector<CSerializable> CComponentFactory::s_compTemplate;


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
	p->addPath	("meshFile", "data/mesh");
	p->addFloat	("animSpeed", 24.0f );

	// add build in static mesh
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::StaticMesh ];
	p->addGroup	(stringOfComponent(IObjectComponent::StaticMesh));
	p->addPath	("meshFile", "data/mesh");

	// add skybox component
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Skybox ];
	p->addGroup	(stringOfComponent(IObjectComponent::Skybox));
	p->addPath	("skyTexture", "data/texture");		

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
	p->addPath("textureFile", "data/texture");

	// add billboard
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Billboard ];
	p->addGroup(stringOfComponent(IObjectComponent::Billboard));
	p->addPath("textureFile", "data/texture");
	p->addFloat("billboardWidth", 50 );
	p->addFloat("billboardHeight", 50 );

	// add particle
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Particle ];
	p->addGroup(stringOfComponent(IObjectComponent::Particle));
	p->addPath( "particleXml", "data/particle" );

	// add collada mesh
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::ColladaMesh ];
	p->addGroup(stringOfComponent(IObjectComponent::ColladaMesh));
	p->addPath("meshFile", "data/mesh");	
	p->addString("defaultNode", "" );

	// add terrain
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Terrain ];
	p->addGroup(stringOfComponent(IObjectComponent::Terrain));
	p->addBool("hideTerrain", true);

	// add grass
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Grass ];
	p->addGroup(stringOfComponent(IObjectComponent::Grass));	
	p->addFloat("grassWidth",		100.0f, true);
	p->addFloat("grassHeight",		50.0f, true);
	p->addPath("grassTexture",		"data/texture");

	// add water
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Water ];
	p->addGroup(stringOfComponent(IObjectComponent::Water));
	p->addFloat("sizeX", 800.0f, true);
	p->addFloat("sizeY", 800.0f, true);
	p->addString("waterTexture", "data/texture");
	p->addString("waterNormalTexture", "data/texture");

	// add ellipsoid
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::EllipsoidCollision ];
	p->addGroup(stringOfComponent(IObjectComponent::EllipsoidCollision));

	// add lighting
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::Lighting ];
	p->addGroup(stringOfComponent(IObjectComponent::Lighting));
	p->addInt("lightingType", 0, true);
	p->addString("diffuseColor","ffffffff", true);
	p->addString("specularColor","ffffffff", true);
	p->addFloat("strength", 1.0f, true);
	p->addFloat("radius",800.0f, true);

	// add object property
	s_compTemplate.push_back( CSerializable() );
	p = &s_compTemplate[ IObjectComponent::ObjectProperty ];
	p->addGroup(stringOfComponent(IObjectComponent::ObjectProperty));
	p->addBool("enableDirectionLight",	true);
	p->addBool("enablePointLight",		true);

	loadAllTemplate();
}

// isBuildInComponent
// return true, false
bool CComponentFactory::isBuildInComponent( CSerializable* p )
{
	const char *lpName = p->getAllRecord()->front().name;
	for ( int i = 0; i < IObjectComponent::NumBuildInComponent; i++ )
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
	std::ofstream file( getIView()->getPath("componentTemplate.css") );
	
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
	const char* path = getIView()->getPath( "componentTemplate.css" );
	io::IReadFile *file = getIView()->getFileSystem()->createAndOpenFile( path );

	if ( file )
	{
		unsigned long length = file->getSize();
		if ( length > 0 )
		{
			char *lpBuffer = new char[length+1];
			file->read(lpBuffer,length);			
			lpBuffer[length] = NULL;	

			CSerializable s;
			char *p = lpBuffer;
			
			while ( *p != NULL )
			{
				if ( s.readData(p) == true )
				{
					s_compTemplate.push_back( s );
					s.clear();
				}
				else
					break;
			}

			delete lpBuffer;
		}
		file->drop();
	}
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
IObjectComponent*	CComponentFactory::loadComponent( CGameObject *pObj, CSerializable *data, bool loadData )
{	
	int cursor = data->getCursorRecord();
	char *lpComponentName = data->readString();
	data->setCursorRecord(cursor);
	
	// unknown name component
	if ( lpComponentName == NULL )
		return NULL;

#ifdef GSGAMEPLAY
	// we init game component first
	IObjectComponent* pComp = CGameComponent::loadComponent( pObj, lpComponentName, data );
#else
	IObjectComponent* pComp = NULL;
#endif


	static SComponentCreation s_componentCreation[] = {
#if defined(GSGAMEPLAY) || defined(GSEDITOR)
		{ stringOfComponent(IObjectComponent::AnimMesh),		newComponent<CAnimMeshComponent> },
		{ stringOfComponent(IObjectComponent::StaticMesh),		newComponent<CStaticMeshComponent> },
		{ stringOfComponent(IObjectComponent::Skybox),			newComponent<CSkyboxComponent> },
		{ stringOfComponent(IObjectComponent::ObjectTransform),	newComponent<CObjectTransformComponent> },
		{ stringOfComponent(IObjectComponent::Shadow),			newComponent<CShadowComponent> },
		{ stringOfComponent(IObjectComponent::BoxObject),		newComponent<CBoxObjectComponent> },
		{ stringOfComponent(IObjectComponent::Billboard),		newComponent<CBillboardComponent> },
		{ stringOfComponent(IObjectComponent::Terrain),			newComponent<CTerrainComponent> },
		{ stringOfComponent(IObjectComponent::Grass),			newComponent<CGrassComponent> },
		{ stringOfComponent(IObjectComponent::Water),			newComponent<CWaterComponent> },
		{ stringOfComponent(IObjectComponent::EllipsoidCollision),	newComponent<CEllipsoidCollisionComponent> },
		{ stringOfComponent(IObjectComponent::Lighting),		newComponent<CLightingComponent> },
		{ stringOfComponent(IObjectComponent::ObjectProperty),	newComponent<CObjectPropertyComponent> },
#endif
#if defined(GSANIMATION) || defined(GSGAMEPLAY) || defined(GSEDITOR)
		{ stringOfComponent(IObjectComponent::ColladaMesh),		newComponent<CColladaMeshComponent> },
#endif
#if defined(GSPARTICLE) || defined(GSGAMEPLAY) || defined(GSEDITOR)
		{ stringOfComponent(IObjectComponent::Particle),		newComponent<CParticleComponent> },
#endif
	};

	// search component
	// and also init built in component
	if ( pComp == NULL )
	{
		const unsigned  int numComponent = sizeof(s_componentCreation)/sizeof(SComponentCreation);
		for ( int i = 0; i < numComponent; i++ )
		{
			if ( strcmp(s_componentCreation[i].componentTypeName, lpComponentName) == 0 )
			{
				pComp = s_componentCreation[i].spawnFn( pObj );
				break;
			}
		}
	}

	// load component data
	if ( pComp && loadData )
		pComp->loadData( data );

	// check not build in component
	return pComp;
}