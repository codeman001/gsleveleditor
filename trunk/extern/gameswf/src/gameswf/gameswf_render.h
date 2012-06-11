#pragma once
// gameswf_render.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Interface to renderer back-end.


#ifndef GAMESWF_RENDER_H
#define GAMESWF_RENDER_H


#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf.h"
#include "base/image.h"


namespace gameswf
{
	extern render_handler* s_render_handler;

	inline void set_render_handler(render_handler* r)
	{
		s_render_handler = r;
	}

	inline render_handler* get_render_handler()
	{
		return s_render_handler;
	}

	namespace render
	{
		bitmap_info*	create_bitmap_info_empty();
		bitmap_info*	create_bitmap_info_alpha(int w, int h, unsigned char* data);
		bitmap_info*	create_bitmap_info_rgb(image_rgb* im);
		bitmap_info*	create_bitmap_info_rgba(image_rgba* im);
		bitmap_info*	create_bitmap_info_texture(video_texture* texture);
		bitmap_info*	create_bitmap_info_native(int w, int h, const membuf* data);
		video_handler*	create_video_handler();

		// Bracket the displaying of a frame from a movie.
		// Fill the background color, and set up default
		// transforms, etc.
		inline void begin_display(
			rgba background_color,
			int viewport_x0, int viewport_y0,
			int viewport_width, int viewport_height,
			float x0, float x1, float y0, float y1)
		{
			if (s_render_handler)
			{
				s_render_handler->begin_display(
					background_color, viewport_x0, viewport_y0,
					viewport_width, viewport_height,
					x0, x1, y0, y1);
			}
		}
		
		inline void end_display()
		{
			if (s_render_handler) s_render_handler->end_display();
		}

		inline void set_blend_mode(blend_mode::id mode)
		{
			if (s_render_handler) s_render_handler->set_blend_mode(mode);
		}
		
		void	set_target(bitmap_info* target);
		void	clear_frame_buffer();
		void	read_frame_buffer(int x, int y, int width, int height, unsigned char* pixels);
		
		inline void	set_context(player_context* context)
		{
			if (s_render_handler) s_render_handler->set_context(context);
		}
		
		inline void set_render_cache(render_cache* cache)
		{
			if (s_render_handler) s_render_handler->set_render_cache(cache);
		}

		// Geometric and color transforms for mesh and line_strip rendering.
		inline void set_matrix(const matrix& m)
		{
			if (s_render_handler) s_render_handler->set_matrix(m);
		}
		inline void set_cxform(const cxform& cx)
		{
			if (s_render_handler) s_render_handler->set_cxform(cx);
		}

		// Draw triangles using the current fill-style 0.
		// Clears the style list after rendering.
		//
		// coords is a list of (x,y) coordinate pairs, in
		// triangle-strip order.  The type of the array should
		// be float[vertex_count*2]
		inline void draw_mesh_strip(const coord_component coords[], int vertex_count)
		{
			if (s_render_handler) s_render_handler->draw_mesh_strip(coords, vertex_count);
		}

		// Draw triangles using the current fill-style 0.
		// Clears the style list after rendering.
		//
		// coords is a list of (x,y) coordinate pairs, in
		// triangle-list order.  The type of the array should
		// be float[vertex_count*2]
		inline void draw_triangle_list(const coord_component coords[], int vertex_count, const unsigned short* indexes, int index_count)
		{
			if (s_render_handler) s_render_handler->draw_triangle_list(coords, vertex_count, indexes, index_count);
		}

		// Draw a line-strip using the current line style.
		// Clear the style list after rendering.
		//
		// Coords is a list of (x,y) coordinate pairs, in
		// sequence.
		inline void draw_line_strip(const coord_component coords[], int vertex_count)
		{
			if (s_render_handler) s_render_handler->draw_line_strip(coords, vertex_count);
		}

		// draw render cache
		inline void draw(render_cache* cache)
		{
			if (s_render_handler) s_render_handler->draw(cache);
		}

		inline void fill_style_disable(int fill_side)
		{
			if (s_render_handler) s_render_handler->fill_style_disable(fill_side);
		}
		
		inline void fill_style_color(int fill_side, const rgba& color)
		{
			if (s_render_handler) s_render_handler->fill_style_color(fill_side, color);
		}
		
		inline void fill_style_bitmap(int fill_side, bitmap_info* bi, const matrix& m, render_handler::bitmap_wrap_mode wm)
		{
			if (s_render_handler) s_render_handler->fill_style_bitmap(fill_side, bi, m, wm);
		}

		inline void line_style_disable()
		{
			if (s_render_handler) s_render_handler->line_style_disable();
		}
		
		inline void line_style_color(rgba color)
		{
			if (s_render_handler) s_render_handler->line_style_color(color);
		}
		
		inline void line_style_width(float width)
		{
			if (s_render_handler) s_render_handler->line_style_width(width);
		}

		inline void begin_submit_mask()
		{
			if (s_render_handler) s_render_handler->begin_submit_mask();
		}
		
		inline void end_submit_mask()
		{
			if (s_render_handler) s_render_handler->end_submit_mask();
		}

		inline void disable_mask()
		{
			if (s_render_handler) s_render_handler->disable_mask();
		}
		
		void	set_mask_bound(const rect& bound);
		const rect&	get_mask_bound();

		// Special function to draw a rectangular bitmap;
		// intended for textured glyph rendering.  Ignores
		// current transforms.
		inline void draw_bitmap(const matrix& m, bitmap_info* bi, const rect& coords, const rect& uv_coords, rgba color)
		{
			if (s_render_handler) s_render_handler->draw_bitmap(m, bi, coords, uv_coords, color);
		}

		inline void set_cursor(render_handler::cursor_type cursor)
		{
			if (s_render_handler) s_render_handler->set_cursor(cursor);
		}
		
		inline bool is_visible(const rect& bound)
		{
			return s_render_handler ? s_render_handler->is_visible(bound) : true;
		}

		inline void set_text_pixel_alignment(bool enabled)
		{
			if (s_render_handler) s_render_handler->set_text_pixel_alignment(enabled);
		}
	};	// end namespace render
};	// end namespace gameswf


#endif // GAMESWF_RENDER_H

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
