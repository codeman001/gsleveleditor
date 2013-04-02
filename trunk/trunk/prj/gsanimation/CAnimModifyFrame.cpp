#include "stdafx.h"
#include "CAnimModifyFrame.h"
#include "Resource.h"

///////////////////////////////////////
// CTreeContainer implement
///////////////////////////////////////

int k_menuShowNode = 0;
int k_menuEnableNode = 1;
int k_menuEnableBoneBBox = 3;
int k_menuEnableAllChildNode = 5;
int k_menuDisableAllChildNode = 6;

CTreeContainer::CTreeContainer( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent )
	:uiWindow( lpTitle, x,y,w,h, pParent )
{

	// tree view init
	m_treeView = ref<uiTreeView>( new uiTreeView(L"treeNode", 0,0, 10,10, true, this) );
	uiIcon iconTreeNode( MAKEINTRESOURCE(IDI_TREENODE) );
	uiIcon iconTreeMesh( MAKEINTRESOURCE(IDI_TREEMESH) );
	m_treeView->pushSmallImage( &iconTreeNode );
	m_treeView->pushSmallImage( &iconTreeMesh );
	m_treeView->setDock( this, UIDOCK_FILL );

	// menu popup init
	uiMenuItem *item;
	m_treePopupMenu = ref<uiMenuPopup>( new uiMenuPopup() );
	
	item = m_treePopupMenu->appendMenuItem(L"Show node");
	item->setEventOnClick<CTreeContainer, &CTreeContainer::_onMenuCommand>( this );

	item = m_treePopupMenu->appendMenuItem(L"Enable node");
	item->setEventOnClick<CTreeContainer, &CTreeContainer::_onMenuCommand>( this );

	m_treePopupMenu->appendMenuItem();
	item = m_treePopupMenu->appendMenuItem(L"Enable bone bbBox");
	item->setEventOnClick<CTreeContainer, &CTreeContainer::_onMenuCommand>( this );

	m_treePopupMenu->appendMenuItem();
	
	item = m_treePopupMenu->appendMenuItem(L"Enable all child node");
	item->setEventOnClick<CTreeContainer, &CTreeContainer::_onMenuCommand>( this );

	item = m_treePopupMenu->appendMenuItem(L"Disable all child node");
	item->setEventOnClick<CTreeContainer, &CTreeContainer::_onMenuCommand>( this );
		
	changeWindowStyle( UISTYLE_CHILD );
	showWindow(true);
}

CTreeContainer::~CTreeContainer()
{
}

// messageMap
// Phan tich su kien cho uiForm
LRESULT	CTreeContainer::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ( uMsg == WM_CONTEXTMENU )
	{
		POINT pt;
		POINTSTOPOINT(pt, lParam);		
		HWND wndSend = (HWND)wParam;		

		if ( wndSend == m_treeView->getHandle() )
		{
			m_clickTreeItem = NULL;

			POINT point;
			point.x = pt.x;
			point.y = pt.y;
			
			POINT hit = point;
			ScreenToClient(m_treeView->getHandle(), &hit);

			m_clickTreeItem = NULL;
			m_clickTreeItem = m_treeView->getHisTest( hit.x, hit.y );					
			
			CGameColladaSceneNode *colladaNode = (CGameColladaSceneNode*) m_clickTreeItem->getData();

			// disable show node item
			m_treePopupMenu->getItem(k_menuShowNode)->setDisable(true);
			((uiMenuItem*)m_treePopupMenu->getItem(k_menuShowNode))->setCheck(false);

			// disable enable node item
			m_treePopupMenu->getItem(k_menuEnableNode)->setDisable(true);
			((uiMenuItem*)m_treePopupMenu->getItem(k_menuEnableNode))->setCheck(false);
						

			if ( m_clickTreeItem && colladaNode )
			{
				// set check on show node item
				if ( colladaNode->getMesh() != NULL )
				{
					m_treePopupMenu->getItem(k_menuShowNode)->setDisable(false);
					((uiMenuItem*)m_treePopupMenu->getItem(k_menuShowNode))->setCheck( colladaNode->isVisible() );
				}				
				
				// set check on node item
				m_treePopupMenu->getItem(k_menuEnableNode)->setDisable(false);
				((uiMenuItem*)m_treePopupMenu->getItem(k_menuEnableNode))->setCheck( colladaNode->isEnableAnim() );


				// disable enable node item
				bool checkBoneBBox = colladaNode->isEnableBoneBBox();
				m_treePopupMenu->getItem(k_menuEnableBoneBBox)->setDisable(false);
				((uiMenuItem*)m_treePopupMenu->getItem(k_menuEnableBoneBBox))->setCheck(checkBoneBBox);
			}			
			
			// popup menu
			m_treePopupMenu->popup( this, point.x, point.y );
		}		
	}	

	return uiWindow::messageMap(hWnd, uMsg, wParam, lParam);
}

