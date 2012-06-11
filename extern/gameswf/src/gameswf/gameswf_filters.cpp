#include "stdafx.h"
// gameswf_filters.cpp	-- Julien Hamaide <julien.hamaide@gmail.com> 2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Filters

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_stream.h"
#include "gameswf/gameswf_filters.h"
#include "gameswf/gameswf_dlist.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_texture_cache.h"

namespace gameswf
{
	effect	effect::identity;

	void read_filter_list( stream* in, effect& eff )
	{
		// reads FILTERLIST
		int filters = in->read_u8();
		eff.m_filters.reserve(filters);

		for (int i = 0; i < filters; i++)
		{
			filter f;
			f.m_id = (filter::ID)in->read_u8();
			switch (f.m_id)
			{
			case filter::DROP_SHADOW : // DropShadowFilter
				{
					rgba drop_shadow_color;
					drop_shadow_color.read_rgba(in);	// RGBA Color of the shadow
					f.drop_shadow.drop_shadow_color[0] = drop_shadow_color.m_b;
					f.drop_shadow.drop_shadow_color[1] = drop_shadow_color.m_g;
					f.drop_shadow.drop_shadow_color[2] = drop_shadow_color.m_r;
					f.drop_shadow.drop_shadow_color[3] = drop_shadow_color.m_a;
					f.blur.blur_x = in->read_fixed();	// Horizontal blur amount
					f.blur.blur_y = in->read_fixed();	// Vertical blur amount
					f.drop_shadow.angle = in->read_fixed();	// Radian angle of the drop shadow
					f.drop_shadow.distance = in->read_fixed();	// Distance of the drop shadow
					f.drop_shadow.sub_strength = in->read_s8();	// hack, must be FIXED8 Strength of the drop shadow
					f.drop_shadow.inner_shadow = in->read_bool();	// Inner shadow mode
					f.drop_shadow.knockout = in->read_bool();	// Knockout mode
					f.drop_shadow.composite_source = in->read_bool();	// Composite source Always 1
					f.drop_shadow.strength = in->read_uint(5); // passes
					f.drop_shadow.mode = in->read_u8();// If not present, when parsing several filters, filter type is not correct
					IF_VERBOSE_PARSE(log_msg("  filter = DropShadowFilter\n" ));
					eff.m_filters.push_back(f);
					break;
				}

			case filter::BLUR: // BlurFilter
				{
					f.blur.blur_x = in->read_fixed(); // Horizontal blur amount
					f.blur.blur_y = in->read_fixed(); // Vertical blur amount
					f.blur.passes = in->read_uint(5);	// Number of blur passes
					in->read_uint(3);	// Reserved UB[3] Must be 0
					IF_VERBOSE_PARSE(log_msg("  filter = BlurFilter\n" ));
					eff.m_filters.push_back(f);
					break;
				}

			case filter::GLOW: // GlowFilter
				{
					rgba glow_color;
					glow_color.read_rgba(in);	//RGBA Color of the shadow
					f.glow.glow_color[0] = glow_color.m_b;
					f.glow.glow_color[1] = glow_color.m_g;
					f.glow.glow_color[2] = glow_color.m_r;
					f.glow.glow_color[3] = glow_color.m_a;
					f.blur.blur_x = in->read_fixed();	// Horizontal blur amount
					f.blur.blur_y = in->read_fixed();	// Vertical blur amount
					f.blur.passes = 1;
					f.glow.sub_strength = in->read_s8();	// hack, must be FIXED8 Strength of the drop shadow
					f.glow.inner_glow = in->read_bool();	// Inner glow mode
					f.glow.knockout = in->read_bool();	// Knockout mode
					f.glow.composite_source = in->read_bool();	// Composite source Always 1
					f.glow.strength = in->read_uint(5); // strength (x 1000 to get flash editor value)
					in->read_u8();// If not present, when parsing several filters, filter type is not correct
					IF_VERBOSE_PARSE(log_msg("  filter = GlowFilter\n" ));
					eff.m_filters.push_back(f);
					break;
				}

			case 3 : // BevelFilter
				{
					rgba shadow_color;
					shadow_color.read_rgba(in);	//RGBA Color of the shadow
					rgba highlight_color;
					highlight_color.read_rgba(in);	//RGBA Color of the highlight
					float blur_x = in->read_fixed();	// Horizontal blur amount
					float blur_y = in->read_fixed();	// Vertical blur amount
					float angle = in->read_fixed();	// Radian angle of the drop shadow
					float distance = in->read_fixed();	// Distance of the drop shadow
					float strength = in->read_s8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_shadow = in->read_bool();	// Inner shadow mode
					bool knockout = in->read_bool();	// Knockout mode
					bool composite_source = in->read_bool();	// Composite source Always 1
					bool on_top = in->read_bool();	// Composite source Always 1
					int passes = in->read_uint(4); // passes
					in->read_u8();// If not present, when parsing several filters, filter type is not correct
					IF_VERBOSE_PARSE(log_msg("  filter = BevelFilter\n" ));
					break;
				}

			case 4 : // GradientGlowFilter
				{
					int num_colors = in->read_u8();		// Number of colors in the gradient
					for (int i = 0; i < num_colors; i++)
					{
						rgba gradient_colors;
						gradient_colors.read_rgba(in);	// RGBA[NumColors] Gradient colors
					}
					for (int i = 0; i < num_colors; i++)
					{
						int gradient_ratio = in->read_u8();	// UI8[NumColors] Gradient ratios
					}
					float blur_x = in->read_fixed();	// Horizontal blur amount
					float blur_y = in->read_fixed();	// Vertical blur amount
					float angle = in->read_fixed();	// Radian angle of the drop shadow
					float distance = in->read_fixed();	// Distance of the drop shadow
					float strength = in->read_s8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_shadow = in->read_bool();	// Inner shadow mode
					bool knockout = in->read_bool();	// Knockout mode
					bool composite_source = in->read_bool();	// Composite source Always 1
					bool on_top = in->read_bool();	// Composite source Always 1
					int passes = in->read_uint(4); // passes
					IF_VERBOSE_PARSE(log_msg("  filter = GradientGlowFilter\n" ));
					break;
				}

			case 5 : // ConvolutionFilter
				{
					int matrix_x = in->read_u8();	// Horizontal matrix size
					int matrix_y = in->read_u8();	// Vertical matrix size
					float divisor = in->read_float();	// Divisor applied to the matrix values
					float bias = in->read_float();	// Bias applied to the matrix values
					for (int k = 0; k < matrix_x * matrix_y; k++)
					{
						in->read_float();	// Matrix values
					}
					rgba default_color;
					default_color.read_rgba(in);	// RGBA Default color for pixels outside the image
					in->read_uint(6);		// Reserved UB[6] Must be 0
					bool clamp = in->read_bool();	// UB[1] Clamp mode
					bool preserve_alpha = in->read_bool();	// UB[1]
					IF_VERBOSE_PARSE(log_msg("  filter = ConvolutionFilter\n" ));
					break;
				}

			case 6 : // ColorMatrixFilter
				// matrix is float[20]
				for (int k = 0; k < 20; k++)
				{
					in->read_float();
				}
				IF_VERBOSE_PARSE(log_msg("  filter = ColorMatrixFilter\n" ));
				break;

			case 7 : // GradientBevelFilter
				{
					int num_colors = in->read_u8();		// Number of colors in the gradient
					for (int i = 0; i < num_colors; i++)
					{
						rgba gradient_colors;
						gradient_colors.read_rgba(in);	// RGBA[NumColors] Gradient colors
					}
					for (int i = 0; i < num_colors; i++)
					{
						int gradient_ratio = in->read_u8();	// UI8[NumColors] Gradient ratios
					}
					float blur_x = in->read_fixed();	// Horizontal blur amount
					float blur_y = in->read_fixed();	// Vertical blur amount
					float angle = in->read_fixed();	// Radian angle of the drop shadow
					float distance = in->read_fixed();	// Distance of the drop shadow
					float strength = in->read_s8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_shadow = in->read_bool();	// Inner shadow mode
					bool knockout = in->read_bool();	// Knockout mode
					bool composite_source = in->read_bool();	// Composite source Always 1
					bool on_top = in->read_bool();	// Composite source Always 1
					int passes = in->read_uint(4); // passes
					in->read_u8();// If not present, when parsing several filters, filter type is not correct
					IF_VERBOSE_PARSE(log_msg("  filter = GradientBevelFilter\n" ));
					break;
				}

			default:
				assert(0);	// invalid input
			}
		}
	}

