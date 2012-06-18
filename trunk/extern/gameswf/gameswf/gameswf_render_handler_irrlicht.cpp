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

ITexture *create_texture( int format, int w, int h, void *data, int level )
{	
	ECOLOR_FORMAT imageFormat = ECF_R8G8B8;
	int sizePixel = 4;

	void *imageData = data;

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

			// we need convert 8bit to 32bit
			imageData = new unsigned char[ w*h*sizePixel ];
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
			imageFormat = ECF_R8G8B8;
			sizePixel = 3;
		}
		break;
	}

	video::IImage *img = g_driver->createImage( imageFormat , irr::core::dimension2du(w,h) );
	memcpy(img->lock(), imageData, w*h*sizePixel);

	ITexture *tex = g_driver->addTexture( io::path("gameswf"), img );

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

struct video_handler_irrlicht : public gameswf::video_handler
{
	float m_scoord;
	float m_tcoord;
	gameswf::rgba m_background_color;

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
	core::matrix4	m_viewMatrix;

	SMaterial		m_material;

	bool	m_enable_antialias;

	// Output size.
	float	m_display_width;
	float	m_display_height;

	gameswf::matrix	m_current_matrix;
	gameswf::cxform	m_current_cxform;

	int m_mask_level;	// nested mask level

	render_handler_irrlicht(irr::IrrlichtDevice	*dev) :
		m_device( dev ),
		m_enable_antialias ( false ),
		m_display_width ( 0 ),
		m_display_height ( 0 ),
		m_mask_level ( 0 )
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
			m_has_nonzero_bitmap_additive_color ( false )
		{
		}

		void	applyTexture ( int primitive_type, const void *coords, int vertex_count ) const		
		{
			if ( m_mode == BITMAP_WRAP || m_mode == BITMAP_CLAMP )
			{
				assert ( m_bitmap_info != NULL );
				irr::f32 *tcoord = new irr::f32[2 * vertex_count];
				Sint16 *vcoord = ( Sint16 * ) coords;

				for ( int i = 0; i < 2 * vertex_count; i = i + 2 )
				{
					tcoord[i]	= vcoord[i] * pS[0] + vcoord[i+1] * pS[1] + pS[3];
					tcoord[i+1] = vcoord[i] * pT[0] + vcoord[i+1] * pT[1] + pT[3];
				}
			

				delete tcoord;
			}
		}

