#include "stdafx.h"
// gameswf_texture_cache.cpp


#include "gameswf/gameswf_bitmapfont.h"
#include "gameswf/gameswf_freetype.h"
#include "gameswf/gameswf_stream.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_shape.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_texture_cache.h"
#include "base/tu_file.h"


namespace gameswf
{
	texture_cache::texture_cache(int width, int height, int bpp, bool target) :
		m_timestamp(0),
		m_invalidate_timestamp(0),
		m_pixels(NULL),
		m_bpp(bpp)
	{
		//assert(width == height);
		get_region_size_requirement(width, height);

		// create the unique texture
		if(m_bpp == 1)
		{
			m_bitmap = get_render_handler()->create_bitmap_info_alpha(width, height, NULL);
			m_bitmap->set_writable();
		}
		else if(target)
		{
			assert(m_bpp == 4);
			m_bitmap = get_render_handler()->create_bitmap_info_target(width, height);
			m_bitmap->set_writable();
		}
		else
		{
			assert(m_bpp == 4);
			image_rgba* rgba = swfnew image_rgba(width, height);
			memset(rgba->m_data, 0, rgba->m_pitch * rgba->m_height);
			m_bitmap = get_render_handler()->create_bitmap_info_rgba(rgba);
			m_bitmap->set_writable();
			swfdelete (rgba);
		}

		reset();
	}

#define DEBUG_REGIONS	0

#if DEBUG_REGIONS
	#define DEBUG_REGION_PRINT	printf
	#define DEBUG_VERIFY_INTEGRITY() verify_integrity();
#else
	#define DEBUG_REGION_PRINT
	#define DEBUG_VERIFY_INTEGRITY()
#endif

	texture_cache::region* texture_cache::find_available_region(int width, int height)
	{
		DEBUG_VERIFY_INTEGRITY();

		int xcount = width/CELL_SIZE;
		int ycount = height/CELL_SIZE;
		int match = -1;

		assert(xcount > 0 && ycount > 0);

		DEBUG_REGION_PRINT("find_available_region %dx%d (%dx%d)\n", width, height, xcount, ycount);

		for(int i = 0; i < m_available_regions.size(); i++)
		{
			DEBUG_REGION_PRINT("\ttest region %d %dx%d\n", i, m_available_regions[i]->xcount, m_available_regions[i]->ycount);

			if(m_available_regions[i]->xcount >= xcount && m_available_regions[i]->ycount >= ycount)
			{
				// better than current match?
				if(match == -1 || (m_available_regions[i]->xcount < m_available_regions[match]->xcount || m_available_regions[i]->ycount < m_available_regions[match]->ycount))
					match = i;
			}
		}

		DEBUG_REGION_PRINT("\tmatch=%d\n", match);

		if(match != -1)
		{
			region* match_region = m_available_regions[match];

			// no more available
			m_available_regions.remove(match);

			// need to subdivide region?
			if(match_region->xcount > xcount || match_region->ycount > ycount)
				subdivide_region(match_region, xcount, ycount);

			DEBUG_VERIFY_INTEGRITY();
			return match_region;
		}

		return NULL;
	}

	texture_cache::region* texture_cache::find_used_region(int width, int height)
	{
		// since nothing can be used in available regions, need to recycle oldest used region with same size
		DEBUG_VERIFY_INTEGRITY();

		int xcount = width/CELL_SIZE;
		int ycount = height/CELL_SIZE;
		key match_key;
		region* match_region = NULL;
		hash<key, region*>::const_iterator it = m_used_regions.begin();
		for(;it != m_used_regions.end(); ++it)
		{
			if(it->second->xcount >= xcount && it->second->ycount >= ycount && (!match_region || it->second->timestamp < match_region->timestamp))
			{
				match_key = it->first;
				match_region = it->second;
			}
		}
		if(match_region)
		{
			//printf("removing char=%c\n", (match_key >> 32) & 0xffff );

			// remove from used regions
			m_used_regions.erase(match_key);

			// need to subdivide region?
			if(match_region->xcount > xcount || match_region->ycount > ycount)
				subdivide_region(match_region, xcount, ycount);

			m_invalidate_timestamp = m_timestamp;
			DEBUG_VERIFY_INTEGRITY();
			return match_region;
		}

		// nothing can be found in used regions, need to merge regions
		// TODO

		return NULL;
	}

