#pragma once
// gameswf_impl.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation code for the gameswf SWF player library.


#ifndef GAMESWF_IMPL_H
#define GAMESWF_IMPL_H


#include "gameswf/gameswf.h"
#include "gameswf/gameswf_action.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_render.h"
#include "base/container.h"
#include "base/utility.h"
#include "base/smart_ptr.h"
#include <stdarg.h>


namespace jpeg { struct input; }


namespace gameswf
{
	struct action_buffer;
	struct bitmap_character_def;
	struct bitmap_info;
	struct character;
	struct character_def;
	struct display_info;
	struct execute_tag;
	struct font;
	struct root;
	struct movie_definition_sub;

	struct stream;
	struct swf_event;

	struct swf_event
	{
		// NOTE: DO NOT USE THESE AS VALUE TYPES IN AN
		// array<>!  They cannot be moved!  The private
		// operator=(const swf_event&) should help guard
		// against that.

		event_id	m_event;
		as_value	m_method;

		swf_event() {}

	private:

		// DON'T USE THESE
		swf_event(const swf_event& s) { assert(0); }
		void	operator=(const swf_event& s) { assert(0); }
	};


	// For characters that don't store unusual state in their instances.
	struct generic_character : public character
	{
		smart_ptr<character_def>	m_def;

		// Unique id of a gameswf resource
		enum { m_class_id = AS_GENERIC_CHARACTER };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return character::is(class_id);
		}

		generic_character(player* player, character_def* def, character* parent, int id) :
			character(player, parent, id, AS_GENERIC_CHARACTER),
			m_def(def)
		{
			assert(m_def != NULL);
			m_need_advance = false;
		}

		~generic_character()
		{
		}

		virtual character_def* get_character_def() { return m_def.get_ptr();	}

		virtual void	display()
		{
			if (is_visible() == false)
			{
				// We're invisible, so don't display!
				return;
			}

			m_def->display(this);	// pass in transform info

			if(has_display_callback())
			{
				do_display_callback();
			}
		}

		virtual character*	get_topmost_mouse_entity(float x, float y)
		{
			assert(get_visible());	// caller should check this.

			const matrix&	m = get_matrix();
			point	p;
			m.transform_by_inverse(&p, point(x, y));

			if (m_def->point_test_local(p.m_x, p.m_y))
			{
				// The mouse is inside the shape.
				return this;
			}
			return NULL;
		}

