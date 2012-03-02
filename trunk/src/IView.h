#ifndef _IVIEW_
#define _IVIEW_

#include "stdafx.h"

class IView
{
protected:
	bool m_repaintWindow;

	IrrlichtDevice*			m_device;
	IVideoDriver*			m_driver;
	ISceneManager*			m_smgr;

	SExposedVideoData		m_videoData;
public:	
	IView();

	virtual ~IView();

	virtual void beginDraw() = 0;
	virtual void drawDocument() = 0;
	virtual void endDraw() = 0;

	virtual void alertError( wchar_t *lpString ) = 0;
	virtual bool yesNoQuestion( wchar_t *lpString ) = 0;
	virtual void setStatusText( int part, wchar_t *lpString ) = 0;

	virtual void idleUpdateWindow() = 0;

public:	
	inline void requestPaintWindow( bool b = true )
	{
		m_repaintWindow = b;
	}	

	IrrlichtDevice* getDevice()
	{
		return m_device;
	}

	IVideoDriver* getDriver()
	{
		return m_driver;
	}

	ISceneManager* getSceneMgr()
	{
		return m_smgr;
	}

};

extern IView* getIView();

#endif