// gameswf_render_handler_irrlicht.cpp	-- Willem Kokke <willem@mindparity.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// OpenGL ES based video handler for mobile units

#include "base/tu_config.h"

#ifdef TU_USE_IRRLICHT

#include "irrlicht.h"

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_types.h"
#include "base/image.h"
#include "base/utility.h"

#include <string.h>

const int GL_RGB = 0;
const int GL_RGBA = 1;
const int GL_ALPHA = 2;

const int GL_TRIANGLE_STRIP = 0;
const int GL_TRIANGLES = 0;

using namespace irr;
using namespace irr::video;

IVideoDriver		*g_driver	= NULL;
IrrlichtDevice		*g_device	= NULL;

void *get_texture_data( int format, int w, int h, void *data  )
{	
	void *imageData = data;
    
	switch( format )
	{
        case GL_RGBA:
		{            
			int totalSize = w*h;			
			unsigned char *s = (unsigned char*)imageData;
			unsigned char color;
            
			for ( int i = 0; i < totalSize; i++ )
			{
				// swap to bgr
				color = s[0];
				s[0] = s[2];
				s[2] = color;
				s+=4;
			}
		}
            break;
        case GL_ALPHA:
		{
			// we need convert 8bit to 32bit
			imageData = new unsigned char[ w*h*4 ];
			int totalSize = w*h;			
			unsigned char *d = (unsigned char*)imageData;
			unsigned char *s = (unsigned char*)data;
            
			for ( int i = 0; i < totalSize; i++ )
			{
				*d = 0xff; d++; //b
				*d = 0xff; d++; //g
				*d = 0xff; d++; //r
				*d = *s;   d++; //a
				s++;
			}
		}
            break;
        default:
		{
			int totalSize = w*h;			
			unsigned char *s = (unsigned char*)imageData;
			unsigned char color;
            
			for ( int i = 0; i < totalSize; i++ )
			{
				// swap to bgr
				color = s[0];
				s[0] = s[2];
				s[2] = color;
				s+=3;
			}
		}
        break;
	}
    
	return imageData;
}


ITexture *create_texture( int format, int w, int h, void *data, int level )
{	
	ECOLOR_FORMAT imageFormat = ECF_R8G8B8;
	int sizePixel = 4;

    void *imageData = get_texture_data(format, w, h, data);
    
	switch( format )
	{
	case GL_RGBA:
		{
			imageFormat = ECF_A8R8G8B8;
			sizePixel = 4;
		}
		break;
	case GL_ALPHA:
		{
			imageFormat = ECF_A8R8G8B8;
			sizePixel = 4;
		}
		break;
	default:
		{
			imageFormat = ECF_R8G8B8;
			sizePixel = 3;
		}
		break;
	}

	video::IImage *img = g_driver->createImage( imageFormat , irr::core::dimension2du(w,h) );
	memcpy(img->lock(), imageData, w*h*sizePixel);

    char name[512];
    sprintf(name, "gameswf_0x%x", (unsigned long)img);
	ITexture *tex = g_driver->addTexture( io::path(name), img );

	img->drop();
	
	if ( format == GL_ALPHA )
		delete imageData;

	return tex;
}


// choose the resampling method:
// 1 = hardware (experimental, should be fast, somewhat buggy)
// 2 = fast software bilinear (default)
// 3 = use image::resample(), slow software resampling
#define RESAMPLE_METHOD 2

// bitmap_info_irrlicht declaration
struct bitmap_info_irrlicht : public gameswf::bitmap_info
{
	ITexture	*m_texture;
	SColor		m_colorBlend;

	int m_width;
	int m_height;
	image::image_base *m_suspended_image;

	bitmap_info_irrlicht();
	bitmap_info_irrlicht ( int width, int height, Uint8 *data );
	bitmap_info_irrlicht ( image::rgb *im );
	bitmap_info_irrlicht ( image::rgba *im );

	virtual void layout();

	// get byte per pixel
	virtual int get_bpp() const
	{
		if ( m_suspended_image )
		{
			switch ( m_suspended_image->m_type )
			{
				default:
					return 0;

				case image::image_base::RGB:
					return 3;

				case image::image_base::RGBA:
					return 4;

				case image::image_base::ALPHA:
					return 1;
			};
		}

		return 0;
	}

	virtual unsigned char *get_data() const
	{
		if ( m_suspended_image )
		{
			return m_suspended_image->m_data;
		}

		return NULL;
	}

	virtual void activate()
	{

	}

	~bitmap_info_irrlicht()
	{
		if ( m_texture )
		{
			g_driver->removeTexture( m_texture );
			m_texture = NULL;
		}

        if ( m_suspended_image )
            delete m_suspended_image;
	}

	virtual int get_width() const
	{
		return m_width;
	}
	virtual int get_height() const
	{
		return m_height;
	}

};

#define BATCH_VERTEX    2000

struct sprite_batch
{
    bitmap_info_irrlicht     *m_currentTexture;
    video::S3DVertex    m_vertexBatch[BATCH_VERTEX];
    s16                 m_indexBatch[BATCH_VERTEX*3];

    int                 m_numVertex;
    int                 m_numIndex;    
    
    sprite_batch()
    {
        m_currentTexture    = NULL;
        m_numVertex         = 0;
        m_numIndex          = 0;
    }
    
