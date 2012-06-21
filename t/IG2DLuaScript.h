//================================================================================================
// Iphone game IG2D
// (C) 2011 La Minh Truong
//================================================================================================

#ifndef LUASCRIPT_H
#define LUASCRIPT_H

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
	#include <lstate.h>
};

//-------------------------------------------------------------------------------------------------
/// NOTE: Put class description here
//-------------------------------------------------------------------------------------------------

#define MAX_LUA_THREADS		(10)

enum
{
	E_STOPPED,
	E_RUNNING,
	E_WAIT_DELAY,
};

class LuaThread
{
	private:				
		lua_State* m_thread;
		lua_State* m_state;

	public:
		int	m_status;
		int	m_funcRef;
		int m_delay;

		LuaThread();
		virtual ~LuaThread();

		void Start(lua_State* state, int funcRef);
		void Stop();
		bool IsRunning();
		void HandleYield();
		void Update(int frameTime);		
		void SetStatus(int status);
};

class LuaScript
{
	private:
//=================================================================================================
//      private sprite variable
//=================================================================================================
	lua_State* m_state;
	LuaThread* m_threads;
	
    public:
//=================================================================================================
//      public sprite variable
//=================================================================================================
		
//=================================================================================================
//      public sprite function
//=================================================================================================
        LuaScript();
		~LuaScript();

		/// Return the singleton instance of this class
		static LuaScript* GetInstance();
		static LuaScript* s_pInstance;

		// Initialize the system
		bool Initialize();

		// Shutdown the system
		void ShutDown();

		// This function is called when LUA got error
		static int LuaPanic(lua_State* L);
		void RegisterCFunction(const char* funcName, lua_CFunction func);

		int	GetFuncRef(const char* funcName);
		int	StartFunction(const char* funcName);
		int	StopFunction(const char* funcName);

		void LoadFromFile(char* filename);		
		void LoadFromBuff(const char* buff, int size);
		void Update();
};

#define GetScriptMgr() LuaScript::GetInstance()
#define REGISTER_C_FUNCTION(f) LuaScript::GetInstance()->RegisterCFunction(#f, f)

#endif // LUASCRIPT_H
