#include "stdafx.h"
#include "CScriptManager.h"
#include "IView.h"

using namespace NSScriptManager;



/////////////////////////////////////////
// Basic lua func Implement
/////////////////////////////////////////

int luaWait(lua_State* state)
{
	const int time = lua_tointeger(state, 1);

	lua_pushinteger(state, NSScriptManager::FuncWaitDelay );
	lua_pushinteger(state, time);

	return lua_yield(state, 2);
}

int luaDebug(lua_State* state)
{
	const char* str = lua_tostring(state, 1);
	printf("- lua out: %s\n", str);
	return 0;
}

/////////////////////////////////////////
// Lua Thread Implement
/////////////////////////////////////////

CLuaThread::CLuaThread()
{
	setStatus(FuncStopped);

	m_delay		= 0;
	m_thread	= NULL;
	m_state		= NULL;
}
	
CLuaThread::~CLuaThread()
{
}

// run, stop
void CLuaThread::start(lua_State* state, int funcRef)
{
	// reset state
	lua_pop(state, 1);

	// init thread
	m_state		= state;
	m_thread	= lua_newthread(state);
	m_funcRef	= funcRef;

	// get function
	lua_getref(m_thread, m_funcRef);

	if(!lua_isnil(m_thread, -1))
	{
		setStatus(FuncRunning);
	}
}

void CLuaThread::stop()
{
	m_delay	= 0;
	setStatus(FuncStopped);
}

// handleYield			
void CLuaThread::handleYield()
{
	// get paramater 1 from function "lua_yield"
	EFuncStatus type = (EFuncStatus)lua_tointeger(m_thread, 1);
	
	if (type == FuncWaitDelay)
	{
		// get paramater 2 from function "lua_yield"
		m_delay = lua_tointeger(m_thread, 2);
		setStatus(type);
	}
}

// update per frame
void CLuaThread::update()
{
	int frameTime = (int) getIView()->getTimeStep();

	switch (m_status)
	{
		case FuncRunning:
		{
			int status = lua_resume(m_thread, 0);

			if(status != LUA_YIELD)
			{
				setStatus(FuncStopped);
			}
			else
			{
				handleYield();
			}
			break;
		}
		case FuncWaitDelay:
		{
			m_delay -= frameTime;
			if (m_delay <= 0)
			{
				setStatus(FuncRunning);
			}
			break;
		}
		case FuncStopped:
		{
			break;
		}
	}
}


/////////////////////////////////////////
// Script Manager Implement
/////////////////////////////////////////

CScriptManager::CScriptManager()
{
	//initialize lua
	m_state = lua_open();

	//load lua base libraries
	luaL_openlibs(m_state);		

	// error state
	lua_atpanic(m_state, CScriptManager::luaPanic);

	for ( int i = 0; i < MAX_LUA_THREADS; i++ )
		m_threads[i] = new CLuaThread();

	//make lua gc run slower (default value is 200, which means it collects when the mem doubles)
	lua_gc(m_state, LUA_GCSETPAUSE, 300);

	// init
	init();
}
	
CScriptManager::~CScriptManager()
{
	for (int i = 0; i < MAX_LUA_THREADS; i++)
	{
		m_threads[i]->stop();
		delete m_threads[i];
		m_threads[i] = NULL;
	}

	lua_close( m_state );
}

// luaPanic
// this function is called when LUA got error
int CScriptManager::luaPanic(lua_State* L)
{
	const char* str = lua_tostring(L, -1);
	printf("------------------------\n%s\n------------------------\n", str);
	lua_pop(L, 1);

	lua_getglobal(L, "ERROR");
	int err = (int)lua_tonumber(L, -1);
	printf("------------------------\nsriptErr: %d\n------------------------\n", err);
	return 0;
}

// update lua per frame
void CScriptManager::update()
{
	for (int i = 0; i < MAX_LUA_THREADS; i++)
	{
		m_threads[i]->update();
	}

	// reset stack
	lua_settop(m_state, 0);
}

// getLuaFuncRefId
// get lua ref
int CScriptManager::getLuaFuncRefId( const char *funcName )
{
	lua_getglobal(m_state, funcName);
	int ref = lua_ref(m_state, -1);

	// clean the stack
	lua_settop(m_state, 0);

	return ref;
}

// startFunc
// run a lua func
int CScriptManager::startFunc( const char *funcName )
{
	int funcRef = getLuaFuncRefId(funcName);

	if (funcRef < 0)
	{
		return -1;
	}

	int idx = -1;
	for (int i = 0; i < MAX_LUA_THREADS; i++)
	{
		if (m_threads[i]->isRunning())
		{
			if (m_threads[i]->getFuncRefId() == funcRef)
			{
				return -2;
			}
		}
		else
		{
			idx = i;
			break;
		}
	}

	if (idx >= 0)
		m_threads[idx]->start(m_state, funcRef);

	return idx;
}

// stopFunc
// stop lua func
int CScriptManager::stopFunc( const char *funcName )
{
	int funcRef = getLuaFuncRefId(funcName);

	if (funcRef < 0)
	{
		return 0;
	}

	for (int i = 0; i < MAX_LUA_THREADS; i++)
	{
		if (m_threads[i]->getFuncRefId() == funcRef)
		{
			m_threads[i]->stop();
			break;
		}
	}	
	return 1;
}

// stopAllFunc
// stop all func
void CScriptManager::stopAllFunc()
{
	for (int i = 0; i < MAX_LUA_THREADS; i++)
	{		
		m_threads[i]->stop();		
	}
}

// registerCFunction
// implement a lua function as a C function
void CScriptManager::registerCFunction(const char* funcName, lua_CFunction func)
{
	lua_register(m_state, funcName, func);
}

// compileLuaSource
// compile
void CScriptManager::compileLuaSource(const char* buff, int size)
{
	int status = luaL_loadbuffer(m_state, buff, size, "");	
	lua_pcall(m_state, 0, 0, 0);
	lua_settop(m_state, 0);
}

// init
// init script machine
void CScriptManager::init()
{
	registerCFunction("wait",	luaWait);
	registerCFunction("debug",	luaDebug);
}