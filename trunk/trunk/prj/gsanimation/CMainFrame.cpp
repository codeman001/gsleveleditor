#include "stdafx.h"
#include "CMainFrame.h"
#include "Resource.h"

CMainFrame::CMainFrame()
{
	m_currentFile = "";
}

CMainFrame::~CMainFrame()
{
}
	
// create
// Khoi tao cua so
int CMainFrame::create(LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, LPWSTR lpName)
{
	int ret = uiForm::create( lpTitle, x, y, w, h, pParent, lpName );	

	// add rebar
	uiRebar *pRebar = ref<uiRebar>( new uiRebar(L"RebarTool", 0,0, 300,30, this) );	

	// toolbar play
	uiToolbar *pToolbar = ref<uiToolbar>( new uiToolbar(L"Tools", 0,0,0,0, 32,32, pRebar ) );
	
	uiBitmap iconOpen( MAKEINTRESOURCE(IDB_TOOLBAROPEN), true );	
	uiBitmap iconSave( MAKEINTRESOURCE(IDB_TOOLBARSAVE), true );	
	uiBitmap iconMesh( MAKEINTRESOURCE(IDB_TOOLBARMESH), true );
	uiBitmap iconAnim( MAKEINTRESOURCE(IDB_TOOLBARANIM), true );

	pToolbar->pushImage( &iconOpen );
	pToolbar->pushImage( &iconSave );
	pToolbar->pushImage( &iconMesh );
	pToolbar->pushImage( &iconAnim );

	pToolbar->setButtonWidth(120);	
	pToolbar->setTextRight(true);

	uiToolbarButton *toolbarButton = pToolbar->addButton(L"Open animation", 0);
	toolbarButton->setEventOnClicked<CMainFrame, &CMainFrame::toolbarOpenAnim>( this );

	toolbarButton = pToolbar->addButton(L"Save animation", 1);	
	toolbarButton->setEventOnClicked<CMainFrame, &CMainFrame::toolbarSaveAnim>( this );

	toolbarButton = pToolbar->addButton(L"Load mesh", 2);
	toolbarButton->setEventOnClicked<CMainFrame, &CMainFrame::toolbarLoadMesh>( this );

	toolbarButton = pToolbar->addButton(L"Load animation", 3);
	toolbarButton->setEventOnClicked<CMainFrame, &CMainFrame::toolbarLoadAnimDae>( this );

	uiRebarBand bandToolbar( pToolbar,L"");
	bandToolbar.setBreakBand( true );	
	bandToolbar.enableGripper(false);
	bandToolbar.setMinWidthHeight( 0, 39 );

	// add band to rebar	
	pRebar->addBand( &bandToolbar );
	pRebar->setDock( this, UIDOCK_TOP );	
	pRebar->showBandBorder( false );
	pRebar->showWindow(true);

	uiSplitContainer *split = ref<uiSplitContainer>( new uiSplitContainer(L"mainSplit", 0, 0, 1000, 600, this, 2, 1) );
	
	m_irrWin = ref<CIrrWindow>( new CIrrWindow(L"irrWindow", split) );
	m_propertyWin = ref<uiListProperty>( new uiListProperty(L"propertyWindow", 0,0, 1000, 30, split,7) );
	
	uiListPropertyGroup *pHeader = m_propertyWin->addGroup(L"Animation header");
	pHeader->enableColText(true);
	pHeader->setColText(L"State name", 0);
	pHeader->setColText(L"Next name", 1);
	pHeader->setColText(L"Anim name", 2);
	pHeader->setColText(L"Time begin", 3);
	pHeader->setColText(L"Duration", 4);
	pHeader->setColText(L"Loop", 5);
	pHeader->setColText(L"MovePosition", 6);

	m_propertyWin->showWindow( true );	
	m_propertyWin->setEventOnItemChange<CMainFrame, &CMainFrame::listPropertyOnItenChange>( this );

	split->setWindow( m_irrWin, 0, 0 );
	split->setWindow( m_propertyWin, 1, 0 );
	split->setRowSize( 0, 400 );
	split->setRowSize( 1, 300 );
	
	split->changeWindowStyle( UISTYLE_CHILD );
	split->setDock( this, UIDOCK_FILL );
	split->updateSplit();
	split->showWindow( true );

	return ret;
}

void CMainFrame::_OnPaint(uiGraphics * pG)
{
	uiForm::_OnPaint( pG );
}

void CMainFrame::_OnSize(uiSizeEvent sizeEvent, int nWidth, int nHeight)
{
	uiForm::_OnSize( sizeEvent, nWidth, nHeight );
}
	
bool CMainFrame::_OnKeyDown( uiKeyEvent keyEvent )
{
	return true;
}

bool CMainFrame::_OnKeyUp( uiKeyEvent keyEvent )
{
	CAnimMeshComponent* animComponent = m_irrWin->getAnimComponent();
	if ( animComponent == NULL )
		return true;	

	CGameAnimatedMeshSceneNode *node = animComponent->getAnimNode();
	if ( node == NULL )
		return true;

	return true;
}

void CMainFrame::_OnMouseWheel	( uiMouseEvent mouseEvent, int x, int y )
{
}

void CMainFrame::_OnIdle()
{
	m_irrWin->irrUpdate();
}

// registerWindow
// Dang ky cua so form
bool CMainFrame::registerWindow(LPWSTR lpNameApp, HINSTANCE hInst)
{
	WNDCLASSEXW wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = uiWindow::windowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;	
	wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject (LTGRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = lpNameApp;
	wndclass.hIconSm = LoadIcon (NULL, MAKEINTRESOURCE(IDI_GSANIMATION) );
	wndclass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_GSANIMATION));

	if (! RegisterClassExW(&wndclass))
		return false;

	return true;
}

