#ifndef _IRRWINDOWS_
#define _IRRWINDOWS_

#include "stdafx.h"
#include "IView.h"

#include "CGameCamera.h"
#include "CGameOxyzSceneNode.h"
#include "CGameGSCameraAnimator.h"

#include "CAnimMeshComponent.h"

class CIrrWindow: public uiWindow, public IView
{
protected:
	CGameCamera *m_designCamera;

	CGameObject *m_animObject;

public:

	CIrrWindow( WCHAR* lpString, uiWindow *pParent );
	
	virtual ~CIrrWindow();

	inline IView *getIView()
	{
		return (IView*)this;
	}

	void createIrrDevice();

	void initScene();

	void destroyScene();

	void irrUpdate();
	
	inline CGameObject* getAnimObject()
	{
		return m_animObject;
	}

	CAnimMeshComponent* getAnimComponent()
	{
		return (CAnimMeshComponent*) m_animObject->getComponent( IObjectComponent::AnimMesh );
	}

	virtual void _OnSize(uiSizeEvent sizeEvent, int nWidth, int nHeight);
	virtual void _OnMouseMove	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnMouseWheel	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnLButtonUp	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnLButtonDown	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnMButtonUp	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnMButtonDown	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnRButtonUp	( uiMouseEvent mouseEvent, int x, int y );
	virtual void _OnRButtonDown	( uiMouseEvent mouseEvent, int x, int y );	
};

#endif