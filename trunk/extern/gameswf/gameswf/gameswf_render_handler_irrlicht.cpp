#include "stdafx.h"
// gameswf_render_handler_ogl.cpp	-- Willem Kokke <willem@mindparity.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A gameswf::render_handler that uses irrlicht

#include "base/tu_types.h"

#ifdef _IRRLICHT

#include "ITexture.h"
#include "IVideoDriver.h"
#include "IReadFile.h"

#ifdef RVL_OS
	#include <revolution.h>
	#include "CWiiDriver.h"
#elif defined _IRR_COMPILE_WITH_OPENGL_ES_
	#include "COpenGLESDriver.h"	// for opengl const
	#include "CCommonGLMaterialRenderer.h"	// material renderer
	#define MERGE_STRIPS
#elif defined _IRR_COMPILE_WITH_DIRECT3D_8_ || defined _IRR_COMPILE_WITH_DIRECT3D_9_
	#define USE_S3DVERTEX
	#define MERGE_STRIPS
#elif defined _IRR_COMPILE_WITH_PS3_
	#include "CPS3Driver.h"	// for opengl const
	#include "CCommonGLMaterialRenderer.h"	// material renderer
	#define MERGE_STRIPS
#else
	#include "COpenGLDriver.h"	// for opengl const
	#include "CCommonGLMaterialRenderer.h"	// material renderer
	#define MERGE_STRIPS
#endif

using namespace irr::video;

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_freetype.h"
#include "gameswf/gameswf_bitmapfont.h"
#include "gameswf/gameswf_filters.h"
#include "gameswf/gameswf_tesselate.h"
//#include "gameswf/gameswf_video_ogl.h"
#include "base/image.h"
#include "base/utility.h"

#include <string.h>	// for memset()

#define ENABLE_MASK_INTERSECTION
#define MASK_POS_Z_STEP		0.001f
#define MASK_POS_Z_INIT		0.0f

#define ZBUFFER_DISABLED	0
#define ZBUFFER_LEQUAL		1
#define ZBUFFER_EQUAL		2

// Pointers to opengl extension functions.
typedef char GLchar;

static irr::video::SMaterial s_material;

#ifdef RVL_OS

#include "gameswf_irrlicht_material_renderer_wii.hpp"

#elif defined _IRR_COMPILE_WITH_OPENGL_ES_

#define PLATFORM_DRIVER					irr::video::COpenGLESDriver
#define PLATFORM_MATERIAL_RENDERER		irr::video::CCommonGLMaterialRenderer
#include "gameswf_irrlicht_material_renderer_gl.hpp"

#elif defined _IRR_COMPILE_WITH_DIRECT3D_8_ || defined _IRR_COMPILE_WITH_DIRECT3D_9_

//#define PLATFORM_DRIVER		irr::video::CD3D9Driver
//#include "gameswf_irrlicht_material_renderer_d3d.hpp"

#elif defined _IRR_COMPILE_WITH_PS3_

#define PLATFORM_DRIVER					irr::video::CPS3Driver
#define PLATFORM_MATERIAL_RENDERER		irr::video::CCommonGLMaterialRenderer
#include "gameswf_irrlicht_material_renderer_gl.hpp"

#else // OPENGL / WIN32

#define PLATFORM_DRIVER					irr::video::COpenGLDriver
#define PLATFORM_MATERIAL_RENDERER		irr::video::CCommonGLMaterialRenderer
#include "gameswf_irrlicht_material_renderer_gl.hpp"

#endif

class BufferedRenderer
{
public:
	gameswf::render_handler::buffer_mode m_buffer_mode;
	bool m_wire_frame;
	int m_flush_offset;
	int m_vertex_count;
	int m_index_count;
	int m_current_primitive_buffer_size;
	irr::scene::E_PRIMITIVE_TYPE m_primitive_type;
	irr::core::vector3df* m_positions;
	irr::core::vector2df* m_texcoords;
	irr::video::SColor* m_colors;
	irr::video::S3DVertex* m_s3dvertices;
	SMaterial m_material;
	unsigned short* m_indexes;
	irr::video::IVideoDriver* m_videoDriver;
	gameswf::player_context* m_context;

	BufferedRenderer() :
		m_buffer_mode(gameswf::render_handler::BUFFER_MODE_REUSE),
		m_wire_frame(false),
		m_flush_offset(0),
		m_vertex_count(0),
		m_index_count(0),
		m_current_primitive_buffer_size(0),
		m_primitive_type(irr::scene::EPT_TRIANGLE_STRIP),
		m_positions(NULL),
		m_texcoords(NULL),
		m_colors(NULL),
		m_s3dvertices(NULL),
		m_indexes(NULL),
		m_videoDriver(NULL),
		m_context(NULL)
	{
		ensureBufferCapacity(1024);
	}

	~BufferedRenderer()
	{
		freeBuffers();
	}

	void setBufferMode(gameswf::render_handler::buffer_mode mode)
	{
		m_buffer_mode = mode;
	}

	void setBufferCapacity(int capacity)
	{
		ensureBufferCapacity(capacity);
	}

	void reset()
	{
		m_vertex_count = 0;
		m_index_count = 0;
		m_flush_offset = 0;
	}

	void setDriver(irr::video::IVideoDriver* driver)
	{
		m_videoDriver = driver;
	}

	void setContext(gameswf::player_context* context)
	{
		m_context = context;
	}

	void setMaterial(SMaterial& mat)
	{
		mat.setFlag(irr::video::EMF_FRONT_FACE_CULLING, false);
		mat.setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
		if(mat != m_material)
			flush();
		m_material = mat;
	}

	void queueBuffer(SS3DVertexComponentArrays& components, int vertexCount, int primCount, irr::scene::E_PRIMITIVE_TYPE pType)
	{
		if(m_vertex_count + vertexCount + 2 >= m_current_primitive_buffer_size || pType != m_primitive_type)
			flush();

		ensureBufferCapacity(m_vertex_count + vertexCount + 2);

		m_primitive_type = pType;

#ifdef MERGE_STRIPS
		if(m_vertex_count > 0 && m_primitive_type == irr::scene::EPT_TRIANGLE_STRIP)
		{
			// degenerate vertex #1
			m_positions[m_vertex_count] = components.Position[0];
			m_texcoords[m_vertex_count] = components.TexCoord[0].Coord[0];
			m_colors[m_vertex_count] = components.Color0[0];
			m_indexes[m_vertex_count] = m_vertex_count - m_flush_offset;
			m_vertex_count++;
			m_index_count++;
		}
#endif // MERGE_STRIPS

		// indexes
		int offset = m_vertex_count;
		for(int i = 0; i < vertexCount; ++i)
		{
			m_indexes[offset] = offset - m_flush_offset;
			offset++;
		}

		// vertices
		memcpy(m_positions + m_vertex_count, components.Position, vertexCount * sizeof(irr::core::vector3df));
		memcpy(m_texcoords + m_vertex_count, components.TexCoord[0].Coord, vertexCount * sizeof(irr::core::vector2df));
		memcpy(m_colors + m_vertex_count, components.Color0, vertexCount * sizeof(irr::video::SColor));
		m_vertex_count += vertexCount;
		m_index_count += vertexCount;

#ifdef MERGE_STRIPS
		if(m_primitive_type == irr::scene::EPT_TRIANGLE_STRIP)
		{
			// degenerate vertex #2
			m_positions[m_vertex_count] = components.Position[vertexCount - 1];
			m_texcoords[m_vertex_count] = components.TexCoord[0].Coord[vertexCount - 1];
			m_colors[m_vertex_count] = components.Color0[vertexCount - 1];
			m_indexes[m_vertex_count] = m_vertex_count - m_flush_offset;
			m_vertex_count++;
			m_index_count++;
		}
#endif // MERGE_STRIPS

#ifndef MERGE_STRIPS
		// always flush if not merging strips
		flush();
#endif // !MERGE_STRIPS
	}

