#include "stdafx.h"
// gameswf_render_handler_ogl.cpp	-- Willem Kokke <willem@mindparity.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A gameswf::render_handler that uses OpenGL ES

#ifdef _OPENGLES

#ifdef WIN32
	#include <windows.h>
	#include <GL/gl.h>
	#include "glext.h"
	#define USE_EXTPOINTER
#endif

#ifdef IPHONEOS
	//#define FLIP_TCOORD_Y
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
#endif
	
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_freetype.h"
//#include "gameswf/gameswf_video_ogl.h"
#include "base/image.h"
#include "base/utility.h"

#include <string.h>	// for memset()

//#define ENABLE_STENCIL_MASK
#define MASK_POS_Z_STEP		-0.001f
#define MASK_POS_Z_INIT		1.0f
#define MASK_BLEND_MODE		-1

// Pointers to opengl extension functions.
typedef char GLchar;

class BufferedRenderer
{
public:

#ifdef USE_EXTPOINTER
	// Extensions
	PFNGLACTIVETEXTUREARBPROC pGlActiveTextureARB;
#endif

	bool m_wire_frame;
	int m_vertex_count;
	int m_current_primitive_buffer_size;
	int m_primitive_type;
	gameswf::vector3df* m_positions;
	gameswf::vector2df* m_texcoords;
	unsigned int* m_colors;
	gameswf::bitmap_info* m_bi;
	gameswf::player_context* m_context;
	int m_blend_mode;
	int m_current_blend_mode;

	BufferedRenderer() :
		m_wire_frame(false),
		m_vertex_count(0),
		m_current_primitive_buffer_size(0),
		m_primitive_type(0),
		m_positions(NULL),
		m_texcoords(NULL),
		m_colors(NULL),
		m_bi(NULL),
		m_context(NULL),
		m_blend_mode(gameswf::blend_mode::NORMAL),
		m_current_blend_mode(gameswf::blend_mode::NORMAL)
	{
		ensureBufferCapacity(256);

#ifdef USE_EXTPOINTER
		pGlActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
#endif
	}

	~BufferedRenderer()
	{
		freeBuffers();
	}

	void setContext(gameswf::player_context* context)
	{
		m_context = context;
	}

	void setMaterial(gameswf::bitmap_info* bi)
	{
		if(bi != m_bi)
			flush();
		m_bi = bi;
	}

	void setBlendMode(int mode)
	{
		if(mode != m_blend_mode)
			flush();

		m_blend_mode = mode;
	}

	inline void activeTexture(unsigned int texture)
	{
#ifdef USE_EXTPOINTER
		if (pGlActiveTextureARB)
			pGlActiveTextureARB(GL_TEXTURE0 + texture);
#else
		glActiveTexture(GL_TEXTURE0 + texture);
#endif
	}

	inline bool supportsMultiTexture()
	{
#ifdef USE_EXTPOINTER
		return pGlActiveTextureARB != NULL;
#else
		return true;
#endif
	}

	void applyBlendMode()
	{
		if(m_blend_mode == m_current_blend_mode)
			return;

		m_current_blend_mode = m_blend_mode;

		GLenum srcFact = GL_SRC_ALPHA;
		GLenum dstFact = GL_ONE_MINUS_SRC_ALPHA;
		GLfloat modulate = 1.0f;
		bool blend = true;
		bool multi = false;

		switch(m_blend_mode)
		{
		case MASK_BLEND_MODE:
			blend = false;
			break;

		case gameswf::blend_mode::MULTIPLY:
			multi = true;
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			srcFact = GL_DST_COLOR;
			dstFact = GL_ONE_MINUS_SRC_ALPHA;

			if (supportsMultiTexture())
			{
				activeTexture(1);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA + 0, GL_PREVIOUS);
				//glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA + 1, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA + 0, GL_SRC_ALPHA);
				
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB + 0, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB + 1, GL_PREVIOUS);
				
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB + 0, GL_SRC_ALPHA);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB + 1, GL_SRC_COLOR);
			}
			break;
		
		case gameswf::blend_mode::SCREEN:
			multi = true;
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			srcFact = GL_ONE;
			dstFact = GL_ONE_MINUS_SRC_COLOR;

			if (supportsMultiTexture())
			{
				activeTexture(1);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA + 0, GL_PREVIOUS);
				//glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA + 1, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA + 0, GL_SRC_ALPHA);
				
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB + 0, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB + 1, GL_PREVIOUS);
				
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB + 0, GL_SRC_ALPHA);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB + 1, GL_SRC_COLOR);
			}
			break;
		
		case gameswf::blend_mode::OVERLAY:
			multi = true;
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			srcFact = GL_DST_COLOR;
			dstFact = GL_ONE;

			if (supportsMultiTexture())
			{
				activeTexture(1);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA + 0, GL_PREVIOUS);
				//glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA + 1, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA + 0, GL_SRC_ALPHA);
				
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB + 0, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB + 1, GL_PREVIOUS);
				
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB + 0, GL_SRC_ALPHA);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB + 1, GL_SRC_COLOR);
			}
			break;

		//case gameswf::blend_mode::NORMAL:
		//case gameswf::blend_mode::NORMAL1:
		default:
			m_current_blend_mode = m_blend_mode = gameswf::blend_mode::NORMAL;
			break;
		}

		// setup textures
		if(supportsMultiTexture())
		{
			activeTexture(1);
			if(multi && m_bi != NULL)
			{
				m_bi->layout();
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
			}
		}

		if(blend)
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, modulate);
			glBlendFunc(srcFact, dstFact);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.f);
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
		}

		if (supportsMultiTexture())
			activeTexture(0);
	}

	void queueBuffer(gameswf::vector3df* positions, gameswf::vector2df* texcoords, unsigned int color, int vertexCount, int pType)
	{
		if(m_vertex_count + vertexCount + 2 >= m_current_primitive_buffer_size || pType != m_primitive_type)
			flush();

		ensureBufferCapacity(vertexCount + 2);

		m_primitive_type = pType;

		if(m_vertex_count > 0 && m_primitive_type == GL_TRIANGLE_STRIP && !m_wire_frame)
		{
			// degenerate vertex #1
			m_positions[m_vertex_count] = positions[0];
			m_texcoords[m_vertex_count] = texcoords[0];
			m_colors[m_vertex_count] = color;
			m_vertex_count++;
		}

		// vertices
		memcpy(m_positions + m_vertex_count, positions, vertexCount * sizeof(gameswf::vector3df));
		memcpy(m_texcoords + m_vertex_count, texcoords, vertexCount * sizeof(gameswf::vector2df));
		for(int i = 0; i < vertexCount; i++)
			m_colors[m_vertex_count + i] = color;
		m_vertex_count += vertexCount;

		if(m_primitive_type == GL_TRIANGLE_STRIP && !m_wire_frame)
		{
			// degenerate vertex #2
			m_positions[m_vertex_count] = positions[vertexCount - 1];
			m_texcoords[m_vertex_count] = texcoords[vertexCount - 1];
			m_colors[m_vertex_count] = color;
			m_vertex_count++;
		}

		if(m_wire_frame)
		{
			flush();
		}
	}

	void queueIndexedTriangles(gameswf::vector3df* positions, gameswf::vector2df* texcoords, unsigned int color, int vertexCount, const unsigned short* indexes, int indexCount)
	{
		if(m_vertex_count + vertexCount >= m_current_primitive_buffer_size || m_primitive_type != GL_TRIANGLES)
			flush();

		ensureBufferCapacity(gameswf::imax(m_vertex_count + vertexCount, m_vertex_count + indexCount));
		m_primitive_type = GL_TRIANGLES;

		for(int i = 0; i < indexCount; i++)
		{
			int index = indexes[i];
			m_positions[m_vertex_count] = positions[index];
			m_texcoords[m_vertex_count] = texcoords[index];
			m_colors[m_vertex_count] = color;
			m_vertex_count++;
		}
	}

	void flush()
	{
		if(m_vertex_count > 0)
		{
			// it's important to transfer all texture caches to vram
			gameswf::texture_cache::unlock_all(m_context);

			const int primCount = m_primitive_type == GL_TRIANGLE_STRIP ? m_vertex_count - 2 : m_vertex_count/3;

			glColor4ub(255, 255, 255, 255);
			if(m_bi != NULL)
			{
				m_bi->layout();

				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 2, m_texcoords);
				glEnable(GL_TEXTURE_2D);
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
			}

			// apply current blend mode
			applyBlendMode();

			// Send the tris to OpenGL
			glDisableClientState(GL_NORMAL_ARRAY);
			
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_UNSIGNED_BYTE, 4, m_colors);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, m_positions);

#ifdef WIN32
			if(m_wire_frame)
			{
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_STENCIL_TEST);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glColorMask(1, 1, 1, 1);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				
				// invert clear color
				//float rgba[4];
				//glGetFloatv(GL_COLOR_CLEAR_VALUE, rgba);
				//rgba[0] = 1.0f - rgba[0];
				//rgba[1] = 1.0f - rgba[1];
				//rgba[2] = 1.0f - rgba[2];
				//glColor3f(rgba[0], rgba[1], rgba[2]);

				glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
				glDrawArrays(m_primitive_type, 0, m_vertex_count);

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			else
#endif
			{
				glDrawArrays(m_primitive_type, 0, m_vertex_count);
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
		}
		
		m_vertex_count = 0;
	}

	void ensureBufferCapacity(int size)
	{
		if(m_current_primitive_buffer_size < size)
		{
			freeBuffers();

			m_current_primitive_buffer_size = size;
			m_positions = swfnew gameswf::vector3df[size];
			m_texcoords = swfnew gameswf::vector2df[size];
			m_colors = swfnew unsigned int[size];
		}
	}

	void freeBuffers()
	{
		if(m_positions)
			swfdelete_array (m_positions, m_current_primitive_buffer_size);
		m_positions = NULL;

		if(m_texcoords)
			swfdelete_array (m_texcoords, m_current_primitive_buffer_size);
		m_texcoords = NULL;

		if(m_colors)
			swfdelete_array (m_colors, m_current_primitive_buffer_size);
		m_colors = NULL;
	}
};

