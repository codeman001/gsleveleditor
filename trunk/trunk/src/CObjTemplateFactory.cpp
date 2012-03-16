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
	wchar_t path[MAX_PATH] = {0};
	wchar_t fileName[MAX_PATH] = {0};

	uiApplication::getAppPath( (LPWSTR)path, MAX_PATH );
	swprintf( fileName, MAX_PATH, L"%s\\objectTemplate.css", path );

	std::ifstream file( fileName );

	if ( file.is_open() )
	{
		file.seekg (0, ios::end);
		unsigned long length = file.tellg();
		file.seekg (0, ios::beg);
		
		char *lpBuffer = new char[length];
		memset( lpBuffer, 0, length );

		file.read(lpBuffer,length);
		file.close();

		

	}
}

// saveAllObjectTemplate
// save obj template to file
void CObjTemplateFactory::saveAllObjectTemplate()
{
	wchar_t path[MAX_PATH] = {0};
	wchar_t fileName[MAX_PATH] = {0};

	uiApplication::getAppPath( (LPWSTR)path, MAX_PATH );
	swprintf( fileName, MAX_PATH, L"%s\\objectTemplate.css", path );

	std::ofstream file( fileName );
	
	ArrayTemplateIter it = s_objectTemplate.begin(), end = s_objectTemplate.end();
	while ( it != end )
	{
		CObjectTemplate *pTemplate = &(*it);
		
		wchar_t *templateName = pTemplate->getObjectTemplateName();
		char	templateNameA[1024];

		uiString::convertUnicodeToUTF8( (unsigned short*) templateName, templateNameA );
		
		file << templateNameA << "\n";
		file << "{\n";
		
		ArraySerializable* listSerializable = pTemplate->getAllComponentProperty();
		ArraySerializable::iterator iSerializable = listSerializable->begin(), iEnd = listSerializable->end();
		while ( iSerializable != iEnd )
		{
			(*iSerializable).saveData( file, 1 );
			iSerializable++;
		}

		file << "}\n";

		++it;
	}

	file.close();
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