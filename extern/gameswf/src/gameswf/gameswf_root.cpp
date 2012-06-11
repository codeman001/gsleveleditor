#include "stdafx.h"
// gameswf_root.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation for SWF player.

// Useful links:
//
// http://sswf.sourceforge.net/SWFalexref.html
// http://www.openswf.org

#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_render.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_debugger.h"
#include "base/tu_random.h"
#include "base/tu_timer.h"

#ifdef _GLITCH
#include <glitch/glitch.h>
#include <glitch/debugger/CDebugger.h>
#endif

namespace gameswf
{

	static tu_mutex s_gameswf_engine;
	tu_mutex& gameswf_engine_mutex()
	{
		return s_gameswf_engine;
	}

	root::root(player* player, movie_def_impl* def)	:
		m_def(def),
		m_movie(NULL),
		m_viewport_x0(0),
		m_viewport_y0(0),
		m_viewport_width(1),
		m_viewport_height(1),
		m_pixel_scale(1.0f),
		m_background_color(0, 0, 0, 255),
		m_mouse_x(0),
		m_mouse_y(0),
		m_mouse_buttons(0),
		m_active_controller(0),
		m_userdata(NULL),
		m_on_event_load_called(false),
		m_text_buffering_enabled(false),
		m_text_buffering_render(false),
		m_current_active_entity(NULL),

		// the first time we needs do advance() and
		// then display() certainly
		m_time_remainder(1.0f),

		m_gc_remainder(0.0f),

		m_frame_time(1.0f),
		m_player(player)
	{
		assert(m_def != NULL);
		set_display_viewport(0, 0, (int) m_def->get_width_pixels(), (int) m_def->get_height_pixels());
		m_frame_time = 1.0f / get_frame_rate();
		player->set_root(this);
	}

	root::~root()
	{
		assert(m_movie != NULL);
		m_movie = NULL;

		assert(m_def != NULL);
		m_def = NULL;
	}

	void root::set_active_entity(character* ch)
	{
		smart_ptr<character>	active_entity = m_current_active_entity;

		// set/kill focus
		// It's another entity ?
		if (m_current_active_entity != active_entity)
		{
			// First to clean focus
			if (m_current_active_entity != NULL)
			{
				m_current_active_entity->on_event(event_id::KILLFOCUS);
				m_current_active_entity = NULL;
			}

			// Then to set focus
			if (active_entity != NULL)
			{
				if (active_entity->on_event(event_id::SETFOCUS))
				{
					m_current_active_entity = active_entity;
				}
			}
		}

		m_current_active_entity = ch;
	}

