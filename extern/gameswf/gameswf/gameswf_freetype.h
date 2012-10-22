// gameswf_freetype.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Truetype font rasterizer based on freetype library

#ifndef GAMESWF_FREETYPE_H
#define GAMESWF_FREETYPE_H

#include "base/tu_config.h"

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_shape.h"
#include "gameswf/gameswf_canvas.h"

#if TU_CONFIG_LINK_TO_FREETYPE == 1

#include <ft2build.h>
#include FT_OUTLINE_H
#include FT_FREETYPE_H
#include FT_GLYPH_H
#define FT_CONST

namespace gameswf
{
	struct face_entity : public ref_counted
	{
		FT_Face m_face;
		hash<int, glyph_entity *> m_ge;	// <code, glyph_entity>

		face_entity ( FT_Face face ) :
			m_face ( face )
		{
			assert ( face );
		}

		~face_entity()
		{
			FT_Done_Face ( m_face );

			for ( hash<int, glyph_entity *>::iterator it = m_ge.begin(); it != m_ge.end(); ++it )
			{
				delete it->second;
			}
		}

	};
    
    struct glyph_region
    {
		gc_ptr<bitmap_info>		m_fontTexture;
		array<gameswf::rect>	m_rects;

		~glyph_region()
		{
			m_fontTexture = NULL;
		}
    };    	

	struct glyph_freetype_provider : public glyph_provider
	{
			glyph_freetype_provider();
			~glyph_freetype_provider();

			virtual bitmap_info *get_char_image ( character_def *shape_glyph, Uint16 code,
			                                      const tu_string &fontname, bool is_bold, bool is_italic, int fontsize,
			                                      rect *bounds, float *advance, float *uvX, float *uvY);
                    
		private:

			face_entity *get_face_entity ( const tu_string &fontname,
			                               bool is_bold, bool is_italic );

            int put_glyph_to_texture( const FT_GlyphSlot &glyph, rect *bounds, float *uvx, float *uvy );
            void calc_cell_size( int *w, int *h );
            
			void insert_sort_rect( int id, rect r );
			gameswf::rect create_rect(int id, int w, int h);
            gameswf::rect devide_rect(int id, int rectID, int w, int h);			                    
			void add_empty_region();

			// callbacks
			static int move_to_callback ( FT_CONST FT_Vector *vec, void *ptr );
			static int line_to_callback ( FT_CONST FT_Vector *vec, void *ptr );
			static int conic_to_callback ( FT_CONST FT_Vector *ctrl, FT_CONST FT_Vector *vec,
			                               void *ptr );
			static int cubic_to_callback ( FT_CONST FT_Vector *ctrl1, FT_CONST FT_Vector *ctrl2,
			                               FT_CONST FT_Vector *vec, void *ptr );

			image::alpha *draw_bitmap ( const FT_Bitmap &bitmap );
			shape_character_def *get_char_def ( Uint16 code,
			                                    const char *fontname, bool is_bold, bool is_italic, int fontsize,
			                                    rect *bounds, float *advance );
            int     m_texSize;
			int		m_maxTex;
			float   m_scale;
                            
			gc_ptr<canvas> m_canvas;
			string_hash<gc_ptr<face_entity> >	m_face_entity;        
            
			
			array<glyph_region>					m_regions;
	};

}

#endif		// TU_CONFIG_LINK_TO_FREETYPE

#endif	// GAMESWF_FREETYPE_H