    void set_texture( bitmap_info_irrlicht *tex )
    {
        m_currentTexture = tex;
    }
    
    void add_triangles( video::S3DVertex* vertex, int vertexCount, s16 *index, int indexCount )
    {        
        for (int i = 0; i < vertexCount; i++ )
            m_vertexBatch[m_numVertex+i] = vertex[i];
        
        for (int i = 0; i < indexCount; i++ )
            m_indexBatch[m_numIndex+i] = m_numVertex + index[i];
        
        m_numVertex += vertexCount;
        m_numIndex += indexCount;
    }
    
    void clear()
    {
        m_currentTexture    = NULL;
        m_numVertex         = 0;
        m_numIndex          = 0;
    }
};

struct video_handler_irrlicht : public gameswf::video_handler
{
	float m_scoord;
	float m_tcoord;
	gameswf::rgba   m_background_color;    
    
	video_handler_irrlicht() :
		m_scoord ( 0 ),
		m_tcoord ( 0 ),
		m_background_color ( 0,0,0,0 )
	{
	}

	~video_handler_irrlicht()
	{		
	}

	void display ( Uint8 *data, int width, int height,
	               const gameswf::matrix *m, const gameswf::rect *bounds, const gameswf::rgba &color )
	{
		
	}

};

struct render_handler_irrlicht : public gameswf::render_handler
{	
	IrrlichtDevice	*m_device;
	IVideoDriver	*m_driver;
	
	core::matrix4	m_projectionMatrix;
	core::matrix4	m_worldMatrix;
	core::matrix4	m_viewMatrix;

	core::matrix4	m_defaultProjectionMatrix;
	core::matrix4	m_defaultWorldMatrix;
	core::matrix4	m_defaultViewMatrix;

	SMaterial		*m_material;
	SColor			m_color;

	bool			m_enable_antialias;

	bool			m_isRenderMask;

	// Output size.
	float	m_display_width;
	float	m_display_height;

	gameswf::matrix	m_current_matrix;
	gameswf::cxform	m_current_cxform;

	int m_mask_level;	// nested mask level

    sprite_batch    m_spriteBatch;
    
	render_handler_irrlicht(irr::IrrlichtDevice	*dev) :
		m_device( dev ),
		m_enable_antialias ( false ),
		m_display_width ( 0 ),
		m_display_height ( 0 ),
		m_mask_level ( 0 ),
		m_isRenderMask( false ),
		m_material(NULL)
	{
		m_driver = m_device->getVideoDriver();

		// set global
		g_driver = m_driver;
		g_device = m_device;
	}

	~render_handler_irrlicht()
	{
	}

	void open()
	{		
	}

	void set_antialiased ( bool enable )
	{
		m_enable_antialias = enable;
	}
	
	struct fill_style
	{
		enum mode
		{
		    INVALID,
		    COLOR,
		    BITMAP_WRAP,
		    BITMAP_CLAMP,
		    LINEAR_GRADIENT,
		    RADIAL_GRADIENT,
		};

		render_handler_irrlicht* m_render;

		mode					m_mode;
		gameswf::rgba			m_color;
		gameswf::bitmap_info*	m_bitmap_info;
		gameswf::matrix			m_bitmap_matrix;
		gameswf::cxform			m_bitmap_color_transform;

		bool			m_has_nonzero_bitmap_additive_color;
		float			m_width;	// for line style

		mutable float	pS[4];
		mutable float	pT[4];

		fill_style() :
			m_mode ( INVALID ),
			m_has_nonzero_bitmap_additive_color ( false ),
			m_render( NULL )
		{
		}

		void	applyTexture ( int primitive_type, video::S3DVertex *vertex, int vertex_count, s16 *index, int index_count, const void *coords ) const		
		{
			bitmap_info_irrlicht * tex = NULL;

			if ( m_mode == BITMAP_WRAP || m_mode == BITMAP_CLAMP )
			{								
				Sint16 *vcoord = ( Sint16 * ) coords;				
				for ( int i = 0, j = 0; i < 2 * vertex_count; i = i + 2, j++ )
				{				
					vertex[j].TCoords.X = vcoord[i] * pS[0] + vcoord[i+1] * pS[1] + pS[3];
					vertex[j].TCoords.Y = vcoord[i] * pT[0] + vcoord[i+1] * pT[1] + pT[3];
				}
					
				tex = (bitmap_info_irrlicht*)m_bitmap_info;
			}
			
			if ( m_render->need_flush(tex) == true )
				m_render->flush_spritebatch();
			
			// set current texture
			m_render->m_spriteBatch.set_texture( tex );

			// add sprite batch
			if ( primitive_type ==  GL_TRIANGLE_STRIP )
			{
				s16 *indexBuff = new s16[index_count*4];
				int nIndex = 0;
				int i = 0;

				while ( i < index_count - 2 )
				{
					indexBuff[nIndex]	= index[i];
					indexBuff[nIndex+1]	= index[i+2];
					indexBuff[nIndex+2]	= index[i+1];
					i += 2;
					nIndex += 3;

					indexBuff[nIndex]	= index[i];
					indexBuff[nIndex+1]	= index[i+1];
					indexBuff[nIndex+2]	= index[i-1];
					nIndex += 3;
				}

				m_render->m_spriteBatch.add_triangles(vertex, vertex_count,indexBuff, nIndex);
				delete indexBuff;
			}
			else
			{
				m_render->m_spriteBatch.add_triangles(vertex, vertex_count,index, index_count);
			}

		}

