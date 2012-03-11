#ifndef _ADDEDITWIN_
#define _ADDEDITWIN_

#include "stdafx.h"
#include "CBaseWindow.h"

class CAddEditWindow: public CBaseWindow
{
protected:
	uiToolbarButton	*m_addButton;
	uiToolbarButton	*m_modifyButton;
	uiToolbarButton	*m_deleteButton;
public:
	CAddEditWindow( WCHAR* lpString, uiWindow *pParent );
	virtual ~CAddEditWindow();	
};

#endif