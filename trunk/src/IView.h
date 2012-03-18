#ifndef _IVIEW_
#define _IVIEW_

#include "stdafx.h"
#include "IDoc.h"
#include "IController.h"

class	CZone;

class IView
{
protected:
	IrrlichtDevice*			m_device;
	IVideoDriver*			m_driver;
	ISceneManager*			m_smgr;

	SExposedVideoData		m_videoData;

	IDoc*					m_document;

public:	
	IView();
	virtual ~IView();
		
	// alertError
	// show error msgbox
	virtual void alertError( wchar_t *lpString ) = 0;

	// yesNoQuestion
	// show yes, no msgbox
	virtual bool yesNoQuestion( wchar_t *lpString ) = 0;

	// setStatusText
	// set text on status bar
	virtual void setStatusText( int part, wchar_t *lpString ) = 0;


	// idleUpdateWindow
	// update window
	virtual void idleUpdateWindow() = 0;

public:

	// setObjectProperty
	// set object property on listview window
	virtual void setObjectProperty(CGameObject *pObj) = 0;

public:
	
	int	m_mouseX;
	int m_mouseY;

	// getScreenCoordinatesFrom3DPosition
	// convert 3d xyz to 2d xy
	virtual bool getScreenCoordinatesFrom3DPosition(const core::vector3df& pos3d, int *x, int *y);

	// getSelectRay
	// get 3d ray from camera to pointer
	core::line3df getSelectRay();

public:	

	// getDevice
	// get irr device
	inline IrrlichtDevice* getDevice()
	{
		return m_device;
	}

	// getDriver
	// get irr driver
	inline IVideoDriver* getDriver()
	{
		return m_driver;
	}

	// getSceneMgr
	// get irr scene
	inline ISceneManager* getSceneMgr()
	{
		return m_smgr;
	}
	
	// getVideoData
	// get viewData
	inline SExposedVideoData& getVideoData()
	{
		return m_videoData;
	}

	// getDocument
	// get current document
	inline IDoc* getDocument()
	{
		return m_document;
	}

	// enableFreeCamera
	// disable or enable change camera view on editor
	virtual void enableFreeCamera( bool b );

	// getCurrentObjectTemplate
	// get current name
	virtual wchar_t* getCurrentObjectTemplate() = 0;

	// objTemplate
	// set current template
	virtual void setCurrentObjectTemplate( wchar_t *objTemplate ) = 0;

	// getGameObjectOfCurrentTemplate
	// get current obj
	virtual CGameObject* getGameObjectOfCurrentTemplate() = 0;

	// getCurrentZone
	// get current zone
	virtual CZone* getCurrentZone() = 0;
};

extern IView* getIView();

#endif