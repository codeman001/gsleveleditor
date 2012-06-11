#include "stdafx.h"
// gameswf_text.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Code for the text tags.


//TODO: csm_textsetting() is common for text_def & edit_text_def
//	therefore make swfnew base class for text_def & edit_text_def
//	and move csm_textsetting() to it.

#include "gameswf/gameswf_text.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_bitmapfont.h"
#include "gameswf/gameswf_filters.h"

// ML

namespace gameswf
{
	void	as_global_textfield_ctor(const fn_call& fn)
	// Constructor for ActionScript class XMLSocket
	{
		root* rm = fn.get_player()->get_root();
		edit_text_character_def* empty_text_def = swfnew edit_text_character_def(fn.get_player(), 0, 0);
		character* ch = swfnew edit_text_character(fn.get_player(),
			NULL, empty_text_def, 0);
		fn.result->set_as_object(ch);
	}

	// Render the given glyph records.

	static void	display_glyph_records(
		const matrix* this_mat,
		character* inst,
		array<text_glyph_record>& records,
		movie_definition_sub* root_def,
		gameswf::rgba* forced_color,
		const unsigned char stroke,
		const unsigned char blurx,
		const unsigned char blury)
	{
		render_handler* handler = get_render_handler();
		if(handler == NULL)
			return;

		player_context* context = inst->m_player->m_context;
		glyph_texture_cache* gtc = context->m_glyph_provider->get_texture_cache();
		bitmap_glyph_texture_cache* bgtc = context->m_bitmap_glyph_provider->get_texture_cache();
		
		glyph_texture_cache::filter_info nofilter;
		nofilter.stroke = 0;
		nofilter.blurx = 0;
		nofilter.blury = 0;

		glyph_texture_cache::filter_info filter;
		filter.stroke = 0;
		filter.blurx = blurx;
		filter.blury = blury;

		fill_style stack_fill_style;
		line_style stack_line_style;

		array<fill_style>	dummy_style(&stack_fill_style, 1);	// used to pass a color on to shape_character::display()
		array<line_style>	dummy_line_style(&stack_line_style, 1);
		dummy_style.resize(1);

		matrix	mat = inst->get_world_matrix();
		if(this_mat != NULL)
		{
			mat.concatenate(*this_mat);
		}

		const cxform& cx = inst->get_world_cxform();
		float	pixel_scale = inst->get_pixel_scale();

		matrix	base_matrix = mat;
		float	base_matrix_max_scale = base_matrix.get_max_scale();
		bool	mat_has_rotation = base_matrix.m_[0][1] != 0.0f || base_matrix.m_[1][0] != 0.0f;
		bool	mat_has_scale = base_matrix.m_[0][0] != 1.0f || base_matrix.m_[1][1] != 1.0f;
		bool	mat_has_rotation_scale = mat_has_rotation || mat_has_scale;

		const float font_scale = context->m_glyph_provider->get_scale();
		const float EM_square = (1024.0f * font_scale);
		const float blurx2 = blurx * 2.0f * font_scale;
		const float blury2 = blury * 2.0f * font_scale;
		const float stroke2 = stroke * 2.0f * font_scale;
		float	scale = 1.0f;
		float	x = 0.0f;
		float	y = 0.0f;

		for (int i = 0; i < records.size(); i++)
		{
			// Draw the characters within the current record; i.e. consecutive
			// chars that share a particular style.
			text_glyph_record&	rec = records[i];

			rec.m_style.resolve_font(root_def);

			font*	fnt = rec.m_style.m_font.get_ptr();
			if (fnt == NULL && rec.m_style.m_has_font)
			{
				continue;
			}

			scale = rec.m_style.m_text_height / EM_square;	// the EM square is 1024 x 1024

			// Flash 8
			// All the EMSquare coordinates are multiplied by 20 at export,
			// allowing fractional resolution to 1/20 of a unit.
			if (fnt && fnt->is_define_font3())
			{
				scale /= 20.0f;
			}

			float	text_screen_height = base_matrix_max_scale
				* scale
				* 1024.0f
				/ 20.0f
				* pixel_scale;

			if (rec.m_style.m_has_x_offset)
			{
				x = rec.m_style.m_x_offset;
			}
			if (rec.m_style.m_has_y_offset)
			{
				y = rec.m_style.m_y_offset;
			}

			dummy_style[0].set_color(rec.m_style.m_color);

			rgba	transformed_color = cx.transform(rec.m_style.m_color);

			if(forced_color != NULL)
			{
				transformed_color.m_a = (transformed_color.m_a * forced_color->m_a) / 255;
				transformed_color.m_r = forced_color->m_r;
				transformed_color.m_g = forced_color->m_g;
				transformed_color.m_b = forced_color->m_b;
			}

			for (int j = 0; j < rec.m_glyphs.size(); j++)
			{
				const glyph& g = rec.m_glyphs[j];

				mat = base_matrix;
				if(mat_has_rotation_scale)
				{
					mat.concatenate_translation(x, y);
					mat.concatenate_scale(scale);
				}
				else
				{
					mat.m_[0][2] += x;
					mat.m_[1][2] += y;
					mat.m_[0][0] *= scale;
					mat.m_[1][1] *= scale;
				}

				if (g.m_glyph_index == -1 && g.m_fontlib_glyph == NULL)
				{
					// Invalid glyph; render it as an empty box.
					handler->set_matrix(mat);
					handler->line_style_color(transformed_color);

					// The EM square is 1024x1024, but usually isn't filled up.
					// We'll use about half the width, and around 3/4 the height.
					// Values adjusted by eye.
					// The Y baseline is at 0; negative Y is up.
					static const coord_component s_empty_char_box[5 * 2] =
					{
						32,   32,
						480,   32,
						480, -656,
						32, -656,
						32,   32
					};
					handler->draw_line_strip(s_empty_char_box, 5);
					x += rec.m_glyphs[j].m_glyph_advance;
					continue;
				}

				// draw base line
				if(false)
				{
					handler->set_matrix(mat);
					handler->line_style_color(transformed_color);

					// The EM square is 1024x1024, but usually isn't filled up.
					// We'll use about half the width, and around 3/4 the height.
					// Values adjusted by eye.
					// The Y baseline is at 0; negative Y is up.
					static const coord_component s_baseline[2 * 2] =
					{
						0,   0,
						512,   0
					};
					handler->draw_line_strip(s_baseline, 2);
				}

				// draw under line
				if(rec.m_style.m_underline)
				{
					handler->set_matrix(mat);
					handler->line_style_color(transformed_color);

					// The EM square is 1024x1024, but usually isn't filled up.
					// We'll use about half the width, and around 3/4 the height.
					// Values adjusted by eye.
					// The Y baseline is at 0; negative Y is up.
					const coord_component s_baseline[2 * 2] =
					{
						0,   60,
						(coord_component)(rec.m_glyphs[j].m_glyph_advance/scale),   60
					};
					handler->draw_line_strip(s_baseline, 2);
				}

				if (g.m_fontlib_glyph != NULL)
				{
					if(g.m_type == glyph::BITMAP_IMAGE)
					{
						// display the html embedded image
						rect bounds;
						bounds.m_x_min = 0;
						bounds.m_y_min = -g.m_bounds.m_y_max;
						bounds.m_x_max = g.m_bounds.m_x_max;
						bounds.m_y_max = 0;

						rect uv_bounds;
						uv_bounds.m_x_min = 0;
						uv_bounds.m_y_min = 0;
						uv_bounds.m_x_max = 1;
						uv_bounds.m_y_max = 1;

						handler->draw_bitmap(mat,
							g.m_fontlib_glyph.get_ptr(),
							bounds,
							uv_bounds,
							transformed_color);
					}
					else if(g.m_bounds.m_x_max > 0 && g.m_bounds.m_y_max > 0)
					{
						rect bounds;
						bounds.m_x_max = g.m_bounds.m_x_max - g.m_bounds.m_x_min; 
						bounds.m_y_max = g.m_bounds.m_y_max - g.m_bounds.m_y_min;
						bounds.m_x_min = - g.m_bounds.m_x_min;
						bounds.m_y_min = - g.m_bounds.m_y_min;

						float s_EM_scale = 1024.0f / g.m_fontsize;
						float xscale;
						float yscale;
						rect uv_bounds;

						if (gtc != NULL || bgtc != NULL)
						{
							bitmap_info* bi = g.m_fontlib_glyph.get_ptr();

							if(bi == gtc->get_bitmap_info())
							{
								// TrueType Font
								gtc->get_glyph_region(g.m_code, g.m_fe, g.m_fontsize, nofilter, uv_bounds);
							}
							else
							{
								// Bitmap Font
								bgtc->get_glyph_region(g.m_code, g.m_fe, g.m_fontsize, uv_bounds);
							}

							xscale = uv_bounds.width() * s_EM_scale;
							yscale = uv_bounds.height() * s_EM_scale;

							if((blurx > 0 || blury > 0) && bi == gtc->get_bitmap_info())
							{
								// adjust bounds for blur
								rect org_bounds;
								gtc->get_glyph_region(g.m_code, g.m_fe, g.m_fontsize, filter, org_bounds);

								float w = uv_bounds.width();
								float h = uv_bounds.height();
								uv_bounds.m_x_min = org_bounds.m_x_min;
								uv_bounds.m_x_max = org_bounds.m_x_min + w;
								uv_bounds.m_y_min = org_bounds.m_y_min;
								uv_bounds.m_y_max = org_bounds.m_y_min + h;

								bounds.m_x_max += blurx2 / uv_bounds.width();
								bounds.m_y_max += blury2 / uv_bounds.height();

								//uv_bounds.m_x_min -= 1.0f * stroke;
								uv_bounds.m_x_max -= (1.0f-g.m_bounds.m_x_max) * uv_bounds.width() - blurx2;
								uv_bounds.m_y_max -= (1.0f-g.m_bounds.m_y_max) * uv_bounds.height() - blury2;
							}
							else if(stroke && bi == gtc->get_bitmap_info())
							{
								// adjust bounds for stroke
								rect org_bounds;
								glyph_texture_cache::filter_info filter;
								filter.stroke = stroke;
								filter.blurx = 0;
								filter.blury = 0;
								gtc->get_glyph_region(g.m_code, g.m_fe, g.m_fontsize, filter, org_bounds);

								float w = uv_bounds.width();
								float h = uv_bounds.height();
								uv_bounds.m_x_min = org_bounds.m_x_min;
								uv_bounds.m_x_max = org_bounds.m_x_min + w;
								uv_bounds.m_y_min = org_bounds.m_y_min;
								uv_bounds.m_y_max = org_bounds.m_y_min + h;

								bounds.m_x_max += stroke2 / uv_bounds.width();
								bounds.m_y_max += stroke2 / uv_bounds.height();

								//uv_bounds.m_x_min -= 1.0f * stroke;
								uv_bounds.m_x_max -= (1.0f-g.m_bounds.m_x_max) * uv_bounds.width() - stroke2;
								uv_bounds.m_y_max -= (1.0f-g.m_bounds.m_y_max) * uv_bounds.height() - stroke2;
							}
							else
							{
								uv_bounds.m_x_max -= (1.0f-g.m_bounds.m_x_max) * uv_bounds.width();
								uv_bounds.m_y_max -= (1.0f-g.m_bounds.m_y_max) * uv_bounds.height();
							}

							uv_bounds.m_x_min /= bi->get_width();
							uv_bounds.m_y_min /= bi->get_height();
							uv_bounds.m_x_max /= bi->get_width();
							uv_bounds.m_y_max /= bi->get_height();

#ifdef FLIP_TCOORD_Y
							uv_bounds.m_y_min = 1.0f - uv_bounds.m_y_min;
							uv_bounds.m_y_max = 1.0f - uv_bounds.m_y_max;
#endif
						}
						else
						{
							uv_bounds.m_x_min = 0;
							uv_bounds.m_y_min = 0;
							uv_bounds.m_x_max = g.m_bounds.m_x_max;
							uv_bounds.m_y_max = g.m_bounds.m_y_max;
							
							xscale = g.m_fontlib_glyph->get_width() * s_EM_scale;
							yscale = g.m_fontlib_glyph->get_height() * s_EM_scale;
						}

						if (fnt && fnt->is_define_font3())
						{
							xscale *= 20.0f;
							yscale *= 20.0f;
						}

						bounds.m_x_min *= xscale;
						bounds.m_x_max *= xscale;
						bounds.m_y_min *= yscale;
						bounds.m_y_max *= yscale;

						handler->draw_bitmap(mat,
							g.m_fontlib_glyph.get_ptr(),
							bounds,
							uv_bounds,
							transformed_color);
					}

				}
				else
				if (g.m_glyph_index >= 0)
				{
					// static text
					shape_character_def* sh = fnt->get_glyph_by_index(g.m_glyph_index);
					if (sh)
					{
						sh->display(mat, cx, pixel_scale, dummy_style, dummy_line_style);
					}
				}

				x += rec.m_glyphs[j].m_glyph_advance;

			}
		}
	}


