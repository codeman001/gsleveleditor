#pragma once
// gameswf.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// SWF (Shockwave Flash) player library.  The info for this came from
// http://www.openswf.org, the flashsource project, and swfparse.cpp


#ifndef GAMESWF_H
#define GAMESWF_H


#include <ctype.h>	// for poxy wchar_t
//#include <stdarg.h>	// for va_list arg to character::call_method_args()
#include "base/image.h"	// for delete m_suspended_image
#include "base/container.h"	// for hash<...>
#include "base/smart_ptr.h"


namespace gameswf
{
	class tu_file;
	struct render_handler;
	class weak_proxy;	// forward decl; defined in base/smart_ptr.h
};

// @@ forward decl to avoid including base/image.h; TODO change the
// render_handler interface to not depend on these structs at all.
namespace gameswf { struct rgb; struct rgba; struct image_base; }

// forward decl

namespace gameswf
{
	class tu_string;
	class tu_stringi;

	namespace jpeg { struct input; }
};

// Forward decl for D3D render handlers, in case they are
// instantiated.  Harmless on non-D3D platforms.
struct IDirect3DDevice9;
struct IDirect3DDevice8;

// function to be implemented in game
bool get_fontfile(const char* font_name, bool bold, bool italic, char* path, int maxlength);

namespace gameswf
{
	// Forward declarations.
	struct player;
	struct as_value;
	struct bitmap_info;
	struct character;
	struct execute_tag;
	struct font;
	struct render_handler;
	struct rgba;
	struct sound_handler;
	struct stream;
	struct video_handler;
	struct event_id;
	struct root;
	struct movie_def_impl;
	struct rect;
	struct as_environment;
	struct character_def;
	struct sound_sample;
	struct video_stream_definition;
	struct sprite_definition;
	struct as_s_function;
	struct as_object;
	struct movie_definition;
	struct membuf;
	struct glyph_provider;
	struct bitmap_glyph_provider;
	struct render_cache;
	struct fn_call;
	
	//
	// Log & error reporting control.
	//

	// Supply a function pointer to receive log & error messages.
	exported_module void	register_log_callback(void (*callback)(bool error, const char* message));


	// Control verbosity of specific categories.
	exported_module bool get_verbose_parse();
	exported_module bool get_verbose_debug();
	exported_module bool get_verbose_action();
	exported_module bool get_verbose_dlist();
	exported_module void	set_verbose_action(bool verbose);
	exported_module void	set_verbose_parse(bool verbose);
	exported_module void	set_verbose_dlist(bool verbose);

	// Supply a function pointer that can be called from action script as any function.
	exported_module void	register_as_native_function(const char* name, void (*func)(const fn_call& fn));

	// Supply a function pointer to substitute textures that have been exported in flash with an identifier.
	// If the return value is not NULL, the texture will be grabbed, otherwize, the default image data will be used.
	exported_module void	register_texture_loader_callback(video_texture* (*callback)(const char* resource_name, int width, int height));

	// Get and set the render handler.  This is one of the first
	// things you should do to initialise the player (assuming you
	// want to display anything).
	exported_module void	set_render_handler(render_handler* s);

	// Pass in a sound handler, so you can handle audio on behalf of
	// gameswf.  This is optional; if you don't set a handler, or set
	// NULL, then sounds won't be played.
	//
	// If you want sound support, you should set this at startup,
	// before loading or playing any movies!
	exported_module void	set_sound_handler(sound_handler* s);

	// You probably don't need this. (@@ make it private?)
	exported_module sound_handler*	get_sound_handler();

	// Register a callback to the host, for providing a file,
	// given a "URL" (i.e. a path name).  This is the only means
	// by which the gameswf library accesses file data, for
	// loading movies, cache files, and so on.
	//
	// gameswf will call this when it needs to open a file.
	//
	// NOTE: the returned tu_file* will be delete'd by gameswf
	// when it is done using it.  Your file_opener_function may
	// return NULL in case the requested file can't be opened.
	typedef tu_file* (*file_opener_callback)(const char* url_or_path);
	exported_module void	register_file_opener_callback(file_opener_callback opener);

	// ActionScripts embedded in a movie can use the built-in
	// fscommand() function to send data back to the host
	// application.	 If you are interested in this data, register
	// a handler, which will be called when the embedded scripts
	// call fscommand().
	//
	// The handler gets the character* that the script is
	// embedded in, and the two string arguments passed by the
	// script to fscommand().
	typedef void (*fscommand_callback)(character* movie, const char* command, const char* arg);
	exported_module void	register_fscommand_callback(fscommand_callback handler);

	// Use this to control how finely curves are subdivided.  1.0
	// is the default; it's a pretty good value.  Larger values
	// result in coarser, more angular curves with fewer vertices.
	exported_module void	set_curve_max_pixel_error(float pixel_error);
	exported_module float	get_curve_max_pixel_error();

	// Some helpers that may or may not be compiled into your
	// version of the library, depending on platform etc.
	exported_module render_handler*	create_render_handler_xbox();
	exported_module render_handler*	create_render_handler_ogl();
	exported_module render_handler*	create_render_handler_irrlicht(void* videoDriver);
	exported_module render_handler* create_render_handler_d3d(IDirect3DDevice9* _pDevice);
	exported_module render_handler* create_render_handler_d3d(IDirect3DDevice8* _pDevice);
	exported_module sound_handler*	create_sound_handler_sdl();


