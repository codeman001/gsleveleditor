#include "stdafx.h"
#include "CZone.h"

#ifdef GSEDITOR
#include "IDoc.h"
#include "IView.h"
#endif

#include "CDocument.h"
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

	p->setID( CDocument::s_objectID++ );
	addChild( p );

#ifdef GSEDITOR
	// set up name
	uiStringW name;
	name.format( L"%s_%d", objTemplate, (int)CDocument::s_objectID );

	// create tree item
	uiTreeViewItem *pTreeItem =	m_treeItem->addChild( (LPWSTR) name.c() );
	CDocument *pDoc = (CDocument*) getIView()->getDocument();

	pTreeItem->setIconIndex( pDoc->m_nTreeMesh );
	pTreeItem->setIconStateIndex( pDoc->m_nTreeMesh );
	pTreeItem->update();

	pTreeItem->setData( p );
	p->setTreeItem( pTreeItem );
	m_treeItem->update();
	m_treeItem->expandChild( true );
#endif

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
			// delete gameObject
			(*iObj)->destroyNode();
			delete (*iObj);
			m_childs.erase( iObj );

			break;
		}

		iObj++;
	}
}