	text_character_def::text_character_def(player* player, movie_definition_sub* root_def) :
		character_def(player),
		m_root_def(root_def),
		m_use_flashtype(false),
		m_grid_fit(0),
		m_thickness(0.0f),
		m_sharpness(0.0f)
	{
		assert(m_root_def);
	}

	void	text_character_def::read(stream* in, int tag_type, movie_definition_sub* m)
	{
		assert(m != NULL);
		assert(tag_type == 11 || tag_type == 33);

		m_rect.read(in);
		m_matrix.read(in);

		int	glyph_bits = in->read_u8();
		int	advance_bits = in->read_u8();

		IF_VERBOSE_PARSE(log_msg("begin text records\n"));

		bool	last_record_was_style_change = false;

		text_style	style;
		for (;;)
		{
			int	first_byte = in->read_u8();

			if (first_byte == 0)
			{
				// This is the end of the text records.
				IF_VERBOSE_PARSE(log_msg("end text records\n"));
				break;
			}

			// Style changes and glyph records just alternate.
			// (Contrary to what most SWF references say!)
			if (last_record_was_style_change == false)
			{
				// This is a style change.

				last_record_was_style_change = true;

				bool	has_font = (first_byte >> 3) & 1;
				bool	has_color = (first_byte >> 2) & 1;
				bool	has_y_offset = (first_byte >> 1) & 1;
				bool	has_x_offset = (first_byte >> 0) & 1;

				IF_VERBOSE_PARSE(log_msg("  text style change\n"));

				if (has_font)
				{
					Uint16	font_id = in->read_u16();
					style.m_font_id = font_id;
					IF_VERBOSE_PARSE(log_msg("  has_font: font id = %d\n", font_id));
				}
				if (has_color)
				{
					if (tag_type == 11)
					{
						style.m_color.read_rgb(in);
					}
					else
					{
						assert(tag_type == 33);
						style.m_color.read_rgba(in);
					}
					IF_VERBOSE_PARSE(log_msg("  has_color\n"));
				}
				if (has_x_offset)
				{
					style.m_has_x_offset = true;
					style.m_x_offset = in->read_s16();
					IF_VERBOSE_PARSE(log_msg("  has_x_offset = %g\n", style.m_x_offset));
				}
				else
				{
					style.m_has_x_offset = false;
					style.m_x_offset = 0.0f;
				}
				if (has_y_offset)
				{
					style.m_has_y_offset = true;
					style.m_y_offset = in->read_s16();
					IF_VERBOSE_PARSE(log_msg("  has_y_offset = %g\n", style.m_y_offset));
				}
				else
				{
					style.m_has_y_offset = false;
					style.m_y_offset = 0.0f;
				}
				if (has_font)
				{
					style.m_text_height = in->read_u16();
					IF_VERBOSE_PARSE(log_msg("  text_height = %g\n", style.m_text_height));
				}
			}
			else
			{
				// Read the glyph record.

				last_record_was_style_change = false;

				int	glyph_count = first_byte;

				// 					if (! last_record_was_style_change)
				// 					{
				// 						glyph_count &= 0x7F;
				// 					}
				// 					// else { Don't mask the top bit; the first record is allowed to have > 127 glyphs. }

				m_text_glyph_records.resize(m_text_glyph_records.size() + 1);
				m_text_glyph_records.back().m_style = style;
				m_text_glyph_records.back().read(in, glyph_count, glyph_bits, advance_bits);

				IF_VERBOSE_PARSE(log_msg("  glyph_records: count = %d\n", glyph_count));
			}
		}
	}

	void	text_character_def::display(character* inst)
	// Draw the string.
	{
		if (inst->is_visible() == false)
		{
			// We're invisible, so don't display!
			return;
		}

		display_glyph_records(&m_matrix, inst, m_text_glyph_records, m_root_def, NULL, 0, 0, 0);
	}

	void	text_character_def::csm_textsetting(stream* in, int tag_type)
	{
		assert(tag_type == 74);	// Flash 8

		m_use_flashtype = in->read_uint(2) == 0 ? false : true;

		// 0 = Do not use grid fitting. AlignmentZones and LCD sub-pixel information
		// will not be used.
		// 1 = Pixel grid fit. Only supported for left-aligned dynamic text.
		// This setting provides the ultimate in FlashType readability, with
		// crisp letters aligned to pixels.
		// 2 = Sub-pixel grid fit. Align letters to the 1/3 pixel used by LCD monitors.
		// Can also improve quality for CRT output.
		m_grid_fit = in->read_uint(3);

		in->read_uint(3); // reserved;

		m_thickness = in->read_fixed();
		m_sharpness = in->read_fixed();

		in->read_u8();	// reserved

		IF_VERBOSE_PARSE(
			log_msg("reading CSMTextSetting tag\n");
		log_msg("	m_use_flashtype = %s\n", m_use_flashtype ? "true" : "false");
		log_msg("	m_grid_fit = %d\n", m_grid_fit);
		log_msg("	m_thickness = %f\n", m_thickness);
		log_msg("	m_sharpness = %f\n", m_sharpness);
		);

	}

	void text_character_def::get_bound(rect* bound)
	{
		*bound = m_rect;
	}

	void	define_text_loader(stream* in, int tag_type, movie_definition_sub* m)
	// Read a DefineText tag.
	{
		assert(tag_type == 11 || tag_type == 33);

		Uint16	character_id = in->read_u16();

		text_character_def*	ch = swfnew text_character_def(m->get_player(), m);
		IF_VERBOSE_PARSE(log_msg("text_character, id = %d\n", character_id));
		ch->read(in, tag_type, m);

		// IF_VERBOSE_PARSE(print some stuff);

		m->add_character(character_id, ch);
	}


