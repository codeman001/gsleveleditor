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
	m_node = new CGameBoxSceneNode(this, 20, smgr->getRootSceneNode(), smgr, m_objectID);
	m_node->getMaterial(0).Lighting = true;
}

CWayPoint::~CWayPoint()
{

}

// saveData
// save data to serializable ( use for save in game .sav )
void CWayPoint::saveData( CSerializable* pObj )
{
	CGameObject::saveData( pObj );
}

// loadData
// load data to serializable ( use for load in game .sav )
void CWayPoint::loadData( CSerializable* pObj )
{
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
	m_next = NULL;	
	if ( id != -1 )
	{
		CGameObject *p = pZone->searchObject( id );
		if ( p && p->getObjectType() == CGameObject::WaypointObject )
			m_next = (CWayPoint*)p;
	}

	// prev waypoint
	id = pObj->readLong();
	m_next = NULL;
	if ( id != -1 )			
	{
		CGameObject *p = pZone->searchObject( id );
		if ( p && p->getObjectType() == CGameObject::WaypointObject )
			m_back = (CWayPoint*)p;
	}

	// wait time
	m_timeWait = pObj->readLong();

	CGameObject::updateData( pObj );	
}

#ifdef GSEDITOR

// drawObject	
void CWayPoint::drawObject()
{
	
}

#endif