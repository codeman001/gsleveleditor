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
	m_nParams	= 0;

	// add function to new thread
	lua_getref(m_thread, m_funcRef);
		
	// call lua
	// lua_atpanic(m_thread, CScriptManager::luaPanic);

	if(!lua_isnil(m_thread, -1))
	{
		// run func when call lua_resume function
		setStatus(FuncRunning);
	}
}

// add parameter for func
void CLuaThread::pushDouble( double d )
{
	luaL_checkstack(m_thread, 1, "too many arguments");
	lua_pushnumber(m_thread, d);
	m_nParams++;
}

void CLuaThread::pushInteger( int i )
{
	luaL_checkstack(m_thread, 1, "too many arguments");
	lua_pushinteger(m_thread, i);
	m_nParams++;
}

void CLuaThread::pushString( const char *s )
{
	luaL_checkstack(m_thread, 1, "too many arguments");
	lua_pushstring(m_thread, s);
	m_nParams++;
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
			int status = lua_resume(m_thread, m_nParams);

			if(status == LUA_YIELD)
			{
				handleYield();
			}
			else
			{
				/*
				switch( status )
				{
				case LUA_ERRRUN:
					printf("lua error run\n");
					break;
				case LUA_ERRSYNTAX:
					printf("lua error syntax\n");
					break;
				case LUA_ERRMEM:
					printf("lua error memory\n");
					break;
				case LUA_ERRERR:
					printf("lua error\n");
					break;
				}
				*/
				setStatus(FuncStopped);	
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
	SEvent errorEvent;
	errorEvent.EventType = EET_LOG_TEXT_EVENT;
	errorEvent.LogEvent.Level = ELL_ERROR;
	errorEvent.LogEvent.Text = lua_tostring(L, -1);

	getIView()->getDevice()->postEventFromUser( errorEvent );	
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

// startFunc
// calc function with paramater
// argType is list param type: 
//	+ i: integer
//	+ d: double
//  + s: string
// example:
// startFunc("function","is",10,"helloWorld")
int CScriptManager::startFunc( const char *func, char *sig, ... )
{
	// call function
	int idx = startFunc(func);
	if ( idx < 0 )
		return idx;

	va_list vl;
	int narg, nres;
	va_start(vl, sig);

	nres = strlen(sig);
	
	double d = 0.0f;
	int i = 0;
	char *s = NULL;
	
	// also push parameter
	for (narg = 0; *sig; narg++) 
	{		
		switch (*sig++) 
		{
		case 'd':
			//double argument
			d = va_arg(vl, double);
			m_threads[idx]->pushDouble(d);
			break;
		case 'i':
			//integer argument
			i = va_arg(vl, int);
			m_threads[idx]->pushInteger(i);
			break;
		case 's':
			//string argument
			s = va_arg(vl, char *);
			m_threads[idx]->pushString(s);
			break;
		case '>':
		default:
			break;
		}
	}

	va_end(vl);
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