// _onMenuCommand
// event when menu item click
void CTreeContainer::_onMenuCommand( uiObject *pSender )
{
	uiMenuItem *pItem = (uiMenuItem*)pSender;
	int pos = pItem->getPos();

	if ( pos == k_menuShowNode )
	{
		if ( m_clickTreeItem )
		{			
			CGameColladaSceneNode *node = (CGameColladaSceneNode*) m_clickTreeItem->getData();
			if ( node )
				node->setVisible( !node->isVisible() );
		}
	}
	else if ( pos == k_menuEnableNode )
	{
		if ( m_clickTreeItem )
		{			
			CGameColladaSceneNode *node = (CGameColladaSceneNode*) m_clickTreeItem->getData();
			if ( node )
			{
				node->setEnableAnim( !node->isEnableAnim() );
			}
		}
	}
	else if ( pos == k_menuEnableBoneBBox )
	{
		if ( m_clickTreeItem )
		{			
			CGameColladaSceneNode *node = (CGameColladaSceneNode*) m_clickTreeItem->getData();
			if ( node )
			{
				node->setBoneBBox(!node->isEnableBoneBBox() );
			}
		}
	}
	else if ( pos == k_menuEnableAllChildNode )
	{
		if ( m_clickTreeItem )
		{			
			CGameColladaSceneNode *node = (CGameColladaSceneNode*) m_clickTreeItem->getData();
			if ( node )
				node->enableAnimOnAllChild( true );
		}
	}
	else if ( pos == k_menuDisableAllChildNode )
	{
		if ( m_clickTreeItem )
		{			
			CGameColladaSceneNode *node = (CGameColladaSceneNode*) m_clickTreeItem->getData();
			if ( node )
				node->enableAnimOnAllChild( false );
		}	
	}

}


