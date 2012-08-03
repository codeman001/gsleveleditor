#include "stdafx.h"
#include "CAnimBlenddingFrame.h"

CAnimBlenddingFrame::CAnimBlenddingFrame( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent )
	:uiWindow( lpTitle, x,y,w,h, pParent )
{
}

CAnimBlenddingFrame::~CAnimBlenddingFrame()
{
}
	
// messageMap
// Phan tich su kien cho uiForm
LRESULT	CAnimBlenddingFrame::messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return uiWindow::messageMap( hWnd, uMsg, wParam, lParam );
}

// setColladaComponent
// update new collada scene
void CAnimBlenddingFrame::setColladaComponent( CColladaMeshComponent *comp )
{
	m_colladaComponent = comp;
}