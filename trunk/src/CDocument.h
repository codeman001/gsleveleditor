#ifndef _CDOCUMENT_H_
#define _CDOCUMENT_H_

#include "stdafx.h"
#include "IDoc.h"
#include "CGameObject.h"
#include "CZone.h"

class CDocument: public IDoc
{
protected:
	ArrayZone		m_zones;

	ArrayGameObject	m_selectObjects;
	ArrayGameObject	m_backupSelectObj;
public:
	CDocument();

	virtual ~CDocument();

	// newDocument
	// new 3d scene document
	virtual void newDocument();
	
	// saveDocument
	// save the document
	virtual bool saveDocument(wchar_t* lpPath);

	// openDocument
	// load document
	virtual bool openDocument(wchar_t* lpPath);

	// updateDocument
	// update all object in doc by frame...
	virtual void updateDocument();

	// drawDocument
	// draw documen by frame
	virtual void drawDocument();

	// searchObjByID
	// search object with id
	virtual CGameObject* searchObject( long id );

	// saveListSelectObj
	// save list select obj
	void saveListSelectObj();

	// loadListSelectObj
	// restore list select obj
	void loadListSelectObj();

	// selectObject
	// detect list objs at mouse xy	
	virtual void selectObject( int mouseX, int mouseY, bool isControlHold );

	// selectObject
	// detect list objs at rect
	virtual void selectObject( int x, int y, int w, int h, bool isControlHold );

	// setStateForSelectObject
	// set state for select object
	virtual void setStateForSelectObject( CGameObject::ObjectState state );

	// clearSelect
	// clear all select
	virtual void clearSelect();

	// getSelectObject
	// get list of object is selected
	virtual ArrayGameObject* getSelectObject()
	{
		return &m_selectObjects;
	}

	// getFirstObjSelect
	// get list of object is selected
	virtual CGameObject* getFirstObjSelect()
	{
		if ( m_selectObjects.size() == 0 )
			return NULL;
		
		return m_selectObjects.front();
	}
	
};

#endif