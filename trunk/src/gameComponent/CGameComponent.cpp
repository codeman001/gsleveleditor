#include "stdafx.h"
#include "CGameComponent.h"

#include "CPlayerComponent.h"

const char *CGameComponent::s_compType[] = 
{
	"Player component",
	"AI player component"
};

#define	stringOfComponent( type )	CGameComponent::s_compType[ (int)type - (int)CGameComponent::PlayerComponent  ]

// loadComponent
// create component
IObjectComponent* CGameComponent::loadComponent( CGameObject *pObj, char *componentName, CSerializable *data )
{
	IObjectComponent* pComp = NULL;
	
	if ( strcmp( componentName, stringOfComponent(CGameComponent::PlayerComponent) ) == 0 )	
		pComp = new CPlayerComponent( pObj );

	return pComp;
}