#ifndef _LEFTSPLITWND_
#define _LEFTSPLITWND_

#include "CBaseWindow.h"
#include "CDocTreeWin.h"
#include "CObjPropertyWin.h"

class CLeftSplitWindow: public CBaseWindow
{
protected:
	uiSplitContainer	*m_splitWnd;

	CDocTreeWindow		*m_pTreeWnd;
	CObjPropertyWindow	*m_pPropertyWnd; 
public:

	CLeftSplitWindow( WCHAR* lpString, uiWindow *p );
	virtual ~CLeftSplitWindow();

	inline CDocTreeWindow* getTreeWnd()
	{
		return m_pTreeWnd;
	}

	inline CObjPropertyWindow* getPropertyWnd()
	{
		return m_pPropertyWnd;
	}
};

#endif