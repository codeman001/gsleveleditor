#ifndef _CMAINFRAME_
#define _CMAINFRAME_

#include "stdafx.h"
#include "IView.h"

class CMainFrame: public uiForm, public IView
{
protected:
	
protected:

public:
	CMainFrame();
	virtual ~CMainFrame();
	
	inline IView *getIView()
	{
		return (IView*)this;
	}
public:

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