#ifndef _ANIM_MODIFYFRAME_
#define _ANIM_MODIFYFRAME_

#include "CColladaMeshComponent.h"
#include "CTimelineControl.h"

class CTreeContainer: public uiWindow
{
protected:
	uiTreeView  *m_treeView;
	uiMenuPopup	*m_treePopupMenu;

	uiTreeViewItem			*m_clickTreeItem;
public:
	CTreeContainer( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent );
	virtual ~CTreeContainer();

	uiTreeView* getTreeView()
	{
		return m_treeView;
	}

	// messageMap
	// Phan tich su kien cho uiForm
	virtual LRESULT	messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

	// _onMenuCommand
	// event when menu item click
	void _onMenuCommand( uiObject *pSender );
};

class CAnimModifyFrame: public uiWindow
{
protected:
	CColladaMeshComponent	*m_colladaComponent;
	
	uiSplitContainer		*m_mainSplit;
		
	CTreeContainer			*m_treeContainer;
	uiTreeView				*m_treeNode;	

	uiToolbarButton			*m_playButton;
	uiToolbarButton			*m_stopButton;
	uiToolbarButton			*m_pauseButton;

	CTimelineControl		*m_timeControl;		
public:	
	CAnimModifyFrame( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent );
	virtual ~CAnimModifyFrame();
	
	// messageMap
	// Phan tich su kien cho uiForm
	virtual LRESULT	messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

	void _onTreeSelect( uiObject *pSender );

	void _onToolbarCommand( uiObject *pSender );

	void setColladaComponent( CColladaMeshComponent *comp );

	void addNodeToTreeView( uiTreeViewItem *parent, ISceneNode* node );

	void updateTimeLine( CGameColladaSceneNode *node, int type );

	
};

#endif