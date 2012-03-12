#ifndef _COMPONENTWINDOW_
#define _COMPONENTWINDOW_

#include "CAddEditWindow.h"

class CComponentWindow: public CAddEditWindow
{
public:
	CComponentWindow( WCHAR *lpString, uiWindow *pParent );	
	virtual ~CComponentWindow();

	virtual void _OnPaint( uiGraphics *pG );

	virtual void onAddButton();
	virtual void onModifyButton();
	virtual void onDelButton();
};

#endif