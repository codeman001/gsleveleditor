#ifndef _COMPONENTFACTORY_
#define _COMPONENTFACTORY_

#include "IObjectComponent.h"

class CComponentFactory
{
public:
	
public:
	// loadComponent
	// create component
	static IObjectComponent*	loadComponent( CGameObject *pObj, CSerializable *data );
};

#endif