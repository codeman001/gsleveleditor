#pragma once
// gameswf_texturecache.h


#ifndef GAMESWF_TEXTURECACHE_H
#define GAMESWF_TEXTURECACHE_H


#include "base/container.h"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_impl.h"

namespace gameswf
{
	struct texture_cache
	{
	public:
		struct region
		{
			Uint64 timestamp;
			int xcount;
			int ycount;
		};

		struct key
		{
			Uint64 major;
			Uint64 minor;

			bool operator == (const key& k) const
			{
				return major == k.major && minor == k.minor;
			}
		};

		texture_cache(int width, int height, int bpp, bool target = false);
		region* find_available_region(int width, int height);
		region* find_used_region(int width, int height);
		
		void get_region_bounds(const region* r, rect& bounds)
		{
			// optimized because width is power of 2
			const int pitch = m_bitmap->get_width() >> CELL_SHIFT;
			const int offset = r - &m_regions[0];
			const int x = (offset & (pitch-1)) << CELL_SHIFT;
			const int y = (offset / pitch) << CELL_SHIFT;
			const int w = r->xcount << CELL_SHIFT;
			const int h = r->ycount << CELL_SHIFT;

			bounds.m_x_min = (float)x;
			bounds.m_y_min = (float)y;
			bounds.m_x_max = (float)x + w;
			bounds.m_y_max = (float)y + h;
		}
		
		bitmap_info* get_bitmap_info() { return m_bitmap.get_ptr(); }
		int get_bpp() const { return m_bpp; }
		
		unsigned char* lock()
		{
			if(m_pixels == NULL)
			{
				m_pixels = (unsigned char*)m_bitmap->lock();
				if(m_used_regions.size() == 0)
				{
					// ensure that texture is cleared to prevent interpolation glitches with old pixels
					memset(m_pixels, 0, m_bitmap->get_width() * m_bitmap->get_height() * m_bpp);
				}
			}
			return m_pixels;
		}
		
		void unlock()
		{
			if(m_pixels != NULL)
			{
				m_bitmap->unlock();
				m_pixels = NULL;
			}
		}
		void reset();

		Uint64 get_invalidate_timestamp() const
		{
			return m_invalidate_timestamp;
		}

		static void get_region_size_requirement(int& width, int& height);
		static void unlock_all(player_context* context);

		const static int CELL_SIZE = 16;
		const static int CELL_SHIFT = 4;

	protected:
		void subdivide_region(region* r, int newxcount, int newycount);
		void verify_integrity();
		void verify_integrity(region* r);

		Uint64 m_timestamp;
		Uint64 m_invalidate_timestamp;
		array<region> m_regions;
		array<region*> m_available_regions;
		hash<key, region*> m_used_regions;
		smart_ptr<bitmap_info> m_bitmap;
		int m_bpp;
		unsigned char* m_pixels;
	};
}	// end namespace gameswf



#endif // GAMESWF_TEXTURECACHE_H

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
