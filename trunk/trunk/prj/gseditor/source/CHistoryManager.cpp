#include "stdafx.h"
#include "CHistoryManager.h"

#include "IView.h"
#include "IDoc.h"

#include "CGameObject.h"
#include "CGameCamera.h"
#include "CWayPoint.h"
#include "CTrigger.h"
#include "CZone.h"

#define strOfObjType(type) ( CGameObject::s_stringObjType[ (int)type ] )

CHistoryManager::CHistoryManager()
{
	m_disable = false;
	m_begin = false;
}

CHistoryManager::~CHistoryManager()
{
}

bool CHistoryManager::doCreate(	SAction* action, bool redo )
{
	IDoc *pDoc = getIView()->getDocument();

	CSerializable *pObject = &action->object1;
	pObject->setCursorRecord(0);
		
	if ( redo == false )
	{
		SSerializableRec *pObjectID	= pObject->getProperty("objectID");

		if ( pObjectID == NULL )
			return false;

		long objID = -1;
		sscanf( pObjectID->data, "%ld", &objID );	

		CGameObject* pObj =	pDoc->searchObject( objID );
		if ( pObj == NULL )
			return false;

		CZone *pZone = (CZone*) pObj->getParent();
		pZone->removeObject( pObj );
	}
	else
	{
		SSerializableRec *pParentID			= pObject->getProperty("parentID");
		SSerializableRec *pObjectTemplate	= pObject->getProperty("objectTemplate");
		SSerializableRec *pObjectType		= pObject->getProperty("objectType");

		if ( pParentID == NULL )
			return false;

		long parentID = -1;
		sscanf( pParentID->data, "%ld", &parentID );

		CGameObject* pParent =	pDoc->searchObject( parentID );
		if ( pParent == NULL )
			return false;

		if ( pParent->getObjectType() != CGameObject::ZoneObject )
			return false;

		CZone *pZone = (CZone*) pParent;
		wchar_t objTemplate[1024];
		uiString::convertUTF8ToUnicode( pObjectTemplate->data, (unsigned short*) objTemplate );

		CGameObject *pObj = NULL;

		if ( strcmp( pObjectType->data, strOfObjType( CGameObject::GameObject ) ) == 0 )
			pObj = pZone->createObject( objTemplate );			
		else if ( strcmp( pObjectType->data, strOfObjType( CGameObject::CameraObject ) ) == 0 )		
			pObj = pZone->createCamera();			
		else if ( strcmp( pObjectType->data, strOfObjType( CGameObject::WaypointObject ) ) == 0 )
			pObj = pZone->createWaypoint();		
		else if ( strcmp( pObjectType->data, strOfObjType( CGameObject::TriggerObject ) ) == 0 )		
			pObj = pZone->createTrigger();			
		
		if ( pObj )
		{
			pObj->updateData( pObject );
			pZone->setSortObject( true );
		}
	}

	return true;	
}

bool CHistoryManager::doDelete(	SAction* action, bool redo )
{	
	return doCreate( action, !redo );
}

bool CHistoryManager::doModify(	SAction* action, bool redo )
{
	IDoc *pDoc = getIView()->getDocument();

	CSerializable *pObject = &action->object1;
	if ( redo )
		pObject = &action->object2;

	pObject->setCursorRecord(0);

	// get object id
	SSerializableRec *pObjectID	= pObject->getProperty("objectID");

	if ( pObjectID == NULL )
		return false;

	long objID = -1;
	sscanf( pObjectID->data, "%ld", &objID );	

	CGameObject* pObj =	pDoc->searchObject( objID );
	if ( pObj == NULL )
		return false;

	pObj->updateData( pObject );

	return true;
}

