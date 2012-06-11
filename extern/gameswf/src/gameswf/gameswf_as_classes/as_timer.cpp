#include "stdafx.h"
// gameswf_timers.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "gameswf/gameswf_as_classes/as_timer.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_function.h"

// hack: timer_id is't a number but the as_object*

namespace gameswf 
{

	as_timer::as_timer(as_value& object_ref, as_value& func, double interval, const fn_call& fn, int param_index) :
		as_object(fn.get_player()),
		m_interval((float) interval / 1000.0f),
		m_object_ref(object_ref),
		m_func(func),
		m_delta_time(0.0f)
	{
		// get params
		for (int i = param_index; i < fn.nargs; i++)
		{
			m_param.push_back(fn.arg(i));
		}

		get_root()->m_advance_listener.add(this);
	}

	as_timer::~as_timer()
	{
//		printf("~as_timer\n");
	}

	void as_timer::advance(float delta_time)
	{
		m_delta_time += delta_time;
		if (m_delta_time >= m_interval)
		{
			m_delta_time = 0.0f;

			assert(m_func.is_function());

			as_environment env(get_player());
			int n = m_param.size();
			for (int i = 0; i < n; i++)
			{
				env.push(m_param[i]);
			}
			as_value this_ptr = m_object_ref;
			call_method(m_func, &env, this_ptr, n, env.get_top_index(), "timer");
		}
	}

	void as_timer::clear()
	{
		get_root()->m_advance_listener.remove(this);
	}

	void as_timer::clear_refs(hash<as_object*, bool>* visited_objects, as_object* this_ptr)
	{
		// Is it a reentrance ?
		if (visited_objects->get(this, NULL))
			return;

		as_object::clear_refs(visited_objects, this_ptr);

		if(m_object_ref.to_object() != NULL)
		{
			if (m_object_ref.to_object() == this_ptr)
				m_object_ref = NULL;
			else
				m_object_ref.to_object()->clear_refs(visited_objects, this_ptr);
		}

		if(m_func.to_object() != NULL)
		{
			if (m_func.to_object() == this_ptr)
				m_func = NULL;
			else
				m_func.to_object()->clear_refs(visited_objects, this_ptr);
		}

		for(int i = 0; i < m_param.size(); i++)
		{
			if(m_param[i].to_object() != NULL)
			{
				if (m_param[i].to_object() == this_ptr)
					m_param[i] = NULL;
				else
					m_param[i].to_object()->clear_refs(visited_objects, this_ptr);
			}
		}
	}
}
