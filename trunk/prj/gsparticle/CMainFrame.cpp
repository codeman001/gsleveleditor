#include "stdafx.h"
#include "CMainFrame.h"
#include "Resource.h"

CMainFrame::CMainFrame()
{	
	m_currentParticle = NULL;
	m_selectTreeItem = NULL;

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

	uiMenu *pMainMenu = ref<uiMenu> (new uiMenu());
	uiKeyAccelEntry e;
	
	// add file popup
	uiMenu *pFile =	pMainMenu->appendMenuPopup( L"&File" );
	
	uiMenuItem *pMenuItem = NULL;
	pMenuItem = pFile->appendMenuItem( L"&New effects\tCtrl+N" );
	pMenuItem->setEventOnClick<CMainFrame, &CMainFrame::onMenuNewEffects>( this );
	e.pushKey( FCONTROL, ('n'-'a' + 1) ,(WORD) pMenuItem->getID() );

	pMenuItem = pFile->appendMenuItem( L"&Open effects\tCtrl+O" );
	pMenuItem->setEventOnClick<CMainFrame, &CMainFrame::onMenuOpenEffects>( this );
	e.pushKey( FCONTROL, ('o'-'a' + 1) ,(WORD) pMenuItem->getID() );

	pMenuItem = pFile->appendMenuItem( L"&Save effects\tCtrl+S" );
	pMenuItem->setEventOnClick<CMainFrame, &CMainFrame::onMenuSaveEffects>( this );
	e.pushKey( FCONTROL, ('s'-'a' + 1) ,(WORD) pMenuItem->getID() );
	
	pFile->appendMenuItem();
	pMenuItem = pFile->appendMenuItem( L"E&xit\tCtrl+X" );
	pMenuItem->setEventOnClick<CMainFrame, &CMainFrame::onMenuExit>( this );
	e.pushKey( FCONTROL, ('x'-'a' + 1) ,(WORD) pMenuItem->getID() );	

	// add menu
	createAccelTable( &e );
	setMenu( pMainMenu );	



	// add rebar
	uiRebar *pRebar = ref<uiRebar>( new uiRebar(L"RebarTool", 0,0, 300,30, this) );	

	// toolbar play
	uiToolbar *pToolbarPlay = ref<uiToolbar>( new uiToolbar(L"Review", 0,0,0,0, 24,24, pRebar ) );

	uiBitmap iconPlay( MAKEINTRESOURCE(IDB_TOOLBARPLAY), true );	
	uiBitmap iconStop( MAKEINTRESOURCE(IDB_TOOLBARSTOP), true );	
	uiBitmap iconDelete( MAKEINTRESOURCE(IDB_TOOLBARDELETE), true );

	DWORD iconPlayIndex = pToolbarPlay->pushImage( &iconPlay );
	DWORD iconStopIndex = pToolbarPlay->pushImage( &iconStop );
	DWORD iconDeleteIndex = pToolbarPlay->pushImage( &iconDelete );
	
	m_playParticleButton = pToolbarPlay->addButton(L"Play particle", iconPlayIndex);
	m_playParticleButton->setEventOnClicked<CMainFrame, &CMainFrame::onToolbarPlayStopParticle>(this);

	m_stopParticleButton = pToolbarPlay->addButton(L"Stop particle", iconStopIndex);
	m_stopParticleButton->setEventOnClicked<CMainFrame, &CMainFrame::onToolbarPlayStopParticle>(this);	

	m_deleteParticleButton = pToolbarPlay->addButton(L"Delete particle, affector", iconDeleteIndex);
	m_deleteParticleButton->setEventOnClicked<CMainFrame, &CMainFrame::onToolbarDeleteParticle>(this);

	uiRebarBand bandToolbarPlay( pToolbarPlay,L"Review particle");
	bandToolbarPlay.setWidth( 140 );
	

	// toolbar emitter/affector
	uiToolbar *pToolbarEdit = ref<uiToolbar>( new uiToolbar(L"Edit", 0,0,0,0, 24,24, pRebar ) );	
	pToolbarEdit->setButtonWidth(80);
	pToolbarEdit->setTextRight(true);

	uiBitmap iconEmitter( MAKEINTRESOURCE(IDB_TOOLBAREMITTER), true );	
	uiBitmap iconAffector( MAKEINTRESOURCE(IDB_TOOLBARAFFECTOR), true );	

	DWORD iconEmitterIndex	= pToolbarEdit->pushImage( &iconEmitter );
	DWORD iconAffectorIndex = pToolbarEdit->pushImage( &iconAffector );

	m_emitterButton = pToolbarEdit->addButton(L"Emitter", iconEmitterIndex);
	uiMenuPopup *pMenu = ref<uiMenuPopup>( new uiMenuPopup() );	

	m_mnuPointEmitter		= pMenu->appendMenuItem(L"Point");
	m_mnuBoxEmitter			= pMenu->appendMenuItem(L"Box");
	m_mnuCylinderEmitter	= pMenu->appendMenuItem(L"Cylinder");
	m_mnuRingEmitter		= pMenu->appendMenuItem(L"Ring");
	m_mnuSphereEmitter		= pMenu->appendMenuItem(L"Sphere");

	// add event handler
	m_mnuPointEmitter->setEventOnClick		<CMainFrame, &CMainFrame::onToolbarEmiter>( this );
	m_mnuBoxEmitter->setEventOnClick		<CMainFrame, &CMainFrame::onToolbarEmiter>( this );
	m_mnuCylinderEmitter->setEventOnClick	<CMainFrame, &CMainFrame::onToolbarEmiter>( this );
	m_mnuRingEmitter->setEventOnClick		<CMainFrame, &CMainFrame::onToolbarEmiter>( this );
	m_mnuSphereEmitter->setEventOnClick		<CMainFrame, &CMainFrame::onToolbarEmiter>( this );


	m_emitterButton->setButtonType( UITOOLBARBUTTON_DROPDOWN );
	m_emitterButton->setMenuPopup( pMenu );	

	m_affectorButton = pToolbarEdit->addButton(L"Affector", iconAffectorIndex);	
	pMenu = ref<uiMenuPopup>( new uiMenuPopup() );	

	m_mnuFadeOutAffector	= pMenu->appendMenuItem(L"Fade out");
	m_mnuGravityAffector	= pMenu->appendMenuItem(L"Gravity");
	m_mnuRotationAffector	= pMenu->appendMenuItem(L"Rotation");
	m_mnuScaleAffector		= pMenu->appendMenuItem(L"Scale");
	m_mnuAttractionAffector	= pMenu->appendMenuItem(L"Attraction");

	// add event handler
	m_mnuFadeOutAffector->setEventOnClick		<CMainFrame, &CMainFrame::onToolbarAffector>( this );
	m_mnuGravityAffector->setEventOnClick		<CMainFrame, &CMainFrame::onToolbarAffector>( this );
	m_mnuRotationAffector->setEventOnClick		<CMainFrame, &CMainFrame::onToolbarAffector>( this );
	m_mnuScaleAffector->setEventOnClick			<CMainFrame, &CMainFrame::onToolbarAffector>( this );
	m_mnuAttractionAffector->setEventOnClick	<CMainFrame, &CMainFrame::onToolbarAffector>( this );			

	m_affectorButton->setButtonType( UITOOLBARBUTTON_DROPDOWN );
	m_affectorButton->setMenuPopup( pMenu );	

	uiRebarBand bandToolbarEdit( pToolbarEdit,L"Edit particle");
	bandToolbarEdit.setBreakBand( true );
	bandToolbarEdit.setMinWidthHeight( 0, 39 );

	// add band to rebar	
	pRebar->addBand( &bandToolbarPlay );
	pRebar->addBand( &bandToolbarEdit );

	pRebar->setDock( this, UIDOCK_TOP );	
	pRebar->showBandBorder( true );
	pRebar->showWindow(true);


	// add main view
	uiSplitContainer *split = ref<uiSplitContainer>( new uiSplitContainer(L"MainSplit", 0,0, 400, 400, this, 1, 2) );	
	uiSplitContainer *splitLeft = ref<uiSplitContainer>( new uiSplitContainer(L"LeftSplit", 0,0, 400, 400, split, 2, 1) );
	
	m_effectTreeWin		= ref<uiTreeView>( new uiTreeView(L"EffectTree", 0,0, 100, 100, false, splitLeft) );	
	m_effectTreeWin->setEventOnSelectChange<CMainFrame, &CMainFrame::onTreeEffectChange>( this );
	m_effectTreeWin->changeWindowStyle( UISTYLE_CHILD );	

	m_effectPropertyWin	= ref<uiListProperty>( new uiListProperty(L"EffectProperty", 0,0, 100, 100, splitLeft, 2) ) ;
	m_effectPropertyWin->changeWindowStyle( UISTYLE_CHILD );	
	m_effectPropertyWin->showWindow( true );
	m_effectPropertyWin->setEventOnUpdateProperty<CMainFrame, &CMainFrame::onPropertyEffectChange> (this);		

	m_irrWin = ref<CIrrWindow>( new CIrrWindow(L"irrWindow", this) );

	// config split left
	splitLeft->setWindow( m_effectTreeWin, 0, 0 );
	splitLeft->setWindow( m_effectPropertyWin, 1, 0 );
	splitLeft->setRowSize( 0, 200 );
	splitLeft->setRowSize( 1, h/2 );
	splitLeft->setExpanderSize( 5 );
	splitLeft->changeWindowStyle( UISTYLE_CHILD );
	splitLeft->showWindow( true );

	// config main split
	split->setWindow( splitLeft, 0, 0 );
	split->setWindow( m_irrWin,  0, 1 );
		
	split->setColSize( 0, 300 );
	split->setColSize( 1, w - 300 );	

	split->setExpanderSize( 5 );
	split->setDock( this, UIDOCK_FILL );
	split->changeWindowStyle( UISTYLE_CHILD );
	split->updateWindow();		
	
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
	wndclass.hIconSm = LoadIcon (NULL, MAKEINTRESOURCE(IDI_GSPARTICLE) );
	wndclass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_GSPARTICLE));

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


