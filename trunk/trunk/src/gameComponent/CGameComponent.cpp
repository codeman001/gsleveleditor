#include "stdafx.h"
#include "CGameComponent.h"

#include "CPlayerComponent.h"
#include "CInventoryComponent.h"
#include "CWeaponComponent.h"

const char *CGameComponent::s_compType[] = 
{
	"Player component",
	"AI player component",
	"Inventory component",
	"Weapon component"
};

#define	stringOfComponent( type )	CGameComponent::s_compType[ (int)type - (int)CGameComponent::PlayerComponent  ]

// loadComponent
// create component
IObjectComponent* CGameComponent::loadComponent( CGameObject *pObj, char *componentName, CSerializable *data )
{
	IObjectComponent* pComp = NULL;
	
	// declare init component
	static SComponentCreation s_gameComponentCreation[] = {
		{ stringOfComponent(CGameComponent::PlayerComponent),		newComponent<CPlayerComponent> },		
		{ stringOfComponent(CGameComponent::InventoryComponent),	newComponent<CInventoryComponent> },
		{ stringOfComponent(CGameComponent::WeaponComponent),		newComponent<CWeaponComponent> },
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