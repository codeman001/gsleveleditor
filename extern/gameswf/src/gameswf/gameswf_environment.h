#pragma once
// gameswf_environment.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// ActionScript "environment", essentially VM state?

#ifndef GAMESWF_ENVIRONMENT_H
#define GAMESWF_ENVIRONMENT_H

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_value.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_player.h"

namespace gameswf
{

	#define GLOBAL_REGISTER_COUNT 4

	struct character;
	struct sprite_instance;
	struct as_object;

	tu_string get_full_url(const tu_string& workdir, const char* url);

	//
	// with_stack_entry
	//
	// The "with" stack is for Pascal-like with-scoping.

	struct with_stack_entry
	{
		smart_ptr<as_object>	m_object;
		int	m_block_end_pc;
		
		with_stack_entry()
			:
			m_object(NULL),
			m_block_end_pc(0)
		{
		}

		with_stack_entry(as_object* obj, int end)
			:
			m_object(obj),
			m_block_end_pc(end)
		{
		}
	};

	struct as_environment
	{
		array<as_value>	m_stack;
		as_value	m_global_register[GLOBAL_REGISTER_COUNT];
		array<as_value>	m_local_register;	// function2 uses this
		smart_ptr<as_object>	m_target;

		// For local vars.  Use empty names to separate frames.
		struct frame_slot
		{
			tu_string	m_name;
			as_value	m_value;

			frame_slot() {}
			frame_slot(const tu_string& name, const as_value& val) : m_name(name), m_value(val) {}
		};
		array<frame_slot>	m_local_frames;

		weak_ptr<player> m_player;

		as_environment(player* player) :
			m_player(player)
		{
		}

//		~as_environment() {}
		bool	set_member(const tu_stringi& name, const as_value& val);
		bool	get_member(const tu_stringi& name, as_value* val);


		character*	get_target() const;
		void set_target(character* target);
		void set_target(as_value& target, character* original_target);

		// stack access/manipulation
		// @@ TODO do more checking on these

		// changes size of stack
		void	push(const as_value& val) 
		{
			m_stack.push_back(val); 
			//printf("push: size of stack = %d\n", m_stack.size());
		}

		void	push(const tu_string& val) 
		{
			m_stack.push_back(val); 
		}

		void	push(const tu_string* val) 
		{
			m_stack.push_back(val); 
		}

		void	push(const char* val) 
		{
			m_stack.push_back(val); 
		}

		void	push(const wchar_t* val) 
		{
			m_stack.push_back(val); 
		}

		void	push(bool val) 
		{
			m_stack.push_back(val); 
		}

		void	push(int val) 
		{
			m_stack.push_back(val); 
		}

		void	push(float val) 
		{
			m_stack.push_back(val); 
		}

		void	push(double val) 
		{
			m_stack.push_back(val); 
		}

		void	push(as_object* val) 
		{
			m_stack.push_back(val); 
		}

		void	push(as_c_function_ptr val) 
		{
			m_stack.push_back(val); 
		}

		void	push(as_s_function* val) 
		{
			m_stack.push_back(val); 
		}

		// changes size of stack
		as_value	pop()
		{
			as_value result = m_stack.back();
			m_stack.pop_back(); 
//			printf("pop: size of stack = %d\n", m_stack.size());
			return result; 
		}

		// changes size of stack
		void	drop(int count)
		{
			m_stack.resize(m_stack.size() - count); 
//			printf("pop: size of stack = %d\n", m_stack.size());
		}

		as_value&	top(int dist) { return m_stack[m_stack.size() - 1 - dist]; }
		as_value&	bottom(int index) { return m_stack[index]; }

		int	get_top_index() const { return m_stack.size() - 1; }

		as_value	get_variable(const tu_string& varname, const array<with_stack_entry>& with_stack, int* with_stack_index = NULL) const;
		// no path stuff:
		as_value	get_variable_raw(const tu_string& varname, const array<with_stack_entry>& with_stack, int* with_stack_index = NULL) const;

		void	set_variable(const tu_string& path, const as_value& val, const array<with_stack_entry>& with_stack);
		// no path stuff:
		void	set_variable_raw(const tu_string& path, const as_value& val, const array<with_stack_entry>& with_stack);

