#include "stdafx.h"
#include "CGameLevel.h"

#include "IView.h"

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

// loadLevel
// load all object from file .lv
void CGameLevel::loadLevel( const char *lpLevel )
{
	const char* path = getIView()->getPath( lpLevel );
	io::IReadFile *file = getIView()->getFileSystem()->createAndOpenFile( path );

	if ( file )
	{	
		unsigned long length = file->getSize();
		
		if ( length > 0 )
		{
			m_loadLevelBuffer = new char[length];
			m_loadPos = m_loadLevelBuffer;

			memset( m_loadLevelBuffer, 0, length );

			// read level data
			file->read(m_loadLevelBuffer, length);
		}

		file->drop();
	}	

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
					pGameObj->updateData( &objData );				
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
				}
				else if ( strcmp( objType, strOfType( CGameObject::WaypointObject ) ) == 0 )
				{
					if ( m_loadZone )
					{
						CWayPoint *obj = m_loadZone->createWaypoint();
						obj->updateData( &objData );
					}
				}
				else if ( strcmp( objType, strOfType( CGameObject::CameraObject ) ) == 0 )
				{
					if ( m_loadZone )
					{
						CGameCamera *obj = m_loadZone->createCamera();
						obj->updateData( &objData );
					}
				}
				else if ( strcmp( objType, strOfType( CGameObject::TriggerObject ) ) == 0 )
				{
					if ( m_loadZone )
					{
						CTrigger *obj = m_loadZone->createTrigger();
						obj->updateData( &objData );
					}
				}
				else if ( strcmp( objType, "Game level" ) == 0 )
				{					
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

	return true;

}


// update
// update per frame
void CGameLevel::update()
{		
	// update script
	CScriptManager::getInstance()->update();

	// update all zone
	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{		
		(*iZone)->updateObject();
		iZone++;
	}
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