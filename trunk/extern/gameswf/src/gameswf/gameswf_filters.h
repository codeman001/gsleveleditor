#pragma once
// gameswf_filters.h	-- Julien Hamaide <julien.hamaide@gmail.com> 2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Filters

#ifndef GAMESWF_FILTERS_H
#define GAMESWF_FILTERS_H

#include "gameswf_stream.h"
#include "gameswf_texture_cache.h"

namespace gameswf
{
	struct filter_texture_cache : public texture_cache
	{
	public:
		filter_texture_cache(int width, int height) :
			texture_cache(width, height, 4, false)
		{
		}

		region* get_character_region(character* ch, int width, int height)
		{
			key k = make_key(ch);
			region* r = NULL;
			if(!m_used_regions.get(k, &r))
			{
				get_region_size_requirement_cellsize(width, height);
				r = find_available_region(width, height);
			}
			return r;
		}

		void get_region_size_requirement_cellsize(int& width, int& height)
		{
			// multiple of CELL_SIZE
			int xcount = width / CELL_SIZE;
			width = (width % CELL_SIZE > 0) ? (xcount + 1) * CELL_SIZE : xcount * CELL_SIZE;
			width = imax(width, CELL_SIZE);
			
			int ycount = height / CELL_SIZE;
			height = (height % CELL_SIZE > 0) ? (ycount + 1) * CELL_SIZE : ycount * CELL_SIZE;
			height = imax(height, CELL_SIZE);

			width = imax(width, CELL_SIZE);
			height = imax(height, CELL_SIZE);
		}


	protected:
		void add_character_region(character* ch);

		inline key make_key(character* ch)
		{
			key k;
			k.major = (Uint64)ch;
			k.minor = 0;
			return k;
		}
	};

	struct filter_cache_infos
	{
		filter_cache_infos() :
			m_character(NULL),
			m_region(NULL),
			m_dirty(true)
		{
		}

		character* m_character;
		rect m_bounds;
		texture_cache::region* m_region;
		bool m_dirty;
	};

// http://java.sun.com/j2se/1.5.0/docs/api/java/awt/AlphaComposite.html#SRC_OVER
#define FILTER_BLEND_COLOR(_src_color, _src_alpha, _dst_color, _dst_alpha)		imin(_src_color + (_dst_color * (255 - _src_alpha) / 255), 255)
#define FILTER_BLEND_ALPHA(_src_alpha, _dst_alpha)								imin(_src_alpha + (_dst_alpha * (255 - _src_alpha) / 255), 255)

	struct filter_engine
	{
	public:
		struct raster
		{
			unsigned char* pixels;
			int x; // origin x
			int y; // origin y
			int width;
			int height;
			int pitch;
			int bpp; // bytes/pixels

			inline unsigned char* get_offset()
			{
				return pixels + (y * pitch) + x;
			}
		};

		struct params
		{
			filter* f;
			raster src;
			raster dst;
		};

		filter_engine() :
			m_filter_texture_cache(256, 256),
			m_active_character(NULL)
		{
			m_filter_texture_cache.get_bitmap_info()->layout();
		}

		void run(root* root);
		bool display_cached(character* ch);

		inline filter_texture_cache& get_filter_texture_cache()
		{
			return m_filter_texture_cache;
		}

		static filter_engine* get_instance()
		{
			return s_instance;
		}

		static filter_engine* s_instance;

		static void clear(raster& dst);
		static void copy(raster& src, raster& dst);
		static void blend(raster& src, raster& dst);
		static void apply_shadow(params& p);
		static void apply_glow(params& p);
		static void apply_blur_h(params& p);
		static void apply_blur_v(params& p);
		static void apply(params& p);
		static void prepare(params& p);

	protected:
		void collect_filtered_characters(character* context);
		void read_frame_buffer(int x, int y, int width, int height);

		filter_texture_cache m_filter_texture_cache;
		array<character*> m_filtered_characters;
		array<unsigned char> m_working_buffer;
		hash<character*, filter_cache_infos> m_cache_infos;
		character* m_active_character;
	};

	void read_filter_list( stream* in, effect& eff );

	void create_filter_engine();
	void close_filter_engine();
}

#endif //GAMESWF_FILTERS_H

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
