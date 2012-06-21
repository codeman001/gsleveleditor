//================================================================================================
// Iphone game IG2D
// (C) 2011 La Minh Truong
//================================================================================================

#include "Stdafx.h"

#ifdef WIN32
    #include <windows.h>
#endif

extern void LuaRegisterCFunction();

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

LuaScript* LuaScript::s_pInstance = 0;
LuaScript* LuaScript::GetInstance ()
{
	if (!s_pInstance)
	{
		s_pInstance = new LuaScript();
		if (!s_pInstance) return NULL;
		if (!s_pInstance->Initialize())
		{
			delete s_pInstance;
			return NULL;
		}
	}

	return s_pInstance;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

int Wait(lua_State* state)
{
	const int time_to_wait = lua_tointeger(state, 1);

	lua_pushinteger(state, E_WAIT_DELAY);
	lua_pushinteger(state, time_to_wait);

	return lua_yield(state, 2);
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

int Debug(lua_State* state)
{
	const char* str = lua_tostring(state, 1);
	DEBUG_OUT("Lua debug: %s\n", str);
	return 0;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

bool LuaScript::Initialize ()
{
	//Core function
	REGISTER_C_FUNCTION(Wait);
	REGISTER_C_FUNCTION(Debug);

	//Function in game
	LuaRegisterCFunction();

	return true;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void LuaScript::ShutDown()
{
	delete(s_pInstance);
	s_pInstance = 0;
}

//-------------------------------------------------------------------------------------------------
/// CTOR
//-------------------------------------------------------------------------------------------------

LuaScript::LuaScript()
{
	//initialize Lua
	m_state = lua_open();

	//load Lua base libraries
	luaL_openlibs(m_state);		
	luaL_openlibs(m_state);
	lua_atpanic(m_state, LuaPanic);

	m_threads = new LuaThread[MAX_LUA_THREADS];

	//make lua gc run slower (default value is 200, which means it collects when the mem doubles)
	lua_gc(m_state, LUA_GCSETPAUSE, 300);
}

//-------------------------------------------------------------------------------------------------
/// DTOR
//-------------------------------------------------------------------------------------------------

LuaScript::~LuaScript()
{
	// stop thread
	for (int i = 0; i < MAX_LUA_THREADS; i++)
	{
		m_threads[i].Stop();
	}
	DELETE_ARRAY(m_threads);

	// close Lua
	lua_close(m_state);
}

//-------------------------------------------------------------------------------------------------
/// FUNCTION
//-------------------------------------------------------------------------------------------------

int LuaScript::LuaPanic(lua_State* L)
{
	const char* str = lua_tostring(L, -1);
	DEBUG_OUT(str);
	lua_pop(L, 1);

	lua_getglobal(L, "ERROR");
	int err = (int)lua_tonumber(L, -1);
	DEBUG_OUT("Scripting error: %d", err);

	return 0;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void LuaScript::RegisterCFunction(const char* funcName, lua_CFunction func)
{
	lua_register(m_state, funcName, func);
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

int LuaScript::StartFunction(const char* funcName)
{
	int funcRef = GetFuncRef(funcName);

	if (funcRef < 0)
	{
		return -1;
	}

	int idx = -1;
	for (int i = 0; i < MAX_LUA_THREADS; i++)
	{
		if (m_threads[i].IsRunning())
		{
			if (m_threads[i].m_funcRef == funcRef)
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
	{
		m_threads[idx].Start(m_state, funcRef);
	}
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

int LuaScript::StopFunction(const char* funcName)
{
	int funcRef = GetFuncRef(funcName);

	if (funcRef < 0)
	{
		return -1;
	}

	for (int i = 0; i < MAX_LUA_THREADS; i++)
	{
		if (m_threads[i].m_funcRef == funcRef)
		{
			m_threads[i].Stop();
		}
	}
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

int LuaScript::GetFuncRef(const char* funcName)
{
	lua_getglobal(m_state, funcName);
	int ref = lua_ref(m_state, -1);

	// clean the stack
	lua_settop(m_state, 0);

	return ref;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void LuaScript::LoadFromFile(char* filename)
{
#ifdef IPHONE
    char file[512];
    Utils::GetInstance()->GetFileName(filename, file);
    luaL_loadfile(m_state, file);
#else
    luaL_loadfile(m_state, filename);
#endif
	
	lua_pcall(m_state, 0, 0, 0);

	lua_register(m_state, "Wait", Wait);

	// clean the stack
	lua_settop(m_state, 0);
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void LuaScript::LoadFromBuff(const char* buff, int size)
{
	int status = luaL_loadbuffer(m_state, buff, size, "");
	//int status = luaL_loadstring(m_state, buff);
	
	lua_pcall(m_state, 0, 0, 0);

	lua_register(m_state, "Wait", Wait);

	// clean the stack
	lua_settop(m_state, 0);
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void LuaScript::Update()
{
	for (int i = 0; i < MAX_LUA_THREADS; i++)
	{
		m_threads[i].Update(Game2D::GetInstance()->GetFrameDT());
	}

	// reset stack
	lua_settop(m_state, 0);
}

//-------------------------------------------------------------------------------------------------
/// LUA THREAD CLASS
/// CTOR
//-------------------------------------------------------------------------------------------------

LuaThread::LuaThread()
{
	SetStatus(E_STOPPED);
	m_delay		= 0;
	m_thread	= NULL;
	m_state		= NULL;
}

//-------------------------------------------------------------------------------------------------
/// DTOR
//-------------------------------------------------------------------------------------------------

LuaThread::~LuaThread()
{

}

//-------------------------------------------------------------------------------------------------
/// FUNCTION
//-------------------------------------------------------------------------------------------------

void LuaThread::Start(lua_State* state, int funcRef)
{
	// Reset state
	lua_pop(state, 1);

	// Init thread
	m_state		= state;
	m_thread	= lua_newthread(state);
	m_funcRef	= funcRef;

	// Get function
	lua_getref(m_thread, m_funcRef);

	if(!lua_isnil(m_thread, -1))
	{
		SetStatus(E_RUNNING);
	}
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void LuaThread::Stop()
{
	m_delay	= 0;
	SetStatus(E_STOPPED);
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

bool LuaThread::IsRunning()
{
	return m_status != E_STOPPED;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void LuaThread::HandleYield()
{
	int type = lua_tointeger(m_thread, 1);
	if (type == E_WAIT_DELAY)
	{
		m_delay = lua_tointeger(m_thread, 2);
		SetStatus(type);
	}
}

//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------
void LuaThread::Update(int frameTime)
{
	switch (m_status)
	{
		case E_RUNNING:
		{
			int status = lua_resume(m_thread, 0);

			if(status != LUA_YIELD)
			{
				SetStatus(E_STOPPED);
			}
			else
			{
				HandleYield();
			}
			break;
		}
		case E_WAIT_DELAY:
		{
			m_delay -= frameTime;
			if (m_delay <= 0)
			{
				SetStatus(E_RUNNING);
			}
			break;
		}
		case E_STOPPED:
		{
			break;
		}
	}
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void LuaThread::SetStatus(int status)
{
	m_status = status;
}