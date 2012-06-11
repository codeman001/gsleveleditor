#pragma once
// gameswf_sprite.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation code for the gameswf SWF player library.


#ifndef GAMESWF_SPRITE_H
#define GAMESWF_SPRITE_H


#include "gameswf/gameswf.h"
#include "gameswf/gameswf_action.h"
#include "gameswf/gameswf_as_classes/as_mcloader.h"
#include "gameswf/gameswf_as_classes/as_transform.h"
#include "gameswf/gameswf_dlist.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_sprite_def.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_canvas.h"

#include "base/container.h"
#include "base/utility.h"
#include "base/smart_ptr.h"
#include <stdarg.h>

namespace gameswf
{

	struct sprite_instance : public character
	{
		struct init_actions
		{
			array<action_buffer*> m_action_list;
			array<bool> m_executed;	// a bit-array class would be ideal for this
		};

		// Unique id of a gameswf resource
		enum { m_class_id = AS_SPRITE };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return character::is(class_id);
		}

		smart_ptr<movie_definition_sub>	m_def;
		root*	m_root;

		display_list	m_display_list;
		array<action_buffer*>	m_action_list;
		array<action_buffer*>	m_goto_frame_action_list;

		init_actions* m_init_actions;

		as_environment*	m_as_environment;

		enum mouse_state
		{
			UP = 0,
			DOWN,
			OVER
		};
		short	m_current_frame;
		char	m_play_state;
		char m_mouse_state;
		bool m_update_frame;
		bool m_has_enter_frame;
		bool m_enabled;
		bool m_constructor_called;
		bool m_on_event_load_called;
		smart_ptr<character> m_canvas;
		smart_ptr<as_transform> m_transform;

		// flash9
		hash<int, smart_ptr<as_function> >* m_script;	// <frame, script>
		smart_ptr<as_function> m_frame_script;	// current script

		sprite_instance(player* player, movie_definition_sub* def, root* r, character* parent, int id);
		virtual ~sprite_instance();

		virtual void recycle(character* ch, int id);
		virtual character_def* get_character_def() { return m_def.get_ptr();	}
		virtual bool has_keypress_event();
		root*	get_root() { return m_root; }

		// used in loadMovieClip()
		void	set_root(root* mroot) { m_root = mroot; }
		uint32	get_file_bytes() const;
		uint32	get_loaded_bytes() const;

		exported_module character*	get_root_movie() { return m_root->get_root_movie(); }
		movie_definition*	get_movie_definition() { return m_def.get_ptr(); }

		virtual void get_bound(rect* bound);

		virtual int	get_current_frame() const { return m_current_frame; }
		virtual int	get_frame_count() const { return m_def->get_frame_count(); }
		virtual int get_loading_frame() const { return m_def->get_loading_frame(); }

		character* add_empty_movieclip(const char* name, int depth);

		exported_module void	set_play_state(play_state s);
		play_state	get_play_state() const;

		character*	get_character(int character_id)
		{
			//			return m_def->get_character_def(character_id);
			// @@ TODO -- look through our dlist for a match
			return NULL;
		}

		float	get_background_alpha() const
		{
			// @@ this doesn't seem right...
			return m_root->get_background_alpha();
		}

		float	get_pixel_scale() const { return m_root->get_pixel_scale(); }

		virtual void	get_mouse_state(int* x, int* y, int* buttons)
		{
			m_root->get_mouse_state(x, y, buttons);
		}

		void	set_background_color(const rgba& color)
		{
			m_root->set_background_color(color);
		}

//		virtual bool	has_looped() const { return m_has_looped; }

		inline int	transition(int a, int b) const
			// Combine the flags to avoid a conditional. It would be faster with a macro.
		{
			return (a << 2) | b;
		}

		virtual bool can_handle_mouse_event();
		virtual character*	get_topmost_mouse_entity(float x, float y);
		virtual bool hit_test(float x, float y);
		virtual void construct();
		virtual void advance(float delta_time);
		virtual void notify_set_member(const tu_stringi& name, const as_value& new_val);