	void filter_engine::run(root* root)
	{
		// collect characters with filters
		m_filtered_characters.clear();
		collect_filtered_characters(root->m_movie.get_ptr());

		// nothing to do
		if(m_filtered_characters.size() == 0)
			return;

		// compute bounds needed for each character
		// TODO: add a m_bound member to character and only compute when necessary
		bool reset = false;
		for(int i = 0; i < m_filtered_characters.size(); i++)
		{
			filter_cache_infos infos;
			bool found = m_cache_infos.get(m_filtered_characters[i], &infos);
			
			if(found && !infos.m_dirty)
				continue;
			
			m_filtered_characters[i]->get_bound(&infos.m_bounds);
			infos.m_region = m_filter_texture_cache.get_character_region(m_filtered_characters[i], (int)TWIPS_TO_PIXELS(infos.m_bounds.width()), (int)TWIPS_TO_PIXELS(infos.m_bounds.height()));
			infos.m_character = m_filtered_characters[i];
			
			if(infos.m_region == NULL)
				reset = true;

			m_cache_infos.set(m_filtered_characters[i], infos);
		}

		if(reset)
		{
			// reset cache if could not reserve memory
			m_filter_texture_cache.reset();

			for(hash<character*, filter_cache_infos>::iterator it = m_cache_infos.begin(); it != m_cache_infos.end(); ++it)
			{
				// get new cache region
				it->second.m_region = m_filter_texture_cache.get_character_region(it.get_key(), (int)TWIPS_TO_PIXELS(it.get_value().m_bounds.width()), (int)TWIPS_TO_PIXELS(it.get_value().m_bounds.height()));
				
				// TODO: if could not allocate memory after a reset => need to allocate another texture cache
				if(it.get_value().m_region == NULL)
				{
					log_error("filter texture cache allocation failed\n");
				}
			}
		}

		// render character in texture regions for post-processing
		for(hash<character*, filter_cache_infos>::iterator it = m_cache_infos.begin(); it != m_cache_infos.end(); ++it)
		{
			filter_cache_infos& infos = it->second;

			texture_cache::region* r = infos.m_region;
			if(r && infos.m_dirty)
			{
				// render character in texture for post-processing
				render_handler* handler = get_render_handler();
				//handler->set_target(m_filter_texture_cache.get_bitmap_info());
				handler->clear_frame_buffer();

				// backup cxform & matrix
				matrix mat_backup = infos.m_character->get_matrix();
				cxform cx_backup = infos.m_character->get_cxform();
				character* parent_backup = infos.m_character->get_parent();
				infos.m_character->set_parent(NULL);
				infos.m_character->set_cxform(cxform());
				m_active_character = infos.m_character;

				// use character bounds
				rect& ch_bounds = infos.m_bounds;
				rect r_bounds;
				m_filter_texture_cache.get_region_bounds(r, r_bounds);
				// adjust size to real size
				r_bounds.m_x_max = r_bounds.m_x_min + TWIPS_TO_PIXELS(infos.m_bounds.width());
				r_bounds.m_y_max = r_bounds.m_y_min + TWIPS_TO_PIXELS(infos.m_bounds.height());

				//r_bounds.m_x_min = root->m_viewport_x0;
				//r_bounds.m_y_min = root->m_viewport_y0;
				//r_bounds.m_x_max = root->m_viewport_width;
				//r_bounds.m_y_max = root->m_viewport_height;
				//ch_bounds.m_x_min = root->m_def->m_frame_size.m_x_min;
				//ch_bounds.m_x_max = root->m_def->m_frame_size.m_x_max;
				//ch_bounds.m_y_min = root->m_def->m_frame_size.m_y_min;
				//ch_bounds.m_y_max = root->m_def->m_frame_size.m_y_max;

				// render flipped (ch_bounds.m_y_max and ch_bounds.m_y_min) vertically since framebuffer origin is bottom
				handler->begin_display(
					rgba(),
					(int)r_bounds.m_x_min, (int)r_bounds.m_y_min,
					(int)r_bounds.width(), (int)r_bounds.height(),
					ch_bounds.m_x_min, ch_bounds.m_x_max,
					ch_bounds.m_y_max, ch_bounds.m_y_min);

				infos.m_character->display();

				handler->end_display();

				// back to frame buffer
				//handler->set_target(NULL);

				// restore
				infos.m_character->set_matrix(mat_backup);
				infos.m_character->set_cxform(cx_backup);
				infos.m_character->set_parent(parent_backup);
				m_active_character = NULL;
				
				// read frame buffer in working buffer
				int width = (int)r_bounds.width();
				int height = (int)r_bounds.height();
				int x = (int)r_bounds.m_x_min;
				int y = root->m_viewport_height - height - (int)r_bounds.m_y_min;
				read_frame_buffer(x, y, width, height);

				// apply filters
				unsigned char* dst_pixels = (unsigned char*)m_filter_texture_cache.get_bitmap_info()->lock();
				/*params p;

				// setup source
				p.src.bpp = 4;
				p.src.pixels = (unsigned char*)&m_working_buffer[0];
				p.src.x = 0;
				p.src.y = 0;
				p.src.pitch = width * 4;
				p.src.width = width;
				p.src.height = height;
				
				// setup destination
				p.dst.bpp = 4;
				p.dst.pixels = dst_pixels;
				p.dst.x = 0;
				p.dst.y = 0;
				p.dst.pitch = m_filter_texture_cache.get_bitmap_info()->get_width() * 4;
				p.dst.width = p.src.width;
				p.dst.height = p.src.height;

				for(int i = 0; i < infos.m_character->m_effect.m_filters.size(); i++)
				{
					p.f = &infos.m_character->m_effect.m_filters[i];
					
					clear(p.dst);
					apply(p);

					// swap buffer pointers between each iteration
					swap<raster>(&p.src, &p.dst);
				}

				// copy in texture cache if not already there
				if(p.src.pixels != dst_pixels)
				{
					copy(p.src, p.dst);
				}*/

				int src_pitch = width * 4;
				unsigned char* src_pixels = &m_working_buffer[0];
				int dst_pitch = m_filter_texture_cache.get_bitmap_info()->get_width() * 4;

				for(int y = 0; y < height; y++)
				{
					memcpy(dst_pixels, src_pixels, src_pitch);
					src_pixels += src_pitch;
					dst_pixels += dst_pitch;
				}

				m_filter_texture_cache.get_bitmap_info()->unlock();

				infos.m_dirty = false;
			}
		}
	}