// messageMap
// Phan tich su kien cho uiForm
LRESULT	CMainFrame::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	return uiForm::messageMap(hWnd, uMsg, wParam, lParam);
}

void CMainFrame::toolbarOpenAnim( uiObject *pSender )
{

}

void CMainFrame::toolbarSaveAnim( uiObject *pSender )
{
	WCHAR lpPath[ MAX_PATH ] = {0};	
	char lpFileName[ MAX_PATH ] = {0};

	if ( m_currentFile.size() == 0 )
	{
		uiSaveOpenDialog dialog;	
		dialog.clearAllFileExt();
		dialog.addFileExt( L"Animation (.anim)", L"*.anim");
		dialog.addFileExt( L"All files (.*)", L"*.*" );
		if ( dialog.doModal( uiApplication::getRoot(), true ) == false )
			return;

		dialog.getFileName( lpPath );
		uiString::copy<char, WCHAR>( lpFileName, lpPath );
		
		//pParticleComponent->saveXML( lpFileName );

		m_currentFile = lpFileName;
	}
	else
	{
		//pParticleComponent->saveXML( m_currentFile.c_str() );		
	}

	uiString::copy<WCHAR, const char>( lpPath, m_currentFile.c_str() );

	WCHAR title[1024];
	swprintf(title, 1024, L"%s - %s", TEXT( STR_APP_TITLE ), lpPath);
	setCaption( title );
}

void CMainFrame::toolbarLoadMesh( uiObject *pSender )
{
	WCHAR lpPath[ MAX_PATH ] = {0};	
	char lpFileName[ MAX_PATH ] = {0};
	
	uiSaveOpenDialog dialog;	
	dialog.clearAllFileExt();
	dialog.addFileExt( L"Mesh file (.x, *.dae)", L"*.x;*.dae" );
	dialog.addFileExt( L"All files (.*)", L"*.*" );
	if ( dialog.doModal( uiApplication::getRoot(), false ) == false )
		return;
		
	dialog.getFileName( lpPath );
	uiString::copy<char, WCHAR>( lpFileName, lpPath );

	// load anim from file
	CAnimMeshComponent* animComponent = m_irrWin->getAnimComponent();
	animComponent->loadFromFile( lpFileName );

}

void CMainFrame::toolbarLoadAnimDae( uiObject *pSender )
{
	WCHAR lpPath[ MAX_PATH ] = {0};	
	char lpFileName[ MAX_PATH ] = {0};
	
	uiSaveOpenDialog dialog;	
	dialog.clearAllFileExt();
	dialog.addFileExt( L"dae animation file (.*.dae)", L"*.dae" );
	dialog.addFileExt( L"All files (.*)", L"*.*" );
	if ( dialog.doModal( uiApplication::getRoot(), false ) == false )
		return;
		
	dialog.getFileName( lpPath );
	uiString::copy<char, WCHAR>( lpFileName, lpPath );

	// load anim from file
	CAnimMeshComponent* animComponent = m_irrWin->getAnimComponent();
	animComponent->loadAnimFile( lpFileName );

	// update anim data
	updateAnimDataToUI();
}


void CMainFrame::updateAnimDataToUI()
{	
	m_propertyWin->deleteAllItem();

	uiListPropertyGroup *pHeader = m_propertyWin->addGroup(L"Animation header");
	pHeader->enableColText(true);
	pHeader->setColText(L"State name", 0);
	pHeader->setColText(L"Next name", 1);
	pHeader->setColText(L"Anim name", 2);
	pHeader->setColText(L"Time begin", 3);
	pHeader->setColText(L"Duration", 4);
	pHeader->setColText(L"Loop", 5);
	pHeader->setColText(L"Move Position", 6);

	WCHAR	wstringBuff[1024];

	CAnimMeshComponent* animComponent = m_irrWin->getAnimComponent();
	int numAnim = animComponent->getAnimCount();

	for ( int i = 0; i < numAnim; i++ )
	{
		const char *animName = animComponent->getAnimName(i);
		SAnimClip *clipInfo = animComponent->getAnimClip( animName );

		if ( clipInfo == NULL )
			continue;

		uiString::copy<WCHAR, const char>( wstringBuff, animName );
		
		// add new row
		uiListPropertyRow* pRow = m_propertyWin->addRowItem( wstringBuff );
		
		// set name of anim
		pRow->setText( wstringBuff, 2 );
		
		// set time of anim
		uiString::format<WCHAR>( wstringBuff, L"%f", clipInfo->m_time );
		pRow->setText( wstringBuff, 3 );

		// set duration on anim
		uiString::format<WCHAR>( wstringBuff, L"%f", clipInfo->m_duration );
		pRow->setText( wstringBuff, 4 );

		// set looping of anim
		if ( clipInfo->m_loop )
			pRow->setText( L"true", 5 );
		else
			pRow->setText( L"false", 5 );

		// set move position of anim
		if ( clipInfo->m_movePosition )
			pRow->setText( L"true", 6 );
		else
			pRow->setText( L"false", 6 );

	}

	// update list property
	m_propertyWin->updateListProperty();

}

void CMainFrame::listPropertyOnItenChange( uiObject *pSender )
{
	uiListPropertyItem *pItem =	m_propertyWin->getLastClickItem();
	if ( pItem == NULL )
		return;

	if ( pItem->getObjectType() != 2 )
		return;

	uiListPropertyRow *pRow = (uiListPropertyRow*) pItem;

	WCHAR	wstringBuff[1024];
	char	animName[1024];

	pRow->getText( wstringBuff, 2 );
	uiString::copy<char, WCHAR>( animName, wstringBuff );

	// set animation for anim mesh
	CAnimMeshComponent* animComponent = m_irrWin->getAnimComponent();
	animComponent->setAnimation( animName );

}