		virtual bool	hit_test(float x, float y)
		{
			return get_topmost_mouse_entity(x, y) != NULL;
		}
	};


	struct bitmap_character_def : public character_def
	{
		bitmap_character_def(player* player) :
			character_def(player)
		{
		}

		virtual gameswf::bitmap_info*	get_bitmap_info() = 0;
	};

	// Bitmap character
	struct bitmap_character : public bitmap_character_def
	{
		bitmap_character(player* player, bitmap_info* bi, rect* bounds = NULL) :
			bitmap_character_def(player),
			m_bitmap_info(bi)
		{
			m_bounds.m_x_min = m_bounds.m_y_min = 0;
			m_bounds.m_x_max = PIXELS_TO_TWIPS(m_bitmap_info->get_width());
			m_bounds.m_y_max = PIXELS_TO_TWIPS(m_bitmap_info->get_height());

			if(bounds != NULL)
				m_bounds = *bounds;
		}

		// Unique id of a gameswf resource
		enum { m_class_id = AS_BITMAP };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return bitmap_character_def::is(class_id);
		}

		bool	point_test_local(float x, float y)
		// Return true if the specified point is on the interior of our shape.
		// Incoming coords are local coords.
		{
			if (m_bounds.point_test(x, y))
			{
				return true;
			}
			return false;
		}

		virtual void get_bound(rect* bound)
		{
			*bound = m_bounds;
		}

		virtual void	display(character* ch)
		{
			// show whole picture
			rect uv_coords;
			uv_coords.m_x_min = 0.0f;
			uv_coords.m_x_max = 1.0f;
			uv_coords.m_y_min = 0.0f;
			uv_coords.m_y_max = 1.0f;

			cxform cx = ch->get_world_cxform();
			rgba color = cx.transform(gameswf::rgba());
			matrix m = ch->get_world_matrix();
			render::draw_bitmap(m, m_bitmap_info.get_ptr(), m_bounds,	uv_coords, color);
		}

		gameswf::bitmap_info*	get_bitmap_info()
		{
			return m_bitmap_info.get_ptr();
		}

		private:
			smart_ptr<gameswf::bitmap_info>	m_bitmap_info;
			rect m_bounds;
	};

	// Execute tags include things that control the operation of
	// the movie.  Essentially, these are the events associated
	// with a frame.
	struct execute_tag
	{
		virtual ~execute_tag() {}
		virtual void	execute(character* m) {}
		virtual void	execute_state(character* m) {}
		virtual void	execute_state_reverse(character* m, int frame) { execute_state(m); }
		virtual bool	is_remove_tag() const { return false; }
		virtual bool	is_action_tag() const { return false; }
		virtual uint32	get_depth_id_of_replace_or_add_tag() const { return static_cast<uint32>(-1); }
		virtual int	get_depth() const { return -1; }
		virtual uint32	get_action_offset() const { return static_cast<uint32>(-1); }
		virtual bool	is_place_tag() const { return false; }
	};

		//
	// place_object_2
	//

	struct place_object_2 : public execute_tag
	{
		unsigned char m_name_offset;
		unsigned char m_cxform_offset;
		unsigned char m_matrix_offset;
		unsigned char m_effect_offset;
		unsigned char m_event_handlers_offset;
		enum place_type {
			PLACE,
			MOVE,
			REPLACE,
		};
		unsigned char m_place_type;
		Uint16	m_ratio;
		Uint16	m_depth;
		Uint16	m_character_id;
		Uint16	m_clip_depth;
		Uint16	m_tag_type;
		cxform*	m_previous_cxform;
		matrix*	m_previous_matrix;

		place_object_2() :
			m_name_offset(0),
			m_cxform_offset(0),
			m_matrix_offset(0),
			m_effect_offset(0),
			m_event_handlers_offset(0),
			m_place_type(PLACE),
			m_ratio(0),
			m_depth(0),
			m_character_id(0),
			m_clip_depth(0),
			m_tag_type(0),
			m_previous_cxform(NULL),
			m_previous_matrix(NULL)
		{
		}

		~place_object_2()
		{
			if(get_character_name() != NULL)
			{
				get_character_name()->clear();
			}
			if(get_effect() != NULL)
			{
				get_effect()->m_filters.clear();
				get_effect()->m_filters.release_buffer();
			}
			if(get_event_handlers() != NULL)
			{
				array<swf_event*>* handlers = get_event_handlers();
				for (int i = 0, n = handlers->size(); i < n; i++)
				{
					swfdelete ((*handlers)[i]);
				}
				handlers->clear();
				handlers->release_buffer();
			}
		}

		inline tu_string* get_character_name() const
		{
			return m_name_offset > 0 ? ((tu_string*)((char*)this + m_name_offset)) : NULL;
		}

		inline matrix* get_matrix() const
		{
			return m_matrix_offset > 0 ? ((matrix*)((char*)this + m_matrix_offset)) : NULL;
		}

		inline cxform* get_cxform() const
		{
			return m_cxform_offset > 0 ? ((cxform*)((char*)this + m_cxform_offset)) : NULL;
		}

		inline effect* get_effect() const
		{
			return m_effect_offset > 0 ? ((effect*)((char*)this + m_effect_offset)) : NULL;
		}

		inline array<swf_event*>* get_event_handlers() const
		{
			return m_event_handlers_offset > 0 ? ((array<swf_event*>*)((char*)this + m_event_handlers_offset)) : NULL;
		}

		static place_object_2* read(player* player, stream* in, int tag_type, int movie_version, movie_definition_sub* m);
		virtual void execute(character* m);
		virtual void execute_state(character* m);
		virtual void execute_state_reverse(character* m, int frame);
		virtual uint32 get_depth_id_of_replace_or_add_tag() const;
		virtual int	get_depth() const { return m_depth; }
		virtual bool is_place_tag() const { return true; }
	};

	//
	// Loader callbacks.
	//

	// Register a loader function for a certain tag type.  Most
	// standard tags are handled within gameswf.  Host apps might want
	// to call this in order to handle special tag types.
	void	register_tag_loader(int tag_type, loader_function lf);


	// Tag loader functions.
	void	null_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	set_background_color_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	jpeg_tables_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_bits_jpeg_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_bits_jpeg2_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_bits_jpeg3_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_shape_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_shape_morph_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_font_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_font_info_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_text_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_edit_text_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	place_object_2_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_bits_lossless_2_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_native_image_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	sprite_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	end_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	remove_object_2_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	do_action_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	button_character_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	frame_label_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	export_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	import_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_sound_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	start_sound_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	button_sound_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	do_init_action_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_video_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	video_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	sound_stream_head_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	sound_stream_block_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_file_attribute_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_font_alignzones(stream* in, int tag_type, movie_definition_sub* m);
	void	define_csm_textsetting_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_metadata_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_debug_id_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_enable_debugger_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_tabindex_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_abc_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	symbol_class_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_scene_loader(stream* in, int tag_type, movie_definition_sub* m);
	void	define_font_name(stream* in, int tag_type, movie_definition_sub* m);

}	// end namespace gameswf


#endif // GAMESWF_IMPL_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