//static GLint s_num_compressed_format = 0;
void create_texture(int format, int w, int h, void* data, int level)
{
	int internal_format = format;
	//if (s_num_compressed_format > 0)
	//{
	//	switch (format)
	//	{
	//		default:
	//			break;
	//		case GL_RGB :
	//			internal_format = GL_COMPRESSED_RGB_ARB;
	//			break;
	//		case GL_RGBA :
	//			internal_format = GL_COMPRESSED_RGBA_ARB;
	//			break;
	//		case GL_ALPHA :
	//			internal_format = GL_COMPRESSED_ALPHA_ARB;
	//			break;
	//		case GL_LUMINANCE :
	//			internal_format = GL_COMPRESSED_LUMINANCE_ARB;
	//			break;
	//	}
	//}

	glTexImage2D(GL_TEXTURE_2D, level, internal_format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
}

// We do not want to use GLU library
// gluPerspective ==> glFrustum
void gluPerspective(float fov, float aspect, float znear, float zfar)
{
	//	gluPerspective(fov, aspect, nnear, ffar) ==> glFrustum(left, right, bottom, top, nnear, ffar);
	//	fov * 0.5 = arctan ((top-bottom)*0.5 / near)
	//	Since bottom == -top for the symmetrical projection that gluPerspective() produces, then:
	//	top = tan(fov * 0.5) * near
	//	bottom = -top
	//	Note: fov must be in radians for the above formulae to work with the C math library. 
	//	If you have comnputer your fov in degrees (as in the call to gluPerspective()), 
	//	then calculate top as follows:
	//	top = tan(fov*3.14159/360.0) * near
	//	The left and right parameters are simply functions of the top, bottom, and aspect:
	//	left = aspect * bottom
	//	right = aspect * top

	float top = tan(fov * 3.141592f / 360.0f) * znear;
	float bottom = - top;
	float left = aspect* bottom;
	float right = aspect * top;

//#ifdef _OPENGL
//	glFrustumf(left, right, bottom, top, znear, zfar);
//#endif
}

// used by gluLookAt
void normalize(float v[3])
{
	float r = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
	if (r == 0.0f) return;
	v[0] /= r; v[1] /= r; v[2] /= r;
}

// used by gluLookAt
void cross(float v1[3], float v2[3], float result[3])
{
	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

// We do not want to use GLU library
void gluLookAt(float eyex, float eyey, float eyez, float centerx, float centery, 
							 float centerz, float upx, float upy, float upz)
{
    float forward[3];
    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    float up[3];
    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    normalize(forward);

    // Side = forward x up
    float side[3];
    cross(forward, up, side);
    normalize(side);

    // Recompute up as: up = side x forward
    cross(side, forward, up);

    GLfloat m[4][4];

		// make identity
		memset(&m[0], 0, sizeof(m));
		for (int i = 0; i < 4; i++)
		{
			m[i][i] = 1;
		}

    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];

    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];

    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];

    glMultMatrixf(&m[0][0]);
    glTranslatef(-eyex, -eyey, -eyez);
}

// choose the resampling method:
// 1 = hardware (experimental, should be fast, somewhat buggy)
// 2 = fast software bilinear (default)
// 3 = use image_resample(), slow software resampling
#define RESAMPLE_METHOD 2


// Determines whether to generate mipmaps for smoother rendering of
// minified textures.  It actually tends to look OK to not use
// mipmaps, though it is potentially a performance hit if you use big
// textures.
//
// TODO: need to code mipmap LOD bias adjustment, to keep mipmaps from
// looking too blurry.  (Also applies to text rendering.)
#define GENERATE_MIPMAPS 0


//inline bool opengl_accessible()
//{
//#ifdef WIN32
//	return wglGetCurrentContext() != 0;
//#else
//	return glXGetCurrentContext() != 0;
//#endif
//}

// bitmap_info_ogl declaration
struct bitmap_info_ogl : public gameswf::bitmap_info
{
	unsigned int	m_texture_id;
	gameswf::image_base* m_suspended_image;
	const gameswf::membuf* m_data;

	int m_width;
	int m_height;
	int m_writable;

	bitmap_info_ogl();
	bitmap_info_ogl(int width, int height, gameswf::Uint8* data);
	bitmap_info_ogl(gameswf::image_rgb* im);
	bitmap_info_ogl(gameswf::image_rgba* im);
	bitmap_info_ogl(gameswf::video_texture* texture);
	bitmap_info_ogl(int width, int height, const gameswf::membuf* data);

	virtual void layout();
	
	virtual void unlayout()
	{
		release();
	}

	virtual void activate()
	{
		assert(m_texture_id > 0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_texture_id);
	}

	virtual void set_writable()
	{
		m_writable = true;
	}

	virtual void* lock()
	{
		assert(m_writable && m_suspended_image);

		return m_suspended_image->m_data;
	}
	
	virtual void unlock()
	{
		assert(m_writable && m_suspended_image);

		layout();

		switch (m_suspended_image->m_type)
		{
			case gameswf::image_base::ALPHA:
			{
				int	w = m_suspended_image->m_width;
				int	h = m_suspended_image->m_height;
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_ALPHA, GL_UNSIGNED_BYTE, m_suspended_image->m_data);
				break;
			}

			case gameswf::image_base::RGBA:
			{
				int	w = m_suspended_image->m_width;
				int	h = m_suspended_image->m_height;
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, m_suspended_image->m_data);
				break;
			}
		}
	}

	// misc
	int p2(int n);

	void release()
	{
		if (m_texture_id > 0)
		{
			glDeleteTextures(1, (GLuint*) &m_texture_id);
			m_texture_id = 0;	// for debuging
		}

		if(m_suspended_image != NULL)
		{
			swfdelete (m_suspended_image);
			m_suspended_image = NULL;
		}

		if(m_data != NULL)
		{
			swfdelete (m_data);
			m_data = NULL;
		}
	}

	~bitmap_info_ogl()
	{
		release();
	}
		
	virtual int get_width() const { return m_width; }
	virtual int get_height() const { return m_height; }

};

struct render_handler_irrlicht : public gameswf::render_handler
{
	// Some renderer state.
	gameswf::orientation_mode m_orientation;
	BufferedRenderer m_bufferedRenderer;
	gameswf::player_context* m_context;
	gameswf::render_cache* m_render_cache;

	// Enable/disable antialiasing.
	bool	m_enable_antialias;
	
	// Output size.
	float	m_display_width;
	float	m_display_height;

	// buffers
	int m_current_primitive_buffer_size;
	gameswf::vector3df* m_positions;
	gameswf::vector2df* m_texcoords;
	
	gameswf::matrix	m_current_matrix;
	gameswf::cxform	m_current_cxform;

	int m_mask_level;	// nested mask level
	float m_pos_z; // pos z used for mask

	render_handler_irrlicht() :
		m_orientation(gameswf::ORIENTATION_0),
		m_enable_antialias(false),
		m_display_width(0),
		m_display_height(0),
		m_current_primitive_buffer_size(0),
		m_positions(NULL),
		m_texcoords(NULL),
		m_mask_level(0),
		m_pos_z(MASK_POS_Z_INIT),
		m_context(NULL),
		m_render_cache(NULL)
	{
	}

	~render_handler_irrlicht()
	{
		freeBuffers();
	}

	void ensureBufferCapacity(int size)
	{
		if(m_current_primitive_buffer_size < size)
		{
			freeBuffers();

			m_current_primitive_buffer_size = size;
			m_positions = swfnew gameswf::vector3df[size];
			m_texcoords = swfnew gameswf::vector2df[size];
		}
	}

	void freeBuffers()
	{
		if(m_positions)
			swfdelete_array (m_positions, m_current_primitive_buffer_size);
		m_positions = NULL;
		if(m_texcoords)
			swfdelete_array (m_texcoords, m_current_primitive_buffer_size);
		m_texcoords = NULL;
		m_current_primitive_buffer_size = 0;
	}

	void open()
	{
		// Scan for extensions used by gameswf
		//gllActiveTextureARB =  (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
		//glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) SDL_GL_GetProcAddress("glActiveTextureARB");
		//glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) SDL_GL_GetProcAddress("glClientActiveTextureARB");
		//glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) SDL_GL_GetProcAddress("glMultiTexCoord2fARB");
		//glMultiTexCoord2fvARB = (PFNGLMULTITEXCOORD2FVARBPROC) SDL_GL_GetProcAddress("glMultiTexCoord2fvARB");
		//glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC) SDL_GL_GetProcAddress("glGenFramebuffersEXT");
		//glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC) SDL_GL_GetProcAddress("glBindFramebufferEXT");
		//glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) SDL_GL_GetProcAddress("glFramebufferTexture2DEXT");
		//glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) SDL_GL_GetProcAddress("glCheckFramebufferStatusEXT");
		//glDeleteProgram = (PFNGLDELETEPROGRAMPROC) SDL_GL_GetProcAddress("glDeleteProgram");
		//glDeleteShader = (PFNGLDELETESHADERPROC) SDL_GL_GetProcAddress("glDeleteShader");
		//glCreateShader = (PFNGLCREATESHADERPROC) SDL_GL_GetProcAddress("glCreateShader");
		//glShaderSource = (PFNGLSHADERSOURCEPROC) SDL_GL_GetProcAddress("glShaderSource");
		//glCompileShader = (PFNGLCOMPILESHADERPROC) SDL_GL_GetProcAddress("glCompileShader");
		//glGetShaderiv = (PFNGLGETSHADERIVPROC) SDL_GL_GetProcAddress("glGetShaderiv");
		//glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) SDL_GL_GetProcAddress("glGetShaderInfoLog");
		//glCreateProgram = (PFNGLCREATEPROGRAMPROC) SDL_GL_GetProcAddress("glCreateProgram");
		//glAttachShader = (PFNGLATTACHSHADERPROC) SDL_GL_GetProcAddress("glAttachShader");
		//glLinkProgram = (PFNGLLINKPROGRAMPROC) SDL_GL_GetProcAddress("glLinkProgram");
		//glGetProgramiv = (PFNGLGETPROGRAMIVPROC) SDL_GL_GetProcAddress("glGetProgramiv");
		//glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) SDL_GL_GetProcAddress("glGetProgramInfoLog");
		//glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) SDL_GL_GetProcAddress("glValidateProgram");
		//glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) SDL_GL_GetProcAddress("glGetUniformLocation");
		//glUniform1f = (PFNGLUNIFORM1FPROC) SDL_GL_GetProcAddress("glUniform1f");
		//glUniform1i = (PFNGLUNIFORM1IPROC) SDL_GL_GetProcAddress("glUniform1i");
		//glUseProgram = (PFNGLUSEPROGRAMPROC) SDL_GL_GetProcAddress("glUseProgram");

		//glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &s_num_compressed_format);
	}

