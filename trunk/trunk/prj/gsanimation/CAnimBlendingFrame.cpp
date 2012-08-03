#include "stdafx.h"
#include "CAnimBlendingFrame.h"

CAnimBlendingFrame::CAnimBlendingFrame( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent )
	:uiWindow( lpTitle, x,y,w,h, pParent )
{
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

// setColladaComponent
// update new collada scene
void CAnimBlendingFrame::setColladaComponent( CColladaMeshComponent *comp )
{
	m_colladaComponent = comp;
}