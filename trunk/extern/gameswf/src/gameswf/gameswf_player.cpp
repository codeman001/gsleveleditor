#include "stdafx.h"
// gameswf.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// here are collected all statics of gameswf library and also
// init_library and clear_library implementation

#include "base/tu_timer.h"
#include "base/tu_file.h"
#include "base/tu_random.h"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_object.h"
#include "gameswf/gameswf_action.h"
#include "gameswf/gameswf_dlist.h"
#include "gameswf/gameswf_sprite.h"

// action script classes
#include "gameswf/gameswf_as_sprite.h"
#include "gameswf/gameswf_text.h"
#include "gameswf/gameswf_as_classes/as_array.h"
#include "gameswf/gameswf_as_classes/as_sound.h"
#include "gameswf/gameswf_as_classes/as_key.h"
#include "gameswf/gameswf_as_classes/as_math.h"
#include "gameswf/gameswf_as_classes/as_mcloader.h"
#include "gameswf/gameswf_as_classes/as_netstream.h"
#include "gameswf/gameswf_as_classes/as_netconnection.h"
#include "gameswf/gameswf_as_classes/as_textformat.h"
#include "gameswf/gameswf_as_classes/as_string.h"
#include "gameswf/gameswf_as_classes/as_color.h"
#include "gameswf/gameswf_as_classes/as_date.h"
//#include "gameswf/gameswf_as_classes/as_xmlsocket.h"
//#include "gameswf/gameswf_as_classes/as_loadvars.h"
#include "gameswf/gameswf_as_classes/as_flash.h"
#include "gameswf/gameswf_as_classes/as_broadcaster.h"
#include "gameswf/gameswf_as_classes/as_selection.h"
#include "gameswf/gameswf_as_classes/as_number.h"
#include "gameswf/gameswf_as_classes/as_boolean.h"
#include "gameswf/gameswf_as_classes/as_global.h"

#include "gameswf/gameswf_freetype.h"
#include "gameswf/gameswf_bitmapfont.h"

namespace gameswf
{
	// context
	player_context::player_context() :
			m_glyph_provider(NULL),
			m_bitmap_glyph_provider(NULL),
			m_file_counter(0),
			m_video_driver(NULL)
	{
	}

	player_context::~player_context()
	{
		assert(m_instances.size() == 0);
		
		if(m_glyph_provider != NULL)
			swfdelete (m_glyph_provider);

		if(m_bitmap_glyph_provider != NULL)
			swfdelete (m_bitmap_glyph_provider);
	}

	// stage
	struct as_stage : public as_object
	{
		as_stage(player* player) :
			as_object(player)
		{
			set_member("width", 1);
			set_member("height", 1);
		}

		// Unique id of a gameswf resource
		enum { m_class_id = AS_STAGE };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		virtual bool get_member(const tu_stringi& name, as_value* val)
		{
			if(name == "width")
			{
				val->set_int(m_player->m_current_root->get_movie_width());
				return true;
			}
			else if(name == "height")
			{
				val->set_int(m_player->m_current_root->get_movie_height());
				return true;
			}
			
			return false;
		}
	};

	//
	//	gameswf's statics
	//

	void register_as_native_function(const char* name, void (*func)(const fn_call& fn))
	{
		stringi_hash<as_value>* map = new_standard_method_map(BUILTIN_OBJECT_METHOD);
		map->set(name, func);
	}

	bool	s_use_cached_movie_def = true;

	//
	// file_opener callback stuff
	//

	static file_opener_callback	s_opener_function = NULL;

	void	register_file_opener_callback(file_opener_callback opener)
	// Host calls this to register a function for opening files,
	// for loading movies.
	{
		s_opener_function = opener;
	}

	static bool	s_use_cache_files = false;

	void	set_use_cache_files(bool use_cache)
	// Enable/disable attempts to read cache files when loading
	// movies.
	{
		s_use_cache_files = use_cache;
	}

	// shared static string
	static permanent_string_cache s_static_strings;

	const tu_string& get_static_string(const tu_string& s)
	{
		return s_static_strings.get(s);
	}

	// standard method map, this stuff should be high optimized

	static stringi_hash<as_value>*	s_standard_method_map[BUILTIN_COUNT];
	void clear_standard_method_map()
	{
		for (int i = 0; i < BUILTIN_COUNT; i++)
		{
			if (s_standard_method_map[i])
			{
				swfdelete (s_standard_method_map[i]);
				s_standard_method_map[i] = NULL;
			}
		}

		s_static_strings.clear();
	}