	virtual void reset()
	{
	}

	virtual void set_orientation(gameswf::orientation_mode mode)
	{
		m_orientation =  mode;
	}
	
	virtual gameswf::orientation_mode get_orientation()
	{
		return m_orientation;
	}

	void set_antialiased(bool enable)
	{
		// first try hardware FSAA (full screen antialiasing)
		//int aa_samples;
		//SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &aa_samples);
		//if (aa_samples > 0)
		//{
		//	if (enable)
		//	{
		//		glEnable(GL_MULTISAMPLE_ARB);
		//	}
		//	else
		//	{
		//		glDisable(GL_MULTISAMPLE_ARB);
		//	}
		//	return;
		//}

		//// there are no hardware antialiasing
		//// use edge antialiasing
		//m_enable_antialias = enable;
	}

	static void make_next_miplevel(int* width, int* height, gameswf::Uint8* data)
	// Utility.  Mutates *width, *height and *data to create the
	// next mip level.
	{
		assert(width);
		assert(height);
		assert(data);

		int	new_w = *width >> 1;
		int	new_h = *height >> 1;
		if (new_w < 1) new_w = 1;
		if (new_h < 1) new_h = 1;
		
		if (new_w * 2 != *width	 || new_h * 2 != *height)
		{
			// Image can't be shrunk along (at least) one
			// of its dimensions, so don't bother
			// resampling.	Technically we should, but
			// it's pretty useless at this point.  Just
			// change the image dimensions and leave the
			// existing pixels.
		}
		else
		{
			// Resample.  Simple average 2x2 --> 1, in-place.
			for (int j = 0; j < new_h; j++) {
				gameswf::Uint8*	out = ((gameswf::Uint8*) data) + j * new_w;
				gameswf::Uint8*	in = ((gameswf::Uint8*) data) + (j << 1) * *width;
				for (int i = 0; i < new_w; i++) {
					int	a;
					a = (*(in + 0) + *(in + 1) + *(in + 0 + *width) + *(in + 1 + *width));
					*(out) = a >> 2;
					out++;
					in += 2;
				}
			}
		}

		// Munge parameters to reflect the shrunken image.
		*width = new_w;
		*height = new_h;
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
		mode	m_mode;
		gameswf::rgba	m_color;
		gameswf::bitmap_info*	m_bitmap_info;
		gameswf::matrix	m_bitmap_matrix;
		gameswf::cxform	m_bitmap_color_transform;
		bool	m_has_nonzero_bitmap_additive_color;
		float m_width;	// for line style
		
		fill_style() :
			m_mode(INVALID),
			m_has_nonzero_bitmap_additive_color(false)
		{
		}

		void apply(gameswf::coord_component* vertices, gameswf::vector2df* tcoords, int vertex_count, gameswf::rgba& color) const
		// Push our style into OpenGL.
		{
			assert(m_mode != INVALID);
		
			if (m_mode == COLOR)
			{
				color = m_color;
				//apply_color(m_color);
				//glDisable(GL_TEXTURE_2D);
			}
			else
			if (m_mode == BITMAP_WRAP || m_mode == BITMAP_CLAMP)
			{
				assert(m_bitmap_info != NULL);

				color = m_color;
				//apply_color(m_color);

				if (m_bitmap_info == NULL)
				{
					//glDisable(GL_TEXTURE_2D);
					//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}
				else
				{
					// Set up the texture for rendering.
					{
						// Do the modulate part of the color
						// transform in the first pass.  The
						// additive part, if any, needs to
						// happen in a second pass.
						//glColor4f(m_bitmap_color_transform.m_[0][0],
						//	  m_bitmap_color_transform.m_[1][0],
						//	  m_bitmap_color_transform.m_[2][0],
						//	  m_bitmap_color_transform.m_[3][0]
						//	  );
						color = gameswf::rgba((gameswf::Uint8)(255 * m_bitmap_color_transform.m_[0][0]),
												(gameswf::Uint8)(255 * m_bitmap_color_transform.m_[1][0]),
												(gameswf::Uint8)(255 * m_bitmap_color_transform.m_[2][0]),
												(gameswf::Uint8)(255 * m_bitmap_color_transform.m_[3][0]));
					}

					//m_bitmap_info->layout();
//					glBindTexture(GL_TEXTURE_2D, m_bitmap_info->m_texture_id);
//					glEnable(GL_TEXTURE_2D);

					// Generate texture coordinates
					float	inv_width = 1.0f / m_bitmap_info->get_width();
					float	inv_height = 1.0f / m_bitmap_info->get_height();
					const gameswf::matrix&	m = m_bitmap_matrix;

					float	pu[4] = { 0, 0, 0, 0 };
					pu[0] = m.m_[0][0] * inv_width;
					pu[1] = m.m_[0][1] * inv_width;
					pu[3] = m.m_[0][2] * inv_width;

					float	pv[4] = { 0, 0, 0, 0 };
					pv[0] = m.m_[1][0] * inv_height;
					pv[1] = m.m_[1][1] * inv_height;
					pv[3] = m.m_[1][2] * inv_height;

					gameswf::coord_component* in = vertices;
					gameswf::vector2df* out = tcoords;
					for(int i = 0; i < vertex_count; i++)
					{
						float x0 = *(in++);
						float y0 = *(in++);
						out->x = (x0 * pu[0]) + (y0 * pu[1]) + pu[3];
#ifdef FLIP_TCOORD_Y
						out->y = 1.0f - ((x0 * pv[0]) + (y0 * pv[1]) + pv[3]);
#else
						out->y = (x0 * pv[0]) + (y0 * pv[1]) + pv[3];
#endif
						out++;
					}

					//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					//glTexCoordPointer(2, GL_FLOAT, 8, tcoords);
				}
			}
		}


		bool	needs_second_pass() const
		// Return true if we need to do a second pass to make
		// a valid color.  This is for cxforms with additive
		// parts; this is the simplest way (that we know of)
		// to implement an additive color with stock OpenGL.
		{
			if (m_mode == BITMAP_WRAP
			    || m_mode == BITMAP_CLAMP)
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
			assert(needs_second_pass());

			// The additive color also seems to be modulated by the texture. So,
			// maybe we can fake this in one pass using using the mean value of 
			// the colors: c0*t+c1*t = ((c0+c1)/2) * t*2
			// I don't know what the alpha component of the color is for.
			//glDisable(GL_TEXTURE_2D);

			glColor4f(
				m_bitmap_color_transform.m_[0][1] / 255.0f,
				m_bitmap_color_transform.m_[1][1] / 255.0f,
				m_bitmap_color_transform.m_[2][1] / 255.0f,
				m_bitmap_color_transform.m_[3][1] / 255.0f
				);

			glBlendFunc(GL_ONE, GL_ONE);
		}

		void	cleanup_second_pass() const
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}


		void	disable() { m_mode = INVALID; }
		void	set_color(gameswf::rgba color) { m_mode = COLOR; m_color = color; }
		void	set_bitmap(gameswf::bitmap_info* bi, const gameswf::matrix& m, bitmap_wrap_mode wm, const gameswf::cxform& color_transform)
		{
			m_mode = (wm == WRAP_REPEAT) ? BITMAP_WRAP : BITMAP_CLAMP;
			m_bitmap_info = bi;
			m_bitmap_matrix = m;
			m_bitmap_color_transform = color_transform;
			m_bitmap_color_transform.clamp();

			m_color = gameswf::rgba(
				gameswf::Uint8(m_bitmap_color_transform.m_[0][0] * 255.0f), 
				gameswf::Uint8(m_bitmap_color_transform.m_[1][0] * 255.0f), 
				gameswf::Uint8(m_bitmap_color_transform.m_[2][0] * 255.0f), 
				gameswf::Uint8(m_bitmap_color_transform.m_[3][0] * 255.0f));

			if (m_bitmap_color_transform.m_[0][1] > 1.0f
			    || m_bitmap_color_transform.m_[1][1] > 1.0f
			    || m_bitmap_color_transform.m_[2][1] > 1.0f
			    || m_bitmap_color_transform.m_[3][1] > 1.0f)
			{
				m_has_nonzero_bitmap_additive_color = true;
			}
			else
			{
				m_has_nonzero_bitmap_additive_color = false;
			}
		}
		bool	is_valid() const { return m_mode != INVALID; }
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


	gameswf::bitmap_info*	create_bitmap_info_rgb(gameswf::image_rgb* im)
	// Given an image, returns a pointer to a bitmap_info struct
	// that can later be passed to fill_styleX_bitmap(), to set a
	// bitmap fill style.
	{
		return swfnew bitmap_info_ogl(im);
	}


	gameswf::bitmap_info*	create_bitmap_info_rgba(gameswf::image_rgba* im)
	// Given an image, returns a pointer to a bitmap_info struct
	// that can later be passed to fill_style_bitmap(), to set a
	// bitmap fill style.
	//
	// This version takes an image with an alpha channel.
	{
		return swfnew bitmap_info_ogl(im);
	}

	gameswf::bitmap_info*	create_bitmap_info_texture(gameswf::video_texture* texture)
	// Given an image, returns a pointer to a bitmap_info struct
	// that can later be passed to fill_style_bitmap(), to set a
	// bitmap fill style.
	//
	// This version takes an image with an alpha channel.
	{
		return swfnew bitmap_info_ogl(texture);
	}

	gameswf::bitmap_info*	create_bitmap_info_native(int w, int h, const gameswf::membuf* data)
	{
		return swfnew bitmap_info_ogl(w, h, data);
	}

	gameswf::bitmap_info*	create_bitmap_info_target(int w, int h)
	{
		return NULL;
	}

	gameswf::bitmap_info*	create_bitmap_info_empty()
	// Create a placeholder bitmap_info.  Used when
	// DO_NOT_LOAD_BITMAPS is set; then later on the host program
	// can use movie_definition::get_bitmap_info_count() and
	// movie_definition::get_bitmap_info() to stuff precomputed
	// textures into these bitmap infos.
	{
		return swfnew bitmap_info_ogl();
	}

	gameswf::bitmap_info*	create_bitmap_info_alpha(int w, int h, gameswf::Uint8* data)
	// Create a bitmap_info so that it contains an alpha texture
	// with the given data (1 byte per texel).
	//
	// Munges *data (in order to make mipmaps)!!
	{
		return swfnew bitmap_info_ogl(w, h, data);
	}

	gameswf::video_handler*	create_video_handler()
	{
		return 0;
		//return swfnew video_ogl();
	}

#ifdef IPHONEOS
	void glOrtho(float left, float right, float bottom, float top, float near, float far)
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
		M[10] = -2/(far - near);
		M[11] = 0;

		M[12] = -(right + left)/(right - left);
		M[13] = -(top + bottom)/(top - bottom);
		M[14] = -(far + near)/(far - near);
		M[15] = 1;

		glLoadMatrixf(M);
	}
#endif

#define OVERSIZE	1.0f

