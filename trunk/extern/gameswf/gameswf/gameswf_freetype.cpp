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
#include "base/utility.h"
#include "base/container.h"

#ifdef _WIN32
#include <Windows.h>
#else
#endif

namespace gameswf
{

#if TU_CONFIG_LINK_TO_FREETYPE == 1

	static fsgetfont_callback	s_getfont_handler = NULL;

	void	register_getfont_callback ( fsgetfont_callback handler )
	{
		s_getfont_handler = handler;
	}
	

	static fsgetfontsize_callback	s_getfontsize_handler = NULL;

	void	register_getfontsize_callback ( fsgetfontsize_callback handler )
	{
		s_getfontsize_handler = handler;
	}


	static FT_Library	m_lib;

	bool get_fontfile ( const char *font_name, tu_string &file_name, bool is_bold, bool is_italic )
	// gets font file name by font name
	{

		if ( gameswf::s_getfont_handler != NULL )
		{		
			return gameswf::s_getfont_handler( font_name, file_name, is_bold, is_italic );
		}
		
		if ( font_name == NULL )
		{
			return false;
		}

#ifdef _WIN32
		//Vitaly: I'm not sure that this code works on all versions of Windows
		tu_stringi fontname = font_name;

		if ( is_bold )
		{
			fontname += " Bold";
		}

		if ( is_italic )
		{
			fontname +=  " Italic";
		}

		fontname += " (TrueType)";
		HKEY hKey;
		// try WinNT
		DWORD retCode = RegOpenKeyEx ( HKEY_LOCAL_MACHINE,
		                               "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts",
		                               0,
		                               KEY_ALL_ACCESS,
		                               &hKey );

		if ( retCode != ERROR_SUCCESS )
		{
			// try Windows
			retCode = RegOpenKeyEx ( HKEY_LOCAL_MACHINE,
			                         "Software\\Microsoft\\Windows\\CurrentVersion\\Fonts",
			                         0,
			                         KEY_ALL_ACCESS,
			                         &hKey );

			if ( retCode != ERROR_SUCCESS )
			{
				return false;
			}
		}

		// Get the value of the 'windir' environment variable.
		tu_string windir ( getenv ( "WINDIR" ) );
		// Get value count.
		DWORD    cValues;              // number of values for key
		retCode = RegQueryInfoKey (
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
		              NULL );	// last write time
		// Enumerate the key values.
		BYTE szValueData[MAX_PATH];
		TCHAR  achValue[MAX_PATH];

		for ( DWORD i = 0, retCode = ERROR_SUCCESS; i < cValues; i++ )
		{
			DWORD cchValue = MAX_PATH;
			DWORD dwValueDataSize = sizeof ( szValueData ) - 1;
			achValue[0] = '\0';
			retCode = RegEnumValueA ( hKey, i,
			                          achValue,
			                          &cchValue,
			                          NULL,
			                          NULL,
			                          szValueData,
			                          &dwValueDataSize );

			if ( retCode == ERROR_SUCCESS )
			{
				if ( ( fontname == ( char * ) achValue ) || ( ( strstr ( achValue, font_name ) != NULL ) && !is_italic && !is_bold ) )
				{
					file_name = windir + tu_string ( "\\Fonts\\" ) + ( char * ) szValueData;
					RegCloseKey ( hKey );
					return true;
				}
			}
		}

		RegCloseKey ( hKey );
		return false;
#else

		//TODO for Linux

		// hack
		if ( strstr ( font_name, "Times New Roman" ) )
		{
			file_name = "/usr/share/fonts/truetype/times";
		}

		else if ( strstr ( font_name, "Arial" ) )
		{
			file_name = "/usr/share/fonts/truetype/arial";
		}

		else
		{
			return false;
		}

		if ( is_bold && is_italic )
		{
			file_name += "bi";
		}

		else if ( is_bold )
		{
			file_name +=  "b";
		}

		else if ( is_italic )
		{
			file_name +=  "b";
		}

		file_name += ".ttf";
		return true;
#endif

	}


	//
	//	glyph provider implementation
	//

	glyph_freetype_provider::glyph_freetype_provider() :
		m_scale ( 0 )
	{        
        m_texSize	= 512;
		m_maxTex	= 10;
		add_empty_region();
	}
	
	glyph_freetype_provider::~glyph_freetype_provider()
	{        
		m_regions.clear();

		m_face_entity.clear();
		int error = FT_Done_FreeType ( m_lib );

		if ( error )
		{
			fprintf ( stderr, "FreeType provider: can't close FreeType!  error = %d\n", error );
		}

		m_lib = NULL;
	}