void CMainFrame::setAttribToPropertyControl( irr::io::IAttributes *attrb )
{	
	m_effectPropertyWin->deleteAllItem();
	
	WCHAR	lpAttribNameW[1024] = {0};
	WCHAR	lpAttribValueW[1024] = {0};

	int attrbCount = attrb->getAttributeCount();
	for ( int i = 0; i < attrbCount; i++ )
	{
		io::E_ATTRIBUTE_TYPE atrbType = attrb->getAttributeType(i);
		
		const char *lpAttribName = (const char *)attrb->getAttributeName(i);
		core::stringc value = attrb->getAttributeAsString(i);

		// convert to nameW		
		uiString::copy<WCHAR, const char>( lpAttribNameW, lpAttribName );
		uiString::copy<WCHAR, const char>( lpAttribValueW, value.c_str() );

		// create new row
		uiListPropertyRow *row = m_effectPropertyWin->addRowItem( lpAttribNameW );
		row->setText( lpAttribValueW, 1 );

		// set edit
		if ( atrbType != io::EAT_ENUM )
			row->setControl( UILISTPROPERTY_EDIT, 1, NULL );

	}
	
	m_effectPropertyWin->reDrawInterface();

}

void CMainFrame::getAttribFromPropertyControl( irr::io::IAttributes *attrb )
{
	if ( m_currentParticle == NULL )
		return;

	irr::io::IAttributes *currentAttrb = getIView()->getDevice()->getFileSystem()->createEmptyAttributes();
	m_currentParticle->serializeAttributes( currentAttrb );
	
	WCHAR	lpAttribValueW[1024] = {0};
	char	lpAttribValue[1024] = {0};

	uiListPropertyItem *pItem = m_effectPropertyWin->getItem(0);

	uiListPropertyRow* pRow = (uiListPropertyRow*) pItem;
	pRow->getText( lpAttribValueW, 1 );
	uiString::copy<char, WCHAR>( lpAttribValue, lpAttribValueW );
	uiString::trim<char>( lpAttribValue );

	// get particle component
	CGameObject *pParticle = m_irrWin->getParticle();	
	CParticleComponent *pParticleComponent = (CParticleComponent*)pParticle->getComponent( IObjectComponent::Particle );	

	// get particle info
	SParticleInfo *psInfo = pParticleComponent->getParticleInfo( m_currentParticle );
	psInfo->texture = lpAttribValue;

	if ( uiString::length<char>( lpAttribValue ) > 0 )
	{
		std::string textureFile = lpAttribValue;

		// set texture to particle
		ITexture *pTex = getIView()->getDriver()->getTexture( textureFile.c_str() );
		if ( pTex == NULL )
		{
			WCHAR appPath[MAX_PATH];
			char  appPathA[MAX_PATH];

			uiApplication::getAppPath(appPath, MAX_PATH);
			uiString::copy<char, WCHAR>( appPathA, appPath  );
			
			std::string path = appPathA;
			path += "\\";
			path += textureFile;
			
			pTex = getIView()->getDriver()->getTexture(path.c_str());
		}

		if ( pTex )
		{
			m_currentParticle->setMaterialTexture(0, pTex );
			m_currentParticle->setMaterialFlag(	video::EMF_LIGHTING, false );
			m_currentParticle->setMaterialType(	video::EMT_TRANSPARENT_ADD_COLOR);
		}

	}

	// start time
	pRow = (uiListPropertyRow*)m_effectPropertyWin->getItem(1);
	pRow->getText( lpAttribValueW, 1 );
	uiString::copy<char, WCHAR>( lpAttribValue, lpAttribValueW );
	uiString::trim<char>( lpAttribValue );

	if ( uiString::length<char>( lpAttribValue ) > 0 )	
		sscanf( lpAttribValue, "%d", &psInfo->startTime );
	else
		psInfo->startTime = 0;

	// life time
	pRow = (uiListPropertyRow*)m_effectPropertyWin->getItem(2);
	pRow->getText( lpAttribValueW, 1 );
	uiString::copy<char, WCHAR>( lpAttribValue, lpAttribValueW );
	uiString::trim<char>( lpAttribValue );
	
	if ( uiString::length<char>( lpAttribValue ) > 0 )
		sscanf( lpAttribValue, "%d", &psInfo->lifeTime );
	else
		psInfo->startTime = -1;


	int nRow = m_effectPropertyWin->getItemCount(); 
	for ( int i = 3; i < nRow; i++ )
	{
		pItem = m_effectPropertyWin->getItem(i);

		// get type of attrb
		io::E_ATTRIBUTE_TYPE atrbType = currentAttrb->getAttributeType( i - 3 );

		uiListPropertyRow *pRow = (uiListPropertyRow*) pItem;
		pRow->getText( lpAttribValueW, 1 );


		uiString::copy<char, WCHAR>( lpAttribValue, lpAttribValueW );
		const char *lpAttribName = (const char *)currentAttrb->getAttributeName(i - 3);

		switch( atrbType )
		{
		case io::EAT_STRING:
			{
				attrb->addString( lpAttribName, lpAttribValue );
			}
			break;
		case io::EAT_INT:
			{
				int value;
				sscanf( lpAttribValue, "%d", &value );
				attrb->addInt( lpAttribName, value );
			}
			break;
		case io::EAT_FLOAT:
			{
				float value;
				sscanf( lpAttribValue, "%f", &value );
				attrb->addFloat( lpAttribName, value );
			}
			break;
		case io::EAT_BOOL:
			{
				if ( strcmp( lpAttribValue, "true") == 0 )
					attrb->addBool( lpAttribName, true );
				else
					attrb->addBool( lpAttribName, false );
			}
			break;
		case io::EAT_ENUM:
			{
				core::array< core::stringc > listEnum;
				currentAttrb->getAttributeEnumerationLiteralsOfEnumeration( i - 1, listEnum );
				
				c8** enumValue;
				int numOfEnum = listEnum.size();

				enumValue = new c8*[ numOfEnum + 1 ];			
				int i = 0;
				for ( i = 0; i < numOfEnum; i++ )
				{
					enumValue[i] = new c8[ listEnum[i].size() + 1 ];
					strcpy( enumValue[i], listEnum[i].c_str() );
				}	
				enumValue[i] = 0;

				attrb->addEnum( lpAttribName, lpAttribValue, enumValue );
				
				for ( i = 0; i < numOfEnum; i++ )				
					delete enumValue[i];

				delete enumValue;
			}
			break;
		case io::EAT_VECTOR3D:
			{
				core::vector3df v;
				sscanf(lpAttribValue, "%f,%f,%f", &v.X, &v.Y, &v.Z );
				attrb->addVector3d( lpAttribName, v );
			}
			break;
		case io::EAT_COLOR:
			{
				u32 r,g,b,a;
				sscanf(lpAttribValue, "%d,%d,%d", &r, &g, &b, &a );
				video::SColor color( a, r, g, b );
				attrb->addColor( lpAttribName, color );
			}
			break;	
		default:
			{			
			}
			break;
		}
	}

	currentAttrb->drop();

}