		void	apply ( ) const
		// Push our style into OpenGL.
		{
			assert ( m_mode != INVALID );

			if ( m_mode == COLOR )
			{
				if ( m_render )
					m_render->apply_color ( m_color );				
			}

			else if ( m_mode == BITMAP_WRAP || m_mode == BITMAP_CLAMP )
			{
				assert ( m_bitmap_info != NULL );

				if ( m_render )
					m_render->apply_color ( m_color );

				if ( m_bitmap_info == NULL )
				{	
					// need disable texture
				}
				else
				{		
					// set color
					SColor color(
						(u32)(255*m_bitmap_color_transform.m_[0][0]),	//r
						(u32)(255*m_bitmap_color_transform.m_[1][0]),	//g
						(u32)(255*m_bitmap_color_transform.m_[2][0]),	//b
						(u32)(255*m_bitmap_color_transform.m_[3][0])	//a
					);
					
					// create texture
					m_bitmap_info->layout();
					((bitmap_info_irrlicht*)m_bitmap_info)->m_colorBlend = color;


					// Set up the bitmap matrix for texgen.
					float	inv_width	= 1.0f / m_bitmap_info->get_width();
					float	inv_height	= 1.0f / m_bitmap_info->get_height();
					const gameswf::matrix	&m = m_bitmap_matrix;
										
					pS[0] = m.m_[0][0] * inv_width;
					pS[1] = m.m_[0][1] * inv_width;
					pS[2] = 0;
					pS[3] = m.m_[0][2] * inv_width;
					
					pT[0] = m.m_[1][0] * inv_height;
					pT[1] = m.m_[1][1] * inv_height;
					pT[2] = 0;
					pT[3] = m.m_[1][2] * inv_height;					
				}
			}
		}


		bool	needs_second_pass() const
		// Return true if we need to do a second pass to make
		// a valid color.  This is for cxforms with additive
		// parts; this is the simplest way (that we know of)
		// to implement an additive color with stock OpenGL.
		{
			if ( m_mode == BITMAP_WRAP || m_mode == BITMAP_CLAMP )
			{
				return m_has_nonzero_bitmap_additive_color;
			}

			else
			{
				return false;
			}
		}

		void	apply_second_pass() const
		// Set OpenGL state for a necessary second pass.
		{
			
		}

		void	cleanup_second_pass() const
		{

		}


		void	disable()
		{
			m_mode = INVALID;
		}
		void	set_color ( gameswf::rgba color )
		{
			m_mode = COLOR;
			m_color = color;
		}
		void	set_bitmap ( gameswf::bitmap_info *bi, const gameswf::matrix &m, bitmap_wrap_mode wm, const gameswf::cxform &color_transform )
		{
			m_mode = ( wm == WRAP_REPEAT ) ? BITMAP_WRAP : BITMAP_CLAMP;
			m_bitmap_info = bi;
			m_bitmap_matrix = m;
			m_bitmap_color_transform = color_transform;
			m_bitmap_color_transform.clamp();
			m_color = gameswf::rgba 
				(
			              Uint8 ( m_bitmap_color_transform.m_[0][0] * 255.0f ),
			              Uint8 ( m_bitmap_color_transform.m_[1][0] * 255.0f ),
			              Uint8 ( m_bitmap_color_transform.m_[2][0] * 255.0f ),
			              Uint8 ( m_bitmap_color_transform.m_[3][0] * 255.0f ) 
				);

			if ( m_bitmap_color_transform.m_[0][1] > 1.0f
			     || m_bitmap_color_transform.m_[1][1] > 1.0f
			     || m_bitmap_color_transform.m_[2][1] > 1.0f
			     || m_bitmap_color_transform.m_[3][1] > 1.0f )
			{
				m_has_nonzero_bitmap_additive_color = true;
			}

			else
			{
				m_has_nonzero_bitmap_additive_color = false;
			}
		}
		bool	is_valid() const
		{
			return m_mode != INVALID;
		}
	};


	// Style state.
	enum style_index
	{
	    LEFT_STYLE = 0,
	    RIGHT_STYLE,
	    LINE_STYLE,

	    STYLE_COUNT
	};
	fill_style	m_current_styles[STYLE_COUNT];


	gameswf::bitmap_info	*create_bitmap_info_rgb ( image::rgb *im )
	// Given an image, returns a pointer to a bitmap_info struct
	// that can later be passed to fill_styleX_bitmap(), to set a
	// bitmap fill style.
	{
		return new bitmap_info_irrlicht ( im );
	}


	gameswf::bitmap_info	*create_bitmap_info_rgba ( image::rgba *im )
	// Given an image, returns a pointer to a bitmap_info struct
	// that can later be passed to fill_style_bitmap(), to set a
	// bitmap fill style.
	//
	// This version takes an image with an alpha channel.
	{
		return new bitmap_info_irrlicht ( im );
	}


