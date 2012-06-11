#pragma once
// gameswf.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// gameswf player implementation
// Here are collected all static variables,
// init_action() and clear_libray()


#ifndef player_H
#define player_H

#include "base/utility.h"
#include "base/tu_loadlib.h"
#include "gameswf/gameswf_object.h"
#include "gameswf/gameswf_debugger.h"

namespace gameswf
{
	struct sprite_instance;
	struct generic_character;
	struct edit_text_character;
	struct movie_definition_sub;
	struct edit_text_character_def;

	as_value	get_property(as_object* obj, int prop_number);
	void	set_property(as_object* obj, int prop_number, const as_value& val);

	fscommand_callback	get_fscommand_callback();
	void	register_fscommand_callback(fscommand_callback handler);

	string_hash<tu_loadlib*>* get_shared_libs();
	void clear_shared_libs();

	const tu_string& get_static_string(const tu_string& name);

	struct player : public ref_counted
	{
		array<smart_ptr<as_object> > m_heap;
		array<smart_ptr<character> > m_dlist_stack;
		permanent_string_cache m_const_strings;
		int m_aliveID;
		smart_ptr<as_object>	m_global;
		smart_ptr<as_object>	m_stage;
		Uint64 m_start_time;
		weak_ptr<root> m_current_root;
		tu_string m_workdir;
		stringi_hash< smart_ptr<character_def> > m_chardef_library;
		tu_string m_flash_vars;
		
		as_value m_as_object;
		as_value m_as_global_movieclip_ctor;
		
		void* m_userdata;
		bool m_render_caching;
		array<blend_mode::id> m_blend_stack;

		player_context* m_context;

#if GAMESWF_ENABLE_DEBUGGER
		debugger m_debugger;
#endif
		
		exported_module  player(player_context* context);
		exported_module  ~player();

		// external interface
		exported_module root* get_root();
		exported_module void set_root(root* m);
		exported_module void notify_key_event(key::code k, bool down);
		exported_module void verbose_action(bool val);
		exported_module void verbose_parse(bool val);
		exported_module void set_flash_vars(const tu_string& param);
		exported_module const char* get_workdir() const;
		exported_module void set_workdir(const char* dir);
	
		// @@ Hm, need to think about these creation API's.  Perhaps
		// divide it into "low level" and "high level" calls.  Also,
		// perhaps we need a "context" object that contains all
		// global-ish flags, libraries, callback pointers, font
		// library, etc.
		//
		// Create a gameswf::movie_definition from the given file name.
		// Normally, will also try to load any cached data file
		// (".gsc") that corresponds to the given movie file.  This
		// will still work even if there is no cache file.  You can
		// disable the attempts to load cache files by calling
		// gameswf::use_cache_files(false).
		//
		// Uses the registered file-opener callback to read the files
		// themselves.
		//
		// This calls add_ref() on the newly created definition; call
		// drop_ref() when you're done with it.
		// Or use smart_ptr<T> from base/smart_ptr.h if you want.
		exported_module movie_definition*	create_movie(const char* filename);
		exported_module	smart_ptr<root>  load_file(const char* filename);


		// Create/hook built-ins.
		exported_module void action_init();
	
		// Clean up any stray heap stuff we've allocated.
		exported_module void	action_clear();

		// library stuff, for sharing resources among different movies.
		stringi_hash< smart_ptr<character_def> >* get_chardef_library();
		void clear_library();

		as_object* get_global() const;
		as_object* get_stage() const;
		void notify_key_object(key::code k, bool down);
		Uint64 get_start_time()	{ return m_start_time; }

		// the garbage manager
		void register_object(as_object* obj)
		{
			m_heap.push_back(obj);
		}

		void set_alive(as_object* obj)
		{
			obj->m_aliveID = m_aliveID;
		}

		bool is_garbage(as_object* obj)
		{
			return obj->m_aliveID != m_aliveID;
		}

		void clear_heap();
		void set_as_garbage();
		void clear_garbage();
		
		// character instanciation/recycling
		sprite_instance* create_sprite_instance(movie_definition_sub* def, root* r, character* parent, int id);
		generic_character* create_generic_character(character_def* def, character* parent, int id);
		edit_text_character* create_edit_text_character(edit_text_character_def* def, character* parent, int id);

		void notify_unused_instance(character* ch);

	private:
		void clear_unused_instances();

		array<smart_ptr<character> > m_sprites;
		array<smart_ptr<character> > m_generics;
		array<smart_ptr<character> > m_edits;

		// strings
	public:
		inline const tu_string& get_const_string(const tu_string& name) { return m_const_strings.get(name); }
	};
}

#endif

