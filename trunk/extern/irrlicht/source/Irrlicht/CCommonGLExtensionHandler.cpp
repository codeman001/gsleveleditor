// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#if defined(_IRR_COMPILE_WITH_OPENGL_ES_) || defined(_IRR_COMPILE_WITH_PS3_)

#include "CCommonGLExtensionHandler.h"
#include "irrString.h"
#include "SMaterial.h" // for MATERIAL_MAX_TEXTURES
#include "fast_atof.h"

namespace irr
{
namespace video
{

// Flags that cans be turned to constant on OpenGL ES (compilation optimization)
const bool CCommonGLExtensionHandler::MultiTextureExtension = true;
const bool CCommonGLExtensionHandler::TextureCompressionExtension = false;

#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
CCommonGLExtensionHandler::SExtensionShadowState::SExtensionShadowState()
	: ActiveTexture(GL_TEXTURE0),
	  ClientActiveTexture(GL_TEXTURE0)
{
}
#endif

CCommonGLExtensionHandler::CCommonGLExtensionHandler()
	: StencilBuffer(false)
	, MultiSamplingExtension(false)
	, AnisotropyExtension(false)
	, MaxTextureUnits(1)
	, MaxLights(1)
	, MaxIndices(65535)
	, MaxAnisotropy(1.0f)
	, MaxUserClipPlanes(0)
	, Version(0)
	, ShaderLanguageVersion(0)
{

#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	pGlBlendEquationSeparateOES = NULL;
	pGlBlendFuncSeparateOES = NULL;
	pGlIsRenderbufferOES = NULL;
	pGlBindRenderbufferOES = NULL;
	pGlDeleteRenderbuffersOES = NULL;
	pGlGenRenderbuffersOES = NULL;
	pGlRenderbufferStorageOES = NULL;
	pGlGetRenderbufferParameterivOES = NULL;
	pGlIsFramebufferOES = NULL;
	pGlBindFramebufferOES = NULL;
	pGlDeleteFramebuffersOES = NULL;
	pGlGenFramebuffersOES = NULL;
	pGlCheckFramebufferStatusOES = NULL;
	pGlFramebufferRenderbufferOES = NULL;
	pGlFramebufferTexture2DOES = NULL;
	pGlGetFramebufferAttachmentParameterivOES = NULL;
	pGlGenerateMipmapOES = NULL;
	pGlMapBufferOES = NULL;
	pGlUnmapBufferOES = NULL;
	pGlGetBufferPointervOES = NULL;
#endif

	for (u32 i=0; i<IRR_OpenGL_Feature_Count; ++i)
		FeatureAvailable[i]=false;
}


void CCommonGLExtensionHandler::dump() const
{
	for (u32 i=0; i<IRR_OpenGL_Feature_Count; ++i)
		os::Printer::log(OpenGLFeatureStrings[i], FeatureAvailable[i]?" true":" false");
}

void CCommonGLExtensionHandler::initExtensions(bool stencilBuffer)
{
	core::stringc ver = reinterpret_cast<const c8*>(glGetString(GL_VERSION));
	int dot = ver.findFirst('.');
	if(dot>0)
	{
		int end = ver.findNext(' ', dot);
		if(end>dot)
			ver = ver.subString(dot-1,end-dot+1);
		else
			ver = ver.subString(dot-1,ver.size()-dot+1);
	}
	const f32 ogl_ver = core::fast_atof(ver.c_str());
	Version = core::floor32(ogl_ver)*100+core::ceil32(core::fract(ogl_ver)*10.0f);
	
	if ( Version >= 101)
		os::Printer::log("OpenGL|ES driver version is 1.1 or better.", ELL_INFORMATION);
	else
		os::Printer::log("OpenGL|ES driver version is not 1.1 or better.", ELL_WARNING);

	
	{
		const char* t = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
		const size_t len = strlen(t);
		c8 *str = new c8[len+1];
		c8* p = str;

		for (size_t i=0; i<len; ++i)
		{
			str[i] = static_cast<char>(t[i]);

			if (str[i] == ' ')
			{
				str[i] = 0;
				for (u32 j=0; j<IRR_OpenGL_Feature_Count; ++j)
				{
					if (!strcmp(OpenGLFeatureStrings[j], p))
					{
						FeatureAvailable[j] = true;
						break;
					}
				}

				p = p + strlen(p) + 1;
			}
		}

		delete [] str;
	}

	

	MultiSamplingExtension = FeatureAvailable[IRR_ARB_multisample];
	AnisotropyExtension = FeatureAvailable[IRR_EXT_texture_filter_anisotropic];
	StencilBuffer=stencilBuffer;

#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)

	pGlBlendEquationSeparateOES = (PFNGLBLENDEQUATIONSEPARATEOESPROC)
		eglGetProcAddress("glBlendEquationSeparateOES");
	pGlBlendFuncSeparateOES = (PFNGLBLENDFUNCSEPARATEOESPROC)
		eglGetProcAddress("glBlendFuncSeparateOES");
	pGlIsRenderbufferOES = (PFNGLISRENDERBUFFEROESPROC)
		eglGetProcAddress("glIsRenderbufferOES");
	pGlBindRenderbufferOES = (PFNGLBINDRENDERBUFFEROESPROC)
		eglGetProcAddress("glBindRenderbufferOES");
	pGlDeleteRenderbuffersOES = (PFNGLDELETERENDERBUFFERSOESPROC)
		eglGetProcAddress("glDeleteRenderbuffersOES");
	pGlGenRenderbuffersOES = (PFNGLGENRENDERBUFFERSOESPROC)
		eglGetProcAddress("glGenRenderbuffersOES");
	pGlRenderbufferStorageOES = (PFNGLRENDERBUFFERSTORAGEOESPROC)
		eglGetProcAddress("glRenderbufferStorageOES");
	pGlGetRenderbufferParameterivOES = (PFNGLGETRENDERBUFFERPARAMETERIVOESPROC)
		eglGetProcAddress("glGetRenderbufferParameterivOES");
	pGlIsFramebufferOES = (PFNGLISFRAMEBUFFEROESPROC)
		eglGetProcAddress("glIsFramebufferOES");
	pGlBindFramebufferOES = (PFNGLBINDFRAMEBUFFEROESPROC)
		eglGetProcAddress("glBindFramebufferOES");
	pGlDeleteFramebuffersOES = (PFNGLDELETEFRAMEBUFFERSOESPROC)
		eglGetProcAddress("glDeleteFramebuffersOES");
	pGlGenFramebuffersOES = (PFNGLGENFRAMEBUFFERSOESPROC)
		eglGetProcAddress("glGenFramebuffersOES");
	pGlCheckFramebufferStatusOES = (PFNGLCHECKFRAMEBUFFERSTATUSOESPROC)
		eglGetProcAddress("glCheckFramebufferStatusOES");
	pGlFramebufferRenderbufferOES = (PFNGLFRAMEBUFFERRENDERBUFFEROESPROC)
		eglGetProcAddress("glFramebufferRenderbufferOES");
	pGlFramebufferTexture2DOES = (PFNGLFRAMEBUFFERTEXTURE2DOESPROC)
		eglGetProcAddress("glFramebufferTexture2DOES");
	pGlGetFramebufferAttachmentParameterivOES = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOESPROC)
		eglGetProcAddress("glGetFramebufferAttachmentParameterivOES");
	pGlGenerateMipmapOES = (PFNGLGENERATEMIPMAPOESPROC)
		eglGetProcAddress("glGenerateMipmapOES");
	pGlMapBufferOES = (PFNGLMAPBUFFEROESPROC)
		eglGetProcAddress("glMapBufferOES");
	pGlUnmapBufferOES = (PFNGLUNMAPBUFFEROESPROC)
		eglGetProcAddress("glUnmapBufferOES");
	pGlGetBufferPointervOES = (PFNGLGETBUFFERPOINTERVOESPROC)
		eglGetProcAddress("glGetBufferPointervOES");
#endif

