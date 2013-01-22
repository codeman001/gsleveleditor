#include "stdafx.h"
#include "CContainerObject.h"

CContainerObject::CContainerObject()
{
	m_needSortObject = true;
}

CContainerObject::~CContainerObject()
{
	ArrayGameObjectIter it = m_childs.begin(), end = m_childs.end();
	while ( it != end )
	{
		delete (*it);
		it++;
	}
	m_childs.clear();

#ifdef GSGAMEPLAY
	m_objectByName.clear();
#endif
}

// updateObject
// update object by frame...
void CContainerObject::updateObject()
{
	ArrayGameObjectIter it = m_childs.begin(), end = m_childs.end();
	while ( it != end )
	{
		CGameObject *pObject = (CGameObject*) (*it);
		if ( pObject->isEnable() )
			pObject->updateObject();

		it++;
	}

	updateAddRemoveObj();
}


// sortNodeByID
// sort object by id in list child
void CContainerObject::sortObjectByID()
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

CGameObject* CContainerObject::searchObject( long objectID )
{
	if ( m_needSortObject )
		sortObjectByID();

	int first = 0, last = (int)m_childs.size() - 1;
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

// getNumberObjects
// get num object
int CContainerObject::getNumberObjects()
{
	return (int)m_childs.size();
}

#ifdef GSGAMEPLAY
CGameObject* CContainerObject::searchObject( const wchar_t *objectName )
{
	core::map<std::wstring, CGameObject*>::Node *node = m_objectByName.find( std::wstring(objectName) );
	if ( node == NULL )
		return NULL;

	return node->getValue();
}

// registerObjectName
// register object name for search object by name
void CContainerObject::registerObjectName( CGameObject* obj )
{
	m_objectByName[ obj->getName() ] = obj;
}
#endif


// updateAddRemoveObj
// add child, remove child
void CContainerObject::updateAddRemoveObj()
{
	ArrayGameObjectIter it = m_add.begin(), end = m_add.end();
	while ( it != end )
	{
		m_childs.push_back( (*it) );
		it++;
	}

	it = m_remove.begin(), end = m_remove.end();
	while ( it != end )
	{
		CGameObject *pObj = (*it);

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

	#ifdef GSGAMEPLAY
				// remove name first
				m_objectByName.remove( pObj->getDefaultName() );
	#endif

				// delete gameObject
				delete pObj;
				m_childs.erase( iObj );

				break;
			}

			iObj++;
		}

		it++;
	}

	if ( m_add.size() > 0 )
		m_needSortObject = true;

	if ( m_remove.size() > 0 )
		m_needSortObject = true;
	
	m_add.clear();
	m_remove.clear();
}

// addChild
// add a game object to child list
void CContainerObject::addChild( CGameObject *p )
{
	p->setParent( this );	
	m_add.push_back(p);
}

// removeObject
// remove object
void CContainerObject::removeObject( CGameObject *pObj )
{	
	m_remove.push_back(pObj);
}
