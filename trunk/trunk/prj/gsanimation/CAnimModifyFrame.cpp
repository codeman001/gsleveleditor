#include "stdafx.h"
#include "CAnimModifyFrame.h"
#include "Resource.h"

CAnimModifyFrame::CAnimModifyFrame( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent )
	:uiWindow( lpTitle, x,y,w,h, pParent )
{

	// add rebar
	uiRebar *pRebar = ref<uiRebar>( new uiRebar(L"RebarTool", 0,0, 300,30, this) );	

	// toolbar play
	uiToolbar *pToolbarPlay = ref<uiToolbar>( new uiToolbar(L"Animation", 0,0,0,0, 32,32, pRebar ) );

	uiBitmap iconPlay( MAKEINTRESOURCE(IDB_TOOLBARPLAY), true );
	uiBitmap iconPause( MAKEINTRESOURCE(IDB_TOOLBARPAUSE), true );
	uiBitmap iconStop( MAKEINTRESOURCE(IDB_TOOLBARSTOP), true );

	DWORD iconPlayIndex = pToolbarPlay->pushImage( &iconPlay );
	DWORD iconPauseIndex = pToolbarPlay->pushImage( &iconPause );
	DWORD iconStopIndex = pToolbarPlay->pushImage( &iconStop );	
	
	uiToolbarButton* playButton = pToolbarPlay->addButton(L"Play animation", iconPlayIndex);
	uiToolbarButton* pauseButton = pToolbarPlay->addButton(L"Pause animation", iconPauseIndex);
	uiToolbarButton* stopButton = pToolbarPlay->addButton(L"Stop animation", iconStopIndex);

	uiRebarBand bandToolbarPlay( pToolbarPlay,L"");
	bandToolbarPlay.setWidth( 140 );
	bandToolbarPlay.setMinWidthHeight( 0, 39 );

	
	pRebar->addBand( &bandToolbarPlay );
	pRebar->setDock( this, UIDOCK_TOP );	
	pRebar->showBandBorder( true );
	pRebar->showWindow(true);

	m_mainSplit = ref<uiSplitContainer>( new uiSplitContainer(L"split", 0,0, w, h, this, 1,2) );
	
	// create tree view node
	m_treeNode = ref<uiTreeView>( new uiTreeView(L"treeNode", 0,0, 10,10, true, m_mainSplit) );

	uiIcon iconTreeNode( MAKEINTRESOURCE(IDI_TREENODE) );
	uiIcon iconTreeMesh( MAKEINTRESOURCE(IDI_TREEMESH) );
	m_treeNode->pushSmallImage( &iconTreeNode );
	m_treeNode->pushSmallImage( &iconTreeMesh );

	// create info node
	uiWindow *containerWin = ref<uiWindow>( new uiWindow(L"container", 0,0, 100, 100, m_mainSplit) );

	m_mainSplit->setWindow( m_treeNode, 0, 0 );
	m_mainSplit->setWindow( containerWin, 0, 1 );
	m_mainSplit->setColSize( 0, 300 );
	m_mainSplit->setColSize( 1, w-200 );
	m_mainSplit->setExpanderSize(5);
	
	m_mainSplit->changeWindowStyle( UISTYLE_CHILD );
	m_mainSplit->setDock( this, UIDOCK_FILL );
	m_mainSplit->updateSplit();
	m_mainSplit->showWindow( true );
	
}

CAnimModifyFrame::~CAnimModifyFrame()
{

}

// messageMap
// Phan tich su kien cho uiForm
LRESULT	CAnimModifyFrame::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ( uMsg == WM_CLOSE )
	{
		getParent()->showWindow( true );
		showWindow(false);
		return 1;
	}
	return uiWindow::messageMap( hWnd, uMsg, wParam, lParam );
}

void CAnimModifyFrame::setColladaComponent( CColladaMeshComponent *comp )
{
	// delete all 
	if ( m_colladaComponent != comp )
	{
		m_treeNode->deleteAllItem();
		uiTreeViewItem *root = m_treeNode->addItem(L"colladaTreeNode");

		ISceneNode* sceneNode = comp->getColladaNode();
		if ( sceneNode == NULL )
			return;

		m_colladaComponent = comp;

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

	if ( ((CGameColladaSceneNode*)node)->getMesh() )
	{
		treeItem->setIconIndex(1);
		treeItem->setIconStateIndex(1);
	}
	else
	{
		treeItem->setIconIndex(0);
		treeItem->setIconStateIndex(0);
	}

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