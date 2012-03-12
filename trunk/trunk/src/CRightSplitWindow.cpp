#include "stdafx.h"
#include "CRightSplitWindow.h"

CRightSplitWindow::CRightSplitWindow( WCHAR* lpString, uiWindow *p )
	:CBaseWindow( lpString, p )
{
	m_splitWnd = ref<uiSplitContainer>( new uiSplitContainer(L"rightSplitWnd", 0,0,200,200, this, 2,1 ) );	
	m_splitWnd->showWindow( true );	
	m_splitWnd->setDock( this, UIDOCK_FILL );
	m_splitWnd->changeWindowStyle( UISTYLE_CHILD );

	m_tempWnd = ref<CTemplateWindow>	( new CTemplateWindow(L"templateWindow", m_splitWnd) );
	m_compWnd = ref<CComponentWindow>	( new CComponentWindow(L"componentWindow", m_splitWnd) );

	m_splitWnd->setWindow( m_tempWnd, 0,0 );
	m_splitWnd->setWindow( m_compWnd, 1,0 );
	m_splitWnd->setRowSize( 0, 400 );
	m_splitWnd->setExpanderSize( 5 );
	m_splitWnd->updateWindow();	 
}

CRightSplitWindow::~CRightSplitWindow()
{
}