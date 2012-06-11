#pragma once
// gameswf_bitmapfont.h

// bitmap A font type for gameswf.


#ifndef GAMESWF_BITMAPFONT_H
#define GAMESWF_BITMAPFONT_H


#include "base/container.h"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_texture_cache.h"

namespace gameswf
{
	struct bitmap_glyph_data
	{
		int m_pitch;
		int m_width;
		int m_height;
		unsigned char* m_pixels;
	};

	struct bitmap_glyph_metrics
	{
		int m_xanchor;
		int m_yanchor;
		int m_width;
		int m_height;
		int m_advance;
	};

	struct bitmap_glyph_provider;
	struct glyph_entity;

	struct bitmap_font_entity : public ref_counted
	{
		bitmap_glyph_provider* m_provider;
		tu_string m_filename;
		hash<int, glyph_entity*> m_ge;	// <code, glyph_entity>
		float m_height;

		bitmap_font_entity(bitmap_glyph_provider* provider) :
			m_provider(provider)
		{
		}
		virtual ~bitmap_font_entity();

		float get_units_per_em() const
		{
			return 1024.0f;
		}

		float get_scale() const
		{
			return get_units_per_em() / get_height();
		}

		float get_height() const
		{
			return PIXELS_TO_TWIPS(m_height);
		}
		bitmap_info* get_char_image(Uint16 code, int fontsize, rect* bounds, float* advance);
		virtual bool get_char_image(bitmap_glyph_data* bgd, Uint16 code, int fontsize, bitmap_glyph_metrics* metrics) = 0;

		static void copy_argb(unsigned char* src, int src_x, int src_y, int src_pitch, unsigned char* dst, int dst_pitch, int dst_height);
	};

	struct default_bitmap_font_entity : bitmap_font_entity
	{
	public:
		enum
		{
			VERSION = 0,
			VERSION_OFFSET = 4,
			TYPE_OFFSET = 8,
			GLYPH_COUNT_OFFSET = 12,
			CELL_WIDTH_OFFSET = 16,
			CELL_HEIGHT_OFFSET = 20,
			BASELINE_OFFSET = 24,
			LINE_HEIGHT_OFFSET = 28,
			CHAR_SPACE_OFFSET = 32,
			FIRST_CHAR_OFFSET = 36,
			TABLE_OFFSET = 40,
			
			GLYPH_MINX_OFFSET = 0,
			GLYPH_MAXX_OFFSET = 2,
			GLYPH_PIXEL_OFFSET = 4
		};

		default_bitmap_font_entity(bitmap_glyph_provider* provider, const gameswf::tu_string& filename);
		virtual ~default_bitmap_font_entity();
		virtual bool get_char_image(gameswf::bitmap_glyph_data* bgd, gameswf::Uint16 code, int fontsize, gameswf::bitmap_glyph_metrics* metrics);

		inline Uint32 read_uint32(int offset)
		{
			const Uint8* font_header = (const Uint8*)m_header.data();
			return (font_header[offset] << 24) | (font_header[offset + 1] << 16) | (font_header[offset + 2] << 8) | font_header[offset + 3];
		}

		inline Uint16 read_uint16(int offset)
		{
			const Uint8* font_header = (const Uint8*)m_header.data();
			return (font_header[offset] << 8) | font_header[offset + 1];
		}

		inline Uint32 read_data_uint32(const Uint8* font_data, int offset)
		{
			return (font_data[offset] << 24) | (font_data[offset + 1] << 16) | (font_data[offset + 2] << 8) | font_data[offset + 3];
		}

		inline Uint16 read_data_uint16(const Uint8* font_data, int offset)
		{
			return (font_data[offset] << 8) | font_data[offset + 1];
		}

		gameswf::array<Uint32> m_glyph_pixels;
		membuf m_glyph_data;
		membuf m_header;
		membuf* m_data;
		tu_file* m_file;
	};

	struct bitmap_glyph_texture_cache : public texture_cache
	{
		bitmap_glyph_texture_cache(int width, int height);

		bool add_glyph_region(Uint16 code, void* fe, int fontsize);
		void get_glyph_region(Uint16 code, void* fe, int fontsize, rect& bounds);
		inline key make_key(Uint16 code, void* fe, unsigned short fontsize)
		{
			key k;
			k.major = (((Uint64)fontsize & 0xff) << 48) | ((Uint64)code << 32) | (((unsigned int)fe) & 0xffffffff);
			k.minor = 0;
			return k;
		}
	};

	struct bitmap_glyph_provider
	{
		bitmap_glyph_provider(int cache_width, int cache_height, bool loadFontsInMemory) :
			m_file_cache_enabled(loadFontsInMemory),
			m_bitmap_glyph_texture_cache(NULL)
		{
			if(cache_width > 0 && cache_height > 0)
			{
				m_bitmap_glyph_texture_cache = swfnew bitmap_glyph_texture_cache(cache_width, cache_height);
			}
		}
		virtual ~bitmap_glyph_provider();
		bitmap_font_entity* get_font_entity(const tu_string& fontname, bool is_bold, bool is_italic);
		virtual bitmap_font_entity* get_font_entity_impl(const tu_string& path) = 0;
		inline void set_file_cache_enabled(bool enabled)
		{
			m_file_cache_enabled = enabled;
		}
		inline bool is_file_cache_enabled() const
		{
			return m_file_cache_enabled;
		}
		void clear()
		{
			m_fonts.clear();
			
			if(m_bitmap_glyph_texture_cache != NULL)
				m_bitmap_glyph_texture_cache->reset();
		}
		bitmap_glyph_texture_cache* get_texture_cache() const
		{
			return m_bitmap_glyph_texture_cache;
		}

	protected:
		gameswf::stringi_hash<smart_ptr<gameswf::bitmap_font_entity> > m_fonts;
		bool m_file_cache_enabled;
		bitmap_glyph_texture_cache* m_bitmap_glyph_texture_cache;
	};

	// default bitmap glyph provider
	struct default_bitmap_glyph_provider : gameswf::bitmap_glyph_provider
	{
		default_bitmap_glyph_provider(int cache_width, int cache_height, bool loadFontsInMemory) :
			bitmap_glyph_provider(cache_width, cache_height, loadFontsInMemory)
		{
		}
		virtual gameswf::bitmap_font_entity* get_font_entity_impl(const gameswf::tu_string& path)
		{
			return swfnew gameswf::default_bitmap_font_entity(this, path);
		}
	};

}	// end namespace gameswf



#endif // GAMESWF_BITMAPFONT_H

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
