#ifndef _CLEVEL_H_
#define _CLEVEL_H_

#include "stdafx.h"
#include "IView.h"
#include "CSnakeObject.h"

class CLevel
{
protected:
	CSnakeObject	*m_snake;
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