#include "stdafx.h"
// gameswf_freetype.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// TrueType font rasterizer based on freetype library
// Also serve as fontlib

#include "base/tu_config.h"

#include "gameswf/gameswf_render.h"
#include "gameswf_freetype.h"
#include "gameswf_log.h"
#include "gameswf_canvas.h"
#include "gameswf_filters.h"
#include "base/utility.h"
#include "base/container.h"
#include "base/tu_file.h"

#include <freetype/ftbitmap.h>

#ifdef WIN32
	#include <Windows.h>
#else
#endif

namespace gameswf
{
#if TU_CONFIG_LINK_TO_FREETYPE == 1

	bool get_fontfile(const char* font_name, tu_string& file_name, bool is_bold, bool is_italic)
	// gets font file name by font name
	{

		if (font_name == NULL)
		{
			return false;
		}

		// Ask file name from game engine
		char path[256] = {0};
		if(::get_fontfile(font_name, is_bold, is_italic, path, 256))
		{
			file_name = path;
			return true;
		}

#ifdef WIN32

		//Vitaly: I'm not sure that this code works on all versions of Windows

		tu_stringi fontname = font_name;
		if (is_bold)
		{
			fontname += " Bold";
		}
		if (is_italic)
		{
			fontname +=  " Italic";
		}
		fontname += " (TrueType)";

		HKEY hKey;

		// try WinNT
		DWORD retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 
			0, 
			KEY_ALL_ACCESS,
			&hKey);

		if (retCode != ERROR_SUCCESS)
		{
			// try Windows
			retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				"Software\\Microsoft\\Windows\\CurrentVersion\\Fonts", 
				0, 
				KEY_ALL_ACCESS,
				&hKey);

			if (retCode != ERROR_SUCCESS)
			{
				return false;
			}
		}

		// Get the value of the 'windir' environment variable.
		tu_string windir(getenv("WINDIR"));

		// Get value count. 
		DWORD    cValues;              // number of values for key 
		retCode = RegQueryInfoKey(
			hKey,	// key handle 
			NULL,	// buffer for class name 
			NULL,	// size of class string 
			NULL,	// reserved 
			NULL,	// number of subkeys 
			NULL,	// longest subkey size 
			NULL,	// longest class string 
			&cValues,	// number of values for this key 
			NULL,	// longest value name 
			NULL,	// longest value data 
			NULL,	// security descriptor 
			NULL);	// last write time 

		// Enumerate the key values. 
		BYTE szValueData[MAX_PATH];
		TCHAR  achValue[MAX_PATH]; 
		for (DWORD i = 0, retCode = ERROR_SUCCESS; i < cValues; i++) 
		{ 
			DWORD cchValue = MAX_PATH; 
			DWORD dwValueDataSize = sizeof(szValueData) - 1;
			achValue[0] = '\0'; 
			retCode = RegEnumValueA(hKey, i, 
				achValue, 
				&cchValue, 
				NULL, 
				NULL,
				szValueData,
				&dwValueDataSize);

			if (retCode == ERROR_SUCCESS) 
			{ 
				if (fontname == (char*) achValue)
				{
					file_name = windir + tu_string("\\Fonts\\") + (char*) szValueData;
					RegCloseKey(hKey);
					return true;
				}
			} 
		}

		RegCloseKey(hKey);
		return false;
 
		// font file is not found, take default value - 'Times New Roman'
/*		file_name = windir + tu_string("\\Fonts\\Times");
		if (is_bold && is_italic)
		{
			file_name += "BI";
		}
		else
		if (is_bold)
		{
			file_name +=  "B";
		}
		else
		if (is_italic)
		{
			file_name +=  "I";
		}
		file_name += ".ttf";
		log_error("can't find font file for '%s'\nit's used '%s' file\n", fontname.c_str(), file_name.c_str());

		return true;*/

#else

	//TODO for Linux

	// hack
	if (strstr(font_name, "Times New Roman"))
	{
		file_name = "/usr/share/fonts/truetype/times";
		if (is_bold && is_italic)
		{
			file_name += "bi";
		}
		else
		if (is_bold)
		{
			file_name +=  "b";
		}
		else
		if (is_italic)
		{
			file_name +=  "b";
		}
		file_name += ".ttf";

		return true;
	}

	return false;


