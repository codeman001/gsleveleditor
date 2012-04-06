#ifndef _CMAINFRAME_
#define _CMAINFRAME_

#include "stdafx.h"
#include "CIrrWindow.h"

class CMainFrame: public uiForm
{
protected:
	CIrrWindow		*m_irrWin;
	
	uiTreeView		*m_effectTreeWin;
	uiListProperty	*m_effectPropertyWin;

	uiToolbarButton*	m_addButton;
	uiToolbarButton*	m_delButton;
public:
	CMainFrame();
	virtual ~CMainFrame();

public:

	// create
	// Khoi tao cua so
	virtual int create(LPWSTR lpTitle, int x, int y, int w, int h,uiWindow* pParent, LPWSTR lpName);

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

protected:

	void onMenuNewEffects( uiObject *pSender );
	void onMenuOpenEffects( uiObject *pSender );
	void onMenuSaveEffects( uiObject *pSender );
	void onMenuExit( uiObject *pSender );
	
	void onToolbarAdd( uiObject *pSender );
	void onToolbarDel( uiObject *pSender );
};

#endif