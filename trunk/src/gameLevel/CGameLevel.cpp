#include "stdafx.h"
#include "CGameLevel.h"
#include "LevelScript.h"
#include "IView.h"

#include "CTerrainComponent.h"

/////////////////////////////////////////////////////////
// CGameLevel static function implement
/////////////////////////////////////////////////////////

CGameLevel* g_currentLevel = NULL;
std::map<std::string, std::string>	g_levelProperty;

// getCurrentLevel
// get current level
CGameLevel* CGameLevel::getCurrentLevel()
{
	return g_currentLevel;
}

// setCurrentLevel
// set current level
void CGameLevel::setCurrentLevel(CGameLevel* lv)
{
	g_currentLevel = lv;
}

// setLevelProperty
// set level tag property
void CGameLevel::setLevelProperty(const char *lpProperty, const char *lpValue)
{
	g_levelProperty[ lpProperty ] = lpValue;
}

// getLevelProperty
// get proprety tag
const char* CGameLevel::getLevelProperty(const char *lpProperty)
{	
	if ( g_levelProperty.find( std::string(lpProperty) ) == g_levelProperty.end() )
		return NULL;

	return g_levelProperty[lpProperty].c_str();
}


/////////////////////////////////////////////////////////
// CGameLevel implement
/////////////////////////////////////////////////////////

CGameLevel::CGameLevel()
{
	m_loadLevelBuffer = NULL;
	m_loadPos = NULL;
	m_loadZone = NULL;

	m_activeZone	= NULL;
	m_activeCamera	= NULL;
}

CGameLevel::~CGameLevel()
{
	releaseLevel();
	CScriptManager::getInstance()->reset();
}

