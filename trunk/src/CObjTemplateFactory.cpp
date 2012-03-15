#include "stdafx.h"
#include "CObjTemplateFactory.h"

// global object template
vector<CObjectTemplate> CObjTemplateFactory::s_objectTemplate;
int CObjTemplateFactory::s_idGenerate = 1;

// initObjectTempalte
// init object template
void CObjTemplateFactory::initObjectTempalte()
{
	s_objectTemplate.clear();
	loadAllObjectTemplate();
}

// loadAllObjectTemplate
// load obj template from file
void CObjTemplateFactory::loadAllObjectTemplate()
{

}

// saveAllObjectTemplate
// save obj template to file
void CObjTemplateFactory::saveAllObjectTemplate()
{

}

// addTemplate
// add a template to list
void CObjTemplateFactory::addTemplate(CObjectTemplate* p)
{
	s_objectTemplate.push_back( *p );
	s_objectTemplate.back().setTemplateID( s_idGenerate++ );
}

// removeTemplate
// remove a template from list
void CObjTemplateFactory::removeTemplate(int templateID)
{
	ArrayTemplateIter i = s_objectTemplate.begin(), end = s_objectTemplate.end();

	while ( i != end )
	{
		CObjectTemplate *p = &(*i);

		if ( p->getTemplateID() == templateID )
		{
			s_objectTemplate.erase( i );
			break;
		}
		i++;
	}
}

// freeData
// delete all list object template
void CObjTemplateFactory::freeData()
{
	s_objectTemplate.clear();
}