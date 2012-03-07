#ifndef _IDOC_H_
#define _IDOC_H_

#include "stdafx.h"
#include "CGameObject.h"

class IDoc: public uiObject
{
public:
	static long s_objectID;

public:
	// newDocument
	// new 3d scene document
	virtual void newDocument()= 0;
	
	// saveDocument
	// save the document
	virtual bool saveDocument(wchar_t* lpPath) = 0;

	// openDocument
	// load document
	virtual bool openDocument(wchar_t* lpPath) = 0;

	// updateDocument
	// update all object in doc by frame...
	virtual void updateDocument() = 0;

	// drawDocument
	// draw documen by frame
	virtual void drawDocument() = 0;

	// pushListSelectObj
	// save list select obj
	virtual void pushListSelectObj() = 0;

	// popListSelectObj
	// restore list select obj
	virtual void popListSelectObj() = 0;

	// selectObject
	// detect list objs at mouse xy	
	virtual void selectObject( int mouseX, int mouseY, bool isControlHold ) = 0;

	// selectObject
	// detect list objs at rect
	virtual void selectObject( int x, int y, int w, int h, bool isControlHold ) = 0;

	// setStateForSelectObject
	// set state for select object
	virtual void setStateForSelectObject( CGameObject::ObjectState state ) = 0;

	// clearSelect
	// clear all select
	virtual void clearSelect() = 0;

	// getSelectObject
	// get list of object is selected
	virtual vector<CGameObject*>* getSelectObject() = 0;

	// getFirstObjSelect
	// get list of object is selected
	virtual CGameObject* getFirstObjSelect() = 0;

	// searchObjByID
	// search object with id
	virtual CGameObject* searchObject( long id ) = 0;
};

#endif