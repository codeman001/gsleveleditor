#include "stdafx.h"
#include "CDocument.h"
#include "IView.h"
#include "IController.h"

#include "CComponentFactory.h"
#include "CObjTemplateFactory.h"

#include "CGameOxyzSceneNode.h"
#include "CGameGSCameraAnimator.h"
#include "CShadowComponent.h"
#include "CParticleComponent.h"

static bool s_isFirstDocument = true;

CDocument::CDocument()
{
	
}

CDocument::~CDocument()
{
	// delete camera
	delete m_designCamera;

	// set null camera
	getIView()->getSceneMgr()->setActiveCamera(NULL);

	// delete grid
	m_gridNode->drop();

	// delete all zone
	removeAllZone();

	// delete all item
	getIView()->getDocumentTreeView()->deleteAllItem();

	// it will make game crash (drop all node on controller)
	// so i am remove it!
	// remove all child
	// getIView()->getSceneMgr()->getRootSceneNode()->removeAll();
	
}

// newDocument
// new 3d scene document
void CDocument::newDocument()
{		
	ISceneManager *smgr = getIView()->getSceneMgr();
	IrrlichtDevice *device = getIView()->getDevice();

	// clear all history
	CHistoryManager::getInstance()->clearAll();

	// create design camera
	m_designCamera = new CGameCamera();
	m_designCamera->setName( L"Design camera" );
	m_designCamera->setTarget(core::vector3df(300,100,300));
	m_designCamera->setPosition( core::vector3df(0, 200, 0) );	
	getIView()->setActiveCamera( m_designCamera );
		
	// add oxyz plane node
	CGameOxyzSceneNode *oxyPlane = new CGameOxyzSceneNode( smgr->getRootSceneNode(), smgr, 1 );
	m_gridNode = oxyPlane;

	// add light
	ISceneNode* pNode = smgr->addEmptySceneNode();

	// default light
	smgr->setAmbientLight( video::SColorf(0.2f, 0.2f, 0.2f ,1.0f) );
	//ILightSceneNode *light = smgr->addLightSceneNode( pNode, core::vector3df(0, 0, 0) );
	//light->setLightType( video::ELT_DIRECTIONAL );

	// reset file path
	m_filePath = L"";

	// begin id
	CGameObject::s_objectID = 1;

	// add root
	const WCHAR *lpRootName = L"level";
	uiTreeView *pTreeview = getIView()->getDocumentTreeView();
	m_treeViewRoot = pTreeview->addItem( (WCHAR*)lpRootName );	

	// create a zone
	CZone *pZone = (CZone*)createZone();
	getIView()->setCurrentZone( pZone );
	getIView()->setObjectProperty( pZone );

	m_treeViewRoot->expandChild( true );

	// register draw all template obj
	if ( s_isFirstDocument == true )
	{
		CObjTemplateFactory::registerDrawAllTemplateObject();
		s_isFirstDocument = false;
	}

	// have 1 zone
	m_totalObjects = 1;
}

// initBeginSaveLevel
// sort & count object when save level
void CDocument::initBeginSaveLevel()
{
	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	m_totalObjects = 0;

	while ( iZone != iEnd )
	{		
		// write to file zone info
		CZone *pZone = (*iZone);
		
		m_totalObjects++;
		pZone->sortObjectByID();		
		m_totalObjects += pZone->getNumberObjects();

		iZone++;
	}
}

// saveDocument
// save the document
bool CDocument::saveDocument(wchar_t* lpPath)
{
	m_filePath = lpPath;
	ofstream file( lpPath );
	if ( file.is_open() == false )
		return false;
	
	initBeginSaveLevel();

	CSerializable serializable;

	char lpString[1024];

	// need save level info
	getData( &serializable );	
	file << "Game\n";
	file << "{\n";
	serializable.saveData( file, 1 );
	serializable.clear();
	file << "}\n";

	// update all zone
	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{		
		// write to file zone info
		CZone *pZone = (*iZone);

		pZone->sortObjectByID();		
		pZone->getData( &serializable );
		
		// write zone info		
		sprintf(lpString, "zone_%d", pZone->getID());
		file << lpString << "\n";
		file << "{\n";
		serializable.saveData( file, 1 );
		file << "}\n";
		
		// write object info
		ArrayGameObjectIter i = pZone->getChilds()->begin(), end = pZone->getChilds()->end();
		while ( i != end )
		{
			CGameObject *pGameObj = (CGameObject*)(*i);

			if ( pGameObj->isDummyObject() == false )
			{
				serializable.clear();
				pGameObj->getData( &serializable );
				
				sprintf(lpString, "object_%d", pGameObj->getID());
				file << lpString << "\n";
				file << "{\n";
				serializable.saveData( file, 1 );
				file << "}\n";
			}

			i++;
		}

		serializable.clear();
		iZone++;
	}

	file.close();
	return true;
}

