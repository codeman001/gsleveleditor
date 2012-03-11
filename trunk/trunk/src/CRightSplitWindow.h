#ifndef _RIGHTSPLITWINDOW_
#define _RIGHTSPLITWINDOW_


#include "CBaseWindow.h"
#include "CComponentWindow.h"
#include "CTemplateWindow.h"

class CRightSplitWindow: public CBaseWindow
{
protected:
	uiSplitContainer	*m_splitWnd;

	CTemplateWindow		*m_tempWnd;
	CComponentWindow	*m_compWnd;	
public:
	CRightSplitWindow( WCHAR* lpString, uiWindow *p );
	virtual ~CRightSplitWindow();
};

#endif