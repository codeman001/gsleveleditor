#pragma once
// gameswf_action.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Implementation and helpers for SWF actions.


#ifndef GAMESWF_ACTION_H
#define GAMESWF_ACTION_H

#include "gameswf/gameswf_value.h"
#include "gameswf/gameswf_environment.h"
#include "gameswf/gameswf_object.h"
#include "gameswf/gameswf_types.h"
#include "base/container.h"
#include "base/smart_ptr.h"
#include "base/membuf.h"
#include <wchar.h>

#ifndef ACTION_BUFFER_PROFILLING
#	define ACTION_BUFFER_PROFILLING 0
#endif

namespace gameswf
{
	struct as_environment;
	struct as_c_function;
	struct as_s_function;

	exported_module const char*	call_method_parsed(
		as_environment* env,
		as_object* this_ptr,
		const char* method_name,
		const char* method_arg_fmt,
		va_list args);
		
	exported_module tu_string	call_method(
		as_environment* env,
		as_object* this_ptr,
		const char* method_name,
		const as_value* arguments,
		int argument_count
		);

	const char* get_gameswf_version();

	// Dispatching methods from C++.
	exported_module as_value	call_method(
		const as_value& method,	as_environment* env, const as_value& this_ptr,
		int nargs, int first_arg_bottom_index, const char* name = "???");

	//
	// event_id
	//
	// For keyDown and stuff like that.

	struct event_id
	{
		// These must match the function names in event_id::get_function_name()
		enum id_code
		{
			INVALID,

			// These are for buttons & sprites.
			PRESS,
			RELEASE,
			RELEASE_OUTSIDE,
			ROLL_OVER,
			ROLL_OUT,
			DRAG_OVER,
			DRAG_OUT,
			KEY_PRESS,

			// These are for sprites only.
			INITIALIZE,
			LOAD,
			UNLOAD,
			ENTER_FRAME,
			MOUSE_DOWN,
			MOUSE_UP,
			MOUSE_MOVE,
			KEY_DOWN,
			KEY_UP,
			DATA,

			CONSTRUCT,
			SETFOCUS,
			KILLFOCUS,			

			// MovieClipLoader events
			ONLOAD_COMPLETE,
			ONLOAD_ERROR,
			ONLOAD_INIT,
			ONLOAD_PROGRESS,
			ONLOAD_START,

			EVENT_COUNT
		};

		// 'm_unused' was added because of problem with hash<event_id, ...>
		// the reason: sizeof(unsigned char+unsigned char+character*)  == 8 
		// and
		//						 sizeof(unsigned char+unsigned char+Uint16+character*)  == 8
		// I think that compiler do aligment
		unsigned char	m_id;
		unsigned char	m_key_code;
		Uint16	m_unused;
		array<as_value>* m_args;

		event_id() :
			m_id(INVALID),
			m_key_code(key::INVALID),
			m_unused(0),
			m_args(NULL)
		{
		}

		event_id(id_code id, array<as_value>* args) :
			m_id((unsigned char) id),
			m_key_code(key::INVALID),
			m_unused(0),
			m_args(args)
		{
		}

		event_id(id_code id, key::code c = key::INVALID) :
			m_id((unsigned char) id),
			m_key_code((unsigned char) c),
			m_unused(0),
			m_args(NULL)
		{
			// For the button key events, you must supply a keycode.
			// Otherwise, don't.
			assert((m_key_code == key::INVALID && (m_id != KEY_PRESS))
				|| (m_key_code != key::INVALID && (m_id == KEY_PRESS)));
		}

		bool	operator==(const event_id& id) const
		{
			return m_id == id.m_id &&
				m_key_code == id.m_key_code &&
				m_args == id.m_args;
		}

		// Return the name of a method-handler function corresponding to this event.
		const tu_stringi&	get_function_name() const;
	};

	// allows sharing of as byte code buffer
	class counted_buffer : public membuf
	{
		mutable int	m_ref_count;

	public:
		counted_buffer()
			:
			m_ref_count(0)
		{
		}

		~counted_buffer()
		{
			assert(m_ref_count == 0);
		}

		void	add_ref() const
		{
			assert(m_ref_count >= 0);
			m_ref_count++;
		}

		void	drop_ref()
		{
			assert(m_ref_count > 0);
			m_ref_count--;
			if (m_ref_count == 0)
			{
				// Delete me!
				swfdelete (this);
			}
		}

		array<const tu_string*> m_dictionary;
	};

	class smart_ptr_counted_buffer
	{
	public:
		smart_ptr_counted_buffer(counted_buffer* ptr)
			:
			m_ptr(ptr)
		{
			if (m_ptr)
			{
				m_ptr->add_ref();
			}
		}

		smart_ptr_counted_buffer() : m_ptr(NULL) {}
		smart_ptr_counted_buffer(const smart_ptr_counted_buffer& s)
			:
			m_ptr(s.m_ptr)
		{
			if (m_ptr)
			{
				m_ptr->add_ref();
			}
		}

		~smart_ptr_counted_buffer()
		{
			if (m_ptr)
			{
				m_ptr->drop_ref();
			}
		}