extern void getBufferString( char *lpBuffer, char *from, char *to );

// readDocumentFromData
// parse document from data
bool CDocument::readDocumentFromData( char *lpData )
{
	char *p = lpData;
	char *from = p;

	char	lpStringA[1024];
	wchar_t	lpString[1024];

	CZone	*currentZone = NULL;
	
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
			if ( r != NULL && currentZone && uiString::length(r->data) > 0 )
			{
				uiString::convertUTF8ToUnicode( r->data, (unsigned short*) lpString );

				// create game object
				CGameObject *pGameObj = currentZone->createObject( lpString );
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
					currentZone = (CZone*)createZone();
					currentZone->updateData( &objData );
				}
				else if ( strcmp( objType, strOfType( CGameObject::WaypointObject ) ) == 0 )
				{
					if ( currentZone )
					{
						CWayPoint *obj = currentZone->createWaypoint();
						obj->updateData( &objData );
					}
				}
				else if ( strcmp( objType, strOfType( CGameObject::CameraObject ) ) == 0 )
				{
					if ( currentZone )
					{
						CGameCamera *obj = currentZone->createCamera();
						obj->updateData( &objData );
					}
				}
				else if ( strcmp( objType, strOfType( CGameObject::TriggerObject ) ) == 0 )
				{
					if ( currentZone )
					{
						CTrigger *obj = currentZone->createTrigger();
						obj->updateData( &objData );
					}
				}
				else if ( strcmp( objType, strOfType( CGameObject::LightObject ) ) == 0 )
				{
					if ( currentZone )
					{
						CLightObject *obj = currentZone->createLight();
						obj->updateData( &objData );
					}
				}
				else if ( strcmp( objType, "Game level" ) == 0 )
				{
					updateData( &objData );
				}
			}

			objData.clear();
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

	return true;
}

// openDocument
// load document
bool CDocument::openDocument(wchar_t* lpPath)
{
	m_filePath = lpPath;
	ifstream file( lpPath );
	if ( file.is_open() == false )
		return false;
	
	file.seekg (0, ios::end);
	unsigned long length = file.tellg();
	file.seekg (0, ios::beg);
		
	char *lpBuffer = new char[length];
	memset( lpBuffer, 0, length );

	file.read(lpBuffer,length);
	file.close();

	// remove old zone
	removeAllZone();

	bool ret = readDocumentFromData( lpBuffer );

	// expand child
	m_treeViewRoot->expandChild(true);

	// select level
	getIView()->setObjectProperty(NULL);

	delete lpBuffer;		
	return ret;
}

// isEmptyDocument
// return true if document is nothing
bool CDocument::isEmptyDocument()
{
	return false;
}

// getCurrentFile
// return current file is open
wchar_t* CDocument::getCurrentFile()
{
	if ( m_filePath.size() == 0 )
		return NULL;

	return (wchar_t*) m_filePath.c_str();
}

// updateDocument
// update all object in doc by frame...
void CDocument::updateDocument()
{		
	// update time
	IrrlichtDevice *device = getIView()->getDevice();
	device->getTimer()->tick();

	// update design
	m_designCamera->updateObject();

	// update all zone
	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{		
		(*iZone)->updateObject();
		iZone++;
	}
}

// drawDocument
// draw documen by frame
void CDocument::drawDocument()
{
	// draw document
	IVideoDriver* driver = getIView()->getDriver();
	ISceneManager* smgr = getIView()->getSceneMgr();

	// draw 3d scene
	driver->beginScene( true, true, SColor(255,30,30,30), getIView()->getVideoData() );
		
	// draw all scene
	smgr->drawAll();

	// controller draw imp
	CControllerManager::getInstance()->getCurrentController()->render();
	
	int fps = driver->getFPS();
	core::stringw text(L"fps: ");
	text += fps;
	text += L" frame/second";

	irr::gui::IGUIFont* font = smgr->getGUIEnvironment()->getBuiltInFont();
	font->draw( text.c_str(), core::recti( 10, 10, 200, 20), video::SColor(255, 255,0,0) );

	// end scene
	driver->endScene();
}

// searchObjByID
// search object with id
CGameObject* CDocument::searchObject( long id )
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

