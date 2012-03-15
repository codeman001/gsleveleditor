#ifndef _OBJTEMPLATEFACTORY_
#define _OBJTEMPLATEFACTORY_

#include "CObjectTemplate.h"

class CObjTemplateFactory
{
public:
	static vector<CObjectTemplate>	s_objectTemplate;
	static int s_idGenerate;

	// initObjectTempalte
	// init object template
	static void initObjectTempalte();

	// loadAllObjectTemplate
	// load obj template from file
	static void loadAllObjectTemplate();

	// saveAllObjectTemplate
	// save obj template to file
	static void saveAllObjectTemplate();

	// addTemplate
	// add a template to list
	static void addTemplate(CObjectTemplate* p);

	// removeTemplate
	// remove a template from list
	static void removeTemplate(int templateID);

	// freeData
	// delete all list object template
	static void freeData();
};

typedef vector<CObjectTemplate>				ArrayTemplate;
typedef vector<CObjectTemplate>::iterator	ArrayTemplateIter;

#endif