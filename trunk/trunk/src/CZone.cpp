#include "stdafx.h"
#include "CZone.h"
#include "IDoc.h"

CZone::CZone()
{
	m_needSortObject = true;
	m_objectType = CGameObject::ZoneObject;
}

CZone::~CZone()
{
	vector<CGameObject*>::iterator it = m_childs.begin(), end = m_childs.end();
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
	vector<CGameObject*>::iterator it = m_childs.begin(), end = m_childs.end();
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

// addChild
// add a game object to child list
void CZone::addChild( CGameObject *p )
{
	m_childs.push_back( p );
	m_needSortObject = true;
}

// createCubeObject
// add a cube object on screen
CCubeObject *CZone::createCubeObject( float size )
{
	CCubeObject *pObj = new CCubeObject( size );
	pObj->setID( IDoc::s_objectID++ );

	addChild( pObj );
	return pObj;
}

// createAnimObject
// add a anim object
CAnimObject *CZone::createAnimObject()
{
	CAnimObject *pObj = new CAnimObject();
	pObj->setID( IDoc::s_objectID++ );
	addChild( pObj );
	return pObj;
}

// removeObject
// remove object
void CZone::removeObject( CGameObject *pObj )
{
	vector<CGameObject*>::iterator iObj = m_childs.begin(), end = m_childs.end();
	while ( iObj != end )
	{
		if ( pObj = (*iObj) )
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