#include "stdafx.h"
#include "CZone.h"
#include "IView.h"

#ifdef GSEDITOR
#include "IDoc.h"
#include "CDocument.h"
#endif

#include "CObjTemplateFactory.h"
#include "CGameContainerSceneNode.h"

#include "CTerrainComponent.h"
#include "CShadowComponent.h"

#ifdef GSGAMEPLAY
#include "gameLevel/CGameLevel.h"
#include "gameComponent/CBulletRayComponent.h"
#include "gameComponent/CSpawnPointComponent.h"
#endif

CZone::CZone()
{
	m_objectType = CGameObject::ZoneObject;
	
	// root node
	ISceneManager *smgr = getIView()->getSceneMgr();

	// create empty node
	m_node = new CGameContainerSceneNode( this, smgr->getRootSceneNode(), smgr, -1 );

#ifdef GSEDITOR
	m_node->setDebugDataVisible( EDS_BBOX );
#endif

#if defined(GSGAMEPLAY)
	// add particle manager
	m_particleMgr = new CContainerObject(this);
	m_particleMgr->setID( CGameObject::s_objectID++ );
	addChild(m_particleMgr);

	m_shadowMgr = createEmptyObject();
	CShadowComponent *shadowComp = new CShadowComponent(m_shadowMgr);
	shadowComp->initComponent();
	m_shadowMgr->addComponent(shadowComp);

	m_bulletMgr = createEmptyObject();
	CBulletRayComponent* bulletComp = new CBulletRayComponent(m_bulletMgr);
	bulletComp->initComponent();
	m_bulletMgr->addComponent(bulletComp);
    m_bulletMgr->setNetworkGlobalObject(true);
	m_bulletMgr->setSyncTransform(false);

	m_spawnPointMgr = createEmptyObject();
	CSpawnPointMgrComponent *spawnPointMgrComp = new CSpawnPointMgrComponent(m_spawnPointMgr);
	spawnPointMgrComp->initComponent();
	m_spawnPointMgr->addComponent(spawnPointMgrComp);
#endif

}

CZone::~CZone()
{
}

// updateObject
// update object by frame...
void CZone::updateObject()
{	
	m_triggerObjects.clear();

	ArrayGameObjectIter it = m_childs.begin(), end = m_childs.end();
	while ( it != end )
	{
		CGameObject *pObject = (CGameObject*) (*it);
		
		if ( pObject->getObjectType() == TriggerObject )
		{
			m_triggerObjects.push_back( pObject );
		}
		else
		{
			if ( pObject->isEnable() )
				pObject->updateObject();
		}
		it++;
	}

	// now we update trigger LUA later
	it = m_triggerObjects.begin(), end = m_triggerObjects.end();
	while ( it != end )
	{
		CGameObject *pObject = (CGameObject*) (*it);

		if ( pObject->isEnable() )
			pObject->updateObject();

		it++;
	}

	updateAddRemoveObj();
}

// createObject
// create a template object
CGameObject* CZone::createObject( wchar_t* objTemplate )
{
	CGameObject *p = CObjTemplateFactory::spawnObject( objTemplate, this );
	if ( p == NULL )
		return NULL;

	wchar_t lpName[1024];
	swprintf( lpName, 1024, L"%s_%d", objTemplate, (int)CGameObject::s_objectID );
	
	p->setID( CGameObject::s_objectID++ );	
	
#ifdef GSEDITOR
	// create tree item
	uiTreeViewItem *pTreeItem =	m_treeItem->addChild( (LPWSTR) lpName );
	CDocument *pDoc = (CDocument*) getIView()->getDocument();

	pTreeItem->setIconIndex( 4 );
	pTreeItem->setIconStateIndex( 4 );
	pTreeItem->update();

	pTreeItem->setData( p );
	p->setTreeItem( pTreeItem );
	m_treeItem->update();
	m_treeItem->expandChild( true );
#endif

	p->setName( lpName );

	addChild( p );
	
	return p;
}

// createObject
// create a template object
CGameObject* CZone::createObjectWithNoData( wchar_t* objTemplate )
{
	CGameObject *p = CObjTemplateFactory::spawnNullObject( objTemplate, this );
	if ( p == NULL )
		return NULL;

	wchar_t lpName[1024];
	swprintf( lpName, 1024, L"%s_%d", objTemplate, (int)CGameObject::s_objectID );
	
	p->setID( CGameObject::s_objectID++ );	
	
#ifdef GSEDITOR
	// create tree item
	uiTreeViewItem *pTreeItem =	m_treeItem->addChild( (LPWSTR) lpName );
	CDocument *pDoc = (CDocument*) getIView()->getDocument();

	pTreeItem->setIconIndex( 4 );
	pTreeItem->setIconStateIndex( 4 );
	pTreeItem->update();

	pTreeItem->setData( p );
	p->setTreeItem( pTreeItem );
	m_treeItem->update();
	m_treeItem->expandChild( true );
#endif

	p->setName( lpName );

	addChild( p );
	
	return p;
}