	bool filter_engine::display_cached(character* ch)
	{
		filter_cache_infos infos;
		if(ch != m_active_character && m_cache_infos.get(ch, &infos) && infos.m_region)
		{
			// show cached region
			rect coords;
			coords.m_x_min = 0.0f;
			coords.m_x_max = infos.m_bounds.width();
			coords.m_y_min = 0.0f;
			coords.m_y_max = infos.m_bounds.height();

			rect uv_coords;
			m_filter_texture_cache.get_region_bounds(infos.m_region, uv_coords);
			// adjust size to real size
			uv_coords.m_x_max = uv_coords.m_x_min + TWIPS_TO_PIXELS(infos.m_bounds.width());
			uv_coords.m_y_max = uv_coords.m_y_min + TWIPS_TO_PIXELS(infos.m_bounds.height());
			// adjust to UV [0-1]
			uv_coords.m_x_min /= m_filter_texture_cache.get_bitmap_info()->get_width();
			uv_coords.m_x_max /= m_filter_texture_cache.get_bitmap_info()->get_width();
			uv_coords.m_y_min /= m_filter_texture_cache.get_bitmap_info()->get_height();
			uv_coords.m_y_max /= m_filter_texture_cache.get_bitmap_info()->get_height();

			cxform cx = infos.m_character->get_world_cxform();
			rgba color = cx.transform(gameswf::rgba());
			matrix m = infos.m_character->get_world_matrix();
			render::draw_bitmap(m, m_filter_texture_cache.get_bitmap_info(), coords, uv_coords, color);
			return true;
		}
		return false;
	}

