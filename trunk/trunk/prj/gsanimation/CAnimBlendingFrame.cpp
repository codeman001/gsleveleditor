#include "stdafx.h"
#include "resource.h"
#include "CAnimBlendingFrame.h"

CAnimBlendingFrame::CAnimBlendingFrame( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent )
	:uiWindow( lpTitle, x,y,w,h, pParent )
{
	m_colladaComponent = NULL;

	// add rebar
	uiRebar *pRebar = ref<uiRebar>( new uiRebar(L"RebarTool", 0,0, 300,30, this) );	

	// toolbar play
	uiToolbar *pToolbarPlay = ref<uiToolbar>( new uiToolbar(L"Animation", 0,0,0,0, 32,32, pRebar ) );

	uiBitmap iconPlay( MAKEINTRESOURCE(IDB_TOOLBARPLAY), true );
	uiBitmap iconStop( MAKEINTRESOURCE(IDB_TOOLBARSTOP), true );

	DWORD iconPlayIndex = pToolbarPlay->pushImage( &iconPlay );
	DWORD iconStopIndex = pToolbarPlay->pushImage( &iconStop );	
	
	uiToolbarButton* playButton = pToolbarPlay->addButton(L"Play blend animation", iconPlayIndex);	
	playButton->setEventOnClicked<CAnimBlendingFrame, &CAnimBlendingFrame::_onToolBarCommand>( this );
	
	uiToolbarButton* stopButton = pToolbarPlay->addButton(L"Stop blend animation", iconStopIndex);
	stopButton->setEventOnClicked<CAnimBlendingFrame, &CAnimBlendingFrame::_onToolBarCommand>( this );

	m_playButton = playButton;	
	m_stopButton = stopButton;


	uiRebarBand bandToolbarPlay( pToolbarPlay,L"");
	bandToolbarPlay.setWidth( 140 );
	bandToolbarPlay.setMinWidthHeight( 0, 39 );

	
	pRebar->addBand( &bandToolbarPlay );
	pRebar->setDock( this, UIDOCK_TOP );	
	pRebar->showBandBorder( true );
	pRebar->showWindow(true);


	int paddingLeft = 100;
	int paddingRight = 4;
	int paddingTop = 4;	

	m_cmbAnim1 = ref<uiComboBox>( new uiComboBox(L"SelectAnim1", 0,0,200,500, true, this) );
	m_cmbAnim1->setEventOnSelectChange<CAnimBlendingFrame, &CAnimBlendingFrame::_onCmbCommand>(this);
	m_cmbAnim1->setMargin( paddingLeft, paddingRight, paddingTop,0 );	
	m_cmbAnim1->setDock( this, UIDOCK_TOP );
	
	m_cmbAnim2 = ref<uiComboBox>( new uiComboBox(L"SelectAnim2", 0,0,200,500, true, this) );
	m_cmbAnim2->setEventOnSelectChange<CAnimBlendingFrame, &CAnimBlendingFrame::_onCmbCommand>(this);
	m_cmbAnim2->setMargin( paddingLeft, paddingRight, paddingTop,0 );
	m_cmbAnim2->setDock( this, UIDOCK_TOP );

	m_trackbar = ref<uiTrackBar>( new uiTrackBar( L"Trackbar", 0,0, 200, 30, this, true, true ) );
	m_trackbar->setMargin( paddingLeft, paddingRight,paddingTop,0 );
	m_trackbar->setDock( this, UIDOCK_FILL );
	m_trackbar->setRange( 0,50 );
}

CAnimBlendingFrame::~CAnimBlendingFrame()
{
}

// messageMap
// Phan tich su kien cho uiForm
LRESULT	CAnimBlendingFrame::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return uiWindow::messageMap( hWnd, uMsg, wParam, lParam );
}

// _onToolBarCommand
// play, stop was press
void CAnimBlendingFrame::_onToolBarCommand( uiObject *pSender )
{	
	if ( m_colladaComponent == NULL )
		return;

	if ( pSender == m_playButton )
	{
		m_colladaComponent->setCurrentFrame( 0.0f, 0 );
		m_colladaComponent->setCurrentFrame( 0.0f, 1 );
		m_colladaComponent->resumeAnim( 0 );
		m_colladaComponent->resumeAnim( 1 );
		updateBlendAnim();
	}
	else if ( pSender == m_stopButton )
	{		
		m_colladaComponent->pauseAtFrame( 0.0f, 0 );
		m_colladaComponent->pauseAtFrame( 0.0f, 1 );
	}
}

// _OnHScroll
// event when scroll
void CAnimBlendingFrame::_OnVScroll( uiScrollBarEvent scrollEvent )
{
	if ( m_colladaComponent == NULL )
		return;

	updateBlendAnim();
}

void CAnimBlendingFrame::_onCmbCommand( uiObject *pSender )
{
	if ( m_colladaComponent == NULL )
		return;	

	wchar_t animW[1024] = {0};
	char anim[1024] = {0};

	// set anim 1
	if ( pSender == m_cmbAnim1 )
	{
		m_cmbAnim1->getSelectString(animW);
		uiString::copy<char, wchar_t>(anim, animW);
		m_colladaComponent->setAnimation( anim, 0 );	
	}

	// set anim 2
	if ( pSender == m_cmbAnim2 )
	{
		m_cmbAnim2->getSelectString(animW);
		uiString::copy<char, wchar_t>(anim, animW);
		m_colladaComponent->setAnimation( anim, 1 );
	}

	m_colladaComponent->pauseAtFrame( 0.0f, 0 );
	m_colladaComponent->pauseAtFrame( 0.0f, 1 );
}

void CAnimBlendingFrame::updateBlendAnim()
{
	if ( m_colladaComponent == NULL )
		return;
		
	// enable blend anim
	m_colladaComponent->enableAnimTrackChanel( 0, true );
	m_colladaComponent->enableAnimTrackChanel( 1, true );	

	// synchronized blend anim
	float f = 1.0f - m_trackbar->getPosition()/50.0f;
	m_colladaComponent->synchronizedByTimeScale( f );
}

// setColladaComponent
// update new collada scene
void CAnimBlendingFrame::setColladaComponent( CColladaMeshComponent *comp )
{
	m_colladaComponent = comp;	
	m_cmbAnim1->clearItem();	
	m_cmbAnim2->clearItem();

	if ( m_colladaComponent == NULL )
		return;

	CColladaAnimation* animPackage = m_colladaComponent->getCurrentAnimPackage();	
	if ( animPackage == NULL )
		return;

	for ( int i = 0; i < animPackage->getAnimCount(); i++ )
	{
		wchar_t animName[1024];

		uiString::copy<wchar_t, const char>( animName,  animPackage->getAnim(i)->animName.c_str() );
		
		m_cmbAnim1->addString( animName );
		m_cmbAnim2->addString( animName );
	}

}