	//// For stuff that's tricky to keep track of w/r/t ownership & cleanup.
	//struct ref_counted
	//{
	//	exported_module ref_counted();
	//	exported_module virtual ~ref_counted();
	//	exported_module void	add_ref() const;
	//	exported_module void	drop_ref();
	//	exported_module int	get_ref_count() const { return m_ref_count; }
	//	exported_module weak_proxy*	get_weak_proxy() const;

	//private:
	//	mutable int	m_ref_count;
	//	mutable weak_proxy*	m_weak_proxy;
	//};

	// Unique id of all gameswf resources
	enum as_classes
	{
		AS_OBJECT,
		AS_CHARACTER,
		AS_SPRITE,
		AS_GENERIC_CHARACTER,
		AS_FUNCTION,
		AS_C_FUNCTION,
		AS_S_FUNCTION,
		AS_3_FUNCTION,	// action script 3 function
		AS_MOVIE_DEF,
		AS_MOVIE_DEF_SUB,
		AS_CHARACTER_DEF,
		AS_SPRITE_DEF,
		AS_VIDEO_DEF,
		AS_SOUND_SAMPLE,
		AS_VIDEO_INST,
		AS_KEY,
		AS_ARRAY,
		AS_COLOR,
		AS_SOUND,
		AS_FONT,
		AS_CANVAS,
		AS_STAGE,
		AS_NETSTREAM,
		AS_STRING,
		AS_SELECTION,
		AS_POINT,
		AS_MATRIX,
		AS_TRANSFORM,
		AS_COLOR_TRANSFORM,
		AS_NETCONNECTION,
		AS_LISTENER,
		AS_DATE,
		AS_EDIT_TEXT,
		AS_BITMAP,
		AS_XML_SOCKET,
		AS_TEXTFORMAT,
		AS_MCLOADER,
		AS_TIMER,
		AS_LOADVARS,

		// flash9
		AS_EVENT,
		AS_MOUSE_EVENT,

		// plugins
		AS_PLUGIN_MYDB,
		AS_PLUGIN_MYTABLE,
		AS_PLUGIN_SYSINFO,
		AS_PLUGIN_3DS,

		// user defined plugins
		// should be the last in this enum
		AS_USER_PLUGIN = 1000

	};

	enum special_chars
	{
		AUTO_LINE_FEED = 17
	};

	//! enumeration for the orthographic projection orientations
	enum orientation_mode
	{
		ORIENTATION_0 = 0,	// default, no rotation
		ORIENTATION_90,		// rotated 90 degrees clockwise
		ORIENTATION_180,	// rotated 180 degrees clockwise
		ORIENTATION_270		// rotated 270 degrees clockwise
	};

	//! enumeration for the scaling modes
	enum scale_mode
	{
		SCALE_EXACTFIT = 0,	// default, makes the entire Flash content visible in the specified area without trying to preserve the original aspect ratio. Distortion can occur.
		SCALE_NOBORDER,		// scales the Flash content to fill the specified area, without distortion but possibly with some cropping, while maintaining the original aspect ratio of the application. 
		SCALE_SHOWALL		// makes the entire Flash content visible in the specified area without distortion while maintaining the original aspect ratio of the. Borders can appear on two sides of the application. 
	};

	// context for each player, which can be shared between mutliple players
	struct player_context : public ref_counted
	{
		player_context();
		~player_context();
		int create_file_id() { return m_file_counter++; }

		glyph_provider* m_glyph_provider;
		bitmap_glyph_provider* m_bitmap_glyph_provider;
		array<player*> m_instances;
		int m_file_counter;
		gameswf::video_driver* m_video_driver;
	};

	// This is the base class for all ActionScript-able objects
	// ("as_" stands for ActionScript).
	struct as_object_interface : public ref_counted
	{
		virtual bool is(int class_id) const = 0;
	};

	// ref counted shared constant string
	struct as_string : ref_counted
	{
		as_string(const char* str) :
			m_string(str)
		{
		}

		as_string(const tu_string& str) :
			m_string(str)
		{
		}

		virtual ~as_string()
		{
			//printf("~as_string '%s'\n", m_string.c_str());
		}

		const tu_string m_string;
	};

	// cast_to<gameswf object>(obj) implementation (from Julien Hamaide)
	template <typename cast_class>
	cast_class* cast_to(as_object_interface* object)
	{
		if (object)
		{
			return object->is(cast_class::m_class_id) ? static_cast<cast_class*>(object) : 0;
		}
		return 0;
	}

	template <typename cast_class>
	const cast_class* cast_to(const as_object_interface* object)
	{
		if (object)
		{
			return object->is(cast_class::m_class_id) ? static_cast<const cast_class*>(object) : 0;
		}
		return 0;
	}

	// For caching precomputed stuff.  Generally of
	// interest to gameswf_processor and programs like it.
	struct cache_options
	{
		bool	m_include_font_bitmaps;

		cache_options()
			:
			m_include_font_bitmaps(true)
		{
		}
	};