	// python script to generate table:
		//for i in xrange(0, 255):
		//	print str(int(255 * math.sqrt(float(i)/255))) + ',',
		//	if (i % 16) == 15:
		//		print ''
	unsigned char sqrt_table[256] = {	0, 15, 22, 27, 31, 35, 39, 42, 45, 47, 50, 52, 55, 57, 59, 61, 
										63, 65, 67, 69, 71, 73, 74, 76, 78, 79, 81, 82, 84, 85, 87, 88, 
										90, 91, 93, 94, 95, 97, 98, 99, 100, 102, 103, 104, 105, 107, 108, 109, 
										110, 111, 112, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 
										127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 141, 
										142, 143, 144, 145, 146, 147, 148, 148, 149, 150, 151, 152, 153, 153, 154, 155, 
										156, 157, 158, 158, 159, 160, 161, 162, 162, 163, 164, 165, 165, 166, 167, 168, 
										168, 169, 170, 171, 171, 172, 173, 174, 174, 175, 176, 177, 177, 178, 179, 179, 
										180, 181, 182, 182, 183, 184, 184, 185, 186, 186, 187, 188, 188, 189, 190, 190, 
										191, 192, 192, 193, 194, 194, 195, 196, 196, 197, 198, 198, 199, 200, 200, 201, 
										201, 202, 203, 203, 204, 205, 205, 206, 206, 207, 208, 208, 209, 210, 210, 211, 
										211, 212, 213, 213, 214, 214, 215, 216, 216, 217, 217, 218, 218, 219, 220, 220, 
										221, 221, 222, 222, 223, 224, 224, 225, 225, 226, 226, 227, 228, 228, 229, 229, 
										230, 230, 231, 231, 232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 
										238, 239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 
										247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 255};