	//	operator bool() const { return m_ptr != NULL; }
		void	operator=(const smart_ptr_counted_buffer& s) { set_ref(s.m_ptr); }
		void	operator=(counted_buffer* ptr) { set_ref(ptr); }
	//	void	operator=(const weak_ptr<T>& w);
		counted_buffer*	operator->() const { assert(m_ptr); return m_ptr; }
		counted_buffer*	get_ptr() const { return m_ptr; }
		bool	operator==(const smart_ptr_counted_buffer& p) const { return m_ptr == p.m_ptr; }
		bool	operator!=(const smart_ptr_counted_buffer& p) const { return m_ptr != p.m_ptr; }
		bool	operator==(counted_buffer* p) const { return m_ptr == p; }
		bool	operator!=(counted_buffer* p) const { return m_ptr != p; }

		// Provide work-alikes for static_cast, dynamic_cast, implicit up-cast?  ("gentle_cast" a la ajb?)

	private:
		void	set_ref(counted_buffer* ptr)
		{
			if (ptr != m_ptr)
			{
				if (m_ptr)
				{
					m_ptr->drop_ref();
				}
				m_ptr = ptr;

				if (m_ptr)
				{
					m_ptr->add_ref();
				}
			}
		}

	//	friend weak_ptr;

		counted_buffer*	m_ptr;
	};

	// Base class for actions.
	struct action_buffer
	{
		action_buffer();
		void	read(stream* in);
		void	execute(as_environment* env);
		void	execute(
			as_environment* env,
			int start_pc,
			int exec_bytes,
			as_value* retval,
			const array<with_stack_entry>& initial_with_stack,
			bool is_function2) const;

		static as_object* load_as_plugin(player* player,
			const tu_string& classname, const array<as_value>& params);
		int	get_length() const { return m_buffer->size(); }
		int  get_stream_position() const { return m_stream_position; }
		void set_stream_position(int pos) { m_stream_position = pos; }
		void operator=(const action_buffer& ab);
		
#if ACTION_BUFFER_PROFILLING
		static void log_and_reset_profiling( void );
#endif

	private:
		// Don't put these as values in array<>!  They contain
		// internal pointers and cannot be moved or copied.
		// If you need to keep an array of them, keep pointers
		// to swfnew'd instances.
		//action_buffer(const action_buffer& a) { assert(0); }

		void	process_decl_dict(int start_pc, int stop_pc, player* player);
		static void	enumerate(as_environment* env, as_object* object);

		// data:
		smart_ptr_counted_buffer	m_buffer;
		int	m_decl_dict_processed_at;
		int m_stream_position;
#if ACTION_BUFFER_PROFILLING		
		static hash<int, Uint64> profiling_table;
#endif
	};


	struct fn_call;

	struct as_property_interface
	{
		virtual ~as_property_interface() {}
		virtual bool	set_property(int index, const as_value& val) = 0;
	};

	//
	// Some handy helpers
	//

	// tulrich: don't use this!  To register a class constructor,
	// just assign the classname to the constructor function.  E.g.:
	//
	// my_movie->set_member("MyClass", as_value(MyClassConstructorFunction));
	// 
	//void register_as_object(const char* object_name, as_c_function_ptr handler);

	// Numerical indices for standard member names.	 Can use this
	// to help speed up get/set member calls, by using a switch()
	// instead of nasty string compares.
	enum as_standard_member
	{
		M_INVALID_MEMBER = -1,
		M_X,
		M_Y,
		M_XSCALE,
		M_YSCALE,
		M_CURRENTFRAME,
		M_TOTALFRAMES,
		M_ALPHA,
		M_VISIBLE,
		M_WIDTH,
		M_HEIGHT,
		M_ROTATION,
		M_TARGET,
		M_FRAMESLOADED,
		M_NAME,
		M_DROPTARGET,
		M_URL,
		M_HIGHQUALITY,
		M_FOCUSRECT,
		M_SOUNDBUFTIME,
		M_XMOUSE,
		M_YMOUSE,
		M_PARENT,
		M_TEXT,
		M_HTMLTEXT,
		M_TEXTWIDTH,
		M_TEXTHEIGHT,
		M_TEXTCOLOR,
		M_BORDER,
		M_MULTILINE,
		M_WORDWRAP,
		M_TYPE,
		M_BACKGROUNDCOLOR,
		M_THIS,
		MTHIS,
		M_ROOT,
		MDOT,
		MDOT2,
		M_LEVEL0,
		M_GLOBAL,
		M_STAGE,
		M_ENABLED,
		M_TRANSFORM,
		M_MATRIX,
		M_WORLDMATRIX,
		M_COLORTRANSFORM,
		M_WORLDCOLORTRANSFORM,

		AS_STANDARD_MEMBER_COUNT
	};
	// Return the standard enum, if the arg names a standard member.
	// Returns M_INVALID_MEMBER if there's no match.
	as_standard_member	get_standard_member(const tu_stringi& name);

	enum builtin_object
	{
		BUILTIN_OBJECT_METHOD,
		BUILTIN_SPRITE_METHOD,
		BUILTIN_NUMBER_METHOD,
		BUILTIN_BOOLEAN_METHOD,
		BUILTIN_STRING_METHOD,
		BUILTIN_S_FUNCTION_METHOD,
		BUILTIN_EDIT_TEXT_METHOD,
		BUILTIN_ARRAY_METHOD,

		// and so far

		BUILTIN_COUNT
	};

	bool get_builtin(builtin_object id, const tu_stringi& name, as_value* val);
	stringi_hash<as_value>* new_standard_method_map(builtin_object id);

}	// end namespace gameswf


#endif // GAMESWF_ACTION_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End: