#include "stdafx.h"
#include "CLeftSplitWindow.h"

CLeftSplitWindow::CLeftSplitWindow( WCHAR* lpString, uiWindow *p )
	:CBaseWindow( lpString, p )
{
	m_splitWnd = ref<uiSplitContainer>( new uiSplitContainer(L"leftSplitWnd", 0,0,200,200, this, 2,1 ) );	
	m_splitWnd->showWindow( true );	
	m_splitWnd->setDock( this, UIDOCK_FILL );
	m_splitWnd->changeWindowStyle( UISTYLE_CHILD );

	m_pTreeWnd		= ref<CMapTreeWindow>( new CMapTreeWindow( L"mapTreeWnd", m_splitWnd ) );
	m_pPropertyWnd	= ref<CMapPropertyWindow>( new CMapPropertyWindow( L"mapPropertyWnd", m_splitWnd ) );

	m_splitWnd->setWindow( m_pTreeWnd, 0,0 );
	m_splitWnd->setWindow( m_pPropertyWnd, 1,0 );
	m_splitWnd->setRowSize( 0, 250 );
	m_splitWnd->setExpanderSize( 5 );
	m_splitWnd->updateWindow();	 
}

CLeftSplitWindow::~CLeftSplitWindow()
{

}