uiTreeViewItem *CMainFrame::getTreeItem( IParticleSystemSceneNode *ps )
{
	int nCount = (int)m_effectTreeWin->getChildCount();
	for ( int i = 0; i < nCount; i++ )
	{
		uiTreeViewItem *pTreeItem = m_effectTreeWin->getChild(i);

		if ( pTreeItem->getData() == (uiObject*)m_currentParticle )
		{
			return pTreeItem;
		}
	}
	return NULL;
}

int CMainFrame::getChildTreeID( uiTreeViewItem *p, uiTreeViewItem *child )
{	
	DWORD count = p->getChildCount();
	for ( DWORD i = 0; i < count; i++ )
	{
		if ( p->getChild(i) == child )
			return i;
	}
	return -1;
}


void CMainFrame::onMenuNewEffects( uiObject *pSender )
{
	m_effectTreeWin->deleteAllItem();
	m_effectPropertyWin->deleteAllItem();
	
	CGameObject *pParticle = m_irrWin->getParticle();	
	CParticleComponent *pParticleComponent = (CParticleComponent*)pParticle->getComponent( IObjectComponent::Particle );
	pParticleComponent->removeAllParticle();

	m_currentFile = "";
	setCaption( STR_APP_TITLE );	
}

void CMainFrame::onMenuOpenEffects( uiObject *pSender )
{
	IView *pView = getIView();

	WCHAR lpPath[ MAX_PATH ] = {0};	
	char lpFileName[ MAX_PATH ] = {0};
	
	uiSaveOpenDialog dialog;	
	dialog.clearAllFileExt();
	dialog.addFileExt( L"Particle (.xml)", L"*.xml" );
	dialog.addFileExt( L"All files (.*)", L"*.*" );
	if ( dialog.doModal( uiApplication::getRoot(), false ) == false )
		return;
	
	// new renew document
	onMenuNewEffects(this);
		
	dialog.getFileName( lpPath );
	uiString::copy<char, WCHAR>( lpFileName, lpPath );

	CGameObject *pParticle = m_irrWin->getParticle();	
	CParticleComponent *pParticleComponent = (CParticleComponent*)pParticle->getComponent( IObjectComponent::Particle );
	
	// load particle system
	pParticleComponent->loadXML( lpFileName );

	int i = 0;
	IParticleSystemSceneNode *ps = pParticleComponent->getParticle(i);
	
	wchar_t	lpText[1024];

	while (ps)
	{
		uiString::copy<WCHAR, const c8>( lpText, ps->getName() );

		uiTreeViewItem *pTreeItemPs = m_effectTreeWin->addItem( lpText );
		pTreeItemPs->setData( (uiObject*) ps );

		int j = 0;		
		IParticleAffector *affector = ps->getAffector(j);
		while ( affector != NULL )
		{
			uiString::copy<WCHAR, const c8>( lpText, affector->getDebugName() );
			pTreeItemPs->addChild( lpText );

			affector = ps->getAffector(++j);
		}

		pTreeItemPs->expandChild( true );
		pTreeItemPs->update();

		ps = pParticleComponent->getParticle(++i);		
	}

	m_currentParticle = NULL;

	m_currentFile = lpFileName;
	WCHAR title[1024];
	swprintf(title, 1024, L"%s - %s", STR_APP_TITLE, lpPath);
	setCaption( title );	
}

