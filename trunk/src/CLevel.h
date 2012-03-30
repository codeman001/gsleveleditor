#ifndef _CLEVEL_H_
#define _CLEVEL_H_

#include "stdafx.h"
#include "IView.h"

#include "CGameObject.h"
#include "CGameCamera.h"
#include "CObjTemplateFactory.h"

class CLevel
{
protected:
	CGameCamera	*m_playerCamera;	
public:
	CLevel();

	virtual ~CLevel();

	// init
	// init level
	void init();

	// update
	// update level by frame
	void update();

	// render
	// render level
	void render();
};

#endif