	void glyph_freetype_provider::add_empty_region()
	{
		int size = m_texSize*m_texSize;
        
        unsigned char *data = new unsigned char[size];
        memset(data, 0, size);		
        bitmap_info *tex = render::create_bitmap_info_alpha(m_texSize, m_texSize, data);
        delete data;
        
        rect r;
        r.m_x_min = 0;
        r.m_y_min = 0;
        r.m_x_max = (float)m_texSize;
        r.m_y_max = (float)m_texSize;
        
        glyph_region region;
		region.m_fontTexture = tex;
        region.m_rects.push_back(r);
		
		m_regions.push_back( region );
	}

	//
	// Get image of character as bitmap
	//
	
	bitmap_info *glyph_freetype_provider::get_char_image ( character_def *shape_glyph, Uint16 code,
	        const tu_string &fontname, bool is_bold,
	        bool is_italic, int fontsize, rect *bounds, float *advance, float *uvX, float *uvY)
	{
		face_entity *fe = get_face_entity ( fontname, is_bold, is_italic );

		if ( fe == NULL )
		{
			return NULL;
		}

		if ( gameswf::s_getfontsize_handler != NULL )
		{
			int currentFontSize = fontsize;
			// modify font size
			if ( gameswf::s_getfontsize_handler( fontname.c_str(), currentFontSize ) )
				fontsize = currentFontSize;
		}

		// form hash key
		int key = ( fontsize << 16 ) | code;
		// try to find the stored image of character
		glyph_entity *ge = NULL;

		if ( fe->m_ge.get ( key, &ge ) == false )
		{
			FT_Set_Pixel_Sizes ( fe->m_face, fontsize, fontsize );

			if ( FT_Load_Char ( fe->m_face, code, FT_LOAD_RENDER ) )
			{
				return NULL;
			}

			ge = new glyph_entity();                                   
			int regionID = put_glyph_to_texture( fe->m_face->glyph, &ge->m_bounds, &ge->m_uvX, &ge->m_uvY);
			ge->m_bi = m_regions[regionID].m_fontTexture;
            
			float scale = 16.0f / fontsize;	// hack
			ge->m_advance = ( float ) fe->m_face->glyph->metrics.horiAdvance * scale;
			// keep image of character
			fe->m_ge.add ( key, ge );
		}

		if ( bounds )
		{
			*bounds = ge->m_bounds;
            *uvX = ge->m_uvX;
            *uvY = ge->m_uvY;
		}

		if ( advance )
		{
			*advance = ge->m_advance;
		}

		return ge->m_bi.get_ptr();
	}  
    
    image::alpha *glyph_freetype_provider::draw_bitmap ( const FT_Bitmap &bitmap )
	{
		// You must use power-of-two dimensions!!
		int	w = 1;
        
		while ( w <= bitmap.width )
		{
			w <<= 1;
		}
        
		int	h = 1;
        
		while ( h <= bitmap.rows )
		{
			h <<= 1;
		}
        
		image::alpha *alpha = image::create_alpha ( w, h );
		memset ( alpha->m_data, 0,  w * h );
        
		for ( int j = 0; j < bitmap.rows; ++j )
		{
			Uint8 *dst = alpha->m_data + j * w;
			const Uint8 *src = bitmap.buffer + j * bitmap.width;
            
			for ( int i = 0; i < bitmap.width; ++i )
			{
				//since w and h have a good chance of getting larger than the bitmaps height and/or width
				*dst++ = *src++;
			}
		}
        
		return alpha;
	}
    