	gameswf::bitmap_info	*create_bitmap_info_empty()
	// Create a placeholder bitmap_info.  Used when
	// DO_NOT_LOAD_BITMAPS is set; then later on the host program
	// can use movie_definition::get_bitmap_info_count() and
	// movie_definition::get_bitmap_info() to stuff precomputed
	// textures into these bitmap infos.
	{
		return new bitmap_info_irrlicht();
	}

	gameswf::bitmap_info	*create_bitmap_info_alpha ( int w, int h, Uint8 *data )
	// Create a bitmap_info so that it contains an alpha texture
	// with the given data (1 byte per texel).
	//
	// Munges *data (in order to make mipmaps)!!
	{
		return new bitmap_info_irrlicht ( w, h, data );
	}

	gameswf::video_handler	*create_video_handler()
	{
		return new video_handler_irrlicht();
	}

	void setOrtho(float left, float right, float bottom, float top, float znear, float zfar, core::matrix4& m)
	{
		float M[16];
		M[0] = 2.0f/(right - left);
		M[1] = 0;
		M[2] = 0;
		M[3] = 0;

		M[4] = 0;
		M[5] = 2.0f/(top - bottom);
		M[6] = 0;
		M[7] = 0;

		M[8] = 0;
		M[9] = 0;
		M[10] = -2/(zfar - znear);
		M[11] = 0;

		M[12] = -(right + left)/(right - left);
		M[13] = -(top + bottom)/(top - bottom);
		M[14] = -(zfar + znear)/(zfar - znear);
		M[15] = 1;
		
		m.setM(M);


		// ROTATE by phone orientation
		core::matrix4 r;
		float w = m_display_width;
		float h = m_display_height;

		E_ORIENTATION currentOrientation = g_driver->getOrientation();
		switch( currentOrientation )
		{		
		case EOO_90:
			{
				r.setTranslation(core::vector3df(0,h,0));
				r.setRotationDegrees(core::vector3df(0,0,-90));
				m *= r;
			}
			break;
		case EOO_270:
			{
				r.setTranslation(core::vector3df(w,0,0));
				r.setRotationDegrees(core::vector3df(0,0,90));
				m *= r;
			}
			break;
		default:
			return;
		}
	}

	void	begin_display (
	    gameswf::rgba background_color,
	    int viewport_x0, int viewport_y0,
	    int viewport_width, int viewport_height,
	    float x0, float x1, float y0, float y1 )
	// Set up to render a full frame from a movie and fills the
	// background.	Sets up necessary transforms, to scale the
	// movie to fit within the given dimensions.  Call
	// end_display() when you're done.
	//
	// The rectangle (viewport_x0, viewport_y0, viewport_x0 +
	// viewport_width, viewport_y0 + viewport_height) defines the
	// window coordinates taken up by the movie.
	//
	// The rectangle (x0, y0, x1, y1) defines the pixel
	// coordinates of the movie that correspond to the viewport
	// bounds.
	{
		
		// swap w,h by orientation
		E_ORIENTATION currentOrientation = g_driver->getOrientation();
		if ( currentOrientation == video::EOO_90 || currentOrientation == video::EOO_270 )
		{
			core::swap<float,float>(x0, y1);
			core::swap<float,float>(x1, y0);
		}

		m_display_width		= fabsf ( x1 - x0 );
		m_display_height	= fabsf ( y1 - y0 );		

		// set viewport
		core::recti rectViewport = core::recti(viewport_x0, viewport_y0, viewport_x0+viewport_width, viewport_y0+viewport_height);		
		m_driver->setViewPort( rectViewport );
		
		// set prj matrix
		m_projectionMatrix = core::IdentityMatrix;
		setOrtho( x0, x1, y0, y1, -1, 1, m_projectionMatrix );		

		// set ortho matrix
		m_worldMatrix = core::IdentityMatrix;
		
		// set world matrix (flip 0y)
		m_viewMatrix = core::IdentityMatrix;

		// translate by orientation
		float translate = m_display_height;
		if ( currentOrientation == video::EOO_90 || currentOrientation == video::EOO_270 )
			translate = m_display_width;

		core::matrix4 trans; trans.setTranslation( core::vector3df(0, translate, 0) );
		core::matrix4 scale; scale.setScale( core::vector3df(1.0, -1.0, 1.0) );
		m_viewMatrix *= trans;
		m_viewMatrix *= scale;		
						
		// save matrix
		m_defaultProjectionMatrix = g_driver->getTransform(ETS_PROJECTION);
		m_defaultWorldMatrix = g_driver->getTransform(ETS_WORLD);
		m_defaultViewMatrix = g_driver->getTransform(ETS_VIEW);

		// enable material 2d
		m_driver->enableMaterial2D();
		m_material = &m_driver->getMaterial2D();

		// set material
		m_material->Lighting = false;
		m_material->ZBuffer = ECFN_ALWAYS;
		m_material->ZWriteEnable = false;
		m_material->BackfaceCulling = false;
		m_material->FrontfaceCulling = false;

		// turn off flag
		m_isRenderMask = false;

		// clear z buffer
		m_driver->clearZBuffer();

		// Clear the background, if background color has alpha > 0.
		if ( background_color.m_a > 0 )
		{
			// Draw a big quad.			
			m_driver->draw2DRectangle(
					SColor(background_color.m_a, background_color.m_r, background_color.m_g, background_color.m_b),
					rectViewport
				);
		}

		//if ( g_fontTex )
		//{
		//	m_driver->draw2DImage(g_fontTex,
		//			core::vector2di(10,10), 
		//			core::recti(0,0,g_fontTex->getSize().Width,g_fontTex->getSize().Height), 
		//			NULL, 
		//			SColor(255,255,0,255), 
		//			true
		//		);
		//}
	}