	void	root::generate_mouse_button_events(mouse_button_state* ms)
	{
		smart_ptr<character>	active_entity = ms->m_active_entity;
		smart_ptr<character>	topmost_entity = ms->m_topmost_entity;

		// set cursor as passive/active object
		if (active_entity != NULL)
		{
			if (active_entity->can_handle_mouse_event())
			{
				render::set_cursor(render_handler::ACTIVE_CURSOR);
			}
			else
			{
				render::set_cursor(render_handler::SYSTEM_CURSOR);
			}
		}
		else
		{
			render::set_cursor(render_handler::SYSTEM_CURSOR);
		}

		if (ms->m_mouse_button_state_last == 1)
		{
			// Mouse button was down.

			// Handle trackAsMenu dragOver
			if (active_entity == NULL
				|| active_entity->get_track_as_menu())
			{
				if (topmost_entity != NULL
					&& topmost_entity != active_entity
					&& topmost_entity->get_track_as_menu() == true)
				{
					// Transfer to topmost entity, dragOver
					active_entity = topmost_entity;
					active_entity->on_event(event_id::DRAG_OVER);
					ms->m_mouse_inside_entity_last = true;
				}
			}

			// Handle onDragOut, onDragOver
			if (ms->m_mouse_inside_entity_last == false)
			{
				if (topmost_entity == active_entity)
				{
					// onDragOver
					if (active_entity != NULL)
					{
						active_entity->on_event(event_id::DRAG_OVER);
					}
					ms->m_mouse_inside_entity_last = true;
				}
			}
			else
			{
				// mouse_inside_entity_last == true
				if (topmost_entity != active_entity)
				{
					// onDragOut
					if (active_entity != NULL)
					{
						active_entity->on_event(event_id::DRAG_OUT);
					}
					ms->m_mouse_inside_entity_last = false;
				}
			}

			// Handle onRelease, onReleaseOutside
			if (ms->m_mouse_button_state_current == 0)
			{
				// Mouse button just went up.
				ms->m_mouse_button_state_last = 0;

				if (active_entity != NULL)
				{
					if (ms->m_mouse_inside_entity_last)
					{
						// onRelease
						active_entity->on_event(event_id::RELEASE);
					}
					else
					{
						// onReleaseOutside
						if (active_entity->get_track_as_menu() == false)
						{
							active_entity->on_event(event_id::RELEASE_OUTSIDE);
						}
					}
				}
			}
		}

		if (ms->m_mouse_button_state_last == 0)
		{
			// Mouse button was up.

			// New active entity is whatever is below the mouse right now.
			if (topmost_entity != active_entity)
			{
				// onRollOut
				if (active_entity != NULL)
				{
					active_entity->on_event(event_id::ROLL_OUT);
				}

				active_entity = topmost_entity;

				// onRollOver
				if (active_entity != NULL)
				{
					active_entity->on_event(event_id::ROLL_OVER);
				}

				ms->m_mouse_inside_entity_last = true;
			}

			// mouse button press
			if (ms->m_mouse_button_state_current == 1)
			{
				// onPress

				// set/kill focus
				// It's another entity ?
				if (m_current_active_entity != active_entity)
				{
					// First to clean focus
					if (m_current_active_entity != NULL)
					{
						m_current_active_entity->on_event(event_id::KILLFOCUS);
						m_current_active_entity = NULL;
					}

					// Then to set focus
					if (active_entity != NULL)
					{
						if (active_entity->on_event(event_id::SETFOCUS))
						{
							m_current_active_entity = active_entity;
						}
					}
				}

				if (active_entity != NULL)
				{
					active_entity->on_event(event_id::PRESS);
				}
				ms->m_mouse_inside_entity_last = true;
				ms->m_mouse_button_state_last = 1;
			}
		}

		// Write the (possibly modified) smart_ptr copies back
		// into the state struct.
		ms->m_active_entity = active_entity;
		ms->m_topmost_entity = topmost_entity;
	}

	// movie info

	int	root::get_movie_version()
	{
		return m_def->get_version();
	}

	int	root::get_movie_width()
	{
		return (int) m_def->get_width_pixels();
	}
	int	root::get_movie_height()
	{
		return (int) m_def->get_height_pixels();
	}
	float	root::get_movie_fps()
	{
		return m_def->get_frame_rate();
	}

	void notify_key_object(player* player, key::code k, bool down);

	void	root::notify_key_event(player* player, key::code k, bool down)
	{
		// multithread plugins can call gameswf core therefore we should 
		// use gameswf mutex to lock gameswf engine
		gameswf_engine_mutex().lock();

		// First notify global Key object
		// listeners that uses the last keypressed code
		player->notify_key_object(k, down);

		// Notify keypress listeners.
		if (down)
		{
			m_keypress_listener.notify(event_id(event_id::KEY_PRESS, (key::code) k));
		}

		gameswf_engine_mutex().unlock();
	}

	void	root::set_root_movie(character* root_movie)
	{
		m_movie = root_movie;
		assert(m_movie != NULL);
	}

	void	root::set_display_viewport(int x0, int y0, int w, int h)
	{
		if(x0 == m_viewport_x0 && m_viewport_y0 == y0 && m_viewport_width == w && m_viewport_height == h)
			return;

		m_viewport_x0 = x0;
		m_viewport_y0 = y0;
		m_viewport_width = w;
		m_viewport_height = h;

		set_display_bounds(x0, y0, w, h, SCALE_EXACTFIT);
	}