///////////////////////////////////////
// CAnimModifyFrame implement
///////////////////////////////////////

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
	playButton->setEventOnClicked<CAnimModifyFrame, &CAnimModifyFrame::_onToolbarCommand>( this );

	uiToolbarButton* pauseButton = pToolbarPlay->addButton(L"Pause animation", iconPauseIndex);
	pauseButton->setEventOnClicked<CAnimModifyFrame, &CAnimModifyFrame::_onToolbarCommand>( this );

	uiToolbarButton* stopButton = pToolbarPlay->addButton(L"Stop animation", iconStopIndex);
	stopButton->setEventOnClicked<CAnimModifyFrame, &CAnimModifyFrame::_onToolbarCommand>( this );

	m_playButton = playButton;	
	m_pauseButton = pauseButton;
	m_stopButton = stopButton;


	uiRebarBand bandToolbarPlay( pToolbarPlay,L"");
	bandToolbarPlay.setWidth( 140 );
	bandToolbarPlay.setMinWidthHeight( 0, 39 );

	
	pRebar->addBand( &bandToolbarPlay );
	pRebar->setDock( this, UIDOCK_TOP );	
	pRebar->showBandBorder( true );
	pRebar->showWindow(true);

	m_mainSplit = ref<uiSplitContainer>( new uiSplitContainer(L"split", 0,0, w, h, this, 1,2) );
	
	// create tree view node
	m_treeContainer = ref<CTreeContainer>( new CTreeContainer(L"treeViewContianer", 0,0,10,10, m_mainSplit) );
	m_treeNode = m_treeContainer->getTreeView();
	m_treeNode->setEventOnSelectChange<CAnimModifyFrame, &CAnimModifyFrame::_onTreeSelect> ( this );

	
	// create info node
	uiWindow *containerWin = ref<uiWindow>( new uiWindow(L"container", 0,0, 100, 100, m_mainSplit) );
	containerWin->changeWindowStyle( UISTYLE_CHILD );
	containerWin->showWindow( true );	

	m_listProperty = ref<uiListProperty>( new uiListProperty(L"property", 0,0, 500, 40, containerWin, 5) );
	m_listProperty->showWindow( true );
	m_listProperty->changeWindowStyle( UISTYLE_CHILD );	
	m_listProperty->setDock( containerWin, UIDOCK_BOTTOM );	
	m_listProperty->setEventOnUpdateProperty<CAnimModifyFrame, &CAnimModifyFrame::_onListPropertyChange>( this );
	//m_listProperty->setEventOnItemChange<CAnimModifyFrame, &CAnimModifyFrame::_onListPropertyChange>( this );

	uiListPropertyGroup *header = m_listProperty->addGroup(L"header");
	header->enableColText(true);
	header->setColText(L"node",0);
	header->setColText(L"frame",1);
	header->setColText(L"x",2);
	header->setColText(L"y",3);
	header->setColText(L"z",4);
	m_listProperty->updateListProperty();

	uiTabControl *tabWin = ref<uiTabControl>( new uiTabControl(L"tabWin", 0,0, 100, 100, containerWin) );
	tabWin->setDock( containerWin, UIDOCK_FILL );
	tabWin->setEventOnTabChanged<CAnimModifyFrame, &CAnimModifyFrame::_onTabChange> (this);

	m_tabTime = tabWin;
	m_currentTab = 0;

	m_timeControlRot = ref<CTimelineControl>( new CTimelineControl(tabWin, 0,0,100,100) );	
	m_timeControlRot->enableChangeTimeAndValue( true );
	m_timeControlRot->setEventOnSelectTime<CAnimModifyFrame,&CAnimModifyFrame::_onSelectTime>( this );
	m_timeControlRot->setEventOnUpdateValue<CAnimModifyFrame,&CAnimModifyFrame::_onUpdateValue>( this );
	m_timeControlRot->setEventOnChangeValue<CAnimModifyFrame,&CAnimModifyFrame::_onChangeValue>(this);

	m_timeControlPos = ref<CTimelineControl>( new CTimelineControl(tabWin, 0,0,100,100) );	
	m_timeControlPos->enableChangeTimeAndValue( true );
	m_timeControlPos->setEventOnSelectTime<CAnimModifyFrame,&CAnimModifyFrame::_onSelectTime>( this );
	m_timeControlPos->setEventOnUpdateValue<CAnimModifyFrame,&CAnimModifyFrame::_onUpdateValue>( this );
	m_timeControlPos->setEventOnChangeValue<CAnimModifyFrame,&CAnimModifyFrame::_onChangeValue>(this);

	tabWin->addTab(L"Rotation keys", m_timeControlRot);
	tabWin->addTab(L"Position keys", m_timeControlPos);

	m_mainSplit->setWindow( m_treeContainer, 0, 0 );
	m_mainSplit->setWindow( containerWin, 0, 1 );
	m_mainSplit->setColSize( 0, 300 );
	m_mainSplit->setColSize( 1, w-200 );
	m_mainSplit->setExpanderSize(5);
	
	m_mainSplit->changeWindowStyle( UISTYLE_CHILD );
	m_mainSplit->setDock( this, UIDOCK_FILL );
	m_mainSplit->updateSplit();
	m_mainSplit->showWindow( true );
	
	SetTimer(getHandle(), 1, 1, 0);	

	m_lastSelectNode = NULL;
}

CAnimModifyFrame::~CAnimModifyFrame()
{
	KillTimer( getHandle(), 1 );
}

