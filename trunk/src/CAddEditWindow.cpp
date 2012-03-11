#include "stdafx.h"
#include "CAddEditWindow.h"
#include "Resource.h"

CAddEditWindow::CAddEditWindow( WCHAR* lpString, uiWindow *pParent )
	:CBaseWindow(lpString, pParent)
{
	uiRebar *pRebar = ref<uiRebar>( new uiRebar(L"RebarTool", 0,0, 300,30, this) );	
	
	uiToolbar *pToolbar = ref<uiToolbar>( new uiToolbar(L"Command", 0,0,0,0, 24,24, this ) );

	uiIcon iconAdd( MAKEINTRESOURCE(IDI_TOOLBARADD) );
	uiIcon iconMod( MAKEINTRESOURCE(IDI_TOOLBAREDIT) );
	uiIcon iconDel( MAKEINTRESOURCE(IDI_TOOLBARDELETE) );

	DWORD iconAddIndex = pToolbar->pushImage( &iconAdd );
	DWORD iconModIndex = pToolbar->pushImage( &iconMod );
	DWORD iconDelIndex = pToolbar->pushImage( &iconDel );

	pToolbar->setButtonWH(100,30);
	pToolbar->setTextRight(true);

	m_addButton	= pToolbar->addButton(L"Add", iconAddIndex);
	//m_addButton->setEventOnClicked<CMapUserDataWindow, &CMapUserDataWindow::onToolbarCommand>( this );

	m_modifyButton = pToolbar->addButton(L"Add", iconModIndex);
	//m_addButton->setEventOnClicked<CMapUserDataWindow, &CMapUserDataWindow::onToolbarCommand>( this );
	m_modifyButton->enableButton( false );

	m_deleteButton	= pToolbar->addButton(L"Delete", iconDelIndex);
	//m_deleteButton->setEventOnClicked<CMapUserDataWindow, &CMapUserDataWindow::onToolbarCommand>( this );	
	m_deleteButton->enableButton( false );

	uiRebarBand bandToolbar( pToolbar,L"");	
	bandToolbar.setWidth(50);
	// set height for toolbar
	bandToolbar.setMinWidthHeight(0, 30);
	bandToolbar.enableGripper( false );
	bandToolbar.setBreakBand( true );
	
	pRebar->addBand( &bandToolbar );
	pRebar->setDock( this, UIDOCK_TOP );	
	pRebar->setMargin(0,0,30,0);
	pRebar->showBandBorder( false );
	pRebar->showWindow(true);	
}

CAddEditWindow::~CAddEditWindow()
{
}