void CMainFrame::onMenuSaveEffects( uiObject *pSender )
{
	CGameObject *pParticle = m_irrWin->getParticle();	
	CParticleComponent *pParticleComponent = (CParticleComponent*)pParticle->getComponent( IObjectComponent::Particle );

	WCHAR lpPath[ MAX_PATH ] = {0};	
	char lpFileName[ MAX_PATH ] = {0};

	if ( m_currentFile.size() == 0 )
	{
		uiSaveOpenDialog dialog;	
		dialog.clearAllFileExt();
		dialog.addFileExt( L"Particle (.xml)", L"*.xml");
		dialog.addFileExt( L"All files (.*)", L"*.*" );
		if ( dialog.doModal( uiApplication::getRoot(), true ) == false )
			return;

		dialog.getFileName( lpPath );
		uiString::copy<char, WCHAR>( lpFileName, lpPath );
		pParticleComponent->saveXML( lpFileName );

		m_currentFile = lpFileName;
	}
	else
	{
		pParticleComponent->saveXML( m_currentFile.c_str() );		
	}

	uiString::copy<WCHAR, const char>( lpPath, m_currentFile.c_str() );

	WCHAR title[1024];
	swprintf(title, 1024, L"%s - %s", STR_APP_TITLE, lpPath);
	setCaption( title );

}

