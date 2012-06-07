#include "stdafx.h"
#include "CMainFrame.h"
#include "Resource.h"

CMainFrame::CMainFrame()
{
	m_currentFile = "";
	m_currentAnimFile = "";

	// init singleton
	CBinaryUtils::createGetInstance();
	CColladaAnimationFactory::createGetInstance();
}

CMainFrame::~CMainFrame()
{
	CBinaryUtils::releaseInstance();
	CColladaAnimationFactory::releaseInstance();
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

	uiToolbarButton *toolbarButton = pToolbar->addButton(L"Save scene", 1);
	toolbarButton->setEventOnClicked<CMainFrame, &CMainFrame::toolbarSaveScene>( this );

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
	
	m_listView = ref<uiListView>( new uiListView(L"propertyWindow", 0, 0, 1000, 30, false, split) );
	m_listView->selectedFullRow(true);

	m_listView->addColumn(L"Anim name", 300);
	m_listView->addColumn(L"Time begin", 150);
	m_listView->addColumn(L"Duration", 150);
	m_listView->addColumn(L"Loop", 150);
	
	m_listView->setEventOnClicked<CMainFrame, &CMainFrame::listPropertyOnItemChange>( this );
	m_listView->setEventOnDbClicked<CMainFrame, &CMainFrame::listPropertyOnItemEdit>( this );

	split->setWindow( m_irrWin, 0, 0 );
	split->setWindow( m_listView, 1, 0 );
	split->setRowSize( 0, 400 );
	split->setRowSize( 1, 300 );
	
	split->changeWindowStyle( UISTYLE_CHILD );
	split->setDock( this, UIDOCK_FILL );
	split->updateSplit();
	split->showWindow( true );

	m_editorWin = ref<CAnimModifyFrame>( new CAnimModifyFrame(L"Anim editor", 50,50, 900,500,this)  );
	m_editorWin->changeWindowStyle( UISTYLE_RESIZE );
	m_editorWin->setParent(NULL);
	m_editorWin->showWindow(false);

	// set event selected node
	m_irrWin->getController()->setEventOnNodeSelected<CAnimModifyFrame, &CAnimModifyFrame::_onSelectedNode>(m_editorWin);

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
	CGameObject* pObj = m_irrWin->getAnimObject();
	core::vector3df scale =	pObj->getScale();

	if ( keyEvent.getVKeyCode() == VK_ADD || keyEvent.getAnsiKey() == 'i' || keyEvent.getAnsiKey() == 'I' )
	{
		scale *= 2.0f;
		pObj->setScale( scale );
	}
	else if ( keyEvent.getVKeyCode() == VK_SUBTRACT || keyEvent.getAnsiKey() == 'o' || keyEvent.getAnsiKey() == 'O' )
	{
		scale /= 2.0f;
		pObj->setScale( scale );
	}
	else if ( keyEvent.getAnsiKey() == 'l' || keyEvent.getAnsiKey() == 'L' )
	{		
		bool light = pObj->isLighting();

		pObj->setLighting( !light );
		ISceneNode *pNode =	pObj->getSceneNode();

		stack<ISceneNode*>	stackNode;
		
		stackNode.push( pNode );

		while ( stackNode.size() )
		{
			ISceneNode *currentNode = stackNode.top();
			stackNode.pop();

			currentNode->setMaterialFlag( video::EMF_LIGHTING, !light );

			core::list<ISceneNode*>::ConstIterator i = currentNode->getChildren().begin(), 
				end = currentNode->getChildren().end();

			while ( i != end )
			{
				stackNode.push( (*i) );
				i++;
			}
		}

	}

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

void CMainFrame::toolbarSaveScene( uiObject *pSender )
{
	WCHAR lpPath[ MAX_PATH ] = {0};	
	char lpFileName[ MAX_PATH ] = {0};

	if ( m_currentFile.size() == 0 )
	{
		uiSaveOpenDialog dialog;	
		dialog.clearAllFileExt();
		dialog.addFileExt( L"Scene file (.scene)", L"*.scene");
		dialog.addFileExt( L"All files (.*)", L"*.*" );
		if ( dialog.doModal( uiApplication::getRoot(), true ) == false )
			return;

		dialog.getFileName( lpPath );
		uiString::copy<char, WCHAR>( lpFileName, lpPath );
		
		// save binary file
		CColladaMeshComponent* colladaComponent = m_irrWin->getAnimComponent();
		colladaComponent->saveSceneToBinary( lpFileName );

		m_currentFile = lpFileName;
	}
	else
	{
		CColladaMeshComponent* colladaComponent = m_irrWin->getAnimComponent();
		colladaComponent->saveSceneToBinary( m_currentFile.c_str() );		
	}
}

void CMainFrame::toolbarSaveAnim( uiObject *pSender )
{
	WCHAR lpPath[ MAX_PATH ] = {0};	
	char lpFileName[ MAX_PATH ] = {0};

	if ( m_currentAnimFile.size() == 0 )
	{
		uiSaveOpenDialog dialog;	
		dialog.clearAllFileExt();
		dialog.addFileExt( L"Animation (.anim)", L"*.anim");
		dialog.addFileExt( L"All files (.*)", L"*.*" );
		if ( dialog.doModal( uiApplication::getRoot(), true ) == false )
			return;

		dialog.getFileName( lpPath );
		uiString::copy<char, WCHAR>( lpFileName, lpPath );
		
		// save binary file
		CColladaMeshComponent* colladaComponent = m_irrWin->getAnimComponent();
		colladaComponent->saveAnimToBinary( lpFileName );

		m_currentAnimFile = lpFileName;
	}
	else
	{
		CColladaMeshComponent* colladaComponent = m_irrWin->getAnimComponent();
		colladaComponent->saveAnimToBinary( m_currentAnimFile.c_str() );
	}

	//uiString::copy<WCHAR, const char>( lpPath, m_currentAnimFile.c_str() );

	//WCHAR title[1024];
	//swprintf(title, 1024, L"%s - %s", STR_APP_TITLE, lpPath);
	//setCaption( title );
}

void CMainFrame::toolbarLoadMesh( uiObject *pSender )
{
	WCHAR lpPath[ MAX_PATH ] = {0};	
	char lpFileName[ MAX_PATH ] = {0};
	
	uiSaveOpenDialog dialog;	
	dialog.clearAllFileExt();	
	dialog.addFileExt( L"Mesh file (*.dae; *.scene)", L"*.dae;*.scene" );
	dialog.addFileExt( L"All files (.*)", L"*.*" );
	if ( dialog.doModal( uiApplication::getRoot(), false ) == false )
		return;
		
	dialog.getFileName( lpPath );
	uiString::copy<char, WCHAR>( lpFileName, lpPath );

	// load anim from file
	CColladaMeshComponent* colladaComponent = m_irrWin->getAnimComponent();
	colladaComponent->loadFromFile( lpFileName );

	// show editor win
	m_editorWin->setColladaComponent( m_irrWin->getAnimComponent() );
	m_editorWin->showWindow(true);
}

void CMainFrame::toolbarLoadAnimDae( uiObject *pSender )
{
	WCHAR lpPath[ MAX_PATH ] = {0};	
	char lpFileName[ MAX_PATH ] = {0};
	
	uiSaveOpenDialog dialog;	
	dialog.clearAllFileExt();
	dialog.addFileExt( L"dae animation file (*.dae; *.anim)", L"*.dae;*.anim" );
	dialog.addFileExt( L"All files (.*)", L"*.*" );
	if ( dialog.doModal( uiApplication::getRoot(), false ) == false )
		return;
		
	dialog.getFileName( lpPath );
	uiString::copy<char, WCHAR>( lpFileName, lpPath );
	
	CColladaAnimation *colladaAnim = CColladaAnimationFactory::getInstance()->loadAnimation("baseAnim", lpFileName);
	
	// load anim from file
	CColladaMeshComponent* colladaComponent = m_irrWin->getAnimComponent();
	colladaComponent->setAnimationPackage( colladaAnim );


	// update anim data
	updateAnimDataToUI();
}


void CMainFrame::updateAnimDataToUI()
{			
	m_listView->deleteAllRow();		

	CColladaMeshComponent* colladaComponent = m_irrWin->getAnimComponent();
	CColladaAnimation *animPackage = colladaComponent->getCurrentAnimPackage();


	int numAnim = animPackage->getAnimCount();

	for ( int i = 0; i < numAnim; i++ )
	{		
		WCHAR	wstringBuff[1024];		

		SColladaAnimClip *clipInfo = animPackage->getAnim(i);

		if ( clipInfo == NULL )
			continue;

		uiString::copy<WCHAR, const char>( wstringBuff, clipInfo->animName.c_str() );
		
		// set name of anim		
		uiListViewRow *row = m_listView->addRow( wstringBuff );
		
		// set time of anim
		uiString::format<WCHAR>( wstringBuff, L"%f", clipInfo->time );		
		row->addParam( wstringBuff );

		// set duration on anim
		uiString::format<WCHAR>( wstringBuff, L"%f", clipInfo->duration );		
		row->addParam( wstringBuff );

		// set looping of anim
		if ( clipInfo->loop )					
			row->addParam( L"true" );		
		else			
			row->addParam( L"false" );

		row->update();		
	}

}

void CMainFrame::listPropertyOnItemEdit( uiObject *pSender )
{
	uiListViewRowSelected selectRow;
	m_listView->getRowSelected( &selectRow );

	if ( selectRow.size() == 0 )
		return;

	char label[512];
	WCHAR labelW[512];

	const SColladaAnimClip *clip = m_irrWin->getAnimComponent()->getCurrentAnim();

	if ( clip )
	{
		sprintf(label,"Anim editor - %s", clip->animName.c_str());
		uiString::copy<WCHAR, char>( labelW, label );

		m_editorWin->setCaption( labelW );
		m_editorWin->updateTimeLine( NULL );
		m_editorWin->showWindow(true);
	}
}

void CMainFrame::listPropertyOnItemChange( uiObject *pSender )
{
	uiListViewRowSelected selectRow;
	m_listView->getRowSelected( &selectRow );

	if ( selectRow.size() == 0 )
		return;

	uiListViewRow *row = selectRow[0];	

	WCHAR	wstringBuff[1024];
	char	animName[1024];

	row->getParam(0, wstringBuff); 
	uiString::copy<char, WCHAR>( animName, wstringBuff );

	// set animation for anim mesh
	CColladaMeshComponent* colladaComponent = m_irrWin->getAnimComponent();
	colladaComponent->setAnimation( animName );
	colladaComponent->pauseAtFrame( 0 );

}