	void setGLState()
	{
		//-----------------------
		// Open GL States
		//-----------------------

		glDisable(GL_DEPTH_TEST);	// Disable depth testing.
		glDisable(GL_CULL_FACE);
		//glDisable(GL_SCISSOR_TEST);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		// Turn on alpha blending.
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	// GL_MODULATE

		// Turn on line smoothing.  Antialiased lines can be used to
		// smooth the outsides of shapes.
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	// GL_NICEST, GL_FASTEST, GL_DONT_CARE
	}

	void	begin_display(
		gameswf::rgba background_color,
		int viewport_x0, int viewport_y0,
		int viewport_width, int viewport_height,
		float x0, float x1, float y0, float y1)
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
		m_display_width = fabsf(x1 - x0);
		m_display_height = fabsf(y1 - y0);

		glViewport(viewport_x0, viewport_y0, viewport_width, viewport_height);

		//-----------------------
		// Open GL Projection
		//-----------------------
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(-OVERSIZE, OVERSIZE, OVERSIZE, -OVERSIZE, -1, 1); // this without glScalef(1,-1,1);

		int mode = (int)get_orientation();
		if(mode != gameswf::ORIENTATION_0)
		{
			glRotatef(mode * 90.0f, 0, 0, 1);
		}

		//-----------------------
		// Open GL Model View
		//-----------------------

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//glScalef(1,-1,1);
		
		//-----------------------
		// End Projection
		//-----------------------

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glOrtho(x0, x1, y0, y1, -1, 1);

		setGLState();

		// init stuff for mask using depth buffer
#ifndef ENABLE_STENCIL_MASK
		glClear(GL_DEPTH_BUFFER_BIT);
#endif
		m_mask_level = 0;
		m_pos_z = MASK_POS_Z_INIT;

		// Clear the background, if background color has alpha > 0.
		if (background_color.m_a > 0)
		{
			// Draw a big quad.
			//apply_color(background_color);
			//glBegin(GL_QUADS);
			//glVertex2f(x0, y0);
			//glVertex2f(x1, y0);
			//glVertex2f(x1, y1);
			//glVertex2f(x0, y1);
			//glEnd();
		}

		// Old unused code.  Might get revived someday.
		#if 0
				// See if we want to, and can, use multitexture
				// antialiasing.
				s_multitexture_antialias = false;
				if (m_enable_antialias)
				{
					int	tex_units = 0;
					glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &tex_units);
					if (tex_units >= 2)
					{
						s_multitexture_antialias = true;
					}

					// Make sure we have an edge texture available.
					if (s_multitexture_antialias == true
						&& s_edge_texture_id == 0)
					{
						// Very simple texture: 2 texels wide, 1 texel high.
						// Both texels are white; left texel is all clear, right texel is all opaque.
						unsigned char	edge_data[8] = { 255, 255, 255, 0, 255, 255, 255, 255 };

						ogl::active_texture(GL_TEXTURE1_ARB);
						glEnable(GL_TEXTURE_2D);
						glGenTextures(1, &s_edge_texture_id);
						glBindTexture(GL_TEXTURE_2D, s_edge_texture_id);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, edge_data);

						glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	// @@ should we use a 1D texture???

