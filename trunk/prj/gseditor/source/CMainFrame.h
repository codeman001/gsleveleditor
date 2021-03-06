#ifndef _CMAINFRAME_
#define _CMAINFRAME_

#include "stdafx.h"
#include "RibbonApplication.h"

#include "CViewWindow.h"
#include "CLeftSplitWindow.h"
#include "CRightSplitWindow.h"

class CMainFrame: public uiForm
{
protected:
	IUIFramework*	m_pFramework;
	IUIApplication* m_pApplication;
	int				m_ribbonUIHeight;
protected:
	uiStatusBar				*m_statusBarWnd;
	uiSplitContainer		*m_mainSplitWnd;
	uiTabControl			*m_tabView;

	CViewWindow				*m_viewWnd;
	CLeftSplitWindow		*m_leftSplitWnd;
	CRightSplitWindow		*m_rightSplitWnd;

public:
	CMainFrame();
	virtual ~CMainFrame();
	
	inline uiStatusBar* getStatusBar()
	{
		return m_statusBarWnd;
	}

	inline CDocTreeWindow *getDocTreeWnd()
	{
		return m_leftSplitWnd->getTreeWnd();
	}

	inline CObjPropertyWindow* getObjPropertyWnd()
	{
		return m_leftSplitWnd->getPropertyWnd();
	}

public:
	virtual void onTabChange( uiObject *pSender );

public:
	virtual void _OnCreate();
	virtual void _OnPaint(uiGraphics * pG);
	virtual void _OnSize(uiSizeEvent sizeEvent, int nWidth, int nHeight);
	
	virtual bool _OnKeyDown( uiKeyEvent keyEvent );
	virtual bool _OnKeyUp( uiKeyEvent keyEvent );

	virtual void _OnMouseWheel	( uiMouseEvent mouseEvent, int x, int y );

	virtual void _OnIdle();

	// registerWindow
	// Dang ky cua so form
	virtual bool registerWindow(LPWSTR lpNameApp, HINSTANCE hInst);	

	// messageMap
	// Phan tich su kien cho uiForm
	virtual LRESULT	messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
	
};

#endif