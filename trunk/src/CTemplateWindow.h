#ifndef _TEMPLATEWINDOW_
#define _TEMPLATEWINDOW_

#include "CAddEditWindow.h"




class CTemplateWindow: public CAddEditWindow
{
public:
	CTemplateWindow( WCHAR *lpString, uiWindow *p );
	virtual ~CTemplateWindow();
	
	virtual void _OnPaint( uiGraphics *pG );
};

#endif