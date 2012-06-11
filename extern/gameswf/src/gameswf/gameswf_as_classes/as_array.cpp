#include "stdafx.h"
// array.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003, Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "gameswf/gameswf_as_classes/as_array.h"
#include "gameswf/gameswf_function.h"
#include <algorithm>

namespace gameswf
{
	as_object* array_init(player* player)
	{
		as_object* obj = swfnew as_c_function(player, as_global_array_ctor);

		obj->builtin_member("CASEINSENSITIVE", as_array::CASEINSENSITIVE);
		obj->builtin_member("DESCENDING", as_array::DESCENDING);
		obj->builtin_member("UNIQUESORT", as_array::UNIQUESORT);
		obj->builtin_member("RETURNINDEXEDARRAY", as_array::RETURNINDEXEDARRAY);
		obj->builtin_member("NUMERIC", as_array::NUMERIC);

		return obj;
	}

	void	as_array_tostring(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		assert(a);

		fn.result->set_tu_string(a->to_string());
	}

	void	as_array_push(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		assert(a);

		for(int i = 0; i < fn.nargs; i++)
		{
			a->push(fn.arg(i));
		}
		fn.result->set_int(a->size());
	}

	void	as_array_pop(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		assert(a);
	
		as_value val;
		a->pop(&val);
		*fn.result = val;
	}

	// Concatenates the elements specified in the parameters with the elements in an array and creates a new array. 
	// If the value parameters specify an array, the elements of that array are concatenated, rather than the array itself.
	// The array my_array is left unchanged.
	// public concat([value:Object]) : Array
	void	as_array_concat(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		assert(a);

		// create new array
		smart_ptr<as_array>	ao = swfnew as_array(fn.get_player());

		// copy original elements
		int size = a->size();
		for(int i = 0; i < size; i++)
		{
			ao->push(a->get(i));
		}

		// concat array(s) in argument
		for(int k = 0; k < fn.nargs; k++)
		{
			as_array* value = cast_to<as_array>(fn.arg(k).to_object());
			if(value)
			{
				int size = value->size();
				for(int i = 0; i < size; i++)
				{
					ao->push(value->get(i));
				}
			}
		}

		fn.result->set_as_object(ao.get_ptr());
	}

	// Converts the elements in an array to strings, inserts the specified separator between the elements, concatenates them, and returns the resulting string.
	// A nested array is always separated by a comma (,), not by the separator passed to the join() method.
	// public join([delimiter:String]) : String
	void	as_array_join(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		assert(a);

		tu_string sep = ",";
		if(fn.nargs >= 1)
		{
			sep = fn.arg(0).to_tu_string();
		}

		// get elements
		tu_string str;
		int size = a->size();
		for(int i = 0; i < size; i++)
		{
			str += a->get(i).to_tu_string();

			if(i < size - 1)
				str += sep;
		}

		fn.result->set_tu_string(str);
	}

	// Reverses the array in place.
	// public reverse() : Void
	void	as_array_reverse(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		assert(a);

		as_value temp;
		const int size = a->size();
		const int half = size/2;
		for(int i = 0; i < half; i++)
		{
			const int index0 = i;
			const int index1 = size - 1 - i;

			// swap values
			temp = a->m_values[index0];
			a->m_values[index0] = a->m_values[index1];
			a->m_values[index1] = temp;
		}
	}

	// Removes the first element from an array and returns that element.
	// public shift() : Object
	void	as_array_shift(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		assert(a);
		
		// get front value
		as_value value;
		a->get_member(0, &value);
		a->remove(0);

		*fn.result = value;
	}

	// Adds one or more elements to the beginning of an array and returns the new length of the array.
	// public unshift(value:Object) : Number
	void	as_array_unshift(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		assert(a);

		// keep original values
		array<as_value> original_values = a->m_values;
		a->m_values.clear();

		// insert new values at front
		for(int i = 0; i < fn.nargs; i++)
		{
			a->push(fn.arg(i));
		}

		// insert original values
		for(int i = 0; i < original_values.size(); i++)
		{
			a->push(original_values[i]);
		}

		fn.result->set_int(a->size());
	}

	// custom sorter
	struct custom_array_sorter
	{
		custom_array_sorter(as_value& function, as_environment& env) :
			m_function(function),
			m_env(env)
		{
		}

		// Return true if [a] < [b].
		bool operator()(const as_value& a, const as_value& b)
		{
			m_env.push(a);
			m_env.push(b);

			as_value result = call_method(
								m_function,
								&m_env,
								m_function,
								2,
								m_env.get_top_index());

			m_env.drop(2);

			return result.to_int() == 1;
		}

		as_value& m_function;
		as_environment& m_env;
	};

	struct standard_array_sorter
	{
		standard_array_sorter(int options) :
			m_options(options)
		{
		}

