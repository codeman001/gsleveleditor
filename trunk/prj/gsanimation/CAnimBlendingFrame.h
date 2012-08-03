#ifndef _ANIM_BLENDINGFRAME_
#define _ANIM_BLENDINGFRAME_

#include "stdafx.h"
#include "CColladaMeshComponent.h"

class CAnimBlendingFrame: public uiWindow
{
protected:
	CColladaMeshComponent	*m_colladaComponent;

public:
	CAnimBlendingFrame( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent );
	virtual ~CAnimBlendingFrame();
	
	// messageMap
	// Phan tich su kien cho uiForm
	virtual LRESULT	messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	// setColladaComponent
	// update new collada scene
	void setColladaComponent( CColladaMeshComponent *comp );
};

#endif