						glDisable(GL_TEXTURE_2D);
						ogl::active_texture(GL_TEXTURE0_ARB);
						glDisable(GL_TEXTURE_2D);
					}
				}
		#endif // 0
	}


	void	end_display()
	// Clean up after rendering a frame.  Client program is still
	// responsible for calling glSwapBuffers() or whatever.
	{
		m_bufferedRenderer.flush();

		//render_glyph_texture_cache();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	void	set_blend_mode(gameswf::blend_mode::id mode)
	{
		m_bufferedRenderer.setBlendMode(mode);
	}

	void	set_target(gameswf::bitmap_info* target)
	{
	}

	void	clear_frame_buffer()
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void	read_frame_buffer(int x, int y, int width, int height, unsigned char* pixels)
	{
		glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	}

	void set_wire_frame(bool enabled)
	{
		m_bufferedRenderer.m_wire_frame = enabled;
	}

	void set_buffer_capacity(gameswf::render_handler::buffer_mode mode, int capacity)
	{
	}

	void set_context(gameswf::player_context* context)
	{
		assert(m_context == NULL || context == NULL);
		m_context = context;
		m_bufferedRenderer.setContext(context);
	}

	void set_render_cache(gameswf::render_cache* cache)
	{
		m_render_cache = cache;

		if(m_render_cache)
		{
			m_render_cache->clear();
		}
	}

	void render_glyph_texture_cache()
	{
		if(m_context->m_glyph_provider->get_texture_cache() != NULL)
		{
			bitmap_info_ogl* bi = (bitmap_info_ogl*)m_context->m_glyph_provider->get_texture_cache()->get_bitmap_info();
			gameswf::rect coords;
			coords.m_x_min = 0.0f;
			coords.m_x_max = PIXELS_TO_TWIPS(bi->get_width());
			coords.m_y_min = 0.0f;
			coords.m_y_max = PIXELS_TO_TWIPS(bi->get_height());

			// show whole picture
			gameswf::rect uv_coords;
			uv_coords.m_x_min = 0.0f;
			uv_coords.m_x_max = 1.0f;
			uv_coords.m_y_min = 0.0f;
			uv_coords.m_y_max = 1.0f;

			gameswf::cxform cx;
			gameswf::matrix m;
			draw_bitmap(m, bi, coords,	uv_coords, gameswf::rgba(255, 255, 255, 255));
			m_bufferedRenderer.flush();
		}
	}

	// called before user callback to backup/prepare states
	virtual void begin_display_callback()
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glEnable(GL_TEXTURE_2D);
	}

	// called after user callback to restore states
	virtual void end_display_callback()
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		setGLState();
	}

	void	set_matrix(const gameswf::matrix& m)
	// Set the current transform for mesh & line-strip rendering.
	{
		m_current_matrix = m;
	}


	void	set_cxform(const gameswf::cxform& cx)
	// Set the current color transform for mesh & line-strip rendering.
	{
		m_current_cxform = cx;
	}
	
	void	apply_matrix(const gameswf::matrix& m)
	// multiply current matrix with opengl matrix
	{
		float	mat[16];
		memset(&mat[0], 0, sizeof(mat));
		mat[0] = m.m_[0][0];
		mat[1] = m.m_[1][0];
		mat[4] = m.m_[0][1];
		mat[5] = m.m_[1][1];
		mat[10] = 1;
		mat[12] = m.m_[0][2];
		mat[13] = m.m_[1][2];
		mat[15] = 1;

		glMultMatrixf(mat);
	}

	static void	apply_color(const gameswf::rgba& c)
	// Set the given color.
	{
		glColor4ub(c.m_r, c.m_g, c.m_b, c.m_a);
	}

	void	fill_style_disable(int fill_side)
	// Don't fill on the {0 == left, 1 == right} side of a path.
	{
		assert(fill_side >= 0 && fill_side < 2);

		m_current_styles[fill_side].disable();
	}


	void	line_style_disable()
	// Don't draw a line on this path.
	{
		m_current_styles[LINE_STYLE].disable();
	}


	void	fill_style_color(int fill_side, const gameswf::rgba& color)
	// Set fill style for the left interior of the shape.  If
	// enable is false, turn off fill for the left interior.
	{
		assert(fill_side >= 0 && fill_side < 2);

		m_current_styles[fill_side].set_color(m_current_cxform.transform(color));
	}


	void	line_style_color(gameswf::rgba color)
	// Set the line style of the shape.  If enable is false, turn
	// off lines for following curve segments.
	{
		m_current_styles[LINE_STYLE].set_color(m_current_cxform.transform(color));
	}


	void	fill_style_bitmap(int fill_side, gameswf::bitmap_info* bi, const gameswf::matrix& m, bitmap_wrap_mode wm)
	{
		assert(fill_side >= 0 && fill_side < 2);
		m_current_styles[fill_side].set_bitmap(bi, m, wm, m_current_cxform);
	}
	
	void	line_style_width(float width)
	{
		m_current_styles[LINE_STYLE].m_width = width;
	}

	void	draw_mesh_primitive(int primitive_type, const void* coords, int vertex_count, const unsigned short* indexes, int index_count)
	// Helper for draw_mesh_strip and draw_triangle_list.
	{
#define NORMAL_RENDERING
//#define MULTIPASS_ANTIALIASING

#ifdef NORMAL_RENDERING

		if(m_current_styles[LEFT_STYLE].m_color.m_a == 0)
			return;
		
		ensureBufferCapacity(vertex_count + 2);

		gameswf::vector3df* pos = m_positions;
		gameswf::point world_pos;
		gameswf::point local_pos;
		gameswf::coord_component* in = (gameswf::coord_component*)coords;
		for(int i = 0; i < vertex_count; i++)
		{
			// convert local to world pos
			local_pos.m_x = (float)*(in++);
			local_pos.m_y = (float)*(in++);
			m_current_matrix.transform(&world_pos, local_pos);

			pos->x = world_pos.m_x;
			pos->y = world_pos.m_y;
			pos->z = m_pos_z;
			pos++;
		}

		// Set up current style.
		gameswf::rgba color;
		m_current_styles[LEFT_STYLE].apply((gameswf::coord_component*)coords, m_texcoords, vertex_count, color);

		// convert color to ARGB
		unsigned int argb = (color.m_a << 24) | (color.m_b << 16) | (color.m_g << 8) | color.m_r;

		// Buffer data
		gameswf::bitmap_info* bi = m_current_styles[LEFT_STYLE].m_mode == fill_style::COLOR ? NULL : m_current_styles[LEFT_STYLE].m_bitmap_info;
		m_bufferedRenderer.setMaterial(bi);

		if(indexes == NULL || index_count == 0)
			m_bufferedRenderer.queueBuffer(m_positions, m_texcoords, argb, vertex_count, primitive_type);
		else
			m_bufferedRenderer.queueIndexedTriangles(m_positions, m_texcoords, argb, vertex_count, indexes, index_count);
		
			//m_bufferedRenderer.flush();

		if(m_render_cache)
		{
			m_render_cache->record(bi, m_positions, m_texcoords, argb, vertex_count);
		}

		//glMatrixMode(GL_MODELVIEW);
		//glPushMatrix();
		
		//apply_matrix(m_current_matrix);

		// Send the tris to OpenGL
		//glEnableClientState(GL_VERTEX_ARRAY);

		//// ML
		//glDisableClientState(GL_NORMAL_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);
        //extGlBindBuffer(GL_ARRAY_BUFFER, 0);


		//#if TU_USES_FLOAT_AS_COORDINATE_COMPONENT
		//	glVertexPointer(2, GL_FLOAT, sizeof(float) * 2, coords);
		//#else
		//	glVertexPointer(2, GL_SHORT, sizeof(gameswf::Sint16) * 2, coords);
		//#endif

		//glDrawArrays(primitive_type, 0, vertex_count);

		//if (m_current_styles[LEFT_STYLE].needs_second_pass())
		//{
		//	m_current_styles[LEFT_STYLE].apply_second_pass();
		//	glDrawArrays(primitive_type, 0, vertex_count);
		//	m_current_styles[LEFT_STYLE].cleanup_second_pass();
		//}

		// the antialiasing of polygon edges
		//if (m_enable_antialias)
		//{
		//	//glEnable(GL_POLYGON_SMOOTH);
		//	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);	// GL_NICEST, GL_FASTEST, GL_DONT_CARE
		//	glDrawArrays(primitive_type, 0, vertex_count);
		//	//glDisable(GL_POLYGON_SMOOTH);
		//}

		//glDeleteBuffers(1, &vbo);

		//glDisableClientState(GL_VERTEX_ARRAY);

		//glPopMatrix();

#endif // NORMAL_RENDERING

#ifdef MULTIPASS_ANTIALIASING
		// So this approach basically works.  This
		// implementation is not totally finished; two pass
		// materials (i.e. w/ additive color) aren't correct,
		// and there are some texture etc issues because I'm
		// just hosing state uncarefully here.  It needs the
		// optimization of only filling the bounding box of
		// the shape.  You must have destination alpha.
		//
		// It doesn't look quite perfect on my GF4.  For one
		// thing, you kinda want to crank down the max curve
		// subdivision error, because suddenly you can see
		// sub-pixel shape much better.  For another thing,
		// the antialiasing isn't quite perfect, to my eye.
		// It could be limited alpha precision, imperfections
		// GL_POLYGON_SMOOTH, and/or my imagination.

		glDisable(GL_TEXTURE_2D);

		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);	// GL_NICEST, GL_FASTEST, GL_DONT_CARE

		// Clear destination alpha.
		//
		// @@ TODO Instead of drawing this huge screen-filling
		// quad, we should take a bounding-box param from the
		// caller, and draw the box (after apply_matrix;
		// i.e. the box is in object space).  The point being,
		// to only fill the part of the screen that the shape
		// is in.
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		glColor4f(1, 1, 1, 0);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(100000, 0);
		glVertex2f(100000, 100000);
		glVertex2f(0, 100000);
		glEnd();

		// Set mode for drawing alpha mask.
		glBlendFunc(GL_ONE, GL_ONE);	// additive blending
		glColor4f(0, 0, 0, m_current_styles[LEFT_STYLE].m_color.m_a / 255.0f);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		apply_matrix(m_current_matrix);

		// Send the tris to OpenGL.  This produces an
		// antialiased alpha mask of the mesh shape, in the
		// destination alpha channel.
		glEnableClientState(GL_VERTEX_ARRAY);
		#if TU_USES_FLOAT_AS_COORDINATE_COMPONENT
			glVertexPointer(2, GL_FLOAT, sizeof(float) * 2, coords);
		#else
			glVertexPointer(2, GL_SHORT, sizeof(gameswf::Sint16) * 2, coords);
		#endif
		glDrawArrays(primitive_type, 0, vertex_count);
		glDisableClientState(GL_VERTEX_ARRAY);

		glPopMatrix();
		
		// Set up desired fill style.
		m_current_styles[LEFT_STYLE].apply();

		// Apply fill, modulated with alpha mask.
		//
		// @@ TODO see note above about filling bounding box only.
		glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(100000, 0);
		glVertex2f(100000, 100000);
		glVertex2f(0, 100000);
		glEnd();