	//
	// edit_text_character_def
	//

	void removeHTML(tu_string& str)
	{
		const char* s = str.c_str();
		const char* end = strstr(s, "</");
		const char* start = end;
		char temp[512];
		
		if(str.size() < 0)
			return;

		while(start >-- s)
		{
			if(*start == '>')
			{
				start++;
				int length = end - start;
				if(length > 0)
				{
					assert(length < 512);
					memcpy(temp, start, length);
					temp[length] = '\0';
					str = temp;
				}
				else
				{
					str = "";
				}
				break;
			}
			start--;
		}
	}

	// creates empty dynamic text field
	edit_text_character_def::edit_text_character_def(player* player, int width, int height) :
		character_def(player),
		m_root_def(NULL),
		m_word_wrap(false),
		m_multiline(false),
		m_password(false),
		m_readonly(false),
		m_auto_size(false),
		m_no_select(false),
		m_border(false),
		m_html(false),
		m_use_outlines(false),
		m_font_id(-1),
		m_font(NULL),
		m_text_height(PIXELS_TO_TWIPS(12)),	//	default 12
		m_max_length(0),
		m_alignment(ALIGN_LEFT),
		m_left_margin(0.0f),
		m_right_margin(0.0f),
		m_indent(0.0f),
		m_leading(0.0f),
		m_use_flashtype(false),
		m_grid_fit(0),
		m_thickness(0.0f),
		m_sharpness(0.0f)
	{
		m_rect.m_x_min = 0;
		m_rect.m_y_min = 0;
		m_rect.m_x_max = PIXELS_TO_TWIPS(width);
		m_rect.m_y_max = PIXELS_TO_TWIPS(height);

		m_color.set(0, 0, 0, 255);
		m_font = swfnew font(player);
	}

	edit_text_character_def::edit_text_character_def(player* player, movie_definition_sub* root_def) :
		character_def(player),
		m_root_def(root_def),
		m_word_wrap(false),
		m_multiline(false),
		m_password(false),
		m_readonly(false),
		m_auto_size(false),
		m_no_select(false),
		m_border(false),
		m_html(false),
		m_use_outlines(false),
		m_font_id(-1),
		m_font(NULL),
		m_text_height(PIXELS_TO_TWIPS(12)),	//	default 12
		m_max_length(0),
		m_alignment(ALIGN_LEFT),
		m_left_margin(0.0f),
		m_right_margin(0.0f),
		m_indent(0.0f),
		m_leading(0.0f),
		m_use_flashtype(false),
		m_grid_fit(0),
		m_thickness(0.0f),
		m_sharpness(0.0f)
	{
		assert(m_root_def);
		m_color.set(0, 0, 0, 255);
	}

	edit_text_character_def::~edit_text_character_def()
	{
	}

	void	edit_text_character_def::read(stream* in, int tag_type, movie_definition_sub* m)
	{
		assert(m != NULL);
		assert(tag_type == 37);

		m_rect.read(in);

		in->align();
		bool	has_text = in->read_uint(1) ? true : false;
		m_word_wrap = in->read_uint(1) ? true : false;
		m_multiline = in->read_uint(1) ? true : false;
		m_password = in->read_uint(1) ? true : false;
		m_readonly = in->read_uint(1) ? true : false;
		bool	has_color = in->read_uint(1) ? true : false;
		bool	has_max_length = in->read_uint(1) ? true : false;
		bool	has_font = in->read_uint(1) ? true : false;

		in->read_uint(1);	// reserved
		m_auto_size = in->read_uint(1) ? true : false;
		bool	has_layout = in->read_uint(1) ? true : false;
		m_no_select = in->read_uint(1) ? true : false;
		m_border = in->read_uint(1) ? true : false;
		in->read_uint(1);	// reserved
		m_html = in->read_uint(1) ? true : false;
		m_use_outlines = in->read_uint(1) ? true : false;

		if (has_font)
		{
			m_font_id = in->read_u16();
			m_text_height = (float) in->read_u16();
		}

		if (has_color)
		{
			m_color.read_rgba(in);
		}

		if (has_max_length)
		{
			m_max_length = in->read_u16();
		}

		if (has_layout)
		{
			m_alignment = (alignment) in->read_u8();
			m_left_margin = (float) in->read_u16();
			m_right_margin = (float) in->read_u16();
			m_indent = (float) in->read_s16();
			m_leading = (float) in->read_s16();
		}

		in->read_string(&m_var_name);

		if (has_text)
		{
			in->read_string(&m_default_text);
			// remove HTML code
			removeHTML(m_default_text);
		}

		IF_VERBOSE_PARSE(log_msg("edit_text_char, varname = %s, text = %s\n",
			m_var_name.c_str(), m_default_text.c_str()));
	}

	void	edit_text_character_def::csm_textsetting(stream* in, int tag_type)
	{
		assert(tag_type == 74);	// Flash 8
		m_use_flashtype = in->read_uint(2) == 0 ? false : true;

		// 0 = Do not use grid fitting. AlignmentZones and LCD sub-pixel information
		// will not be used.
		// 1 = Pixel grid fit. Only supported for left-aligned dynamic text.
		// This setting provides the ultimate in FlashType readability, with
		// crisp letters aligned to pixels.
		// 2 = Sub-pixel grid fit. Align letters to the 1/3 pixel used by LCD monitors.
		// Can also improve quality for CRT output.
		m_grid_fit = in->read_uint(3);

		in->read_uint(3); // reserved;

		m_thickness = in->read_fixed();
		m_sharpness = in->read_fixed();

		in->read_u8();	// reserved

		IF_VERBOSE_PARSE(
			log_msg("reading CSMTextSetting tag\n");
		log_msg("	m_use_flashtype = %s\n", m_use_flashtype ? "true" : "false");
		log_msg("	m_grid_fit = %d\n", m_grid_fit);
		log_msg("	m_thickness = %f\n", m_thickness);
		log_msg("	m_sharpness = %f\n", m_sharpness);
		);

	}

	//
	// edit_text_character
	//

	void	set_textformat(const fn_call& fn)
	{
		edit_text_character* et = cast_to<edit_text_character>(fn.this_ptr);
		assert(et);
		if (fn.nargs == 1)
		{
			as_textformat* tf = cast_to<as_textformat>(fn.arg(0).to_object());
			et->reset_format(tf);
		}
	}

	void	get_textformat(const fn_call& fn)
	{
		edit_text_character* et = cast_to<edit_text_character>(fn.this_ptr);
		assert(et);
		
		as_global_textformat_ctor(fn);

		fn.result->to_object()->set_member("leftMargin", as_value(TWIPS_TO_PIXELS(et->m_left_margin)));
		fn.result->to_object()->set_member("indent", as_value(TWIPS_TO_PIXELS(et->m_indent)));
		fn.result->to_object()->set_member("rightMargin", as_value(TWIPS_TO_PIXELS(et->m_right_margin)));
		fn.result->to_object()->set_member("leading", as_value(TWIPS_TO_PIXELS(et->m_leading)));
		fn.result->to_object()->set_member("letterSpacing", as_value(TWIPS_TO_PIXELS(et->m_letter_spacing)));
		fn.result->to_object()->set_member("color", as_value(et->m_color.get()));
		fn.result->to_object()->set_member("size", as_value(TWIPS_TO_PIXELS(et->m_text_height)));
		
		if(et->m_alignment == edit_text_character_def::ALIGN_LEFT)
			fn.result->to_object()->set_member("align", fn.get_player()->get_const_string("left"));
		else if(et->m_alignment == edit_text_character_def::ALIGN_CENTER)
			fn.result->to_object()->set_member("align", fn.get_player()->get_const_string("center"));
		else if(et->m_alignment == edit_text_character_def::ALIGN_RIGHT)
			fn.result->to_object()->set_member("align", fn.get_player()->get_const_string("right"));
		else if(et->m_alignment == edit_text_character_def::ALIGN_JUSTIFY)
			fn.result->to_object()->set_member("align", fn.get_player()->get_const_string("justify"));

		fn.result->to_object()->set_member("font", fn.get_player()->get_const_string(et->m_font->get_name()));
		fn.result->to_object()->set_member("bold", as_value(et->m_font->is_bold()));
		fn.result->to_object()->set_member("italic", as_value(et->m_font->is_italic()));
	}

	edit_text_character::edit_text_character(player* player, character* parent,
			edit_text_character_def* def, int id)	:
		character(player, parent, id, AS_EDIT_TEXT),
		m_def(def),
		m_has_focus(false), 
		m_cursor(0), 
		m_xcursor(0.0f), 
		m_ycursor(0.0f),
		m_xglobal(0.0f),
		m_yglobal(0.0f),
		m_last_line_start_record(0),
		m_last_space_record(0),
		m_last_space_glyph(-1)
	{
		assert(parent);
		assert(m_def != NULL);

		init();
	}

	edit_text_character::~edit_text_character() 
	{ 
		// on_event(event_id::KILLFOCUS) will be executed
		// during remove_display_object()
	} 

