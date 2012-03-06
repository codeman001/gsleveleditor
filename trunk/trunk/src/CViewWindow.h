#ifndef _VIEWWIN_
#define _VIEWWIN_

#include "IView.h"
#include "CBaseWindow.h"

class CViewWindow: public CBaseWindow, IView
{
public:
	CViewWindow( WCHAR* lpString, uiWindow *p );
	virtual ~CViewWindow();

	virtual void _OnSize(	uiSizeEvent sizeEvent, int nWidth, int nHeight);
	virtual void _OnPaint( uiGraphics *pG );

	virtual void _OnMouseMove	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnMouseWheel	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnLButtonUp	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnLButtonDown	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnMButtonUp	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnMButtonDown	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnRButtonUp	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnRButtonDown	( uiMouseEvent mouseEvent, int x, int y );	
	
public:
	void createIrrDevice();
public:
	virtual void idleUpdateWindow();

	virtual void alertError( wchar_t *lpString );
	virtual bool yesNoQuestion( wchar_t *lpString );
	virtual void setStatusText( int part, wchar_t *lpString );

};

#endif