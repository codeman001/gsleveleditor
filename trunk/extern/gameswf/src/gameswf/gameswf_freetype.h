#pragma once
// gameswf_freetype.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Truetype font rasterizer based on freetype library

#ifndef GAMESWF_FREETYPE_H
#define GAMESWF_FREETYPE_H

#include "base/tu_config.h"

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_shape.h"
#include "gameswf/gameswf_canvas.h"
#include "gameswf/gameswf_texture_cache.h"
#include "base/membuf.h"

#if TU_CONFIG_LINK_TO_FREETYPE == 1

#include <ft2build.h>
#include FT_OUTLINE_H
#include FT_FREETYPE_H
#include FT_GLYPH_H
#define FT_CONST 

namespace gameswf
{
	struct glyph_provider;

	// helper

	struct glyph_entity
	{
		smart_ptr<bitmap_info> m_bi;
		float m_advance;
		rect m_bounds;
	};

	struct face_entity : public ref_counted
	{
		tu_string m_filename;
		membuf* m_fontdata;
		FT_Face m_face;
		hash<int, glyph_entity*> m_ge;	// <code, glyph_entity>

		face_entity(FT_Face face, tu_string& filename) :
			m_face(face),
			m_fontdata(NULL)
		{
			m_filename = filename;
		}

		face_entity(FT_Face face, membuf* fontdata, tu_string& filename) :
			m_face(face),
			m_fontdata(fontdata)
		{
			assert(face);
			assert(fontdata);

			m_filename = filename;
		}

		~face_entity()
		{
			FT_Done_Face(m_face);
			for (hash<int, glyph_entity*>::iterator it = m_ge.begin(); it != m_ge.end(); ++it)
			{
				swfdelete (it->second);
			}
			if(m_fontdata)
				swfdelete (m_fontdata);
		}

	};

	struct glyph_texture_cache : public texture_cache
	{
	public:
		struct filter_info
		{
			unsigned char stroke;
			unsigned char blurx;
			unsigned char blury;
		};

		glyph_texture_cache(glyph_provider* provider, int width, int height, int bpp) :
			texture_cache(width, height, bpp),
			m_glyph_provider(provider)
		{
			//get_bitmap_info()->set_min_filter(bitmap_info::FILTER_MODE_NEAREST);
			//get_bitmap_info()->set_mag_filter(bitmap_info::FILTER_MODE_NEAREST);
		}

		bool add_glyph_region(Uint16 code, void* fe, int fontsize, filter_info& fi, bool reuse = false);
		void get_glyph_region(Uint16 code, void* fe, int fontsize, filter_info& fi, rect& bounds);
		inline key make_key(Uint16 code, void* fe, unsigned short fontsize, filter_info& fi)
		{
			key k;
			k.major = (((Uint64)fontsize & 0xff) << 48) | ((Uint64)code << 32) | (((unsigned int)fe) & 0xffffffff);
			k.minor = (fi.blurx << 16) | (fi.blury << 8) | fi.stroke;
			return k;
		}
	
	protected:
		array<uint8> m_working_buffer;
		glyph_provider* m_glyph_provider;
	};

	struct glyph_provider
	{
		glyph_provider(int cache_width, int cache_height, bool loadFontsInMemory, float scale);
		~glyph_provider();
		
		face_entity* get_face_entity(const tu_string& fontname,
			bool is_bold, bool is_italic);

		void set_file_cache_enabled(bool enabled)
		{
			m_file_cache_enabled = enabled;
		}

		void clear()
		{
			m_face_entity.clear();

			if(m_glyph_texture_cache != NULL)
				m_glyph_texture_cache->reset();
		}

		glyph_texture_cache* get_texture_cache() const
		{
			return m_glyph_texture_cache;
		}

		const string_hash< smart_ptr<face_entity> >& get_face_entities()
		{
			return m_face_entity;
		}

		float get_scale() const
		{
			return m_scale;
		}

		FT_Library get_library() const
		{
			return m_lib;
		}

		// callbacks
		static int move_to_callback(FT_CONST FT_Vector* vec, void* ptr);
		static int line_to_callback(FT_CONST FT_Vector* vec, void* ptr);
		static int conic_to_callback(FT_CONST FT_Vector* ctrl, FT_CONST FT_Vector* vec, 
			void* ptr);
		static int cubic_to_callback(FT_CONST FT_Vector* ctrl1, FT_CONST FT_Vector* ctrl2,
			FT_CONST FT_Vector* vec, void* ptr);

		image_alpha* draw_bitmap(const FT_Bitmap& bitmap);
		bitmap_info* get_char_image(Uint16 code, 
			const tu_string& fontname, bool is_bold, bool is_italic, int fontsize,
			rect* bounds, float* advance);
		shape_character_def* get_char_def(Uint16 code,
			const char* fontname, bool is_bold, bool is_italic, int fontsize,
			rect* bounds, float* advance);

	private:
		
		FT_Library	m_lib;
		float m_scale;
		bool m_file_cache_enabled;
		smart_ptr<canvas> m_canvas;
		string_hash< smart_ptr<face_entity> > m_face_entity;
		glyph_texture_cache* m_glyph_texture_cache;
	};

}

#else	// TU_CONFIG_LINK_TO_FREETYPE == 0

namespace gameswf
{

	struct glyph_provider
	{
		glyph_provider(void*	lib) {}
		~glyph_provider() {}
		
		bitmap_info* get_char_image(Uint16 code, 
			const tu_string& fontname, bool is_bold, bool is_italic, int fontsize,
			rect* bounds, float* advance)
		{
			return NULL;
		}

	};
}

#endif		// TU_CONFIG_LINK_TO_FREETYPE

#endif	// GAMESWF_FREETYPE_H
