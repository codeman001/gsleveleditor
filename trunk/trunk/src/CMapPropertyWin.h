#ifndef _CMAPPROPERTY_WIN_
#define _CMAPPROPERTY_WIN_

#include "CBaseWindow.h"

class CMapPropertyWindow: public CBaseWindow
{
protected:
public:
	CMapPropertyWindow( WCHAR* lpString, uiWindow *p );
	virtual ~CMapPropertyWindow();
	
	virtual void _OnPaint( uiGraphics *pG );
};

#endif