#include "stdafx.h"
#include "CGameLevel.h"
#include "LevelScript.h"
#include "IView.h"

CGameLevel* g_currentLevel = NULL;
std::string	g_levelLoadFile;

// getCurrentLevel
// get current level
CGameLevel* CGameLevel::getCurrentLevel()
{
	return g_currentLevel;
}

// setLevelLoad
// set level will load
void CGameLevel::setLevelLoad(const char *lvFile )
{
	g_levelLoadFile = lvFile;
}

// getLevelLoadFile
// get level load file
const char* CGameLevel::getLevelFileToLoad()
{
	return g_levelLoadFile.c_str();
}

// setCurrentLevel
// set current level
void CGameLevel::setCurrentLevel(CGameLevel* lv)
{
	g_currentLevel = lv;
}

CGameLevel::CGameLevel()
{
	m_loadLevelBuffer = NULL;
	m_loadPos = NULL;
	m_loadZone = NULL;
}

CGameLevel::~CGameLevel()
{
	releaseLevel();
}

// createZone
// create a zone
CGameObject* CGameLevel::createZone()
{
	CZone *pZone = new CZone();
		
	// set up name
	wchar_t name[512];
	swprintf(name, L"zone%d", (int)CGameObject::s_objectID );

	// create name + id
	pZone->setName( name );
	pZone->setID( CGameObject::s_objectID++ );

	// add zone to document
	m_zones.push_back( pZone );
	return pZone;
}


// removeZone
// remove zone
void CGameLevel::removeZone(CGameObject* zone)
{
	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{
		CZone *p = (CZone*) (*iZone);
		if ( p == zone )
		{			
			delete p;
			m_zones.erase( iZone );
			return;
		}
		iZone++;
	}
}

// removeAllZone
// remove all zone on level
void CGameLevel::releaseLevel()
{
	// stop all lua script func
	CScriptManager::getInstance()->stopAllFunc();

	// release all zone
	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{
		CZone *p = (CZone*) (*iZone);		
		delete p;
		iZone++;
	}
	m_zones.clear();	
}

// searchObjByID
// search object with id
CGameObject* CGameLevel::searchObject( long id )
{
	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{		
		CZone *pZone = (*iZone);
		
		if ( pZone->getID() == id )
			return pZone;

		CGameObject* p = pZone->searchObject( id );
		
		if ( p )
			return p;

		iZone++;
	}

	return NULL;
}

// searchObject
// search object with name
CGameObject* CGameLevel::searchObject( const char* name )
{
	wchar_t buffer[1024] = {0};
	uiString::convertUTF8ToUnicode( name, (unsigned short*) buffer );

	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{		
		CZone *pZone = (*iZone);
		
		if ( uiString::comp<const wchar_t>(pZone->getName(), buffer) == 0 )
			return pZone;

		CGameObject* p = pZone->searchObject( buffer );
		
		if ( p )
			return p;

		iZone++;
	}

	return NULL;
}


// loadLevel
// load all object from file .lv
void CGameLevel::loadLevel( const char *lpLevel )
{
	m_levelFile = lpLevel;

	const char* path = getIView()->getPath( lpLevel );
	io::IReadFile *file = getIView()->getFileSystem()->createAndOpenFile( path );

	if ( file )
	{	
		unsigned long length = file->getSize();
		
		if ( length > 0 )
		{
			// init level buffer
			m_loadLevelBuffer = new char[length+1];
			m_loadPos = m_loadLevelBuffer;

			// read level data
			file->read(m_loadLevelBuffer, length);

			// end buffer
			m_loadLevelBuffer[length] = NULL;
		}

		file->drop();
	}	
	
	m_numObjects = 0;
	m_numObjectsLoaded = 0;

}

extern void getBufferString( char *lpBuffer, char *from, char *to );