// createZone
// create a zone
CGameObject* CDocument::createZone()
{
	CZone *pZone = new CZone();
		
	// set up name
	uiStringW name;
	name.format( L"zone%d", (int)CGameObject::s_objectID );

	// create name + id
	pZone->setName( name.c() );
	pZone->setID( CGameObject::s_objectID++ );

	// create tree item
	uiTreeViewItem *pTreeItem =	m_treeViewRoot->addChild( (LPWSTR) name.c() );
	
	pTreeItem->setIconIndex( 1 );
	pTreeItem->setIconStateIndex( 2 );
	pTreeItem->update();

	pTreeItem->setData( pZone );

	pZone->setTreeItem( pTreeItem );

	// add zone to document
	m_zones.push_back( pZone );
	return pZone;
}


// removeZone
// remove zone
void CDocument::removeZone(CGameObject* zone)
{
	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{
		CZone *p = (CZone*) (*iZone);
		if ( p == zone )
		{
			m_treeViewRoot->destroyChild( p->getTreeItem() );
			delete p;
			m_zones.erase( iZone );
			return;
		}
		iZone++;
	}
}

// removeAllZone
// remove all zone on level
void CDocument::removeAllZone()
{
	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{
		CZone *p = (CZone*) (*iZone);		
		m_treeViewRoot->destroyChild( p->getTreeItem() );
		delete p;
		iZone++;
	}
	m_zones.clear();
}


// selectObject
// detect list objs at mouse xy	
void CDocument::selectObject( int mouseX, int mouseY, bool isControlHold )
{	
	ISceneManager *smgr = getIView()->getSceneMgr();	
	
	ICameraSceneNode *camera = smgr->getActiveCamera();

	// if no camera
	if (  camera == NULL )
		return;

	ISceneCollisionManager *collMan = smgr->getSceneCollisionManager();

	// get select ray
	core::line3df selectRay = getIView()->getSelectRay();
	
	// check hit test
	core::vector3df intersection;
	core::triangle3df hitTriangle;

	// check select
	ISceneNode *selectedSceneNode = collMan->getSceneNodeAndCollisionPointFromRay
		(
			selectRay,
			intersection,
			hitTriangle
		);
	
	if ( selectedSceneNode )
	{
		CGameObject *pObj =	searchObject( selectedSceneNode->getID() );

		// add to select list
		if ( pObj && pObj->isVisible() )
		{
			if ( isControlHold == false || pObj->getObjectState() == CGameObject::Normal )
				m_selectObjects.push_back( pObj );
			else
			{
				pObj->setObjectState( CGameObject::Normal );
				ArrayGameObjectIter i = m_selectObjects.begin(), iEnd = m_selectObjects.end();
				while ( i != iEnd )
				{
					if ( (*i) == pObj )
					{
						m_selectObjects.erase( i );
						break;
					}
					i++;
				}
			}
		}

	}

}

// selectObject
// detect list objs at rect
void CDocument::selectObject( int x, int y, int w, int h, bool isControlHold )
{
	IView *pView = getIView();
	ISceneManager *smgr = pView->getSceneMgr();	
	
	ICameraSceneNode *camera = smgr->getActiveCamera();

	// if no camera
	if (  camera == NULL )
		return;

	const SViewFrustum* viewFrustum = camera->getViewFrustum();
	ISceneCollisionManager *collMan = smgr->getSceneCollisionManager();

	int screenX = -1, screenY = -1;
		
	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{
		ArrayGameObject* listObj = (*iZone)->getChilds();
		ArrayGameObjectIter iObj = listObj->begin(), objEnd = listObj->end();
		ISceneNode *pNode = NULL;

		while ( iObj != objEnd )
		{
			CGameObject *pGameObj = (CGameObject*)(*iObj);
			
			pNode = pGameObj->getSceneNode();
			
			if ( pNode != NULL && pGameObj->isVisible() )
			{
				core::vector3df center = pGameObj->getPosition();

				// check object is in frustum
				if ( viewFrustum->getBoundingBox().isPointInside( center ) )
				{					
					if ( pView->getScreenCoordinatesFrom3DPosition( center, &screenX, &screenY ) )
					{
						if ( x <= screenX && screenX <= x + w && y <= screenY && screenY <= y + h )
						{
							
							if ( isControlHold == false || pGameObj->getObjectState() == CGameObject::Normal )
								m_selectObjects.push_back( pGameObj );							

						}	// inselect
					}	// getScreenCoordinatesFrom3DPosition			
				} // frustum
			}

			iObj++;
		}

		iZone++;
	}

}

// saveListSelectObj
// save list select obj
void CDocument::saveListSelectObj()
{
	m_backupSelectObj.clear();
	m_backupSelectObj = m_selectObjects;
}