// xxxxx ??? Hm, is our mask still intact, or did we just erase it?
// 		if (m_current_styles[LEFT_STYLE].needs_second_pass())
// 		{
// 			m_current_styles[LEFT_STYLE].apply_second_pass();
// 			glDrawArrays(primitive_type, 0, vertex_count);
// 			m_current_styles[LEFT_STYLE].cleanup_second_pass();
// 		}

		// @@ hm, there is perhaps more state that needs
		// fixing here, or setting elsewhere.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif // MULTIPASS_ANTIALIASING
	}

	void draw_mesh_strip(const void* coords, int vertex_count)
	{
		draw_mesh_primitive(GL_TRIANGLE_STRIP, coords, vertex_count, NULL, 0);
	}
			
	void	draw_triangle_list(const void* coords, int vertex_count, const unsigned short* indexes, int index_count)
	{
		draw_mesh_primitive(GL_TRIANGLES, coords, vertex_count, indexes, index_count);
	}


	void	draw_line_strip(const void* coords, int vertex_count)
	// Draw the line strip formed by the sequence of points.
	{
		if(vertex_count != 5)
			return;

		m_bufferedRenderer.flush();
		m_bufferedRenderer.setMaterial(NULL);
		m_bufferedRenderer.applyBlendMode();

		ensureBufferCapacity(vertex_count + 2);

		// Set up current style.
		gameswf::rgba color;
		m_current_styles[LINE_STYLE].apply((gameswf::coord_component*)coords, m_texcoords, vertex_count, color);
		apply_color(color);
		glDisable(GL_TEXTURE_2D);

		// apply line width

		float scale = fabsf(m_current_matrix.get_x_scale()) + fabsf(m_current_matrix.get_y_scale());
		float w = m_current_styles[LINE_STYLE].m_width * scale / 2.0f;
		w = TWIPS_TO_PIXELS(w);

		//GLfloat width_info[2];
		//glGetFloatv(GL_LINE_WIDTH_RANGE, width_info); 
//		if (w > width_info[1])
//		{
//		printf("Your OpenGL implementation does not support the line width" \
//				" requested. Lines will be drawn with reduced width.");
//		}

		glLineWidth(w <= 1.0f ? 1.0f : w);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		apply_matrix(m_current_matrix);

		// ML
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
        //extGlBindBuffer(GL_ARRAY_BUFFER, 0);

		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	// ML
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// ML

		// Send the line-strip to OpenGL
		glEnableClientState(GL_VERTEX_ARRAY);
		#if TU_USES_FLOAT_AS_COORDINATE_COMPONENT
			glVertexPointer(2, GL_FLOAT, sizeof(float) * 2, coords);
		#else
			glVertexPointer(2, GL_SHORT, sizeof(gameswf::Sint16) * 2, coords);
		#endif
		glDrawArrays(GL_LINE_STRIP, 0, vertex_count);

		// Draw a round dot on the beginning and end coordinates to lines.
		glPointSize(w <= 1.0f ? 1.0f : w);
		glEnable(GL_POINT_SMOOTH);
		glDrawArrays(GL_POINTS, 0, vertex_count);
		glDisable(GL_POINT_SMOOTH);
		glPointSize(1);

		glDisableClientState(GL_VERTEX_ARRAY);

		// restore defaults
		glPointSize(1);
		glLineWidth(1);

		glPopMatrix();
	}

	void draw(gameswf::render_cache* cache)
	{
		gameswf::render_cache& rco = *cache;
		// check if in mask mode
		bool need_z_update = m_mask_level > 0;
		for(int i = 0; i < rco.m_dlist.size(); i++)
		{
			gameswf::render_cache::entry& entry = rco.m_dlist[i];
			m_bufferedRenderer.setMaterial(entry.bi);

			if(need_z_update)
			{
				for(int k = 0; k < entry.vertex_count; k++)
					rco.m_coords[entry.vertex_start + k].z = m_pos_z;
			}

			m_bufferedRenderer.queueBuffer(&rco.m_coords[entry.vertex_start], &rco.m_uv_coords[entry.vertex_start], entry.color, entry.vertex_count, GL_TRIANGLE_STRIP);
		}
	}

	void	draw_bitmap(
		const gameswf::matrix& m,
		gameswf::bitmap_info* bi,
		const gameswf::rect& coords,
		const gameswf::rect& uv_coords,
		gameswf::rgba color)
	// Draw a rectangle textured with the given bitmap, with the
	// given color.	 Apply given transform; ignore any currently
	// set transforms.
	//
	// Intended for textured glyph rendering.
	{
		assert(bi);

		m_bufferedRenderer.setMaterial(bi);

		gameswf::point a, b, c, d;
		m.transform(&a, gameswf::point(coords.m_x_min, coords.m_y_min));
		m.transform(&b, gameswf::point(coords.m_x_max, coords.m_y_min));
		m.transform(&c, gameswf::point(coords.m_x_min, coords.m_y_max));
		d.m_x = b.m_x + c.m_x - a.m_x;
		d.m_y = b.m_y + c.m_y - a.m_y;

		gameswf::vector3df positions[4] = {
			gameswf::vector3df(a.m_x, a.m_y, m_pos_z),
			gameswf::vector3df(b.m_x, b.m_y, m_pos_z),
			gameswf::vector3df(c.m_x, c.m_y, m_pos_z),
			gameswf::vector3df(d.m_x, d.m_y, m_pos_z)
		};

		// align to pixel and offset
		if(m_text_pixel_alignment)
		{
			for(int i = 0; i < 4; ++i)
			{
				positions[i].x = (float)((((int)positions[i].x + 10) / 20) * 20);// + (0.375f * 20.0f);
				positions[i].y = (float)((((int)positions[i].y + 10) / 20) * 20);// + (0.375f * 20.0f);
			}
		}

		gameswf::vector2df texcoords[4] = {
			gameswf::vector2df(uv_coords.m_x_min, uv_coords.m_y_min),
			gameswf::vector2df(uv_coords.m_x_max, uv_coords.m_y_min),
			gameswf::vector2df(uv_coords.m_x_min, uv_coords.m_y_max),
			gameswf::vector2df(uv_coords.m_x_max, uv_coords.m_y_max)
		};
		unsigned int argb = (color.m_a << 24) | (color.m_b << 16) | (color.m_g << 8) | color.m_r;
		m_bufferedRenderer.queueBuffer(positions, texcoords, argb, 4, GL_TRIANGLE_STRIP);

		if(m_render_cache)
		{
			m_render_cache->record(bi, positions, texcoords, argb, 4);
		}
	}
	
	void begin_submit_mask()
	{
		// using mask breaks the buffered pipeline
		m_bufferedRenderer.flush();

		// set material to opaque solid
		m_bufferedRenderer.setBlendMode(MASK_BLEND_MODE);

#ifdef ENABLE_STENCIL_MASK
		if (m_mask_level == 0)
		{
			//assert(glIsEnabled(GL_STENCIL_TEST) == false);
			glEnable(GL_STENCIL_TEST);
			glClearStencil(0);
			glClear(GL_STENCIL_BUFFER_BIT);
		}

		// disable framebuffer writes
		glColorMask(0, 0, 0, 0);

		// we set the stencil buffer to 'm_mask_level+1' 
		// where we draw any polygon and stencil buffer is 'm_mask_level'
		glStencilFunc(GL_EQUAL, m_mask_level++, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR); 
#else
		m_mask_level++;
		m_pos_z += MASK_POS_Z_STEP;

		// enable depth buffer write
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(true);

		// disable color write
		glColorMask(0, 0, 0, 0);
#endif
	}

	// called after begin_submit_mask and the drawing of mask polygons
	void end_submit_mask()
	{	
		// using mask breaks the buffered pipeline
		m_bufferedRenderer.flush();

		// set material to normal
		m_bufferedRenderer.setBlendMode(gameswf::blend_mode::NORMAL);

#ifdef ENABLE_STENCIL_MASK
		// enable framebuffer writes
		glColorMask(1, 1, 1, 1);

		// we draw only where the stencil is m_mask_level (where the current mask was drawn)
		glStencilFunc(GL_EQUAL, m_mask_level, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);	
#else
		// enable depth buffer compare "equal" with mask
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_EQUAL);
		glDepthMask(false);

		// enable color write
		glColorMask(1, 1, 1, 1);
#endif
	}

	void disable_mask()
	{	
		// using mask breaks the buffered pipeline
		m_bufferedRenderer.flush();
		assert(m_mask_level > 0);

#ifdef ENABLE_STENCIL_MASK
		if (--m_mask_level == 0)
		{
			glDisable(GL_STENCIL_TEST); 
			return;
		}

		// begin submit previous mask

		glColorMask(0, 0, 0, 0);

		// we set the stencil buffer to 'm_mask_level' 
		// where the stencil buffer m_mask_level + 1
		glStencilFunc(GL_EQUAL, m_mask_level + 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR); 

		// draw the quad to fill stencil buffer
		//glBegin(GL_QUADS);
		//glVertex2f(0, 0);
		//glVertex2f(m_display_width, 0);
		//glVertex2f(m_display_width, m_display_height);
		//glVertex2f(0, m_display_height);
		//glEnd();

		end_submit_mask();
#else
		m_mask_level--;

		// disable depth buffer
		glDisable(GL_DEPTH_TEST);
		glDepthMask(false);
		glColorMask(1, 1, 1, 1);
#endif
	}

	virtual void flush()
	{
		m_bufferedRenderer.flush();
	}

	bool is_visible(const gameswf::rect& bound)
	{
		gameswf::rect viewport;
		viewport.m_x_min = 0;
		viewport.m_y_min = 0;
		viewport.m_x_max = m_display_width;
		viewport.m_y_max = m_display_height;
		return viewport.bound_test(bound);
	}

#ifdef SDL_CURSOR_HANDLING
	// SDL cursor handling.
	sdl_cursor_handler m_cursor_handler;
	
	void set_cursor(cursor_type cursor)
	{
		m_cursor_handler.set_cursor(cursor);
	}
#endif  // SDL_CURSOR_HANDLING

};	// end struct render_handler_ogl


// bitmap_info_ogl implementation


#if (RESAMPLE_METHOD == 1)

void	hardware_resample(int bytes_per_pixel, int src_width, int src_height, gameswf::Uint8* src_data, int dst_width, int dst_height)
// Code from Alex Streit
//
// Sets the current texture to a resampled/expanded version of the
// given image data.
{
	assert(bytes_per_pixel == 3 || bytes_per_pixel == 4);

	unsigned int	in_format = bytes_per_pixel == 3 ? GL_RGB : GL_RGBA;
	unsigned int	out_format = bytes_per_pixel == 3 ? GL_RGB : GL_RGBA;

	// alex: use the hardware to resample the image
	// issue: does not work when image > allocated window size!
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT);
	{
		char* temp = swfnew char[dst_width * dst_height * bytes_per_pixel];
		//memset(temp,255,w*h*3);
		glTexImage2D(GL_TEXTURE_2D, 0, in_format, dst_width, dst_height, 0, out_format, GL_UNSIGNED_BYTE, temp);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, src_width, src_height, out_format, GL_UNSIGNED_BYTE, src_data);

		glLoadIdentity();
		glViewport(0, 0, dst_width, dst_height);
		glOrtho(0, dst_width, 0, dst_height, 0.9, 1.1);
		glColor3f(1, 1, 1);
		glNormal3f(0, 0, 1);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, (float) src_height / dst_height);
			glVertex3f(0, 0, -1);
			glTexCoord2f( (float) src_width / dst_width, (float) src_height / dst_height);
			glVertex3f((float) dst_width, 0, -1);
			glTexCoord2f( (float) src_width / dst_width, 0);
			glVertex3f((float) dst_width, (float) dst_height, -1);
			glTexCoord2f(0, 0);
			glVertex3f(0, (float) dst_height, -1);
		}
		glEnd();
		glCopyTexImage2D(GL_TEXTURE_2D, 0, out_format, 0,0, dst_width, dst_height, 0);
		swfdelete (temp);
	}
	glPopAttrib();
	glPopMatrix();
	glPopMatrix();
}

#endif

#if GENERATE_MIPMAPS
void	generate_mipmaps(unsigned int internal_format, unsigned int input_format, int bytes_per_pixel, image_image_base* im)
// DESTRUCTIVELY generate mipmaps of the given image.  The image data
// and width/height of im are munged in this process.
{
	int	level = 1;
	while (im->m_width > 1 || im->m_height > 1)
	{
		if (bytes_per_pixel == 3)
		{
			image_make_next_miplevel((gameswf::image_rgb*) im);
		}
		else
		{
			image_make_next_miplevel((gameswf::image_rgba*) im);
		}

//		glTexImage2D(GL_TEXTURE_2D, level, internal_format, im->m_width, im->m_height, 0,
//			     input_format, GL_UNSIGNED_BYTE, im->m_data);
		create_texture(input_format, im->m_width, im->m_height, im->m_data, level);
		level++;
	}
}
#endif

