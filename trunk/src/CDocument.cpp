#include "stdafx.h"
#include "CDocument.h"
#include "IView.h"

CDocument::CDocument()
{

}

CDocument::~CDocument()
{
	vector<CZone*>::iterator iZone = m_zones.begin(), iEnd = m_zones.end();
	while ( iZone != iEnd )
	{
		delete (*iZone);
		iZone++;
	}
	m_zones.clear();
}

// newDocument
// new 3d scene document
void CDocument::newDocument()
{
	// add root zone
	CZone *pRoot = new CZone();
	
	// add a cube box
	//pRoot->createCubeObject( 40.0f );
	CAnimObject *pAnimObj =	pRoot->createAnimObject();	
	pAnimObj->loadFromFile( "media/dwarf.x" );	
	
	// add zone to document
	m_zones.push_back( pRoot );

	// add camera
	scene::ICameraSceneNode* cam = getIView()->getSceneMgr()->addCameraSceneNode();
	cam->setTarget(core::vector3df(0,0,0));
	cam->setPosition( core::vector3df(200, 200, 200) );
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


	// update camera
	
	// update all zone
	vector<CZone*>::iterator iZone = m_zones.begin(), iEnd = m_zones.end();
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

	driver->beginScene( true, true, 0, getIView()->getVideoData() );
	getIView()->getSceneMgr()->drawAll();
	driver->endScene();
}