		// Return true if [a] < [b].
		bool operator()(const as_value& a, const as_value& b)
		{
			bool result;
			if(m_options & as_array::NUMERIC)
			{
				result = a.to_number() < b.to_number();
			}
			else
			{
				// case insensitive?
				if(m_options & as_array::CASEINSENSITIVE)
				{
					if(a.to_tu_stringi() == b.to_tu_stringi())
					{
						if(a.to_tu_string() == b.to_tu_string())
							result = &a < &b;
						else
							result = a.to_tu_string() < b.to_tu_string();
					}
					else
						result = a.to_tu_stringi() < b.to_tu_stringi();
				}
				else
				{
					if(a.to_tu_string() == b.to_tu_string())
						result = &a < &b;
					else
						result = a.to_tu_string() < b.to_tu_string();
				}
			}

			return (m_options & as_array::DESCENDING) ? !result : result;
		}

		as_value m_function;
		int m_options;
	};

	// Sorts the elements in an array. Flash sorts according to Unicode values. 
	// public sort([compareFunction:Object], [options:Number]) : Array
	void	as_array_sort(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		assert(a);

		int arg_index = 0;
		as_value compare_function;
		if(fn.nargs >= 1 && fn.arg(arg_index).is_function())
		{
			// if 1st parameter not a function, ignore it for now
			compare_function = fn.arg(arg_index++);
		}

		int options = 0;
		if(arg_index < fn.nargs)
		{
			options = fn.arg(arg_index++).to_int();
		}

		// check if need to use custom comparator or standard
		array<as_value>& values = a->m_values;
		if(values.size() > 0)
		{
			if(compare_function.is_undefined())
			{
				standard_array_sorter sorter(options);
				std::sort(&values[0], &values[0] + values.size(), sorter);
			}
			else
			{
				custom_array_sorter sorter(compare_function, *fn.env);
				std::sort(&values[0], &values[0] + values.size(), sorter);
			}
		}

		// create new array for result?
		//smart_ptr<as_array>	ao;
		//if(options & as_array::RETURNINDEXEDARRAY)
		//{
		//	// Flash returns an array that reflects the results of the sort and does not modify the array.
		//	ao = swfnew as_array(fn.get_player());
		//	a = ao.get_ptr();
		//}

		fn.result->set_as_object(a);
	}

	// Returns a new array that consists of a range of elements from the original array, without modifying the original array.
	// The returned array includes the startIndex element and all elements up to, but not including, the endIndex element.
	// public slice([startIndex:Number], [endIndex:Number]) : Array
	void	as_array_slice(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		assert(a);
		
		int size = a->size();
		int startIndex = 0;
		if(fn.nargs >= 1)
		{
			startIndex = fn.arg(0).to_int();
			if(startIndex < 0)
				startIndex += size;
		}

		int endIndex = size;
		if(fn.nargs >= 2)
		{
			endIndex = fn.arg(1).to_int();
			if(endIndex < 0)
				endIndex += size;
		}

		startIndex = iclamp(startIndex, 0, size);
		endIndex = iclamp(endIndex, 0, size);

		// create new array with sub elements
		smart_ptr<as_array>	ao = swfnew as_array(fn.get_player());
		for(int i = startIndex; i < endIndex; i++)
		{
			ao->push(a->get(i));
		}

		fn.result->set_as_object(ao.get_ptr());
	}

	// Adds elements to and removes elements from an array. This method modifies the array without making a copy.
	// public splice(startIndex:Number, [deleteCount:Number], [value:Object]) : Array
	void	as_array_splice(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		assert(a);

		int size = a->size();
		int startIndex = 0;
		if(fn.nargs >= 1)
		{
			startIndex = fn.arg(0).to_int();
			if(startIndex < 0)
				startIndex += size;
		}

		int deleteCount = size - startIndex;
		if(fn.nargs >= 2)
		{
			deleteCount = fn.arg(1).to_int();
		}
		int endIndex = startIndex + deleteCount;

		as_value insert_value;
		if(fn.nargs >= 3)
		{
			insert_value = fn.arg(2);
		}

		startIndex = iclamp(startIndex, 0, size);
		endIndex = iclamp(endIndex, 0, size);

		// create new array with sub elements, and modify original array
		smart_ptr<as_array>	ao = swfnew as_array(fn.get_player());
		array<as_value>	keep_values;
		for(int i = 0; i < size; i++)
		{
			as_value val = a->get(i);

			if(i == startIndex && !insert_value.is_undefined())
			{
				// insert new value in original array
				keep_values.push_back(insert_value);
			}

			if(i >= startIndex && i < endIndex)
			{
				// put in new array
				ao->push(val);
			}
			else
			{
				// keep in original array
				keep_values.push_back(val);
			}
		}

		a->m_values = keep_values;
		fn.result->set_as_object(ao.get_ptr());
	}

	void	as_array_length(const fn_call& fn)
	{
		as_array* a = cast_to<as_array>(fn.this_ptr);
		assert(a);
		fn.result->set_int(a->size());
	}

