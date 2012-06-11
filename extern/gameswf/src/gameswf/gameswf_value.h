#pragma once
// gameswf_value.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// ActionScript value type.


#ifndef GAMESWF_VALUE_H
#define GAMESWF_VALUE_H

#include "base/container.h"
#include "base/smart_ptr.h"
#include "gameswf/gameswf.h"	// for ref_counted
#include <wchar.h>

namespace gameswf
{
	struct fn_call;
	struct as_c_function;
	struct as_function;
	struct as_object;
	struct as_environment;

	typedef void (*as_c_function_ptr)(const fn_call& fn);

	// helper, used in as_value
	struct as_property : public ref_counted
	{
		smart_ptr<as_function>	m_getter;
		smart_ptr<as_function>	m_setter;

		as_property(const as_value& getter,	const as_value& setter);
		~as_property();
	
		void	set(as_object* target, const as_value& val);
		void	get(as_object* target, as_value* val) const;
		void	get(const as_value& primitive, as_value* val) const;
	};

	struct unaligned_double
	{
		inline operator double() const
		{
			double d;
			memcpy(&d, m_number, sizeof(double));
			return d;
		}
		inline void	operator=(const double d)
		{
			memcpy(m_number, &d, sizeof(double));
		}

	private:
		int m_number[2];
	};

	struct as_value
	{
		// flags defining the level of protection of a value
		enum value_flag
		{
			DONT_ENUM = 0x01,
			DONT_DELETE = 0x02,
			READ_ONLY = 0x04
		};

		private:

		enum type
		{
			UNDEFINED,
			BOOLEAN,
			NUMBER,
			STRING,
			STRING_BUFFER,
			OBJECT,
			PROPERTY
		};

		// Numeric flags
		mutable char m_flags;
		char	m_type;

		union
		{
			struct
			{
				const tu_string*	m_string;
				as_string*			m_shared_string;
			};
			tu_string*	m_string_buffer;
			as_object*	m_object;
			unaligned_double m_number;
			bool m_bool;
			struct
			{
				as_object*	m_property_target;
				as_property* m_property;
			};
		};

	public:

		// constructors
		exported_module as_value();
		exported_module as_value(const as_value& v);
		exported_module as_value(const tu_string& str);
		exported_module as_value(const tu_string* str);
		exported_module as_value(as_string* str);
		exported_module as_value(const char* str);
		exported_module as_value(const wchar_t* wstr);
		exported_module as_value(bool val);
		exported_module as_value(int val);
		exported_module as_value(float val);
		exported_module as_value(double val);
		exported_module as_value(as_object* obj);
		exported_module as_value(as_c_function_ptr func);
		exported_module as_value(as_s_function* func);
		exported_module as_value(const as_value& getter, const as_value& setter);

		~as_value()
		{
			drop_refs();
		}

		// Useful when changing types/values.
		exported_module void	drop_refs();

		// for debuging
		exported_module const char*	to_xstring() const;

		exported_module const char*	to_string() const;
		exported_module const tu_string&	to_tu_string() const;
		exported_module const tu_stringi&	to_tu_stringi() const;
		exported_module const tu_string&	to_string(tu_string& buffer) const;
		exported_module double	to_number() const;
		exported_module int	to_int() const { return (int) to_number(); };
		exported_module float	to_float() const { return (float) to_number(); };
		exported_module bool	to_bool() const;
		exported_module as_function*	to_function() const;
		exported_module as_object*	to_object() const;
		exported_module as_property*	to_property() const;

		// These set_*()'s are more type-safe; should be used
		// in preference to generic overloaded set().  You are
		// more likely to get a warning/error if misused.
		exported_module void	set_tu_string(const tu_string& str);
		exported_module void	set_string(const char* str);
		exported_module void	set_string(as_string* str);
		exported_module void	set_double(double val);
		exported_module void	set_bool(bool val);
		exported_module void	set_int(int val) { set_double(val); }
		exported_module void	set_nan() { set_double(get_nan()); }
		exported_module void	set_as_object(as_object* obj);
		exported_module void	set_as_c_function(as_c_function_ptr func);
		exported_module void	set_undefined() { drop_refs(); m_type = UNDEFINED; }
		exported_module void	set_null() { set_as_object(NULL); }

		void	set_property(const as_value& val);
		void	get_property(as_value* val) const;
		void	get_property(const as_value& primitive, as_value* val) const;
		const as_object*	get_property_target() const;  // for debugging
		void	set_property_target(as_object* new_target);