	void	end_display()
	// Clean up after rendering a frame.  Client program is still
	// responsible for calling glSwapBuffers() or whatever.
	{
        flush_spritebatch();
        
		// restore matrix
		g_driver->setTransform(ETS_PROJECTION, m_defaultProjectionMatrix);
		g_driver->setTransform(ETS_WORLD, m_defaultWorldMatrix);
		g_driver->setTransform(ETS_VIEW, m_defaultViewMatrix);	
        
		// enable material 2d
		m_driver->enableMaterial2D(false);        
	}


	void	set_matrix ( const gameswf::matrix &m )
	// Set the current transform for mesh & line-strip rendering.
	{
		m_current_matrix = m;
	}


	void	set_cxform ( const gameswf::cxform &cx )
	// Set the current color transform for mesh & line-strip rendering.
	{
		m_current_cxform = cx;
	}

	void	apply_matrix ( const gameswf::matrix &m )
	// multiply current matrix with opengl matrix
	{
		core::matrix4 mulMatrix;
		float	*mat = mulMatrix.pointer();

		memset ( &mat[0], 0, sizeof ( mat ) );
		mat[0] = m.m_[0][0];
		mat[1] = m.m_[1][0];
		mat[4] = m.m_[0][1];
		mat[5] = m.m_[1][1];
		mat[10] = 1;
		mat[12] = m.m_[0][2];
		mat[13] = m.m_[1][2];
		mat[15] = 1;
		
		m_worldMatrix *= mulMatrix;
	}

	void	apply_color ( const gameswf::rgba &c )
	// Set the given color.
	{
		m_color = SColor( c.m_a, c.m_r, c.m_g, c.m_b );
	}

	void	fill_style_disable ( int fill_side )
	// Don't fill on the {0 == left, 1 == right} side of a path.
	{
		assert ( fill_side >= 0 && fill_side < 2 );
		m_current_styles[fill_side].disable();
	}


	void	line_style_disable()
	// Don't draw a line on this path.
	{
		m_current_styles[LINE_STYLE].disable();
	}


	void	fill_style_color ( int fill_side, const gameswf::rgba &color )
	// Set fill style for the left interior of the shape.  If
	// enable is false, turn off fill for the left interior.
	{
		assert ( fill_side >= 0 && fill_side < 2 );
		m_current_styles[fill_side].set_color ( m_current_cxform.transform ( color ) );
	}


	void	line_style_color ( gameswf::rgba color )
	// Set the line style of the shape.  If enable is false, turn
	// off lines for following curve segments.
	{
		m_current_styles[LINE_STYLE].set_color ( m_current_cxform.transform ( color ) );
	}


	void	fill_style_bitmap ( int fill_side, gameswf::bitmap_info *bi, const gameswf::matrix &m,
	                            bitmap_wrap_mode wm, bitmap_blend_mode bm )
	{
		assert ( fill_side >= 0 && fill_side < 2 );
		m_current_styles[fill_side].set_bitmap ( bi, m, wm, m_current_cxform );
	}

	void	line_style_width ( float width )
	{
		m_current_styles[LINE_STYLE].m_width = width;
	}


	void	draw_mesh_primitive ( int primitive_type, const void *coords, int vertex_count )
	// Helper for draw_mesh_strip and draw_triangle_list.
	{                
		// Set up current style.
		m_current_styles[LEFT_STYLE].m_render = this;
		m_current_styles[LEFT_STYLE].apply();
		
		// push matrix
		core::matrix4 viewMat = m_worldMatrix;

		// apply view matrix
		apply_matrix ( m_current_matrix );
		
		// set transform
		m_driver->setTransform( ETS_PROJECTION, m_projectionMatrix );
        m_driver->setTransform( ETS_VIEW,		m_viewMatrix );
		m_driver->setTransform( ETS_WORLD,		core::IdentityMatrix );		

		Sint16 *f = (Sint16*) coords;

		video::S3DVertex	*vertices	= new video::S3DVertex[ vertex_count ];
		s16					*index		= new s16[ vertex_count ];
		
		int index_count = vertex_count;
		
		// turn off transparent if is render mask
		if ( m_isRenderMask )
			m_color.setAlpha(255);

        core::vector3df pos;
        
		for ( int i = 0; i < vertex_count; i++ )
		{
            pos.X = (float)(*f);f++;
            pos.Y = (float)(*f);f++;
            m_worldMatrix.transformVect(pos);
            
			vertices[i].Pos.X = pos.X;
			vertices[i].Pos.Y = pos.Y;
			vertices[i].Color = m_color;

            
			index[i] = i;
		}
        
        // draw sprite
        m_current_styles[LEFT_STYLE].applyTexture ( primitive_type, vertices, vertex_count, index, index_count, coords );	
        
		// draw second pass
		//if ( m_current_styles[LEFT_STYLE].needs_second_pass() )
		//{
		//	m_current_styles[LEFT_STYLE].apply_second_pass();
					
			// todo later

		//	m_current_styles[LEFT_STYLE].cleanup_second_pass();
		//}								

		// pop matrix
		m_worldMatrix = viewMat;
		
		delete vertices;
		delete index;
	}