// createZone
// create a zone
CGameObject* CGameLevel::createZone()
{
	CZone *pZone = new CZone();
		
	// set up name
	wchar_t name[512];
	swprintf(name, 512, L"zone%d", (int)CGameObject::s_objectID );

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
	uiString::convertUTF8ToUnicode(name, buffer);

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
			file->read(m_loadLevelBuffer, (u32)length);

			// end buffer
			m_loadLevelBuffer[length] = NULL;
		}

		file->drop();
	}	
	
	m_numObjects = 0;
	m_numObjectsLoaded = 0;

	// init id
	CGameObject::s_objectID = 1;
	CGameObject::s_mapObjIDOnFileSaved.clear();
	CGameObject::s_repairIDMode = true;

	m_activeZone	= NULL;
	m_activeCamera	= NULL;
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

	while ( *p != 0 )
	{
		if ( *p == '{' )
		{
			getBufferString( lpStringA, from, p );
			uiString::convertUTF8ToUnicode(lpStringA, lpString );
						
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
				uiString::convertUTF8ToUnicode(r->data, lpString);

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

					// set first zone is active zone
					if ( m_activeZone == NULL )
						m_activeZone = m_loadZone;
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
				else if ( strcmp( objType, strOfType( CGameObject::LightObject ) ) == 0 )
				{
					if ( m_loadZone )
					{
						CLightObject *obj = m_loadZone->createLight();
						obj->updateData( &objData );
						
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

				// add child
				if ( m_loadZone )
					m_loadZone->updateAddRemoveObj();

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
	
	// turn off repair id mode
	CGameObject::s_repairIDMode = false;
	CGameObject::s_mapObjIDOnFileSaved.clear();

	// load lua script
	compileGameScript();

	return true;

}

// addScriptFile
// add file script to compile
void CGameLevel::addScriptFile(const std::string& path)
{
	int numScriptFile = (int)m_listScriptFile.size();
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

	char folderPath[1024];
	char fullPath[1024];
	uiString::getFolderPath<const char, char>( m_levelFile.c_str(), folderPath );
	uiString::cat<char, const char>(folderPath,"/");
	
	// add lua component
	m_listScriptFile.push_back("../script/VectorUtils.lua");
	m_listScriptFile.push_back("../script/PlayerComponent.lua");

		
	int numScriptFile = (int)m_listScriptFile.size();
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
				file->read(m_loadLevelBuffer, (u32)length);
			}

			file->drop();

			// compile lua source
			CScriptManager::getInstance()->compileLuaSource( m_loadLevelBuffer, length );			
		}	

	}

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

// packDataMultiplayer
// pack data multiplayer
void CGameLevel::packDataMultiplayer(CDataPacket *packet)
{
#ifdef HAS_MULTIPLAYER    
    // add number zone
    packet->addByte((unsigned char)m_zones.size() );
    
    // pack data all zone
    ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{		
		(*iZone)->packDataMultiplayer(packet);
		iZone++;
	}
#endif
}

// unPackDataMultiplayer
// unpack data on multiplayer
void CGameLevel::unpackDataMultiplayer(CDataPacket *packet, int hostKeyId)
{
#ifdef HAS_MULTIPLAYER
    int nZone = (int)packet->getByte();
    if ( nZone < 0 || nZone > (int)m_zones.size() )
    {
        // sync error
        return;
    }
    
    // loop in all zone
    for (int i = 0; i < nZone; i++)
    {
        // read zone infomation
        int zoneType = packet->getByte();
        if ( zoneType != CGameObject::ZoneObject )
        {
            // sync error
            return;
        }
        
        // get zone id
        int zoneid = packet->getInt();
        
        CZone *zone = m_zones[i];        
        if ( zone->getID() != zoneid )
        {
            // sync error
            return;
        }
        
        // unpack zone game data
        zone->unpackDataMultiplayer( packet, hostKeyId );        
    }
#endif
}

long CGameLevel::getNetworkObjID( SNetworkObjID& networkID )
{
    if ( m_mapNetworkObjID.find(networkID) == m_mapNetworkObjID.end() )
        return  -1;
    
    return m_mapNetworkObjID[networkID];
}

void CGameLevel::registerNetworkObjID( SNetworkObjID& networkID, long objectID )
{
    m_mapNetworkObjID[networkID] = objectID;
}

void CGameLevel::unRegisterNetworkObjID( SNetworkObjID& networkID )
{
    m_mapNetworkObjID.erase(networkID);
}

// removeDisconectedObject
// remove all object at host is disconected
void CGameLevel::removeDisconectedObject( int hostKeyId )
{
    std::map<SNetworkObjID, long>::iterator i = m_mapNetworkObjID.begin(), end = m_mapNetworkObjID.end();
        
    std::vector<SNetworkObjID>  listDeleted;
    
    // remove object
    while ( i != end )
    {
        if ( i->first.hostID == hostKeyId )
        {
            long objectID = i->second;
            CGameObject *obj = searchObject( objectID);
            if ( obj )
            {
                // delete object
                obj->remove();
            }
            listDeleted.push_back( i->first );
        }
        i++;
    }
    
    // remove key map
    std::vector<SNetworkObjID>::iterator it = listDeleted.begin(), endit = listDeleted.end();
    while ( it != endit )
    {
        m_mapNetworkObjID.erase( (*it) );
        it++;
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
	m_activeCamera = cam;
}

// getCamera
// get current camera
CGameCamera* CGameLevel::getCamera()
{
	return getIView()->getActiveCamera();
}

// checkCollide
// check terrain collide
bool CGameLevel::checkTerrainCollide( core::line3df& ray, core::vector3df& outPos, core::triangle3df& outTri )
{	
	core::aabbox3df myBox(ray.start, ray.end);	
	myBox.repair();
	
	core::triangle3df	colTri;
	core::vector3df		colPos;

	bool	hasCol = false;

	float	maxLength	= ray.getLengthSQ();		
	float	minDistance = ray.getLength();

	// loop for all zone
	int nZone = getZoneCount();
	for ( int iZone = 0; iZone < nZone; iZone++ )
	{
		CZone *z = getZone( iZone );
		core::aabbox3df zoneBox = z->getSceneNode()->getTransformedBoundingBox();		
		
		if ( zoneBox.intersectsWithBox( myBox ) == true )
		{
			// loop all terrain object in zone
			ArrayGameObject& terrains = z->getTerrainList();
			ArrayGameObject::iterator itTerrain = terrains.begin(), itTerrainEnd = terrains.end();
			while ( itTerrain != itTerrainEnd )
			{
				CGameObject *objTerrain = ((CGameObject*) *itTerrain);

				// loop all scene node in a terrain object
				CTerrainComponent* com  = (CTerrainComponent*)objTerrain->getComponent( IObjectComponent::Terrain );
				if ( com )
				{
					core::line3df colRay = ray;

					if ( com->getCollisionFromRay(colRay, maxLength, colPos, colTri) == true )
					{
						float distance = colPos.getDistanceFrom(ray.start);
						if ( distance < minDistance ) 
						{							
							minDistance = distance;
							hasCol = true;

							outPos = colPos;
							outTri = colTri;
						}
					}
				}
				
				itTerrain++;
			}
		}

	}

	return hasCol;
}