	void edit_text_character::recycle(character* parent, int id)
	{
		character::recycle(parent, id);
		init();
	};

	void edit_text_character::init()
	{
		m_has_focus = false;
		m_cursor = 0;
		m_xcursor = 0.0f;
		m_ycursor = 0.0f;
		m_xglobal = 0.0f;
		m_yglobal = 0.0f;
		m_last_line_start_record = 0;
		m_last_space_record = 0;
		m_last_space_glyph = -1;

		// defaults
		m_color = m_def->m_color;
		m_text_height = m_def->m_text_height;
		m_font = m_def->m_font;
		m_alignment = m_def->m_alignment;
		m_left_margin = m_def->m_left_margin;
		m_right_margin = m_def->m_right_margin;
		m_indent = m_def->m_indent;
		m_leading = m_def->m_leading;
		m_letter_spacing = 0;
		m_background_color.set(255, 255, 255, 255);

		// first set default text value
		set_text(m_def->m_default_text.c_str());

		// then reset VAR / TEXT_FIELD value
		set_text_value(to_string());

		m_dummy_style.clear();
		m_dummy_style.push_back(fill_style());
		//reset_bounding_box(0, 0);

		m_need_advance = get_var_name().size() > 0;
	}

	void edit_text_character::reset_format(as_textformat* tf)
	{
		assert(tf);
		as_value val;

		if (tf->get_member("leftMargin", &val))
		{
			m_left_margin = PIXELS_TO_TWIPS(val.to_float());
		}

		if (tf->get_member("indent", &val))
		{
			m_indent = PIXELS_TO_TWIPS(val.to_float());
		}

		if (tf->get_member("rightMargin", &val))
		{
			m_right_margin = PIXELS_TO_TWIPS(val.to_float());
		}

		if (tf->get_member("leading", &val))
		{
			m_leading = PIXELS_TO_TWIPS(val.to_float());
		}

		if (tf->get_member("letterSpacing", &val))
		{
			m_letter_spacing = PIXELS_TO_TWIPS(val.to_float());
		}

		if (tf->get_member("color", &val))
		{
			int rgb = val.to_int();
			Uint8 r = (rgb >> 16) & 0xFF;
			Uint8 g = (rgb >> 8) & 0xFF;
			Uint8 b = rgb & 0xFF;
			m_color.set(r, g, b, 255);
		}

		if (tf->get_member("size", &val))
		{
			m_text_height = PIXELS_TO_TWIPS(val.to_float());
		}

		if (tf->get_member("align", &val))
		{
			if (val.to_tu_string() == "left")
			{
				m_alignment = edit_text_character_def::ALIGN_LEFT;
			}
			else
			if (val.to_tu_string() == "center")
			{
				m_alignment = edit_text_character_def::ALIGN_CENTER;
			}
			else
			if (val.to_tu_string() == "right")
			{
				m_alignment = edit_text_character_def::ALIGN_RIGHT;
			}
			else
			if (val.to_tu_string() == "justify")
			{
				m_alignment = edit_text_character_def::ALIGN_JUSTIFY;
			}
		}

		tu_string fontname = m_font->get_name();
		if (tf->get_member("font", &val))
		{
			fontname = val.to_tu_string();
		}

		bool bold = m_font->is_bold();
		if (tf->get_member("bold", &val))
		{
			bold = val.to_bool();
		}

		bool italic  = m_font->is_italic();
		if (tf->get_member("italic", &val))
		{
			italic = val.to_bool();
		}

		if (italic  != m_font->is_italic() ||
				bold  != m_font->is_bold() ||
				fontname  != m_font->get_name())
		{
			// try to find embedded font
			character_def* res = find_exported_resource(fontname);
			font* embedded_font = NULL;
			if (res)
			{
				embedded_font = cast_to<font>(res);
			}

			if (embedded_font)
			{
				// we have embedded font
				m_font = cast_to<font>(embedded_font);
			}
			else
			{
				m_font = swfnew font(get_player());
			}
			m_font->set_bold(bold);
			m_font->set_italic(italic);
			m_font->set_name(fontname);
		}

		format_text();
	}

	root* edit_text_character::get_root()
	{
		return get_parent()->get_root(); 
	} 

	void edit_text_character::show_cursor() 
	{ 
		coord_component x = (coord_component) m_xcursor; 
		coord_component y = (coord_component) m_ycursor; 
		coord_component h = (coord_component) m_text_height;

		coord_component box[4]; 
		box[0] = x; 
		box[1] = y; 
		box[2] = x; 
		box[3] = y + h; 

		matrix mat = get_world_matrix(); 
		render::set_matrix(mat); 
		render::line_style_color(rgba(255, 0, 0, 255));   // red cursor 
		render::line_style_width(PIXELS_TO_TWIPS(2));
		render::draw_line_strip(box, 2);        // draw line 
	} 

	void edit_text_character::display() 
	{ 
		if (is_visible() == false)
		{
			// We're invisible, so don't display!
			return;
		}

		root* root = m_player->get_root();
		if(root->m_text_buffering_enabled && !root->m_text_buffering_render)
		{
			// buffer the text field
			root->m_buffered_text_dlist.push_back(this);
			return;
		}

		if (m_def->m_border == true) 
		{ 
			matrix  mat = get_world_matrix(); 

			// @@ hm, should we apply the color xform?  It seems logical; need to test. 
			// cxform       cx = get_world_cxform(); 

			// Show white background + black bounding box. 
			render::set_matrix(mat); 

			point   coords[4]; 
			coords[0] = m_def->m_rect.get_corner(0); 
			coords[1] = m_def->m_rect.get_corner(1); 
			coords[2] = m_def->m_rect.get_corner(3); 
			coords[3] = m_def->m_rect.get_corner(2); 

			coord_component icoords[18] = 
			{ 
				// strip (fill in) 
				(coord_component) coords[0].m_x, (coord_component) coords[0].m_y, 
				(coord_component) coords[1].m_x, (coord_component) coords[1].m_y, 
				(coord_component) coords[2].m_x, (coord_component) coords[2].m_y, 
				(coord_component) coords[3].m_x, (coord_component) coords[3].m_y, 

				// outline 
				(coord_component) coords[0].m_x, (coord_component) coords[0].m_y, 
				(coord_component) coords[1].m_x, (coord_component) coords[1].m_y, 
				(coord_component) coords[3].m_x, (coord_component) coords[3].m_y, 
				(coord_component) coords[2].m_x, (coord_component) coords[2].m_y, 
				(coord_component) coords[0].m_x, (coord_component) coords[0].m_y, 
			}; 

			render::fill_style_color(0,	m_background_color);
			render::draw_mesh_strip(&icoords[0], 4); 

			render::line_style_color(rgba(0,0,0,255)); 
			render::line_style_width(0);
			render::draw_line_strip(&icoords[8], 5); 
		} 

		if(m_player->m_context->m_glyph_provider->get_scale() != 1.0f)
			render::set_text_pixel_alignment(false);
		else
			render::set_text_pixel_alignment(m_def->m_grid_fit != 0);
		
		if(m_player->m_render_caching && m_render_cache.is_valid(this))
		{
			render::draw(&m_render_cache);
		}
		else
		{
			if(m_player->m_render_caching)
			{
				render::set_render_cache(&m_render_cache);
			}

			//printf("update text cache: %s\n", m_text.c_str());
			matrix this_mat;
#if 0
			if(m_def->m_rect.get_corner(0).m_x != -40.0f || m_def->m_rect.get_corner(0).m_y != -40.0f)
			{
				// for an unknown reason with static textfields converted to dynamic,
				// flash somtime exports position strangely that needs to be re-adjusted
				this_mat.concatenate_translation(m_def->m_rect.get_corner(0).m_x + 40.0f, m_def->m_rect.get_corner(0).m_y + 40.0f); 
			}
#endif

			//if(m_def->m_grid_fit != 0)
			//{
			//	const matrix& mat = get_world_matrix();
			//	float dx = (float)((((int)mat.get_raw_x() + 10) / 20) * 20) - mat.get_raw_x();
			//	float dy = (float)((((int)mat.get_raw_y() + 10) / 20) * 20) - mat.get_raw_y();
			//	if(dx != 0.0f || dy != 0.0f)
			//		this_mat.concatenate_translation(dx, 0);
			//}

			bool render_normal = true;

			// render glow/stroke effect??
			if(filter_engine::get_instance() == NULL)
			{
				character* ch_effect = this;
				while(ch_effect != NULL)
				{
					if(ch_effect->get_effect().m_filters.size() > 0)
						break;
					ch_effect = ch_effect->get_parent();
				}
				if(ch_effect != NULL)
				{
					for(int i = ch_effect->get_effect().m_filters.size() - 1; i >= 0; i--)
					{
						const filter& params = ch_effect->get_effect().m_filters[i];

						if(params.m_id == filter::GLOW)
						{
							int maxx = (int)params.blur.blur_x;
							int maxy = (int)params.blur.blur_y;

							float offsetx = (float)-maxx;
							float offsety = (float)-maxy;

							rgba effect_color;
							effect_color.m_b = params.glow.glow_color[0];
							effect_color.m_g = params.glow.glow_color[1];
							effect_color.m_r = params.glow.glow_color[2];
							effect_color.m_a = params.glow.glow_color[3];
							int strenght = params.glow.strength;

							unsigned char stroke = (unsigned char)fmax(params.blur.blur_x, params.blur.blur_y);
							const int max = imax(imax(iabs(maxx), iabs(maxy)), 1);
							float factor = strenght / 10.0f;
							//factor *= (1 + max - factor * max) / max;
							//factor *= factor;
							effect_color.m_a = imin((int)(effect_color.m_a * factor), 255);

							if(effect_color.m_a > 0)
							{
								matrix mat = this_mat;
								mat.concatenate_translation(PIXELS_TO_TWIPS(offsetx), PIXELS_TO_TWIPS(offsety));
								display_glyph_records(&mat, this, m_text_glyph_records, m_def->m_root_def, &effect_color, stroke, 0, 0);
							}
						}
						else if(params.m_id == filter::DROP_SHADOW)
						{
							int maxx = (int)params.blur.blur_x;
							int maxy = (int)params.blur.blur_y;

							float offsetx = -maxx + params.drop_shadow.distance * cos(params.drop_shadow.angle);
							float offsety = -maxy + params.drop_shadow.distance * sin(params.drop_shadow.angle);

							rgba effect_color;
							effect_color.m_b = params.drop_shadow.drop_shadow_color[0];
							effect_color.m_g = params.drop_shadow.drop_shadow_color[1];
							effect_color.m_r = params.drop_shadow.drop_shadow_color[2];
							effect_color.m_a = params.drop_shadow.drop_shadow_color[3];

							matrix mat = this_mat;
							mat.concatenate_translation(PIXELS_TO_TWIPS(offsetx), PIXELS_TO_TWIPS(offsety));
							display_glyph_records(&mat, this, m_text_glyph_records, m_def->m_root_def, &effect_color, 0, maxx, maxy);
						}
						else if(params.m_id == filter::BLUR)
						{
							unsigned char blurx = (unsigned char)params.blur.blur_x;
							unsigned char blury = (unsigned char)params.blur.blur_y;

							int offsetx = (int)-blurx;
							int offsety = (int)-blury;

							if(blurx > 0 || blury > 0)
							{
								matrix mat = this_mat;
								mat.concatenate_translation(PIXELS_TO_TWIPS(offsetx), PIXELS_TO_TWIPS(offsety));
								display_glyph_records(&mat, this, m_text_glyph_records, m_def->m_root_def, NULL, 0, blurx, blury);
								render_normal = false;
							}
						}
					}
				}
			}

			// Draw our actual text. 
			if(render_normal && m_text_glyph_records.size() > 0)
			{
				display_glyph_records(NULL, this, m_text_glyph_records, m_def->m_root_def, NULL, 0, 0, 0);
			}

			if(m_player->m_render_caching)
			{
				render::set_render_cache(NULL);
			}
		}

		if (m_has_focus) 
		{ 
			show_cursor(); 
		} 

		if(has_display_callback())
		{
			do_display_callback();
		}
	} 