	void draw_mesh_strip ( const void *coords, int vertex_count )
	{
		draw_mesh_primitive ( GL_TRIANGLE_STRIP, coords, vertex_count );
	}

	void	draw_triangle_list ( const void *coords, int vertex_count )
	{
		draw_mesh_primitive ( GL_TRIANGLES, coords, vertex_count );
	}


	void	draw_line_strip ( const void *coords, int vertex_count )
	// Draw the line strip formed by the sequence of points.
	{        
		flush_spritebatch();
        
		// Set up current style.
		m_current_styles[LINE_STYLE].apply();

		// apply line width
		float scale = fabsf ( m_current_matrix.get_x_scale() ) + fabsf ( m_current_matrix.get_y_scale() );
		float w = m_current_styles[LINE_STYLE].m_width * scale / 2.0f;
		w = TWIPS_TO_PIXELS ( w );
			
		apply_matrix ( m_current_matrix );

		// to do draw line strip
		//
		// 
	}


	void	draw_bitmap (
	    const gameswf::matrix &m,
	    gameswf::bitmap_info *bi,
	    const gameswf::rect &coords,
	    const gameswf::rect &uv_coords,
	    gameswf::rgba color )
	// Draw a rectangle textured with the given bitmap, with the
	// given color.	 Apply given transform; ignore any currently
	// set transforms.
	//
	// Intended for textured glyph rendering.
	{
		assert ( bi );
		
		apply_color ( color );		

		gameswf::point a, b, c, d;
		m.transform ( &a, gameswf::point ( coords.m_x_min, coords.m_y_min ) );
		m.transform ( &b, gameswf::point ( coords.m_x_max, coords.m_y_min ) );
		m.transform ( &c, gameswf::point ( coords.m_x_min, coords.m_y_max ) );
		d.m_x = b.m_x + c.m_x - a.m_x;
		d.m_y = b.m_y + c.m_y - a.m_y;		

        core::vector3df     pos;
		video::S3DVertex	vertices[4];

        pos.X = a.m_x;
        pos.Y = a.m_y;
        m_worldMatrix.transformVect(pos);
        
		vertices[0].Pos.X = pos.X;
		vertices[0].Pos.Y = pos.Y;
		vertices[0].Pos.Z = 0.0f;
		vertices[0].TCoords.X = uv_coords.m_x_min;
		vertices[0].TCoords.Y = uv_coords.m_y_min;
		vertices[0].Color = m_color;

        pos.X = b.m_x;
        pos.Y = b.m_y;
        m_worldMatrix.transformVect(pos);
        
		vertices[1].Pos.X = pos.X;
		vertices[1].Pos.Y = pos.Y;
		vertices[1].Pos.Z = 0.0f;
		vertices[1].TCoords.X = uv_coords.m_x_max;
		vertices[1].TCoords.Y = uv_coords.m_y_min;
		vertices[1].Color = m_color;

        pos.X = c.m_x;
        pos.Y = c.m_y;
        m_worldMatrix.transformVect(pos);
        
		vertices[2].Pos.X = pos.X;
		vertices[2].Pos.Y = pos.Y;
		vertices[2].Pos.Z = 0.0f;
		vertices[2].TCoords.X = uv_coords.m_x_min;
		vertices[2].TCoords.Y = uv_coords.m_y_max;
		vertices[2].Color = m_color;

        pos.X = d.m_x;
        pos.Y = d.m_y;
        m_worldMatrix.transformVect(pos);
        
		vertices[3].Pos.X = pos.X;
		vertices[3].Pos.Y = pos.Y;
		vertices[3].Pos.Z = 0.0f;
		vertices[3].TCoords.X = uv_coords.m_x_max;
		vertices[3].TCoords.Y = uv_coords.m_y_max;
		vertices[3].Color = m_color;

		s16 index[] = {0,1,2, 1,3,2};
        
        
        bitmap_info_irrlicht* tex = (bitmap_info_irrlicht*)bi;        
        if ( need_flush( tex ) == true )
            flush_spritebatch();
        
        
        m_spriteBatch.set_texture(tex);
        m_spriteBatch.add_triangles(vertices, 4, index, 6);                    
	}

	bool test_stencil_buffer ( const gameswf::rect &bound, Uint8 pattern )
	{
		return false;		
	}
    
    bool need_flush( bitmap_info_irrlicht *tex )
    {
        int safe = 50;    
        
        if ( 
                m_spriteBatch.m_numIndex >= BATCH_VERTEX*3 - safe || 
                m_spriteBatch.m_numVertex >= BATCH_VERTEX - safe
            )
        {
            return true;
        }
        
        if ( m_spriteBatch.m_numVertex > 0 && m_spriteBatch.m_currentTexture != tex )
        {
            return true;
        }
        
        return false;
    }
    
