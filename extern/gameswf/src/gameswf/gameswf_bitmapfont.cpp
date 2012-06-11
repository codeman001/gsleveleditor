#include "stdafx.h"
// gameswf_font.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A font type for gameswf.


#include "gameswf/gameswf_bitmapfont.h"
#include "gameswf/gameswf_stream.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_shape.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_freetype.h"
#include "base/tu_file.h"


namespace gameswf
{
	bitmap_font_entity::~bitmap_font_entity()
	{
		for (hash<int, glyph_entity*>::iterator it = m_ge.begin(); it != m_ge.end(); ++it)
		{
			swfdelete (it->second);
		}
	}

	bitmap_info* bitmap_font_entity::get_char_image(Uint16 code, int fontsize, rect* bounds, float* advance)
	{
		// form hash key
		int key = (fontsize << 16) | code;

		// try to find the stored image of character
		glyph_entity* ge = NULL;
		if (m_ge.get(key, &ge) == false)
		{
			bitmap_glyph_data bitmap;
			bitmap_glyph_metrics metrics;
			if(get_char_image(&bitmap, code, fontsize, &metrics) == false)
				return NULL;

			if(m_provider->get_texture_cache() == NULL)
			{
				gameswf::log_error("bitmap texture cache not initialized\n");
				return NULL;
			}
			
			ge = swfnew glyph_entity();

			// You must use power-of-two dimensions!!
			int	w = bitmap.m_width + 1;
			int	h = bitmap.m_height + 1;
			glyph_texture_cache::get_region_size_requirement(w, h);

			ge->m_bounds.m_x_max = float(bitmap.m_width) / float(w);
			ge->m_bounds.m_y_max = float(bitmap.m_height) / float(h);
			
			ge->m_bounds.m_x_min = float(-metrics.m_xanchor) / float(metrics.m_width);
			ge->m_bounds.m_y_min = float(metrics.m_yanchor) / float(metrics.m_height);
			ge->m_bounds.m_x_min *= - ge->m_bounds.m_x_max;
			ge->m_bounds.m_y_min *= ge->m_bounds.m_y_max;

			ge->m_advance = PIXELS_TO_TWIPS(metrics.m_advance);
			
			// keep glyph
			m_ge.add(key, ge);
		}

		*bounds = ge->m_bounds;
		*advance = ge->m_advance;

		return m_provider->get_texture_cache()->get_bitmap_info();
	}

	void bitmap_font_entity::copy_argb(unsigned char* src, int src_x, int src_y, int src_pitch, unsigned char* dst, int dst_pitch, int dst_height)
	{
		src += src_y * src_pitch + src_x * sizeof(int);
		for(int y = 0; y < dst_height; y++)
		{
			memcpy(dst, src, dst_pitch);
			src += src_pitch;
			dst += dst_pitch;
		}
	}

	bitmap_glyph_provider::~bitmap_glyph_provider()
	{
		clear();

		// smart pointers free bitmap fonts
		if(m_bitmap_glyph_texture_cache != NULL)
		{
			swfdelete (m_bitmap_glyph_texture_cache);
		}
	}

	bitmap_font_entity* bitmap_glyph_provider::get_font_entity(const tu_string& fontname, bool is_bold, bool is_italic)
	{
		tu_string full_name = fontname;
		if(is_bold)
			full_name += "B";
		if(is_italic)
			full_name += "I";

		smart_ptr<bitmap_font_entity> bfe;
		if(m_fonts.get(full_name, &bfe) == false)
		{
			char path[256] = {0};
			if (get_fontfile(fontname.c_str(), is_bold, is_italic, path, 256) == true)
			{
				if(strstr(path, ".fnt") || strstr(path, ".FNT"))
				{
					// Scan in loaded fonts for a filename match to avoid loading same font mutliple times
					for(gameswf::stringi_hash<smart_ptr<gameswf::bitmap_font_entity> >::iterator it = m_fonts.begin(); it != m_fonts.end(); ++it)
					{
						bitmap_font_entity* bfe = it->second.get_ptr();
						if(bfe != NULL && bfe->m_filename == path)
						{
							// keep mapping in cache
							m_fonts.add(full_name, bfe);
							return bfe;
						}
					}

					bfe = get_font_entity_impl(path);
				}
			}

			m_fonts[full_name] = bfe; // bfe can be null
		}

		return bfe.get_ptr();
	}