	void filter_engine::read_frame_buffer(int x, int y, int width, int height)
	{
		int count = width * height * 4;
		
		if(m_working_buffer.size() < count)
			m_working_buffer.resize(count);

		unsigned char* src_pixels = &m_working_buffer[0];

		get_render_handler()->read_frame_buffer(x, y, width, height, src_pixels);

		// convert pixels
		unsigned char* src = src_pixels;
		for(int i = width * height - 1; i != 0; i--)
		{
			const unsigned char r = src[0];
			const unsigned char g = src[1];
			const unsigned char b = src[2];
			const unsigned char a = src[3];
			
			if(a != 0 && a != 255)
			{
				// this conversion works correctly with 1 layer of blending
				const float ratio = 255.0f/sqrt_table[a];
				src[0] = imin((int)(b * ratio), 255);
				src[1] = imin((int)(g * ratio), 255);
				src[2] = imin((int)(r * ratio), 255);
				src[3] = imin((int)(a * ratio), 255);
			}
			else
			{
				src[0] = b;
				src[1] = g;
				src[2] = r;
				src[3] = a;
			}
			src += 4;
		}

		//int dst_pitch = src_pitch;
		//unsigned char* dst_pixels = src_pixels;

		//int dst_pitch = m_filter_texture_cache.get_bitmap_info()->get_width() * 4;
		//unsigned char* dst_pixels = (unsigned char*)m_filter_texture_cache.get_bitmap_info()->lock() +  + (dst_pitch * (height - 1));
	}