	bool edit_text_character::on_event(const event_id& id) 
	{ 
		if (m_def->m_readonly == true) 
		{ 
			return false; 
		} 

		switch (id.m_id) 
		{ 
			case event_id::SETFOCUS: 
			{ 
				get_root()->set_active_entity(this);
				if (m_has_focus == false) 
				{ 
					// Invoke user defined event handler
					as_value function;
					if (get_member("onSetFocus", &function))
					{
						as_environment env(get_player());
						env.push(as_value());	// oldfocus, TODO
						gameswf::call_method(function, &env, this, 1, env.get_top_index());
					}

					get_root()->m_keypress_listener.add(this);
					m_has_focus = true; 
					m_cursor = m_text.size(); 
					format_text(); 
				} 
				break; 
			} 

		case event_id::KILLFOCUS: 
			{ 
				if (m_has_focus == true) 
				{ 
					// Invoke user defined event handler
					as_value function;
					if (get_member("onKillFocus", &function))
					{
						as_environment env(get_player());
						env.push(as_value());	// newfocus, TODO
						gameswf::call_method(function, &env, this, 1, env.get_top_index());
					}

					m_has_focus = false; 
					get_root()->m_keypress_listener.remove(this); 
					format_text(); 
				} 
				break; 
			} 

		case event_id::KEY_PRESS: 
			{ 
				tu_string s(m_text); 

				// may be m_text is changed in ActionScript 
				m_cursor = imin(m_cursor, m_text.size()); 

				switch (id.m_key_code) 
				{ 
				case key::BACKSPACE: 
					if (m_cursor > 0) 
					{ 
						s.erase(m_cursor - 1, 1); 
						m_cursor--; 
						set_text_value(s); 
					} 
					break; 

				case key::DELETEKEY: 
					if (s.size() > m_cursor) 
					{ 
						s.erase(m_cursor, 1); 
						set_text_value(s); 
					} 
					break; 

				case key::HOME: 
				case key::PGUP: 
				case key::UP: 
					m_cursor = 0; 
					format_text(); 
					break; 

				case key::END: 
				case key::PGDN: 
				case key::DOWN: 
					m_cursor = m_text.size(); 
					format_text(); 
					break; 

				case key::LEFT: 
					m_cursor = m_cursor > 0 ? m_cursor - 1 : 0; 
					format_text(); 
					break; 

				case key::RIGHT: 
					m_cursor = m_cursor < m_text.size() ? m_cursor + 1 : m_text.size(); 
					format_text(); 
					break; 

				default: 
					{ 
						s.insert(m_cursor, id.m_key_code); 
						m_cursor++; 
						set_text_value(s); 
						break; 
					} 
				} 
			} 

		default: 
			return false; 
		} 
		return true; 
	} 

	bool edit_text_character::can_handle_mouse_event() 
	{
		return m_def->m_readonly == false;
	}

	character*  edit_text_character::get_topmost_mouse_entity(float x, float y) 
	{ 
		if (get_visible() == false) 
		{ 
			return NULL; 
		} 

		const matrix&  m = get_matrix();

		point   p; 
		m.transform_by_inverse(&p, point(x, y)); 

		if (m_def->m_rect.point_test(p.m_x, p.m_y)) 
		{ 
			return this; 
		} 
		return NULL; 
	} 

	bool edit_text_character::hit_test(float x, float y)
	{
		return get_topmost_mouse_entity(x, y) != NULL;
	}

	const tu_string&	edit_text_character::get_var_name() const
	{
		return m_def->m_var_name; 
	}

	void	edit_text_character::reset_bounding_box(float x, float y)
	// Reset our text bounding box to the given point.
	{
		m_text_bounding_box.m_x_min = x;
		m_text_bounding_box.m_x_max = x;
		m_text_bounding_box.m_y_min = y;
		m_text_bounding_box.m_y_max = y;
	}

	void edit_text_character::advance(float delta_time) 
	{ 
		to_string();
	}

	void	edit_text_character::set_text(const tu_string& new_text, bool html)
	// Local. Set our text to the given string.
	{
		if (m_text == new_text)
		{
			return;
		}

		m_text = new_text;
		if (m_def->m_max_length > 0	&& m_text.length() > m_def->m_max_length)
		{
			m_text.resize(m_def->m_max_length);
		}
		format_text(html);
	}

	void	edit_text_character::set_text_value(const tu_string& new_text, bool html)
	// Set our text to the given string.
	{
		// set our text
		set_text(new_text, html);

		// set VAR
		if (get_var_name().size() > 0)
		{
			as_object* ch = get_parent();	// target, default is parent
			tu_string path;
			tu_string var = get_var_name();
			if (as_environment::parse_path(get_var_name(), &path, &var))
			{
				ch = ch->find_target(path.c_str());
			}

			if (ch)
			{
				ch->set_member(var, new_text.c_str());
			}
		}
	}

	const char*	edit_text_character::to_string()
	{
		// get text from VAR
		if (get_var_name().size() > 0)
		{
			as_object* ch = get_parent();	// target, default is parent
			tu_string path;
			tu_string var = get_var_name();
			if (as_environment::parse_path(get_var_name(), &path, &var))
			{
				ch = ch->find_target(path.c_str());
			}

			if (ch)
			{
				as_value val;
				if (ch->get_member(var, &val) && val.to_object() != this)
				{
					if (val.to_tu_string() != m_text)
					{
						set_text(val.to_tu_string().c_str());
					}
				}
			}
		}

		return m_text.c_str();
	}


	bool	edit_text_character::set_member(const tu_stringi& name, const as_value& val)
	// We have a "text" member.
	{
		// first try text field properties
		as_standard_member	std_member = get_standard_member(name);
		switch (std_member)
		{
			default:
				break;

			case M_TEXT:
			{
				tu_string buffer;
				set_text_value(val.to_string(buffer));
				break;
			}

			case M_HTMLTEXT:
			{
				tu_string buffer;
				set_text_value(val.to_string(buffer), true);
				break;
			}

			case M_TEXTCOLOR:
			{
				// The arg is 0xRRGGBB format.
				rgba color(val.to_number());
				m_color.m_r = color.m_r;
				m_color.m_g = color.m_g;
				m_color.m_b = color.m_b;
				m_color.m_a = color.m_a;
				format_text();
				break;
			}

			case M_BORDER:
				m_def->m_border = val.to_bool();
				format_text();
				break;

			case M_MULTILINE:
				m_def->m_multiline = val.to_bool();
				format_text();
				break;

			case M_WORDWRAP:
				m_def->m_word_wrap = val.to_bool();
				format_text();
				break;

			case M_TYPE:
				// Specifies the type of text field.
				// There are two values: "dynamic", which specifies a dynamic text field
				// that cannot be edited by the user, and "input", 
				// which specifies an input text field.
				if (val.to_tu_stringi() == "input")
				{
					m_def->m_readonly = false;
				}
				else
				if (val.to_tu_stringi() == "dynamic")
				{
					m_def->m_readonly = true;
				}
				else
				{
					// log_error("not input &  dynamic");
				}
				break;

			case M_BACKGROUNDCOLOR:
				m_background_color = rgba(val.to_number());
				format_text();
				break;
		}

		return character::set_member(name, val);
	}


	bool	edit_text_character::get_member(const tu_stringi& name, as_value* val)
	{
		// first try text field properties
		as_standard_member	std_member = get_standard_member(name);
		switch (std_member)
		{
			default:
				break;

			case M_TEXT:
				val->set_tu_string(m_text);
				break;

			case M_HTMLTEXT:
				val->set_tu_string(m_text);
				break;

			case M_TEXTCOLOR:
				// Return color in 0xRRGGBB format
				val->set_int((m_color.m_r << 16) + (m_color.m_g << 8) + m_color.m_b);
				break;
		
			case M_TEXTWIDTH:
				// Return the width, in pixels, of the text as laid out.
				// (I.e. the actual text content, not our defined
				// bounding box.)
				//
				// In local coords.  Verified against Macromedia Flash.
				val->set_double(TWIPS_TO_PIXELS(m_text_bounding_box.width()));
				break;

			case M_TEXTHEIGHT:
				// Return the height, in pixels, of the text as laid out.
				// (I.e. the actual text content, not our defined
				// bounding box.)
				//
				// In local coords.  Verified against Macromedia Flash.
				val->set_double(TWIPS_TO_PIXELS(m_text_bounding_box.height()));
				break;

			case M_BORDER:
				val->set_bool(m_def->m_border);
				break;

			case M_MULTILINE:
				val->set_bool(m_def->m_multiline);
				break;
		
			case M_WORDWRAP:
				val->set_bool(m_def->m_word_wrap);
				break;

			case M_TYPE:
				val->set_tu_string(m_def->m_readonly ? "dynamic" : "input");
				break;

			case M_BACKGROUNDCOLOR:
				val->set_int(m_background_color.m_r << 16 | m_background_color.m_g << 8 |
					m_background_color.m_b);
				break;

		}

		// try built-ins sprite methods
		if (get_builtin(BUILTIN_EDIT_TEXT_METHOD, name, val))
		{
			return true;
		}

		return character::get_member(name, val);
	}

	// @@ WIDTH_FUDGE is a total fudge to make it match the Flash player!  Maybe
	// we have a bug?
#define WIDTH_FUDGE 80.0f


	void	edit_text_character::align_line(edit_text_character_def::alignment align, int last_line_start_record, float x)
	// Does LEFT/CENTER/RIGHT alignment on the records in
	// m_text_glyph_records[], starting with
	// last_line_start_record and going through the end of
	// m_text_glyph_records.
	{
		float	extra_space = (m_def->m_rect.width() - m_right_margin) - x - WIDTH_FUDGE;

		// extra_space may be < 0
		// assert(extra_space >= 0.0f);

		float	shift_right = 0.0f;

		if (align == edit_text_character_def::ALIGN_LEFT)
		{
			// Nothing to do; already aligned left.
			return;
		}
		else if (align == edit_text_character_def::ALIGN_CENTER)
		{
			// Distribute the space evenly on both sides.
			shift_right = extra_space / 2;
		}
		else if (align == edit_text_character_def::ALIGN_RIGHT)
		{
			// Shift all the way to the right.
			shift_right = extra_space;
		}

		// Shift the beginnings of the records on this line.
		for (int i = last_line_start_record; i < m_text_glyph_records.size(); i++)
		{
			text_glyph_record&	rec = m_text_glyph_records[i];

			if (rec.m_style.m_has_x_offset)
			{
				rec.m_style.m_x_offset += shift_right;
			}
		}

		// This fix the cursor position with centered texts
		// Thanks Francois Guibert
		m_xcursor += shift_right; 
	}

	struct html_reader
	{
		inline const char* rfindbound(const char* str)
		{
			while(*str == ' ')
				str--;
			return str;
		}

		inline const char* rfindchar(const char* str, int ch)
		{
			while(*str != ch)
				str--;
			return str;
		}

		bool parse_tag(stringi_hash<tu_string>& tag_attr, const char* tag_buffer)
		{
			if(tag_buffer[0] == '/')
				return false;
			
			const char* sep = strchr(tag_buffer, ' ');
			if(!sep)
				sep = strchr(tag_buffer, '/');
			if(!sep)
				sep = tag_buffer + strlen(tag_buffer);
			tu_string tag_name(tag_buffer, sep - tag_buffer);
			tag_attr.set(tu_string("name"), tag_name);

			const char* eq = strstr(tag_buffer, "=");
			while(eq)
			{
				const char* endname = rfindbound(eq);
				if(!endname)
					return false;
				const char* startname = rfindchar(eq, ' ');
				if(!startname)
					return false;
				const char* startvalueQuote = strchr(eq, '\"');
				const char* startvalueApp = strchr(eq, '\'');
				if(!startvalueQuote && !startvalueApp)
					return false;
				const char* startvalue = startvalueQuote;
				if(startvalueQuote && startvalueApp)
					startvalue = startvalueQuote < startvalueApp ? startvalueQuote : startvalueApp;
				else if(startvalueApp)
					startvalue = startvalueApp;
				const char* endvalue = strchr(startvalue + 1, *startvalue);
				if(!endvalue)
					return false;

				tu_string name(startname + 1, endname - startname - 1);
				tu_string value(startvalue + 1, endvalue - startvalue - 1);
				tag_attr.set(name, value);

				eq = strstr(endvalue, "=");
			}

			return true;
		}

		void parse(edit_text_character* edit_text_character)
		{
			tu_string& html = edit_text_character->m_text;
			if(html.length() == 0)
				return;

			edit_text_character::text_attributes default_attr;
			default_attr.m_color = edit_text_character->m_color;
			default_attr.m_font = edit_text_character->m_font.get_ptr();
			default_attr.m_size = (int)edit_text_character->m_text_height;
			m_attributes_stack.push_back(default_attr);
			
			int paragraph_count = 0;
			const char* buffer = html.c_str();
			for(int i = 0; i < html.size();)
			{
				if(buffer[i] == '<')
				{
					const char* end = strstr(&buffer[i], ">");
					if(end == NULL || i + 1 >= html.size())
						break;

					// closing tag?
					if(buffer[i + 1] == '/')
					{
						// remove style from stack
						if(m_attributes_stack.size() > 1)
							m_attributes_stack.pop_back();
					}
					else
					{
						char tag_buffer[512] = {0};
						memcpy(tag_buffer, &buffer[i + 1], end - &buffer[i] - 1);

						// parse tag and convert to attributes
						stringi_hash<tu_string> tag;
						parse_tag(tag, tag_buffer);

						// create style using current as base attributes
						edit_text_character::text_attributes attr;
						attr = m_attributes_stack.back();
						tu_string name;
						if(tag.get(tu_string("name"), &name))
						{
							if(name == "p")
							{
								// ignore aligment for now and use text field properties
								
								if(paragraph_count > 0)
								{
									edit_text_character->append_text(tu_string("\n"), m_attributes_stack.back(), true);
								}
								paragraph_count++;
							}
							else if(name == "font")
							{
								tu_string font_name;
								if(tag.get("face", &font_name))
								{
									font* f = swfnew font(edit_text_character->get_player());
									f->copy_from(attr.m_font.get_ptr());
									f->set_name(font_name);
									attr.m_font = f;
								}
								tu_string color;
								if(tag.get("color", &color) && color.size() > 0)
								{
									// parse hex?
									if(color.c_str()[0] == '#')
									{
										int rgb = 0xff000000;
										for(int k = color.size() - 1, order = 0; k >= 1; k--, order += 4)
										{
											char digit = tolower(color.c_str()[k]);
											if(digit >= '0' && digit <= '9')
												rgb |= (digit - '0') << order;
											else if(digit >= 'a' && digit <= 'f')
												rgb |= (digit - 'a' + 0x0A) << order;
										}
										attr.m_color = rgba(rgb);
									}
									else
									{
										int rgb = 0xff000000 | atoi(color.c_str());
										attr.m_color = rgba(rgb);
									}
								}
								tu_string size;
								if(tag.get("size", &size))
								{
									if(strchr(size.c_str(), '%') != NULL)
									{
										size.erase(size.size() - 1, 1);
										attr.m_size = m_attributes_stack.back().m_size * atoi(size.c_str()) / 100;
									}
									else if(strchr(size.c_str(), '+') == size.c_str())
									{
										attr.m_size = m_attributes_stack.back().m_size + (int)PIXELS_TO_TWIPS(atoi(size.c_str() + 1));
									}
									else if(strchr(size.c_str(), '-') == size.c_str())
									{
										attr.m_size = m_attributes_stack.back().m_size - (int)PIXELS_TO_TWIPS(atoi(size.c_str() + 1));
									}
									else if(atoi(size.c_str()) > 0)
									{
										attr.m_size = (int)PIXELS_TO_TWIPS(atoi(size.c_str()));
									}
								}
								m_attributes_stack.push_back(attr);
							}
							else if(name == "b")
							{
								font* f = swfnew font(edit_text_character->get_player());
								f->copy_from(attr.m_font.get_ptr());
								f->set_bold(true);
								attr.m_font = f;
								m_attributes_stack.push_back(attr);
							}
							else if(name == "i")
							{
								font* f = swfnew font(edit_text_character->get_player());
								f->copy_from(attr.m_font.get_ptr());
								f->set_italic(true);
								attr.m_font = f;
								m_attributes_stack.push_back(attr);
							}
							else if(name == "u")
							{
								attr.m_underline = true;
								m_attributes_stack.push_back(attr);
							}
							else if(name == "img")
							{
								tu_string src;
								tu_string width;
								tu_string height;
								tag.get("src", &src);
								tag.get("width", &width);
								tag.get("height", &height);
								edit_text_character->append_image(src, atoi(width.c_str()), atoi(height.c_str()));
							}
						}
					}
					i = end - buffer + 1;
				}
				else
				{
					const char* end = strstr(&buffer[i], "<");
					bool buffer_end = end == NULL;
					if(buffer_end)
						end = buffer + html.size();
					tu_string str(&buffer[i], end - &buffer[i]);
					edit_text_character->append_text(str, m_attributes_stack.back(), true);
					
					if(buffer_end)
						break;
					
					i = end - buffer;
				}
			}
		}

	private:
		array<edit_text_character::text_attributes> m_attributes_stack;
	};

	// parse html tags and delegates to append_record
	void	edit_text_character::format_text(bool html)
	{
		m_render_cache.reset();
		
		m_text_glyph_records.resize(0);
		m_xglobal = 0;
		m_yglobal = 0;
		m_last_line_start_record = 0;
		m_last_space_record = 0;
		m_last_space_glyph = -1;

		// Start the bbox at the upper-left corner of the first glyph.
		reset_bounding_box(0, 0);

		if (m_font == NULL)
		{
			return;
		}

		if(html)
		{
			html_reader reader;
			reader.parse(this);
		}
		else
		{
			text_attributes attr;
			attr.m_color = m_color;
			attr.m_font = m_font.get_ptr();
			attr.m_size =  (int)m_text_height;

			append_text(m_text, attr, false);
		}

		align_line(m_alignment, m_last_line_start_record, m_xglobal);

		// ML: workaround to perform vertical alignment using single line textbox
		if(m_def->m_multiline == false && m_text_glyph_records.size() > 1)
		{
			float maxHeight = 0;//m_text_glyph_records[0].m_style.m_text_height;
			for (int i = 0; i < m_text_glyph_records.size(); i++)
			{
				const text_glyph_record&	rec = m_text_glyph_records[i];
				if(rec.m_style.m_has_y_offset && rec.m_style.m_y_offset > maxHeight)
					maxHeight = rec.m_style.m_y_offset;
			}

			// apply half-height to all offsets
			float centerY = m_text_glyph_records[0].m_style.m_y_offset - m_text_glyph_records[0].m_style.m_text_height/2;
			float offsetAdjust = -maxHeight/2 + centerY;
			for (int i = 0; i < m_text_glyph_records.size(); i++)
			{
				text_glyph_record&	rec = m_text_glyph_records[i];
				if(rec.m_style.m_has_y_offset)
					rec.m_style.m_y_offset += offsetAdjust;
			}
		}
	}

	// Convert the characters in m_text into a series of
	// text_glyph_records to be rendered.
	void	edit_text_character::append_text(const tu_string& str, text_attributes& attributes, bool html)
	{

		// @@ mostly for debugging
		// Font substitution -- if the font has no
		// glyphs, try some other defined font!
//		if (m_font->get_glyph_count() == 0)
//		{
			// Find a better font.
//			font*	newfont = m_font.get_ptr();
//			for (int i = 0, n = fontlib::get_font_count(); i < n; i++)
//			{
//				font*	f = fontlib::get_font(i);
//				assert(f);

//				if (f->get_glyph_count() > 0)
//				{
					// This one looks good.
//					newfont = f;
//					break;
//				}
//			}

//			if (m_font != newfont)
//			{
//				log_error("error: substituting font!  font '%s' has no glyphs, using font '%s'\n",
//					fontlib::get_font_name(m_font.get_ptr()),
//					fontlib::get_font_name(newfont));

//				m_font = newfont;
//			}
//		}

		float	scale = attributes.m_size / (1024.0f * m_player->m_context->m_glyph_provider->get_scale());	// the EM square is 1024 x 1024

		// Flash 8
		// All the EMSquare coordinates are multiplied by 20 at export,
		// allowing fractional resolution to 1/20 of a unit.
		if (attributes.m_font->is_define_font3())
		{
			scale /= 20.0f;
		}

		float units_per_em = attributes.m_font->get_units_per_em();
		float real_text_height = attributes.m_size * (attributes.m_font->get_height() / units_per_em); // TTF height is relative to 2048
		if(real_text_height == 0.0f)
			real_text_height = (float)attributes.m_size;

		text_glyph_record	rec;	// one to work on

		float baseline = m_yglobal + attributes.m_size + (attributes.m_font->get_leading() - attributes.m_font->get_descent()) * scale;

		// continue from last record?
		if(m_text_glyph_records.size() > 0)
		{
			rec.m_style = m_text_glyph_records.back().m_style;

			// adjust baseline if lower than previous glyphs on the same line
			if(baseline > rec.m_style.m_y_offset)
			{
				for(int i = m_text_glyph_records.size() - 1; i >= 0 && m_text_glyph_records[i].m_style.m_y_offset == rec.m_style.m_y_offset; i--)
				{
					m_text_glyph_records[i].m_style.m_y_offset = baseline;
				}
				rec.m_style.m_y_offset = baseline;
			}
		}
		else
		{
			rec.m_style.m_y_offset = baseline;
		}
		
		// m_y_offset is the baseline
		rec.m_style.m_x_offset = fmax(0, m_left_margin + m_indent);
		rec.m_style.m_font = attributes.m_font;
		rec.m_style.m_color = attributes.m_color;
		rec.m_style.m_underline = attributes.m_underline;
		rec.m_style.m_text_height = (float)attributes.m_size;
		rec.m_style.m_has_x_offset = true;
		rec.m_style.m_has_y_offset = true;
		rec.m_style.m_has_font = true;

		rec.m_style.m_x_offset += m_xglobal;

		float x0 = rec.m_style.m_x_offset;
		float y0 = rec.m_style.m_y_offset;
		float x = rec.m_style.m_x_offset;
		float y = rec.m_style.m_y_offset;

		float	leading = m_leading;
		leading += attributes.m_font->get_leading() * scale;

		int	last_code = -1;
		int	last_space_glyph = -1;
		int character_idx = 0; 
		m_xcursor = x; 
		m_ycursor = y; 

		const char*	text = &str[0];
		while (Uint32 code = decode_next_unicode_character(&text))
		{
			// @@ try to truncate overflow text??
#if 0
			if (y + attributes.m_font->get_descent() * scale > m_def->m_rect.height())
			{
				// Text goes below the bottom of our bounding box.
				rec.m_glyphs.resize(0);
				break;
			}
#endif // 0

			//Uint16	code = m_text[j];

			x += attributes.m_font->get_kerning_adjustment(last_code, (int) code) * scale;
			//last_code = (int) code;

			if (code == 13 || code == 10)
			{
				// newline.

				// Frigging Flash seems to use '\r' (13) as its
				// default newline character.  If we get DOS-style \r\n
				// sequences, it'll show up as double newlines, so maybe we
				// need to detect \r\n and treat it as one newline.
				if ( code == 10 && last_code == 13)
				{
					last_code = (int) code;
					continue;
				}
				// Close out this stretch of glyphs.
				m_text_glyph_records.push_back(rec);
				align_line(m_alignment, m_last_line_start_record, x);

				x = fmax(0, m_left_margin + m_indent);	// swfnew paragraphs get the indent.
				y += real_text_height + leading;

				// Start a swfnew record on the next line.
				rec.m_glyphs.resize(0);
				rec.m_style.m_font = attributes.m_font;
				rec.m_style.m_color = attributes.m_color;
				rec.m_style.m_underline = attributes.m_underline;
				rec.m_style.m_x_offset = x;
				rec.m_style.m_y_offset = y;
				rec.m_style.m_text_height = (float)attributes.m_size;
				rec.m_style.m_has_x_offset = true;
				rec.m_style.m_has_y_offset = true;
				rec.m_style.m_has_font = true;

				m_last_space_glyph = -1;
				m_last_space_record = m_text_glyph_records.size();
				m_last_line_start_record = m_text_glyph_records.size();

				last_code = (int) code;
				continue;
			}

			if (code == 8)
			{
				// backspace (ASCII BS).

				// This is a limited hack to enable overstrike effects.
				// It backs the cursor up by one character and then continues
				// the layout.  E.g. you can use this to display an underline
				// cursor inside a simulated text-entry box.
				//
				// ActionScript understands the '\b' escape sequence
				// for inserting a BS character.
				//
				// ONLY WORKS FOR BACKSPACING OVER ONE CHARACTER, WON'T BS
				// OVER NEWLINES, ETC.

				if (rec.m_glyphs.size() > 0)
				{
					// Peek at the previous glyph, and zero out its advance
					// value, so the next char overwrites it.
					float	advance = rec.m_glyphs.back().m_glyph_advance;
					x -= advance;	// maintain formatting
					rec.m_glyphs.back().m_glyph_advance = 0;	// do the BS effect
				}
				last_code = (int) code;
				continue;
			}

			float code_scale = code == AUTO_LINE_FEED ? 0.0f : 1.0f;

			// Remember where word breaks occur.
			if (code == 32 || code == AUTO_LINE_FEED)
			{
				m_last_space_glyph = rec.m_glyphs.size();
				m_last_space_record = m_text_glyph_records.size();
				code = 32;
			}

			// convert non-breaking space to hard space without word breaking
			if (code == 0xA0)
			{
				code = 32;
			}

			if (code == '&' && html)
			{
				if(strncmp(text, "nbsp;", 5) == 0)
				{
					// convert non-breaking space to hard space without word breaking
					code = ' ';
					text += 5;
				}
			}

			// find glyph
			glyph	g;
			if (attributes.m_font->get_glyph(&g, (Uint16) code, (int) TWIPS_TO_PIXELS(attributes.m_size)) == false)
			{
				// error -- missing glyph!
				// Log an error, but don't log too many times.
				static int	s_log_count = 0;
				if (s_log_count < 10)
				{
					s_log_count++;
					log_error("edit_text_character::display() -- missing glyph for char %d "
						"-- make sure character shapes for font %s are being exported "
						"into your SWF file!\n",
						code,
						attributes.m_font->get_name().c_str());
				}
			}

			g.m_glyph_advance += m_letter_spacing;
			g.m_glyph_advance *= scale * code_scale;
			g.m_fontsize = (int) TWIPS_TO_PIXELS(attributes.m_size);
			g.m_code = code;

			rec.m_glyphs.push_back(g);

			x += g.m_glyph_advance;
			if (x >= m_def->m_rect.width() - m_right_margin - WIDTH_FUDGE)
			{
				// Whoops, we just exceeded the box width.  Do word-wrap.

				// Insert newline.

				// Close out this stretch of glyphs.
				m_text_glyph_records.push_back(rec);
				float	previous_x = x;

				x = m_left_margin;
				y += real_text_height + leading;

				// Start a swfnew record on the next line.
				rec.m_glyphs.resize(0);
				rec.m_style.m_font = attributes.m_font;
				rec.m_style.m_color = attributes.m_color;
				rec.m_style.m_underline = attributes.m_underline;
				rec.m_style.m_x_offset = x;
				rec.m_style.m_y_offset = y;
				rec.m_style.m_text_height = (float)attributes.m_size;
				rec.m_style.m_has_x_offset = true;
				rec.m_style.m_has_y_offset = true;

				text_glyph_record&	last_line = m_text_glyph_records.back();
				if (m_last_space_glyph == -1)
				{
					// Pull the previous glyph down onto the
					// swfnew line.
					if (last_line.m_glyphs.size() > 0)
					{
						rec.m_glyphs.push_back(last_line.m_glyphs.back());
						x += last_line.m_glyphs.back().m_glyph_advance;
						previous_x -= last_line.m_glyphs.back().m_glyph_advance;
						last_line.m_glyphs.resize(last_line.m_glyphs.size() - 1);
					}
				}
				else
				{
					// Move the previous word down onto the next line.
					previous_x -= m_text_glyph_records[m_last_space_record].m_glyphs[m_last_space_glyph].m_glyph_advance;
					int start = m_last_space_record == m_text_glyph_records.size() - 1 ? m_last_space_glyph + 1 : 0;

					for (int i = start; i < last_line.m_glyphs.size(); i++)
					{
						rec.m_glyphs.push_back(last_line.m_glyphs[i]);
						x += last_line.m_glyphs[i].m_glyph_advance;
						previous_x -= last_line.m_glyphs[i].m_glyph_advance;
					}
					
					int split = m_last_space_record == m_text_glyph_records.size() - 1 ? m_last_space_glyph : 0;
					last_line.m_glyphs.resize(split);
				}

				align_line(m_alignment, m_last_line_start_record, previous_x);

				m_last_space_glyph = -1;
				m_last_space_record = m_text_glyph_records.size();
				m_last_line_start_record = m_text_glyph_records.size();
			}
			if (m_cursor > character_idx) 
			{ 
				m_xcursor = x; 
				m_ycursor = y; 
			} 
			character_idx++; 

			// Expand the bounding-box to the lower-right corner of each glyph as
			// we generate it.
			m_text_bounding_box.expand_to_point(x, y + attributes.m_font->get_descent() * scale);

			last_code = (int) code;
			// TODO: HTML markup
		}
		m_xcursor += attributes.m_font->get_leading() * scale; 
		m_ycursor -= attributes.m_size + 
			(attributes.m_font->get_leading() - attributes.m_font->get_descent()) * scale; 

		// Add this line to our output.
		m_text_glyph_records.push_back(rec);

		m_xglobal += x - x0;
		m_yglobal += y - y0;
	}