	void queueIndexedTriangles(SS3DVertexComponentArrays& components, int vertexCount, const unsigned short* indexes, int indexCount)
	{
		if(m_vertex_count + vertexCount >= m_current_primitive_buffer_size || m_index_count + indexCount > m_current_primitive_buffer_size || m_primitive_type != irr::scene::EPT_TRIANGLES)
			flush();

		ensureBufferCapacity(gameswf::imax(m_vertex_count + vertexCount, m_index_count + indexCount));
		m_primitive_type = irr::scene::EPT_TRIANGLES;

		// indexes
		int offset = m_vertex_count - m_flush_offset;
		for(int i = 0; i < indexCount; ++i)
		{
			m_indexes[m_index_count++] = indexes[i] + offset;
		}

		// vertices
		memcpy(m_positions + m_vertex_count, components.Position, vertexCount * sizeof(irr::core::vector3df));
		memcpy(m_texcoords + m_vertex_count, components.TexCoord[0].Coord, vertexCount * sizeof(irr::core::vector2df));
		memcpy(m_colors + m_vertex_count, components.Color0, vertexCount * sizeof(irr::video::SColor));
		m_vertex_count += vertexCount;
	}

	void flush()
	{
		if(m_vertex_count > 0)
		{
			// it's important to transfer all texture caches to vram
			gameswf::texture_cache::unlock_all(m_context);

			const int vertexCount = m_vertex_count - m_flush_offset;
			const int primCount = m_primitive_type == irr::scene::EPT_TRIANGLE_STRIP ? m_index_count - 2 : m_index_count/3;
			const unsigned short* indexes = m_indexes + m_flush_offset;
			
			SS3DVertexComponentArrays components;
			components.Position = m_positions + m_flush_offset;
			components.Color0 = m_colors + m_flush_offset;
			components.TexCoord[0].Coord = m_texcoords + m_flush_offset;

			if(m_wire_frame)
			{
				m_videoDriver->setColorMask(true, true, true, true);
				m_material.setMaterialType(EMT_SOLID);
				m_material.setFlag(EMF_ZBUFFER, false);
				m_material.setTexture(0, NULL);
				m_material.setFlag(EMF_WIREFRAME, true);
			}

			m_videoDriver->setMaterial(m_material);

#ifdef RVL_OS
			draw2DVertexPrimitiveList(m_videoDriver, components, vertexCount, indexes, primCount, m_primitive_type);
#elif defined USE_S3DVERTEX
			// dump into S3DVertex container (EVT_STANDARD) if EVT_COMPONENT_ARRAYS not supported
			for(int i = 0; i < vertexCount; i++)
			{
				m_s3dvertices[i].Pos = components.pPosition[i];
				m_s3dvertices[i].Color = components.pColor0[i];
				m_s3dvertices[i].TCoords = components.pTexCoord0[i];
			}
			m_videoDriver->drawVertexPrimitiveList(m_s3dvertices, indexes, 0, vertexCount, primCount, EVT_STANDARD, m_primitive_type, EIT_16BIT);
#else
			m_videoDriver->drawVertexPrimitiveList(&components, indexes, 0, vertexCount, primCount, EVT_COMPONENT_ARRAYS, m_primitive_type, EIT_16BIT);
#endif
		}
		
		if(m_buffer_mode == gameswf::render_handler::BUFFER_MODE_APPEND)
		{
			m_flush_offset = m_vertex_count;
		}
		else
		{
			m_vertex_count = 0;
		}
		m_index_count = 0;
	}

	void ensureBufferCapacity(int size)
	{
		if(m_current_primitive_buffer_size < size)
		{
			freeBuffers();

			m_current_primitive_buffer_size = size;
			m_positions = swfnew irr::core::vector3df[size];
			m_texcoords = swfnew irr::core::vector2df[size];
			m_colors = swfnew irr::video::SColor[size];
			m_indexes = swfnew unsigned short[size];

#ifdef USE_S3DVERTEX
			m_s3dvertices = swfnew S3DVertex[size];
#endif
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
		
		if(m_indexes)
			swfdelete_array (m_indexes, m_current_primitive_buffer_size);
		m_indexes = NULL;

		if(m_s3dvertices)
			swfdelete_array (m_s3dvertices, m_current_primitive_buffer_size);
		m_s3dvertices = NULL;
	}
};

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
static const E_TEXTURE_FILTER_TYPE s_filter_map[] = {	
	irr::video::ETFT_NEAREST,
	irr::video::ETFT_LINEAR,
	irr::video::ETFT_NEAREST_MIPMAP_NEAREST,
	irr::video::ETFT_LINEAR_MIPMAP_NEAREST,
	irr::video::ETFT_NEAREST_MIPMAP_LINEAR,
	irr::video::ETFT_LINEAR_MIPMAP_LINEAR
};

struct bitmap_info_ogl : public gameswf::bitmap_info
{
	bool			m_added_texture;
	ITexture*		m_texture;
	IImage*			m_suspended_image;
	const gameswf::membuf* m_data;

	int m_width;
	int m_height;
	IVideoDriver*	m_videoDriver;
	filter_mode		m_min_filter_mode;
	filter_mode		m_mag_filter_mode;

	bitmap_info_ogl(IVideoDriver* videoDriver);
	bitmap_info_ogl(IVideoDriver* videoDriver, int width, int height, gameswf::Uint8* data);
	bitmap_info_ogl(IVideoDriver* videoDriver, gameswf::image_rgb* im);
	bitmap_info_ogl(IVideoDriver* videoDriver, gameswf::image_rgba* im);
	bitmap_info_ogl(IVideoDriver* videoDriver, irr::video::ITexture* texture);
	bitmap_info_ogl(IVideoDriver* videoDriver, int width, int height, const gameswf::membuf* data);
	bitmap_info_ogl(IVideoDriver* videoDriver, int width, int height);

	virtual void layout();

	virtual void unlayout()
	{
		release();
	}

	virtual void activate()
	{
	}

	virtual void* lock()
	{
		layout();
		return m_texture->lock();
	}
	
	virtual void unlock()
	{
		m_texture->unlock();
	}

	

	virtual void set_min_filter(filter_mode mode) 
	{
		m_min_filter_mode = mode;
		if(m_texture != NULL)
		{
			m_texture->setMinFilter(s_filter_map[m_min_filter_mode]);
		}
	}
	
	virtual void set_mag_filter(filter_mode mode) 
	{
		m_mag_filter_mode = mode;
		if(m_texture != NULL)
		{
			m_texture->setMagFilter(s_filter_map[m_mag_filter_mode]);
		}
	}

	virtual void set_texture(irr::video::ITexture* texture)
	{
		assert(texture != NULL);
		assert(m_texture == NULL);
		release();

		m_texture = texture;
		m_texture->grab();

		m_width = m_texture->getSize().Width;
		m_height = m_texture->getSize().Height;
	}

	// misc
	int p2(int n);

	void release()
	{
		if(m_texture)
		{
			// since addTexture was called, need to call removeTexture
			if(m_added_texture)
				m_videoDriver->removeTexture(m_texture);
			// otherwize, drop refcount on texture
			else
				m_texture->drop();
			m_texture = NULL;
		}
		if(m_suspended_image)
		{
			m_suspended_image->drop();
			m_suspended_image = NULL;
		}
		if(m_data)
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
	struct TransformState
	{
		irr::core::matrix4 projection;
		irr::core::matrix4 view;
		irr::core::matrix4 world;
	};

	// Some renderer state.
	gameswf::player_context* m_context;
	gameswf::render_cache* m_render_cache;
	IVideoDriver* m_videoDriver;
	TransformState m_oldTransforms;
	TransformState m_currentTransforms;
	BufferedRenderer m_bufferedRenderer;

#ifdef PLATFORM_MATERIAL_RENDERER
	MaterialRenderer_GameSWF* m_materialRenderer;
	int m_materialRendererID;
#endif

	// Enable/disable antialiasing.
	bool	m_enable_antialias;
	bool	m_enable_batching;
	
	// Output size.
	float	m_display_width;
	float	m_display_height;
	
	gameswf::matrix	m_current_matrix;
	gameswf::cxform	m_current_cxform;

	int m_mask_level;	// nested mask level
	float m_pos_z; // pos z used for mask

#ifdef ENABLE_MASK_INTERSECTION
	gameswf::array<gameswf::array<irr::core::vector2df> > m_mask_stack;
	gameswf::array<irr::core::vector2df> m_mask_temp;
	gameswf::triangle_intersector m_triangle_intersector;
#endif // ENABLE_MASK_INTERSECTION

	// vertex primitive buffer
	int m_current_primitive_buffer_size;
	irr::core::vector3df* m_positions;
	irr::core::vector2df* m_texcoords;
	irr::video::SColor* m_colors;
	unsigned short* m_indexes;

	render_handler_irrlicht(IVideoDriver* videoDriver) :
		m_context(NULL),
		m_render_cache(NULL),
		m_videoDriver(videoDriver),
		m_enable_antialias(false),
		m_enable_batching(false),
		m_display_width(0),
		m_display_height(0),
		m_mask_level(0),
		m_pos_z(MASK_POS_Z_INIT),
		m_current_primitive_buffer_size(0),
		m_positions(NULL),
		m_texcoords(NULL),
		m_colors(NULL),
		m_indexes(NULL)
	{
		if(m_videoDriver)
			m_videoDriver->grab();

		ensureBufferCapacity(256);
		m_bufferedRenderer.setDriver(m_videoDriver);

#ifdef PLATFORM_MATERIAL_RENDERER
		const char* const k_materialRendererName = "MaterialRenderer_GameSWF";
		irr::u32 rendererCount = m_videoDriver->getMaterialRendererCount();
		m_materialRenderer = 0;
		for(irr::u32 i = 0; i < rendererCount; ++i)
		{
			if (strcmp(m_videoDriver->getMaterialRendererName(i), k_materialRendererName) == 0)
			{
				m_materialRenderer = static_cast<MaterialRenderer_GameSWF*>(m_videoDriver->getMaterialRenderer(i));
				m_materialRenderer->grab();
				m_materialRendererID = i;
			}
		}

		if (!m_materialRenderer)
		{
			m_materialRenderer = irrnew MaterialRenderer_GameSWF((PLATFORM_DRIVER*)m_videoDriver);
			m_materialRendererID = m_videoDriver->addMaterialRenderer(m_materialRenderer, k_materialRendererName);
		}
#endif
	}

	~render_handler_irrlicht()
	{
		freeBuffers();

#ifdef PLATFORM_MATERIAL_RENDERER
		// maybe add IVideoDriver::removeMaterialRenderer
		if(m_materialRenderer)
		{
			m_materialRenderer->drop();
			m_materialRenderer = NULL;
		}
#endif

		s_material.setTexture(0, NULL);

		if(m_videoDriver)
		{
			m_videoDriver->drop();
			m_videoDriver = 0;
		}
	}

	void ensureBufferCapacity(int size)
	{
		if(m_current_primitive_buffer_size < size)
		{
			freeBuffers();

			m_current_primitive_buffer_size = size;
			m_positions = swfnew irr::core::vector3df[size];
			m_texcoords = swfnew irr::core::vector2df[size];
			m_colors = swfnew irr::video::SColor[size];
			m_indexes = swfnew unsigned short[size];
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

		if(m_indexes)
			swfdelete_array (m_indexes, m_current_primitive_buffer_size);
		m_indexes = NULL;
	}

	void open()
	{
		// Scan for extensions used by gameswf
		//glActiveTexture =  (PFNGLACTIVETEXTUREPROC) SDL_GL_GetProcAddress("glActiveTexture");
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
		SMaterial material;
		m_videoDriver->setMaterial(material);
	}

	virtual gameswf::orientation_mode get_orientation()
	{
		return (gameswf::orientation_mode)m_videoDriver->getOrthoOrientation();
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

		void	apply(IVideoDriver* videoDriver, irr::core::vector3df* positions, irr::core::vector2df* texcoords, irr::video::SColor* colors, int vertex_count) const
		// Push our style into OpenGL.
		{
			assert(m_mode != INVALID);
			bitmap_info_ogl* bitmap_info = (bitmap_info_ogl*)m_bitmap_info;

			SMaterial& material = s_material;
			if (m_mode == COLOR)
			{
				material.setTexture(0, NULL);
			}
			else if (m_mode == BITMAP_WRAP || m_mode == BITMAP_CLAMP)
			{
				bitmap_info->layout();

				material.setAmbientColor(irr::video::SColor(255, 255, 255, 255));
				material.setTexture(0, bitmap_info->m_texture);
				if (bitmap_info->m_texture)
				{
					bitmap_info->m_texture->setWrap(m_mode == BITMAP_WRAP ? ETC_REPEAT : ETC_CLAMP_TO_EDGE);
				}

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

				for(int i = 0; i < vertex_count; i++)
				{
					float x0 = positions[i].X;
					float y0 = positions[i].Y;
					texcoords[i].X = (x0 * pu[0]) + (y0 * pu[1]) + pu[3];
					texcoords[i].Y = (x0 * pv[0]) + (y0 * pv[1]) + pv[3];

#ifdef FLIP_TCOORD_Y
					texcoords[i].Y = 1.0f - texcoords[i].Y;
#endif
				}

				/*irr::video::SColor color(m_bitmap_color_transform.m_[0][0] * 255,
							  m_bitmap_color_transform.m_[1][0] * 255,
							  m_bitmap_color_transform.m_[2][0] * 255,
							  m_bitmap_color_transform.m_[3][0] * 255
							  );*/
			}

			irr::video::SColor color(m_color.m_a, m_color.m_r, m_color.m_g, m_color.m_b);
			for(int i = 0; i < vertex_count; i++)
				colors[i] = color;
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

		void	apply_second_pass(IVideoDriver* videoDriver) const
		// Set OpenGL state for a necessary second pass.
		{
			assert(needs_second_pass());

			// The additive color also seems to be modulated by the texture. So,
			// maybe we can fake this in one pass using using the mean value of 
			// the colors: c0*t+c1*t = ((c0+c1)/2) * t*2
			// I don't know what the alpha component of the color is for.
			//glDisable(GL_TEXTURE_2D);

			//videoDriver->setMaterial(
		}

		void	cleanup_second_pass(IVideoDriver* videoDriver) const
		{
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
		return swfnew bitmap_info_ogl(m_videoDriver, im);
	}


	gameswf::bitmap_info*	create_bitmap_info_rgba(gameswf::image_rgba* im)
	// Given an image, returns a pointer to a bitmap_info struct
	// that can later be passed to fill_style_bitmap(), to set a
	// bitmap fill style.
	//
	// This version takes an image with an alpha channel.
	{
		return swfnew bitmap_info_ogl(m_videoDriver, im);
	}

	gameswf::bitmap_info*	create_bitmap_info_texture(irr::video::ITexture* texture)
	// Given an image, returns a pointer to a bitmap_info struct
	// that can later be passed to fill_style_bitmap(), to set a
	// bitmap fill style.
	//
	// This version takes an image with an alpha channel.
	{
		return swfnew bitmap_info_ogl(m_videoDriver, texture);
	}

	gameswf::bitmap_info*	create_bitmap_info_native(int w, int h, const gameswf::membuf* data)
	{
		return swfnew bitmap_info_ogl(m_videoDriver, w, h, data);
	}

	gameswf::bitmap_info*	create_bitmap_info_target(int w, int h)
	{
		return swfnew bitmap_info_ogl(m_videoDriver, w, h);
	}

	gameswf::bitmap_info*	create_bitmap_info_empty()
	// Create a placeholder bitmap_info.  Used when
	// DO_NOT_LOAD_BITMAPS is set; then later on the host program
	// can use movie_definition::get_bitmap_info_count() and
	// movie_definition::get_bitmap_info() to stuff precomputed
	// textures into these bitmap infos.
	{
		return swfnew bitmap_info_ogl(m_videoDriver);
	}

	gameswf::bitmap_info*	create_bitmap_info_alpha(int w, int h, gameswf::Uint8* data)
	// Create a bitmap_info so that it contains an alpha texture
	// with the given data (1 byte per texel).
	//
	// Munges *data (in order to make mipmaps)!!
	{
		return swfnew bitmap_info_ogl(m_videoDriver, w, h, data);
	}

	gameswf::video_handler*	create_video_handler()
	{
		return 0;
		//return swfnew video_ogl();
	}

	void setOrtho(float left, float right, float bottom, float top, float znear, float zfar, irr::core::matrix4& m)
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

#define OVERSIZE	1.0f

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

		// Backup old matrices
		m_oldTransforms.projection = m_videoDriver->getTransform(ETS_PROJECTION);
		m_oldTransforms.view = m_videoDriver->getTransform(ETS_VIEW);
		m_oldTransforms.world = m_videoDriver->getTransform(ETS_WORLD);
		m_enable_batching = m_videoDriver->getOption(EVDO_BATCHING);

		irr::core::rect<irr::s32> area(viewport_x0, viewport_y0, viewport_x0 + viewport_width, viewport_y0 + viewport_height);
		m_videoDriver->setViewPort(area);

		setColorMask(true);
		
		// Reset renderer
		m_bufferedRenderer.reset();

		// disable batching
		m_videoDriver->setOption(EVDO_BATCHING, false);

		// init stuff for mask using depth buffer
		m_videoDriver->clearZBuffer();
		m_mask_level = 0;
		m_pos_z = MASK_POS_Z_INIT;

#ifdef ENABLE_MASK_INTERSECTION
		for(int i = 0; i < m_mask_stack.size(); i++)
			m_mask_stack[i].clear();
#endif // ENABLE_MASK_INTERSECTION

		// init default irrlicht material flags
		SMaterial& material = s_material;
		material.setMaterialType(EMT_TRANSPARENT_TEXTURE_VERTEX_ALPHA);
		material.setFlag(EMF_ZBUFFER, false);
		material.setFlag(EMF_ZWRITE_ENABLE, false);
		material.setFlag(EMF_LIGHTING, false);
		material.setFlag(EMF_FRONT_FACE_CULLING, false);
		material.setFlag(EMF_BACK_FACE_CULLING, false);
		//material.setFlag(EMF_WIREFRAME, true);

		//-----------------------
		// Irrlicht Projection
		//-----------------------
		m_currentTransforms.projection = irr::core::IdentityMatrix;
		m_currentTransforms.projection.buildProjectionMatrixOrtho(OVERSIZE * 2, -OVERSIZE * 2, -1, 1);
		// Screen orientation (iphone)
		irr::core::matrix4 r;
		switch(m_videoDriver->getOrthoOrientation())
		{
			case EOO_0:
				break;
			case EOO_180:
				r.setRotationDegrees(irr::core::vector3df(0, 0, 180));
				m_currentTransforms.projection *= r;
				break;
			case EOO_90:
				r.setRotationDegrees(irr::core::vector3df(0, 0, -90));
				m_currentTransforms.projection *= r;
				break;
			case EOO_270:
				r.setRotationDegrees(irr::core::vector3df(0, 0, 90));
				m_currentTransforms.projection *= r;
				break;
		}
		m_videoDriver->setTransform(ETS_PROJECTION, m_currentTransforms.projection);

		//-----------------------
		// Irrlicht Model View
		//-----------------------
		m_currentTransforms.view = irr::core::IdentityMatrix;
		setOrtho(x0, x1, y0, y1, -1, 1, m_currentTransforms.view);
		m_videoDriver->setTransform(ETS_VIEW, m_currentTransforms.view);

		//-----------------------
		// Irrlicht World View
		//-----------------------
		m_currentTransforms.world = irr::core::IdentityMatrix; 
		m_videoDriver->setTransform(ETS_WORLD, m_currentTransforms.world);

		//-----------------------
		// Irrlicht Texture
		//-----------------------
//#ifdef IPHONEOS
//		Not working...so enable FLIP_TCOORD_Y
//		irr::core::matrix4 texture;
//		texture.buildTextureTransform(0.0f, irr::core::vector2df(0,0), irr::core::vector2df(1.0f,1.0f), irr::core::vector2df(-1.0f,-1.0f));
//		m_videoDriver->setTransform(ETS_TEXTURE_0, texture);
//#endif

		// Clear the background, if background color has alpha > 0.
		if (background_color.m_a > 0)
		{
			// Draw a big quad.
			apply_color(background_color);
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

		m_videoDriver->setTransform(ETS_PROJECTION, m_oldTransforms.projection);
		m_videoDriver->setTransform(ETS_VIEW, m_oldTransforms.view);
		m_videoDriver->setTransform(ETS_WORLD, m_oldTransforms.world);

		// reset batching to initial value
		m_videoDriver->setOption(EVDO_BATCHING, m_enable_batching);

		//render_glyph_texture_cache();
	}

	void	set_blend_mode(gameswf::blend_mode::id mode)
	{
#ifdef PLATFORM_MATERIAL_RENDERER
		SMaterial& material = s_material;

		// ignore blend mode when in mask mode
		if(material.getMaterialType() == EMT_SOLID)
			return;

		switch(mode)
		{
		case gameswf::blend_mode::MULTIPLY:
		case gameswf::blend_mode::SCREEN:
		case gameswf::blend_mode::OVERLAY:
			material.setMaterialType((irr::video::E_MATERIAL_TYPE)m_materialRendererID);
			material.setMaterialTypeParam(0, (float)mode);
			break;

		case gameswf::blend_mode::NORMAL:
		case gameswf::blend_mode::NORMAL1:
		default:
			material.setMaterialType(EMT_TRANSPARENT_TEXTURE_VERTEX_ALPHA);
			break;
		}
#endif // PLATFORM_MATERIAL_RENDERER
	}

	void	set_target(gameswf::bitmap_info* target)
	{
		bitmap_info_ogl* bi = (bitmap_info_ogl*)target;
		if(bi)
		{
			m_videoDriver->setRenderTarget(bi->m_texture);
		}
		else
		{
			m_videoDriver->setRenderTarget(NULL);
		}
	}

	void	clear_frame_buffer()
	{
#ifdef RVL_OS
		
#elif defined _IRR_COMPILE_WITH_DIRECT3D_8_ || defined _IRR_COMPILE_WITH_DIRECT3D_9_
		
#else
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
	}

	void	read_frame_buffer(int x, int y, int width, int height, unsigned char* pixels)
	{
#ifdef RVL_OS
		
#elif defined _IRR_COMPILE_WITH_DIRECT3D_8_ || defined _IRR_COMPILE_WITH_DIRECT3D_9_
		
#else
		glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
#endif
	}

	void set_wire_frame(bool enabled)
	{
		m_bufferedRenderer.m_wire_frame = enabled;
	}

	void set_buffer_capacity(gameswf::render_handler::buffer_mode mode, int capacity)
	{
		m_bufferedRenderer.setBufferMode(mode);
		m_bufferedRenderer.setBufferCapacity(capacity);
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

	void render_texture(bitmap_info_ogl* bi)
	{
		if(bi->m_texture == NULL)
			return;

#ifdef FLIP_TCOORD_Y
		irr::core::rect<irr::s32> src(0,bi->m_texture->getSize().Height,bi->m_texture->getSize().Width, 0);
#else
		irr::core::rect<irr::s32> src(0,0,bi->m_texture->getSize().Width, bi->m_texture->getSize().Height);
#endif
		irr::core::rect<irr::s32> dst(200,0,200 + bi->m_texture->getSize().Width * 2, bi->m_texture->getSize().Height * 2);
		m_videoDriver->beginScene2D();
		m_videoDriver->draw2DImage(bi->m_texture, dst, src, NULL, NULL, true);
		m_videoDriver->endScene2D();
	}

	void render_glyph_texture_cache()
	{
		if(m_context->m_glyph_provider->get_texture_cache() != NULL)
		{
			render_texture((bitmap_info_ogl*)m_context->m_glyph_provider->get_texture_cache()->get_bitmap_info());
		}
	}

	void render_bitmap_glyph_texture_cache()
	{
		if(m_context->m_bitmap_glyph_provider->get_texture_cache() != NULL)
		{
			render_texture((bitmap_info_ogl*)m_context->m_bitmap_glyph_provider->get_texture_cache()->get_bitmap_info());
		}
	}

	void render_filter_engine_texture_cache()
	{
		if(gameswf::filter_engine::get_instance() != NULL)
		{
			render_texture((bitmap_info_ogl*)gameswf::filter_engine::get_instance()->get_filter_texture_cache().get_bitmap_info());
		}
	}

	// called before user callback to backup/prepare states
	virtual void begin_display_callback()
	{
		m_bufferedRenderer.flush();
	}

	// called after user callback to restore states
	virtual void end_display_callback()
	{
		m_videoDriver->setTransform(ETS_PROJECTION, m_currentTransforms.projection);
		m_videoDriver->setTransform(ETS_VIEW, m_currentTransforms.view);
		m_videoDriver->setTransform(ETS_WORLD, m_currentTransforms.world);
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
	
	irr::core::matrix4	get_matrix(const gameswf::matrix& m)
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

		irr::core::matrix4 transform;
		transform.setM(mat);
		return transform;
	}

	static void	apply_color(const gameswf::rgba& c)
	// Set the given color.
	{
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

		irr::core::vector3df* pos = m_positions;

		gameswf::coord_component* in = (gameswf::coord_component*)coords;
		for(int i = 0; i < vertex_count; i++)
		{
			pos->X = (float)*(in++);
			pos->Y = (float)*(in++);
			pos->Z = m_pos_z;
			pos++;
		}

		SS3DVertexComponentArrays components;
		components.Position = m_positions;
		components.TexCoord[0].Coord = m_texcoords;
		components.Color0 = m_colors;

		// Set up current style.
		m_current_styles[LEFT_STYLE].apply(m_videoDriver, m_positions, m_texcoords, m_colors, vertex_count);
		m_bufferedRenderer.setMaterial(s_material);

		// convert to world pos
		irr::core::matrix4 mat = get_matrix(m_current_matrix);
		for(int i = 0; i < vertex_count; i++)
			mat.transformVect(m_positions[i]);
		
#ifdef ENABLE_MASK_INTERSECTION
		if(process_mask_intersection(m_positions, vertex_count, NULL, 0, (irr::scene::E_PRIMITIVE_TYPE)primitive_type) == false)
#endif
		{
			// Send the tris to Irrlicht
			if(indexes == NULL || index_count == 0)
				m_bufferedRenderer.queueBuffer(components, vertex_count, vertex_count, (irr::scene::E_PRIMITIVE_TYPE)primitive_type);
			else
				m_bufferedRenderer.queueIndexedTriangles(components, vertex_count, indexes, index_count);
		}

		//if (m_current_styles[LEFT_STYLE].needs_second_pass())
		//{
		//	m_current_styles[LEFT_STYLE].apply_second_pass(m_videoDriver);
		//	m_videoDriver->drawVertexPrimitiveList(m_mesh_primitive_buffer, vertex_count, m_indexes, vertex_count, EVT_STANDARD, (irr::scene::E_PRIMITIVE_TYPE)primitive_type, EIT_16BIT);
		//	m_current_styles[LEFT_STYLE].cleanup_second_pass(m_videoDriver);
		//}

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
			glVertexPointer(2, GL_SHORT, sizeof(Sint16) * 2, coords);
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
		draw_mesh_primitive(irr::scene::EPT_TRIANGLE_STRIP, coords, vertex_count, NULL, 0);
	}
			
	void	draw_triangle_list(const void* coords, int vertex_count, const unsigned short* indexes, int index_count)
	{
		draw_mesh_primitive(irr::scene::EPT_TRIANGLES, coords, vertex_count, indexes, index_count);
	}


	void	draw_line_strip(const void* coords, int vertex_count)
	// Draw the line strip formed by the sequence of points.
	{
		// drawing a line strip breaks the buffered pipeline
		m_bufferedRenderer.flush();

		// apply line width
		float scale = fabsf(m_current_matrix.get_x_scale()) + fabsf(m_current_matrix.get_y_scale());
		float w = m_current_styles[LINE_STYLE].m_width * scale / 2.0f;
		w = TWIPS_TO_PIXELS(w);
		s_material.setThickness(w <= 1.0f ? 1.0f : w);
		s_material.setTexture(0, NULL);
		m_videoDriver->setMaterial(s_material);

		gameswf::coord_component* in = (gameswf::coord_component*)coords;

		irr::video::SColor color(m_current_styles[LINE_STYLE].m_color.m_a, m_current_styles[LINE_STYLE].m_color.m_r, m_current_styles[LINE_STYLE].m_color.m_g, m_current_styles[LINE_STYLE].m_color.m_b);

		ensureBufferCapacity(vertex_count);

		irr::core::matrix4 mat = get_matrix(m_current_matrix);
		for(int i = 0; i < vertex_count; i++)
		{
			m_positions[i].X = *(in++);
			m_positions[i].Y = *(in++);
			m_positions[i].Z = m_pos_z;
			mat.transformVect(m_positions[i]);
			m_colors[i] = color;
			m_indexes[i] = i;

			//if(i > 0)
			//	m_videoDriver->draw3DLine(m_positions[i-1], m_positions[i], color);
		}

		SS3DVertexComponentArrays components;
		components.Position = m_positions;
		components.Color0 = m_colors;
		m_videoDriver->drawVertexPrimitiveList(&components, m_indexes, 0, vertex_count, vertex_count - 1, EVT_COMPONENT_ARRAYS, irr::scene::EPT_LINE_STRIP, EIT_16BIT);

#ifdef RVL_OS
		GXDrawDone();
#endif
	}

	void draw(gameswf::render_cache* cache)
	{
		gameswf::render_cache& rco = *cache;
		SS3DVertexComponentArrays components;
		// check if in mask mode
		bool need_z_update = m_mask_level > 0;
		for(int i = 0; i < rco.m_dlist.size(); i++)
		{
			gameswf::render_cache::entry& entry = rco.m_dlist[i];
			
			// Set up current texture.
			s_material.setTexture(0, ((bitmap_info_ogl*)entry.bi)->m_texture);
			m_bufferedRenderer.setMaterial(s_material);

			if(need_z_update)
			{
				for(int k = 0; k < entry.vertex_count; k++)
					rco.m_coords[entry.vertex_start + k].z = m_pos_z;
			}

			// colors
			ensureBufferCapacity(entry.vertex_count);
			for(int k = 0; k < entry.vertex_count; k++)
				*(m_colors[k].getDataPtr()) = entry.color;

			components.Position = (irr::core::vector3df*)&cache->m_coords[entry.vertex_start];
			components.TexCoord[0].Coord = (irr::core::vector2df*)&cache->m_uv_coords[entry.vertex_start];
			components.Color0 = m_colors;

			unsigned short* indexes = &cache->m_indexes[entry.index_start];

#ifdef ENABLE_MASK_INTERSECTION
			if(process_mask_intersection(components.Position, entry.vertex_count, indexes, entry.index_count, irr::scene::EPT_TRIANGLES) == false)
#endif
			{
				// Send the tris to Irrlicht
				m_bufferedRenderer.queueIndexedTriangles(components, entry.vertex_count, indexes, entry.index_count);
			}
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

		if(color.m_a == 0.0f)
			return;

		apply_color(color);

		gameswf::point a, b, c, d;
		m.transform(&a, gameswf::point(coords.m_x_min, coords.m_y_min));
		m.transform(&b, gameswf::point(coords.m_x_max, coords.m_y_min));
		m.transform(&c, gameswf::point(coords.m_x_min, coords.m_y_max));
		d.m_x = b.m_x + c.m_x - a.m_x;
		d.m_y = b.m_y + c.m_y - a.m_y;

		bi->layout();
//		glBindTexture(GL_TEXTURE_2D, bi->m_texture_id);
//		glEnable(GL_TEXTURE_2D);

		bitmap_info_ogl* bitmap_info = (bitmap_info_ogl*)bi;

		//m_bufferedRenderer.flush();

		SMaterial& material = s_material;
		material.setTexture(0, bitmap_info->m_texture);
		if (bitmap_info->m_texture)
		{
			bitmap_info->m_texture->setWrap(ETC_CLAMP);
		}
		//m_videoDriver->setMaterial(material);
		m_bufferedRenderer.setMaterial(material);

		SS3DVertexComponentArrays components;
		components.Position = m_positions;
		components.TexCoord[0].Coord = m_texcoords;
		components.Color0 = m_colors;
		const int index = 0;

		m_positions[index + 0] = irr::core::vector3df(a.m_x, a.m_y, m_pos_z);
		m_positions[index + 1] = irr::core::vector3df(b.m_x, b.m_y, m_pos_z);
		m_positions[index + 2] = irr::core::vector3df(c.m_x, c.m_y, m_pos_z);
		m_positions[index + 3] = irr::core::vector3df(d.m_x, d.m_y, m_pos_z);
				
		m_texcoords[index + 0] = irr::core::vector2df(uv_coords.m_x_min, uv_coords.m_y_min);
		m_texcoords[index + 1] = irr::core::vector2df(uv_coords.m_x_max, uv_coords.m_y_min);
		m_texcoords[index + 2] = irr::core::vector2df(uv_coords.m_x_min, uv_coords.m_y_max);
		m_texcoords[index + 3] = irr::core::vector2df(uv_coords.m_x_max, uv_coords.m_y_max);
		
		irr::video::SColor scolor(color.m_a, color.m_r, color.m_g, color.m_b);
		for(int i = 0; i < 4; ++i)
		{
			m_colors[index + i] = scolor;
			
			// align to pixel and offset
			if(m_text_pixel_alignment)
			{
				m_positions[index + i].X = (float)((((int)m_positions[index + i].X + 10) / 20) * 20);// + (0.375f * 20.0f);
				m_positions[index + i].Y = (float)((((int)m_positions[index + i].Y + 10) / 20) * 20);// + (0.375f * 20.0f);
			}

#ifdef FLIP_TCOORD_Y
			m_texcoords[index + i].Y = 1.0f - m_texcoords[index + i].Y;
#endif
		}
		
		unsigned short indexes[6] = {0, 1, 2, 1, 2, 3};

#ifdef ENABLE_MASK_INTERSECTION
		if(process_mask_intersection(m_positions, 4, indexes, 6, irr::scene::EPT_TRIANGLES) == false)
#endif
		{
			m_bufferedRenderer.queueIndexedTriangles(components, 4, indexes, 6);
		}

		if(m_render_cache)
		{
			m_render_cache->record(bi, (gameswf::vector3df*)m_positions, (gameswf::vector2df*)m_texcoords, *(scolor.getDataPtr()), 4, indexes, 6);
		}
	}

#ifdef ENABLE_MASK_INTERSECTION
	bool process_mask_intersection(irr::core::vector3df* positions, int vertex_count, const unsigned short* indexes, int index_count, irr::scene::E_PRIMITIVE_TYPE primitive_type)
	{
		if(m_mask_level >= 1 && s_material.getMaterialType() == EMT_SOLID && vertex_count >= 3)
		{
			gameswf::array<irr::core::vector2df>& src = m_mask_temp;
			src.clear();

			int triCount = 0;
			if(primitive_type == irr::scene::EPT_TRIANGLE_STRIP)
			{
				triCount = vertex_count - 2;
				
				// convert from strips to tris
				src.push_back(irr::core::vector2df(positions[0].X, positions[0].Y));
				src.push_back(irr::core::vector2df(positions[1].X, positions[1].Y));
				src.push_back(irr::core::vector2df(positions[2].X, positions[2].Y));

				for(int i = 3; i < vertex_count; i++)
				{
					src.push_back(irr::core::vector2df(positions[i-2].X, positions[i-2].Y));
					src.push_back(irr::core::vector2df(positions[i-1].X, positions[i-1].Y));
					src.push_back(irr::core::vector2df(positions[i].X, positions[i].Y));
				}
			}
			else if(indexes != NULL)
			{
				triCount = index_count/3;

				// copy tris
				for(int i = 0; i < index_count; i++)
					src.push_back(irr::core::vector2df(positions[indexes[i]].X, positions[indexes[i]].Y));
			}
			else
			{
				triCount = vertex_count/3;

				// copy tris
				for(int i = 0; i < vertex_count; i++)
					src.push_back(irr::core::vector2df(positions[i].X, positions[i].Y));
			}

			gameswf::array<irr::core::vector2df>& dst = m_mask_stack[m_mask_level - 1];

			if(m_mask_level >= 2)
			{
				// perform intersection of 2 masks
				for(int i = 0; i < triCount; i++)
				{
					gameswf::point* t1 = (gameswf::point*)&src[i * 3];
					gameswf::array<gameswf::point> result_tristrip;

					gameswf::array<irr::core::vector2df>& previousMask = m_mask_stack[m_mask_level - 2];
					int clipTriangleCount = previousMask.size() /  3;
					for(int j = 0; j < clipTriangleCount; j++)
					{
						gameswf::point* t2 = (gameswf::point*)&previousMask[j * 3];
						m_triangle_intersector.process(t1, t2, result_tristrip);

						if(result_tristrip.size() > 0)
						{
							dst.push_back((irr::core::vector2df&)result_tristrip[0]);
							dst.push_back((irr::core::vector2df&)result_tristrip[1]);
							dst.push_back((irr::core::vector2df&)result_tristrip[2]);

							for(int i = 3; i < result_tristrip.size(); i++)
							{
								dst.push_back((irr::core::vector2df&)result_tristrip[i-2]);
								dst.push_back((irr::core::vector2df&)result_tristrip[i-1]);
								dst.push_back((irr::core::vector2df&)result_tristrip[i]);
							}
						}
					}
				}

				return true;
			}
			else
			{
				dst.append(src);
			}
		}

		return false;
	}

	void render_mask_intersection()
	{
		// Set up current style.
		s_material.setTexture(0, NULL);
		m_bufferedRenderer.setMaterial(s_material);

		// send the result mask to engine in batch of 256 max
		gameswf::array<irr::core::vector2df>& mask2d = m_mask_stack[m_mask_level - 1];
		int maskLimit = mask2d.size();
		int maskSize = 0;
		irr::core::vector3df mask3d[256];
		irr::core::vector2df texcoords[256];
		irr::video::SColor colors[256];

		for(int i = 0; i < maskLimit; i++)
		{
			colors[maskSize] = irr::video::SColor(255, 255, 255, 255);
			mask3d[maskSize++] = irr::core::vector3df(mask2d[i].X, mask2d[i].Y, m_pos_z);

			// Send the tris to Irrlicht
			if(maskSize >= 256 || maskSize == maskLimit)
			{
				SS3DVertexComponentArrays components;
				components.Position = mask3d;
				components.TexCoord[0].Coord = texcoords;
				components.Color0 = colors;
				m_bufferedRenderer.queueBuffer(components, maskSize, maskSize / 3, irr::scene::EPT_TRIANGLES);
				maskSize = 0;
			}
		}
		m_bufferedRenderer.flush();
	}
#endif // ENABLE_MASK_INTERSECTION
	
	void begin_submit_mask()
	{
		// using mask breaks the buffered pipeline
		m_bufferedRenderer.flush();

		m_mask_level++;
		m_pos_z += MASK_POS_Z_STEP;

#ifdef ENABLE_MASK_INTERSECTION
		if(m_mask_stack.size() < m_mask_level)
		{
			gameswf::array<irr::core::vector2df> temp;
			m_mask_stack.push_back(temp);
		}
#endif

		// enable depth buffer write
		s_material.setFlag(EMF_ZBUFFER, true);
		s_material.setZBufferFunc(ECFN_LEQUAL);
		s_material.setFlag(EMF_ZWRITE_ENABLE, true);

		// set material to opaque solid
		s_material.setMaterialType(EMT_SOLID);

		// disable color write
		setColorMask(false);
	}

	// called after begin_submit_mask and the drawing of mask polygons
	void end_submit_mask()
	{
#ifdef ENABLE_MASK_INTERSECTION
		if(m_mask_level >= 2)
		{
			render_mask_intersection();
		}
#endif // ENABLE_MASK_INTERSECTION

		// using mask breaks the buffered pipeline
		m_bufferedRenderer.flush();

		// enable depth buffer compare "equal" with mask
		s_material.setFlag(EMF_ZBUFFER, true);
		s_material.setZBufferFunc(ECFN_EQUAL);
		s_material.setFlag(EMF_ZWRITE_ENABLE, false);

		// set material to normal
		s_material.setMaterialType(EMT_TRANSPARENT_TEXTURE_VERTEX_ALPHA);

		// enable color write
		setColorMask(true);
	}

	void disable_mask()
	{	
		// using mask breaks the buffered pipeline
		m_bufferedRenderer.flush();

		assert(m_mask_level > 0);

		m_mask_level--;

		// disable depth buffer
		s_material.setFlag(EMF_ZBUFFER, false);
		s_material.setFlag(EMF_ZWRITE_ENABLE, false);
		setColorMask(true);

		// begin submit previous mask
#ifdef ENABLE_MASK_INTERSECTION
		m_mask_stack[m_mask_level].clear();

		if(m_mask_level > 0)
		{
			// enable depth buffer write
			s_material.setFlag(EMF_ZBUFFER, true);
			s_material.setZBufferFunc(ECFN_LEQUAL);
			s_material.setFlag(EMF_ZWRITE_ENABLE, true);

			// set material to opaque solid
			s_material.setMaterialType(EMT_SOLID);

			// disable color write
			setColorMask(false);
			render_mask_intersection();

			// enable depth buffer compare "equal" with mask
			// set material to normal
			s_material.setMaterialType(EMT_TRANSPARENT_TEXTURE_VERTEX_ALPHA);
			s_material.setZBufferFunc(ECFN_EQUAL);
			s_material.setFlag(EMF_ZWRITE_ENABLE, false);
			setColorMask(true);
		}
#endif // ENABLE_MASK_INTERSECTION
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

	void setColorMask(bool enable)
	{
		m_videoDriver->setColorMask(enable, enable, enable, enable);
	}

};	// end struct render_handler_ogl


// bitmap_info_ogl implementation


#if (RESAMPLE_METHOD == 1)

void	hardware_resample(int bytes_per_pixel, int src_width, int src_height, uint8* src_data, int dst_width, int dst_height)
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
			image_make_next_miplevel((image_rgb*) im);
		}
		else
		{
			image_make_next_miplevel((image_rgba*) im);
		}

//		glTexImage2D(GL_TEXTURE_2D, level, internal_format, im->m_width, im->m_height, 0,
//			     input_format, GL_UNSIGNED_BYTE, im->m_data);
		create_texture(input_format, im->m_width, im->m_height, im->m_data, level);
		level++;
	}
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

bitmap_info_ogl::bitmap_info_ogl(IVideoDriver* videoDriver) :
	m_width(0),
	m_height(0),
	m_videoDriver(videoDriver),
	m_added_texture(false),
	m_texture(NULL),
	m_suspended_image(NULL),
	m_data(NULL),
	m_min_filter_mode(bitmap_info::FILTER_MODE_LINEAR),
	m_mag_filter_mode(bitmap_info::FILTER_MODE_LINEAR)
{
}

bitmap_info_ogl::bitmap_info_ogl(IVideoDriver* videoDriver, gameswf::image_rgba* im) :
	m_width(im->m_width),
	m_height(im->m_height),
	m_videoDriver(videoDriver),
	m_added_texture(false),
	m_texture(NULL),
	m_data(NULL),
	m_min_filter_mode(bitmap_info::FILTER_MODE_LINEAR),
	m_mag_filter_mode(bitmap_info::FILTER_MODE_LINEAR)
{
	assert(im);
	irr::core::dimension2d<irr::s32> dim(im->m_width, im->m_height);

	m_suspended_image = videoDriver->createImage(irr::video::ECF_A8R8G8B8, dim);
	unsigned char* out = (unsigned char*)m_suspended_image->lock();
	unsigned char* in = im->m_data;
	int count = im->m_width * im->m_height;
	for(int i = 0; i < count; i++)
	{
#if _TU_LITTLE_ENDIAN_ == 0
		const unsigned char r = *(in+0);
		const unsigned char g = *(in+1);
		const unsigned char b = *(in+2);
		const unsigned char a = *(in+3);
		*(out) = a;	// B
		*(out+1) = r;	// G
		*(out+2) = g;	// R
		*(out+3) = b;	// A
#else
		*(out) = *(in+2);	// B
		*(out+1) = *(in+1);	// G
		*(out+2) = *(in+0);	// R
		*(out+3) = *(in+3);	// A
#endif
		out += 4;
		in += 4;
	}
	m_suspended_image->unlock();
}

bitmap_info_ogl::bitmap_info_ogl(IVideoDriver* videoDriver, int width, int height, gameswf::Uint8* data) :
	m_width(width),
	m_height(height),
	m_videoDriver(videoDriver),
	m_added_texture(false),
	m_texture(NULL),
	m_data(NULL),
	m_min_filter_mode(bitmap_info::FILTER_MODE_LINEAR),
	m_mag_filter_mode(bitmap_info::FILTER_MODE_LINEAR)
{
	assert(width > 0 && height > 0);

	irr::core::dimension2d<irr::s32> dim(width, height);
#ifdef USE_ALPHA_TEXTURES
	m_suspended_image = videoDriver->createImage(irr::video::ECF_A8, dim);

	if(data)
	{
		unsigned char* pixels = (unsigned char*)m_suspended_image->lock();
		memcpy(pixels, data, width * height);
		m_suspended_image->unlock();
	}
#else // USE_ALPHA_TEXTURES
	m_suspended_image = videoDriver->createImage(irr::video::ECF_A8R8G8B8, dim);

	if(data)
	{
		unsigned char* pixels = (unsigned char*)m_suspended_image->lock();
		int count = width * height;
		for(int i = 0; i < count; i++)
		{
	#if _TU_LITTLE_ENDIAN_ == 0
			*(pixels++) = *(data++);
			*(pixels++) = 0xff;
			*(pixels++) = 0xff;
			*(pixels++) = 0xff;
	#else
			*(pixels++) = 0xff;
			*(pixels++) = 0xff;
			*(pixels++) = 0xff;
			*(pixels++) = *(data++);
	#endif
		}
		m_suspended_image->unlock();
	}
#endif // USE_ALPHA_TEXTURES
}

bitmap_info_ogl::bitmap_info_ogl(IVideoDriver* videoDriver, gameswf::image_rgb* im) :
	m_width(im->m_width),
	m_height(im->m_height),
	m_videoDriver(videoDriver),
	m_added_texture(false),
	m_texture(NULL),
	m_data(NULL),
	m_min_filter_mode(bitmap_info::FILTER_MODE_LINEAR),
	m_mag_filter_mode(bitmap_info::FILTER_MODE_LINEAR)
{
	assert(im);

	irr::core::dimension2d<irr::s32> dim(im->m_width, im->m_height);
	m_suspended_image = videoDriver->createImage(irr::video::ECF_A8R8G8B8, dim);
	unsigned char* out = (unsigned char*)m_suspended_image->lock();
	int count = im->m_width * im->m_height;
	for(int y = 0; y < im->m_height; y++)
	{
		unsigned char* in = scanline(im, y);

		for(int x = 0; x < im->m_width; x++)
		{
			//*(unsigned int*)out = (0xff << 24) | (*(in+0) << 16) | (*(in+1) << 8) | (*(in+2) << 0);
	#if _TU_LITTLE_ENDIAN_ == 0
			*(out) = 255;	// A
			*(out+1) = *(in+0);	// R
			*(out+2) = *(in+1);	// G
			*(out+3) = *(in+2);	// B
	#else
			*(out) = *(in+2);	// B
			*(out+1) = *(in+1);	// G
			*(out+2) = *(in+0);	// R
			*(out+3) = 255;	// A
	#endif
			out += 4;
			in += 3;
		}
	}
	m_suspended_image->unlock();
}

bitmap_info_ogl::bitmap_info_ogl(IVideoDriver* videoDriver, irr::video::ITexture* texture) :
	m_width(texture->getSize().Width),
	m_height(texture->getSize().Height),
	m_videoDriver(videoDriver),
	m_added_texture(false),
	m_texture(texture),
	m_suspended_image(NULL),
	m_data(NULL),
	m_min_filter_mode(bitmap_info::FILTER_MODE_LINEAR),
	m_mag_filter_mode(bitmap_info::FILTER_MODE_LINEAR)
{
	// grab the texture
	m_texture->grab();
}

bitmap_info_ogl::bitmap_info_ogl(IVideoDriver* videoDriver, int width, int height, const gameswf::membuf* data) :
	m_width(width),
	m_height(height),
	m_videoDriver(videoDriver),
	m_added_texture(false),
	m_texture(NULL),
	m_suspended_image(NULL),
	m_data(data),
	m_min_filter_mode(bitmap_info::FILTER_MODE_LINEAR),
	m_mag_filter_mode(bitmap_info::FILTER_MODE_LINEAR)
{
}

bitmap_info_ogl::bitmap_info_ogl(IVideoDriver* videoDriver, int width, int height) :
	m_width(width),
	m_height(height),
	m_videoDriver(videoDriver),
	m_added_texture(true),
	m_texture(NULL),
	m_suspended_image(NULL),
	m_data(NULL),
	m_min_filter_mode(bitmap_info::FILTER_MODE_LINEAR),
	m_mag_filter_mode(bitmap_info::FILTER_MODE_LINEAR)
{
	// create the texture target
	irr::core::dimension2d<irr::s32> dim(width, height);
	m_texture = videoDriver->createRenderTargetTexture(dim, "gameswf_target");
}

// layout image to opengl texture memory
void bitmap_info_ogl::layout()
{
	if(m_texture == NULL)
	{
		if(m_suspended_image != NULL)
		{
			// create texture from suspended image
			m_added_texture = true;
			m_texture = m_videoDriver->addTexture("image", m_suspended_image);
			m_texture->setMinFilter(s_filter_map[m_min_filter_mode]);
			m_texture->setMagFilter(s_filter_map[m_mag_filter_mode]);
			m_suspended_image->drop();
			m_suspended_image = NULL;
		}
		else if(m_data != NULL)
		{
			// create texture from native data
			char name[64];
			sprintf(name, "bitmap_info_ogl_0x%x", this);
			irr::io::IReadFile* file = irr::io::createMemoryReadFile((void*)m_data->data(), m_data->size(), name, false);
			m_texture = m_videoDriver->getTexture(file);
			m_texture->setMinFilter(s_filter_map[m_min_filter_mode]);
			m_texture->setMagFilter(s_filter_map[m_mag_filter_mode]);
			m_added_texture = true;
			file->drop();
			swfdelete (m_data);
			m_data = NULL;
		}
		else
		{
			assert(false);
		}
	}
}

gameswf::render_handler*	gameswf::create_render_handler_irrlicht(void* videoDriver)
// Factory.
{
	return swfnew render_handler_irrlicht((irr::video::IVideoDriver*)videoDriver);
}

#endif // _IRRLICHT

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