	GLint num;
	

	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &num);
	MaxTextureUnits=core::min_((u32)num, MATERIAL_MAX_TEXTURES);

	glGetIntegerv(GL_MAX_LIGHTS, &MaxLights);
	
#ifdef GL_EXT_texture_filter_anisotropic
	if (FeatureAvailable[IRR_EXT_texture_filter_anisotropic])
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &MaxAnisotropy);
#endif
		
#ifdef GL_OES_matrix_palette
	if (FeatureAvailable[IRR_OES_matrix_palette])
	{
		glGetIntegerv(GL_MAX_PALETTE_MATRICES, &num);
		MaxPaletteMatrices = num;
		glGetIntegerv(GL_MAX_VERTEX_UNITS_OES, &num);
		MaxVertexUnits = num;
	}
	else
	{
		MaxPaletteMatrices = 0;
		MaxVertexUnits = 0;
	}
#elif defined(GL_ARB_matrix_palette) && defined(GL_ARB_vertex_blend)
	if (FeatureAvailable[IRR_ARB_matrix_palette])
	{
		glGetIntegerv(GL_MAX_PALETTE_MATRICES_ARB, &num);
		MaxPaletteMatrices = num;
	}
	else
	{
		MaxPaletteMatrices = 0;
	}
	if (FeatureAvailable[IRR_ARB_vertex_blend])
	{
		glGetIntegerv(GL_MAX_VERTEX_UNITS_ARB, &num);
		MaxVertexUnits = num;
	}
	else
	{
		MaxVertexUnits = 0;
	}
#endif	

}

bool CCommonGLExtensionHandler::queryFeature(E_VIDEO_DRIVER_FEATURE feature) const
{
	switch (feature)
	{
#if defined(_IRR_USE_PS3_DEVICE_)
	case EVDF_MULTITEXTURE:
		return MultiTextureExtension;
	case EVDF_BILINEAR_FILTER:
	case EVDF_MIP_MAP:
	case EVDF_TEXTURE_NPOT:
	case EVDF_TEXTURE_NSQUARE:
	case EVDF_FRAMEBUFFER_OBJECT:
	case EVDF_VERTEX_BUFFER_OBJECT:
	case EVDF_STENCIL_BUFFER:
		return true;
#else
	case EVDF_MULTITEXTURE:
		return MultiTextureExtension;
	case EVDF_BILINEAR_FILTER:
	case EVDF_MIP_MAP:
	case EVDF_RENDER_TO_TARGET:
		return true;
	case EVDF_MIP_MAP_AUTO_UPDATE:
		return true;
	case EVDF_TEXTURE_NPOT:
		// Some ATI cards seem to have only SW support in OpenGL 2.0
		// drivers if the extension is not exposed, so we skip this
		// extra test for now!
		// return (FeatureAvailable[IRR_ARB_texture_non_power_of_two]||Version>=200);
		return FeatureAvailable[IRR_ARB_texture_non_power_of_two] || FeatureAvailable[IRR_OES_texture_npot];
	case EVDF_FRAMEBUFFER_OBJECT:
		return FeatureAvailable[IRR_EXT_framebuffer_object] || FeatureAvailable[IRR_OES_framebuffer_object];
	case EVDF_VERTEX_BUFFER_OBJECT:
		return true;
#endif
	default:
		return false;
	};
}


} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OPENGL_ || _IRR_COMPILE_WITH_OPENGL_ES_