	bool get_builtin(builtin_object id, const tu_stringi& name, as_value* val)
	{
		if (s_standard_method_map[id])
		{
			return s_standard_method_map[id]->get(name, val);
		}
		return false;
	}

	stringi_hash<as_value>* new_standard_method_map(builtin_object id)
	{
		if (s_standard_method_map[id] == NULL)
		{
			s_standard_method_map[id] = swfnew stringi_hash<as_value>;
		}
		return s_standard_method_map[id];
	}

	void standard_method_map_init()
	{
		// setup builtin methods
		stringi_hash<as_value>* map;

		// as_object builtins
		map = new_standard_method_map(BUILTIN_OBJECT_METHOD);
		map->add("addProperty", as_object_addproperty);
		map->add("registerClass", as_object_registerclass);
		map->add("hasOwnProperty", as_object_hasownproperty);
		map->add("watch", as_object_watch);
		map->add("unwatch", as_object_unwatch);

		// flash9
		map->add("addEventListener", as_object_add_event_listener);

		// for debugging
#ifdef _DEBUG
		map->add("dump", as_object_dump);
#endif

		// as_number builtins
		map = new_standard_method_map(BUILTIN_NUMBER_METHOD);
		map->add("toString", as_number_to_string);
		map->add("valueOf", as_number_valueof);

		// as_boolean builtins
		map = new_standard_method_map(BUILTIN_BOOLEAN_METHOD);
		map->add("toString", as_boolean_to_string);
		map->add("valueOf", as_boolean_valueof);

		// as_string builtins
		map = new_standard_method_map(BUILTIN_STRING_METHOD);
		map->add("toString", string_to_string);
		map->add("fromCharCode", string_from_char_code);
		map->add("charCodeAt", string_char_code_at);
		map->add("concat", string_concat);
		map->add("indexOf", string_index_of);
		map->add("lastIndexOf", string_last_index_of);
		map->add("slice", string_slice);
		map->add("split", string_split);
		map->add("substring", string_substring);
		map->add("substr", string_substr);
		map->add("toLowerCase", string_to_lowercase);
		map->add("toUpperCase", string_to_uppercase);
		map->add("charAt", string_char_at);
		map->add("length", as_value(string_length, as_value()));

		// as_s_function builtins
		map = new_standard_method_map(BUILTIN_S_FUNCTION_METHOD);
		map->add("call", as_s_function_call);

		// sprite_instance builtins
		map = new_standard_method_map(BUILTIN_SPRITE_METHOD);
		map->add("play", sprite_play);
		map->add("stop", sprite_stop);
		map->add("gotoAndStop", sprite_goto_and_stop);
		map->add("gotoAndPlay", sprite_goto_and_play);
		map->add("nextFrame", sprite_next_frame);
		map->add("prevFrame", sprite_prev_frame);
		map->add("getBytesLoaded", sprite_get_bytes_loaded);
		map->add("getBytesTotal", sprite_get_bytes_total);
		map->add("swapDepths", sprite_swap_depths);
		map->add("duplicateMovieClip", sprite_duplicate_movieclip);
		map->add("getDepth", sprite_get_depth);
		map->add("createEmptyMovieClip", sprite_create_empty_movieclip);
		map->add("removeMovieClip", sprite_remove_movieclip);
		map->add("hitTest", sprite_hit_test);
		map->add("getBounds", sprite_get_bounds);
		map->add("localToGlobal", sprite_local_global);
		map->add("globalToLocal", sprite_global_local);
		map->add("startDrag", sprite_start_drag);
		map->add("stopDrag", sprite_stop_drag);
		map->add("loadMovie", sprite_loadmovie);
		map->add("unloadMovie", sprite_unloadmovie);
		map->add("getNextHighestDepth", sprite_getnexthighestdepth);
		map->add("getInstanceAtDepth", sprite_instance_at_depth);
		map->add("createTextField", sprite_create_text_field);
		map->add("attachMovie", sprite_attach_movie);

		// drawing API
		map->add("beginFill", sprite_begin_fill);
		map->add("endFill", sprite_end_fill);
		map->add("lineTo", sprite_line_to);
		map->add("moveTo", sprite_move_to);
		map->add("curveTo", sprite_curve_to);
		map->add("clear", sprite_clear);
		map->add("lineStyle", sprite_line_style);

		// gameSWF extension
		// reset root FPS
		map->add("setFPS", sprite_set_fps);

		// Flash9
		map->add("addFrameScript", sprite_add_script);

		// edit text builtins
		map = new_standard_method_map(BUILTIN_EDIT_TEXT_METHOD);
		map->add("setTextFormat", set_textformat);
		map->add("getTextFormat", get_textformat);

		// array builtins
		map = new_standard_method_map(BUILTIN_ARRAY_METHOD);
		map->add("toString", as_array_tostring);
		map->add("push", as_array_push);
		map->add("pop", as_array_pop);
		map->add("concat", as_array_concat);
		map->add("join", as_array_join);
		map->add("reverse", as_array_reverse);
		map->add("sort", as_array_sort);
		map->add("shift", as_array_shift);
		map->add("unshift", as_array_unshift);
		map->add("slice", as_array_slice);
		map->add("splice", as_array_splice);
		map->add("length", as_value(as_array_length, as_value()));

	}