#endif
	}


	glyph_provider::glyph_provider(int cache_width, int cache_height, bool loadFontsInMemory, float scale) :
		m_lib(NULL),
		m_scale(scale),
		m_file_cache_enabled(loadFontsInMemory),
		m_glyph_texture_cache(NULL)
	{
		int	error = FT_Init_FreeType(&m_lib);
		if (error)
		{
			fprintf(stderr, "can't init FreeType!  error = %d\n", error);
			exit(1);
		}

		if(cache_width > 0 && cache_height > 0)
		{
#ifdef USE_ALPHA_TEXTURES
			m_glyph_texture_cache = swfnew glyph_texture_cache(this, cache_width, cache_height, 1);
#else
			m_glyph_texture_cache = swfnew glyph_texture_cache(this, cache_width, cache_height, 4);
#endif
		}
	}

	glyph_provider::~glyph_provider()
	{
		clear();

		if(m_glyph_texture_cache != NULL)
		{
			swfdelete (m_glyph_texture_cache);
		}

		int error = FT_Done_FreeType(m_lib);
		if (error)
		{
			fprintf(stderr, "can't close FreeType!  error = %d\n", error);
		}
	}

	//
	// Get image of character as bitmap
	//

	bitmap_info* glyph_provider::get_char_image(Uint16 code, 
		const tu_string& fontname, bool is_bold, bool is_italic, int fontsize,
		rect* bounds, float* advance)
	{
		face_entity* fe = get_face_entity(fontname, is_bold, is_italic);
		if (fe == NULL)
		{
			return NULL;
		}

		// form hash key
		int key = (fontsize << 16) | code;

		// try to find the stored image of character
		glyph_entity* ge = NULL;
		if (fe->m_ge.get(key, &ge) == false)
		{
			FT_Set_Pixel_Sizes(fe->m_face, 0, (int)(fontsize * m_scale));

			if(m_glyph_texture_cache == NULL)
			{
				if (FT_Load_Char(fe->m_face, code, FT_LOAD_RENDER))
				{
					return NULL;
				}

				ge = swfnew glyph_entity();

				image_alpha* im = draw_bitmap(fe->m_face->glyph->bitmap);
				ge->m_bi = render::create_bitmap_info_alpha(im->m_width, im->m_height, im->m_data);
				swfdelete (im);

				ge->m_bounds.m_x_max = float(fe->m_face->glyph->bitmap.width) /
					float(ge->m_bi->get_width());
				ge->m_bounds.m_y_max = float(fe->m_face->glyph->bitmap.rows) /
					float(ge->m_bi->get_height());
			}
			else
			{
				if (FT_Load_Char(fe->m_face, code, FT_LOAD_DEFAULT))
				{
					return NULL;
				}

				ge = swfnew glyph_entity();

				// You must use power-of-two dimensions!!
				int	w = fe->m_face->glyph->metrics.width/64 + 1;
				int	h = fe->m_face->glyph->metrics.height/64 + 1;
				glyph_texture_cache::get_region_size_requirement(w, h);

				ge->m_bounds.m_x_max = float(fe->m_face->glyph->metrics.width/64) / float(w);
				ge->m_bounds.m_y_max = float(fe->m_face->glyph->metrics.height/64) / float(h);
			}

			ge->m_bounds.m_x_min = fe->m_face->glyph->metrics.width > 0 ? float(fe->m_face->glyph->metrics.horiBearingX) / float(fe->m_face->glyph->metrics.width) : 0.0f;
			ge->m_bounds.m_y_min = fe->m_face->glyph->metrics.height > 0 ? float(fe->m_face->glyph->metrics.horiBearingY) / float(fe->m_face->glyph->metrics.height) : 0.0f;
			ge->m_bounds.m_x_min *= - ge->m_bounds.m_x_max;
			ge->m_bounds.m_y_min *= ge->m_bounds.m_y_max;

			float scale = 16.0f / fontsize;	// hack
			ge->m_advance = (float) fe->m_face->glyph->metrics.horiAdvance * scale;

			// keep image of character
			fe->m_ge.add(key, ge);
		}

		*bounds = ge->m_bounds;
		*advance = ge->m_advance;

		if(m_glyph_texture_cache == NULL)
			return ge->m_bi.get_ptr();
		else
			return m_glyph_texture_cache->get_bitmap_info();
	}

	face_entity* glyph_provider::get_face_entity(const tu_string& fontname, 
		bool is_bold, bool is_italic)
	{

		// form hash key
		tu_string key = fontname;
		if (is_bold)
		{
			key += "B";
		}
		if (is_italic)
		{
			key += "I";
		}

		// first try to find from hash
		smart_ptr<face_entity> fe;
		if (m_face_entity.get(key, &fe))
		{
			return fe.get_ptr();
		}

		tu_string font_filename;
		if (get_fontfile(fontname, font_filename, is_bold, is_italic) == false)
		{
			log_error("can't find font file '%s'\n", fontname.c_str());
			m_face_entity.add(key, NULL);
			return NULL;
		}

		// Scan in loaded fonts for a filename match to avoid loading same font multiple times
		for(string_hash< smart_ptr<face_entity> >::iterator it = m_face_entity.begin(); it != m_face_entity.end(); ++it)
		{
			face_entity* fe = it->second.get_ptr();
			if(fe->m_filename == font_filename)
			{
				// keep mapping in cache
				m_face_entity.add(key, fe);
				return fe;
			}
		}

		FT_Face face = NULL;

		bool success = false;
		if(m_file_cache_enabled)
		{
			// load font from memory
			tu_file file(font_filename, "rb");
			if(file.is_open())
			{
				// buffer font data
				file.go_to_end();
				int filesize = file.get_position();
				file.set_position(0);
				
				membuf* fontdata = swfnew membuf();
				fontdata->resize(filesize);
				file.read_fully(fontdata);
				FT_New_Memory_Face(m_lib, (FT_Byte*)fontdata->data(), filesize, 0, &face);
				if (face)
				{
					fe = swfnew face_entity(face, fontdata, font_filename);
					m_face_entity.add(key, fe);
					success = true;
				}
				else
				{
					swfdelete (fontdata);
				}
			}
		}
		else
		{
			// load font from file
			FT_New_Face(m_lib, font_filename.c_str(), 0, &face);
			fe = swfnew face_entity(face, font_filename);
			m_face_entity.add(key, fe);
			success = true;
		}
		
		if(success == false)
		{
			log_error("some error opening font '%s'\n", font_filename.c_str());
		}
		return fe.get_ptr();
	}

	image_alpha* glyph_provider::draw_bitmap(const FT_Bitmap& bitmap)
	{
		// You must use power-of-two dimensions!!
		int	w = 1; while (w < bitmap.pitch || w < 4) { w <<= 1; }
		int	h = 1; while (h < bitmap.rows) { h <<= 1; }

		image_alpha* alpha = create_alpha(w, h);
		memset(alpha->m_data, 0, alpha->m_width * alpha->m_height);

		// copy image to alpha
		for (int i = 0; i < bitmap.rows; i++)
		{
			uint8*	src = bitmap.buffer + bitmap.pitch * i;
			uint8*	dst = alpha->m_data + alpha->m_pitch * i;
			int	x = bitmap.width;
			while (x-- > 0)
			{
				*dst++ = *src++;
			}
		}

		return alpha;
	}

	//
	// freetype callbacks, called from freetype lib  through get_char_def()
	//

	// static
	int glyph_provider::move_to_callback(FT_CONST FT_Vector* to, void* user)
	{
		glyph_provider* _this = static_cast<glyph_provider*>(user);
		_this->m_canvas->move_to(to->x * _this->m_scale, - to->y * _this->m_scale);
		return 0;
	}
	
	// static 
	int glyph_provider::line_to_callback(FT_CONST FT_Vector* to, void* user)
	{
		glyph_provider* _this = static_cast<glyph_provider*>(user);
		_this->m_canvas->line_to(to->x * _this->m_scale, - to->y * _this->m_scale);
		return 0;
	}

	//static
	int glyph_provider::conic_to_callback(FT_CONST FT_Vector* ctrl, FT_CONST FT_Vector* to,
		void* user)
	{
		glyph_provider* _this = static_cast<glyph_provider*>(user);
		_this->m_canvas->curve_to(ctrl->x * _this->m_scale, - ctrl->y * _this->m_scale, 
				to->x * _this->m_scale, - to->y * _this->m_scale);
		return 0;
	}

	//static
	int glyph_provider::cubic_to_callback(FT_CONST FT_Vector* ctrl1, FT_CONST FT_Vector* ctrl2,
			FT_CONST FT_Vector* to, void* user)
	{
		glyph_provider* _this = static_cast<glyph_provider*>(user);
		float x = float(ctrl1->x + ((ctrl2->x - ctrl1->x) * 0.5));
		float y = float(ctrl1->y + ((ctrl2->y - ctrl1->y) * 0.5));
		_this->m_canvas->curve_to(x * _this->m_scale, - y * _this->m_scale, 
				to->x * _this->m_scale, - to->y * _this->m_scale);
		return 0;
	}

	//
	// Get image of character as shape
	//

	shape_character_def* glyph_provider::get_char_def(Uint16 code, 
		const char* fontname, bool is_bold, bool is_italic, int fontsize,
		rect* box, float* advance)
	{
		assert(0);	// FIXME
/*
		FT_Face face = 0; //get_face(fontname, is_bold, is_italic);
		if (face == NULL)
		{
			return NULL;
		}

		if (FT_Load_Char(face, code, FT_LOAD_NO_BITMAP | FT_LOAD_NO_SCALE))
		{
			return NULL;
		}

		if (face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
		{
			return NULL;
		}


		m_canvas = swfnew canvas();
		m_canvas->begin_fill(rgba(255, 255, 255, 255));

		FT_Outline_Funcs callback_func;
 		callback_func.move_to = move_to_callback;
		callback_func.line_to = line_to_callback;
		callback_func.conic_to = conic_to_callback;
		callback_func.cubic_to = cubic_to_callback;

		m_scale = 1024.0f / face->units_per_EM;
		*advance = (float) face->glyph->metrics.horiAdvance * m_scale;

		FT_Outline_Decompose(&face->glyph->outline, &callback_func, this);

		m_canvas->end_fill();

		return m_canvas.get_ptr();
		*/
		return 0;
	}

	// 
	//	glyph texture cache
	//


	extern unsigned char sqrt_table[256];

	bool glyph_texture_cache::add_glyph_region(Uint16 code, void* _fe, int fontsize, filter_info& fi, bool reuse)
	{
		face_entity* fe = (face_entity*)_fe;
		const float font_scale = m_glyph_provider->get_scale();
		unsigned char stroke = (unsigned char)(fi.stroke * font_scale);
		unsigned char blurx = (unsigned char)(fi.blurx * font_scale);
		unsigned char blury = (unsigned char)(fi.blury * font_scale);
		unsigned char* pixels = lock();

		FT_Set_Pixel_Sizes(fe->m_face, 0, (int)(fontsize * font_scale));
		if (FT_Load_Char(fe->m_face, code, FT_LOAD_RENDER) == 0)
		{
			FT_Bitmap temp_bmp;
			bool mono = fe->m_face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO;
			if(mono)
			{
				FT_Bitmap_New(&temp_bmp);
				FT_Bitmap_Convert(m_glyph_provider->get_library(), &fe->m_face->glyph->bitmap, &temp_bmp, 1);
				int count = temp_bmp.width * temp_bmp.rows;
				for(int i = 0; i < count; i++)
					temp_bmp.buffer[i] = (unsigned char)(temp_bmp.buffer[i] * 255);
			}
			const FT_Bitmap& bitmap = mono ? temp_bmp : fe->m_face->glyph->bitmap;

			int	w = bitmap.pitch + 1;
			int	h = bitmap.rows + 1;
			if(stroke)
			{
				w += (2 * stroke);
				h += (2 * stroke);
			}
			else if(blurx || blury)
			{
				w += (2 * blurx);
				h += (2 * blury);
			}
			get_region_size_requirement(w, h);

			region* r = NULL;
			if(reuse)
				r = find_used_region(w, h);
			else
				r = find_available_region(w, h);
			if(r == NULL)
				return false;
			
			// update infos
			r->timestamp = m_timestamp++;
			
			key k = make_key(code, fe, fontsize, fi);
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

			uint8* src_pixels = bitmap.buffer;
			int src_pitch = bitmap.pitch;
			int src_width = bitmap.width;
			int src_height = bitmap.rows;
			if(stroke > 0)
			{
				// enlarge the glyph with stroke
				m_working_buffer.resize(w * h);
				memset(&m_working_buffer[0], 0, m_working_buffer.size());
				float maxstrength = 2.0f * stroke;

				for(int sy = 1 + 2 * stroke; sy >= 0; sy--)
				{
					for(int sx = 1 + 2 * stroke; sx >= 0; sx--)
					{
						uint8*	src = bitmap.buffer;
						uint8*	dst = &m_working_buffer[0] + w * sy + sx;
						float strength = (maxstrength - iabs(sx-stroke) - iabs(sy-stroke)) / maxstrength;
						strength = sqrt_table[(int)fmin(fmax(strength * 255, 0), 255)] / 255.0f;
						strength = sqrt_table[(int)fmin(fmax(strength * 255, 0), 255)] / 255.0f;
						
						for (int i = 0; i < bitmap.rows; i++)
						{
							uint8*	in = src;
							uint8*	out = dst;

							for (int j = 0; j < bitmap.width; j++)
							{
								*out = (uint8)imax(*out, imin((int)(*in * strength), 255));
								in++;
								out++;
							}

							src += bitmap.pitch;
							dst += w;
						}
					}
				}

				src_pixels = &m_working_buffer[0];
				src_pitch = w;
				src_width = w - 1;
				src_height = h - 1;
			}
			else if(blurx || blury)
			{
				// blur the glyph
				int halfsize = (w + blurx * 2) * (h + blury * 2);
				m_working_buffer.resize(2 * halfsize);
				memset(&m_working_buffer[0], 0, m_working_buffer.size());

				// copy to buffer with same size as destination
				filter_engine::raster src;
				src.bpp = 1;
				src.pixels = bitmap.buffer;
				src.x = src.y = 0;
				src.pitch = bitmap.pitch;
				src.width = bitmap.width;
				src.height = bitmap.rows;

				filter_engine::raster dst;
				dst.bpp = 1;
				dst.pixels = (unsigned char*)&m_working_buffer[0];
				dst.x = blurx;
				dst.y = blury;
				dst.pitch = w;
				dst.width = w - 1;
				dst.height = h - 1;
				filter_engine::copy(src, dst);

				// blur filter
				filter f;
				f.m_id = filter::BLUR;
				f.blur.blur_x = blurx;
				f.blur.blur_y = blury;
				f.blur.passes = 1;
				
				filter_engine::params p;
				p.f = &f;

				p.src.bpp = 1;
				p.src.pixels = (unsigned char*)&m_working_buffer[0];
				p.src.x = 0;
				p.src.y = 0;
				p.src.pitch = w;
				p.src.width = w - 1;
				p.src.height = h - 1;

				p.dst.bpp = 1;
				p.dst.pixels = (unsigned char*)&m_working_buffer[halfsize];
				p.dst.x = 0;
				p.dst.y = 0;
				p.dst.pitch = w;
				p.dst.width = w - 1;
				p.dst.height = h - 1;

				filter_engine::apply(p);

				src_pixels = p.dst.pixels;
				src_pitch = w;
				src_width = w - 1;
				src_height = h - 1;
			}

			// copy bitmap glyph to texture cache
			for (int i = 0; i < src_height; i++)
			{
				uint8*	src = src_pixels + src_pitch * i;
				uint8*	dst = region_pixels + dst_pitch * i;
				if(BPP == 1)
				{
					memcpy(dst, src, src_width * BPP);
				}
				else
				{
					int	x = src_width;
					while (x-- > 0)
					{
#if _TU_LITTLE_ENDIAN_ == 0
							*(dst++) = 0xff;
							*(dst++) = 0xff;
							*(dst++) = 0xff;
							*(dst++) = *(src++);
#else
							*(dst++) = 0xff;
							*(dst++) = 0xff;
							*(dst++) = 0xff;
							*(dst++) = *(src++);
#endif
					}
				}
			}

			if(mono)
			{
				FT_Bitmap_Done(m_glyph_provider->get_library(), &temp_bmp);
			}

			return true;
		}
		return false;
	}

	void glyph_texture_cache::get_glyph_region(Uint16 code, void* fe, int fontsize, filter_info& filter, rect& bounds)
	{
		key k = make_key(code, fe, fontsize, filter);
		region* r = NULL;
		if(!m_used_regions.get(k, &r))
		{
			// this can happen if add_glyph_regions with string of character failed previously
			// (ie not enough place inside cache to contain all characters)
			if(!add_glyph_region(code, fe, fontsize, filter))
			{
				// flush
				get_render_handler()->flush();

				// try to reuse a region
				if(!add_glyph_region(code, fe, fontsize, filter, true))
				{
					// if no region could be reused, reset the cache
					reset();

					// add in empty cache
					add_glyph_region(code, fe, fontsize, filter);
				}
			}

			bool found = m_used_regions.get(k, &r);
			assert(found);
		}
		if(r)
		{
			get_region_bounds(r, bounds);
		}
	}

#else	// TU_CONFIG_LINK_TO_FREETYPE == 1

	void create_glyph_provider() {}
	void close_glyph_provider() {}

#endif

}	// end of namespace gameswf
