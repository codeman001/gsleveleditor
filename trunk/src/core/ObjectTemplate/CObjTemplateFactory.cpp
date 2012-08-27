#include "stdafx.h"
#include "CObjTemplateFactory.h"
#include "CGameObject.h"
#include "IView.h"

// global object template
std::vector<CObjectTemplate> CObjTemplateFactory::s_objectTemplate;

#ifdef GSEDITOR
std::vector<CGameObject*>	CObjTemplateFactory::s_objects;
#endif

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

#ifdef GSEDITOR
			// create game object
			CGameObject *pObj = CObjTemplateFactory::spawnObject( lpString );
			pObj->setObjectState( CGameObject::Move );
			CObjTemplateFactory::addTemplateObject( pObj );
#endif

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
	// todo load file
	const char* path = getIView()->getPath( "objectTemplate.css" );
	io::IReadFile *file = getIView()->getFileSystem()->createAndOpenFile( path );

	if ( file )
	{	
		unsigned long length = file->getSize();
		
		if ( length > 0 )
		{
			char *lpBuffer = new char[length];
			memset( lpBuffer, 0, length );

			file->read(lpBuffer,length);			

			readTemplateFromData( lpBuffer );

			delete lpBuffer;
		}

		file->drop();
				
	}	
}

// getTemplate
// get template
CObjectTemplate* CObjTemplateFactory::getTemplate( wchar_t* templateName )
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
			return p;
		}
		i++;
	}
	return NULL;
}

// spawnObject
// create a object on template
CGameObject* CObjTemplateFactory::spawnObject( wchar_t* templateName )
{
	CObjectTemplate *pTemplate = getTemplate( templateName );
	if ( pTemplate == NULL )
		return NULL;

	CGameObject *pObj = new CGameObject();
	ArraySerializable::iterator i =	pTemplate->getAllComponentProperty()->begin(), 
		end = pTemplate->getAllComponentProperty()->end();

	while ( i != end )
	{
		pObj->createComponent( &(*i ) );
		i++;
	}

	pObj->initComponent();
	pObj->setVisible( false );
	pObj->setObjectTemplate( templateName );
	pObj->setLighting( false );
	return pObj;
}

// spawnObject
// create a object on template with parent
CGameObject* CObjTemplateFactory::spawnObject( wchar_t* templateName, CGameObject *parent )
{
	CObjectTemplate *pTemplate = getTemplate( templateName );
	if ( pTemplate == NULL )
		return NULL;

	CGameObject *pObj = new CGameObject(parent);
	ArraySerializable::iterator i =	pTemplate->getAllComponentProperty()->begin(), 
		end = pTemplate->getAllComponentProperty()->end();

	while ( i != end )
	{
		pObj->createComponent( &(*i ) );
		i++;
	}

	pObj->initComponent();
	pObj->setVisible( false );
	pObj->setObjectTemplate( templateName );
	pObj->setLighting( false );
	return pObj;
}


// saveAllObjectTemplate
// save obj template to file
void CObjTemplateFactory::saveAllObjectTemplate()
{
	wchar_t path[512] = {0};
	wchar_t fileName[512] = {0};

	std::ofstream file( getIView()->getPath("objectTemplate.css") );
	
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

#ifdef GSEDITOR
// getGameObject
// get a template object
CGameObject* CObjTemplateFactory::getGameObject( wchar_t* templateName )
{
	ArrayGameObjectIter i = s_objects.begin(), end = s_objects.end();
	while ( i != end )
	{
		CGameObject *pObject = (*i);
		if ( wcscmp( pObject->getTemplate(), templateName ) == 0 )
		{
			return pObject;
		}
		i++;
	}
	return NULL;
}

// addTemplateObject
// add a template object
void CObjTemplateFactory::addTemplateObject( CGameObject *pObj )
{
	s_objects.push_back( pObj );
}

// registerDrawAllObject
// add all to scene node
void CObjTemplateFactory::registerDrawAllTemplateObject()
{
	ISceneManager *pSceneMgr = getIView()->getSceneMgr();

	ArrayGameObjectIter i = s_objects.begin(), end = s_objects.end();
	while ( i != end )
	{
		CGameObject *pObject = (*i);
		
		ISceneNode *pNode = pObject->getSceneNode();
		if ( pNode )
			pSceneMgr->getRootSceneNode()->addChild( pNode );

		i++;
	}	
}
#endif

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

#ifdef GSEDITOR
	ArrayGameObjectIter i = s_objects.begin(), end = s_objects.end();
	while ( i != end )
	{
		CGameObject *pObject = (*i);
		if ( pObject )
			delete pObject;
		i++;
	}
	s_objects.clear();
#endif

}