#if defined(GSEDITOR) || defined(GSGAMEPLAY)
// createWaypoint
// create a waypoint object
CWayPoint* CZone::createWaypoint()
{
	CWayPoint *p = new CWayPoint(this);
	
	wchar_t lpName[1024];
	swprintf( lpName, 1024, L"%s_%d", L"waypoint", (int)CGameObject::s_objectID );
	
	p->setID( CGameObject::s_objectID++ );
	p->setParent( this );

	
#ifdef GSEDITOR
	// create tree item
	uiTreeViewItem *pTreeItem =	m_treeItem->addChild( (LPWSTR) lpName );
	CDocument *pDoc = (CDocument*) getIView()->getDocument();

	pTreeItem->setIconIndex( 7 );
	pTreeItem->setIconStateIndex( 7 );
	pTreeItem->update();

	pTreeItem->setData( p );
	p->setTreeItem( pTreeItem );
	m_treeItem->update();
	m_treeItem->expandChild( true );
#endif

	p->setName( lpName );

	addChild( p );

	return p;
}


// createTrigger
// create a trigger object
CTrigger* CZone::createTrigger()
{
	CTrigger *p = new CTrigger(this);
	
	wchar_t lpName[1024];
	swprintf( lpName, 1024, L"%s_%d", L"trigger", (int)CGameObject::s_objectID );
	
	p->setID( CGameObject::s_objectID++ );
	p->setParent( this );

#ifdef GSEDITOR
	// create tree item
	uiTreeViewItem *pTreeItem =	m_treeItem->addChild( (LPWSTR) lpName );
	CDocument *pDoc = (CDocument*) getIView()->getDocument();

	pTreeItem->setIconIndex( 5 );
	pTreeItem->setIconStateIndex( 5 );
	pTreeItem->update();

	pTreeItem->setData( p );
	p->setTreeItem( pTreeItem );
	m_treeItem->update();
	m_treeItem->expandChild( true );	
#endif

	p->setName( lpName );
	addChild( p );

	return p;
}

// createLight
// create a light object
CLightObject* CZone::createLight()
{
	CLightObject *p = new CLightObject(this);
	
	wchar_t lpName[1024];
	swprintf( lpName, 1024, L"%s_%d", L"light", (int)CGameObject::s_objectID );
	
	p->setID( CGameObject::s_objectID++ );
	p->setParent( this );

#ifdef GSEDITOR
	// create tree item
	uiTreeViewItem *pTreeItem =	m_treeItem->addChild( (LPWSTR) lpName );
	CDocument *pDoc = (CDocument*) getIView()->getDocument();

	pTreeItem->setIconIndex( 9 );
	pTreeItem->setIconStateIndex( 9 );
	pTreeItem->update();

	pTreeItem->setData( p );
	p->setTreeItem( pTreeItem );
	m_treeItem->update();
	m_treeItem->expandChild( true );	
#endif

	p->setName( lpName );
	addChild( p );

	return p;
}


#endif

// createCamera
// create a cameraObject
CGameCamera* CZone::createCamera()
{
	CGameCamera *p = new CGameCamera();

	wchar_t lpName[1024];
	swprintf( lpName, 1024, L"%s_%d", L"camera", (int)CGameObject::s_objectID );
	
	p->setID( CGameObject::s_objectID++ );
	p->setParent( this );

	
#ifdef GSEDITOR
	// create tree item
	uiTreeViewItem *pTreeItem =	m_treeItem->addChild( (LPWSTR) lpName );
	CDocument *pDoc = (CDocument*) getIView()->getDocument();

	pTreeItem->setIconIndex( 8 );
	pTreeItem->setIconStateIndex( 8 );
	pTreeItem->update();

	pTreeItem->setData( p );
	p->setTreeItem( pTreeItem );
	m_treeItem->update();
	m_treeItem->expandChild( true );	
#endif

	p->setName( lpName );
	addChild( p );

	return p;
}