// messageMap
// Phan tich su kien cho uiForm
LRESULT	CAnimModifyFrame::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ( uMsg == WM_CLOSE )
	{
		getParent()->showWindow( true );
		getParent()->setForegroundWindow();
		showWindow(false);
		return 0;
	}	
	else if ( uMsg == WM_TIMER )
	{		
		if ( isShow() == true )			
		{			
			if ( m_colladaComponent->isPauseAnim() == false )
			{
				// update time on timeline control
				m_timeControlRot->setCurrentTime( m_colladaComponent->getCurrentFrame() );
				m_timeControlPos->setCurrentTime( m_colladaComponent->getCurrentFrame() );

				m_timeControlRot->update();
				m_timeControlPos->update();
			}
			else
			{
				// set animation frame from timeline control
				if ( m_currentTab == 0 )
				{
					m_colladaComponent->pauseAtFrame( m_timeControlRot->getCurrentTime() );									
					m_timeControlPos->setCurrentTime( m_colladaComponent->getCurrentFrame() );
				}
				else
				{
					m_colladaComponent->pauseAtFrame( m_timeControlPos->getCurrentTime() );
					m_timeControlRot->setCurrentTime( m_colladaComponent->getCurrentFrame() );
				}
				
			}
		}
	}
	return uiWindow::messageMap( hWnd, uMsg, wParam, lParam );
}

void CAnimModifyFrame::_onToolbarCommand( uiObject *pSender )
{
	if ( m_colladaComponent == NULL )
		return;

	if ( pSender == m_playButton )
	{
		m_colladaComponent->resumeAnim();
		m_colladaComponent->onlyEnableAnimTrackChannel(0);

		m_timeControlRot->enableChangeTimeAndValue( false );
		m_timeControlPos->enableChangeTimeAndValue( false );
	}
	else if ( pSender == m_stopButton )
	{
		m_colladaComponent->pauseAtFrame(0);
		m_colladaComponent->onlyEnableAnimTrackChannel(0);

		m_timeControlRot->setCurrentTime(0);
		m_timeControlRot->enableChangeTimeAndValue( true );
		m_timeControlPos->setCurrentTime(0);
		m_timeControlPos->enableChangeTimeAndValue( true );

		m_timeControlPos->update();
		m_timeControlRot->update();
	}
	else
	{
		m_colladaComponent->pauseAtFrame( m_colladaComponent->getCurrentFrame() );
		m_colladaComponent->onlyEnableAnimTrackChannel(0);

		m_timeControlRot->enableChangeTimeAndValue( true );
		m_timeControlPos->enableChangeTimeAndValue( true );
	}
}

void CAnimModifyFrame::_onTabChange( uiObject *pSender )
{
	m_currentTab = (int)m_tabTime->getSelectTab();
}

void CAnimModifyFrame::setNodeInfoToProperty( CGameColladaSceneNode *node )
{			
	if ( m_listProperty->getItemCount() == 2 )
		m_listProperty->deleteItem(1);	

	wchar_t stringw[512];

	uiString::copy<wchar_t, const c8>( stringw, node->getName() );
	uiListPropertyRow *pRow = m_listProperty->addRowItem( stringw );
	pRow->setControl( UILISTPROPERTY_EDIT, 0, NULL );

	int select = -1;
	int numValue = 0;
	CTimelineControl *timeControl = NULL;

	if ( m_currentTab == 0 )
	{
		// rotation
		timeControl = m_timeControlRot;
	}
	else
	{
		// position
		timeControl = m_timeControlPos;
	}

	select = timeControl->getSelectTimeID();
	numValue = timeControl->getValueCount();

	if ( select == -1 )
		select = 0;
	
	if ( numValue > 0 && select < numValue )
	{
		STimelineValue value = timeControl->getValue(select);
		
		// tag control to row
		pRow->setData( timeControl );

		uiListPropertyEdit *edit = NULL;		
		
		swprintf(stringw, 512, L"%f", value.time);
		pRow->setText( stringw, 1 );
		edit = (uiListPropertyEdit*)pRow->setControl( UILISTPROPERTY_EDIT, 1, NULL );
		edit->setNumberFloatValueOnly(true);

		swprintf(stringw, 512, L"%f", value.x);
		pRow->setText( stringw, 2 );
		edit = (uiListPropertyEdit*)pRow->setControl( UILISTPROPERTY_EDIT, 2, NULL );
		edit->setNumberFloatValueOnly(true);

		swprintf(stringw, 512, L"%f", value.y);
		pRow->setText( stringw, 3 );
		edit = (uiListPropertyEdit*)pRow->setControl( UILISTPROPERTY_EDIT, 3, NULL );
		edit->setNumberFloatValueOnly(true);

		swprintf(stringw, 512, L"%f", value.z);
		pRow->setText( stringw, 4 );
		edit = (uiListPropertyEdit*)pRow->setControl( UILISTPROPERTY_EDIT, 4, NULL );
		edit->setNumberFloatValueOnly(true);
	}

	m_listProperty->updateListProperty();
}


