#ifndef _COMPONENTFACTORY_
#define _COMPONENTFACTORY_

#include "IObjectComponent.h"
#include "3rdsource/tinyxml/tinyxml.h"

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

	// saveAllTemplate
	// save all template info
	static void saveAllTemplate();

	// loadAllTemplate
	// load all template info
	static void loadAllTemplate();

	// getComponentName
	// return the name of component
	static const char* getComponentName( int comID );

	// getComponentID
	// return the id of component
	static int getComponentID( const char *name );
#endif

	// loadComponent
	// create component
	static IObjectComponent*	loadComponent( CGameObject *pObj, CSerializable *data );
};

#endif