	// Keyboard handling
	namespace key
	{
		enum code
		{
			INVALID = 0,
			A = 65,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,
			_0 = 48,
			_1,
			_2,
			_3,
			_4,
			_5,
			_6,
			_7,
			_8,
			_9,
			KP_0 = 96,
			KP_1,
			KP_2,
			KP_3,
			KP_4,
			KP_5,
			KP_6,
			KP_7,
			KP_8,
			KP_9,
			KP_MULTIPLY,
			KP_ADD,
			KP_ENTER,
			KP_SUBTRACT,
			KP_DECIMAL,
			KP_DIVIDE,
			F1 = 112,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			F13,
			F14,
			F15,
			BACKSPACE = 8,
			TAB,
			CLEAR = 12,
			ENTER,
			SHIFT = 16,
			CONTROL,
			ALT,
			CAPSLOCK = 20,
			ESCAPE = 27,
			SPACE = 32,
			PGDN,
			PGUP,
			END = 35,
			HOME,
			LEFT,
			UP,
			RIGHT,
			DOWN,
			INSERT = 45,
			DELETEKEY,
			HELP,
			NUM_LOCK = 144,
			SEMICOLON = 186,
			EQUALS = 187,
			MINUS = 189,
			SLASH = 191,
			BACKTICK = 192,
			LEFT_BRACKET = 219,
			BACKSLASH = 220,
			RIGHT_BRACKET = 221,
			QUOTE = 222,

			KEYCOUNT
		};
	}	// end namespace key

	// Try to grab movie info from the header of the given .swf
	// file.
	//
	// Sets *version to 0 if info can't be extracted.
	//
	// You can pass NULL for any entries you're not interested in.
	void	get_movie_info(
		const char*	filename,
		int*		version,
		int*		width,
		int*		height,
		float*		frames_per_second,
		int*		frame_count,
		int*		tag_count);

	// Enable/disable attempts to read cache files (.gsc) when
	// loading movies.
	exported_module void	set_use_cache_files(bool use_cache);

	//
	// Use DO_NOT_LOAD_BITMAPS if you have pre-processed bitmaps
	// stored externally somewhere, and you plan to install them
	// via get_bitmap_info()->...
	enum create_bitmaps_flag
	{
		DO_LOAD_BITMAPS,
		DO_NOT_LOAD_BITMAPS
	};
	// Use DO_NOT_LOAD_FONT_SHAPES if you know you have
	// precomputed texture glyphs (in cached data) and you know
	// you always want to render text using texture glyphs.
	enum create_font_shapes_flag
	{
		DO_LOAD_FONT_SHAPES,
		DO_NOT_LOAD_FONT_SHAPES
	};

	// unused
//	movie_definition*	create_movie_no_recurse(
//		tu_file*		input_stream,
//		create_bitmaps_flag	cbf,
//		create_font_shapes_flag cfs);

	// Helper to pregenerate cached data (basically, shape
	// tesselations).  Does this by running through each frame of
	// the movie and displaying the shapes with a null renderer.
	// The pregenerated data is stored in the movie_definition
	// object itself, and is included with the cached data written
	// by movie_definition::output_cached_data().
	//
	// Note that this tesselates shapes to the resolution they
	// explicitly appear in the linear frames of the movie.	 Does
	// not try very hard to run your ActionScript to account for
	// dynamic scaling (that's more or less futile anyway due to
	// the halting problem).
	void	precompute_cached_data(movie_definition* movie_def);

	//
	// Library management
	//

	// Release any library movies we've cached.  Do this when you want
	// maximum cleanup.
	void	clear_library();

	//
	// Sound callback handler.
	//

	// You may define a subclass of this, and pass an instance to
	// set_sound_handler().
	struct sound_handler
	{
		// audio for video
		typedef void (*aux_streamer_ptr)(as_object* netstream, unsigned char* stream, int len);

		enum format_type
		{
			FORMAT_RAW = 0,		// unspecified format.	Useful for 8-bit sounds???
			FORMAT_ADPCM = 1,	// gameswf doesn't pass this through; it uncompresses and sends FORMAT_NATIVE16
			FORMAT_MP3 = 2,
			FORMAT_UNCOMPRESSED = 3,	// 16 bits/sample, little-endian
			FORMAT_NELLYMOSER = 6,	// Mystery proprietary format; see nellymoser.com

			// gameswf tries to convert data to this format when possible:
			FORMAT_NATIVE16 = 7	// gameswf extension: 16 bits/sample, native-endian
		};

		// If stereo is true, samples are interleaved w/ left sample first.

		// gameswf calls at load-time with sound data, to be
		// played later.  You should create a sample with the
		// data, and return a handle that can be used to play
		// it later.  If the data is in a format you can't
		// deal with, then you can return 0 (for example), and
		// then ignore 0's in play_sound() and delete_sound().
		//
		// Assign handles however you like.
		virtual int	create_sound(
			void*		data,
			int		data_bytes,
			int		sample_count,
			format_type	format,
			int		sample_rate,	/* one of 5512, 11025, 22050, 44100 */
			bool		stereo) = 0;

		// loads external sound file
		virtual int	load_sound(const char* url) = 0;

		virtual void append_sound(int sound_handle, void* data, int data_bytes) = 0;

		// gameswf calls this when it wants you to play the defined sound.
		//
		// loop_count == 0 means play the sound once (1 means play it twice, etc)
		virtual void	play_sound(int sound_handle, int loop_count /* , volume, pan, etc? */) = 0;

		virtual void	set_volume(int sound_handle, int volume) = 0;

		// Stop the specified sound if it's playing.
		// (Normally a full-featured sound API would take a
		// handle specifying the *instance* of a playing
		// sample, but SWF is not expressive that way.)
		virtual void	stop_sound(int sound_handle) = 0;
		virtual void	stop_all_sounds() = 0;

		// gameswf calls this when it's done with a particular sound.
		virtual void	delete_sound(int sound_handle) = 0;

		// They are called from netstream when it wishes to sound video
		virtual void attach_aux_streamer(aux_streamer_ptr ptr, as_object* netstream) {};
		virtual void detach_aux_streamer(as_object* netstream) {};
		virtual void cvt(short int** adjusted_data, int* adjusted_size, unsigned char* data,
				 int size, int channels, int freq) {};

		virtual ~sound_handler() {};
		virtual bool is_open() { return false; };
		virtual void pause(int sound_handle, bool paused) {};

		// The number of milliseconds a sound has been playing. 
		// If the sound is looped, the position is reset to 0 at the beginning of each loop.
		virtual int get_position(int sound_handle) { return 0; };
	};

	// tu_float is used in matrix & cxform because
	// Flash converts inf to zero when works with matrix & cxform
	struct tu_float
	{
		operator float() const { return m_float; }
		inline void	operator=(const float x)
		{
			m_float = x >= -3.402823466e+38F && x <= 3.402823466e+38F ? x : 0.0f;
		}
		void	operator+=(const float x) { operator=(m_float + x); }
		void	operator-=(const float x) { operator=(m_float - x); }
		void	operator*=(const float x) { operator=(m_float * x); }
		void	operator/=(const float x) { operator=(m_float / x); }

	private:
		float m_float;
	};

	//
	// point: used by rect which is used by render_handler (otherwise would be in internal gameswf_types.h)
	//


	struct point
	{
		float	m_x, m_y;

		point() : m_x(0), m_y(0) {}
		point(float x, float y) : m_x(x), m_y(y) {}

		void	set_lerp(const point& a, const point& b, float t)
		// Set to a + (b - a) * t
		{
			m_x = a.m_x + (b.m_x - a.m_x) * t;
			m_y = a.m_y + (b.m_y - a.m_y) * t;
		}

		bool operator==(const point& p) const { return m_x == p.m_x && m_y == p.m_y; }

		bool	bitwise_equal(const point& p) const;
		
		float get_length() const;

		void twips_to_pixels();
		void pixels_to_twips();
	};

	struct vector2df
	{
		float x;
		float y;

		vector2df() {}
		vector2df(float x, float y) : x(x), y(y) {}
	};

	struct vector3df
	{
		float x;
		float y;
		float z;

		vector3df() {}
		vector3df(float x, float y, float z) : x(x), y(y), z(z) {}
	};

	//
	// matrix type, used by render handler
	//

	struct matrix
	{
		tu_float	m_[2][3];

		static matrix	identity;

		matrix()
		{
			// Default to identity.
			set_identity();
		}
		void	set_identity();
		void	concatenate(const matrix& m);
		void	concatenate_translation(float tx, float ty);
		void	concatenate_scale(float s);
		void	set_lerp(const matrix& m1, const matrix& m2, float t);
		void	set_scale_rotation(float x_scale, float y_scale, float rotation);
		void	read(stream* in);
		void	print() const;
		void	transform(point* result, const point& p) const;
		void	transform(rect* bound) const;
		void	transform_vector(point* result, const point& p) const;
		void	transform_by_inverse(point* result, const point& p) const;
		void	transform_by_inverse(rect* bound) const;
		void	set_inverse(const matrix& m);
		bool	does_flip() const;	// return true if we flip handedness
		float	get_determinant() const;	// determinant of the 2x2 rotation/scale part only
		float	get_max_scale() const;	// return the maximum scale factor that this transform applies
		float	get_x_scale() const;	// return the magnitude scale of our x coord output
		float	get_y_scale() const;	// return the magnitude scale of our y coord output
		float	get_rotation() const;	// return our rotation component (in radians)

		
		tu_float get_raw_x() const { return m_[0][2]; }
		tu_float get_raw_y() const { return m_[1][2]; }
	};

	inline void matrix::set_identity()
	// Set the matrix to identity.
	{
		memset(&m_[0], 0, sizeof(m_));
		m_[0][0] = 1;
		m_[1][1] = 1;
	}

	inline void	matrix::transform(point* result, const point& p) const
	// Transform point 'p' by our matrix.  Put the result in
	// *result.
	{
		assert(result);
		assert(&p != result);

		result->m_x = m_[0][0] * p.m_x + m_[0][1] * p.m_y + m_[0][2];
		result->m_y = m_[1][0] * p.m_x + m_[1][1] * p.m_y + m_[1][2];
	}


	inline void matrix::concatenate_translation(float tx, float ty)
	// Concatenate a translation onto the front of our
	// matrix.  When transforming points, the translation
	// happens first, then our original xform.
	{
		m_[0][2] += m_[0][0] * tx + m_[0][1] * ty;
		m_[1][2] += m_[1][0] * tx + m_[1][1] * ty;
	}