#include "CIrrWindowController.h"
void CAnimModifyFrame::_onSelectedNode( uiObject *pSender )
{
	if ( m_lastSelectNode )
		m_lastSelectNode->showName( false );

	CIrrWindowController *controller = (CIrrWindowController*)pSender;

	m_lastSelectNode = controller->getSelectNode();

	if ( m_lastSelectNode )
	{
		updateTimeLine( m_lastSelectNode );
		m_lastSelectNode->showName( true );	

		// update info
		setNodeInfoToProperty( m_lastSelectNode );
	}	
}

void CAnimModifyFrame::_onTreeSelect( uiObject *pSender )
{
	uiListTreeViewItem listNodeSelect;
	m_treeNode->getItemSelected( &listNodeSelect );

	if ( listNodeSelect.size() > 0 )
	{
		uiTreeViewItem* item = listNodeSelect[0];
		
		if ( m_lastSelectNode )
			m_lastSelectNode->showName( false );

		CGameColladaSceneNode *node = (CGameColladaSceneNode*)item->getData();
		if ( node )
		{
			updateTimeLine( node );
			node->showName( true );
			m_lastSelectNode = node;

			// update info
			setNodeInfoToProperty( m_lastSelectNode );
		}
	}
}

void CAnimModifyFrame::_onSelectTime( uiObject *pSender )
{
	if ( m_lastSelectNode )
		setNodeInfoToProperty( m_lastSelectNode );
}

void CAnimModifyFrame::_onUpdateValue( uiObject *pSender )
{
	if ( m_lastSelectNode )
	{
		setNodeInfoToProperty( m_lastSelectNode );

		updateTimelineToSceneNode( (CTimelineControl*) pSender );
	}
}

void CAnimModifyFrame::_onChangeValue( uiObject *pSender )
{	
	if ( m_lastSelectNode == NULL )
		return;

	CGameAnimationTrack *track = m_lastSelectNode->getAnimation(0)->getTrack(0);

	if ( pSender == m_timeControlRot )
	{
		int timeID = m_timeControlRot->getSelectTimeID();
		STimelineValue& value =	m_timeControlRot->getValue( timeID );

		// get rotation from scenenode
		CGameAnimationTrack::SRotationKey &key = track->RotationKeys[timeID];
		core::vector3df rotVec = key.rotation.getMatrix().getRotationDegrees();

		if ( rotVec.X > 180 )
			rotVec.X -= 360;
		if ( rotVec.Y > 180 )
			rotVec.Y -= 360;
		if ( rotVec.Z > 180 )
			rotVec.Z -= 360;
		
		// set rotation from timeline
		rotVec.X = value.x;
		rotVec.Y = value.y;
		rotVec.Z = value.z;

		core::matrix4 mat;
		mat.setRotationDegrees( rotVec );

		// update rotation
		core::quaternion quat( mat );
		key.rotation = quat;
	}
	else if ( pSender == m_timeControlPos )
	{
		int timeID = m_timeControlPos->getSelectTimeID();
		STimelineValue& value =	m_timeControlPos->getValue( timeID );

		//update position
		CGameAnimationTrack::SPositionKey &key = track->PositionKeys[timeID];
		key.position.X = value.x;
		key.position.Y = value.y;
		key.position.Z = value.z;
	}
}

void CAnimModifyFrame::_onListPropertyChange( uiObject *pSender )
{
	if ( m_listProperty->getItemCount() == 2 )
	{
		uiListPropertyRow *pRow = (uiListPropertyRow*) m_listProperty->getItem(1);
		CTimelineControl *control = (CTimelineControl*)pRow->getData();
		if ( control == NULL )
			return;
	
		int timeID = control->getSelectTimeID();
		if ( timeID == -1 )
			return;

		STimelineValue& value = control->getValue( timeID );

		wchar_t stringw[512];
		float floatValue;

		pRow->getText( stringw, 1 );	// time
		swscanf(stringw, L"%f", &floatValue );
			
		value.time = floatValue;
		if ( value.time < 0 )
			value.time = 0.0f;

		pRow->getText( stringw, 2 );	// x
		swscanf(stringw, L"%f", &floatValue );
		value.x = floatValue;

		pRow->getText( stringw, 3 );	// y
		swscanf(stringw, L"%f", &floatValue );
		value.y = floatValue;

		pRow->getText( stringw, 4 );	// z
		swscanf(stringw, L"%f", &floatValue );
		value.z = floatValue;
			
		// update value to node
		updateTimelineToSceneNode( control );
		
		control->update();

		// select current time
		control->selectFrame( value.time );

	}
}


