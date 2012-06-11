#pragma once
// array.h	-- Thatcher Ulrich <tu@tulrich.com> 2003, Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Action Script Array implementation code for the gameswf SWF player library.


#ifndef GAMESWF_AS_ARRAY_H
#define GAMESWF_AS_ARRAY_H

#include "gameswf/gameswf_action.h"	// for as_object

namespace gameswf
{
	as_object* array_init(player* player);

	void	as_array_tostring(const fn_call& fn);
	void	as_array_push(const fn_call& fn);
	void	as_array_pop(const fn_call& fn);
	void	as_array_concat(const fn_call& fn);
	void	as_array_join(const fn_call& fn);
	void	as_array_length(const fn_call& fn);
	void	as_array_reverse(const fn_call& fn);
	void	as_array_shift(const fn_call& fn);
	void	as_array_unshift(const fn_call& fn);
	void	as_array_sort(const fn_call& fn);
	void	as_array_slice(const fn_call& fn);
	void	as_array_splice(const fn_call& fn);
	void	as_global_array_ctor(const fn_call& fn);

	struct as_array : public as_object
	{
		enum sort_options
		{
			CASEINSENSITIVE = 1,
			DESCENDING = 2,
			UNIQUESORT = 4,
			RETURNINDEXEDARRAY = 8,
			NUMERIC = 16
		};

		// Unique id of a gameswf resource
		enum { m_class_id = AS_ARRAY };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		exported_module as_array(player* player);
		exported_module virtual const char* to_string();
		exported_module virtual bool get_member(const tu_stringi& name, as_value* val);
		exported_module virtual bool set_member(const tu_stringi& name, const as_value& val);
		exported_module virtual bool get_member(int index, as_value* val);
		exported_module virtual bool set_member(int index, const as_value& val);
		exported_module virtual void enumerate(as_environment* env);
		exported_module virtual void this_alive();
		exported_module virtual void clear_refs(hash<as_object*, bool>* visited_objects, as_object* this_ptr);

		exported_module void push(const as_value& val);
		exported_module void pop(as_value* val);
		exported_module int size() { return m_values.size(); }
		exported_module as_value& get(int index) { return m_values[index]; }
		exported_module void remove(int index) { m_values.remove(index); }
		exported_module bool is_index(const tu_stringi& str);

		tu_string m_string_value;
		array<as_value> m_values;
	};

}	// end namespace gameswf


#endif // GAMESWF_AS_ARRAY_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
