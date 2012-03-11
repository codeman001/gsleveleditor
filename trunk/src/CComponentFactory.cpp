#include "stdafx.h"
#include "CComponentFactory.h"

#include "CAnimMeshComponent.h"

// loadComponent
// create component
IObjectComponent*	CComponentFactory::loadComponent( CGameObject *pObj, CSerializable *data )
{	
	int cursor = data->getCursorRecord();	
	char *lpComponentName = data->readString();
	data->setCursorRecord(cursor);

	int buildInType = (int)IObjectComponent::AnimMesh;
	while ( buildInType != (int)IObjectComponent::UnknownType )
	{
		if ( strcmp( lpComponentName, IObjectComponent::s_compType[buildInType] ) == 0 )
		{
			break;
		}
		buildInType++;
	}

	IObjectComponent *pComp = NULL;

	if ( buildInType < (int)IObjectComponent::UnknownType )
	{
		if ( buildInType == (int)IObjectComponent::AnimMesh )
		{
			pComp = new CAnimMeshComponent( pObj );
			pComp->loadData( data );
		}
		else if ( buildInType == (int)IObjectComponent::Mesh )
		{
		}
		else if ( buildInType == (int)IObjectComponent::Health )
		{
		}
		else if ( buildInType == (int)IObjectComponent::Collision )
		{
		}
		else if ( buildInType == (int)IObjectComponent::AINpcComponent )
		{
		}
		else if ( buildInType == (int)IObjectComponent::NpcComponent )
		{
		}
	}

	// check not build in component

	return pComp;
}