#ifndef _CONTAINEROBJ_
#define _CONTAINEROBJ_

#include "CGameObject.h"

class CContainerObject: public CGameObject
{
protected:
	ArrayGameObject						m_childs;
	ArrayGameObject						m_add;
	ArrayGameObject						m_remove;

	bool								m_needSortObject;

#ifdef GSGAMEPLAY
	core::map<std::wstring, CGameObject*>	m_objectByName;
#endif

public:
	CContainerObject();
	virtual ~CContainerObject();

	// updateObject
	// update object by frame...
	virtual void updateObject();

	// updateAddRemoveObj
	// add child, remove child
	void updateAddRemoveObj();

	// sortObjectByID
	// sort object by id in list child
	void sortObjectByID();
	
	// getNumberObjects
	// get num object
	int getNumberObjects();

	// setSortObject
	// tell the zone need sort object
	inline void setSortObject( bool b )
	{
		m_needSortObject = true;
	}

	// searchObject
	// search object by id
	CGameObject* searchObject( long objectID );

#ifdef GSGAMEPLAY
	CGameObject* searchObject( const wchar_t *objectName );

	// registerObjectName
	// register object name for search object by name
	void registerObjectName( CGameObject* obj );
#endif

	// removeObject
	// remove object
	void removeObject( CGameObject *pObj );

	// addChild
	// add a game object to child list
	void addChild( CGameObject *p );

		// getChilds
	// get list child
	inline ArrayGameObject* getChilds()
	{
		return &m_childs;
	}
};

#endif