	// default bitmap font entity
	default_bitmap_font_entity::default_bitmap_font_entity(bitmap_glyph_provider* provider, const gameswf::tu_string& filename) :
		bitmap_font_entity(provider),
		m_data(NULL),
		m_file(NULL)
	{
		m_filename = filename;
		m_height = 1;
		m_file = swfnew tu_file(filename, "rb");
		if(m_file->is_open())
		{
			m_file->go_to_end();
			int filesize = m_file->get_position();
			m_file->set_position(0);

			// read top header
			m_header.resize(TABLE_OFFSET);
			m_file->read_fully(&m_header);

			// validate file signature and version
			char* font_data = (char*)m_header.data();
			assert(font_data[0] == 'G' && font_data[1] == 'F' && font_data[2] == 'N' && font_data[3] == 'T');
			Uint32 version = read_uint16(4);
			assert(version == VERSION);

			m_height = (float)read_uint32(LINE_HEIGHT_OFFSET);
			float scale = get_scale();
			m_height *= scale;

			// get glyph count and read header + offsets
			int glyph_count	= read_uint32(GLYPH_COUNT_OFFSET);
			m_header.resize(TABLE_OFFSET + ((glyph_count + 1) * sizeof(Uint32)));
			m_file->set_position(0);
			m_file->read_fully(&m_header);

			if(m_provider->is_file_cache_enabled())
			{
				// buffer pixel data
				m_data = swfnew membuf();
				m_data->resize(filesize - m_header.size());
				m_file->read_fully(m_data);
				swfdelete (m_file);
				m_file = NULL;
			}
		}
		else
		{
			assert(false);
			swfdelete (m_file);
			m_file = NULL;
		}
	}

	default_bitmap_font_entity::~default_bitmap_font_entity()
	{
		if(m_data)
			swfdelete (m_data);
		if(m_file)
			swfdelete (m_file);
	}

	bool default_bitmap_font_entity::get_char_image(gameswf::bitmap_glyph_data* bgd, gameswf::Uint16 code, int fontsize, gameswf::bitmap_glyph_metrics* metrics)
	{
		// read header
		const Uint8* font_data = (const Uint8*)m_header.data();
		int glyph_count	= read_uint32(GLYPH_COUNT_OFFSET);
		int cell_width	= read_uint32(CELL_WIDTH_OFFSET);
		int cell_height	= read_uint32(CELL_HEIGHT_OFFSET);
		int baseline	= read_uint32(BASELINE_OFFSET);
		int line_height = read_uint32(LINE_HEIGHT_OFFSET);
		int char_space	= read_uint32(CHAR_SPACE_OFFSET);
		int first_char	= read_uint32(FIRST_CHAR_OFFSET);

		// read glyph infos
		int glyph_index = code - first_char;

		// contains this glyph?
		if(glyph_index < 0 || glyph_index >= glyph_count)
			return false;
		
		const int glyph_data_offset = read_uint32(TABLE_OFFSET + (glyph_index * sizeof(Uint32)));
		const int next_glyph_data_offset = read_uint32(TABLE_OFFSET + ((glyph_index + 1) * sizeof(Uint32)));
		const int glyph_data_size = next_glyph_data_offset - glyph_data_offset;
		const Uint8* glyph_data = NULL;
		
		if(glyph_data_size == 0)
		{
			return false;
		}
		else if(m_data != NULL)
		{
			glyph_data = (const Uint8*)m_data->data() + glyph_data_offset - m_header.size();
		}
		else
		{
			// read glyph data from file into temporary buffer
			if(glyph_data_size > m_glyph_data.size())
				m_glyph_data.resize(glyph_data_size);

			assert(m_file != NULL);
			m_file->set_position(glyph_data_offset);
			m_file->read_fully(&m_glyph_data, glyph_data_size);
			glyph_data = &m_glyph_data[0];
		}

		const int minx = read_data_uint16(glyph_data, GLYPH_MINX_OFFSET);
		const int maxx = read_data_uint16(glyph_data, GLYPH_MAXX_OFFSET);

		if(bgd != NULL)
		{
			// ensure capacity
			int totalPixels = cell_width * cell_height;
			if(totalPixels > m_glyph_pixels.size())
				m_glyph_pixels.resize(totalPixels);
			
			// uncompress RLE data in to glyph pixels
			int pixelOffset = GLYPH_PIXEL_OFFSET;
			int pixelLimit = GLYPH_PIXEL_OFFSET + glyph_data_size;
			int pixel = 0;
			while(pixel < totalPixels)
			{
				int rleInfo = glyph_data[pixelOffset++];
				bool isCopyRun = (rleInfo & 0x80) != 0;
				int count = (rleInfo & 0x7f) + 1;
				Uint32 color = 0;

				for(int i = 0; i < count; ++i)
				{
                    if((isCopyRun && i == 0) || !isCopyRun)
                    {
                       color = swap_le32( read_data_uint32(glyph_data, pixelOffset) );

#ifdef _GLITCH
#if _TU_LITTLE_ENDIAN_ == 0 || GLITCH_REVISION >= GAMESWF_PIXEL_FORMAT_REV
					   // convert from ARGB to BGRA (bytes)
					   color = swap32(color);
#endif
#endif // _GLITCH

					   pixelOffset += sizeof(Uint32);
                    }

					assert(pixelOffset < pixelLimit);
                    
					m_glyph_pixels[pixel] = color;
                    pixel++;
                }
			}

			bgd->m_pitch = cell_width * 4;
			bgd->m_pixels = (unsigned char*)&m_glyph_pixels[0];
			bgd->m_width = cell_width;
			bgd->m_height = cell_height;
		}

		if(metrics != NULL)
		{
			float scale = get_units_per_em() / PIXELS_TO_TWIPS(fontsize);

			metrics->m_xanchor = minx;
			metrics->m_yanchor = baseline;
			metrics->m_width = cell_width;
			metrics->m_height = cell_height;
			metrics->m_advance = int((maxx - minx + 1 + char_space) * scale);
		}
		return true;
	}