#if 1
	void	software_resample(
	int bytes_per_pixel,
	int src_width,
	int src_height,
	int src_pitch,
	gameswf::Uint8* src_data,
	int dst_width,
	int dst_height)
{
	unsigned int	internal_format = bytes_per_pixel == 3 ? GL_RGB : GL_RGBA;
	unsigned int	input_format = bytes_per_pixel == 3 ? GL_RGB : GL_RGBA;
	gameswf::Uint8* rescaled = swfnew gameswf::Uint8[dst_width * dst_height * bytes_per_pixel];

	const int dst_pitch = dst_width * bytes_per_pixel;
	const float sourceXStep = (float)src_width / (float)dst_width;
	const float sourceYStep = (float)src_height / (float)dst_height;
	int yval=0, syval=0;
	float sy = 0.0f;
	for (int y=0; y<dst_height; ++y)
	{
		float sx = 0.0f;
		for (int x=0; x<dst_width; ++x)
		{
			memcpy(rescaled + yval + (x*bytes_per_pixel), src_data + syval + ((int)sx) * bytes_per_pixel, bytes_per_pixel);
			sx+=sourceXStep;
		}
		sy+=sourceYStep;
		syval=((int)sy) * src_pitch;
		yval+=dst_pitch;
	}

	create_texture(input_format, dst_width, dst_height, rescaled, 0);
	delete [] rescaled;
}
#else

void	software_resample(
	int bytes_per_pixel,
	int src_width,
	int src_height,
	int src_pitch,
	gameswf::Uint8* src_data,
	int dst_width,
	int dst_height)
// Code from Alex Streit
//
// Creates an OpenGL texture of the specified dst dimensions, from a
// resampled version of the given src image.  Does a bilinear
// resampling to create the dst image.
{
//	printf("original bitmap %dx%d, resampled bitmap %dx%d\n",
//		src_width, src_height, dst_width, dst_height);

	assert(bytes_per_pixel == 3 || bytes_per_pixel == 4);

//	assert(dst_width >= src_width);
//	assert(dst_height >= src_height);

	unsigned int	internal_format = bytes_per_pixel == 3 ? GL_RGB : GL_RGBA;
	unsigned int	input_format = bytes_per_pixel == 3 ? GL_RGB : GL_RGBA;

	// FAST bi-linear filtering
	// the code here is designed to be fast, not readable
	gameswf::Uint8* rescaled = swfnew gameswf::Uint8[dst_width * dst_height * bytes_per_pixel];
	float Uf, Vf;		// fractional parts
	float Ui, Vi;		// integral parts
	float w1, w2, w3, w4;	// weighting
	gameswf::Uint8* psrc;
	gameswf::Uint8* pdst = rescaled;
	// i1,i2,i3,i4 are the offsets of the surrounding 4 pixels
	const int i1 = 0;
	const int i2 = bytes_per_pixel;
	int i3 = src_pitch;
	int i4 = src_pitch + bytes_per_pixel;
	// change in source u and v
	float dv = (float)(src_height) / dst_height;
	float du = (float)(src_width) / dst_width;
	// source u and source v
	float U;
	float V=0;

#define BYTE_SAMPLE(offset)	\
	(gameswf::Uint8) (w1 * psrc[i1 + (offset)] + w2 * psrc[i2 + (offset)] + w3 * psrc[i3 + (offset)] + w4 * psrc[i4 + (offset)])

	if (bytes_per_pixel == 3)
	{
		for (int v = 0; v < dst_height; ++v)
		{
			Vf = modff(V, &Vi);
			V+=dv;
			U=0;

			for (int u = 0; u < dst_width; ++u)
			{
				Uf = modff(U, &Ui);
				U+=du;

				w1 = (1 - Uf) * (1 - Vf);
				w2 = Uf * (1 - Vf);
				w3 = (1 - Uf) * Vf;
				w4 = Uf * Vf;
				psrc = &src_data[(int) (Vi * src_pitch) + (int) (Ui * bytes_per_pixel)];

				*pdst++ = BYTE_SAMPLE(0);	// red
				*pdst++ = BYTE_SAMPLE(1);	// green
				*pdst++ = BYTE_SAMPLE(2);	// blue

				psrc += 3;
			}
		}

#ifdef DEBUG_WRITE_TEXTURES_TO_PPM
		static int s_image_sequence = 0;
		char temp[256];
		sprintf(temp, "image%d.ppm", s_image_sequence++);
		FILE* f = fopen(temp, "wb");
		if (f)
		{
			fprintf(f, "P6\n# test code\n%d %d\n255\n", dst_width, dst_height);
			fwrite(rescaled, dst_width * dst_height * 3, 1, f);
			fclose(f);
		}
#endif
	}
	else
	{
		assert(bytes_per_pixel == 4);

		for (int v = 0; v < dst_height; ++v)
		{
			Vf = modff(V, &Vi);
			V+=dv;
			U=0;

			for (int u = 0; u < dst_width; ++u)
			{
				Uf = modff(U, &Ui);
				U+=du;

				w1 = (1 - Uf) * (1 - Vf);
				w2 = Uf * (1 - Vf);
				w3 = (1 - Uf) * Vf;
				w4 = Uf * Vf;
				psrc = &src_data[(int) (Vi * src_pitch) + (int) (Ui * bytes_per_pixel)];

				*pdst++ = BYTE_SAMPLE(0);	// red
				*pdst++ = BYTE_SAMPLE(1);	// green
				*pdst++ = BYTE_SAMPLE(2);	// blue
				*pdst++ = BYTE_SAMPLE(3);	// alpha

				psrc += 4;
			}
		}
	}

//	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, dst_width, dst_height, 0, input_format, GL_UNSIGNED_BYTE, rescaled);
	create_texture(input_format, dst_width, dst_height, rescaled, 0);

#if GENERATE_MIPMAPS
	// Build mipmaps.
	image_image_base	im(rescaled, dst_width, dst_height, dst_width * bytes_per_pixel);
	generate_mipmaps(internal_format, input_format, bytes_per_pixel, &im);
#endif // GENERATE_MIPMAPS

	delete [] rescaled;
}
#endif

int bitmap_info_ogl::p2(int n)
{
	int	p = 1; while (p < n) { p <<= 1; }

	// There is no sense to do 2048 from 1025
	// it is better to take 1024 instead of 1025, for example
	if ((float) n / (float) p < 0.6f)
	{
		p >>= 1;
	}
	return p;
}

bitmap_info_ogl::bitmap_info_ogl() :
	m_width(0),
	m_height(0),
	m_texture_id(0),
	m_suspended_image(0),
	m_data(NULL),
	m_writable(false)
{
}

bitmap_info_ogl::bitmap_info_ogl(gameswf::image_rgba* im) :
	m_texture_id(0),
	m_data(NULL),
	m_width(im->m_width),
	m_height(im->m_height),
	m_writable(false)
{
	assert(im);
	m_suspended_image = gameswf::create_rgba(im->m_width, im->m_height);
	memcpy(m_suspended_image->m_data, im->m_data, im->m_pitch * im->m_height);
}

bitmap_info_ogl::bitmap_info_ogl(int width, int height, gameswf::Uint8* data) :
	m_texture_id(0),
	m_data(NULL),
	m_width(width),
	m_height(height),
	m_writable(false)
{
	assert(width > 0 && height > 0);

	m_suspended_image = gameswf::create_alpha(width, height);

	if(data)
	{
		memcpy(m_suspended_image->m_data, data, m_suspended_image->m_pitch * m_suspended_image->m_height);
	}
}

bitmap_info_ogl::bitmap_info_ogl(gameswf::image_rgb* im) :
	m_texture_id(0),
	m_data(NULL),
	m_width(im->m_width),
	m_height(im->m_height),
	m_writable(false)
{
	assert(im);
	m_suspended_image = gameswf::create_rgb(im->m_width, im->m_height);
	memcpy(m_suspended_image->m_data, im->m_data, im->m_pitch * im->m_height);
}

bitmap_info_ogl::bitmap_info_ogl(gameswf::video_texture* texture) :
	m_texture_id(0),
	m_suspended_image(NULL),
	m_data(NULL),
	m_width(0/*texture->getSize().Width*/),
	m_height(0/*texture->getSize().Height*/),
	m_writable(false)
{
	// not implemented
}

bitmap_info_ogl::bitmap_info_ogl(int width, int height, const gameswf::membuf* data) :
	m_texture_id(0),
	m_suspended_image(NULL),
	m_data(data),
	m_width(width),
	m_height(height),
	m_writable(false)
{
	// not implemented
}

#define ENABLE_NATIVE_TEXTURE_FORMAT

