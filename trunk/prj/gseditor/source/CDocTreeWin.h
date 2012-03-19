#ifndef _CMAPTREE_WIN_
#define _CMAPTREE_WIN_

#include "CBaseWindow.h"

class CDocTreeWindow: public CBaseWindow
{
protected:
	uiTreeView		*m_treeView;
	uiTreeViewItem	*m_rootItem;

	DWORD m_nTreeRootIcon;
	DWORD m_nTreeZone;
	DWORD m_nTreeZoneOpen;
	DWORD m_nTreeHidden;
	DWORD m_nTreeMesh;
	DWORD m_nTreeTrigger;
	DWORD m_nTreeCollision;

public:
	CDocTreeWindow( WCHAR* lpString, uiWindow *p );
	virtual ~CDocTreeWindow();

	virtual void _OnPaint( uiGraphics *pG );
	
	inline HWND getHandleTreeView()
	{
		return m_treeView->getHandle();
	}

	virtual LRESULT messageMap( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void initTreeView();

	void clearAllTreeView();

	void _onTreeSelect(uiObject *sender);
};

#endif