	void filter_engine::collect_filtered_characters(character* node)
	{
		if(!node->m_visible || node->get_world_cxform().m_[3][0] == 0.0f)
			return;

		if(node->get_effect().m_filters.size() != 0)
			m_filtered_characters.push_back(node);

		// collect children
		if(node->is(gameswf::AS_SPRITE))
		{
			gameswf::sprite_instance* sprite = (gameswf::sprite_instance*)node;
			gameswf::display_list& display_list = sprite->m_display_list;

			for(int i = 0; i < display_list.size(); i++)
			{
				collect_filtered_characters(display_list.get_character(i));
			}
		}
	}

	void filter_engine::clear(raster& dst)
	{
		unsigned char* pixels = dst.pixels;
		for(int y = 0; y < dst.height; y++, pixels += dst.pitch)
		{
			memset(pixels, 0, dst.pitch);
		}
	}

	void filter_engine::copy(raster& rsrc, raster& rdst)
	{
		int src_pitch = rsrc.pitch;
		unsigned char* src_pixels = rsrc.get_offset();
		int dst_pitch = rdst.pitch;
		unsigned char* dst_pixels = rdst.get_offset();
		assert(rsrc.bpp == rdst.bpp);

		for(int y = 0; y < rsrc.height; y++)
		{
			memcpy(dst_pixels, src_pixels, rsrc.width * rsrc.bpp);
			src_pixels += src_pitch;
			dst_pixels += dst_pitch;
		}
	}

	void filter_engine::blend(raster& rsrc, raster& rdst)
	{
		int src_pitch = rsrc.width * 4;
		unsigned char* src_pixels = rsrc.pixels;
		int dst_pitch = rdst.pitch;
		unsigned char* dst_pixels = rdst.pixels;

		for(int y = 0; y < rsrc.height; y++)
		{
			unsigned char* src = src_pixels;
			unsigned char* dst = dst_pixels;

			for(int x = 0; x < rsrc.width; x++)
			{
				const unsigned char src_b = *(src++);
				const unsigned char src_g = *(src++);
				const unsigned char src_r = *(src++);
				const unsigned char src_a = *(src++);

				if(src_a > 0)
				{
					dst[0] = FILTER_BLEND_COLOR(src_b, src_a, dst[0], dst[3]);
					dst[1] = FILTER_BLEND_COLOR(src_g, src_a, dst[1], dst[3]);
					dst[2] = FILTER_BLEND_COLOR(src_r, src_a, dst[2], dst[3]);
					dst[3] = FILTER_BLEND_ALPHA(src_a, dst[3]);
				}
				
				dst += 4;
			}
			
			src_pixels += src_pitch;
			dst_pixels += dst_pitch;
		}
	}

	void filter_engine::apply_shadow(params& p)
	{
		// apply the shadow
		// show object?
		filter::drop_shadow_params& dsp = p.f->drop_shadow;
		int dx = (int)(dsp.distance * cos(dsp.angle));
		int dy = (int)(dsp.distance * sin(dsp.angle));
		unsigned char* line_src = p.src.get_offset();
		unsigned char* line_dst = p.dst.get_offset() + (dy * p.dst.pitch) + (dx * 4);

		for(int y = 0; y < p.src.height; y++)
		{
			unsigned char* src = line_src;
			unsigned char* dst = line_dst;
			for(int x = 0; x < p.src.width; x++)
			{
				// read BGRA
				const unsigned char src_b = *(src++);
				const unsigned char src_g = *(src++);
				const unsigned char src_r = *(src++);
				const unsigned char src_a = *(src++);

				if(src_a > 0)
				{
					dst[0] = dsp.drop_shadow_color[0];
					dst[1] = dsp.drop_shadow_color[1];
					dst[2] = dsp.drop_shadow_color[2];
					dst[3] = (dsp.drop_shadow_color[3] * src_a) / 255;
				}
				
				dst += 4;
				
			}
			line_src += p.src.pitch;
			line_dst += p.dst.pitch;
		}

		if((dsp.mode & 32) != 0)
		{
			blend(p.src, p.dst);
		}
	}

