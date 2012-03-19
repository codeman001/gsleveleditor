#include "stdafx.h"
#include "CDocTreeWin.h"
#include "ids.h"
#include "CGlobalInstance.h"
#include "IView.h"
#include "Resource.h"

#include "CDocument.h"
#include "CZone.h"

CDocTreeWindow::CDocTreeWindow( WCHAR* lpString, uiWindow *p )
	:CBaseWindow( lpString, p )
{
	m_treeView = ref<uiTreeView>( new uiTreeView(L"MapTree", 0,0, 1, 1, true, this) );
	m_treeView->setMargin(0,0,30,0);
	m_treeView->setDock( this, UIDOCK_FILL );
	m_treeView->changeStyleEx( 0, WS_EX_CLIENTEDGE );	
	m_treeView->showWindow( true );
	m_treeView->setEventOnSelectChange<CDocTreeWindow, &CDocTreeWindow::_onTreeSelect>( this );

	uiIcon iconRoot( MAKEINTRESOURCE(IDI_TREEROOT) );
	uiIcon iconZone( MAKEINTRESOURCE(IDI_TREEZONE) );
	uiIcon iconZoneOpen( MAKEINTRESOURCE(IDI_TREEZONEOPEN) );
	uiIcon iconHidden( MAKEINTRESOURCE(IDI_TREEHIDDEN) );
	uiIcon iconMesh( MAKEINTRESOURCE(IDI_TREEMESH) );
	uiIcon iconTrigger( MAKEINTRESOURCE(IDI_TREETRIGGER) );
	uiIcon iconCollision( MAKEINTRESOURCE(IDI_TREECOLLISION) );

	m_nTreeRootIcon			= m_treeView->pushSmallImage( &iconRoot );
	m_nTreeZone				= m_treeView->pushSmallImage( &iconZone );
	m_nTreeZoneOpen			= m_treeView->pushSmallImage( &iconZoneOpen );
	m_nTreeHidden			= m_treeView->pushSmallImage( &iconHidden );
	m_nTreeMesh				= m_treeView->pushSmallImage( &iconMesh );
	m_nTreeTrigger			= m_treeView->pushSmallImage( &iconTrigger );
	m_nTreeCollision		= m_treeView->pushSmallImage( &iconCollision );

	CDocument *pDoc = (CDocument*) getIView()->getDocument();
	pDoc->setTreeIcon( 
			m_nTreeRootIcon, 
			m_nTreeZone, 
			m_nTreeZoneOpen,
			m_nTreeHidden,
			m_nTreeMesh,
			m_nTreeTrigger,
			m_nTreeCollision
		);
		
	SetWindowTheme(m_treeView->getHandle(), TEXT("explorer"), NULL);
		
	// init
	initTreeView();
}

CDocTreeWindow::~CDocTreeWindow()
{
}

void CDocTreeWindow::initTreeView()
{
	// add root
	const WCHAR *lpRootName = L"level";
	m_rootItem = m_treeView->addItem( (WCHAR*)lpRootName );
	
	CDocument *pDoc = (CDocument*) getIView()->getDocument();
	pDoc->setTreeViewItem( m_rootItem );

	// create a zone
	CZone *pZone = (CZone*)pDoc->createZone();
	getIView()->setCurrentZone( pZone );

	// select root
	m_treeView->selectItem( pZone->getTreeItem() );
	m_rootItem->expandChild( true );
}

void CDocTreeWindow::clearAllTreeView()
{
	m_treeView->deleteAllItem();
}

void CDocTreeWindow::_OnPaint( uiGraphics *pG )
{
	uiWindow::_OnPaint( pG );
	drawTitleBackground(pG, 0,0, this->getWidth(), 30, TEXT("Level"));
}

LRESULT CDocTreeWindow::messageMap( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( uMsg == WM_CONTEXTMENU )
	{
		POINT pt;
		POINTSTOPOINT(pt, lParam);		
		HWND wndSend = (HWND)wParam;

		if ( wndSend == m_treeView->getHandle() )
		{
			POINT point;
			point.x = pt.x;
			point.y = pt.y;
			ScreenToClient( m_treeView->getHandle(), &point);
			
			uiTreeViewItem *pItem = m_treeView->getHisTest( point.x, point.y );
									
			if ( pItem )
			{

			}
		}
	}

	return uiWindow::messageMap( hWnd, uMsg, wParam, lParam );
}

void CDocTreeWindow::_onTreeSelect(uiObject *sender)
{
	IView	*pView = getIView();
	IDoc	*pDoc = pView->getDocument();
	ArrayGameObject *selectList = pDoc->getSelectObject();
	bool setProperty = true;

	uiListTreeViewItem list;
	m_treeView->getItemSelected( &list );
	
	int selectCount = list.size();
	if ( selectCount == 1 )
	{
		uiTreeViewItem *pItem = list[0];		
		
		CGameObject *pObj = (CGameObject*)pItem->getData();
		if ( pObj )
		{
			if ( pObj->getObjectType() == CGameObject::ZoneObject )
			{
				CZone* pZone = (CZone*)pObj;
				pView->setCurrentZone( pZone );			
			}
			else
			{
				// clear old selected obj
				pDoc->setStateForSelectObject( CGameObject::Normal );
				selectList->clear();

				pView->setObjectProperty( pObj );
				pObj->setObjectState( CGameObject::Move );
				selectList->push_back( pObj );			
			}
		}
	}
	else if ( selectCount > 0 )
	{
		// clear old selected obj
		pDoc->setStateForSelectObject( CGameObject::Normal );
		selectList->clear();

		for ( int i = 0; i < selectCount; i++ )
		{
			uiTreeViewItem *pItem = list[i];
			CGameObject *pObj = (CGameObject*)pItem->getData();
		
			if ( pObj )
			{
				if ( pObj->getObjectType() != CGameObject::ZoneObject )
				{
					if ( setProperty == true )
					{
						pView->setObjectProperty( pObj );
						setProperty = false;
					}

					pObj->setObjectState( CGameObject::Move );
					selectList->push_back( pObj );
				}
			}
		}
	}

}