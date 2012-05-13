#include "stdafx.h"
#include "CAnimModifyFrame.h"

CAnimModifyFrame::CAnimModifyFrame( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent )
	:uiWindow( lpTitle, x,y,w,h, pParent )
{
	m_mainSplit = ref<uiSplitContainer>( new uiSplitContainer(L"split", 0,0, w, h, this, 1,2) );
	
	// create tree view node
	m_treeNode = ref<uiTreeView>( new uiTreeView(L"treeNode", 0,0, 10,10, true, m_mainSplit) );

	// create info node
	uiWindow *containerWin = ref<uiWindow>( new uiWindow(L"container", 0,0, 100, 100, m_mainSplit) );

	m_mainSplit->setWindow( m_treeNode, 0, 0 );
	m_mainSplit->setWindow( containerWin, 0, 1 );
	m_mainSplit->setColSize( 0, 300 );
	m_mainSplit->setColSize( 1, w-300 );
	m_mainSplit->setExpanderSize(5);
	
	m_mainSplit->changeWindowStyle( UISTYLE_CHILD );
	m_mainSplit->setDock( this, UIDOCK_FILL );
	m_mainSplit->updateSplit();
	m_mainSplit->showWindow( true );
	
}

CAnimModifyFrame::~CAnimModifyFrame()
{

}

void CAnimModifyFrame::setColladaComponent( CColladaMeshComponent *comp )
{
	// delete all 
	if ( m_colladaComponent != comp )
	{
		m_treeNode->deleteAllItem();
		m_colladaComponent = comp;

		uiTreeViewItem *root = m_treeNode->addItem(L"colladaTreeNode");

		ISceneNode* sceneNode = comp->getColladaNode();
		if ( sceneNode == NULL )
			return;

		core::list<ISceneNode*>::ConstIterator it = sceneNode->getChildren().begin(),
			end = sceneNode->getChildren().end();
		while ( it != end )
		{
			addNodeToTreeView( root, (*it) );
			it++;
		}

		root->expandChild(true);
		root->update();

	}
}

void CAnimModifyFrame::addNodeToTreeView( uiTreeViewItem *parent, ISceneNode* node )
{
	const char* name = node->getName();
	wchar_t nodeName[1024] = {0};

	if ( uiString::length<const char>(name) == 0 )
		uiString::copy<wchar_t, const wchar_t>(nodeName, L"Unknown Name");
	else
		uiString::copy<wchar_t, const char>(nodeName, name);
	
	uiTreeViewItem *treeItem = parent->addChild(nodeName);
	treeItem->setData( (uiObject*) node );

	core::list<ISceneNode*>::ConstIterator it = node->getChildren().begin(),
			end = node->getChildren().end();
	while ( it != end )
	{
		addNodeToTreeView( treeItem, (*it) );
		it++;
	}
	
	treeItem->expandChild(true);
	treeItem->update();
}