void CMainFrame::onMenuExit( uiObject *pSender )
{
	if ( GetFocus() == this->getHandle() )
	{
		if ( question(L"Do you want exit?", L"exit") == true )
			uiApplication::exit();
	}
}

void CMainFrame::onTreeEffectChange( uiObject *pSender )
{
	uiListTreeViewItem listSelect;
	m_effectTreeWin->getItemSelected( &listSelect );	

	IParticleSystemSceneNode *particle = NULL;

	if ( listSelect.size() > 0 )
	{
		uiTreeViewItem *pTreeItem = listSelect.front();
		uiTreeViewItem *pParentItem = pTreeItem->getFather();

		// set current tree item
		m_selectTreeItem = pTreeItem;
		
		if ( pParentItem == NULL )
		{
			// particle item
			IParticleSystemSceneNode *ps = (IParticleSystemSceneNode*) pTreeItem->getData();
			particle = ps;
		}
		else
		{
			// affector item
			IParticleSystemSceneNode *ps = (IParticleSystemSceneNode*) pParentItem->getData();
			particle = ps;		
		}
	}

	if ( m_currentParticle == particle )
		return;

	m_currentParticle = particle;
	if ( m_currentParticle == NULL )
		return;

	// get particle component
	CGameObject *pParticle = m_irrWin->getParticle();	
	CParticleComponent *pParticleComponent = (CParticleComponent*)pParticle->getComponent( IObjectComponent::Particle );	

	// get particle info
	SParticleInfo *psInfo = pParticleComponent->getParticleInfo( m_currentParticle );

	if ( psInfo )
	{		
		// get attrib
		irr::io::IAttributes *attrib = getIView()->getDevice()->getFileSystem()->createEmptyAttributes();
				
		attrib->addString( "Texture", (const c8*) psInfo->texture.c_str() );
		attrib->addInt("StartTime", psInfo->startTime);
		attrib->addInt("LifeTime", psInfo->lifeTime);

		m_currentParticle->getEmitter()->setMinParticlesPerSecond( psInfo->minParticle );
		m_currentParticle->getEmitter()->setMaxParticlesPerSecond( psInfo->maxParticle );

		m_currentParticle->serializeAttributes(  attrib );

		// set attrib to control
		setAttribToPropertyControl( attrib );

		attrib->drop();
	}
}

