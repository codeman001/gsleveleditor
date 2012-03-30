// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_OPENGLES_GL_FEATURE_MAP_H_INCLUDED__
#define __C_OPENGLES_GL_FEATURE_MAP_H_INCLUDED__

#include "IrrCompileConfig.h"
#if defined(_IRR_COMPILE_WITH_OPENGL_ES_)

#include "COpenGLESHeaders.h"
#include "COpenGLESExtensionList.h"
#include "EDriverFeatures.h"
#include "os.h"

#pragma warning( disable : 4530)
#include <bitset>

namespace irr
{
namespace video
{

class COpenGLESExtensionHandler
{
public:
	// constructor
	COpenGLESExtensionHandler();

	// deferred initialization
	void initExtensions(bool stencilBuffer);

	//! queries the features of the driver, returns true if feature is available
	bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const;

	//! queries the features of the driver, returns true if feature is available
	bool queryOpenGLFeature(EOpenGLFeatures feature) const
	{
		return FeatureAvailable[feature];
	}

	// Some non-boolean properties
	u32 getMaxTextureUnits() {return MaxTextureUnits;}
	GLint getMaxLights() {return MaxLights;}
	GLint getMaxIndices() {return MaxIndices;}
	f32 getMaxAnisotropy() {return MaxAnisotropy;}
	u32 getMaxUserClipPlanes() {return MaxUserClipPlanes;}
	u32 getMaxPaletteMatrices() {return MaxPaletteMatrices;}
	u32 getMaxVertexUnits() {return MaxVertexUnits;}
	u32 getVersion() {return Version;}
	u32 getShaderLanguageVersion() {return ShaderLanguageVersion;}

	void setMaxTextureUnits(u32 value) {MaxTextureUnits = value;}
	void setMaxLights(GLint value) {MaxLights = value;}
	void setMaxIndices(GLint value) {MaxIndices = value;}
	void setMaxAnisotropy(f32 value) {MaxAnisotropy = value;}
	void setMaxUserClipPlanes(u32 value) {MaxUserClipPlanes = value;}
	void setMaxPaletteMatrices(u32 value) {MaxPaletteMatrices = value;}
	void setMaxVertexUnits(u32 value) {MaxVertexUnits = value;}
	void setVersion(u32 value) {Version = value;}
	void setShaderLanguageVersion(u32 value) {ShaderLanguageVersion = value;}

	//! show all features with availablity
	void dump() const;

			
	void pointParameterf(GLint loc, GLfloat f);
	void pointParameterfv(GLint loc, const GLfloat *v);

	void compressedTexImage2D(GLenum target, GLint level,
		GLenum internalformat, GLsizei width, GLsizei height,
		GLint border, GLsizei imageSize, const void* data);

	
	void extGlBindFramebuffer(GLenum target, GLuint framebuffer);
	void extGlDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
	void extGlGenFramebuffers(GLsizei n, GLuint *framebuffers);
	GLenum extGlCheckFramebufferStatus(GLenum target);
	void extGlFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	void extGlBindRenderbuffer(GLenum target, GLuint renderbuffer);
	void extGlDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
	void extGlGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
	void extGlRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	void extGlFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	void extGlActiveStencilFace(GLenum face);

	void extGlActiveTexture(GLenum texture);
	void extGlClientActiveTexture(GLenum texture);
	GLenum extGlGetActiveTexture();

	// vertex buffer object
	void genBuffers(GLsizei n, GLuint *buffers);
	void bindBuffer(GLenum target, GLuint buffer);
	void bufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
	void deleteBuffers(GLsizei n, const GLuint *buffers);
	void bufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);

	void *mapBuffer (GLenum target, GLenum access);
	GLboolean unmapBuffer (GLenum target);
	GLboolean isBuffer (GLuint buffer);
	void getBufferParameteriv (GLenum target, GLenum pname, GLint *params);
	void getBufferPointerv (GLenum target, GLenum pname, GLvoid **params);

public:

	// Some variables for properties
	// Flags that cans be turned to constant on OpenGL ES
	static const bool MultiTextureExtension;
	static const bool TextureCompressionExtension;

	bool StencilBuffer;
	bool MultiSamplingExtension;
	bool AnisotropyExtension;
	
	// Some non-boolean properties
	//! Maxmimum texture layers supported by the fixed pipeline
	u32 MaxTextureUnits;
	//! Maximum hardware lights supported
	GLint MaxLights;
	//! Optimal number of indices per meshbuffer
	GLint MaxIndices;
	//! Maximal Anisotropy
	f32 MaxAnisotropy;
	//! Number of user clipplanes
	u32 MaxUserClipPlanes;
	//! Maximum number of palette matrices
	u32 MaxPaletteMatrices;
	//! Maximum number of vertex units
	u32 MaxVertexUnits;
	//! Maximum size of texture 2048
	u32 MaxTextureSize;

