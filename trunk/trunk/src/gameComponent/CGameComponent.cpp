#include "stdafx.h"
#include "CGameComponent.h"

#include "CPlayerComponent.h"
#include "CNetworkPlayerComponent.h"
#include "CGunLightComponent.h"

const char *CGameComponent::s_compType[] = 
{
	"Player component",
	"Network player component",
    "Gun light component",
	"Bullet ray component",
	"Gun light component",
};

#define	stringOfComponent( type )	CGameComponent::s_compType[ (int)type - (int)CGameComponent::PlayerComponent  ]

// loadComponent
// create component
IObjectComponent* CGameComponent::loadComponent( CGameObject *pObj, char *componentName, CSerializable *data )
{
	IObjectComponent* pComp = NULL;
	
	// declare init component
	static SComponentCreation s_gameComponentCreation[] = {
		{ stringOfComponent(CGameComponent::PlayerComponent),			newComponent<CPlayerComponent> },
		{ stringOfComponent(CGameComponent::NetworkPlayerComponent),	newComponent<CNetworkPlayerComponent> },
		{ stringOfComponent(CGameComponent::GunLightComponent),			newComponent<CGunLightComponent> }
	};

	// search all game component
	const unsigned  int numComponent = sizeof(s_gameComponentCreation)/sizeof(SComponentCreation);
	for ( int i = 0; i < numComponent; i++ )
	{
		if ( strcmp(s_gameComponentCreation[i].componentTypeName, componentName) == 0 )
		{
			pComp = s_gameComponentCreation[i].spawnFn( pObj );
			break;
		}
	}

	return pComp;
}