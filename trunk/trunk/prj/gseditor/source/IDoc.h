#ifndef _IDOC_H_
#define _IDOC_H_

#include "stdafx.h"
#include "CGameObject.h"
#include "CGameCamera.h"

class IDoc: public uiObject
{
protected:
	bool	m_isGirdDocument;
	int		m_gridSize;

	bool	m_shadowMode;
public:

	IDoc()
	{
		m_isGirdDocument	= false;
		m_gridSize			= 20;
		m_shadowMode		= false;
	}

	virtual ~IDoc()
	{
	};

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

	// saveListSelectObj
	// save list select obj
	virtual void saveListSelectObj() = 0;

	// loadListSelectObj
	// restore list select obj
	virtual void loadListSelectObj() = 0;

	// selectObject
	// detect list objs at mouse xy	
	virtual void selectObject( int mouseX, int mouseY, bool isControlHold ) = 0;

	// selectObject
	// detect list objs at rect
	virtual void selectObject( int x, int y, int w, int h, bool isControlHold ) = 0;

	// setStateForSelectObject
	// set state for select object
	virtual void setStateForSelectObject( CGameObject::EObjectState state ) = 0;

	// clearSelect
	// clear all select
	virtual void clearSelect() = 0;

	// clearSelectObject
	// remove a object in list select
	virtual void clearSelectObject( CGameObject *p ) = 0;

	// getSelectObject
	// get list of object is selected
	virtual ArrayGameObject* getSelectObject() = 0;

	// getFirstObjSelect
	// get list of object is selected
	virtual CGameObject* getFirstObjSelect() = 0;

	// searchObjByID
	// search object with id
	virtual CGameObject* searchObject( long id ) = 0;

	// createZone
	// create a zone
	virtual CGameObject* createZone() = 0;

	// removeZone
	// remove zone
	virtual void removeZone(CGameObject* zone) = 0;

	// removeAllZone
	// remove all zone on level
	virtual void removeAllZone() = 0;

	// getCurrentFile
	// return current file is open
	virtual wchar_t* getCurrentFile() = 0;

	// isEmptyDocument
	// return true if document is nothing
	virtual bool isEmptyDocument() = 0;

	// getData
	// get data to serializable
	virtual void getData( CSerializable *pObj ) = 0;

	// updateData
	// read data from serializable
	virtual void updateData( CSerializable *pObj ) = 0;

	// isGridGame
	// return true if the game is grid type
	inline bool isGridGame()
	{
		return m_isGirdDocument;
	}

	// getGridSize
	// return size of grid
	inline int getGridSize()
	{
		return m_isGirdDocument;
	}

	// setGridSize
	// set grid
	void setGridSize(int size)
	{
		m_gridSize = size;
	}

	// setShadowMode
	virtual void setShadowMode( bool b )
	{
		m_shadowMode = b;
	}

	// isShadowMode
	inline bool isShadowMode()
	{
		return m_shadowMode;
	}	

	// getDesignCamera
	// get camera
	virtual CGameCamera* getDesignCamera() = 0;
};

#endif