	void	root::set_display_bounds(int x0, int y0, int w, int h, scale_mode mode)
	{
		bool normal_orientation = (get_render_handler()->get_orientation() == ORIENTATION_0) || (get_render_handler()->get_orientation() == ORIENTATION_180);
		float scale_x = normal_orientation ? w / TWIPS_TO_PIXELS(m_def->m_frame_size.width()) : w / TWIPS_TO_PIXELS(m_def->m_frame_size.height());
		float scale_y = normal_orientation ? h / TWIPS_TO_PIXELS(m_def->m_frame_size.height()) : h / TWIPS_TO_PIXELS(m_def->m_frame_size.width());
		float ratio = scale_y / scale_x;

		switch(mode)
		{
		case SCALE_NOBORDER:
			if(ratio >= 1.0f)
			{
				// maximize height
				int adjust = (int)(w * ratio) - w;
				x0 -= adjust/2;
				w += adjust;
			}
			else
			{
				// maximize width
				int adjust = (int)(h / ratio) - h;
				y0 -= adjust/2;
				h += adjust;
			}
			break;
		
		case SCALE_SHOWALL:
			if(ratio >= 1.0f)
			{
				// top bars
				int adjust = (int)(h / ratio) - h;
				y0 -= adjust/2;
				h += adjust;
			}
			else
			{
				// side bars
				int adjust = (int)(w * ratio) - w;
				x0 -= adjust/2;
				w += adjust;
			}
			break;
		
		case SCALE_EXACTFIT:
		default:
			// nothing to change
			break;
		}

		if(x0 == m_bound_x0 && m_bound_y0 == y0 && m_bound_width == w && m_bound_height == h)
			return;

		m_bound_x0 = x0;
		m_bound_y0 = y0;
		m_bound_width = w;
		m_bound_height = h;

		// Recompute pixel scale.
		scale_x = normal_orientation ? m_bound_width / TWIPS_TO_PIXELS(m_def->m_frame_size.width()) : m_bound_width / TWIPS_TO_PIXELS(m_def->m_frame_size.height());
		scale_y = normal_orientation ? m_bound_height / TWIPS_TO_PIXELS(m_def->m_frame_size.height()) : m_bound_height / TWIPS_TO_PIXELS(m_def->m_frame_size.width());
		m_pixel_scale = fmax(scale_x, scale_y);

		if(m_player != NULL)
		{
			point top_left(0, 0);
			point bottom_right((float)m_viewport_x0 + m_viewport_width, (float)m_viewport_y0 + m_viewport_height);
			screen_to_logical(top_left);
			screen_to_logical(bottom_right);

			// add global variable named "viewport"
			as_object* viewport = swfnew as_object(m_player.get_ptr());
			viewport->set_member("xMin", top_left.m_x);
			viewport->set_member("yMin", top_left.m_y);
			viewport->set_member("xMax", bottom_right.m_x);
			viewport->set_member("yMax", bottom_right.m_y);
			as_value val(viewport);
			m_player->m_global->set_member("Viewport", val);
		}
	}

	void	root::screen_to_logical(point& p)
	{
		orientation_mode mode = get_render_handler()->get_orientation();
		bool normal_orientation = (mode == ORIENTATION_0) || (mode == ORIENTATION_180);

		float movie_width = normal_orientation ? m_def->m_frame_size.width() : m_def->m_frame_size.height();
		float movie_height = normal_orientation ? m_def->m_frame_size.height() : m_def->m_frame_size.width();

		float scale_x = m_bound_width / TWIPS_TO_PIXELS(movie_width);
		float scale_y = m_bound_height / TWIPS_TO_PIXELS(movie_height);

		if(normal_orientation)
		{
			p.m_x = (p.m_x - m_bound_x0) / scale_x;
			p.m_y = (p.m_y - m_bound_y0) / scale_y;
		}
		else
		{
			p.m_x = (p.m_x - m_bound_y0) / scale_y;
			p.m_y = (p.m_y - m_bound_x0) / scale_x;
		}
	}

