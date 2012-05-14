#ifndef _ANIM_MODIFYFRAME_
#define _ANIM_MODIFYFRAME_

#include "CColladaMeshComponent.h"
#include "CTimelineControl.h"

class CAnimModifyFrame: public uiWindow
{
protected:
	CColladaMeshComponent	*m_colladaComponent;
	
	uiSplitContainer		*m_mainSplit;
	uiTreeView				*m_treeNode;

	CTimelineControl		*m_timeControlX;
	CTimelineControl		*m_timeControlY;
	CTimelineControl		*m_timeControlZ;

public:	
	CAnimModifyFrame( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent );
	virtual ~CAnimModifyFrame();
	
	// messageMap
	// Phan tich su kien cho uiForm
	virtual LRESULT	messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);

	void setColladaComponent( CColladaMeshComponent *comp );

	void addNodeToTreeView( uiTreeViewItem *parent, ISceneNode* node );
};

#endif