void CMainFrame::onPropertyEffectChange( uiObject *pSender )
{
	if ( m_currentParticle == NULL )
		return;

	CGameObject *pParticle = m_irrWin->getParticle();	
	CParticleComponent *pParticleComponent = (CParticleComponent*)pParticle->getComponent( IObjectComponent::Particle );	

	// get attrib
	irr::io::IAttributes *attrib = getIView()->getDevice()->getFileSystem()->createEmptyAttributes();
	getAttribFromPropertyControl( attrib );

	m_currentParticle->deserializeAttributes(  attrib );

	SParticleInfo *psInfo =	pParticleComponent->getParticleInfo(m_currentParticle);	
	psInfo->minParticle = m_currentParticle->getEmitter()->getMinParticlesPerSecond();
	psInfo->maxParticle = m_currentParticle->getEmitter()->getMaxParticlesPerSecond();

	attrib->drop();
}

void CMainFrame::onToolbarPlayStopParticle( uiObject *pSender )
{
	CGameObject *pParticle = m_irrWin->getParticle();	
	CParticleComponent *pParticleComponent = (CParticleComponent*)pParticle->getComponent( IObjectComponent::Particle );	

	if ( pSender == m_playParticleButton )
		pParticleComponent->startParticle();
	else
		pParticleComponent->stopParticle();

}