#ifdef ENABLE_NATIVE_TEXTURE_FORMAT
GLuint
loadPVRTexture(const gameswf::membuf* file,
			   int& imageWidth,
			   int& imageHeight,
			   GLint& internalFormat,
			   GLenum& pixelFormat,
			   GLenum& pixelType,
			   gameswf::Uint32& mipmaps,
			   gameswf::Uint32 prefixSize = 0)
{
	// most of the code here is inspired by PVRTextureLoader sample from Apple

	// TODO: check endianness of host vs format (which is little endian)
	//_IRR_DEBUG_BREAK_IF(isBigEndian());

	gameswf::Uint32 fileBufferSize = file->size() - prefixSize;
	gameswf::Uint8* buffer = ((gameswf::Uint8*)file->data() + prefixSize);
	gameswf::Uint8* fileBufferEnd = buffer + fileBufferSize;
	gameswf::Uint8* pvrtDestBuffer = NULL;

	struct PVRHeader
	{
		gameswf::Uint32 HeaderLength;
		gameswf::Uint32 Height;
		gameswf::Uint32 Width;
		gameswf::Uint32 NumMipmaps;
		gameswf::Uint32 Flags;
		gameswf::Uint32 DataLength;
		gameswf::Uint32 BitsPerPixel;
		gameswf::Uint32 BitmaskRed;
		gameswf::Uint32 BitmaskGreen;
		gameswf::Uint32 BitmaskBlue;
		gameswf::Uint32 BitmaskAlpha;
		char PVRTag[4];
		gameswf::Uint32 NumSurfs;
	}* header = reinterpret_cast<PVRHeader*>(buffer);

	if (fileBufferSize < sizeof(PVRHeader)
		|| header->HeaderLength != sizeof(PVRHeader)
		|| strncmp(header->PVRTag, "PVR!", 4) != 0
		|| header->DataLength != fileBufferSize - sizeof(PVRHeader))
	{
		return 0;
	}
	buffer += sizeof(PVRHeader);

	mipmaps = header->NumMipmaps + 1;

	gameswf::Uint32 format = header->Flags & 0xff;

	gameswf::Uint32 blockSizeX = 1;
	gameswf::Uint32 blockSizeY = 1;
	gameswf::Uint32 minBlocks = 1;
	pixelType = GL_UNSIGNED_BYTE;
	bool isCompressed = false;

	// Format values taken from PVRTexLib
	switch (format)
	{
		case 0x10 : // OGL_RGBA_4444
		{
			internalFormat = pixelFormat = GL_RGBA;
			pixelType = GL_UNSIGNED_SHORT_4_4_4_4;
		}
		break;

		case 0x01 : // OGL_ARGB_1555
		{
			// TODO: needs validation (doesn't seems like this will work)
			internalFormat = GL_RGBA;
			pixelFormat = GL_BGRA;
			pixelType = GL_UNSIGNED_SHORT_5_5_5_1;
		}
		break;

		case 0x11 : // OGL_RGBA_5551
		{
			internalFormat = pixelFormat = GL_RGBA;
			pixelType = GL_UNSIGNED_SHORT_5_5_5_1;
		}
		break;

		case 0x12 : // OGL_RGBA_8888
		{
			internalFormat = pixelFormat = GL_RGBA;
		}
		break;
		
		case 0x13 : // OGL_RGB_565
		{
			internalFormat = pixelFormat = GL_RGB;
			pixelType = GL_UNSIGNED_SHORT_5_6_5;
		}
		break;

		case 0x15 : // OGL_RGB_888
		{
			internalFormat = pixelFormat = GL_RGB;
		}
		break;

		case 0x16 : // OGL_I_8
		{
			internalFormat = pixelFormat = GL_LUMINANCE;
		}
		break;

		case 0x17 : // OGL_AI_88
		{
			internalFormat = pixelFormat = GL_LUMINANCE_ALPHA;
		}
		break;

#if  defined(_IRR_WINDOWS_API_)
		// Decompress PVRT format in OpenGL RGB/RGBA texture (windows)
		case 0x18 : // OGL_PVRTC2
		{
			internalFormat = pixelFormat = GL_RGBA;

			pvrtDestBuffer = irrnew gameswf::Uint8[header->Width * header->Height * 4]; 
			PVRTCDecompress( buffer, 1, header->Width, header->Height, pvrtDestBuffer);
			buffer = pvrtDestBuffer;
		}
		break;

		case 0x19 : // OGL_PVRTC4
		{
			internalFormat = pixelFormat = GL_RGBA;

			pvrtDestBuffer = irrnew gameswf::Uint8[header->Width * header->Height * 4]; 
			PVRTCDecompress( buffer, 0, header->Width, header->Height, pvrtDestBuffer);
			buffer = pvrtDestBuffer;
		}
		break;

#elif defined(GL_IMG_texture_compression_pvrtc)
		// Below cases: 0x8000 is the bit mask for presence of alpha (from
		// PVRTexTool documentation)

		case 0x18 : // OGL_PVRTC2
		{
			internalFormat = pixelFormat
				= ((header->Flags & 0x8000)
				   ? GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG
				   : GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG);
			blockSizeX = 8;
			blockSizeY = 4;
			minBlocks = 2;
			isCompressed = true;
		}
		break;

		case 0x19 : // OGL_PVRTC4
		{
			internalFormat = pixelFormat
				= ((header->Flags & 0x8000)
				   ? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
				   : GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG);
			blockSizeX = blockSizeY = 4;
			minBlocks = 2;
			isCompressed = true;
		}
		break;
#endif

		default :
		{
			return 0;
		}
		break;
	}

	glEnable(GL_TEXTURE_2D);

	GLuint textureName = 0;
	glGenTextures(1, &textureName);
	if (textureName == 0)
	{
		return 0;
	}
	glBindTexture(GL_TEXTURE_2D, textureName);
	if (glGetError() != GL_NO_ERROR)
	{
		glDeleteTextures(1, &textureName);
		return 0;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// GL_NEAREST ?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLint oldUnpackAlignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &oldUnpackAlignment);
	if (oldUnpackAlignment != 1)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	bool error = false;
	GLsizei width = imageWidth = header->Width;
	GLsizei height = imageHeight = header->Height;
	for (gameswf::Uint32 m = 0; m <= header->NumMipmaps; ++m)
	{
		gameswf::Uint32 dataSize = (gameswf::imax(width / blockSizeX, minBlocks)
						* gameswf::imax(height / blockSizeY, minBlocks)
						* blockSizeX
						* blockSizeY
						* header->BitsPerPixel
						/ 8);

#if  defined(_IRR_WINDOWS_API_)
		if(buffer + dataSize > fileBufferEnd && format != 0x18 && format != 0x19 )
		{
			error = true;
			break;
		}
#else
		if (buffer + dataSize > fileBufferEnd)
		{
			error = true;
			break;
		}
#endif
		
		if (isCompressed)
		{
			/*driver->compressedTexImage2D(GL_TEXTURE_2D,
										 (GLint)m,
										 internalFormat,
										 width,
										 height,
										 0,
										 dataSize,
										 buffer);*/
			error = true;
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D,
						 (GLint)m,
						 internalFormat,
						 width,
						 height,
						 0,
						 pixelFormat, // not sure about this one
						 pixelType,
						 buffer);

			GLenum err = glGetError();
			if(err != GL_NO_ERROR)
				error = true;
		}

		buffer += dataSize;
		width = gameswf::imax(width >> 1, 1);
		height = gameswf::imax(height >> 1, 1);
	}

	// reset unpack alignment
	if (oldUnpackAlignment != 1)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, oldUnpackAlignment);
	}

	if (error)
	{
		imageWidth = imageHeight = 0;
		glDeleteTextures(1, &textureName);
		glBindTexture(GL_TEXTURE_2D, 0);
		textureName = 0;
	}

	if(pvrtDestBuffer)
		swfdelete_array (pvrtDestBuffer, header->Width * header->Height * 4);

	return textureName;
}
#endif

// layout image to opengl texture memory
void bitmap_info_ogl::layout()
{
	if (m_texture_id == 0)
	{
		if(m_suspended_image != NULL)
		{
			// Create the texture.
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, (GLuint*) &m_texture_id);
			glBindTexture(GL_TEXTURE_2D, m_texture_id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// GL_NEAREST ?
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			m_width = m_suspended_image->m_width;
			m_height = m_suspended_image->m_height;

			int bpp = 4;
			int format = GL_RGBA;

			switch (m_suspended_image->m_type)
			{
				case gameswf::image_base::RGB:
				{
					bpp = 3;
					format = GL_RGB;
				}

				case gameswf::image_base::RGBA:
				{
					int	w = p2(m_suspended_image->m_width);
					int	h = p2(m_suspended_image->m_height);
					if (w != m_suspended_image->m_width || h != m_suspended_image->m_height)
					{
						// Faster/simpler software bilinear rescale.
						software_resample(bpp, m_suspended_image->m_width, m_suspended_image->m_height,
							m_suspended_image->m_pitch, m_suspended_image->m_data, w, h);
					}
					else
					{
						// Use original image directly.
						create_texture(format, w, h, m_suspended_image->m_data, 0);
					}
					break;
				}

				case gameswf::image_base::ALPHA:
				{
					//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

					int	w = m_suspended_image->m_width;
					int	h = m_suspended_image->m_height;
					create_texture(GL_ALPHA, w, h, m_suspended_image->m_data, 0);

					// Build mips.
					//int	level = 1;
					//while (w > 1 || h > 1)
					//{
					//	render_handler_irrlicht::make_next_miplevel(&w, &h, m_suspended_image->m_data);
					//	create_texture(GL_ALPHA, w, h, m_suspended_image->m_data, level);
					//	level++;
					//}

					break;
				}

				default:
					assert(0);
			}

			if(m_writable == false)
			{
				swfdelete (m_suspended_image);
				m_suspended_image = NULL;
			}
		}
		else if(m_data != NULL)
		{
			if (m_data->size() > 8 && strncmp((char*)m_data->data(), "BTEX", 4) == 0)
			{
				if (strncmp(((char*)m_data->data()) + 4, "pvr", 4) == 0)
				{
					// create texture from native data
					char name[64];
					sprintf(name, "bitmap_info_ogl_0x%x", this);
					GLint internalFormat;
					GLenum pixelFormat;
					GLenum pixelType;
					gameswf::Uint32 mipmaps = 0;
					int newWidth = 0;
					int newHeight = 0;
					m_texture_id = loadPVRTexture(m_data, newWidth, newHeight, internalFormat, pixelFormat, pixelType, mipmaps, 8);

					swfdelete (m_data);
					m_data = NULL;
				}
			}
		}
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_texture_id);
		glEnable(GL_TEXTURE_2D);
	}
}

gameswf::render_handler*	gameswf::create_render_handler_irrlicht(void* videoDriver)
// Factory.
{
	return swfnew render_handler_irrlicht();
}

#endif // _IRRLICHT

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