    void flush_spritebatch()
    {
        if ( m_spriteBatch.m_numVertex == 0 || m_spriteBatch.m_numIndex == 0 )
            return;
        
        m_driver->setTransform( ETS_PROJECTION, m_projectionMatrix );
        m_driver->setTransform( ETS_VIEW,		m_viewMatrix );
		m_driver->setTransform( ETS_WORLD,		core::IdentityMatrix );
        
        SMaterial material;
        
		// set texture
        if ( m_spriteBatch.m_currentTexture )
        {
            m_spriteBatch.m_currentTexture->layout();
            material.setTexture(0, m_spriteBatch.m_currentTexture->m_texture );
            material.TextureLayer[0].BilinearFilter = true;
        }
        else
        {
            material.setTexture(0, NULL );
        }
        
		if ( m_isRenderMask )
			material.MaterialType = EMT_SOLID;
		else
			material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

		m_driver->setMaterial( material );
        
		m_driver->enableChangeProjectionMatrixWhenSetRenderMode( false );
		m_driver->draw2DVertexPrimitiveList(
                                            m_spriteBatch.m_vertexBatch,
                                            m_spriteBatch.m_numVertex,
                                            m_spriteBatch.m_indexBatch, 
                                            m_spriteBatch.m_numIndex/3,
                                            EVT_STANDARD,
                                            scene::EPT_TRIANGLES
                                            );
        
		m_driver->enableChangeProjectionMatrixWhenSetRenderMode( true );
        
        // clear sprite batch
        m_spriteBatch.clear();
    }

	void begin_submit_mask()
	{		
        flush_spritebatch();
        
		m_driver->clearZBuffer();

		m_isRenderMask = true;

		// write only write depth, disable color
		m_material->ZBuffer = video::ECFN_ALWAYS;
		m_material->ZWriteEnable = true;
		m_material->ColorMask = ECP_NONE;
	}

	// called after begin_submit_mask and the drawing of mask polygons
	void end_submit_mask()
	{
        flush_spritebatch();
        
		m_isRenderMask = false;

		// enable depth test, enable full color
		m_material->ZBuffer = video::ECFN_EQUAL;
		m_material->ZWriteEnable = false;
		m_material->ColorMask = ECP_ALL;
	}

	void disable_mask()
	{		
        flush_spritebatch();
        
		m_isRenderMask = false;

		m_material->ZBuffer = video::ECFN_NEVER;
		m_material->ZWriteEnable = false;
		m_material->ColorMask = ECP_ALL;

	}

	bool is_visible ( const gameswf::rect &bound )
	{
		gameswf::rect viewport;
		viewport.m_x_min = 0;
		viewport.m_y_min = 0;
		viewport.m_x_max = m_display_width;
		viewport.m_y_max = m_display_height;

		return viewport.bound_test ( bound );
	}

};	// end struct render_handler_irrlicht


// bitmap_info_irrlicht implementation

ITexture*	software_resample (
    int bytes_per_pixel,
    int src_width,
    int src_height,
    int src_pitch,
    uint8 *src_data,
    int dst_width,
    int dst_height )
// Code from Alex Streit
//
// Creates an OpenGL texture of the specified dst dimensions, from a
// resampled version of the given src image.  Does a bilinear
// resampling to create the dst image.
{	
	assert ( bytes_per_pixel == 3 || bytes_per_pixel == 4 );
	
	unsigned int	input_format = bytes_per_pixel == 3 ? GL_RGB : GL_RGBA;
	
	Uint8 *rescaled = new Uint8[dst_width * dst_height * bytes_per_pixel];
	float Uf, Vf;		// fractional parts
	float Ui, Vi;		// integral parts
	float w1, w2, w3, w4;	// weighting
	Uint8 *psrc;
	Uint8 *pdst = rescaled;
	// i1,i2,i3,i4 are the offsets of the surrounding 4 pixels
	const int i1 = 0;
	const int i2 = bytes_per_pixel;
	int i3 = src_pitch;
	int i4 = src_pitch + bytes_per_pixel;
	// change in source u and v
	float dv = ( float ) ( src_height - 2 ) / dst_height;
	float du = ( float ) ( src_width - 2 ) / dst_width;
	// source u and source v
	float U;
	float V = 0;
#define BYTE_SAMPLE(offset)	\
	(Uint8) (w1 * psrc[i1 + (offset)] + w2 * psrc[i2 + (offset)] + w3 * psrc[i3 + (offset)] + w4 * psrc[i4 + (offset)])

	if ( bytes_per_pixel == 3 )
	{
		for ( int v = 0; v < dst_height; ++v )
		{
			Vf = modff ( V, &Vi );
			V += dv;
			U = 0;

			for ( int u = 0; u < dst_width; ++u )
			{
				Uf = modff ( U, &Ui );
				U += du;
				w1 = ( 1 - Uf ) * ( 1 - Vf );
				w2 = Uf * ( 1 - Vf );
				w3 = ( 1 - Uf ) * Vf;
				w4 = Uf * Vf;
				psrc = &src_data[ ( int ) ( Vi * src_pitch ) + ( int ) ( Ui * bytes_per_pixel )];
				*pdst++ = BYTE_SAMPLE ( 0 );	// red
				*pdst++ = BYTE_SAMPLE ( 1 );	// green
				*pdst++ = BYTE_SAMPLE ( 2 );	// blue
				psrc += 3;
			}
		}
	}

	else
	{
		assert ( bytes_per_pixel == 4 );

		for ( int v = 0; v < dst_height; ++v )
		{
			Vf = modff ( V, &Vi );
			V += dv;
			U = 0;

			for ( int u = 0; u < dst_width; ++u )
			{
				Uf = modff ( U, &Ui );
				U += du;
				w1 = ( 1 - Uf ) * ( 1 - Vf );
				w2 = Uf * ( 1 - Vf );
				w3 = ( 1 - Uf ) * Vf;
				w4 = Uf * Vf;
				psrc = &src_data[ ( int ) ( Vi * src_pitch ) + ( int ) ( Ui * bytes_per_pixel )];
				*pdst++ = BYTE_SAMPLE ( 0 );	// red
				*pdst++ = BYTE_SAMPLE ( 1 );	// green
				*pdst++ = BYTE_SAMPLE ( 2 );	// blue
				*pdst++ = BYTE_SAMPLE ( 3 );	// alpha
				psrc += 4;
			}
		}
	}

	ITexture  *texture = create_texture ( input_format, dst_width, dst_height, rescaled, 0 );

	delete [] rescaled;
	return texture;
}