	void filter_engine::apply_glow(params& p)
	{
		filter::glow_params& params = p.f->glow;
	}

	void gaussian_kernel(float* kernel, int radius)
	{
		float sigma = radius / 2.0f;
		float norm = 1.0f / ((float)sqrt(2 * 3.14159265359f) * sigma); // normalisation constant makes sure total of matrix is 1
		float coeff = 2.0f * sigma * sigma; // the bit you divide x^2 by in the exponential
		float total = 0;

		for(int i = -radius, index = 0; i <= radius; i++)
		{
			kernel[index] = norm * exp(-i * i / coeff);
			total += kernel[index];
			index++;
		}

		for(int i = 0; i < 1 + (2 * radius); i++)
			kernel[i] /= total;
	}

	void filter_engine::apply_blur_h(params& p)
	{
		filter::blur_params& params = p.f->blur;
		int dx = (int)(params.blur_x * 2);
		int stepx = (int)params.blur_x;
		int bpp = p.src.bpp;
		
		assert(p.src.bpp == p.dst.bpp);

		// gaussian distribution
		assert(params.blur_x < 256);
		float kernel_x[256];
		gaussian_kernel(kernel_x, stepx);

		unsigned char* line_src = p.src.get_offset();
		unsigned char* line_dst = p.dst.get_offset();

		// horizontal blur
		if(dx > 0)
		{
			for(int y = 0; y < p.src.height; y++)
			{
				unsigned char* src = line_src;
				unsigned char* dst = line_dst;
				
				for(int x = 0; x < p.src.width; x++)
				{
					int start = -stepx;
					int end = stepx;

					if(x - stepx < 0)
					{
						start = -x;
					}
					if(x + stepx >= p.src.width)
					{
						end = p.src.width - 1 - x;
					}

					if(p.src.bpp == 4)
					{
						float acc_b = 0;
						float acc_g = 0;
						float acc_r = 0;
						float acc_a = 0;
						unsigned char* kernel_src = src + (start * bpp);
						
						for(int i = start; i <= end; i++)
						{
							const float factor = kernel_x[i + stepx] * (float)kernel_src[3]/255;
							acc_b += kernel_src[0] * factor;
							acc_g += kernel_src[1] * factor;
							acc_r += kernel_src[2] * factor;
							acc_a += kernel_src[3] * kernel_x[i + stepx];
							kernel_src += 4;
						}
						
						*(dst++) = (unsigned char)acc_b;
						*(dst++) = (unsigned char)acc_g;
						*(dst++) = (unsigned char)acc_r;
						*(dst++) = (unsigned char)acc_a;
						src += 4;
					}
					else
					{
						float acc_a = 0;
						unsigned char* kernel_src = src + start;
						
						for(int i = start; i <= end; i++)
						{
							acc_a += (float)*kernel_src * kernel_x[i + stepx];
							kernel_src++;
						}
						
						*(dst++) = (unsigned char)acc_a;
						src++;
					}
				}
				line_src += p.src.pitch;
				line_dst += p.dst.pitch;
			}
		}
	}