		void	apply ( ) const
		// Push our style into OpenGL.
		{
			assert ( m_mode != INVALID );

			if ( m_mode == COLOR )
			{
				apply_color ( m_color );				
			}

			else if ( m_mode == BITMAP_WRAP || m_mode == BITMAP_CLAMP )
			{
				assert ( m_bitmap_info != NULL );
				apply_color ( m_color );

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
		m_display_width		= fabsf ( x1 - x0 );
		m_display_height	= fabsf ( y1 - y0 );
								
		// set viewport
		core::recti rectViewport = core::recti(viewport_x0, viewport_y0, viewport_x0+viewport_width, viewport_y0+viewport_height);
		m_driver->setViewPort( rectViewport );
		
		// set ortho matrix
		setOrtho( x0, x1, y0, y1, -1, 1, m_projectionMatrix );
		
		// Clear the background, if background color has alpha > 0.
		if ( background_color.m_a > 0 )
		{
			// Draw a big quad.
			apply_color ( background_color );
			
			// clear screen
			m_driver->draw2DRectangle(
					SColor(background_color.m_a, background_color.m_r, background_color.m_g, background_color.m_b),
					rectViewport
				);
		}

	}

	void	end_display()
	// Clean up after rendering a frame.  Client program is still
	// responsible for calling glSwapBuffers() or whatever.
	{
		
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
		float	mat[16];
		memset ( &mat[0], 0, sizeof ( mat ) );
		mat[0] = m.m_[0][0];
		mat[1] = m.m_[1][0];
		mat[4] = m.m_[0][1];
		mat[5] = m.m_[1][1];
		mat[10] = 1;
		mat[12] = m.m_[0][2];
		mat[13] = m.m_[1][2];
		mat[15] = 1;
		
		m_viewMatrix.setM( mat );
	}

	static void	apply_color ( const gameswf::rgba &c )
	// Set the given color.
	{		
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
		m_current_styles[LEFT_STYLE].apply();


		apply_matrix ( m_current_matrix );


		if ( m_current_styles[LEFT_STYLE].needs_second_pass() )
		{
			m_current_styles[LEFT_STYLE].apply_second_pass();
			
			// draw primitive
			//glDrawArrays ( primitive_type, 0, vertex_count );

			m_current_styles[LEFT_STYLE].cleanup_second_pass();
		}

		
		m_current_styles[LEFT_STYLE].applyTexture ( primitive_type, coords, vertex_count );		
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
		
		bi->layout();
		apply_color ( color );

		video::SColor irrColor( color.m_a, color.m_r, color.m_g, color.m_b ); 
		
		gameswf::point a, b, c, d;
		m.transform ( &a, gameswf::point ( coords.m_x_min, coords.m_y_min ) );
		m.transform ( &b, gameswf::point ( coords.m_x_max, coords.m_y_min ) );
		m.transform ( &c, gameswf::point ( coords.m_x_min, coords.m_y_max ) );
		d.m_x = b.m_x + c.m_x - a.m_x;
		d.m_y = b.m_y + c.m_y - a.m_y;		

		video::S3DVertex	vertices[4];

		vertices[0].Pos.X = a.m_x;
		vertices[0].Pos.Y = a.m_y;
		vertices[0].Pos.Z = 0.0;
		vertices[0].TCoords.X = uv_coords.m_x_min;
		vertices[0].TCoords.Y = uv_coords.m_y_min;
		vertices[0].Color = irrColor;

		vertices[1].Pos.X = b.m_x;
		vertices[1].Pos.Y = b.m_y;
		vertices[1].Pos.Z = 0.0;
		vertices[1].TCoords.X = uv_coords.m_x_max;
		vertices[1].TCoords.Y = uv_coords.m_y_min;
		vertices[1].Color = irrColor;

		vertices[2].Pos.X = c.m_x;
		vertices[2].Pos.Y = c.m_y;
		vertices[2].Pos.Z = 0.0;
		vertices[2].TCoords.X = uv_coords.m_x_min;
		vertices[2].TCoords.Y = uv_coords.m_y_max;
		vertices[2].Color = irrColor;

		vertices[3].Pos.X = d.m_x;
		vertices[3].Pos.Y = d.m_y;
		vertices[3].Pos.Z = 0.0;
		vertices[3].TCoords.X = uv_coords.m_x_max;
		vertices[3].TCoords.Y = uv_coords.m_y_max;
		vertices[3].Color = irrColor;

		s16 index[] = {0,2,3,1};		

		m_driver->setTransform( ETS_PROJECTION, m_projectionMatrix );
		m_driver->setTransform( ETS_WORLD,		core::IdentityMatrix );
		m_driver->setTransform( ETS_VIEW,		core::IdentityMatrix );

		// set texture
		m_material.setTexture(0, ((bitmap_info_irrlicht*)bi)->m_texture );		
		m_material.Lighting = false;
		m_material.ZBuffer = false;
		m_material.ZWriteEnable = false;
		m_material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

		m_driver->setMaterial( m_material );
		m_driver->drawVertexPrimitiveList(
				vertices, 4,
				index, 2,
				EVT_STANDARD,
				scene::EPT_TRIANGLE_FAN 
			);		
	}

	bool test_stencil_buffer ( const gameswf::rect &bound, Uint8 pattern )
	{
		return false;		
	}

	void begin_submit_mask()
	{
		
	}

	// called after begin_submit_mask and the drawing of mask polygons
	void end_submit_mask()
	{
		
	}

	void disable_mask()
	{
		
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

		delete m_suspended_image;
		m_suspended_image = NULL;
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
