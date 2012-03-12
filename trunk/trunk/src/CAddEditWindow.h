#ifndef _ADDEDITWIN_
#define _ADDEDITWIN_

#include "stdafx.h"
#include "CBaseWindow.h"

class CComboList: public uiComboBox
{
public:
	CComboList(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);	
	virtual ~CComboList(void);

	// messageMap
	// sub class combolist
	LRESULT messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class CAddEditWindow: public CBaseWindow
{
protected:
	uiToolbarButton	*m_addButton;
	uiToolbarButton	*m_modifyButton;
	uiToolbarButton	*m_deleteButton;
	
	CComboList		*m_comboList;
public:
	CAddEditWindow( WCHAR* lpString, uiWindow *pParent );
	virtual ~CAddEditWindow();	

	// onToolbarCommand
	// event khi nhan toolbar
	void onToolbarCommand( uiObject *pObj );

	virtual void onAddButton();
	virtual void onModifyButton();
	virtual void onDelButton();
};

#endif