	// Standard property lookup.

	static stringi_hash<as_standard_member>	s_standard_property_map;
	void clear_standard_property_map()
	{
		s_standard_property_map.clear();
	}
	const stringi_hash<as_standard_member>& get_standard_property_map()
	{
		return s_standard_property_map;
	}

#ifdef WIN32
	static tu_string s_gameswf_version("WIN");
#else
	static tu_string s_gameswf_version("LINUX");
#endif

	const char* get_gameswf_version() {	return s_gameswf_version.c_str(); }

	// dynamic library stuff, for sharing DLL/shared library among different movies.

	static string_hash<tu_loadlib*> s_shared_libs;

	string_hash<tu_loadlib*>* get_shared_libs()
	{
		return &s_shared_libs;
	}

	void clear_shared_libs()
	{
		for (string_hash<tu_loadlib*>::iterator it = s_shared_libs.begin();
			it != s_shared_libs.end(); ++it)
		{
			swfdelete (it->second);
		}
		s_shared_libs.clear();
	}

	// External interface.

	static fscommand_callback	s_fscommand_handler = NULL;

	fscommand_callback	get_fscommand_callback()
	{
		return s_fscommand_handler;
	}

	void	register_fscommand_callback(fscommand_callback handler)
	{
		s_fscommand_handler = handler;
	}

	as_standard_member	get_standard_member(const tu_stringi& name)
	{
		if (s_standard_property_map.size() == 0)
		{
			s_standard_property_map.set_capacity(int(AS_STANDARD_MEMBER_COUNT));

			s_standard_property_map.add("_x", M_X);
			s_standard_property_map.add("_y", M_Y);
			s_standard_property_map.add("_xscale", M_XSCALE);
			s_standard_property_map.add("_yscale", M_YSCALE);
			s_standard_property_map.add("_currentframe", M_CURRENTFRAME);
			s_standard_property_map.add("_totalframes", M_TOTALFRAMES);
			s_standard_property_map.add("_alpha", M_ALPHA);
			s_standard_property_map.add("_visible", M_VISIBLE);
			s_standard_property_map.add("_width", M_WIDTH);
			s_standard_property_map.add("_height", M_HEIGHT);
			s_standard_property_map.add("_rotation", M_ROTATION);
			s_standard_property_map.add("_target", M_TARGET);
			s_standard_property_map.add("_framesloaded", M_FRAMESLOADED);
			s_standard_property_map.add("_name", M_NAME);
			s_standard_property_map.add("_droptarget", M_DROPTARGET);
			s_standard_property_map.add("_url", M_URL);
			s_standard_property_map.add("_highquality", M_HIGHQUALITY);
			s_standard_property_map.add("_focusrect", M_FOCUSRECT);
			s_standard_property_map.add("_soundbuftime", M_SOUNDBUFTIME);
			s_standard_property_map.add("_xmouse", M_XMOUSE);
			s_standard_property_map.add("_ymouse", M_YMOUSE);
			s_standard_property_map.add("_parent", M_PARENT);
			s_standard_property_map.add("text", M_TEXT);
			s_standard_property_map.add("htmlText", M_HTMLTEXT);
			s_standard_property_map.add("textWidth", M_TEXTWIDTH);
			s_standard_property_map.add("textHeight", M_TEXTHEIGHT);
			s_standard_property_map.add("textColor", M_TEXTCOLOR);
			s_standard_property_map.add("border", M_BORDER);
			s_standard_property_map.add("multiline", M_MULTILINE);
			s_standard_property_map.add("wordWrap", M_WORDWRAP);
			s_standard_property_map.add("type", M_TYPE);
			s_standard_property_map.add("backgroundColor", M_BACKGROUNDCOLOR);
			s_standard_property_map.add("_this", M_THIS);
			s_standard_property_map.add("this", MTHIS);
			s_standard_property_map.add("_root", M_ROOT);
			s_standard_property_map.add(".", MDOT);
			s_standard_property_map.add("..", MDOT2);
			s_standard_property_map.add("_level0", M_LEVEL0);
			s_standard_property_map.add("_global", M_GLOBAL);
			s_standard_property_map.add("Stage", M_STAGE);
			s_standard_property_map.add("enabled", M_ENABLED);
			s_standard_property_map.add("transform", M_TRANSFORM);
			s_standard_property_map.add("matrix", M_MATRIX);
			s_standard_property_map.add("concatenatedMatrix", M_WORLDMATRIX);
			s_standard_property_map.add("colorTransform", M_COLORTRANSFORM);
			s_standard_property_map.add("concatenatedColorTransform", M_WORLDCOLORTRANSFORM);
		}

		as_standard_member	result = M_INVALID_MEMBER;
		s_standard_property_map.get(name, &result);

		return result;
	}