	void	root::logical_to_screen(point& p)
	{
		orientation_mode mode = get_render_handler()->get_orientation();
		bool normal_orientation = (mode == ORIENTATION_0) || (mode == ORIENTATION_180);

		float movie_width = normal_orientation ? m_def->m_frame_size.width() : m_def->m_frame_size.height();
		float movie_height = normal_orientation ? m_def->m_frame_size.height() : m_def->m_frame_size.width();

		float scale_x = (float)m_viewport_width / m_bound_width;
		float scale_y = (float)m_viewport_height / m_bound_height;

		float off_x = PIXELS_TO_TWIPS(m_bound_x0) / (m_bound_width / TWIPS_TO_PIXELS(movie_width));
		float off_y = PIXELS_TO_TWIPS(m_bound_y0) / (m_bound_height / TWIPS_TO_PIXELS(movie_height));

		if(normal_orientation)
		{
			p.m_x = (p.m_x * scale_x) - off_x;
			p.m_y = (p.m_y * scale_y) - off_y;
		}
		else
		{
			p.m_x = (p.m_x * scale_y) - off_y;
			p.m_y = (p.m_y * scale_x) - off_x;
		}
	}

	void	root::notify_mouse_state(int x, int y, int buttons)
	// The host app uses this to tell the movie where the
	// user's mouse pointer is.
	{
		m_mouse_x = x;
		m_mouse_y = y;
		m_mouse_buttons = buttons;
	}

	void	root::get_mouse_state(int* x, int* y, int* buttons)
	// Use this to retrieve the last state of the mouse, as set via
	// notify_mouse_state().  Coordinates are in PIXELS, NOT TWIPS.
	{
		assert(x);
		assert(y);
		assert(buttons);

		*x = m_mouse_x;
		*y = m_mouse_y;
		*buttons = m_mouse_buttons;
	}

	character*	root::get_root_movie() { return m_movie.get_ptr(); }

	void	root::start_drag(character* ch, bool lock_center, bool lock_bound, rect& bound)
	{
		assert(ch);

		if( m_drag_state.m_character != NULL )
		{
			stop_drag();
		}

		m_drag_state.m_character = ch;
		m_drag_state.m_lock_center = lock_center;
		m_drag_state.m_bound = lock_bound;
		m_drag_state.m_bound_x0 = bound.m_x_min;
		m_drag_state.m_bound_y0 = bound.m_y_min;
		m_drag_state.m_bound_x1 = bound.m_x_max;
		m_drag_state.m_bound_y1 = bound.m_y_max;
		m_drag_state.m_origin_set = false;
		ch->notify_need_advance();
	}

	void	root::stop_drag()
	{
		m_drag_state.m_character = NULL;
	}

	movie_definition*	root::get_movie_definition() { return m_movie->get_movie_definition(); }

	// 0-based!!
	int	root::get_current_frame() const { return m_movie->get_current_frame(); }
	float	root::get_frame_rate() const { return m_def->get_frame_rate(); }
	void	root::set_frame_rate(float rate)
	{
		if (rate >= 1 && rate <= 120)
		{
			m_frame_time = 1.0f / rate;
			m_def->set_frame_rate(rate); 
		}
	}

	float	root::get_pixel_scale() const
	// Return the size of a logical movie pixel as
	// displayed on-screen, with the current device
	// coordinates.
	{
		return m_pixel_scale;
	}

	// @@ Is this one necessary?
	character*	root::get_character(int character_id)
	{
		return m_movie->get_character(character_id);
	}

	void	root::set_background_color(const rgba& color)
	{
		m_background_color = color;
	}

	void	root::set_background_alpha(float alpha)
	{
		m_background_color.m_a = iclamp(frnd(alpha * 255.0f), 0, 255);
	}

	float	root::get_background_alpha() const
	{
		return m_background_color.m_a / 255.0f;
	}

