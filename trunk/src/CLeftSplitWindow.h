#ifndef _LEFTSPLITWND_
#define _LEFTSPLITWND_

#include "CBaseWindow.h"
#include "CMapTreeWin.h"
#include "CMapPropertyWin.h"

class CLeftSplitWindow: public CBaseWindow
{
protected:
	uiSplitContainer	*m_splitWnd;

	CMapTreeWindow		*m_pTreeWnd;
	CMapPropertyWindow	*m_pPropertyWnd; 
public:

	CLeftSplitWindow( WCHAR* lpString, uiWindow *p );
	virtual ~CLeftSplitWindow();

	inline CMapTreeWindow* getTreeWnd()
	{
		return m_pTreeWnd;
	}

	inline CMapPropertyWindow* getPropertyWnd()
	{
		return m_pPropertyWnd;
	}
};

#endif