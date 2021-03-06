#ifndef _OBJTEMPLATEFACTORY_
#define _OBJTEMPLATEFACTORY_

#include "CObjectTemplate.h"

class CGameObject;

class CObjTemplateFactory
{
public:
	static std::vector<CObjectTemplate>	s_objectTemplate;

#ifdef GSEDITOR
	static std::vector<CGameObject*>		s_objects;
#endif

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
	static void removeTemplate(wchar_t* templateName);

	// freeData
	// delete all list object template
	static void freeData();

	// spawnObject
	// create a object on template
	static CGameObject* spawnObject( wchar_t* templateName );

	// spawnObject
	// create a object on template with parent
	static CGameObject* spawnObject( wchar_t* templateName, CGameObject *parent );

	// spawnNullObject
	// create a object on template with parent (no load data)
	static CGameObject* spawnNullObject( wchar_t* templateName, CGameObject *parent );

	// getTemplate
	// get template
	static CObjectTemplate* getTemplate( wchar_t* templateName );

    // getTemplateId
    // get template id for networking transfer
    static int getTemplateId( wchar_t* templateName );
    
    // getTemplateName
    // template name by id
    static wchar_t* getTemplateName( int id );
    
#ifdef GSEDITOR
	// getGameObject
	// get a template object
	static CGameObject* getGameObject( wchar_t* templateName );

	// addTemplateObject
	// add a template object
	static void addTemplateObject( CGameObject *pObj );

	// registerDrawAllObject
	// add all to scene node
	static void registerDrawAllTemplateObject();
#endif

};

typedef std::vector<CObjectTemplate>			ArrayTemplate;
typedef std::vector<CObjectTemplate>::iterator	ArrayTemplateIter;

#endif