	void	root::advance(float delta_time, bool multiple_updates)
	{
		// Lock gameswf engine. Video is running in separate thread and
		// it calls gameswf functions from separate thread to set
		// status of netstream object
		gameswf_engine_mutex().lock();

#if GAMESWF_ENABLE_DEBUGGER
		m_player->m_debugger.update();
#endif

		// Handle the mouse.
		//m_mouse_button_state.m_topmost_entity =
		//	m_movie->get_topmost_mouse_entity(PIXELS_TO_TWIPS(m_mouse_x), PIXELS_TO_TWIPS(m_mouse_y));

		//m_mouse_button_state.m_mouse_button_state_current = (m_mouse_buttons & 1);
		//generate_mouse_button_events(&m_mouse_button_state);

		// advance Action script objects
		m_advance_listener.advance(delta_time);

		m_time_remainder += delta_time;
		m_gc_remainder -= delta_time;

		if (m_time_remainder >= m_frame_time)
		{
			// this should be called infinitely to not repeat
			// the game situation after restart
			tu_random::next_random();

			if (m_on_event_load_called == false)
			{
				set_flash_vars(m_player->m_flash_vars);
			}

			while(m_time_remainder >= m_frame_time)
			{
				if (m_on_event_load_called == false)
				{
					GAMESWF_PROFILER_SCOPE_EVENT("do_init_actions");
					cast_to<sprite_instance>(m_movie.get_ptr())->do_init_actions();
					m_movie->construct();
				}

				{
					GAMESWF_PROFILER_SCOPE_EVENT("m_movie->advance");
					m_movie->advance(multiple_updates ? m_frame_time : delta_time); 
				}

				if (m_on_event_load_called == false)
				{
					// Must do loading events.  For child sprites this is
					// done by the dlist, but root movies don't get added
					// to a dlist, so we do it here.
					GAMESWF_PROFILER_SCOPE_EVENT("root onLoad");
					m_on_event_load_called = true;
					m_movie->on_event(event_id::LOAD);
				}

				// decrement remainder
				m_time_remainder -= m_frame_time;

				if(!multiple_updates)
					break;
			}
		
			if(m_gc_remainder <= 0)
			{
#ifdef GLITCH_ENABLE_DEBUGGER
				glitch::debugger::SScopeEvent dbgEvent(m_player->m_context->m_video_driver, "GarbageCollector");
#endif

				GAMESWF_PROFILER_SCOPE_EVENT("garbage collector");

				// perform a GC at a fixed delay interval (ie: 2000 ms)
				// mark all as garbage, dig in all reacheable objects, then delete unused ones
				m_player->set_as_garbage();
				m_advance_listener.alive();
				m_movie->this_alive();
				m_player->clear_garbage();

				m_gc_remainder = 2.0f;
			}

			m_time_remainder = fmod(m_time_remainder, m_frame_time);
		}

		gameswf_engine_mutex().unlock();
	}

	// 0-based!!
	void	root::goto_frame(int target_frame_number) { m_movie->goto_frame(target_frame_number); }

	bool	root::has_looped() const { return m_movie->has_looped(); }

	void	root::begin_display()
	{
		// Adjust to fit bounds
		point top_left(m_def->m_frame_size.m_x_min, m_def->m_frame_size.m_y_min);
		point bottom_right(m_def->m_frame_size.m_x_max, m_def->m_frame_size.m_y_max);
		logical_to_screen(top_left);
		logical_to_screen(bottom_right);

		gameswf::render::set_blend_mode(blend_mode::NORMAL);
		gameswf::render::set_context(this->m_player->m_context);
		gameswf::render::begin_display(
			m_background_color,
			m_viewport_x0, m_viewport_y0,
			m_viewport_width, m_viewport_height,
			top_left.m_x, bottom_right.m_x,
			top_left.m_y, bottom_right.m_y);
	}

	void	root::end_display()
	{
		gameswf::render::end_display();
		gameswf::render::set_context(NULL);
	}

