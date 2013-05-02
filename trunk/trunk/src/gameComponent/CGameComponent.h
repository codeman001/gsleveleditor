#ifndef _GAME_COMPONENT_
#define _GAME_COMPONENT_

#include "IObjectComponent.h"

class CGameComponent
{
public:
	static const char *s_compType[];

public:
	enum EComponentType
	{
		PlayerComponent = IObjectComponent::NumBuildInComponent,
		NetworkPlayerComponent,
        GunLightComponent,
		BulletRayComponent,
		LifeTimeComponent,
		SpawnPointComponent,
		SpawnPointMgrComponent,
		WeaponComponent,
	};

	// loadComponent
	// create component
	static IObjectComponent*	loadComponent( CGameObject *pObj, char *componentName, CSerializable *data );

};

#endif