		virtual void	this_alive();
		void	execute_frame_tags(int frame, bool state_only = false);
		void	execute_frame_tags_reverse(int frame);
		execute_tag*	find_previous_replace_or_add_tag(int frame, int depth, int id);
		void	execute_remove_tags(int frame);
		void	set_frame_script(int frame);	// flash9
		void	do_actions();
		void	do_init_actions();
		virtual void do_actions(const array<action_buffer*>& action_list);
		exported_module bool	goto_frame(const tu_string& target_frame);
		exported_module bool	goto_frame(int target_frame_number);
		exported_module bool	goto_labeled_frame(const tu_stringi& label);
		int get_frameid_from_label(const tu_stringi& label);

		virtual void update_world_matrix();
		virtual void update_world_cxform();

		void	display();

		character*	add_display_object(
			Uint16 character_id,
			const tu_string& name,
			const array<swf_event*>& event_handlers,
			int depth,
			bool replace_if_depth_is_occupied,
			const cxform* color_transform,
			const matrix* matrix,
			const effect* effect,
			float ratio,
			Uint16 clip_depth);

		void	move_display_object(
			int depth,
			const cxform* color_xform,
			const matrix* mat,
			const effect* effect,
			float ratio,
			Uint16 clip_depth);


		void	replace_display_object(
			Uint16 character_id,
			const char* name,
			int depth,
			const cxform* color_transform,
			const matrix* mat,
			const effect* effect,
			float ratio,
			Uint16 clip_depth);

		void	replace_display_object(
			character* ch,
			const char* name,
			int depth,
			const cxform* color_transform,
			const matrix* mat,
			const effect* effect,
			float ratio,
			Uint16 clip_depth);

		void	remove_display_object(int depth, int id);
		void	remove_display_object(const tu_string& name);
		void	remove_display_object(character* ch);
		void	clear_display_objects();

		virtual character* replace_me(movie_definition*	md);
		virtual character* replace_me(character_def*	def);

		void	add_action_buffer(action_buffer* a);
		int	get_id_at_depth(int depth);
		int	get_highest_depth();

		//
		// ActionScript support
		//

		virtual void	set_variable(const char* path_to_var, const char* new_value);
		virtual void	set_variable(const char* path_to_var, const wchar_t* new_value);
		virtual const char*	get_variable(const char* path_to_var) const;

		virtual bool	set_member(const tu_stringi& name, const as_value& val);
		virtual bool	get_member(const tu_stringi& name, as_value* val);
		virtual void	call_frame_actions(const as_value& frame_spec);
		virtual void	set_drag_state(const drag_state& st);
		virtual void	stop_drag();
		virtual void	get_drag_state(drag_state* st);
		character*	clone_display_object(const tu_string& newname, int depth);
		virtual bool	on_event(const event_id& id);
		virtual const char*	call_method_args(const char* method_name, const char* method_arg_fmt, va_list args);
		virtual tu_string	call_method(const char* method_name, as_value * arguments, int argument_count );
		virtual void	attach_display_callback(const char* path_to_object, void (*callback)(render_state&, void*), void* user_ptr);
		bool	hit_test(character* target);
		bool	hit_test(float x, float y, bool shape);
		sprite_instance* attach_movie(const tu_string& id, const tu_string name, int depth);

		character* create_text_field(const char* name, int depth, int x, int y, int width, int height);

		virtual void clear_refs(hash<as_object*, bool>* visited_objects, as_object* this_ptr);
		virtual as_environment*	get_environment();
		virtual void dump(tu_string& tabs);

		virtual character_def*	find_exported_resource(const tu_string& symbol);

		// gameSWF extension
		void	set_fps(float fps);

		// drawing API
		canvas* get_canvas();

		as_transform* get_transform();

		virtual const char*	_typeof() { return "movieclip"; }
		virtual	void enumerate(as_environment* env);
		virtual	bool is_enabled() const;

		// flash9
		void add_script(int frame, as_function* func);
	};
}

#endif