	//
	// properties by number
	//

	static const tu_string	s_property_names[] =
	{
		tu_string("_x"),
		tu_string("_y"),
		tu_string("_xscale"),
		tu_string("_yscale"),
		tu_string("_currentframe"),
		tu_string("_totalframes"),
		tu_string("_alpha"),
		tu_string("_visible"),
		tu_string("_width"),
		tu_string("_height"),
		tu_string("_rotation"),
		tu_string("_target"),
		tu_string("_framesloaded"),
		tu_string("_name"),
		tu_string("_droptarget"),
		tu_string("_url"),
		tu_string("_highquality"),
		tu_string("_focusrect"),
		tu_string("_soundbuftime"),
		tu_string("@@ mystery"),
		tu_string("_xmouse"),
		tu_string("_ymouse"),
	};

	as_value	get_property(as_object* obj, int prop_number)
	{
		as_value	val;
		if (prop_number >= 0 && prop_number < int(sizeof(s_property_names)/sizeof(s_property_names[0])))
		{
			obj->get_member(s_property_names[prop_number], &val);
		}
		else
		{
			log_error("error: invalid property query, property number %d\n", prop_number);
		}
		return val;
	}

	void	set_property(as_object* obj, int prop_number, const as_value& val)
	{
		if (prop_number >= 0 && prop_number < int(sizeof(s_property_names)/sizeof(s_property_names[0])))
		{
			obj->set_member(s_property_names[prop_number], val);
		}
		else
		{
			log_error("error: invalid set_property, property number %d\n", prop_number);
		}
	}


	//
	//	player
	//

	player::player(player_context* context)
	{
		m_context = context;
		m_aliveID = 1;
		m_global = swfnew as_object(this);
		m_stage = swfnew as_stage(this);

		m_as_object.set_as_object(swfnew as_object(this));
		m_as_global_movieclip_ctor.set_as_c_function(as_global_movieclip_ctor);

		action_init();

		m_context->m_instances.push_back(this);
		
		// set startup random position
		Uint64 t = tu_timer::get_systime();
		t &= 0xFF;	// truncate
		for (int i = 0; i < t; i++)
		{
			tu_random::next_random();
		}

		m_userdata = NULL;
		m_render_caching = false;
	}

	player::~player()
	{
		// Clean up gameswf as much as possible, so valgrind will help find actual leaks.
		// Maximum release of resources.  Calls clear_library() and
		// fontlib::clear(), and also clears some extra internal stuff
		// that may have been allocated (e.g. global ActionScript
		// objects).  This should get all gameswf structures off the
		// heap, with the exception of any objects that are still
		// referenced by the host program and haven't had drop_ref()
		// called on them.

#if GAMESWF_ENABLE_DEBUGGER
		m_debugger.close();
#endif

		clear_unused_instances();

		m_current_root = NULL;
		m_global = NULL;
		m_context->m_instances.remove_value(this);

		clear_heap();

		gameswf_engine_mutex().lock();

		clear_library();

		gameswf_engine_mutex().unlock();

		action_clear();
	}

