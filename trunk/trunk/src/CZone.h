#ifndef _CZONE_H_
#define _CZONE_H_

#include "CGameObject.h"

class CZone: public CGameObject
{
protected:
	ArrayGameObject		m_childs;
	
	bool				m_needSortObject;

protected:
	// addChild
	// add a game object to child list
	void addChild( CGameObject *p );

public:
	CZone();

	virtual ~CZone();

	// updateObject
	// update object by frame...
	virtual void updateObject();

	// sortObjectByID
	// sort object by id in list child
	void sortObjectByID();
	
	// searchObject
	// search object by id
	CGameObject* searchObject( long objectID );
public:
	
	// getChilds
	// get list child
	inline ArrayGameObject* getChilds()
	{
		return &m_childs;
	}	

	// createAnimObject
	// add a anim object
	// CAnimObject *createAnimObject();

	// removeObject
	// remove object
	void removeObject( CGameObject *pObj );

};

typedef vector<CZone*>				ArrayZone;
typedef vector<CZone*>::iterator	ArrayZoneIter;

#endif