// loadStep
// load level step
bool CGameLevel::loadStep( int nStep )
{
	if ( m_loadLevelBuffer == NULL )
		return true;
	
	int step = 0;

	char *p = m_loadPos;
	char *from = p;

	char	lpStringA[1024];
	wchar_t	lpString[1024];

	while ( *p != NULL )
	{
		if ( *p == '{' )
		{
			getBufferString( lpStringA, from, p );
			uiString::convertUTF8ToUnicode( lpStringA, (unsigned short*) lpString );
						
			p++;
			
			// read data object
			CSerializable	objData;
			while ( objData.readData( p ) )
			{
			}
		
			// parse object info
			SSerializableRec *r = objData.getProperty("objectTemplate");
			if ( r != NULL && m_loadZone && uiString::length(r->data) > 0 )
			{
				uiString::convertUTF8ToUnicode( r->data, (unsigned short*) lpString );

				// create game object
				CGameObject *pGameObj = m_loadZone->createObject( lpString );
				if ( pGameObj )
				{
					// update obj visible, position, rotation
					pGameObj->updateData( &objData );

					// visible object collision
					if ( pGameObj->getComponent( IObjectComponent::Terrain ) )
						pGameObj->setVisible( true );
				}

				// register name for search object by name
				m_loadZone->registerObjectName( pGameObj );
				m_numObjectsLoaded++;
			}
			else
			{
				#define strOfType(type)		( CGameObject::s_stringObjType[ (int)type ] )

				char *objType = objData.getAllRecord()->front().data;
				if ( strcmp( objType, strOfType( CGameObject::ZoneObject ) ) == 0 )
				{
					// create zone
					m_loadZone = (CZone*)createZone();
					m_loadZone->updateData( &objData );

					// register name for search object by name
					m_loadZone->registerObjectName( m_loadZone );
					m_numObjectsLoaded++;
				}
				else if ( strcmp( objType, strOfType( CGameObject::WaypointObject ) ) == 0 )
				{
					if ( m_loadZone )
					{
						CWayPoint *obj = m_loadZone->createWaypoint();
						obj->updateData( &objData );
						obj->setVisible( false );
						
						// register name for search object by name
						m_loadZone->registerObjectName( obj );
						m_numObjectsLoaded++;
					}
				}
				else if ( strcmp( objType, strOfType( CGameObject::CameraObject ) ) == 0 )
				{
					if ( m_loadZone )
					{
						CGameCamera *obj = m_loadZone->createCamera();
						obj->updateData( &objData );

						// register name for search object by name
						m_loadZone->registerObjectName( obj );
						m_numObjectsLoaded++;
					}
				}
				else if ( strcmp( objType, strOfType( CGameObject::TriggerObject ) ) == 0 )
				{
					if ( m_loadZone )
					{
						CTrigger *obj = m_loadZone->createTrigger();
						obj->updateData( &objData );
						obj->setVisible( false );
						
						if ( obj->getScripFile().size() > 0 )
						{
							addScriptFile( obj->getScripFile() );
						}

						// register name for search object by name
						m_loadZone->registerObjectName( obj );
						m_numObjectsLoaded++;
					}
				}
				else if ( strcmp( objType, "Game level" ) == 0 )
				{
					SSerializableRec* r = objData.getProperty("numberObjects");
					if ( r )
						sscanf( r->data, "%d", &m_numObjects );
				}
			}

			objData.clear();
		}
		else if ( *p == '}' )
		{
			p++;
			from = p;

			if ( ++step == nStep )
			{
				m_loadPos = p;
				return false;
			}
		}
		else
		{
			p++;
		}
	}

	delete m_loadLevelBuffer;
	
	m_loadLevelBuffer = NULL;
	m_loadPos = NULL;
	m_loadZone = NULL;

	// load lua script
	compileGameScript();

	return true;

}

// addScriptFile
// add file script to compile
void CGameLevel::addScriptFile(const std::string& path)
{
	int numScriptFile = m_listScriptFile.size();
	for ( int i = 0; i < numScriptFile; i++ )
	{
		if ( path == m_listScriptFile[i] )
			return;
	}

	m_listScriptFile.push_back( path );
}

// getLoadingPercent
// get percent of load level
int CGameLevel::getLoadingPercent()
{
	if ( m_numObjects == 0 )
		return 0;
	float f = m_numObjectsLoaded/(float)m_numObjects;
	return (int)(f*100);
}

// compileGameScript
// compile lua script
void CGameLevel::compileGameScript()
{
	// cache 2mb script
	m_loadLevelBuffer = new char[2*1024*1024];
	m_loadPos = m_loadLevelBuffer;

	char folderPath[1024];
	char fullPath[1024];
	uiString::getFolderPath<const char, char>( m_levelFile.c_str(), folderPath );
	uiString::cat<char,char>(folderPath,"/");
	
	int numScriptFile = m_listScriptFile.size();
	for ( int i = 0; i < numScriptFile; i++ )
	{
		uiString::copy<char,char>(fullPath,folderPath);
		uiString::cat<char,const char>(fullPath, m_listScriptFile[i].c_str());

		io::IReadFile *file = getIView()->getFileSystem()->createAndOpenFile( getIView()->getPath(fullPath) );
		if ( file )
		{	
			unsigned long length = file->getSize();
			
			if ( length > 0 )
			{				
				// read level data
				file->read(m_loadPos, length);
			}

			m_loadPos[length++] = '\n';
			m_loadPos[length]	= NULL;
			m_loadPos += length;
			file->drop();
		}	

	}

	// compile lua source
	CScriptManager::getInstance()->compileLuaSource( m_loadLevelBuffer, m_loadPos - m_loadLevelBuffer );

	// implement function
	registerCFunction();
	

	delete m_loadLevelBuffer;
	m_loadLevelBuffer = NULL;
	m_loadPos = NULL;
}


// update
// update per frame
void CGameLevel::update()
{	
	// update all zone
	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{		
		(*iZone)->updateObject();
		iZone++;
	}
	// update script
	CScriptManager::getInstance()->update();
}

// render
// render level per frame
void CGameLevel::render()
{
	// draw all 3d scene
	getIView()->getSceneMgr()->drawAll();
}

// setCamera
// set current camera
void CGameLevel::setCamera( CGameCamera* cam )
{
	getIView()->setActiveCamera( cam );
}

// getCamera
// get current camera
CGameCamera* CGameLevel::getCamera()
{
	return getIView()->getActiveCamera();
}