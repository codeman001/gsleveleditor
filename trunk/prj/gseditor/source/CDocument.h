#ifndef _CDOCUMENT_H_
#define _CDOCUMENT_H_

#include "stdafx.h"
#include "IDoc.h"

#include "CGameObject.h"
#include "CGameCamera.h"
#include "CZone.h"

#include "CObjTemplateFactory.h"

class CDocument: public IDoc
{
protected:
	ArrayZone		m_zones;

	ArrayGameObject	m_selectObjects;
	ArrayGameObject	m_backupSelectObj;

	uiTreeViewItem	*m_treeViewRoot;	
	wstring			m_filePath;	
	
	ISceneNode		*m_gridNode;

	CGameCamera		*m_designCamera;

	int				m_totalObjects;
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

	// isEmptyDocument
	// return true if document is nothing
	virtual bool isEmptyDocument();

	// getCurrentFile
	// return current file is open
	virtual wchar_t* getCurrentFile();

	// updateDocument
	// update all object in doc by frame...
	virtual void updateDocument();

	// drawDocument
	// draw documen by frame
	virtual void drawDocument();

	// createZone
	// create a zone
	virtual CGameObject* createZone();

	// removeZone
	// remove zone
	virtual void removeZone(CGameObject* zone);

	// removeAllZone
	// remove all zone on level
	virtual void removeAllZone();

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
	virtual void setStateForSelectObject( CGameObject::EObjectState state );

	// clearSelect
	// clear all select
	virtual void clearSelect();

	// clearSelectObject
	// remove a object in list select
	virtual void clearSelectObject( CGameObject *p );

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
	
	// setTreeViewItem
	// set tree view item
	inline void setTreeViewItem( uiTreeViewItem *p )
	{
		m_treeViewRoot = p;
	}

	// isShowGrid
	// is show grid on map
	bool isShowGrid()
	{
		if ( m_gridNode )
			return m_gridNode->isVisible();
		return false;
	}

	// showGrid
	// show grid on map
	void showGrid( bool b )
	{
		if ( m_gridNode )
			m_gridNode->setVisible( b );
	}

	// getData
	// get data to serializable
	virtual void getData( CSerializable *pObj );

	// updateData
	// read data from serializable
	virtual void updateData( CSerializable *pObj );

	// setShadowMode
	// change shadow mode
	virtual void setShadowMode( bool b );

	// getDesignCamera
	virtual CGameCamera* getDesignCamera()
	{
		return m_designCamera;
	}

	// getAllZone
	ArrayZone *getAllZone()
	{
		return &m_zones;
	}

protected:
	// readDocumentFromData
	// parse document from data
	bool readDocumentFromData( char *lpData );

	// initBeginSaveLevel
	// sort & count object when save level
	void initBeginSaveLevel();

};

#endif