	inline void matrix::concatenate_scale(float scale)
	// Concatenate a uniform scale onto the front of our
	// matrix.  When transforming points, the scale
	// happens first, then our original xform.
	{
		m_[0][0] *= scale;
		m_[0][1] *= scale;
		m_[1][0] *= scale;
		m_[1][1] *= scale;
	}

	inline void matrix::concatenate(const matrix& m)
	// Concatenate m's transform onto ours.  When
	// transforming points, m happens first, then our
	// original xform.
	{
		matrix	t;
		t.m_[0][0] = m_[0][0] * m.m_[0][0] + m_[0][1] * m.m_[1][0];
		t.m_[1][0] = m_[1][0] * m.m_[0][0] + m_[1][1] * m.m_[1][0];
		t.m_[0][1] = m_[0][0] * m.m_[0][1] + m_[0][1] * m.m_[1][1];
		t.m_[1][1] = m_[1][0] * m.m_[0][1] + m_[1][1] * m.m_[1][1];
		t.m_[0][2] = m_[0][0] * m.m_[0][2] + m_[0][1] * m.m_[1][2] + m_[0][2];
		t.m_[1][2] = m_[1][0] * m.m_[0][2] + m_[1][1] * m.m_[1][2] + m_[1][2];

		*this = t;
	}

	inline void matrix::transform_vector(point* result, const point& v) const
	// Transform vector 'v' by our matrix. Doesn't apply translation.
	// Put the result in *result.
	{
		assert(result);
		assert(&v != result);

		result->m_x = m_[0][0] * v.m_x + m_[0][1] * v.m_y;
		result->m_y = m_[1][0] * v.m_x + m_[1][1] * v.m_y;
	}

	inline void matrix::transform_by_inverse(point* result, const point& p) const
	// Transform point 'p' by the inverse of our matrix.  Put result in *result.
	{
		// @@ TODO optimize this!
		matrix	m;
		m.set_inverse(*this);
		m.transform(result, p);
	}

	inline void matrix::transform_by_inverse(rect* bound) const
	// Transform point 'p' by the inverse of our matrix.  Put result in *result.
	{
		// @@ TODO optimize this!
		matrix	m;
		m.set_inverse(*this);
		m.transform(bound);
	}

	inline float matrix::get_determinant() const
	// Return the determinant of the 2x2 rotation/scale part only.
	{
		return m_[0][0] * m_[1][1] - m_[1][0] * m_[0][1];
	}

	inline float matrix::get_x_scale() const
	{
		float	scale = sqrtf(m_[0][0] * m_[0][0] + m_[0][1] * m_[0][1]);

		// Are we turned inside out?
		if (get_determinant() < 0.f)
		{
			scale = -scale;
		}

		return scale;
	}

	inline float matrix::get_y_scale() const
	{
		return sqrtf(m_[1][1] * m_[1][1] + m_[1][0] * m_[1][0]);
	}

	inline float matrix::get_max_scale() const
	// Return the maximum scale factor that this transform
	// applies.  For assessing scale, when determining acceptable
	// errors in tesselation.
	{
		// @@ not 100% sure what the heck I'm doing here.  I
		// think this is roughly what I want; take the max
		// length of the two basis vectors.
		float	basis0_length2 = m_[0][0] * m_[0][0] + m_[0][1] * m_[0][1];
		float	basis1_length2 = m_[1][0] * m_[1][0] + m_[1][1] * m_[1][1];
		float	max_length2 = fmax(basis0_length2, basis1_length2);
		return sqrtf(max_length2);
	}


	//
	// rect: rectangle type, used by render handler
	//


	struct rect
	{
		float	m_x_min, m_x_max, m_y_min, m_y_max;

		void	read(stream* in);
		void	print() const;
		bool	point_test(float x, float y) const;
		bool	bound_test(const rect& bound) const;
		void	set_to_point(float x, float y);
		void	set_to_point(const point& p);
		void	expand_to_point(float x, float y);
		void	expand_to_point(const point& p);
		void	expand_to_rect(const rect& r);
		float	width() const { return m_x_max - m_x_min; }
		float	height() const { return m_y_max - m_y_min; }

		point	get_corner(int i) const;

		void	enclose_transformed_rect(const matrix& m, const rect& r);

		void	set_lerp(const rect& a, const rect& b, float t);

		void twips_to_pixels();
		void pixels_to_twips();
	};