	//! OpenGL version as Integer: 100*Major+Minor, i.e. 2.1 becomes 201
	u32 Version;
	//! GLSL version as Integer: 100*Major+Minor
	u32 ShaderLanguageVersion;


#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
	//! Implements a state shadow layer for caching some render states.
	struct SExtensionShadowState
	{
		// Add more as needed

		GLenum ActiveTexture;
		GLenum ClientActiveTexture;

		SExtensionShadowState();

	} ExtensionShadowState;
#endif

	// the global feature array
	std::bitset<IRR_OpenGL_Feature_Count> FeatureAvailable;

#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	PFNGLBLENDEQUATIONSEPARATEOESPROC	pGlBlendEquationSeparateOES;
	PFNGLBLENDFUNCSEPARATEOESPROC		pGlBlendFuncSeparateOES;
	PFNGLISRENDERBUFFEROESPROC			pGlIsRenderbufferOES;
	PFNGLBINDRENDERBUFFEROESPROC		pGlBindRenderbufferOES;
	PFNGLDELETERENDERBUFFERSOESPROC		pGlDeleteRenderbuffersOES;
	PFNGLGENRENDERBUFFERSOESPROC		pGlGenRenderbuffersOES;
	PFNGLRENDERBUFFERSTORAGEOESPROC		pGlRenderbufferStorageOES;
	PFNGLGETRENDERBUFFERPARAMETERIVOESPROC pGlGetRenderbufferParameterivOES;
	PFNGLISFRAMEBUFFEROESPROC			pGlIsFramebufferOES;
	PFNGLBINDFRAMEBUFFEROESPROC			pGlBindFramebufferOES;
	PFNGLDELETEFRAMEBUFFERSOESPROC		pGlDeleteFramebuffersOES;
	PFNGLGENFRAMEBUFFERSOESPROC			pGlGenFramebuffersOES;
	PFNGLCHECKFRAMEBUFFERSTATUSOESPROC	pGlCheckFramebufferStatusOES;
	PFNGLFRAMEBUFFERRENDERBUFFEROESPROC pGlFramebufferRenderbufferOES;
	PFNGLFRAMEBUFFERTEXTURE2DOESPROC	pGlFramebufferTexture2DOES;
	PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOESPROC pGlGetFramebufferAttachmentParameterivOES;
	PFNGLGENERATEMIPMAPOESPROC			pGlGenerateMipmapOES;
	PFNGLMAPBUFFEROESPROC				pGlMapBufferOES;
	PFNGLUNMAPBUFFEROESPROC				pGlUnmapBufferOES;
	PFNGLGETBUFFERPOINTERVOESPROC		pGlGetBufferPointervOES;
#endif
};



inline void COpenGLESExtensionHandler::extGlActiveTexture(GLenum texture)
{
	if (MultiTextureExtension
#    ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		&& texture != ExtensionShadowState.ActiveTexture
#    endif
	)
	{
		glActiveTexture(texture);
#    ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		ExtensionShadowState.ActiveTexture = texture;
#    endif
	}
}

inline void COpenGLESExtensionHandler::extGlClientActiveTexture(GLenum texture)
{
	if (MultiTextureExtension
#    ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		&& texture != ExtensionShadowState.ClientActiveTexture
#    endif
	)
	{
		glClientActiveTexture(texture);
#    ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		ExtensionShadowState.ClientActiveTexture = texture;
#    endif
	}
}

inline GLenum COpenGLESExtensionHandler::extGlGetActiveTexture()
{
#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
	return ExtensionShadowState.ClientActiveTexture;
#else
	return 0;
#endif
}

inline void COpenGLESExtensionHandler::pointParameterf(GLint loc, GLfloat f)
{
#if defined(GL_ARB_point_parameters) || defined(_IRR_COMPILE_WITH_OPENGL_ES_)
	glPointParameterf(loc, f);
#elif defined(_DEBUG)
	os::Printer::log("glPointParameterf not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::pointParameterfv(GLint loc, const GLfloat *v)
{
#if defined(GL_ARB_point_parameters)
	glPointParameterfv(loc, v);
#elif defined(_DEBUG)
	os::Printer::log("glPointParameterfv not supported", ELL_ERROR);
#endif
}


inline void COpenGLESExtensionHandler::compressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width,
		GLsizei height, GLint border, GLsizei imageSize, const void* data)
{
#if defined(GL_ARB_texture_compression) || defined(_IRR_COMPILE_WITH_OPENGL_ES_)
	glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
#elif defined(_DEBUG)
	os::Printer::log("glCompressedTexImage2D not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::extGlBindFramebuffer(GLenum target, GLuint framebuffer)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlBindFramebufferOES)
		pGlBindFramebufferOES(target, framebuffer);
#elif defined(GL_EXT_framebuffer_object) || defined(GL_OES_framebuffer_object)
	glBindFramebuffer(target, framebuffer);
#elif defined(_DEBUG)
	os::Printer::log("glBindFramebuffer not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::extGlDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlDeleteFramebuffersOES)
		pGlDeleteFramebuffersOES(n, framebuffers);
#elif defined(GL_EXT_framebuffer_object) || defined(GL_OES_framebuffer_object)
	glDeleteFramebuffers(n, framebuffers);
#elif defined(_DEBUG)
	os::Printer::log("glDeleteFramebuffers not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::extGlGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlGenFramebuffersOES)
		pGlGenFramebuffersOES(n, framebuffers);
#elif defined(GL_EXT_framebuffer_object) || defined(GL_OES_framebuffer_object)
	glGenFramebuffers(n, framebuffers);
#elif defined(_DEBUG)
	os::Printer::log("glGenFramebuffers not supported", ELL_ERROR);
#endif
}

inline GLenum COpenGLESExtensionHandler::extGlCheckFramebufferStatus(GLenum target)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlCheckFramebufferStatusOES)
		return pGlCheckFramebufferStatusOES(target);
	else
		return 0;
#elif defined(GL_EXT_framebuffer_object) || defined(GL_OES_framebuffer_object)
	return glCheckFramebufferStatus(target);
#elif defined(_DEBUG)
	os::Printer::log("glCheckFramebufferStatus not supported", ELL_ERROR);
#endif
	return 0;
}

inline void COpenGLESExtensionHandler::extGlFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlFramebufferTexture2DOES)
		pGlFramebufferTexture2DOES(target, attachment, textarget, texture, level);
#elif defined(GL_EXT_framebuffer_object) || defined(GL_OES_framebuffer_object)
	glFramebufferTexture2D(target, attachment, textarget, texture, level);
#elif defined(_DEBUG)
	os::Printer::log("glFramebufferTexture2D not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::extGlBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlBindRenderbufferOES)
		pGlBindRenderbufferOES(target, renderbuffer);
#elif defined(GL_EXT_framebuffer_object) || defined(GL_OES_framebuffer_object)
	glBindRenderbuffer(target, renderbuffer);
#elif defined(_DEBUG)
	os::Printer::log("glBindRenderbuffer not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::extGlDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlDeleteRenderbuffersOES)
		pGlDeleteRenderbuffersOES(n, renderbuffers);
#elif defined(GL_EXT_framebuffer_object) || defined(GL_OES_framebuffer_object)
	glDeleteRenderbuffers(n, renderbuffers);
#elif defined(_DEBUG)
	os::Printer::log("glDeleteRenderbuffers not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::extGlGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlGenRenderbuffersOES)
		pGlGenRenderbuffersOES(n, renderbuffers);
#elif defined(GL_EXT_framebuffer_object) || defined(GL_OES_framebuffer_object)
	glGenRenderbuffers(n, renderbuffers);
#elif defined(_DEBUG)
	os::Printer::log("glGenRenderbuffers not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::extGlRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlRenderbufferStorageOES)
		pGlRenderbufferStorageOES(target, internalformat, width, height);
#elif defined(GL_EXT_framebuffer_object) || defined(GL_OES_framebuffer_object)
	glRenderbufferStorage(target, internalformat, width, height);
#elif defined(_DEBUG)
	os::Printer::log("glRenderbufferStorage not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::extGlFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlFramebufferRenderbufferOES)
		pGlFramebufferRenderbufferOES(target, attachment, renderbuffertarget, renderbuffer);
#elif defined(GL_EXT_framebuffer_object) || defined(GL_OES_framebuffer_object)
	glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
#elif defined(_DEBUG)
	os::Printer::log("glFramebufferRenderbuffer not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::extGlActiveStencilFace(GLenum face)
{
#if defined(GL_EXT_stencil_two_side)
	glActiveStencilFaceEXT(face);
#elif defined(_DEBUG)
	os::Printer::log("glActiveStencilFace not supported", ELL_ERROR);
#endif
}


inline void COpenGLESExtensionHandler::genBuffers(GLsizei n, GLuint *buffers)
{
#if defined(GL_ARB_vertex_buffer_object) || defined(_IRR_COMPILE_WITH_OPENGL_ES_)
	glGenBuffers(n, buffers);
#elif defined(_DEBUG)
	os::Printer::log("glGenBuffers not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::bindBuffer(GLenum target, GLuint buffer)
{
#if defined(GL_ARB_vertex_buffer_object) || defined(_IRR_COMPILE_WITH_OPENGL_ES_)
	glBindBuffer(target, buffer);
#elif defined(_DEBUG)
	os::Printer::log("glBindBuffer not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::bufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
#if defined(GL_ARB_vertex_buffer_object) || defined(_IRR_COMPILE_WITH_OPENGL_ES_)
	glBufferData(target, size, data, usage);
#elif defined(_DEBUG)
	os::Printer::log("glBufferData not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::deleteBuffers(GLsizei n, const GLuint *buffers)
{
#if defined(GL_ARB_vertex_buffer_object) || defined(_IRR_COMPILE_WITH_OPENGL_ES_)
	glDeleteBuffers(n, buffers);
#elif defined(_DEBUG)
	os::Printer::log("glDeleteBuffers not supported", ELL_ERROR);
#endif
}

inline void COpenGLESExtensionHandler::bufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data)
{
#if defined(GL_ARB_vertex_buffer_object) || defined(_IRR_COMPILE_WITH_OPENGL_ES_)
	glBufferSubData(target, offset, size, data);
#elif defined(_DEBUG)
	os::Printer::log("glBufferSubData not supported", ELL_ERROR);
#endif
}

#if !defined(_IRR_IPHONE_EMULATION_)
inline void *COpenGLESExtensionHandler::mapBuffer(GLenum target, GLenum access)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlMapBufferOES)
		return pGlMapBufferOES(target, access);
	return 0;
#elif defined(GL_ARB_vertex_buffer_object) || defined(_IRR_COMPILE_WITH_OPENGL_ES_)
	return glMapBuffer(target, access);
#elif defined(_DEBUG)
	os::Printer::log("glMapBuffer not supported", ELL_ERROR);
#endif
	return 0;
}

inline GLboolean COpenGLESExtensionHandler::unmapBuffer(GLenum target)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlUnmapBufferOES)
		return pGlUnmapBufferOES(target);
	return false;
#elif defined(GL_ARB_vertex_buffer_object) || defined(_IRR_COMPILE_WITH_OPENGL_ES_)
	return glUnmapBuffer(target);
#elif defined(_DEBUG)
	os::Printer::log("glUnmapBuffer not supported", ELL_ERROR);
#endif
	return false;
}
#endif //!_IRR_IPHONE_EMULATION_

inline GLboolean COpenGLESExtensionHandler::isBuffer (GLuint buffer)
{
#if defined(GL_ARB_vertex_buffer_object) || defined(_IRR_COMPILE_WITH_OPENGL_ES_) 
	return glIsBuffer(buffer);
#elif defined(_DEBUG)
	os::Printer::log("glDeleteBuffers not supported", ELL_ERROR);
#endif
	return false;
}

inline void COpenGLESExtensionHandler::getBufferParameteriv (GLenum target, GLenum pname, GLint *params)
{
#if defined(GL_ARB_vertex_buffer_object) || defined(_IRR_COMPILE_WITH_OPENGL_ES_)
	glGetBufferParameteriv(target, pname, params);
#elif defined(_DEBUG)
	os::Printer::log("glGetBufferParameteriv not supported", ELL_ERROR);
#endif
}

#if !defined(_IRR_IPHONE_EMULATION_)
inline void COpenGLESExtensionHandler::getBufferPointerv (GLenum target, GLenum pname, GLvoid **params)
{
#if defined(_IRR_OPENGLES_USE_EXTPOINTER_)
	if (pGlGetBufferPointervOES)
		pGlGetBufferPointervOES(target, pname, params);
#elif defined(GL_ARB_vertex_buffer_object) || defined(_IRR_COMPILE_WITH_OPENGL_ES_)
	glGetBufferPointerv(target, pname, params);
#elif defined(_DEBUG)
	os::Printer::log("glGetBufferPointerv not supported", ELL_ERROR);
#endif
}
#endif //!_IRR_IPHONE_EMULATION_

} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OPENGL_ || _IRR_COMPILE_WITH_OPENGL_ES_
#endif // __C_COMMON_GL_FEATURE_MAP_H_INCLUDED__
