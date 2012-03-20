#include "stdafx.h"
#include "CZone.h"

#ifdef GSEDITOR
#include "IDoc.h"
#include "IView.h"
#include "CDocument.h"
#endif

#include "CObjTemplateFactory.h"

CZone::CZone()
{
	m_needSortObject = true;
	m_objectType = CGameObject::ZoneObject;
}

CZone::~CZone()
{
	ArrayGameObjectIter it = m_childs.begin(), end = m_childs.end();
	while ( it != end )
	{
		delete (*it);
		it++;
	}
	m_childs.clear();
}

// updateObject
// update object by frame...
void CZone::updateObject()
{
	ArrayGameObjectIter it = m_childs.begin(), end = m_childs.end();
	while ( it != end )
	{
		CGameObject *pObject = (CGameObject*) (*it);

		if ( pObject->isEnable() )
			pObject->updateObject();

		it++;
	}
}

// sortNodeByID
// sort object by id in list child
void CZone::sortObjectByID()
{
	int len = (int)m_childs.size();
	
	CGameObject *obj;
		
	for (int i = 1; i <= len - 1; i++)
	{
		obj = m_childs[i];
		
		int j = i - 1;
		int done = 0;

		do
		{
			if ( m_childs[j]->getID() > obj->getID() )
			{
				m_childs[j + 1] = m_childs[j];

				j = j - 1;
				if ( j < 0 )
					done = 1;
			}
			else
				done = 1;
		}
		while (done == 0);
		
		m_childs[j + 1] = obj;		
	}

	m_needSortObject = false;
}

CGameObject* CZone::searchObject( long objectID )
{
	if ( m_needSortObject )
		sortObjectByID();

	int first = 0, last = m_childs.size() - 1;
	int mid = 0;
	
	while (first <= last) 
	{
		mid = (first + last) / 2;
		
		if ( objectID > m_childs[mid]->getID() )
			first = mid + 1;
		else if ( objectID < m_childs[mid]->getID() )
			last = mid - 1;
		else		
			return m_childs[mid];
   }

	return NULL;
}

// spawnObject
// create a template object
CGameObject* CZone::spawnObject( wchar_t* objTemplate )
{
	CGameObject *p = CObjTemplateFactory::spawnObject( objTemplate );
	if ( p == NULL )
		return NULL;

	wchar_t lpName[1024];
	swprintf( lpName, 1024, L"%s_%d", objTemplate, (int)CGameObject::s_objectID );
	
	p->setID( CGameObject::s_objectID++ );
	p->setParent( this );

	
	
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

// addChild
// add a game object to child list
void CZone::addChild( CGameObject *p )
{
	p->setParent( this );
	m_childs.push_back( p );
	m_needSortObject = true;
}

// removeObject
// remove object
void CZone::removeObject( CGameObject *pObj )
{
	ArrayGameObjectIter iObj = m_childs.begin(), end = m_childs.end();
	while ( iObj != end )
	{
		if ( pObj == (*iObj) )
		{

#ifdef GSEDITOR
			uiTreeViewItem *pTreeItem = pObj->getTreeItem();
			if ( pTreeItem )
				m_treeItem->destroyChild( pTreeItem );
#endif

			// delete gameObject
			pObj->destroyNode();
			delete pObj;

			m_childs.erase( iObj );

			break;
		}

		iObj++;
	}
}

// getData
// get data from zone
void CZone::getData( CSerializable* pObj )
{
	pObj->addGroup	("Game zone");

	pObj->addLong	("objectID",	m_objectID, true);
	pObj->addString	("objectType",	s_stringObjType[ (int)m_objectType ], true);

	char lpText[1024] = {0};
	uiString::convertUnicodeToUTF8( (unsigned short*)m_name.c_str(), lpText );
	pObj->addString	("objectName",	lpText, false);

	pObj->addBool	("enable",		m_enable );
	pObj->addBool	("visible",		m_visible );	

}

// updateData
// update data
void CZone::updateData( CSerializable* pObj )
{
	int pos = pObj->getCursorRecord();
	pObj->nextRecord();
	
	// object id
	m_objectID	= pObj->readLong();

	// object type
	char *type = pObj->readString();
	for ( int i = 0; i < CGameObject::NumObject; i++ )
	{
		if ( strcmp(s_stringObjType[i], type) == 0 )
		{
			m_objectType = (CGameObject::ObjectType)i;
			break;
		}
	}

	// read obj name
	wchar_t lpText[1024] = {0};
	uiString::convertUTF8ToUnicode( pObj->readString(), (unsigned short*)lpText );
	setName( lpText );

	// object enable
	m_enable	= pObj->readBool();

	// object visible
	m_visible	= pObj->readBool();

	// reset position
	pObj->setCursorRecord( pos );
}