void CMainFrame::onToolbarDeleteParticle( uiObject *pSender )
{
	if ( m_currentParticle == NULL )
	{
		alert(L"Please select particle or affector!", L"Delete particle");
		return;
	}

	CGameObject *pParticle = m_irrWin->getParticle();	
	CParticleComponent *pParticleComponent = (CParticleComponent*)pParticle->getComponent( IObjectComponent::Particle );

	if ( m_selectTreeItem && m_selectTreeItem->getFather() == NULL )
	{
		WCHAR	text[1024];
		swprintf(text, 1024, L"Do you want delete the selected particle?");

		if ( question( text, L"Delete particle" ) == true )
		{			
			int id = pParticleComponent->getParticleID( m_currentParticle );
			if ( id >= 0 )
			{				
				// clear property
				m_effectPropertyWin->deleteAllItem();

				// remove on treeview
				uiTreeViewItem *pTreeItem = getTreeItem( m_currentParticle );
				if ( pTreeItem )
					m_effectTreeWin->deleteItem( pTreeItem );

				// remove particle
				pParticleComponent->removeParticle( id );
			}
		}
	}
	else
	{
		WCHAR	text[1024];
		swprintf(text, 1024, L"Do you want delete the selected affector?");

		if ( question( text, L"Delete affector" ) == true )
		{
			int id = pParticleComponent->getParticleID( m_currentParticle );
			if ( id >= 0 )
			{								
				// remove on treeview
				uiTreeViewItem *pTreeItem = m_selectTreeItem;
				uiTreeViewItem *pParentItem = pTreeItem->getFather();

				if ( pTreeItem )
				{
					int childID = getChildTreeID( pParentItem, m_selectTreeItem );
					if ( childID >= 0 )
					{
						// delete affector
						m_currentParticle->removeAffector( childID );
						
						pParentItem->destroyChild( m_selectTreeItem );
						m_selectTreeItem = pParentItem;

						m_currentParticle = NULL;
						onTreeEffectChange( this );
					}
				}
			}
		}	// question
	}
}

