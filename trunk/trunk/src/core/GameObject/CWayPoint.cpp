#include "stdafx.h"

#include "CZone.h"
#include "CWayPoint.h"
#include "CGameBoxSceneNode.h"

#include "IView.h"

CWayPoint::CWayPoint()
{
	m_objectType	= CGameObject::WaypointObject;
	m_next			= NULL;
	m_back			= NULL;
	m_timeWait		= 0;

	// init box
	ISceneManager *smgr = getIView()->getSceneMgr();
	m_node = new CGameBoxSceneNode(this, 5, smgr->getRootSceneNode(), smgr, m_objectID);
	m_node->getMaterial(0).Lighting = true;	

#ifdef GSEDITOR
	setVisible ( true );

	// add collision
	ITriangleSelector *selector = smgr->createTriangleSelectorFromBoundingBox( m_node );
	m_node->setTriangleSelector(selector);
	selector->drop();
#else
	setVisible ( false );
#endif
}

CWayPoint::~CWayPoint()
{
	if ( m_back )
		m_back->setNext( NULL );

	if ( m_next )
		m_next->setBack( NULL );
}

// saveData
// save data to serializable ( use for save in game .sav )
void CWayPoint::saveData( CSerializable* pObj )
{
	pObj->addGroup	("Waypoint");
	
	long id = -1;
	if ( m_next )
		id = m_next->getID();
	pObj->addLong( "nextWayPoint", id );

	id = -1;
	if ( m_back )
		id = m_back->getID();
	pObj->addLong( "prevWayPoint", id );
	pObj->addLong( "waitTime", m_timeWait );

	CGameObject::saveData( pObj );
}

// loadData
// load data to serializable ( use for load in game .sav )
void CWayPoint::loadData( CSerializable* pObj )
{
	pObj->nextRecord();
	
	CZone* pZone = (CZone*) getParent();

	// next waypoint
	long id = pObj->readLong();	
	CGameObject *p = pZone->searchObject( id );

	if ( id != -1 && p )
	{		
		if ( p && p->getObjectType() == CGameObject::WaypointObject )
		{
			((CWayPoint*)p)->setBack( this );
			setNext( (CWayPoint*)p );
		}
	}
	else
	{
		if ( m_next )
			m_next->setBack( NULL );
		setNext(NULL);
	}

	// prev waypoint
	id = pObj->readLong();
	p = pZone->searchObject( id );
	
	if ( id != -1 && p )
	{		
		if ( p && p->getObjectType() == CGameObject::WaypointObject )
		{
			((CWayPoint*)p)->setNext( this );
			setBack( (CWayPoint*)p );
		}
	}
	else
	{
		if ( m_back )
			m_back->setNext( NULL );
		setBack(NULL);
	}

	// wait time
	m_timeWait = pObj->readLong();

	CGameObject::loadData( pObj );
}

// getData
// get basic data to serializable
void CWayPoint::getData( CSerializable* pObj )
{
	pObj->addGroup	("Waypoint");
	
	long id = -1;
		
	if ( m_next )
		id = m_next->getID();
	pObj->addLong( "nextWayPoint", id );

	id = -1;
	if ( m_back )
		id = m_back->getID();
	pObj->addLong( "prevWayPoint", id );

	pObj->addLong( "waitTime", m_timeWait );

	CGameObject::getData( pObj );
}

// updateData
// update data
void CWayPoint::updateData( CSerializable* pObj )
{
	pObj->nextRecord();
	
	CZone* pZone = (CZone*) getParent();

	// next waypoint
	long id = pObj->readLong();	
	CGameObject *p = pZone->searchObject( id );

	if ( id != -1 && p )
	{		
		if ( p && p->getObjectType() == CGameObject::WaypointObject )
		{
			((CWayPoint*)p)->setBack( this );
			setNext( (CWayPoint*)p );
		}
	}
	else
	{
		if ( m_next )
			m_next->setBack( NULL );
		setNext(NULL);
	}

	// prev waypoint
	id = pObj->readLong();
	p = pZone->searchObject( id );

	if ( id != -1 && p )
	{
		if ( p && p->getObjectType() == CGameObject::WaypointObject )
		{
			((CWayPoint*)p)->setNext( this );
			setBack( (CWayPoint*)p );
		}
	}
	else
	{
		if ( m_back )
			m_back->setNext( NULL );
		setBack(NULL);
	}

	// wait time
	m_timeWait = pObj->readLong();

	CGameObject::updateData( pObj );
}

#ifdef GSEDITOR
// drawObject	
void CWayPoint::drawObject()
{
	if ( m_visible && m_next )
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
		driver->draw3DLine( m_position, m_next->getPosition(), SColor(255, 0, 255, 0) );
	}
}
#endif