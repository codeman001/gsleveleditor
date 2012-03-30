#ifndef __C_OPENGLES_INCLUDES_GL_H_INCLUDED__
#define __C_OPENGLES_INCLUDES_GL_H_INCLUDED__

#if defined(_IRR_COMPILE_WITH_OPENGL_ES_)

#include "IrrCompileConfig.h"
#include "irrTypes.h"

// ----------------------------------------------------------------------------
// Select the right headers (specific to the active OpenGL platform)
// ----------------------------------------------------------------------------
#if defined(_IRR_WINDOWS_API_)
	// include windows headers for HWND
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

	#define _IRR_OPENGLES_USE_EXTPOINTER_

	#include <GLES/egl.h>
	#include <GLES/gl.h>
	#include <GLES/glext.h>	

// ----------------------------------------------------------------------------
#elif defined(_IRR_USE_IPHONEOS_DEVICE_)
	#include "CIrrDeviceIPhoneOS.h"

	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
#endif

// ----------------------------------------------------------------------------
// Remove extension suffix to OpenGL functions for each implementation 
// and make sure to map to the right function
// ----------------------------------------------------------------------------
	#define glMapBuffer					glMapBufferOES
	#define glUnmapBuffer				glUnmapBufferOES
	#define glGetBufferPointerv			glGetBufferPointervOES
	#define glDeleteFramebuffers		glDeleteFramebuffersOES
	#define glBindFramebuffer			glBindFramebufferOES
	#define glGenFramebuffers			glGenFramebuffersOES
	#define glCheckFramebufferStatus	glCheckFramebufferStatusOES
	#define glFramebufferRenderbuffer	glFramebufferRenderbufferOES
	#define glFramebufferTexture2D		glFramebufferTexture2DOES
	#define glBindRenderbuffer			glBindRenderbufferOES
	#define glDeleteRenderbuffers		glDeleteRenderbuffersOES
	#define glGenRenderbuffers			glGenRenderbuffersOES
	#define glRenderbufferStorage		glRenderbufferStorageOES

	#define GL_SOURCE0_ALPHA			GL_SRC0_ALPHA
	#define GL_SOURCE1_ALPHA			GL_SRC1_ALPHA
	#define GL_SOURCE2_ALPHA			GL_SRC2_ALPHA
	#define GL_SOURCE0_RGB				GL_SRC0_RGB
	#define GL_SOURCE1_RGB				GL_SRC1_RGB
	#define GL_SOURCE2_RGB				GL_SRC2_RGB
	#define GL_RENDERBUFFER				GL_RENDERBUFFER_OES
	#define GL_FRAMEBUFFER				GL_FRAMEBUFFER_OES
	#define GL_COLOR_ATTACHMENT0		GL_COLOR_ATTACHMENT0_OES
	#define GL_STENCIL_ATTACHMENT		GL_STENCIL_ATTACHMENT_OES
	#define GL_DEPTH_ATTACHMENT			GL_DEPTH_ATTACHMENT_OES
	#define GL_POINT_SPRITE				GL_POINT_SPRITE_OES
	#define GL_COORD_REPLACE			GL_COORD_REPLACE_OES
	#define GL_DEPTH_STENCIL			GL_DEPTH_STENCIL_OES
	#define GL_DEPTH_COMPONENT24		GL_DEPTH_COMPONENT24_OES
	#define GL_UNSIGNED_INT_24_8		GL_UNSIGNED_INT_24_8_OES
	#define GL_MAX_PALETTE_MATRICES		GL_MAX_PALETTE_MATRICES_OES

	#define GL_FRAMEBUFFER_COMPLETE							GL_FRAMEBUFFER_COMPLETE_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT			GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS				GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS			GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT	GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES
	#define GL_FRAMEBUFFER_UNSUPPORTED						GL_FRAMEBUFFER_UNSUPPORTED_OES

#endif
#endif // __C_COMMON_INCLUDES_GL_H_INCLUDED__
