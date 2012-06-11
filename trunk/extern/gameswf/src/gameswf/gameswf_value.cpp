#include "stdafx.h"
// gameswf_value.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// ActionScript value type.

#include "gameswf/gameswf_value.h"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_action.h"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_as_classes/as_number.h"
#include "gameswf/gameswf_as_classes/as_boolean.h"
#include "gameswf/gameswf_as_classes/as_string.h"
#include <float.h>

namespace gameswf
{
	const char*	as_value::to_string() const
	// Conversion to string.
	{
		return to_tu_string().c_str();
	}

	const char*	as_value::to_xstring() const
	// Conversion to string, for debuging
	{
		static char buf[16];
		if (m_type == OBJECT)
		{
			snprintf(buf, 16, "0x%p", m_object);
			return buf;
		}
		return to_tu_string().c_str();
	}

	const tu_string&	as_value::to_string(tu_string& buffer) const
	// Conversion to const tu_string&.
	{
		switch (m_type)
		{
			case STRING:
				assert(m_string);
				return *m_string;

			case STRING_BUFFER:
				assert(m_string_buffer);
				return *m_string_buffer;

			case UNDEFINED:
			{
				// Behavior depends on file version.  In
				// version 7+, it's "undefined", in versions
				// 6-, it's "".
				int version = 8; //vv fixme: get_root()->get_movie_version();
				if (version <= 6)
				{
					buffer = "";
				}
				else
				{
					buffer = "undefined";
				}
				break;
			}

			case BOOLEAN:
				buffer = m_bool ? "true" : "false";
				break;

			case NUMBER:
				// @@ Moock says if value is a NAN, then result is "NaN"
				// INF goes to "Infinity"
				// -INF goes to "-Infinity"
				{
					using namespace std;
					if (isnan(m_number))
					{
						buffer = "NaN";
					} 
					else
					{
						char str[350];
						snprintf(str, 50, "%.14g", (double)m_number);
						//gcvt(m_number, 14, str);
						//int len = strlen(str);
						//if(len > 0 && str[len-1] == '.')
						//	str[len-1] = '\0';
						buffer = str;
					}
				}	
				break;

			case OBJECT:
				// Moock says, "the value that results from
				// calling toString() on the object".
				//
				// The default toString() returns "[object
				// Object]" but may be customized.
				if (m_object == NULL)
				{
					buffer = "null";
				}
				else
				{
					buffer = m_object->to_string();
				}
				break;
	
			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				tu_string temp;
				buffer = val.to_string(temp);
				break;
			}

			default:
				buffer = "";
				assert(0);
				break;
		}

		//printf("to_string '%s'\n", buffer.c_str());
		return buffer;
	}

	double	as_value::to_number() const
	// Conversion to double.
	{
		switch (m_type)
		{
			case STRING:
			{
				// @@ Moock says the rule here is: if the
				// string is a valid float literal, then it
				// gets converted; otherwise it is set to NaN.
				//
				// Also, "Infinity", "-Infinity", and "NaN"
				// are recognized.
				double val;
				if (! string_to_number(&val, m_string->c_str()))
				{
					// Failed conversion to Number.
					val = get_nan();
				}
				return val;
			}

			case STRING_BUFFER:
			{
				double val;
				if (! string_to_number(&val, m_string_buffer->c_str()))
				{
					// Failed conversion to Number.
					val = get_nan();
				}
				return val;
			}

			case NUMBER:
				return m_number;

			case BOOLEAN:
				return m_bool ? 1 : 0;

			case OBJECT:
				if (m_object)
				{
					return m_object->to_number();
				}
				return get_nan();
	
			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				return val.to_number();
			}

			case UNDEFINED:
			{
				return get_nan();
			}

			default:
				return 0.0;
		}
	}


	bool	as_value::to_bool() const
	// Conversion to boolean.
	{
		switch (m_type)
		{
			case STRING:
				// From Moock
//vv fixme:				if (get_root()->get_movie_version() >= 7)
				{
					return m_string->size() > 0 ? true : false;
				}

				if (*m_string == "false")
				{
					return false;
				}
				else
				if (*m_string == "true")
				{
					return true;
				}
				else
				{
					// @@ Moock: "true if the string can
					// be converted to a valid nonzero
					// number".
					//
					// Empty string --> false
					return to_number() != 0.0;
				}

			case STRING_BUFFER:
				return m_string_buffer->size() > 0 ? true : false;

			case OBJECT:
				if (m_object)
				{
					return m_object->to_bool();
				}
				return false;

			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				return val.to_bool();
			}

			case NUMBER:
				return m_number != 0;

			case BOOLEAN:
				return m_bool;

			case UNDEFINED:
				return false;

			default:
				assert(0);
		}
		return false;
	}

	as_function*	as_value::to_function() const
	// Return value as an function.
	{
		if (m_type == OBJECT)
		{
			// OK.
			return cast_to<as_function>(m_object);
		}
		return NULL;
	}

	void	as_value::set_as_object(as_object* obj)
	{
		if (m_type != OBJECT || m_object != obj)
		{
			drop_refs();
			m_type = OBJECT;
			m_object = obj;
			if (obj)
			{
				m_object->add_ref();
			}
		}
	}

	void	as_value::operator=(const as_value& v)
	{
		m_flags = v.m_flags;
		switch (v.m_type)
		{
			case UNDEFINED:
				m_flags = 0;
				set_undefined();
				break;
			case NUMBER:
				set_double(v.m_number);
				break;
			case BOOLEAN:
				set_bool(v.m_bool);
				break;
			case STRING:
				if(v.m_shared_string != NULL)
					set_string(v.m_shared_string);
				else
					set_tu_string(*v.m_string);
				break;
			case STRING_BUFFER:
				assert(v.m_string_buffer);
				set_tu_string(*v.m_string_buffer);
				break;
			case OBJECT:
				set_as_object(v.m_object);
				break;

			case PROPERTY:
				drop_refs(); 
				
				// is binded property ?
				if (v.m_property_target == NULL)
				{
					m_type = PROPERTY;
					m_property = v.m_property;
					m_property->add_ref();
					m_property_target = NULL;
				}
				else
				{
					v.get_property(this);
				}

				break;

			default:
				assert(0);
		}
	}

	bool	as_value::operator==(const as_value& v) const
	// Return true if operands are equal.
	{
		// types don't match
		//if (!is_string() && !v.is_string() && m_type != PROPERTY && v.m_type != PROPERTY && m_type != v.m_type)
		//{
		//	return false;
		//}

		switch (m_type)
		{
			case UNDEFINED:
				return v.is_undefined() || v.is_null();

			case STRING:
				switch(v.m_type)
				{
				case BOOLEAN:		return to_number() == v.to_number();
				case NUMBER:		return to_number() == v.m_number;
				case STRING:		return *m_string == *v.m_string;
				case STRING_BUFFER:	return *m_string == *v.m_string_buffer;
				default:			return false;
				}

			case STRING_BUFFER:
				switch(v.m_type)
				{
				case BOOLEAN:		return to_number() == v.to_number();
				case NUMBER:		return to_number() == v.m_number;
				case STRING:		return *m_string_buffer == *v.m_string;
				case STRING_BUFFER:	return *m_string_buffer == *v.m_string_buffer;
				default:			return false;
				}

			case NUMBER:
				switch(v.m_type)
				{
				case BOOLEAN:		return m_number == v.to_number();
				case NUMBER:		return m_number == v.m_number;
				case STRING:		return m_number == v.to_number();
				case STRING_BUFFER:	return m_number == v.to_number();
				default:			return false;
				}

			case BOOLEAN:
				switch(v.m_type)
				{
				case BOOLEAN:		return m_bool == v.m_bool;
				case NUMBER:		return m_bool == v.to_bool();
				case STRING:		return v.to_number() == to_number();
				case STRING_BUFFER:	return v.to_number() == to_number();
				default:			return false;
				}

			case OBJECT:
				return (v.is_object() && m_object == v.to_object()) || (is_null() && v.is_undefined());

			case PROPERTY:
			{
				as_value prop;
				get_property(&prop);
				return prop == v;
			}

			default:
				assert(0);
				return false;
		}
	}

	
	bool	as_value::operator!=(const as_value& v) const
	// Return true if operands are not equal.
	{
		return ! (*this == v);
	}

	void	as_value::drop_refs()
	// Drop any ref counts we have; this happens prior to changing our value.
	{
		switch(m_type)
		{
		case STRING:
			if(m_shared_string)
			{
				//printf("-as_string '%s' ref=%d\n", m_shared_string->m_string.c_str(), m_shared_string->get_ref_count() - 1);
				m_shared_string->drop_ref();
				m_shared_string = NULL;
			}
			break;
		case STRING_BUFFER:
			if(m_string_buffer)
			{
				swfdelete (m_string_buffer);
				m_string_buffer = NULL;
			}
			break;
		case OBJECT:
			if (m_object)
			{
				m_object->drop_ref();
				m_object = 0;
			}
			break;
		case PROPERTY:
			if (m_property)
			{
				m_property->drop_ref();
				m_property = NULL;
			}
			if (m_property_target)
			{
				m_property_target->drop_ref();
				m_property_target = NULL;
			}
		}
	}

	void	as_value::set_property(const as_value& val)
	{
		assert(is_property());
		m_property->set(m_property_target, val);
	}

	// get property of primitive value, like Number
	void as_value::get_property(const as_value& primitive, as_value* val) const
	{
		assert(is_property());
		m_property->get(primitive, val);
	}

	void as_value::get_property(as_value* val) const
	{
		assert(is_property());
		m_property->get(m_property_target, val);
	}

	as_property* as_value::to_property() const
	{
		if (is_property())
		{
			return m_property;
		}
		return NULL;
	}

	const as_object* as_value::get_property_target() const
	{
		if (is_property())
		{
			return m_property_target;
		}
		return NULL;
	}

	void as_value::set_property_target(as_object* target)
	// Sets the target to the given object.
	{
		assert(is_property());
		if (m_property_target)
		{
			m_property_target->drop_ref();
		}
		m_property_target = target;
		m_property_target->add_ref();
	}

	void	as_value::set_double(double val)
	{
		drop_refs(); m_type = NUMBER; m_number = val;
	}

	void	as_value::set_bool(bool val)
	{
		drop_refs(); m_type = BOOLEAN; m_bool = val;
	}


	bool as_value::is_function() const
	{
		if (m_type == OBJECT)
		{
			return cast_to<as_function>(m_object) ? true : false;
		}
		return false;
	}

	void	as_value::set_as_c_function(as_c_function_ptr func)
	{
		//vv hack, c_function object has no pointer to player instance
		set_as_object(swfnew as_c_function(NULL, func));
	}

		
	bool as_value::is_instance_of(const as_function* constructor) const
	{
		switch (m_type)
		{
			case UNDEFINED:
				break;

			case STRING:
			case STRING_BUFFER:
			{
				const as_c_function* func = cast_to<as_c_function>(constructor);
				if (func)
				{
					return 
						(func->m_func == as_global_string_ctor) |
						(func->m_func == as_global_object_ctor);
				}
				break;
			}

			case NUMBER:
			{
				const as_c_function* func = cast_to<as_c_function>(constructor);
				if (func)
				{
					return 
						(func->m_func == as_global_number_ctor) |
						(func->m_func == as_global_object_ctor);
				}
				break;
			}

			case BOOLEAN:
			{
				const as_c_function* func = cast_to<as_c_function>(constructor);
				if (func)
				{
					return 
						(func->m_func == as_global_boolean_ctor) |
						(func->m_func == as_global_object_ctor);
				}
				break;
			}

			case OBJECT:
				if (m_object)
				{
					return m_object->is_instance_of(constructor);
				}
				break;

			case PROPERTY:
			{
				//vv fixme:	
				break;
			}

			default:
				break;

		}
		return false;
	}

	const char*	as_value::_typeof() const
	{
		switch (m_type)
		{
			case UNDEFINED:
				return "undefined";

			case STRING:
			case STRING_BUFFER:
				return "string";

			case NUMBER:
				return "number";

			case BOOLEAN:
				return "boolean";

			case OBJECT:
				if (m_object)
				{
					return m_object->_typeof();
				}
				return "null";

			case PROPERTY:
			{
				as_value val;
				get_property(&val);
				return val._typeof();
			}

			default:
				assert(0);

		}
		return 0;
	}


	bool as_value::get_member( const tu_string& name, as_value* val)
	{
		switch (m_type)
		{
			default:
				break;

			case STRING:
			case STRING_BUFFER:
			{
				return get_builtin(BUILTIN_STRING_METHOD, name, val);
			}

			case NUMBER:
			{
				return get_builtin(BUILTIN_NUMBER_METHOD, name, val);
			}

			case BOOLEAN:
			{
				return get_builtin(BUILTIN_BOOLEAN_METHOD, name, val);
			}

			case OBJECT:
			{
				if (m_object)
				{
					return m_object->get_member(name.to_tu_stringi(), val);
				}
			}
		}
		return false;
	}

	void	as_value::set_tu_string(const tu_string& str)
	{
		if(m_type == STRING_BUFFER && m_string_buffer == &str)
		{
			return;
		}
		
		drop_refs();

		if(str.is_permanent())
		{
			m_type = STRING;
			m_string = &str;
			m_shared_string = NULL;
		}
		else
		{
			m_type = STRING_BUFFER;
			m_string_buffer = swfnew tu_string(str);
		}
	}
	
	void	as_value::set_string(const char* str)
	{
		if(m_type == STRING_BUFFER)
		{
			// copy string into temp string in case of substring
			assert(m_string_buffer);
			tu_string temp(str);
			*m_string_buffer = temp;
		}
		else
		{
			drop_refs();
			m_type = STRING_BUFFER;
			m_string_buffer = swfnew tu_string(str);
		}
	}

	void	as_value::set_string(as_string* str)
	{
		assert(str != NULL);
		str->add_ref();

		drop_refs();
		m_type = STRING;
		m_string = &str->m_string;
		m_shared_string = str;

		//printf("+as_string '%s' ref=%d\n", m_shared_string->m_string.c_str(), m_shared_string->get_ref_count());
	}

	as_value::as_value(const wchar_t* wstr)	:
		m_type(STRING_BUFFER),
		m_flags(0)
	{
		// Encode the string value as UTF-8.
		//
		// Is this dumb?  Alternatives:
		//
		// 1. store a tu_wstring instead of tu_string?
		// Bloats typical ASCII strings, needs a
		// tu_wstring type, and conversion back the
		// other way to interface with char[].
		// 
		// 2. store a tu_wstring as a union with
		// tu_string?  Extra complexity.
		//
		// 3. ??
		//
		// Storing UTF-8 seems like a pretty decent
		// way to do it.  Everything else just
		// continues to work.

//#if (WCHAR_MAX != MAXINT)
		set_string("");
		tu_string::encode_utf8_from_wchar(m_string_buffer, (const uint16 *)wstr);
//#else
//# if (WCHAR_MAX != MAXSHORT)
//# error "Can't determine the size of wchar_t"
//# else
//			tu_string::encode_utf8_from_wchar(&m_string, (const uint32 *)wstr);
//# endif
//#endif
	}


	//
	//	as_property
	//

	as_property::as_property(const as_value& getter,	const as_value& setter)
	{
		m_getter = cast_to<as_function>(getter.to_object());
		m_setter = cast_to<as_function>(setter.to_object());
	}

	as_property::~as_property()
	{
	}

	void	as_property::set(as_object* target, const as_value& val)
	{
		assert(target);

		as_environment env(target->get_player());
		env.push(val);
		if (m_setter != NULL)
		{
			smart_ptr<as_object> tar = target;
			(*m_setter.get_ptr())(fn_call(NULL, tar.get_ptr(),	&env, 1, env.get_top_index(), "set"));
		}
	}

	void as_property::get(as_object* target, as_value* val) const
	{
		assert(target);

		// env is used when m_getter->m_env is NULL
		as_environment env(target->get_player());
		if (m_getter != NULL)
		{
			smart_ptr<as_object> tar = target;
			(*m_getter.get_ptr())(fn_call(val, tar.get_ptr(), &env, 0,	0, "get"));
		}
	}

	// call static method
	void as_property::get(const as_value& primitive, as_value* val) const
	{
		if (m_getter != NULL)
		{
			(*m_getter.get_ptr())(fn_call(val, primitive, NULL, 0,	0, "get"));
		}
	}

}
