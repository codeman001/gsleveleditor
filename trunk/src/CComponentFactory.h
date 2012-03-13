#ifndef _COMPONENTFACTORY_
#define _COMPONENTFACTORY_

#include "IObjectComponent.h"

class CComponentFactory
{
public:
	static CSerializable		s_compTemplate[ IObjectComponent::NumComponent ];

public:
	// initComponentTemplate
	// read component from template
	static void initComponentTemplate();

	// loadComponent
	// create component
	static IObjectComponent*	loadComponent( CGameObject *pObj, CSerializable *data );
};

#endif