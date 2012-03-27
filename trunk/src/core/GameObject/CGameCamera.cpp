#include "stdafx.h"
#include "CGameCamera.h"
#include "IView.h"
#include "CGameBoxSceneNode.h"
#include "CZone.h"

#ifdef GSEDITOR
#include "CGameGSCameraAnimator.h"
#endif

CGameCamera::CGameCamera()
{
	m_targetObject = NULL;
	m_objectType = CGameObject::CameraObject;

	// add camera
	ISceneManager *smgr = getIView()->getSceneMgr();

	// save old camera
	ICameraSceneNode *oldCam = smgr->getActiveCamera();

	// create new camera
	m_camera = smgr->addCameraSceneNode();		
	m_camera->setFOV( core::degToRad(60.0f) );
	m_camera->setFarValue( 8000.0f );
	m_camera->grab();	

	// store node
	m_node = m_camera;

	setPosition( core::vector3df(0,0,0) );
	setTarget( core::vector3df(1,0,0) );	

#ifdef GSEDITOR
	m_cameraMesh = new CGameBoxSceneNode(this, 5, m_camera, smgr, -1);
	m_cameraMesh->setVisible( true );	

	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( m_cameraMesh );
	m_cameraMesh->setTriangleSelector(selector);
	selector->drop();


	IrrlichtDevice *device = getIView()->getDevice();
	CGameGSCameraAnimators* camAnimator = new CGameGSCameraAnimators( device->getCursorControl() );
	m_camera->addAnimator( camAnimator );	
	camAnimator->drop();
#endif
	
	// restore camera
	smgr->setActiveCamera( oldCam );
}

CGameCamera::~CGameCamera()
{
#ifdef GSEDITOR
	m_cameraMesh->drop();
#endif
}

// updateObject
// update object by frame...
void CGameCamera::updateObject()
{
	if ( m_enable && m_targetObject )
	{
		// look target
		setTarget( m_targetObject->getPosition() );
	}
#ifdef GSEDITOR
	else
	{
		if ( getIView()->getActiveCamera() == this )
			m_targetPos = m_camera->getTarget();
	}

	// maybe modify by free animator
	core::vector3df pos = m_camera->getPosition();
	setPosition( pos );
	
	if ( getIView()->getActiveCamera() == this )
		m_cameraMesh->setVisible( false );
	else
		m_cameraMesh->setVisible( true );

#endif
}

// saveData
// save data to serializable ( use for save in game .sav )
void CGameCamera::saveData( CSerializable* pObj )
{
	pObj->addGroup	( s_stringObjType[m_objectType] );

	pObj->addFloat( "targetX", m_targetPos.X, true );
	pObj->addFloat( "targetY", m_targetPos.Y, true );
	pObj->addFloat( "targetZ", m_targetPos.Z, true );
	
	long id = -1;
	if ( m_targetObject )
		id = m_targetObject->getID();
	pObj->addLong( "targetID", id );

	CGameObject::saveData( pObj );
}

// loadData
// load data to serializable ( use for load in game .sav )
void CGameCamera::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	m_targetPos.X = pObj->readFloat();
	m_targetPos.Y = pObj->readFloat();
	m_targetPos.Z = pObj->readFloat();
	setTarget( m_targetPos );

	CZone* pZone = (CZone*) getParent();

	// next waypoint
	m_targetObject = NULL;

	long id = pObj->readLong();
	if ( id != -1 )
	{
		CGameObject *p = pZone->searchObject( id );
		if ( p != this )
			m_targetObject = p;
	}

	CGameObject::loadData( pObj );
}

// getData
// get basic data to serializable
void CGameCamera::getData( CSerializable* pObj )
{
	pObj->addGroup	( s_stringObjType[m_objectType] );

	pObj->addFloat( "targetX", m_targetPos.X, true );
	pObj->addFloat( "targetY", m_targetPos.Y, true );
	pObj->addFloat( "targetZ", m_targetPos.Z, true );

	long id = -1;
	if ( m_targetObject )
		id = m_targetObject->getID();
	pObj->addLong( "targetID", id );

	
	CGameObject::getData( pObj );
}

// updateData
// update data
void CGameCamera::updateData( CSerializable* pObj )
{
	pObj->nextRecord();

	m_targetPos.X = pObj->readFloat();
	m_targetPos.Y = pObj->readFloat();
	m_targetPos.Z = pObj->readFloat();
	setTarget( m_targetPos );

	CZone* pZone = (CZone*) getParent();

	// next waypoint
	m_targetObject = NULL;

	long id = pObj->readLong();
	if ( id != -1 )
	{
		CGameObject *p = pZone->searchObject( id );
		if ( p != this )
			m_targetObject = p;
	}

	CGameObject::updateData( pObj );
}

#ifdef GSEDITOR
// drawObject
void CGameCamera::drawObject()
{
	if ( m_visible && m_objectID != -1  && getIView()->getActiveCamera() != this )
	{
		IVideoDriver *driver = getIView()->getDriver();

		// set material
		SMaterial debug_mat;	
		debug_mat.Lighting = false;
		debug_mat.AntiAliasing=0;
		driver->setMaterial(debug_mat);

		// reset transform
		core::matrix4 mat;
		mat.makeIdentity();
		driver->setTransform(video::ETS_WORLD, mat);

		// draw up
		core::vector3df ray = (m_targetPos - m_position).normalize();

		driver->draw3DLine( m_position, m_position + ray*80, SColor(255, 255, 255, 0) );
	}
}
#endif