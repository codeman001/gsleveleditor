#ifndef _SCRIPT_MANAGER_H_
#define _SCRIPT_MANAGER_H_

#include "stdafx.h"

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
	#include <lstate.h>
};

class CScriptManager: public uiSingleton<CScriptManager>
{
public:
	CScriptManager();
	virtual ~CScriptManager();

protected:

};

#endif

