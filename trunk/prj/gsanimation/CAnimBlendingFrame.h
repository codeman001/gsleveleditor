#ifndef _ANIM_BLENDINGFRAME_
#define _ANIM_BLENDINGFRAME_

#include "stdafx.h"
#include "CColladaMeshComponent.h"

class CAnimBlendingFrame: public uiWindow
{
protected:
	CColladaMeshComponent	*m_colladaComponent;

	uiTrackBar				*m_trackbar;
	uiToolbarButton			*m_playButton;
	uiToolbarButton			*m_stopButton;
	uiComboBox				*m_cmbAnim1;
	uiComboBox				*m_cmbAnim2;
public:
	CAnimBlendingFrame( LPWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent );
	virtual ~CAnimBlendingFrame();
	
	// _OnVScroll
	// event when scroll
	virtual void _OnVScroll( uiScrollBarEvent scrollEvent );

	// messageMap
	// Phan tich su kien cho uiForm
	virtual LRESULT	messageMap(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	// _onToolBarCommand
	// play, stop was press
	virtual void _onToolBarCommand( uiObject *pSender );
	virtual void _onCmbCommand( uiObject *pSender );

	// setColladaComponent
	// update new collada scene
	void setColladaComponent( CColladaMeshComponent *comp );
	void updateBlendAnim();
};

#endif