	void	as_global_array_ctor(const fn_call& fn)
	// Constructor for ActionScript class Array.
	{
		smart_ptr<as_array>	ao = swfnew as_array(fn.get_player());

		// case of "var x = ["abc","def", 1,2,3,4,..];"
		// called from "init array" operation only
		if (fn.nargs == -1 && fn.first_arg_bottom_index == -1)
		{

			// Use the arguments as initializers.
			int	size = fn.env->pop().to_int();
			for (int i = 0; i < size; i++)
			{
				ao->push(fn.env->pop());
			}
		}
		else

		// case of "var x = swfnew Array(777)"
		if (fn.nargs == 1)
		{
			// Create an empty array with the given number of undefined elements.
			int size = fn.arg(0).to_int();
			for (int i = 0; i < size; i++)
			{
				ao->push(as_value());
			}
		}
		else

		// case of "var x = swfnew Array(1,2,3,4,5,6,7,8,..);"
		{
			assert(fn.env);

			// Use the arguments as initializers.
			for (int i = 0; i < fn.nargs; i++)
			{
				ao->push(fn.arg(i));
			}
		}

		fn.result->set_as_object(ao.get_ptr());
	}


	as_array::as_array(player* player) :
		as_object(player)
	{
		//this->set_member("sortOn", &array_not_impl);
		//builtin_member("toString", as_array_tostring);
		//builtin_member("push", as_array_push);
		//builtin_member("pop", as_array_pop);
		//builtin_member("length", as_value(as_array_length, as_value()));
		set_ctor(as_global_array_ctor);
	}

	const char* as_array::to_string()
	{
		// form string
		m_string_value = "";
		for (int i = 0, n = m_values.size(); i < n; i++)
		{
			as_value val;
			m_string_value += m_values[i].to_string();

			if (i < n - 1)
			{
				m_string_value +=  ",";
			}
		}

		return m_string_value.c_str();
	}

	bool as_array::get_member(const tu_stringi& name, as_value* val)
	{
		// try built-ins array methods
		if (get_builtin(BUILTIN_ARRAY_METHOD, name, val))
		{
			if (val->is_property())
			{
				val->set_property_target(this);
			}
			return true;
		}
		
		if(as_object::get_member(name, val))
		{
			return true;
		}

		if(is_index(name) && get_member(atoi(name.c_str()), val))
		{
			return true;
		}

		return false;
	}

	bool as_array::set_member(const tu_stringi& name, const as_value& val)
	{
		if(is_index(name) && set_member(atoi(name.c_str()), val))
		{
			return true;
		}

		return as_object::set_member(name, val);
	}

	bool as_array::get_member(int index, as_value* val)
	{
		if(index >= 0 && index < m_values.size())
		{
			*val = m_values[index];
			return true;
		}

		return false;
	}
	
	bool as_array::set_member(int index, const as_value& val)
	{
		if(index >= 0 && index < m_values.size())
		{
			m_values[index] = val;
			return true;
		}
		else if(index >= 0)
		{
			// stretch the array with undefined values (ensure it's not too large)
			assert(index < 64000);
			m_values.resize(index + 1);
			m_values[index] = val;
			return true;
		}

		return false;
	}

	void as_array::enumerate(as_environment* env)
	{
		assert(env);

		// enumerate variables
		as_object::enumerate(env);

		// enumerate indexes
		for (int i = 0, n = m_values.size(); i < n; i++)
		{
			env->push(i);
		}
	}

	void as_array::this_alive()
	{
		if(!m_player->is_garbage(this))
			return;

		as_object::this_alive();

		// mark display list as useful
		for (int i = 0, n = m_values.size(); i < n; i++)
		{
			as_object* obj = m_values[i].to_object();
			if(obj && m_player->is_garbage(obj))
				obj->this_alive();
		}
	}

	void as_array::clear_refs(hash<as_object*, bool>* visited_objects, as_object* this_ptr)
	{
		// Is it a reentrance ?
		if (visited_objects->get(this, NULL))
		{
			return;
		}

		as_object::clear_refs(visited_objects, this_ptr);

		// clear values
		for(int i = 0; i < m_values.size(); i++)
		{
			as_object* obj = m_values[i].to_object();
			if(obj)
				obj->clear_refs(visited_objects, this_ptr);
		}
	}

	void as_array::push(const as_value& val)
	// Insert the given value at the end of the array.
	{
		m_values.push_back(val);
	}

	void as_array::pop(as_value* val)
	// Removes the last element from an array and returns the value of that element.
	{
		assert(val);
		*val = m_values.back();
		m_values.pop_back();
	}

	bool as_array::is_index(const tu_stringi& str)
	{
		const char* ptr = str.c_str();
		while(*ptr)
		{
			if(*ptr < '0' || *ptr > '9')
				return false;
			ptr++;
		}

		return true;
	}
};