void CAnimModifyFrame::setColladaComponent( CColladaMeshComponent *comp )
{
	// delete all 
	{
		m_treeNode->deleteAllItem();
		uiTreeViewItem *root = m_treeNode->addItem(L"colladaTreeNode");
		
		m_colladaComponent = comp;

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

	updateTimeLine( NULL );		
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

void CAnimModifyFrame::updateTimelineToSceneNode( CTimelineControl *control )
{
	if ( control == m_timeControlRot )
	{
		// clear old key frames
		m_lastSelectNode->getAnimation(0)->getTrack(0)->RotationKeys.clear();
		
		// add new key frames
		int n = m_timeControlRot->getValueCount();
		for ( int i = 0; i < n; i++ )
		{
			STimelineValue& value =	m_timeControlRot->getValue(i);
			core::vector3df rot(value.x, value.y, value.z);
			
			core::matrix4 mat;
			mat.setRotationDegrees( rot );

			core::quaternion quat( mat );

			CGameAnimationTrack::SRotationKey key;
			key.frame = value.time;
			key.rotation = quat;

			m_lastSelectNode->getAnimation(0)->getTrack(0)->RotationKeys.push_back( key );

		}			
	}
	else if ( control == m_timeControlPos )
	{
		// clear old key frames		
		m_lastSelectNode->getAnimation(0)->getTrack(0)->PositionKeys.clear();
		
		// add new key frames
		int n = m_timeControlRot->getValueCount();
		for ( int i = 0; i < n; i++ )
		{
			STimelineValue& value =	m_timeControlPos->getValue(i);
			core::vector3df pos(value.x, value.y, value.z);
			
			CGameAnimationTrack::SPositionKey key;
			key.frame = value.time;
			key.position = pos;

			m_lastSelectNode->getAnimation(0)->getTrack(0)->PositionKeys.push_back( key );

		}
	}

	updateTimeLine( m_lastSelectNode );

}

void CAnimModifyFrame::updateTimeLine( CGameColladaSceneNode *node )
{
	m_timeControlRot->clearAllValue();
	m_timeControlPos->clearAllValue();

	if ( node == NULL )
	{
		m_timeControlRot->invalidateRect(NULL, true);
		m_timeControlPos->invalidateRect(NULL, true);
		return;
	}

	CGameAnimationTrack *track = node->getAnimation(0)->getTrack(0);

	// ROTATION
	{		
		// rotation
		int nKeys = track->RotationKeys.size();
		for ( int i = 0; i < nKeys; i++ )
		{
			CGameAnimationTrack::SRotationKey &key = track->RotationKeys[i];			
			
			core::vector3df rotVec = key.rotation.getMatrix().getRotationDegrees();
			if ( rotVec.X > 180 )
				rotVec.X -= 360;
			if ( rotVec.Y > 180 )
				rotVec.Y -= 360;
			if ( rotVec.Z > 180 )
				rotVec.Z -= 360;
			m_timeControlRot->addValue( key.frame, rotVec.X, rotVec.Y, rotVec.Z );
		}

		// set time
		if ( nKeys > 0 )
			m_timeControlRot->setTimeLength( track->RotationKeys.getLast().frame );
	}
	
	// POSITION
	{
		// position
		int nKeys = track->PositionKeys.size();
		for ( int i = 0; i < nKeys; i++ )
		{
			CGameAnimationTrack::SPositionKey &key = track->PositionKeys[i];			
						
			m_timeControlPos->addValue( key.frame, key.position.X, key.position.Y, key.position.Z );			
		}

		// set time
		if ( nKeys > 0 )
			m_timeControlPos->setTimeLength( track->PositionKeys.getLast().frame );
	}

	m_timeControlRot->invalidateRect(NULL, true);
	m_timeControlPos->invalidateRect(NULL, true);
}