// saveData
// save data
void CZone::saveData( CSerializable *pObj )
{
	pObj->addGroup	( s_stringObjType[m_objectType] );

	pObj->addLong	("objectID",	m_objectID, true);
	pObj->addString	("objectType",	s_stringObjType[ (int)m_objectType ], true);

	char lpText[1024] = {0};
	uiString::convertUnicodeToUTF8(m_name.c_str(), lpText );
	pObj->addString	("objectName",	lpText, false);

	pObj->addBool	("enable",		m_enable );
	pObj->addBool	("visible",		m_visible );
}

// loadData
// load data
void CZone::loadData( CSerializable *pObj )
{
	pObj->nextRecord();
	
	// object id
	long objectID	= pObj->readLong();
	
	if ( s_repairIDMode == true )
	{
		// save id
		s_mapObjIDOnFileSaved[objectID] = m_objectID;
	}
	else
	{
		m_objectID = objectID;
	}

	// object type
	char *type = pObj->readString();
	for ( int i = 0; i < CGameObject::NumObject; i++ )
	{
		if ( strcmp(s_stringObjType[i], type) == 0 )
		{
			m_objectType = (CGameObject::EObjectType)i;
			break;
		}
	}

	// read obj name
	wchar_t lpText[1024] = {0};
	uiString::convertUTF8ToUnicode( pObj->readString(),lpText );
	setName( lpText );

	// object enable
	m_enable	= pObj->readBool();

	// object visible
	m_visible	= pObj->readBool();
}

// getData
// get data from zone
void CZone::getData( CSerializable* pObj )
{
	pObj->addGroup	( s_stringObjType[m_objectType] );

	pObj->addLong	("objectID",	m_objectID, true);
	pObj->addString	("objectType",	s_stringObjType[ (int)m_objectType ], true);

	char lpText[1024] = {0};
	uiString::convertUnicodeToUTF8(m_name.c_str(), lpText );
	pObj->addString	("objectName",	lpText, false);

	pObj->addBool	("enable",		m_enable );
	pObj->addBool	("visible",		m_visible );	

}

// updateData
// update data
void CZone::updateData( CSerializable* pObj )
{
	pObj->nextRecord();
	
	// object id
	long objectID = pObj->readLong();

	if ( s_repairIDMode == true )
	{
		// save id
		s_mapObjIDOnFileSaved[objectID] = m_objectID;
	}
	else
	{
		m_objectID = objectID;
	}

	// object type
	char *type = pObj->readString();
	for ( int i = 0; i < CGameObject::NumObject; i++ )
	{
		if ( strcmp(s_stringObjType[i], type) == 0 )
		{
			m_objectType = (CGameObject::EObjectType)i;
			break;
		}
	}

	// read obj name
	wchar_t lpText[1024] = {0};
	uiString::convertUTF8ToUnicode( pObj->readString(),lpText );
	setName( lpText );

	// object enable
	m_enable	= pObj->readBool();

	// object visible
	m_visible	= pObj->readBool();
}


// packDataMultiplayer
// pack data multiplayer
void CZone::packDataMultiplayer(CDataPacket *packet)
{
#ifdef HAS_MULTIPLAYER    
    // obj type
    packet->addByte( (unsigned char) m_objectType );
    packet->addInt( (int)m_objectID );    
    
    
    // find object need sync game data
    ArrayGameObject listObjectSync;    
	ArrayGameObjectIter it = m_childs.begin(), end = m_childs.end();
	while ( it != end )
	{
		CGameObject *pObject = (CGameObject*) (*it);
        
		if ( pObject->isEnable() && pObject->isSyncNetwork() && pObject->isNetworkController() == false )
            listObjectSync.push_back(pObject);
        
		it++;
	}
    
    // sync object game data to packet
    packet->addShort((short)listObjectSync.size() );
    it = listObjectSync.begin(), end = listObjectSync.end();
	while ( it != end )
	{
		CGameObject *pObject = (CGameObject*) (*it);
        pObject->packDataMultiplayer(packet);        
        it++;
    }
#endif   
}