		exported_module void	operator=(const as_value& v);
		exported_module bool	operator==(const as_value& v) const;
		exported_module bool	operator!=(const as_value& v) const;
		exported_module bool	operator<(double v) const { return to_number() < v; }
		exported_module void	operator+=(double v) { set_double(to_number() + v); }
		exported_module void	operator-=(double v) { set_double(to_number() - v); }
		exported_module void	operator*=(double v) { set_double(to_number() * v); }
		exported_module void	operator/=(double v) { set_double(to_number() / v); }  // @@ check for div/0
		exported_module void	operator&=(int v) { set_int(int(to_number()) & v); }
		exported_module void	operator|=(int v) { set_int(int(to_number()) | v); }
		exported_module void	operator^=(int v) { set_int(int(to_number()) ^ v); }
		exported_module void	shl(int v) { set_int(int(to_number()) << v); }
		exported_module void	asr(int v) { set_int(int(to_number()) >> v); }
		exported_module void	lsr(int v) { set_int((Uint32(to_number()) >> v)); }

		bool is_function() const;
		inline bool is_bool() const { return m_type == BOOLEAN; }
		inline bool is_string() const { return m_type == STRING || m_type == STRING_BUFFER; }
		inline bool is_number() const { using namespace std; return m_type == NUMBER && isnan(m_number) == false; }
		inline bool is_object() const { return m_type == OBJECT; }
		inline bool is_property() const { return m_type == PROPERTY; }
		inline bool is_null() const { return m_type == OBJECT && m_object == NULL; }
		inline bool is_undefined() const { return m_type == UNDEFINED; }

		const char*	_typeof() const;
		bool is_instance_of(const as_function* constructor) const;
		bool get_member(const tu_string& name, as_value* val);

		// flags
		inline bool is_enum() const { return m_flags & DONT_ENUM ? false : true; }
		inline bool is_readonly() const { return m_flags & READ_ONLY ? true : false; }
		inline bool is_protected() const { return m_flags & DONT_DELETE ? true : false; }
		inline int get_flags() const { return m_flags; }
		inline void set_flags(int flags) const  { m_flags = flags; }
	};

	inline as_value::as_value(as_object* obj) :
		m_type(OBJECT),
		m_object(obj),
		m_flags(0)
	{
		if (m_object)
		{
			((as_object_interface*)m_object)->add_ref();
		}
	}


	inline as_value::as_value(as_s_function* func)	:
		m_type(UNDEFINED),
		m_flags(0)
	{
		set_as_object((as_object*)func);
	}

	inline as_value::as_value(const as_value& getter, const as_value& setter) :
		m_property_target(NULL),
		m_type(PROPERTY),
		m_flags(0)
	{
		m_property = swfnew as_property(getter, setter);
		m_property->add_ref();
	}

	inline as_value::as_value(float val) :
		m_type(NUMBER),
		m_flags(0)
	{
		m_number = (double)val;
	}

	inline as_value::as_value(int val) :
		m_type(NUMBER),
		m_flags(0)
	{
		m_number = (double)val;
	}

	inline as_value::as_value(double val) :
		m_type(NUMBER),
		m_flags(0)
	{
		m_number = val;
	}

	inline as_value::as_value(bool val) :
		m_type(BOOLEAN),
		m_bool(val),
		m_flags(0)
	{
	}

	inline as_value::as_value(as_c_function_ptr func) :
		m_type(UNDEFINED),
		m_flags(0)
	{
		set_as_c_function(func);
	}

	inline as_value::as_value(const tu_string& str) :
		m_type(UNDEFINED),
		m_string_buffer(NULL),
		m_flags(0)
	{
		set_tu_string(str);
	}

	inline as_value::as_value(const tu_string* str) :
		m_type(STRING),
		m_string(str),
		m_shared_string(NULL),
		m_flags(0)
	{
		assert(str->is_permanent());
	}

	inline as_value::as_value(as_string* str) :
		m_type(STRING),
		m_string(NULL),
		m_shared_string(NULL),
		m_flags(0)
	{
		set_string(str);
	}

	inline as_value::as_value(const char* str) :
		m_type(UNDEFINED),
		m_flags(0)
	{
		set_string(str);
	}

	inline as_value::as_value() :
		m_type(UNDEFINED),
		m_flags(0)
	{
	}

	inline as_value::as_value(const as_value& v) :
		m_type(UNDEFINED),
		m_flags(0)
	{
		*this = v;
	}

	inline as_object* as_value::to_object() const
	// Return value as an object.
	{
		if (m_type == OBJECT)
		{
			// OK.
			return m_object;
		}
		return NULL;
	}

	inline const tu_string&	as_value::to_tu_string() const
	// Conversion to const tu_string&.
	{
		if(m_type == STRING)
		{
			return *m_string;
		}
		else if(m_type == STRING_BUFFER)
		{
			return *m_string_buffer;
		}
		else
		{
			static tu_string s_dummy;
			return s_dummy;
		}
	}

	inline const tu_stringi& as_value::to_tu_stringi() const
	{
		return reinterpret_cast<const tu_stringi&>(to_tu_string());
	}
}


#endif
