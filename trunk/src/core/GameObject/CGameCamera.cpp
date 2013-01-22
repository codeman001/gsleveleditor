#include "stdafx.h"
#include "CGameCamera.h"
#include "IView.h"
#include "CGameBoxSceneNode.h"
#include "CZone.h"

#include "CGameGSCameraAnimator.h"

#ifdef GSGAMEPLAY
#include "CGameCameraFollowAnimator.h"
#endif

#ifdef GSEDITOR
#include "CColladaMeshComponent.h"
#include "CGameContainerSceneNode.h"
#endif

CGameCamera::CGameCamera()
{
	m_targetObject = NULL;
	m_objectType = CGameObject::CameraObject;

#ifdef GSEDITOR
	m_cameraMesh = NULL;
#endif

	ISceneManager *smgr = getIView()->getSceneMgr();

	// save old camera
	ICameraSceneNode *oldCam = smgr->getActiveCamera();

	// create new camera
	m_camera = smgr->addCameraSceneNode();		
	m_camera->setFOV( core::degToRad(60.0f) );
	m_camera->setFarValue( 8000.0f );
	m_camera->grab();	
	
	core::vector3df	pos(0,0,0);
	setPosition( pos );

	core::vector3df target(1,0,0);
	setTarget( target );

#ifdef GSEDITOR	
	CColladaMeshComponent *comp = new CColladaMeshComponent( this );
	comp->loadFromFile( getIView()->getPath("data/editor/camera.dae") );
	addComponent( comp );
	setLighting( false );

	m_cameraMesh = m_node;	
	m_cameraMesh->setVisible( true );	

	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( m_cameraMesh );
	m_cameraMesh->setTriangleSelector(selector);
	selector->drop();
#endif

	// set camera node
	m_node = m_camera;

	// restore camera
	smgr->setActiveCamera( oldCam );

#ifndef GSGAMEPLAY
	setEditorCamera();
#else
	setFreeCamera();
#endif
}

CGameCamera::~CGameCamera()
{
#ifdef GSEDITOR
	m_cameraMesh->drop();
#endif
	
    if ( getIView()->getActiveCamera() == this )
        getIView()->setActiveCamera(NULL);
    
	ISceneManager *smgr = getIView()->getSceneMgr();

	// set null camera on scenenode
	if ( smgr->getActiveCamera() == m_node )
		smgr->setActiveCamera( NULL );
}

void CGameCamera::setTarget( core::vector3df& v )
{
	m_targetPos = v;
	m_camera->setTarget( m_targetPos );
}

// updateObject
// update object by frame...
void CGameCamera::updateObject()
{
	CGameObject::updateObject();

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
	{
		m_cameraMesh->setVisible( true );
		m_node = m_cameraMesh;

		core::vector3df rot = m_targetPos - m_position;
		setOrientation( rot );

		m_node = m_camera;
	}
#endif

#ifdef GSGAMEPLAY
	if ( m_camera->getAnimators().size() > 0 )
	{
		m_position = m_camera->getPosition();
		if ( m_targetObject == NULL )
			m_targetPos = m_camera->getTarget();
	}
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

		driver->draw3DLine( m_position, m_position + ray*100, SColor(255, 255, 255, 0) );
	}
}
#endif

// setEditorCamera
// set camera editor
void CGameCamera::setEditorCamera()
{
	m_camera->removeAnimators();
	m_cameraType = CGameCamera::EditorCamera;

#ifndef GSGAMEPLAY
	// add gseditor animator
	CGameGSCameraAnimators* camAnimator = new CGameGSCameraAnimators( getIView()->getDevice()->getCursorControl() );
	m_camera->addAnimator( camAnimator );	
	camAnimator->drop();
#endif
}

// setFollowObjectCamera
// set camera follow a object
void CGameCamera::setFollowObjectCamera( CGameObject* obj, float radius )
{
	m_camera->removeAnimators();
	m_cameraType = CGameCamera::FollowObjectCamera;

#ifdef GSGAMEPLAY
	CGameCameraFollowAnimator *anim = new CGameCameraFollowAnimator( getIView()->getDevice()->getCursorControl(), radius );
	anim->setFollowNode( obj->getSceneNode() );
	m_camera->addAnimator( anim );
	anim->drop();
#endif
}

// setFreeCamera
// set camera free with custom pos & target
void CGameCamera::setFreeCamera()
{
	m_camera->removeAnimators();
	m_cameraType = CGameCamera::FreeCamera;
}