		void	set_local(const tu_string& varname, const as_value& val);
		void	add_local(const tu_string& varname, const as_value& val);	// when you know it doesn't exist.
		void	declare_local(const tu_string& varname);	// Declare varname; undefined unless it already exists.

		// Parameter/local stack frame management.
		int	get_local_frame_top() const { return m_local_frames.size(); }
		void	set_local_frame_top(int t) { assert(t <= m_local_frames.size()); m_local_frames.resize(t); }
		void	add_frame_barrier() { m_local_frames.push_back(frame_slot()); }

		// Local registers.
		void	add_local_registers(int register_count)
		{
			// Flash 8 can have zero register (+1 for zero)
			m_local_register.resize(m_local_register.size() + register_count + 1);
		}
		void	drop_local_registers(int register_count)
		{
			// Flash 8 can have zero register (-1 for zero)
			m_local_register.resize(m_local_register.size() - register_count - 1);
		}

		as_value* get_register(int reg);
		void set_register(int reg, const as_value& val);

		// may be used in outside of class instance
		static bool	parse_path(const tu_string& var_path, tu_string* path, tu_string* var);

		// Internal.
		int	find_local(const tu_string& varname, bool ignore_barrier) const;
		character* load_file(const char* url, const as_value& target);
		as_object*	find_target(const as_value& target) const;
		as_object*	find_target(const char* target) const;
		void clear_refs(hash<as_object*, bool>* visited_objects, as_object* this_ptr);
		player* get_player() const;
		root* get_root() const;

		private:

		as_value*	local_register_ptr(int reg);

	};

	inline as_value* as_environment::get_register(int reg)
	{
		as_value* val = local_register_ptr(reg);
		IF_VERBOSE_ACTION(log_msg("-------------- get_register(%d): %s at 0x%X\n", 
			reg, val->to_string(), val->to_object()));
		return val;
	}

	inline void as_environment::set_register(int reg, const as_value& val)
	{
		IF_VERBOSE_ACTION(log_msg("-------------- set_register(%d): %s at 0x%X\n",
			reg, val.to_string(), val.to_object()));
		*local_register_ptr(reg) = val;
	}

	inline as_value* as_environment::local_register_ptr(int reg)
	// Return a pointer to the specified local register.
	// Local registers are numbered starting with 1.
	//
	// Return value will never be NULL.  If reg is out of bounds,
	// we log an error, but still return a valid pointer (to
	// global reg[0]).  So the behavior is a bit undefined, but
	// not dangerous.
	{
		// We index the registers from the end of the register
		// array, so we don't have to keep base/frame
		// pointers.

		assert(reg >=0 && reg <= m_local_register.size());

		// Flash 8 can have zero register (-1 for zero)
		return &m_local_register[m_local_register.size() - reg - 1];
	}

	inline player* as_environment::get_player() const
	{
		return m_player.get_ptr();
	}

	inline root* as_environment::get_root() const
	{
		return m_player->get_root();
	}

	// Parameters/environment for C functions callable from ActionScript.
	struct fn_call
	{
		as_value* result;
		as_object* this_ptr;
		const as_value& this_value;	// Number or String or Boolean value
		as_environment* env;
		int nargs;
		int first_arg_bottom_index;
		const char* name;

		fn_call(as_value* res_in, const as_value& this_in, as_environment* env_in, int nargs_in, int first_in, const char* name_in) :
			result(res_in),
			this_value(this_in),
			env(env_in),
			nargs(nargs_in),
			first_arg_bottom_index(first_in),
			name(name_in)
		{
			this_ptr = this_in.to_object();
		}

		as_value& arg(int n) const
		// Access a particular argument.
		{
			assert(n < nargs);
			return env->bottom(first_arg_bottom_index - n);
		}

		inline player* get_player() const
		{
			assert(env);
			return env->get_player();
		}
		inline root* get_root() const
		{
			assert(env);
			return env->get_player()->get_root();
		}
	};

	struct fn_call_iterator
	{
		fn_call_iterator(const fn_call& args) : m_args(args), m_index(0) {}

		bool has_next() const
		{
			return m_index < m_args.nargs;
		}

		int get_pos() const
		{
			return m_index;
		}

		as_value& peek_next() const
		{
			return m_args.arg(m_index);
		}

		as_value& next() const
		{
			return m_args.arg(m_index++);
		}

	private:
		const fn_call& m_args;
		mutable int m_index;
	};
}

#endif