bitmap_info_irrlicht::bitmap_info_irrlicht() :
	m_texture (NULL),
	m_colorBlend(255,255,255,255),
	m_width ( 0 ),
	m_height ( 0 ),
	m_suspended_image ( 0 )
{
}

bitmap_info_irrlicht::bitmap_info_irrlicht ( image::rgba *im ) :
	m_texture (NULL),
	m_colorBlend(255,255,255,255),
	m_width ( im->m_width ),
	m_height ( im->m_height )
{
	assert ( im );
	m_suspended_image = image::create_rgba ( im->m_width, im->m_height );
	memcpy ( m_suspended_image->m_data, im->m_data, im->m_pitch * im->m_height );
}

bitmap_info_irrlicht::bitmap_info_irrlicht ( int width, int height, Uint8 *data ) :
	m_texture (NULL),
	m_colorBlend(255,255,255,255),
	m_width ( width ),
	m_height ( height )
{
	assert ( width > 0 && height > 0 && data );
	m_suspended_image = image::create_alpha ( width, height );
	memcpy ( m_suspended_image->m_data, data, m_suspended_image->m_pitch * m_suspended_image->m_height );
}

bitmap_info_irrlicht::bitmap_info_irrlicht ( image::rgb *im ) :
	m_texture (NULL),
	m_colorBlend(255,255,255,255),
	m_width ( im->m_width ),
	m_height ( im->m_height )
{
	assert ( im );
	m_suspended_image = image::create_rgb ( im->m_width, im->m_height );
	memcpy ( m_suspended_image->m_data, im->m_data, im->m_pitch * im->m_height );
}

// layout image to opengl texture memory
void bitmap_info_irrlicht::layout()
{
	if ( m_texture == NULL )
	{
		// need create texture
		assert ( m_suspended_image );
		
        m_dirty = false;
		m_width = m_suspended_image->m_width;
		m_height = m_suspended_image->m_height;
		int bpp = 4;
		int format = GL_RGBA;

		switch ( m_suspended_image->m_type )
		{
			case image::image_base::RGB:
				{
					bpp = 3;
					format = GL_RGB;
				}

			case image::image_base::RGBA:
				{
					int	w = p2 ( m_suspended_image->m_width );
					int	h = p2 ( m_suspended_image->m_height );

					if ( w != m_suspended_image->m_width || h != m_suspended_image->m_height )
					{
						// Faster/simpler software bilinear rescale.
						m_texture = software_resample
							( 
								bpp,
								m_suspended_image->m_width, m_suspended_image->m_height,
								m_suspended_image->m_pitch, 
								m_suspended_image->m_data,
								w, h 
							);
					}

					else
					{
						// Use original image directly.
						m_texture = create_texture ( format, w, h, m_suspended_image->m_data, 0 );
					}

					break;
				}

			case image::image_base::ALPHA:
				{
					int	w = m_suspended_image->m_width;
					int	h = m_suspended_image->m_height;
					m_texture = create_texture ( GL_ALPHA, w, h, m_suspended_image->m_data, 0 );
					break;
				}
			default:
				assert ( 0 );
		}
	}
	else 
    {
        if ( m_dirty )
        {            
            int format = GL_RGBA;
            int totalSize = m_width*m_height;
            
            switch ( m_suspended_image->m_type )
            {
                case image::image_base::RGB:
                    format = GL_RGB;
                    totalSize *= 3;
                    break;
                case image::image_base::RGBA:
                    format = GL_RGBA;
                    totalSize *= 4;
                    break;
                case image::image_base::ALPHA:
					format = GL_ALPHA;
					totalSize *= 4;
                    break;
            }
            
            void *imageData = get_texture_data(format, m_width, m_height, m_suspended_image->m_data);
            
            void *data = m_texture->lock();            
            memcpy(data, imageData, totalSize);
            m_texture->unlock();
            
            if (format == GL_ALPHA)
                delete imageData;
            
            m_dirty = false;
        }
    }
    
}

namespace gameswf
{
	render_handler	*create_render_handler_irrlicht(void *dev)
	// Factory.
	{
		return new render_handler_irrlicht( (irr::IrrlichtDevice*) dev);
	}
}

#endif	// USE_SDL

// Local Variables:
// mode: C++
// c-basic-offset: 8
// tab-width: 8
// indent-tabs-mode: t
// End:
