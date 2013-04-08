#ifndef _COMPONENTFACTORY_
#define _COMPONENTFACTORY_

#include "IObjectComponent.h"

class CComponentFactory
{
public:

#ifdef GSEDITOR
	// array of default template component 
	static std::vector<CSerializable>	s_compTemplate;
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

	// addComponent
	// add component
	static bool addComponent( CSerializable* p );

	// removeComponent
	// remove component
	static bool removeComponent( CSerializable* p );

	// getComponent
	// get component data
	CSerializable* getComponent( char *lpName );
#endif

	// loadComponent
	// create component
	static IObjectComponent*	loadComponent( CGameObject *pObj, CSerializable *data, bool loadData = true );
};

#endif