	extern video_texture* (*s_texture_loader_callback)(const char* resource_name, int width, int height);

	void edit_text_character::append_image(const gameswf::tu_string &src, int width, int height)
	{
		bitmap_info* bi = NULL;

		character_def* def = find_exported_resource(src);
		if(def != NULL && def->is(AS_BITMAP))
		{
			bitmap_character* bitmap = (bitmap_character*)def;
			bi = bitmap->get_bitmap_info();
		}
		else
		{
			if (s_texture_loader_callback == NULL)
				return;

			video_texture* texture = s_texture_loader_callback(src.c_str(), width, height);
			if(!texture)
				return;

			bi = get_render_handler()->create_bitmap_info_texture(texture);
		}

		if(width <= 0)
			width = bi->get_width();
		if(height <= 0)
			height = bi->get_height();

		glyph g;
		g.m_type = glyph::BITMAP_IMAGE;
		g.m_fontlib_glyph = bi;
		g.m_glyph_advance = PIXELS_TO_TWIPS(width);
		g.m_fontsize = 1024;
		g.m_code = -1;
		g.m_bounds.m_x_min = 0;
		g.m_bounds.m_y_min = 0;
		g.m_bounds.m_x_max = PIXELS_TO_TWIPS(width);
		g.m_bounds.m_y_max = PIXELS_TO_TWIPS(height);

		float baseline = m_yglobal + PIXELS_TO_TWIPS(height);
		text_glyph_record	rec;	// one to work on
		// continue from last record?
		if(m_text_glyph_records.size() > 0)
		{
			rec.m_style = m_text_glyph_records.back().m_style;

			// adjust baseline if lower than previous glyphs on the same line
			if(baseline > rec.m_style.m_y_offset)
			{
				for(int i = m_text_glyph_records.size() - 1; i >= 0 && m_text_glyph_records[i].m_style.m_y_offset == rec.m_style.m_y_offset; i--)
				{
					m_text_glyph_records[i].m_style.m_y_offset = baseline;
				}
				rec.m_style.m_y_offset = baseline;
			}
		}
		else
		{
			rec.m_style.m_y_offset = baseline;
		}
		
		// m_y_offset is the baseline
		rec.m_style.m_x_offset = fmax(0, m_left_margin + m_indent) + m_xglobal;
		rec.m_style.m_font = NULL;
		rec.m_style.m_color.set(0xff, 0xff, 0xff, 0xff);
		rec.m_style.m_underline = false;
		rec.m_style.m_text_height = 1024;
		rec.m_style.m_has_x_offset = true;
		rec.m_style.m_has_y_offset = true;
		rec.m_style.m_has_font = false;

		m_xglobal += PIXELS_TO_TWIPS(width);

		rec.m_glyphs.push_back(g);
		m_text_glyph_records.push_back(rec);
	}

	character*	edit_text_character_def::create_character_instance(character* parent, int id)
	{
		if (m_font == NULL)
		{
			// Resolve the font, if possible.
			if (m_root_def)
			{
				m_font = m_root_def->get_font(m_font_id);
				if (m_font == NULL)
				{
					log_error("error: text style with undefined font; font_id = %d\n", m_font_id);
				}
			}
		}

		edit_text_character*	ch = get_player()->create_edit_text_character(this, parent, id);
		// instanciate_registered_class(ch);	//TODO: test it

		return ch;
	}


	void	define_edit_text_loader(stream* in, int tag_type, movie_definition_sub* m)
	// Read a DefineText tag.
	{
		assert(tag_type == 37);

		Uint16	character_id = in->read_u16();

		edit_text_character_def*	ch = swfnew edit_text_character_def(m->get_player(), m);
		IF_VERBOSE_PARSE(log_msg("edit_text_char, id = %d\n", character_id));
		ch->read(in, tag_type, m);

		m->add_character(character_id, ch);
	}

}	// end namespace gameswf


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
