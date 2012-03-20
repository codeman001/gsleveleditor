#include "stdafx.h"
#include "CComponentFactory.h"

#include "CAnimMeshComponent.h"
#include "CStaticMeshComponent.h"
#include "CSkyboxComponent.h"

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

	if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::AnimMesh) ) == 0 )
	{
		pComp = new CAnimMeshComponent( pObj );
		pComp->loadData( data );
	}
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::StaticMesh ) ) == 0 )
	{
		pComp = new CStaticMeshComponent( pObj );
		pComp->loadData( data );
	}
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::Skybox ) ) == 0 )
	{
		pComp = new CSkyboxComponent( pObj );
		pComp->loadData( data );
	}

	// check not build in component
	return pComp;
}