	void	root::display()
	{
		if (m_movie->get_visible() == false)
		{
			// Don't display.
			return;
		}

		if(m_on_event_load_called == false)
		{
			advance(1);
		}

		begin_display();

		character* ch = m_movie.get_ptr();

#if GAMESWF_ENABLE_DEBUGGER
		bitmap_info* bitmap = m_player->m_debugger.get_texture_selection();
		if(bitmap != NULL)
		{
			rect coords;
			coords.m_x_min = coords.m_y_min = 0;
			coords.m_x_max = m_def->m_frame_size.m_x_max;
			coords.m_y_max = m_def->m_frame_size.m_y_max;
			rect uv_coords;
			uv_coords.m_x_min = uv_coords.m_y_min = 0;
			uv_coords.m_x_max = uv_coords.m_y_max = 1;
			cxform cx;
			rgba color;
			matrix mat;
			render::draw_bitmap(mat, bitmap, coords, uv_coords, color);
			
			// display nothing else
			ch = NULL;
		}
		else
		{
			character* selection = m_player->m_debugger.get_character_selection();
			if(selection != NULL)
				ch = selection;
		}
#endif

		if(ch != NULL)
			ch->display();

		flush_buffered_text();

		end_display();
	}

	void	root::flush_buffered_text()
	{
		if(m_buffered_text_dlist.size() > 0)
		{
			m_text_buffering_render = true;
			for (int i = 0; i < m_buffered_text_dlist.size(); i++)
			{
				m_buffered_text_dlist[i]->display();
			}
			m_text_buffering_render = false;
			m_buffered_text_dlist.clear();
		}
	}

	bool	root::goto_labeled_frame(const tu_stringi& label)
	{
		int	target_frame = -1;
		if (m_def->get_labeled_frame(label, &target_frame))
		{
			goto_frame(target_frame);
			return true;
		}
		else
		{
			IF_VERBOSE_ACTION(
				log_error("error: movie_impl::goto_labeled_frame('%s') unknown label\n", label.c_str() ));
			return false;
		}
	}

	void	root::set_play_state(character::play_state s) { m_movie->set_play_state(s); }
	character::play_state	root::get_play_state() const { return m_movie->get_play_state(); }

	void	root::set_variable(const char* path_to_var, const char* new_value)
	{
		m_movie->set_variable(path_to_var, new_value);
	}

	void	root::set_variable(const char* path_to_var, const wchar_t* new_value)
	{
		m_movie->set_variable(path_to_var, new_value);
	}

	const char*	root::get_variable(const char* path_to_var) const
	{
		return m_movie->get_variable(path_to_var);
	}

	// For ActionScript interfacing convenience.
	const char*	root::call_method(const char* method_name, const char* method_arg_fmt, ...)
	{
		assert(m_movie != NULL);

		va_list	args;
		va_start(args, method_arg_fmt);
		const char*	result = m_movie->call_method_args(method_name, method_arg_fmt, args);
		va_end(args);

		return result;
	}

	const char*	root::call_method_args(const char* method_name, const char* method_arg_fmt, va_list args)
	{
		assert(m_movie != NULL);
		return m_movie->call_method_args(method_name, method_arg_fmt, args);
	}
	
	tu_string root::call_method(const char* method_name, as_value * arguments, int argument_count )
	{
		assert(m_movie != NULL);
		return m_movie->call_method(method_name, arguments, argument_count);
	}

	void	root::set_visible(bool visible) { m_movie->set_visible(visible); }
	bool	root::get_visible() const { return m_movie->get_visible(); }

	void* root::get_userdata() { return m_userdata; }
	void root::set_userdata(void * ud ) { m_userdata = ud;  }

	void	root::attach_display_callback(const char* path_to_object, void (*callback)(void* user_ptr), void* user_ptr)
	{
		m_movie->attach_display_callback(path_to_object, callback, user_ptr);
	}

	// parse Flash vars and create _root variables
	// Flash vars come in the format:
	// myvar1=value1,myvar2=value2,myvar3=value3,...
	void root::set_flash_vars(const tu_string& vars)
	{
		for (const char* word = vars.c_str(); *word; )
		{
			const char* delimiter = strchr(word, '=');
			if (delimiter == NULL)
			{
				// no value
				return;
			}
			tu_string varname(word, int(delimiter - word));

			word = delimiter + 1;
			delimiter = strchr(word, ',');
			if (delimiter == NULL)
			{
				delimiter = vars.c_str() + vars.size();
			}
			tu_string value(word, int(delimiter - word));
		
			get_root_movie()->set_member(varname, value.c_str());
			word = delimiter + 1;
		}

	}

}