	inline bool rect::point_test(float x, float y) const
	// Return true if the specified point is inside this rect.
	{
		if (x < m_x_min
		    || x > m_x_max
		    || y < m_y_min
		    || y > m_y_max)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	
	inline bool rect::bound_test(const rect& bound) const
	// Return true if the specified rect intersect this rect
	{
		if (m_x_max < bound.m_x_min
			|| m_x_min > bound.m_x_max 
			|| m_y_min > bound.m_y_max 
			|| m_y_max < bound.m_y_min)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	inline	void rect::set_to_point(float x, float y)
	{
		m_x_min = m_x_max = x;
		m_y_min = m_y_max = y;
	}

	inline void rect::set_to_point(const point& p)
	{
		set_to_point(p.m_x, p.m_y);
	}

	inline void	rect::expand_to_point(float x, float y)
	// Expand this rectangle to enclose the given point.
	{
		m_x_min = fmin(m_x_min, x);
		m_y_min = fmin(m_y_min, y);
		m_x_max = fmax(m_x_max, x);
		m_y_max = fmax(m_y_max, y);
	}

	inline void	rect::expand_to_point(const point& p)
	{
		expand_to_point(p.m_x, p.m_y);
	}

	inline void rect::expand_to_rect(const rect& r)
	// Expand to enclose the given rectangle.
	{
		expand_to_point(r.m_x_min, r.m_y_min);
		expand_to_point(r.m_x_max, r.m_y_max);
	}

	inline	point	rect::get_corner(int i) const
	// Get one of the rect verts.
	{
		assert(i >= 0 && i < 4);
		return point(
			(i == 0 || i == 3) ? m_x_min : m_x_max,
			(i < 2) ? m_y_min : m_y_max);
	}

	//
	// cxform: color transform type, used by render handler
	//
	struct cxform
	{
		tu_float	m_[4][2];	// [RGBA][mult, add]

		cxform()
		// Initialize to identity transform.
		{
			m_[0][0] = 1;
			m_[1][0] = 1;
			m_[2][0] = 1;
			m_[3][0] = 1;
			m_[0][1] = 0;
			m_[1][1] = 0;
			m_[2][1] = 0;
			m_[3][1] = 0;
		}
		
		void	concatenate(const cxform& c);
		rgba	transform(const rgba in) const;
		void	read_rgb(stream* in);
		void	read_rgba(stream* in);
		void	clamp();  // Force component values to be in range.
		void	print() const;

		static cxform	identity;
	};


	//
	// filter: drop shadow, glow, etc
	//
	struct filter
	{
		enum ID
		{
			DROP_SHADOW,
			BLUR,
			GLOW
		};

		// Filter parameters
		struct drop_shadow_params
		{
			Uint8 drop_shadow_color[4]; // RGBA Color of the shadow
			//float blur_x;	// Horizontal blur amount
			//float blur_y;	// Vertical blur amount
			float angle;	// Radian angle of the drop shadow
			float distance;	// Distance of the drop shadow
			float sub_strength;	// hack, must be FIXED8 Strength of the drop shadow
			bool inner_shadow;	// Inner shadow mode
			bool knockout;	// Knockout mode
			bool composite_source;	// Composite source Always 1
			int mode; // ShowObject: (mode & 32) != 0
			int strength; // passes
		};

		struct glow_params
		{
			Uint8 glow_color[4];	//RGBA Color of the shadow
			//float blur_x;	// Horizontal blur amount
			//float blur_y;	// Vertical blur amount
			float sub_strength;	// hack, must be FIXED8 Strength of the drop shadow
			bool inner_glow;	// Inner glow mode
			bool knockout;	// Knockout mode
			bool composite_source;	// Composite source Always 1
			int strength; // passes
		};

		struct blur_params
		{
			float blur_x; // Horizontal blur amount
			float blur_y; // Vertical blur amount
			int passes;	// Number of blur passes
		};

		ID m_id;

		union
		{
			drop_shadow_params drop_shadow;
			glow_params glow;
		};

		blur_params blur;
	};

	struct blend_mode
	{
		enum id
		{
			NORMAL = 0,
			NORMAL1 = 1,
			LAYER = 2,
			MULTIPLY = 3,
			SCREEN = 4,
			LIGHTEN = 5,
			DARKEN = 6,
			ADD = 7,
			SUBTRACT = 8,
			DIFF = 9,
			INVERT = 10,
			ALPHA = 11,
			ERASE = 12,
			OVERLAY = 13,
			HARDLIGHT = 14,
			COUNT
		};
	};

	//
	// effect: blend mode, filters, etc
	//
	struct effect
	{
		effect() :
			m_blend_mode(blend_mode::NORMAL)
		{
		}

		blend_mode::id m_blend_mode;
		array<filter> m_filters;
		
		static effect identity;
	};

	//
	// texture and render callback handler.
	//

	// Your render_handler creates bitmap_info's for gameswf.  You
	// need to subclass bitmap_info in order to add the
	// information and functionality your app needs to render
	// using textures.
	struct bitmap_info : public ref_counted
	{
		enum filter_mode
		{
			FILTER_MODE_NEAREST = 0,
			FILTER_MODE_LINEAR,
			FILTER_MODE_NEAREST_MIPMAP_NEAREST,
			FILTER_MODE_LINEAR_MIPMAP_NEAREST,
			FILTER_MODE_NEAREST_MIPMAP_LINEAR,
			FILTER_MODE_LINEAR_MIPMAP_LINEAR
		};

		virtual void layout() {};
		virtual void unlayout() {};
		virtual void activate() {};
		virtual void set_writable() {};
		virtual void* lock() { return NULL; }
		virtual void unlock() {};
		virtual void set_texture(video_texture* texture) {}
		virtual int get_width() const { return 0; }
		virtual int get_height() const { return 0; }
		virtual void set_min_filter(filter_mode mode) {}
		virtual void set_mag_filter(filter_mode mode) {}
		virtual bool is_layout() const { return false; }
		virtual int get_internal_width() const { return get_width(); }
		virtual int get_internal_height() const { return get_height(); }
	};

	// You must define a subclass of render_handler, and pass an
	// instance to set_render_handler().
	struct render_handler
	{
		enum buffer_mode
		{
			BUFFER_MODE_REUSE = 0,
			BUFFER_MODE_APPEND = 1
		};

		render_handler() :
			m_text_pixel_alignment(true)
		{
		}
		virtual ~render_handler() {}

		// Your handler should return these with a ref-count of 0.  (@@ is that the right policy?)
		virtual bitmap_info*	create_bitmap_info_empty() = 0;	// used when DO_NOT_LOAD_BITMAPS is set
		virtual bitmap_info*	create_bitmap_info_alpha(int w, int h, unsigned char* data) = 0;
		virtual bitmap_info*	create_bitmap_info_rgb(image_rgb* im) = 0;
		virtual bitmap_info*	create_bitmap_info_rgba(image_rgba* im) = 0;
		virtual bitmap_info*	create_bitmap_info_texture(video_texture* texture) = 0;
		virtual bitmap_info*	create_bitmap_info_native(int w, int h, const membuf* data) = 0;
		virtual bitmap_info*	create_bitmap_info_target(int w, int h) = 0;
		virtual video_handler*	create_video_handler() = 0;

		// Bracket the displaying of a frame from a movie.
		// Fill the background color, and set up default
		// transforms, etc.
		virtual void	begin_display(
			rgba background_color,
			int viewport_x0, int viewport_y0,
			int viewport_width, int viewport_height,
			float x0, float x1, float y0, float y1) = 0;
		virtual void	end_display() = 0;

		virtual void	set_blend_mode(blend_mode::id mode) = 0;
		virtual void	set_target(bitmap_info* target) = 0;
		virtual void	clear_frame_buffer() = 0;
		virtual void	read_frame_buffer(int x, int y, int width, int height, unsigned char* pixels) = 0;
		virtual void	set_wire_frame(bool enabled) = 0;
		virtual void	set_buffer_capacity(buffer_mode mode, int capacity) = 0;
		virtual void	set_context(player_context* context) = 0;
		virtual void	set_render_cache(render_cache* cache) = 0;

		// Geometric and color transforms for mesh and line_strip rendering.
		virtual void	set_matrix(const matrix& m) = 0;
		virtual void	set_cxform(const cxform& cx) = 0;

		// Draw triangles using the current fill-style 0.
		// Clears the style list after rendering.
		//
		// coords is a list of (x,y) coordinate pairs, in
		// triangle-strip order.  The type of the array should
		// be Sint16[vertex_count*2]
		virtual void	draw_mesh_strip(const void* coords, int vertex_count) = 0;
		// As above, but coords is in triangle list order.
		virtual void	draw_triangle_list(const void* coords, int vertex_count, const unsigned short* indexes, int index_count) = 0;

		// Draw a line-strip using the current line style.
		// Clear the style list after rendering.
		//
		// Coords is a list of (x,y) coordinate pairs, in
		// sequence.  Each coord is a 16-bit signed integer.
		virtual void	draw_line_strip(const void* coords, int vertex_count) = 0;

		// draw render cache
		virtual void	draw(render_cache* cache) = 0;

		// Set line and fill styles for mesh & line_strip
		// rendering.
		enum bitmap_wrap_mode
		{
			WRAP_REPEAT,
			WRAP_CLAMP
		};
		virtual void	fill_style_disable(int fill_side) = 0;
		virtual void	fill_style_color(int fill_side, const rgba& color) = 0;
		virtual void	fill_style_bitmap(int fill_side, bitmap_info* bi, const matrix& m, bitmap_wrap_mode wm) = 0;

		virtual void	line_style_disable() = 0;
		virtual void	line_style_color(rgba color) = 0;
		virtual void	line_style_width(float width) = 0;

		// Special function to draw a rectangular bitmap;
		// intended for textured glyph rendering.  Ignores
		// current transforms.
		virtual void	draw_bitmap(
			const matrix&		m,
			bitmap_info*	bi,
			const rect&		coords,
			const rect&		uv_coords,
			rgba			color) = 0;
		virtual void	set_antialiased(bool enable) = 0;

		virtual void begin_submit_mask() = 0;
		virtual void end_submit_mask() = 0;
		virtual void disable_mask() = 0;
		virtual void flush() {};

		// Mouse cursor handling.
		enum cursor_type
		{
			SYSTEM_CURSOR,
			ACTIVE_CURSOR
		};
		virtual void set_cursor(cursor_type cursor) {}
		virtual bool is_visible(const rect& bound) = 0;
		virtual void open() = 0;
		virtual void reset() {}
		virtual void set_orientation(orientation_mode mode) {}
		virtual orientation_mode get_orientation() { return ORIENTATION_0; }

		virtual void begin_display_callback() {}
		virtual void end_display_callback() {}

		void set_text_pixel_alignment(bool enabed) { m_text_pixel_alignment = enabed; }

	protected:
		bool m_text_pixel_alignment;
	};

	struct render_cache
	{
		struct entry
		{
			bitmap_info* bi;
			unsigned int color;
			int vertex_start;
			int vertex_count;
			int index_start;
			int index_count;
		};

		render_cache() :
			m_gtc_timestamp(-1),
			m_bgtc_timestamp(-1)
		{
		}

		bool is_valid(character* ch);

		void record(bitmap_info* bi, const vector3df* positions, const vector2df* texcoords, unsigned int color, int vertex_count)
		{
			entry entry;

			entry.vertex_start = m_coords.size();
			entry.vertex_count = vertex_count;
			entry.index_start = 0;
			entry.index_count = 0;

			m_coords.resize(m_coords.size() + vertex_count);
			memcpy(&m_coords[entry.vertex_start], positions, vertex_count * sizeof(vector3df));

			m_uv_coords.resize(m_uv_coords.size() + vertex_count);
			memcpy(&m_uv_coords[entry.vertex_start], texcoords, vertex_count * sizeof(vector2df));

			entry.bi = bi;
			entry.color = color;
			m_dlist.push_back(entry);
		}

		void record(bitmap_info* bi, const vector3df* positions, int position_stride, const vector2df* texcoords, int texcoord_stride, unsigned int color, int vertex_count)
		{
			entry entry;

			entry.vertex_start = m_coords.size();
			entry.vertex_count = vertex_count;
			entry.index_start = 0;
			entry.index_count = 0;

			m_coords.resize(m_coords.size() + vertex_count);
			m_uv_coords.resize(m_uv_coords.size() + vertex_count);
			vector3df* destpos = &m_coords[entry.vertex_start];
			vector2df* desttex = &m_uv_coords[entry.vertex_start];
			for(const vector3df* endpos = positions + vertex_count * position_stride; positions != endpos; ++destpos, ++desttex)
			{
				*destpos = *positions;
				*desttex = *texcoords;
				positions = (const vector3df*)((const char*)positions + position_stride);
				texcoords = (const vector2df*)((const char*)texcoords + texcoord_stride);
			}

			entry.bi = bi;
			entry.color = color;
			m_dlist.push_back(entry);
		}

		void record(bitmap_info* bi, const vector3df* positions, const vector2df* texcoords, unsigned int color, int vertex_count, unsigned short* indexes, int index_count)
		{
			entry entry;

			entry.vertex_start = m_coords.size();
			entry.vertex_count = vertex_count;
			entry.index_start = m_indexes.size();
			entry.index_count = index_count;

			m_indexes.resize(m_indexes.size() + index_count);
			memcpy(&m_indexes[entry.index_start], indexes, index_count * sizeof(unsigned short));

			m_coords.resize(m_coords.size() + vertex_count);
			memcpy(&m_coords[entry.vertex_start], positions, vertex_count * sizeof(vector3df));

			m_uv_coords.resize(m_uv_coords.size() + vertex_count);
			memcpy(&m_uv_coords[entry.vertex_start], texcoords, vertex_count * sizeof(vector2df));

			entry.bi = bi;
			entry.color = color;
			m_dlist.push_back(entry);
		}

		void record(bitmap_info* bi, const vector3df* positions, int position_stride, const vector2df* texcoords, int texcoord_stride, unsigned int color, int vertex_count, unsigned short* indexes, int index_count)
		{
			entry entry;

			entry.vertex_start = m_coords.size();
			entry.vertex_count = vertex_count;
			entry.index_start = m_indexes.size();
			entry.index_count = index_count;

			m_indexes.resize(m_indexes.size() + index_count);
			memcpy(&m_indexes[entry.index_start], indexes, index_count * sizeof(unsigned short));

			m_coords.resize(m_coords.size() + vertex_count);
			m_uv_coords.resize(m_uv_coords.size() + vertex_count);
			vector3df* destpos = &m_coords[entry.vertex_start];
			vector2df* desttex = &m_uv_coords[entry.vertex_start];
			for(const vector3df* endpos = positions + vertex_count * position_stride; positions != endpos; ++destpos, ++desttex)
			{
				*destpos = *positions;
				*desttex = *texcoords;
				positions = (const vector3df*)((const char*)positions + position_stride);
				texcoords = (const vector2df*)((const char*)texcoords + texcoord_stride);
			}

			entry.bi = bi;
			entry.color = color;
			m_dlist.push_back(entry);
		}

		void clear()
		{
			m_dlist.clear();
			m_coords.clear();
			m_uv_coords.clear();
		}

		void reset()
		{
			m_gtc_timestamp = -1;
			m_bgtc_timestamp = -1;
		}

		Uint64 m_gtc_timestamp;
		Uint64 m_bgtc_timestamp;
		array<entry> m_dlist;
		array<vector3df> m_coords;
		array<vector2df> m_uv_coords;
		array<unsigned short> m_indexes;
	};

	// Some optional helpers.
	namespace tools
	{
		struct process_options
		{
			bool	m_zip_whole_file;	// @@ not implemented yet (low priority?)
			bool	m_remove_image_data;	// removes existing image data; leaves minimal placeholder tags
			bool	m_remove_font_glyph_shapes;

			process_options()
				:
				m_zip_whole_file(false),
				m_remove_image_data(false),
				m_remove_font_glyph_shapes(false)
			{}
		};

		// Copy tags from *in to *out, applying the given
		// options.  *in should be a SWF-format stream.	 The
		// output will be a SWF-format stream.
		//
		// Returns 0 on success, or a non-zero error-code on
		// failure.
		int	process_swf(gameswf::tu_file* swf_out, gameswf::tu_file* swf_in, const process_options& options);
	}

}	// namespace gameswf


#endif // GAMESWF_H


// Local Variables:
// mode: C++
// c-basic-offset: 8
// tab-width: 8
// indent-tabs-mode: t
// End:
