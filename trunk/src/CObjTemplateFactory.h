#ifndef _OBJTEMPLATEFACTORY_
#define _OBJTEMPLATEFACTORY_

#include "CObjectTemplate.h"

class CObjTemplateFactory
{
public:
	static vector<CObjectTemplate>	s_objectTemplate;

	// loadAllObjectTemplate
	// load obj template from file
	static void loadAllObjectTemplate();

	// saveAllObjectTemplate
	// save obj template to file
	static void saveAllObjectTemplate();

};

#endif