	void player::set_flash_vars(const tu_string& param)
	// Allow pass user variables to Flash
	{
		m_flash_vars = param;
	}

	void player::verbose_action(bool val)
	{
		set_verbose_action(val);
	}

	void player::verbose_parse(bool val)
	{
		set_verbose_parse(val);
	}

	void	as_global_trace(const fn_call& fn);
	void	player::action_init()
	// Create/hook built-ins.
	{
		m_start_time = tu_timer::get_ticks();

		//
		// global init
		//
		set_alive(m_global.get_ptr());
		m_global->m_members.set_capacity(32);
		m_global->builtin_member("trace", as_global_trace);
		m_global->builtin_member("Object", as_global_object_ctor);
		m_global->builtin_member("Sound", as_global_sound_ctor);
		m_global->builtin_member("Array", array_init(this));
		m_global->builtin_member("MovieClip", as_global_movieclip_ctor);
		m_global->builtin_member("TextField", as_global_textfield_ctor);
		m_global->builtin_member("TextFormat", as_global_textformat_ctor);

		//			m_global->set_member("XML", as_value(xml_new));
		//m_global->builtin_member("XMLSocket", as_global_xmlsock_ctor);
		m_global->builtin_member("MovieClipLoader", as_global_mcloader_ctor);
		m_global->builtin_member("String", as_global_string_ctor);
		m_global->builtin_member("Number", as_global_number_ctor);
		m_global->builtin_member("Boolean", as_global_boolean_ctor);
		m_global->builtin_member("Color", as_global_color_ctor);
		m_global->builtin_member("Date", as_global_date_ctor);
		m_global->builtin_member("Selection", selection_init(this));
		//m_global->builtin_member("LoadVars", as_global_loadvars_ctor);

		// ASSetPropFlags
		m_global->builtin_member("ASSetPropFlags", as_global_assetpropflags);

		// for video
		m_global->builtin_member("NetStream", as_global_netstream_ctor);
		m_global->builtin_member("NetConnection", as_global_netconnection_ctor);

		m_global->builtin_member("math", math_init(this));
		m_global->builtin_member("Key", key_init(this));
		m_global->builtin_member("AsBroadcaster", broadcaster_init(this));
		m_global->builtin_member("flash", flash_init(this));

		// global builtins functions
		m_global->builtin_member("setInterval",  as_global_setinterval);
		m_global->builtin_member("clearInterval",  as_global_clearinterval);
		m_global->builtin_member("getVersion",  as_global_get_version);
		m_global->builtin_member("parseFloat",  as_global_parse_float);
		m_global->builtin_member("parseInt",  as_global_parse_int);
		m_global->builtin_member("isNaN",  as_global_isnan);
		m_global->builtin_member("$version", get_const_string("gameSWF"));

		m_global->builtin_member("getActiveController", as_global_get_active_controller);

	}


	void	player::action_clear()
	{
	}

	as_object* player::get_global() const
	{
		return m_global.get_ptr();
	}

	as_object* player::get_stage() const
	{
		return m_stage.get_ptr();
	}

	void player::notify_key_object(key::code k, bool down)
	{
		as_value	kval;
		as_object* global = get_global();
		global->get_member("Key", &kval);
		as_key*	ko = cast_to<as_key>(kval.to_object());
		if (ko)
		{
			if (down) ko->set_key_down(k);
			else ko->set_key_up(k);
		}
		else
		{
			log_error("gameswf::notify_key_event(): no Key built-in\n");
		}
	}

	root* player::get_root()
	{
		// on exit m_current_root may be NULL
//		assert(m_current_root.get_ptr() != NULL);
		return m_current_root.get_ptr();
	}

	void player::notify_key_event(key::code k, bool down)
	{
		m_current_root->notify_key_event(this, k, down);
	}

	void player::set_root(root* m)
	{
		assert(m != NULL);
		m_current_root = m;
	}

	const char* player::get_workdir() const
	{
		return m_workdir.c_str();
	}

	void player::set_workdir(const char* dir)
	{
		assert(dir != NULL);
		m_workdir = dir;
	}

	// library stuff, for sharing resources among different movies.
	stringi_hash< smart_ptr<character_def> >* player::get_chardef_library()
	{
		return &m_chardef_library;
	}

