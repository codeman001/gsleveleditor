#ifndef _IRR_WINDOWS_CONTROLLER_
#define _IRR_WINDOWS_CONTROLLER_

#include "stdafx.h"
#include "CGameColladaSceneNode.h"


class CIrrWindowController: public uiObject
{
protected:
	int		m_controllerState;
	
	bool	m_leftMouseDown;
	int		m_mouseDownX;
	int		m_mouseDownY;
	int		m_lastMouseX;

	CGameColladaSceneNode *m_currentSelect;
	int		m_actionState;

public:
	CGameColladaSceneNode* getSelectNode()
	{
		return m_currentSelect;
	}

public:
	CIrrWindowController();
	virtual ~CIrrWindowController();

	void onLMouseDown(int x, int y);
	void onRMouseDown(int x, int y);

	void onMouseMove(int x, int y);

	void onLMouseUp(int x, int y);
	void onRMouseUp(int x, int y);

public:
	uiEventCallback	_onSelectNode;

	template<class T, void (T::*pTMethod)(uiObject*)>
	void setEventOnNodeSelected( T *pObj )
	{
		_onSelectNode.setEvent<T,pTMethod>(pObj);
	}
};

#endif