	// bitmap glyph provider
	bitmap_glyph_texture_cache::bitmap_glyph_texture_cache(int width, int height) :
		texture_cache(width, height, 4)
	{
		//bitmap_info * bitmapInfo = get_bitmap_info();
		//bitmapInfo->set_min_filter(bitmap_info::FILTER_MODE_NEAREST);
		//bitmapInfo->set_mag_filter(bitmap_info::FILTER_MODE_NEAREST);
	}

	bool bitmap_glyph_texture_cache::add_glyph_region(Uint16 code, void* fe, int fontsize)
	{
		bitmap_font_entity* bfe = (bitmap_font_entity*)fe;
		unsigned char* pixels = lock();

		bitmap_glyph_data bitmap;
		bitmap_glyph_metrics metrics;
		if(bfe->get_char_image(&bitmap, code, fontsize, &metrics))
		{
			int	w = bitmap.m_width + 1;
			int	h = bitmap.m_height + 1;
			get_region_size_requirement(w, h);

			region* r = find_available_region(w, h);
			if(r == NULL)
				return false;
			
			// update infos
			r->timestamp = m_timestamp++;
			
			key k = make_key(code, fe, fontsize);
			m_used_regions[k] = r;
			
			rect bounds;
			get_region_bounds(r, bounds);
			assert(bounds.width() == w && bounds.height() == h);
			const int BPP = m_bpp;
			int region_offset = (int)(bounds.m_y_min * m_bitmap->get_width() * BPP + bounds.m_x_min * BPP);
			unsigned char* region_pixels = pixels + region_offset;
			int dst_pitch = m_bitmap->get_width() * BPP;

			//static int writecount = 0;
			//if(writecount++ > 2)
			//	continue;

			// clear region
			unsigned char* ptr = region_pixels;
			for (int i = 0; i < h; i++)
			{
				memset(ptr, 0, w * BPP);
				ptr += dst_pitch;
			}

			// copy bitmap glyph to texture cache
			for (int i = 0; i < bitmap.m_height; i++)
			{
				uint8*	src = bitmap.m_pixels + bitmap.m_pitch * i;
				uint8*	dst = region_pixels + dst_pitch * i;
				memcpy(dst, src, bitmap.m_width * BPP);
			}
			return true;
		}
		return false;
	}

	void bitmap_glyph_texture_cache::get_glyph_region(Uint16 code, void* fe, int fontsize, rect& bounds)
	{
		key k = make_key(code, fe, fontsize);
		region* r = NULL;
		if(!m_used_regions.get(k, &r))
		{
			// this can happen if add_glyph_regions with string of character failed previously
			// (ie not enough place inside cache to contain all characters)
			if(!add_glyph_region(code, fe, fontsize))
			{
				// flush + reset
				get_render_handler()->flush();
				reset();

				// add in empty cache
				add_glyph_region(code, fe, fontsize);
			}

			bool found = m_used_regions.get(k, &r);
			assert(found);
		}
		if(r)
		{
			get_region_bounds(r, bounds);
		}
	}

};	// end namespace gameswf


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