	void player::clear_library()
		// Drop all library references to movie_definitions, so they
		// can be cleaned up.
	{
		for (stringi_hash< smart_ptr<character_def> >::iterator it = 
			m_chardef_library.begin(); it != m_chardef_library.end(); ++it)
		{
			if (it->second->get_ref_count() > 1)
			{
				printf("memory leaks is found out: on exit movie_definition_sub ref_count > 1\n");
				printf("this = 0x%p, ref_count = %d\n", it->second.get_ptr(),
					it->second->get_ref_count());

				// to detect memory leaks
				while (it->second->get_ref_count() > 1)	it->second->drop_ref();
			}
		}
		m_chardef_library.clear();
	}

	void	ensure_loaders_registered();
	movie_definition*	player::create_movie(const char* filename)
	{
		assert(filename);

		// Is the movie already in the library?
		if (s_use_cached_movie_def)
		{
			smart_ptr<character_def>	m;
			get_chardef_library()->get(filename, &m);
			if (m != NULL)
			{
				// Return cached movie.
				return cast_to<movie_definition>(m.get_ptr());
			}
		}

		if (s_opener_function == NULL)
		{
			// Don't even have a way to open the file.
			log_error("error: no file opener function; can't create movie.	"
				"See gameswf::register_file_opener_callback\n");
			return NULL;
		}

		tu_file* in = s_opener_function(filename);
		if (in == NULL)
		{
			log_error("failed to open '%s'; can't create movie.\n", filename);
			return NULL;
		}
		else if (in->get_error())
		{
			log_error("error: file opener can't open '%s'\n", filename);
			swfdelete (in);
			return NULL;
		}

		ensure_loaders_registered();

		movie_def_impl*	m = swfnew movie_def_impl(this, DO_LOAD_BITMAPS, DO_NOT_LOAD_FONT_SHAPES);

		m->read(in);

		// "in" will be deleted after termination of the loader thread
		//	swfdelete (in);

		if (m && s_use_cache_files)
		{
			// Try to load a .gsc file.
			tu_string	cache_filename(filename);
			cache_filename += ".gsc";
			tu_file*	cache_in = s_opener_function(cache_filename.c_str());
			if (cache_in == NULL
				|| cache_in->get_error() != TU_FILE_NO_ERROR)
			{
				// Can't open cache file; don't sweat it.
				IF_VERBOSE_PARSE(log_msg("note: couldn't open cache file '%s'\n", cache_filename.c_str()));

//				m->generate_font_bitmaps();	// can't read cache, so generate font texture data.
			}
			else
			{
				// Load the cached data.
				m->input_cached_data(cache_in);
			}

			swfdelete (cache_in);
		}

		// We should not do m->add_ref() in order to prevent memory leaks
		// More correctly to do so:
		// smart_ptr<movie_definition_sub> md = create_movie_sub("my.swf")
		//		m->add_ref();

		if (s_use_cached_movie_def)
		{
			get_chardef_library()->add(filename, m);
		}

		return m;
	}

	smart_ptr<root> player::load_file(const char* infile)
	// Load the actual movie.
	{
		GAMESWF_PROFILER_SCOPE_EVENT("player::load_file");

		smart_ptr<gameswf::movie_definition>	md = create_movie(infile);
		if (md == NULL)
		{
			fprintf(stderr, "error: can't create a movie from '%s'\n", infile);
			return NULL;
		}

		smart_ptr<gameswf::root>	m = md->create_instance();
		if (m == NULL)
		{
			fprintf(stderr, "error: can't create movie instance\n");
			return NULL;
		}

		int	movie_version = m->get_movie_version();

	#ifdef _DEBUG
		log_msg("Playing %s, swf version %d\n", infile, movie_version);
	#else
		IF_VERBOSE_PARSE(log_msg("Playing %s, swf version %d\n", infile, movie_version));
	#endif

#if GAMESWF_ENABLE_DEBUGGER
		m_debugger.connect(this);
#endif

		return m;
	}


	// garbage collector

	void player::clear_heap()
	{
		for (int i = 0; i < m_heap.size(); i++)
		{
			as_object* obj = m_heap[i].get_ptr();
			if (obj)
			{
				if (obj->get_ref_count() > 1)
				{
					hash<as_object*, bool> visited_objects;
					obj->clear_refs(&visited_objects, obj);
				}
			}
		}
		m_heap.clear();
	}

