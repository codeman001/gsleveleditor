#pragma once
// gameswf_timers.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#ifndef GAMESWF_AS_TIMER_H
#define GAMESWF_AS_TIMER_H

#include "gameswf/gameswf_action.h"	// for as_object

namespace gameswf 
{
	struct as_timer : public as_object
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_TIMER };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		float m_interval;	// sec
		as_value m_object_ref;
		as_value m_func;
		float m_delta_time;
		array<as_value> m_param;

		as_timer(as_value& object_ref, as_value& func, double interval, const fn_call& fn, int param_index);
		~as_timer();

		virtual void advance(float delta_time);
		virtual void clear_refs(hash<as_object*, bool>* visited_objects, as_object* this_ptr);

		void clear();
	};


}

#endif