void CMainFrame::onToolbarEmiter( uiObject *pSender )
{
	CGameObject *pParticle = m_irrWin->getParticle();
	if ( pParticle == NULL )
		return;
	
	CParticleComponent *pParticleComponent = (CParticleComponent*)pParticle->getComponent( IObjectComponent::Particle );
	if ( pParticleComponent == NULL )
		return;

	// create new particle system
	IParticleSystemSceneNode *ps = pParticleComponent->createParticle();
	ps->getMaterial(0).setFlag( EMF_LIGHTING, false );	
	ps->setName("effect");
	

	// create emitter
	IParticleEmitter *emitter = NULL;
		

	if ( pSender == m_mnuPointEmitter )
	{
		ps->setName("pointEmitter");
		emitter = ps->createPointEmitter();		
	}
	else if ( pSender == m_mnuBoxEmitter )
	{
		ps->setName("boxEmitter");
		emitter = ps->createBoxEmitter();
	}
	else if ( pSender == m_mnuCylinderEmitter )
	{
		ps->setName("cylinderEmitter");
		emitter = ps->createCylinderEmitter( core::vector3df(0,0,0), 40, core::vector3df(0,1,0), 40 );
	}
	else if ( pSender == m_mnuRingEmitter )
	{
		ps->setName("ringEmitter");
		emitter = ps->createRingEmitter( core::vector3df(0,0,0), 40, 4 );
	}
	else if ( pSender == m_mnuSphereEmitter )
	{
		ps->setName("sphereEmitter");
		emitter = ps->createSphereEmitter( core::vector3df(0,0,0), 40 );
	}

	if ( emitter )
	{
		ps->setEmitter( emitter );

		SParticleInfo *psInfo = pParticleComponent->getParticleInfo( ps );
		
		psInfo->minParticle = ps->getEmitter()->getMinParticlesPerSecond();
		psInfo->maxParticle = ps->getEmitter()->getMaxParticlesPerSecond();

		emitter->drop();
	}

	WCHAR	label[512];
	uiString::copy<WCHAR,const irr::c8>( label, ps->getName() );

	uiTreeViewItem *pTreeItem =	m_effectTreeWin->addItem( label );
	m_effectTreeWin->selectItem( pTreeItem );

	pTreeItem->setData( (uiObject*) ps );	

	m_currentParticle = ps;
	m_selectTreeItem = pTreeItem;

	onTreeEffectChange( this );
}

void CMainFrame::onToolbarAffector( uiObject *pSender )
{
	if ( m_currentParticle == NULL )
		return;

	IParticleAffector *affector = NULL;

	WCHAR	label[512];
	
	if ( pSender == m_mnuFadeOutAffector )
	{
		affector = m_currentParticle->createFadeOutParticleAffector();		
	}
	else if ( pSender == m_mnuGravityAffector )
	{
		affector = m_currentParticle->createGravityAffector();		
	}
	else if ( pSender == m_mnuRotationAffector )
	{
		affector = m_currentParticle->createRotationAffector();		
	}
	else if ( pSender == m_mnuScaleAffector )
	{				
		affector = m_currentParticle->createScaleParticleAffector();		
	}
	else if ( pSender == m_mnuAttractionAffector )
	{
		affector = m_currentParticle->createAttractionAffector( core::vector3df(0,0,0) );		
	}

	// add new affector
	if ( affector == NULL )
		return;

	uiString::copy<WCHAR,const irr::c8>( label, affector->getDebugName() );
	m_currentParticle->addAffector( affector );
	
	// create tree item
	uiTreeViewItem *pTreeItem =	getTreeItem( m_currentParticle );
	if ( pTreeItem )
	{
		uiTreeViewItem *pTreeItemAffector = pTreeItem->addChild( label );		
		m_selectTreeItem = pTreeItem;
	}

	affector->drop();

	pTreeItem->expandChild(true);
	pTreeItem->update();

	m_currentParticle = NULL;
	onTreeEffectChange( this );
}