    int glyph_freetype_provider::put_glyph_to_texture( const FT_GlyphSlot &glyph, rect *bounds, float *uvx, float *uvy )
    {
        int glyphW = glyph->bitmap.width;
        int glyphH = glyph->bitmap.rows;
        int cellW = glyphW;
        int cellH = glyphH;
                        
        
        // fix cell size
        calc_cell_size(&cellW, &cellH);
                
        // find a region for write character
		int regionid = -1;
		gameswf::rect region;

		for ( int i = 0; i < (int)m_regions.size(); i++ )
		{
			region = create_rect(i, cellW, cellH);

			if ( region.width() != 0 && region.height() != 0 )
			{
				regionid = i;
				break;
			}
		}
		
		if ( regionid == -1 && m_regions.size() < m_maxTex )
		{
			add_empty_region();
			regionid = (int)(m_regions.size() - 1);
			region = create_rect( regionid, cellW, cellH);
		}
		
		if ( regionid == -1 )
			return 0;

        // draw character at region
        int x = (int)region.m_x_min;
        int y = (int)region.m_y_min;

		x += (cellW - glyphW)/2;
		y += (cellH - glyphH)/2;

        *uvx = x/(float)m_texSize;
        *uvy = y/(float)m_texSize;
        
        Uint8 *dstImg = m_regions[regionid].m_fontTexture->get_data();
               
        // need for upload texture
		m_regions[regionid].m_fontTexture->m_dirty = true;
        
        // copy to image
        const Uint8 *srcImg = glyph->bitmap.buffer;

        for ( int j = 0; j < glyph->bitmap.rows; ++j )
		{
            Uint8 *dst = dstImg + (y+j)*m_texSize + x;            
			const Uint8 *src = srcImg + j*glyph->bitmap.width;
            
			for ( int i = 0; i < glyph->bitmap.width; ++i )
			{
				*dst++ = *src++;
			}
		}              
        
        // calc bound
        bounds->m_x_min = (float)glyph->metrics.horiBearingX/(float)glyph->metrics.width;
        bounds->m_y_min = (float)glyph->metrics.horiBearingY/(float)glyph->metrics.height;
        
        bounds->m_x_max = glyphW/(float)m_texSize;
        bounds->m_y_max = glyphH/(float)m_texSize;
        
        bounds->m_x_min *= -bounds->m_x_max;
        bounds->m_y_min *= bounds->m_y_max;            
        
        return regionid;
    }
    
    gameswf::rect glyph_freetype_provider::create_rect(int id, int w, int h)
    {
        gameswf::rect ret;
        ret.m_x_min = 0;
        ret.m_y_min = 0;
        ret.m_x_max = 0;
        ret.m_y_max = 0;        
        
        
        for ( int i = 0, n = m_regions[id].m_rects.size(); i < n; i++ )
        {
            if (	m_regions[id].m_rects[i].width() >= w && 
					m_regions[id].m_rects[i].height() >= h )
            {
                // found region & devide this region
                ret = devide_rect(id, i, w, h);                
                return ret;
            }
        }
        
        return ret;
    }
    
    gameswf::rect glyph_freetype_provider::devide_rect(int id, int rectID, int w, int h)
    {
        gameswf::rect r = m_regions[id].m_rects[rectID];
    
        // remove this region
        m_regions[id].m_rects.remove(rectID);
        
        // result
        rect ret = r;
        ret.m_x_max = r.m_x_min + (float)w;
        ret.m_y_max = r.m_y_min + (float)h;
        
        // divide region r
        // more info: pham hong duc
        //
        // *---+-----------*
        // | r |           |
        // | e |   right   |
        // | t |           |
        // +---+-----------*
        // |               |
        // |     bottom    |
        // |               |
        // *---------------*
        //
        gameswf::rect right, bottom;
        right.m_x_min = ret.m_x_max;
        right.m_x_max = right.m_x_min + r.width() - (float)w;
        right.m_y_min = ret.m_y_min;
        right.m_y_max = ret.m_y_max;
        
        bottom.m_x_min = r.m_x_min;
        bottom.m_x_max = r.m_x_max;
        bottom.m_y_min = ret.m_y_max;
        bottom.m_y_max = bottom.m_y_min + r.height() - (float)h;
                
        // add new region (sort by height)
        if ( right.width() > 0 && right.height() > 0 )
            insert_sort_rect(id, right);
        
        if ( bottom.width() > 0 && bottom.height() > 0 )
            insert_sort_rect(id, bottom);
        
        return ret;
    }
    
    void glyph_freetype_provider::insert_sort_rect( int id, gameswf::rect r )
    {
        bool add = false;
        float h = r.height();
        
        for ( int i = m_regions[id].m_rects.size() - 1; i >= 0 ; i-- )
        {
            if ( h > m_regions[id].m_rects[i].height() )
            {
                add = true;
				m_regions[id].m_rects.insert(i+1, r);
                break;
            }
        }
        
        if ( add == false )
            m_regions[id].m_rects.insert(0, r);
    }
    
