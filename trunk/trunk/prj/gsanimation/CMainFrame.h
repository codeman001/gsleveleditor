#ifndef _CMAINFRAME_
#define _CMAINFRAME_

#include "stdafx.h"
#include "CIrrWindow.h"

#include "CAnimModifyFrame.h"
#include "CAnimBlendingFrame.h"

class CMainFrame: public uiForm
{
protected:
	CIrrWindow		*m_irrWin;
	
	CAnimModifyFrame	*m_editorWin;
	CAnimBlendingFrame *m_blenddingWin;

	uiListView		*m_listView;

	bool			m_needSetCamera;
	const wchar_t	*m_lpCmdLine;

	uiStringA		m_file1;
	uiStringA		m_file2;

	enum UserAction
	{
		None = 0,
		ExportMesh,
		ExportAnim,
		Open,		
	};

	UserAction	m_action;

public:
	CMainFrame(const wchar_t* lpCmdLine);
	virtual ~CMainFrame();

protected:
	
	string	m_currentFile;
	string	m_currentAnimFile;

public:

	// create
	// Khoi tao cua so
	virtual int create(LPWSTR lpTitle, int x, int y, int w, int h,uiWindow* pParent, LPWSTR lpName);

	virtual void _OnPaint(uiGraphics * pG);
	virtual void _OnSize(uiSizeEvent sizeEvent, int nWidth, int nHeight);
	
	virtual bool _OnKeyDown( uiKeyEvent keyEvent );
	virtual bool _OnKeyUp( uiKeyEvent keyEvent );

	virtual void _OnMouseWheel	( uiMouseEvent mouseEvent, int x, int y );

	virtual void _OnIdle();

	// registerWindow
	// Dang ky cua so form
	virtual bool registerWindow(LPWSTR lpNameApp, HINSTANCE hInst);	

	// messageMap
	// Phan tich su kien cho uiForm
	virtual LRESULT	messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	bool doExportMode();

	CColladaMeshComponent* getMeshComponent()
	{
		return m_irrWin->getAnimComponent();
	}

	void listPropertyOnItemChange( uiObject *pSender );
	void listPropertyOnItemEdit( uiObject *pSender );

	void toolbarSaveScene( uiObject *pSender );
	void toolbarSaveAnim( uiObject *pSender );
	void toolbarLoadMesh( uiObject *pSender );
	void toolbarLoadAnimDae( uiObject *pSender );

	void updateAnimDataToUI();

};

#endif