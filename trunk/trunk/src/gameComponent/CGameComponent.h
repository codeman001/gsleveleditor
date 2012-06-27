#ifndef _GAME_COMPONENT_
#define _GAME_COMPONENT_

#include "IObjectComponent.h"

class CGameComponent
{
protected:

public:
	
	// loadComponent
	// create component
	static IObjectComponent*	loadComponent( CGameObject *pObj, char *componentName, CSerializable *data );

};

#endif