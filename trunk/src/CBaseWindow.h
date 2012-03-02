#ifndef _BASEWIN_
#define _BASEWIN_

#include "stdafx.h"

class CBaseWindow: public uiWindow
{
protected:
	HTHEME m_theme;
public:
	CBaseWindow( WCHAR* lpString, uiWindow *pParent );
	virtual ~CBaseWindow();
	
	virtual void _OnCreate();

	void drawTitleBackground(uiGraphics *pG,int x, int y, int w, int h, wchar_t* lpText);
};

#endif