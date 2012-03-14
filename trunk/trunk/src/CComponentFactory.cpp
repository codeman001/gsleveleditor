#include "stdafx.h"
#include "CComponentFactory.h"
#include "CAnimMeshComponent.h"

#define	stringOfComponent( type )	IObjectComponent::s_compType[ (int)type ]

#ifdef GSEDITOR
// global template
vector<CSerializable> CComponentFactory::s_compTemplate;


// initComponentTemplate
// read component from template
void CComponentFactory::initComponentTemplate()
{
	s_compTemplate.clear();
	s_compTemplate.resize( IObjectComponent::NumComponent );

	// default component anim mesh
	CSerializable *p = &s_compTemplate[ IObjectComponent::AnimMesh ];
	p->addGroup	(stringOfComponent(IObjectComponent::AnimMesh));
	p->addPath	("meshFile", "data/mesh/dwarf.x");
	p->addFloat	("animSpeed", 24.0f );
}

// isBuildInComponent
// return true, false
bool CComponentFactory::isBuildInComponent( CSerializable* p )
{
	for ( int i = 0; i < IObjectComponent::NumComponent; i++ )
	{
		if ( p == &s_compTemplate[i] )
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
	uiApplication::getAppPath( (LPWSTR)path, MAX_PATH );
}

// loadAllTemplate
// load all template info
void CComponentFactory::loadAllTemplate()
{
	
}

#endif

// loadComponent
// create component
IObjectComponent*	CComponentFactory::loadComponent( CGameObject *pObj, CSerializable *data )
{	
	int cursor = data->getCursorRecord();	
	char *lpComponentName = data->readString();
	data->setCursorRecord(cursor);

	IObjectComponent* pComp = NULL;

	if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::AnimMesh) ) == 0 )
	{
		pComp = new CAnimMeshComponent( pObj );
		pComp->loadData( data );
	}
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::Mesh ) ) == 0 )
	{
	}
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::Health) ) == 0 )
	{
	}
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::Collision) ) == 0 )
	{
	}
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::AINpcComponent) ) == 0 )
	{
	}
	else if ( strcmp( lpComponentName, stringOfComponent(IObjectComponent::NpcComponent) ) == 0 )
	{
	}	
	// check not build in component

	return pComp;
}