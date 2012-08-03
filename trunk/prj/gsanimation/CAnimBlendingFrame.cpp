#include "stdafx.h"
#include "CAnimBlendingFrame.h"

CAnimBlendingFrame::CAnimBlendingFrame( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent )
	:uiWindow( lpTitle, x,y,w,h, pParent )
{
	m_colladaComponent = NULL;

	m_trackbar = ref<uiTrackBar>( new uiTrackBar( L"Trackbar", 0,0, 200, 40, this, true, true ) );
	m_trackbar->setRange( 0,10 );
}

CAnimBlendingFrame::~CAnimBlendingFrame()
{
}

// _OnHScroll
// event when scroll
void CAnimBlendingFrame::_OnVScroll( uiScrollBarEvent scrollEvent )
{
}

// messageMap
// Phan tich su kien cho uiForm
LRESULT	CAnimBlendingFrame::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return uiWindow::messageMap( hWnd, uMsg, wParam, lParam );
}

// setColladaComponent
// update new collada scene
void CAnimBlendingFrame::setColladaComponent( CColladaMeshComponent *comp )
{
	m_colladaComponent = comp;
}