	void player::set_as_garbage()
	{
		// increment alive ID
		m_aliveID++;
	}



	void player::clear_garbage()
	{
		GAMESWF_PROFILER_SCOPE_EVENT("player::clear_garbage");

		as_object* global = get_global();
		global->this_alive();

		for (int i = m_heap.size() - 1; i >= 0; i--)
		{
			as_object* obj = m_heap[i].get_ptr();
			if (obj)
			{
				if (obj->m_aliveID != m_aliveID /*&& obj->m_aliveID != -1*/)	// is garbage ?
				{
					if (obj->get_ref_count() > 1)	// is in heap only ?
					{
						hash<as_object*, bool> visited_objects;
						obj->clear_refs(&visited_objects, obj);
					}
					
					//if(obj->get_ref_count() == 1 && obj->is(AS_CHARACTER))
					//{
					//	notify_unused_instance(static_cast<character*>(obj));
					//}

					// replace entry with last and shrink
					m_heap[i] = m_heap.back();
					m_heap.remove(m_heap.size() - 1);
				}
			}
		}
	}

	sprite_instance* player::create_sprite_instance(movie_definition_sub* def, root* r, character* parent, int id)
	{
		if(m_sprites.size() > 0)
		{
			sprite_instance* ch = static_cast<sprite_instance*>(m_sprites.back().get_ptr());
			ch->m_root = r;
			ch->m_def = def;
			ch->recycle(parent, id);
			
			// may need to register object again (if only referenced by m_dead_instances), and remove from dead instance
			if(ch->get_ref_count() == 1)
				register_object(ch);
			
			set_alive(ch);
			m_sprites.pop_back();

			return ch;
		}
		else
		{
			//printf("swfnew sprite_instance id=%d\n", id);
			return swfnew sprite_instance(this, def, r, parent, id);
		}
	}

	generic_character* player::create_generic_character(character_def* def, character* parent, int id)
	{
		if(m_generics.size() > 0)
		{
			generic_character* ch = static_cast<generic_character*>(m_generics.back().get_ptr());
			ch->m_def = def;
			ch->recycle(parent, id);
			
			// may need to register object again (if only referenced by m_dead_instances), and remove from dead instance
			if(ch->get_ref_count() == 1)
				register_object(ch);
			
			set_alive(ch);
			m_generics.pop_back();

			return ch;
		}
		else
		{
			//printf("swfnew generic_character id=%d\n", id);
			return swfnew generic_character(this, def, parent, id);
		}
	}

	edit_text_character* player::create_edit_text_character(edit_text_character_def* def, character* parent, int id)
	{
		if(m_edits.size() > 0)
		{
			edit_text_character* ch = static_cast<edit_text_character*>(m_edits.back().get_ptr());
			ch->m_def = def;
			ch->recycle(parent, id);
			
			// may need to register object again (if only referenced by m_dead_instances), and remove from dead instance
			if(ch->get_ref_count() == 1)
				register_object(ch);
			
			set_alive(ch);
			m_edits.pop_back();

			return ch;
		}
		else
		{
			//printf("swfnew edit_text_character id=%d\n", id);
			return swfnew edit_text_character(this, parent, def, id);
		}
	}

	void player::notify_unused_instance(character* ch)
	{
		//ch->m_aliveID = -1;
		ch->detach_weak_proxy();

		switch(ch->m_type)
		{
		case AS_SPRITE:
			for(int i = 0; i < m_sprites.size(); i++)
			{
				assert(m_sprites[i] != ch);
			}
			m_sprites.push_back(ch);
			break;
			
		case AS_GENERIC_CHARACTER:
			cast_to<generic_character>(ch)->m_def = NULL;
			for(int i = 0; i < m_generics.size(); i++)
			{
				assert(m_generics[i] != ch);
			}
			m_generics.push_back(ch);
			break;
		
		case AS_EDIT_TEXT:
			for(int i = 0; i < m_edits.size(); i++)
			{
				assert(m_edits[i] != ch);
			}
			m_edits.push_back(ch);
			break;

		default:
			return;
		}

		ch->m_members.clear();
		hash<as_object*, bool> visited_objects;
		ch->clear_refs(&visited_objects, ch);
	}

	void player::clear_unused_instances()
	{
		m_sprites.clear();
		m_generics.clear();
		m_edits.clear();
	}
}



// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:

