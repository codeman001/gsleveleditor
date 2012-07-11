#ifndef _GAME_COMPONENT_
#define _GAME_COMPONENT_

#include "IObjectComponent.h"
#include "CPlayerComponent.h"
#include "CInventoryComponent.h"
#include "CWeaponComponent.h"

class CGameComponent
{
public:
	static const char *s_compType[];

public:
	enum ComponentType
	{
		PlayerComponent = IObjectComponent::NumBuildInComponent,
		AIPlayerComponent,
		InventoryComponent,
		WeaponComponent
	};

	// loadComponent
	// create component
	static IObjectComponent*	loadComponent( CGameObject *pObj, char *componentName, CSerializable *data );

};

#endif