// loadListSelectObj
// restore list select obj
void CDocument::loadListSelectObj()
{
	m_selectObjects.clear();
	m_selectObjects = m_backupSelectObj;
}

// setStateForSelectObject
// set state for select object
void CDocument::setStateForSelectObject( CGameObject::ObjectState state )
{
	ArrayGameObjectIter i = m_selectObjects.begin(), iEnd = m_selectObjects.end();
	while ( i != iEnd )
	{
		(*i)->setObjectState( state );
		i++;
	}
}

// clearSelect
// clear all select
void CDocument::clearSelect()
{
	setStateForSelectObject( CGameObject::Normal );
	m_selectObjects.clear();
}

// clearSelectObject
// remove a object in list select
void CDocument::clearSelectObject( CGameObject *p )
{
	ArrayGameObjectIter i = m_selectObjects.begin(), end = m_selectObjects.end();
	while ( i != end )
	{
		if ( (*i) == p )
		{
			p->setObjectState( CGameObject::Normal );
			m_selectObjects.erase( i );
			return;
		}
		i++;
	}
}

// getData
// get data to serializable
void CDocument::getData( CSerializable *pObj )
{
	pObj->addGroup( "Game level" );

	pObj->addBool(	"gridGame",	m_isGirdDocument );
	pObj->addInt(	"gridSize",	m_gridSize );	

	core::vector3df& camPos		= m_designCamera->getPosition();
	core::vector3df& camTarget	= m_designCamera->getTarget(); 

	pObj->addFloat( "cameraX", camPos.X, true );
	pObj->addFloat( "cameraY", camPos.Y, true );
	pObj->addFloat( "cameraZ", camPos.Z, true );

	pObj->addFloat( "cameraTargetX", camTarget.X, true );
	pObj->addFloat( "cameraTargetY", camTarget.Y, true );
	pObj->addFloat( "cameraTargetZ", camTarget.Z, true );

	pObj->addInt(	"numberObjects",	m_totalObjects );
}

// updateData
// read data from serializable
void CDocument::updateData( CSerializable *pObj )
{
	pObj->nextRecord();

	m_isGirdDocument	= pObj->readBool();
	m_gridSize			= pObj->readInt();	

	core::vector3df camPos;
	core::vector3df camTarget;

	camPos.X = pObj->readFloat();
	camPos.Y = pObj->readFloat();
	camPos.Z = pObj->readFloat();

	camTarget.X = pObj->readFloat();
	camTarget.Y = pObj->readFloat();
	camTarget.Z = pObj->readFloat();
	
	m_designCamera->setPosition( camPos );
	m_designCamera->setTarget( camTarget );

	m_totalObjects = pObj->readInt();
}

// setShadowMode
// change shadow mode
void CDocument::setShadowMode( bool b )
{
	/*
	IDoc::setShadowMode( b );

	ISceneManager *smgr = getIView()->getSceneMgr();
	
	
	ArrayGameObject	shadowRevcList;
	core::aabbox3df globalBox;
	bool addBox = false;

	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{
		CZone *p = (CZone*) (*iZone);		
		
		ArrayGameObjectIter it = p->getChilds()->begin(), end = p->getChilds()->end();
		while ( it != end )
		{
			CGameObject *pObj = (*it);
			ISceneNode *pNode = pObj->getSceneNode();

			if ( pNode )
			{
				CShadowComponent *pShadow =	(CShadowComponent*) pObj->getComponent( IObjectComponent::Shadow );
				
				if ( pShadow && pShadow->isShadowReceiving() )
				{
					shadowRevcList.push_back( pObj );
				}

				core::aabbox3df box = pNode->getBoundingBox();
				pNode->getAbsoluteTransformation().transformBox( box );
				
				if ( addBox == false )
				{
					globalBox = box;
					addBox = true;
				}
				else
				{
					globalBox.addInternalBox( box );
				}
				
			}
			it++;
		}
		iZone++;
	}

	if ( m_shadowMode == true && addBox == true )
	{
		float width		= globalBox.MaxEdge.X - globalBox.MinEdge.X;
		float height	= globalBox.MaxEdge.Z - globalBox.MinEdge.Z;

		core::matrix4 mat;
		mat.buildProjectionMatrixOrthoLH(width, height, 0, 5000);
		
		core::vector3df	pos = globalBox.getCenter() + core::vector3df(0, 3000, 0);

		m_camOrtho->setProjectionMatrix(mat, true);
		m_camOrtho->setPosition( pos );
		m_camOrtho->setTarget( pos - core::vector3df(0, 10, 0) );

		smgr->setActiveCamera(m_camOrtho);
	}
	else
	{
		
		smgr->setActiveCamera(m_cam);
	}
	*/
}