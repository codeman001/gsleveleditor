#include "stdafx.h"
#include "CDocument.h"
#include "IView.h"
#include "IController.h"

#include "CComponentFactory.h"
#include "CObjTemplateFactory.h"

#include "CGameOxyzSceneNode.h"
#include "CGameGSCameraAnimators.h"

long CDocument::s_objectID = 1;

CDocument::CDocument()
{	
}

CDocument::~CDocument()
{
	ArrayZoneIter iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{
		delete (*iZone);
		iZone++;
	}
	m_zones.clear();

	// remove all child
	getIView()->getSceneMgr()->getRootSceneNode()->removeAll();
	
	CComponentFactory::freeData();
	CObjTemplateFactory::freeData();
}

// newDocument
// new 3d scene document
void CDocument::newDocument()
{	
	CComponentFactory::initComponentTemplate();
	CObjTemplateFactory::initObjectTempalte();
		
	ISceneManager *smgr = getIView()->getSceneMgr();
	IrrlichtDevice *device = getIView()->getDevice();

	// add camera
	scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
	cam->setTarget(core::vector3df(300,100,300));
	cam->setPosition( core::vector3df(0, 200, 0) );
	cam->setFOV( core::degToRad(60.0f) );
	cam->setFarValue( 8000.0f );
	
	CGameGSCameraAnimators* camAnimator = new CGameGSCameraAnimators( device->getCursorControl() );
	cam->addAnimator( camAnimator );
	camAnimator->drop();

	// add oxyz plane node
	CGameOxyzSceneNode *oxyPlane = new CGameOxyzSceneNode( smgr->getRootSceneNode(), smgr, 1 );
	oxyPlane->drop();
}
	
// saveDocument
// save the document
bool CDocument::saveDocument(wchar_t* lpPath)
{
	return false;
}

// openDocument
// load document
bool CDocument::openDocument(wchar_t* lpPath)
{
	return false;
}

// updateDocument
// update all object in doc by frame...
void CDocument::updateDocument()
{	
	// update time
	IrrlichtDevice *device = getIView()->getDevice();
	device->getTimer()->tick();

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

	// draw 3d scene
	driver->beginScene( true, true, 0, getIView()->getVideoData() );
		
	// draw all scene
	getIView()->getSceneMgr()->drawAll();

	// controller draw imp
	CControllerManager::getInstance()->getCurrentController()->render();
	
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
		CGameObject* p = (*iZone)->searchObject( id );
		
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
	name.format( L"zone%d", (int)CDocument::s_objectID );

	// create name + id
	pZone->setName( name.c() );
	pZone->setID( CDocument::s_objectID++ );

	// create tree item
	uiTreeViewItem *pTreeItem =	m_treeViewRoot->addChild( (LPWSTR) name.c() );
	
	pTreeItem->setIconIndex( m_nTreeZone );
	pTreeItem->setIconStateIndex( m_nTreeZoneOpen );
	pTreeItem->update();

	pTreeItem->setData( pZone );

	pZone->setTreeItem( pTreeItem );

	// add zone to document
	m_zones.push_back( pZone );
	return pZone;
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
		if ( pObj )
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
			
			if ( pNode != NULL )
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
							/*else
							{
								pGameObj->setObjectState( CGameObject::Normal );
								ArrayGameObjectIter i = m_selectObjects.begin(), iEnd = m_selectObjects.end();
								while ( i != iEnd )
								{
									if ( (*i) == pGameObj )
									{
										m_selectObjects.erase( i );
										break;
									}
									i++;
								}
							}*/

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