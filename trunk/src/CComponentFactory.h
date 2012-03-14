#ifndef _COMPONENTFACTORY_
#define _COMPONENTFACTORY_

#include "IObjectComponent.h"

class CComponentFactory
{
public:

#ifdef GSEDITOR
	// array of default template component 
	static vector<CSerializable>	s_compTemplate;
#endif

public:

#ifdef GSEDITOR
	// initComponentTemplate
	// read component from template
	static void initComponentTemplate();

	// freeData	
	// release all component template info
	static void freeData();

	// isBuildInComponent
	// return true, false
	static bool isBuildInComponent( CSerializable* p );
#endif

	// loadComponent
	// create component
	static IObjectComponent*	loadComponent( CGameObject *pObj, CSerializable *data );
};

#endif