bool CHistoryManager::undo()
{
	if ( m_undoAction.size() == 0 )
		return false;	

	getIView()->getDocument()->clearSelect();		

	m_disable = true;

	HistoryAction lastAction = m_undoAction.back();
	SAction* action;

	bool isFailed = false;	
	
	int nAction = lastAction.size();
	for ( int iAction = nAction - 1; iAction >= 0; iAction-- )
	{
		action = &lastAction[iAction];

		if ( action->actionType == CHistoryManager::CreateObject )
		{
			if ( doCreate( action ) == false )
			{
				isFailed = true;
				break;
			}
		}
		
		if ( action->actionType == CHistoryManager::DeleteObject )
		{
			if ( doDelete( action ) == false )
			{
				isFailed = true;
				break;
			}
		}
		
		if ( action->actionType == CHistoryManager::ModifyObject )
		{
			if ( doModify( action ) == false )
			{
				isFailed = true;
				break;
			}
		}
	}

	if ( isFailed == true )
	{				
		m_redoAction.clear();
		m_undoAction.clear();
	}
	else
	{
		m_redoAction.push_back( lastAction );
		m_undoAction.pop_back();
	}
	
	getIView()->setObjectProperty( NULL );
	m_disable = false;

	return true;
}

bool CHistoryManager::redo()
{
	if ( m_redoAction.size() == 0 )	
		return false;	

	getIView()->getDocument()->clearSelect();		

	m_disable = true;

	HistoryAction lastAction = m_redoAction.back();
	SAction* action;

	bool isFailed = false;

	HistoryAction::iterator i = lastAction.begin(), end = lastAction.end();
	while ( i != end )
	{
		action = &(*i);

		if ( action->actionType == CHistoryManager::CreateObject )
		{		
			if ( doCreate( action, true ) == false )
			{
				isFailed = true;
				break;
			}
		}
		
		if ( action->actionType == CHistoryManager::DeleteObject )
		{
			if ( doDelete( action, true ) == false )
			{
				isFailed = true;
				break;
			}
		}
		
		if ( action->actionType == CHistoryManager::ModifyObject )
		{			
			if ( doModify( action, true ) == false )
			{
				isFailed = true;
				break;
			}
		}

		++i;
	}

	if ( isFailed == true )
	{
		m_redoAction.clear();
		m_undoAction.clear();
		return false;
	}
	else
	{
		m_undoAction.push_back( lastAction );
		m_redoAction.pop_back();		
	}


	// update box
	getIView()->setObjectProperty( NULL );
	m_disable = false;

	return true;
}

void CHistoryManager::beginHistory()
{
	if ( m_disable || m_begin == true )
		return;

	m_begin = true;

	m_currentAction.clear();
	m_redoAction.clear();
}

void CHistoryManager::endHistory()
{
	if ( m_disable || m_currentAction.size() == 0 || m_begin == false )
		return;

	m_begin = false;

	// only store 20 action
	if ( m_undoAction.size() > 20 )
		m_undoAction.erase( m_undoAction.begin() );

	// add current action to undo buffer
	m_undoAction.push_back( m_currentAction );
	m_redoAction.clear();
}

void CHistoryManager::addHistoryCreateObj( CGameObject* pObj )
{
	if ( m_disable || m_begin == false )
		return;

	SAction action;

	action.actionType = CHistoryManager::CreateObject;
	pObj->getData( &action.object1 );

	// add to current action
	m_currentAction.push_back( action );
}

void CHistoryManager::addHistoryDeleteObj( CGameObject* pObj )
{
	if ( m_disable || m_begin == false )
		return;

	SAction action;

	action.actionType = CHistoryManager::DeleteObject ;
	pObj->getData( &action.object1 );

	// add to current action
	m_currentAction.push_back( action );
}

void CHistoryManager::addHistoryBeginModifyObj(	CGameObject* pObj )
{
	if ( m_disable || m_begin == false )
		return;

	m_modifyAction.actionType = CHistoryManager::ModifyObject;
	m_modifyAction.object1.clear();
	pObj->getData( &m_modifyAction.object1 );
}

void CHistoryManager::addHistoryEndModifyObj(	CGameObject* pObj )
{
	if ( m_disable || m_begin == false )
		return;

	m_modifyAction.actionType = CHistoryManager::ModifyObject;
	m_modifyAction.object2.clear();
	pObj->getData( &m_modifyAction.object2 );

	// add to current action
	m_currentAction.push_back( m_modifyAction );
}

void CHistoryManager::clearAll()
{
	m_currentAction.clear();
	m_redoAction.clear();
}