	void texture_cache::subdivide_region(region* r, int newxcount, int newycount)
	{
		DEBUG_VERIFY_INTEGRITY();

		const int xcount = r->xcount;
		const int ycount = r->ycount;
		const int pitch = m_bitmap->get_width() / CELL_SIZE;
		const int offset = r - &m_regions[0];

		assert(newxcount > 0 && newycount > 0);

		DEBUG_REGION_PRINT("subdivide region %dx%d -> %dx%d\n", xcount, ycount, newxcount, newycount);

		// right residue?
		if(xcount - newxcount > 0)
		{
			region* right = &m_regions[offset + newxcount];
			right->xcount = xcount - newxcount;
			right->ycount = newycount;
			right->timestamp = 0;
			DEBUG_REGION_PRINT("\tright %dx%d\n", right->xcount, right->ycount);
			m_available_regions.push_back(right);

			DEBUG_VERIFY_INTEGRITY();
		}

		// bottom residue?
		if(ycount - newycount > 0)
		{
			region* bottom = &m_regions[offset + pitch * newycount];
			bottom->xcount = xcount;
			bottom->ycount = ycount - newycount;
			bottom->timestamp = 0;
			DEBUG_REGION_PRINT("\tbottom %dx%d\n", bottom->xcount, bottom->ycount);
			m_available_regions.push_back(bottom);

			DEBUG_VERIFY_INTEGRITY();
		}

		// resized region
		r->xcount = newxcount;
		r->ycount = newycount;

		DEBUG_VERIFY_INTEGRITY();
	}

	void texture_cache::verify_integrity()
	{
		for(int i = 0; i < m_available_regions.size(); i++)
		{
			verify_integrity(m_available_regions[i]);
		}

		hash<region*, region*> singles;

		hash<key, region*>::const_iterator it = m_used_regions.begin();
		for(;it != m_used_regions.end(); ++it)
		{
			verify_integrity(it->second);
			
			// ensure they exist only one time
			region* r;
			assert(singles.get(it->second, &r) == false);
			singles[it->second] = it->second;
		}
	}

	void texture_cache::verify_integrity(region* r)
	{
		const int pitch = m_bitmap->get_width() / CELL_SIZE;
		const int height = m_bitmap->get_height() / CELL_SIZE;
		const int offset = r - &m_regions[0];
		const int x = offset % pitch;
		const int y = offset / pitch;

		assert(r->xcount > 0 && r->ycount > 0);
		assert(x + r->xcount <= pitch && y + r->ycount <= height);
	}

	void texture_cache::get_region_size_requirement(int& width, int& height)
	{
		// power of 2
		//int w = 1; while (w < width) { w <<= 1; }
		//int h = 1; while (h < height) { h <<= 1; }
		//width = imax(w, CELL_SIZE);
		//height = imax(h, CELL_SIZE);

		// multiple of CELL_SIZE
		int xcount = width / CELL_SIZE;
		width = (width % CELL_SIZE > 0) ? (xcount + 1) * CELL_SIZE : xcount * CELL_SIZE;
		width = imax(width, CELL_SIZE);
		
		int ycount = height / CELL_SIZE;
		height = (height % CELL_SIZE > 0) ? (ycount + 1) * CELL_SIZE : ycount * CELL_SIZE;
		height = imax(height, CELL_SIZE);
	}

	void texture_cache::reset()
	{
		m_used_regions.clear();
		m_available_regions.clear();

		m_timestamp++;
		m_invalidate_timestamp = m_timestamp;

		// init 1st region with whole surface
		region r;
		r.xcount = m_bitmap->get_width() / CELL_SIZE;
		r.ycount = m_bitmap->get_height() / CELL_SIZE;

		if(m_pixels != NULL)
		{
			// ensure that texture is cleared to prevent interpolation glitches with old pixels
			memset(m_pixels, 0, m_bitmap->get_width() * m_bitmap->get_height() * m_bpp);
		}

		// allocate region infos
		m_regions.resize(r.xcount * r.ycount);

		m_regions[0] = r;
		m_available_regions.push_back(&m_regions[0]);
	}

	void texture_cache::unlock_all(player_context* context)
	{
		if(context->m_glyph_provider->get_texture_cache() != NULL)
			context->m_glyph_provider->get_texture_cache()->unlock();

		if(context->m_bitmap_glyph_provider->get_texture_cache() != NULL)
			context->m_bitmap_glyph_provider->get_texture_cache()->unlock();
	}

};	// end namespace gameswf


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