	void filter_engine::apply_blur_v(params& p)
	{
		filter::blur_params& params = p.f->blur;
		int dy = (int)(params.blur_y * 2);
		int stepy = (int)params.blur_y;
		int bpp = p.src.bpp;
		
		assert(p.src.bpp == p.dst.bpp);

		// gaussian distribution
		assert(params.blur_y < 256);
		float kernel_y[256];
		gaussian_kernel(kernel_y, stepy);

		unsigned char* line_src = p.src.get_offset();
		unsigned char* line_dst = p.dst.get_offset();

		// vertical blur
		if(dy > 0)
		{
			for(int x = 0; x < p.src.width; x++)
			{
				unsigned char* src = line_src;
				unsigned char* dst = line_dst;
				
				for(int y = 0; y < p.src.height; y++)
				{
					int start = -stepy;
					int end = stepy;

					if(y - stepy < 0)
					{
						start = -y;
					}
					if(y + stepy >= p.src.height)
					{
						end = p.src.height - 1 - y;
					}

					if(p.src.bpp == 4)
					{
						float acc_b = 0;
						float acc_g = 0;
						float acc_r = 0;
						float acc_a = 0;
						unsigned char* kernel_src = src + (start * bpp);
						
						for(int i = start; i <= end; i++)
						{
							const float factor = kernel_y[i + stepy] * (float)kernel_src[3]/255;
							acc_b += kernel_src[0] * factor;
							acc_g += kernel_src[1] * factor;
							acc_r += kernel_src[2] * factor;
							acc_a += kernel_src[3] * kernel_y[i + stepy];
							kernel_src += 4;
						}
						
						*(dst) = (unsigned char)acc_b;
						*(dst+1) = (unsigned char)acc_g;
						*(dst+2) = (unsigned char)acc_r;
						*(dst+3) = (unsigned char)acc_a;
						src += p.src.pitch;
						dst += p.dst.pitch;
					}
					else
					{
						float acc_a = 0;
						unsigned char* kernel_src = src + (start * p.src.pitch);
						
						for(int i = start; i <= end; i++)
						{
							acc_a += (float)*kernel_src * kernel_y[i + stepy];
							kernel_src += p.src.pitch;
						}
						
						*(dst) = (unsigned char)acc_a;
						src += p.src.pitch;
						dst += p.dst.pitch;
					}
				}
				line_src++;
				line_dst++;
			}
		}
	}

	void filter_engine::apply(params& p)
	{
		switch(p.f->m_id)
		{
		case filter::DROP_SHADOW:
			copy(p.src, p.dst);
			//apply_shadow(p);
			// swap buffer pointers between each iteration
			//swap<raster>(&p.src, &p.dst);
			//clear(p.dst);
			//apply_blur(p);
			break;
		
		case filter::GLOW:
			apply_glow(p);
			// swap buffer pointers between each iteration
			swap<raster>(&p.src, &p.dst);
			clear(p.dst);
			//apply_blur_x(p);
			break;
		
		case filter::BLUR:
			if(p.f->blur.blur_x > 0)
			{
				apply_blur_h(p);
				swap<raster>(&p.src, &p.dst);
			}
			if(p.f->blur.blur_y > 0)
			{
				apply_blur_v(p);
				swap<raster>(&p.src, &p.dst);
			}
			swap<raster>(&p.src, &p.dst);
			break;
		}
	}

	void filter_engine::prepare(params& p)
	{
		// check the required dimension for output
		p.dst.x = 0;
		p.dst.y = 0;
		p.dst.width = p.src.width;
		p.dst.height = p.src.height;

		switch(p.f->m_id)
		{
		case filter::DROP_SHADOW:
			p.dst.x = (int)(p.f->drop_shadow.distance * cos(p.f->drop_shadow.angle) + 0.5f);
			p.dst.y = (int)(p.f->drop_shadow.distance * sin(p.f->drop_shadow.angle) + 0.5f);
			p.dst.width += iabs(p.dst.x);
			p.dst.height += iabs(p.dst.y);
			break;

		case filter::GLOW:
			p.dst.width += 2;
			p.dst.height += 2;
			break;
		}
		
		p.dst.width += (int)(p.f->blur.blur_x + 0.5f);
		p.dst.height += (int)(p.f->blur.blur_y + 0.5f);
		p.dst.x -= (int)(p.f->blur.blur_x/2.0f);
		p.dst.y -= (int)(p.f->blur.blur_y/2.0f);
	}

	filter_engine* filter_engine::s_instance = NULL;

	void create_filter_engine()
	{
		if(filter_engine::s_instance == NULL)
		{
			filter_engine::s_instance = swfnew filter_engine();
		}
	}
	void close_filter_engine()
	{
		if(filter_engine::s_instance != NULL)
		{
			swfdelete (filter_engine::s_instance);
		}
		filter_engine::s_instance = NULL;
	}
}

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
