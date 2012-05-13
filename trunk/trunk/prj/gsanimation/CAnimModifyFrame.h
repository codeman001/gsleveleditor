#ifndef _ANIM_MODIFYFRAME_
#define _ANIM_MODIFYFRAME_

#include "CColladaMeshComponent.h"

class CAnimModifyFrame: public uiWindow
{
protected:
	CColladaMeshComponent	*m_colladaComponent;
	
	uiSplitContainer		*m_mainSplit;
	uiTreeView				*m_treeNode;

public:	
	CAnimModifyFrame( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent );
	virtual ~CAnimModifyFrame();

	void setColladaComponent( CColladaMeshComponent *comp );

	void addNodeToTreeView( uiTreeViewItem *parent, ISceneNode* node );
};

#endif