// unPackDataMultiplayer
// unpack data on multiplayer
void CZone::unpackDataMultiplayer(CDataPacket *packet, int hostKeyId)
{
#ifdef HAS_MULTIPLAYER
    int nObjectSync = (int)packet->getShort();
	
    for (int i = 0; i < nObjectSync; i++)
    {
        // sync object game data
        // obj type
        int     objType = packet->getByte();
        if ( objType != CGameObject::GameObject )
        {            
            return;
        }
        
        long    networkObjectID = (long)packet->getInt();
        short   templateID = packet->getShort();
        
		// get object template
        wchar_t *templateName = CObjTemplateFactory::getTemplateName(templateID);

     
        bool globalObject = (bool)(packet->getByte() != 0);
        
        CGameObject *obj = NULL;
        
        SNetworkObjID networkObjID;
        networkObjID.hostID = hostKeyId;
        networkObjID.objectID = networkObjectID;

        CGameLevel *level = CGameLevel::getCurrentLevel();
        
        // search network object id
        long objectID = networkObjectID;
        
        if ( globalObject == false )
            objectID = level->getNetworkObjID(networkObjID);
        
        if ( objectID == -1 )
        {
			if ( templateName != NULL )
			{
				// need spawn object
				obj = createObject( templateName );
	            
				char string[512] = {0};
				char temp[512] = {0};
				uiString::convertUnicodeToUTF8(templateName, temp);
	            
				if ( obj == NULL )
				{
					// sync error                
					sprintf(string, "- Network warning: can not create obj template:'%s' from host id: %d", temp, hostKeyId);
					os::Printer::log( string );
					return;
				}
	            
				// set object is create from host
				obj->setNetworkController(true);
				obj->setNetworkObjID(networkObjID);

				// register network object id
				level->registerNetworkObjID(networkObjID, obj->getID());            
				sprintf(string, "- Network warning: create obj template:'%s' with id: %ld from host id: %d", temp, obj->getID(), hostKeyId);
				os::Printer::log( string );
			}
			else
			{
				char string[512] = {0};
				sprintf(string, "- Network warning: Sync error OBJECT TEMPLATE from host id: %d", hostKeyId);
				os::Printer::log( string );
				return;
			}

        }
        else
        {
            obj = level->searchObject(objectID);
            if ( obj == NULL )
            {
                char string[512] = {0}; 
                sprintf(string, "- Network warning: cannot get object %ld, hostid: %d", objectID, hostKeyId);
                os::Printer::log( string );
                
                // sync error
                return;
            }
        }
        
        // unpack object data
        obj->unpackDataMultiplayer( packet, hostKeyId );
        
    }
#endif
}

#if defined(GSEDITOR) || defined(GSGAMEPLAY)
// registerTerrainObj
// add obj to terrain list
void CZone::registerTerrainObj( CGameObject *pObj )
{
	if ( std::find( m_terrains.begin(), m_terrains.end(), pObj ) == m_terrains.end() )
		m_terrains.push_back( pObj );
}

// unRegisterTerrainObj
// remove obj from list terrain
void CZone::unRegisterTerrainObj( CGameObject *pObj )
{
	ArrayGameObjectIter it = std::find( m_terrains.begin(), m_terrains.end(), pObj );
	if ( it != m_terrains.end() )
		m_terrains.erase( it );

}

// getTerrainCollision
// check hit with a ray with a terrain
bool CZone::getTerrainCollision( core::line3df & ray, f32 &outBestDistanceSquared, core::vector3df &outBestCollisionPoint, core::triangle3df &outBestTriangle )
{
	ArrayGameObjectIter it = m_terrains.begin(), end = m_terrains.end();
	while ( it != end )
	{
		CTerrainComponent *terrainComp = (CTerrainComponent*) (*it)->getComponent( IObjectComponent::Terrain );		
		if ( terrainComp->getCollisionFromRay( ray, outBestDistanceSquared, outBestCollisionPoint, outBestTriangle ) == true )
			return true;

		it++;
	}

	return false;
}

// getHeigthFromTerraint
// get height of terrain at position
bool CZone::getHeigthFromTerrain( core::vector3df &position, float *h, core::triangle3df *outTri )
{
	core::line3df ray;

	float outDistance = 10000.0f;

	ray.start = position;
	ray.end = position + core::vector3df(0, -outDistance, 0);

	core::vector3df		collisionPoint;
	core::triangle3df	collisionTriangle;
	
	outDistance *= outDistance;

	if ( getTerrainCollision( ray, outDistance, collisionPoint, collisionTriangle ) == true )
	{
		*h = collisionPoint.Y;

		if ( outTri != NULL )
			*outTri = collisionTriangle;

		return true;
	}

	return false;
}

#endif


#ifdef GSEDITOR

// setUIVisible
// set visible on editor
void CZone::setUIVisible( bool b )
{
	CGameObject::setUIVisible( b );
	ArrayGameObjectIter iObj = m_childs.begin(), end = m_childs.end();
	while ( iObj != end )
	{
		(*iObj)->setUIVisible( b );
		iObj++;
	}
}

#endif