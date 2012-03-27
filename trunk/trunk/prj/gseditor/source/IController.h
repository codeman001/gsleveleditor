#ifndef _ICONTROLLER_H_
#define _ICONTROLLER_H_

#include "stdafx.h"
#include "CHistoryManager.h"

// interface controller
class IController
{
public:
	enum IControllerType
	{
		NullController = 0,
		SelectObject,
		RotateObject,
		ScaleObject,
		AddObject,
		AddWaypoint,
		AddCamera,
		NumController
	};

	IController(){}
	virtual ~IController(){}

	// initController
	// Run when choose controller
	virtual void initController()	{}	

	// cancelController
	// Run when cancel (press esc, or change another controller)
	virtual void cancelController()	{}

	// render
	// Event render 2d interface
	virtual void render(){}

	virtual void onKeyUp		(int keyCode){}
	virtual void onKeyDown		(int keyCode){}
	
	virtual void onMouseMove	(int x, int y){}
	virtual void onLMouseUp		(int x, int y){}
	virtual void onRMouseUp		(int x, int y){}

	virtual void onLMouseDown	(int x, int y){}
	virtual void onRMouseDown	(int x, int y){}

	virtual void onMouseWhell	(bool scrollDown, int x, int y){}

	virtual void onMenuCommand( int idCmd ){}

	virtual void onOK(){}
};



class CControllerManager: public uiSingleton<CControllerManager>
{
public:
	IController	*m_currentController;
	IController	**m_controller;

public:
	CControllerManager();
	virtual ~CControllerManager();

public:
	// Choose controler
	void setController( IController::IControllerType controller );

	void onKeyUp		(int keyCode);
	void onKeyDown		(int keyCode);
	void onMouseMove	(int x, int y);
	void onLMouseUp		(int x, int y);
	void onRMouseUp		(int x, int y);
	void onLMouseDown	(int x, int y);
	void onRMouseDown	(int x, int y);
	void onMouseWhell	(bool scrollDown, int x, int y);
	void onMenuCommand	(int idCmd);

	// getCurrentController
	// get current controller
	inline IController* getCurrentController()
	{
		return m_currentController;
	}
};


#endif