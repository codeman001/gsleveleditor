#include "stdafx.h"
#include "CObjTemplateFactory.h"

// global object template
vector<CObjectTemplate> CObjTemplateFactory::s_objectTemplate;

// initObjectTempalte
// init object template
void CObjTemplateFactory::initObjectTempalte()
{
	s_objectTemplate.clear();
	loadAllObjectTemplate();
}

extern void getBufferString( char *lpBuffer, char *from, char *to );
//void getBufferString( char *lpBuffer, char *from, char *to )
//{
//	int len = to - from;
//	
//	uiString::mid(lpBuffer, from, 0, len);
//	uiString::trim(lpBuffer);
//}

void readTemplateFromData( char *lpData )
{
	char *p = lpData;
	char *from = p;

	char	lpStringA[1024];
	wchar_t	lpString[1024];
	while ( *p != NULL )
	{
		if ( *p == '{' )
		{
			getBufferString( lpStringA, from, p );
			uiString::convertUTF8ToUnicode( lpStringA, (unsigned short*) lpString );

			CObjectTemplate obj;
			CSerializable	component;

			obj.setObjectTemplateName( lpString );
			
			p++;

			while ( component.readData( p ) )
			{
				obj.getAllComponentProperty()->push_back( component );				
				component.clear();
			}
			
			CObjTemplateFactory::addTemplate( &obj );		
		}
		else if ( *p == '}' )
		{
			p++;
			from = p;
		}
		else
		{
			p++;
		}
	}
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

		readTemplateFromData( lpBuffer );

		delete lpBuffer;
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
}

// removeTemplate
// remove a template from list
void CObjTemplateFactory::removeTemplate(wchar_t* templateName)
{
	ArrayTemplateIter i = s_objectTemplate.begin(), end = s_objectTemplate.end();

	char lpNameA[1024];
	
	char templateNameA[1024];
	uiString::convertUnicodeToUTF8( (unsigned short*) templateName, templateNameA );

	while ( i != end )
	{
		CObjectTemplate *p = &(*i);
	
		uiString::convertUnicodeToUTF8( (unsigned short*) p->getObjectTemplateName(), lpNameA );

		if ( strcmp( lpNameA, templateNameA) == 0 )
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