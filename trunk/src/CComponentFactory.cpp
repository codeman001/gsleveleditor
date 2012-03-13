#include "stdafx.h"
#include "CComponentFactory.h"

#include "CAnimMeshComponent.h"

#define	stringOfComponent( type )	IObjectComponent::s_compType[ (int)type ]

// global template
CSerializable CComponentFactory::s_compTemplate[ IObjectComponent::NumComponent ];


// initComponentTemplate
// read component from template
void CComponentFactory::initComponentTemplate()
{
	// default component anim mesh
	CSerializable *p = &s_compTemplate[ IObjectComponent::AnimMesh ];
	p->clear();
	p->addGroup	(stringOfComponent(IObjectComponent::AnimMesh));
	p->addPath	("meshFile", "data/mesh/dwarf.x");
	p->addFloat	("animSpeed", 24.0f );
}




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