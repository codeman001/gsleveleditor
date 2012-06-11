#include "stdafx.h"
// gameswf_render.cpp	-- Willem Kokke <willem@mindparity.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Interface to back-end renderer.


#include "gameswf/gameswf_render.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_freetype.h"
#include "gameswf/gameswf_bitmapfont.h"

namespace gameswf 
{
	render_handler* s_render_handler;

	namespace render
	{
		struct bogus_bi : public bitmap_info
		{
		};


		bitmap_info*	create_bitmap_info_empty()
		{
			Uint8 data[4] = {0xff};
			if (s_render_handler) return s_render_handler->create_bitmap_info_alpha(2, 2, data);
			else return swfnew bogus_bi;
		}

		bitmap_info*	create_bitmap_info_alpha(int w, int h, unsigned char* data)
		{
			if (s_render_handler) return s_render_handler->create_bitmap_info_alpha(w, h, data);
			else return swfnew bogus_bi;
		}

		bitmap_info*	create_bitmap_info_rgb(image_rgb* im)
		{
			if (s_render_handler) return s_render_handler->create_bitmap_info_rgb(im);
			else return swfnew bogus_bi;
		}

		bitmap_info*	create_bitmap_info_rgba(image_rgba* im)
		{
			if (s_render_handler) return s_render_handler->create_bitmap_info_rgba(im);
			else return swfnew bogus_bi;
		}

		bitmap_info*	create_bitmap_info_texture(video_texture* texture)
		{
			if (s_render_handler) return s_render_handler->create_bitmap_info_texture(texture);
			else return swfnew bogus_bi;
		}

		bitmap_info*	create_bitmap_info_native(int w, int h, const membuf* data)
		{
			if (s_render_handler) return s_render_handler->create_bitmap_info_native(w, h, data);
			else return swfnew bogus_bi;
		}

		video_handler*	create_video_handler()
		{
			if (s_render_handler) return s_render_handler->create_video_handler();
			else return NULL; //hack swfnew bogus_bi;
		}

// 		// Set line and fill styles for mesh & line_strip
// 		// rendering.
// 		enum bitmap_wrap_mode
// 		{
// 			WRAP_REPEAT,
// 			WRAP_CLAMP
// 		};

	
		static rect s_mask_bound;

		void	set_mask_bound(const rect& bound)
		{
			s_mask_bound = bound;
		}

		const rect& get_mask_bound()
		{
			return s_mask_bound;
		}
	}

	bool render_cache::is_valid(character* ch)
	{
		assert(ch);
		player_context* context = ch->m_player->m_context;

		// update matrices
		ch->get_world_cxform();
		ch->get_world_matrix();

		bool need_update = false;
		if(context->m_glyph_provider->get_texture_cache() != NULL)
		{
			Uint64 timestamp = context->m_glyph_provider->get_texture_cache()->get_invalidate_timestamp();
			if(timestamp != m_gtc_timestamp)
			{
				m_gtc_timestamp = timestamp;
				need_update = true;
			}
		}
		if(context->m_bitmap_glyph_provider->get_texture_cache() != NULL)
		{
			Uint64 timestamp = context->m_bitmap_glyph_provider->get_texture_cache()->get_invalidate_timestamp();
			if(timestamp != m_bgtc_timestamp)
			{
				m_bgtc_timestamp = timestamp;
				need_update = true;
			}
		}

		if(need_update)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
