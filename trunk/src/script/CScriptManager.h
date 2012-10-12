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

#define MAX_LUA_THREADS	10

namespace NSScriptManager
{
	enum EFuncStatus
	{
		FuncStopped = 0,
		FuncRunning,
		FuncWaitDelay
	};

	class CLuaThread
	{
		protected:				
			lua_State* m_thread;
			lua_State* m_state;

			EFuncStatus	m_status;
			int			m_funcRef;
			int			m_nParams;
			int			m_delay;

		public:
			CLuaThread();
			virtual ~CLuaThread();

			// run, stop
			void start(lua_State* state, int funcRef);
			void stop();

			// add parameter for func
			void pushDouble( double d );
			void pushInteger( int i );
			void pushString( const char *s );

			// isRunning
			// check thread is stop!
			bool isRunning()
			{
				return m_status != FuncStopped;
			}

			// handleYield			
			void handleYield();

			// update per frame
			void update();

			// setStatus
			// set state for lua script
			void setStatus(EFuncStatus status)
			{
				m_status = status;
			}

			// getState
			// get current state
			EFuncStatus getState()
			{
				return m_status;
			}

			// getFuncRefId
			// get func ref in lua machine
			int getFuncRefId()
			{
				return m_funcRef;
			}
	};

	class CScriptManager: public uiSingleton<CScriptManager>
	{	
	protected:
		lua_State*	m_state;
		CLuaThread* m_threads[MAX_LUA_THREADS];

	public:
		CScriptManager();
		virtual ~CScriptManager();
	
		// luaPanic
		// this function is called when LUA got error
		static int luaPanic(lua_State* L);

		// update lua per frame
		void update();

		// startFunc
		// run a lua func
		int startFunc( const char *funcName );

		// startFunc
		// calc function with paramater
		// argType is list param type: 
		//	+ i: integer
		//	+ d: double
		//  + s: string
		// example:
		// startFunc("function","is",10,"helloWorld")
		int startFunc( const char *func, const char *argType, ... );

		// stopFunc
		// stop lua func
		int stopFunc( const char *funcName );

		// stopAllFunc
		// stop all func
		void stopAllFunc();

		// registerCFunction
		// implement a lua function as a C function
		void registerCFunction(const char* funcName, lua_CFunction func);

		// compileLuaSource
		// compile
		void compileLuaSource(const char* buff, int size);

	protected:
		// getLuaFuncRefId
		// get lua ref
		int getLuaFuncRefId( const char *funcName );

		// init
		// init script machine
		void init();
	};
	
	#define REGISTER_C_FUNCTION(f) CScriptManager::getInstance()->registerCFunction(#f, f)
}

#endif