    void glyph_freetype_provider::calc_cell_size( int *w, int *h )
    {
        const int cellSize = 4;
        
        int cellW = cellSize;
        while ( cellW < *w ) 
        {
            cellW += cellSize;
        }
        *w = cellW;
        
        int cellH = cellSize;
        while ( cellH < *h )
        {
            cellH += cellSize;
        }
        *h = cellH;
    }
    
	face_entity *glyph_freetype_provider::get_face_entity ( const tu_string &fontname, bool is_bold, bool is_italic )
	{
		// form hash key
		tu_string key = fontname;

		if ( is_bold )
		{
			key += "B";
		}

		if ( is_italic )
		{
			key += "I";
		}

		// first try to find from hash
		gc_ptr<face_entity> fe;

		if ( m_face_entity.get ( key, &fe ) )
		{
			return fe.get_ptr();
		}

		tu_string font_filename;

		if ( get_fontfile ( fontname, font_filename, is_bold, is_italic ) == false )
		{
			log_msg ( "can't find font file '%s', using Arial font\n", fontname.c_str() );

			if ( get_fontfile ( "Arial", font_filename, is_bold, is_italic ) == false )
			{
				log_error ( "can't find font file '%s', failed\n", fontname.c_str() );
				m_face_entity.add ( key, NULL );
				return NULL;
			}
		}

		FT_Face face = NULL;
		FT_New_Face ( m_lib, font_filename.c_str(), 0, &face );

		if ( face )
		{
			if ( is_bold )
			{
				face->style_flags |= FT_STYLE_FLAG_BOLD;
			}

			if ( is_italic )
			{
				face->style_flags |= FT_STYLE_FLAG_ITALIC;
			}

			fe = new face_entity ( face );
			m_face_entity.add ( key, fe );
		}

		else
		{
			log_error ( "some error opening font '%s'\n", font_filename.c_str() );
		}

		return fe.get_ptr();
	}
	

	//
	// freetype callbacks, called from freetype lib  through get_char_def()
	//

	// static
	int glyph_freetype_provider::move_to_callback ( FT_CONST FT_Vector *to, void *user )
	{
		glyph_freetype_provider *_this = static_cast<glyph_freetype_provider *> ( user );
		_this->m_canvas->move_to ( to->x * _this->m_scale, - to->y * _this->m_scale );
		return 0;
	}

	// static
	int glyph_freetype_provider::line_to_callback ( FT_CONST FT_Vector *to, void *user )
	{
		glyph_freetype_provider *_this = static_cast<glyph_freetype_provider *> ( user );
		_this->m_canvas->line_to ( to->x * _this->m_scale, - to->y * _this->m_scale );
		return 0;
	}

	//static
	int glyph_freetype_provider::conic_to_callback ( FT_CONST FT_Vector *ctrl, FT_CONST FT_Vector *to,
	                                                 void *user )
	{
		glyph_freetype_provider *_this = static_cast<glyph_freetype_provider *> ( user );
		_this->m_canvas->curve_to ( ctrl->x * _this->m_scale, - ctrl->y * _this->m_scale,
		                            to->x * _this->m_scale, - to->y * _this->m_scale );
		return 0;
	}

	//static
	int glyph_freetype_provider::cubic_to_callback ( FT_CONST FT_Vector *ctrl1, FT_CONST FT_Vector *ctrl2,
	                                                 FT_CONST FT_Vector *to, void *user )
	{
		glyph_freetype_provider *_this = static_cast<glyph_freetype_provider *> ( user );
		float x = float ( ctrl1->x + ( ( ctrl2->x - ctrl1->x ) * 0.5 ) );
		float y = float ( ctrl1->y + ( ( ctrl2->y - ctrl1->y ) * 0.5 ) );
		_this->m_canvas->curve_to ( x * _this->m_scale, - y * _this->m_scale,
		                            to->x * _this->m_scale, - to->y * _this->m_scale );
		return 0;
	}

	//
	// Get image of character as shape
	//

	shape_character_def *glyph_freetype_provider::get_char_def ( Uint16 code,
	        const char *fontname, bool is_bold, bool is_italic, int fontsize,
	        rect *box, float *advance )
	{
		assert ( 0 );	// FIXME
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


				m_canvas = new canvas();
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

	glyph_provider	*create_glyph_provider_freetype()
	{
		int	error = FT_Init_FreeType ( &m_lib );

		if ( error )
		{
			fprintf ( stderr, "FreeType provider: can't init FreeType!  error = %d\n", error );
			return NULL;
		}

		return new glyph_freetype_provider();
	}
#endif

}	// end of namespace gameswf
