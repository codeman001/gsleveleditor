// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "COpenGLDriver.h"
// needed here also because of the create methods' parameters
#include "CNullDriver.h"

#ifdef _IRR_COMPILE_WITH_OPENGL_

#include "COpenGLTexture.h"
#include "COpenGLMaterialRenderer.h"
#include "COpenGLShaderMaterialRenderer.h"
#include "COpenGLSLMaterialRenderer.h"
#include "COpenGLNormalMapRenderer.h"
#include "COpenGLParallaxMapRenderer.h"
#include "CImage.h"
#include "os.h"

#ifdef _IRR_USE_SDL_DEVICE_
#include <SDL/SDL.h>
#endif

#ifdef IPHONEOS
extern bool isLeftLandscape();
#endif

#include "irrProcessBufferHeap.h"

#include "EVertexAttributes.h"

//CT GLES2 driver
extern bool useHighLevelShader;

extern irr::s32 getImageMaterialType(irr::video::ITexture* texture);

#if defined(_IRR_COMPILE_WITH_OPENGL_ES2_) && !defined(_IRR_USE_IPHONEOS_DEVICE_)
EGLDisplay			eglDisplay	= 0;
EGLSurface			eglSurface	= 0;
EGLContext			eglContext	= 0;
EGLNativeWindowType	eglWindow	= 0;
#endif

#if defined(_IRR_USE_IPHONEOS_DEVICE_)
extern bool isLeftLandscape(); 
#endif

namespace irr
{
namespace video
{

#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
// Initial values are taken from OpenGL specifications

COpenGLDriver::SShadowState::STexEnv::STexEnv()
#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
	  : Mode(GL_MODULATE),
	  CombineRGB(GL_MODULATE),
	  CombineAlpha(GL_MODULATE),
	  RGBScale(1.0f),
	  AlphaScale(1.0f)
#endif
{
#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
	Source[0].RGB = Source[0].Alpha = GL_TEXTURE;
	Source[0].OperandRGB = GL_SRC_COLOR;
	Source[0].OperandAlpha = GL_SRC_ALPHA;

	Source[1].RGB = Source[1].Alpha = GL_PREVIOUS;
	Source[1].OperandRGB = GL_SRC_COLOR;
	Source[1].OperandAlpha = GL_SRC_ALPHA;

	Source[2].RGB = Source[2].Alpha = GL_CONSTANT;
	Source[2].OperandRGB = GL_SRC_ALPHA;
	Source[2].OperandAlpha = GL_SRC_ALPHA;
#endif
}

COpenGLDriver::SShadowState::SShadowState()
{
}
#endif

// -----------------------------------------------------------------------
// WINDOWS CONSTRUCTOR
// -----------------------------------------------------------------------
#ifdef _IRR_USE_WINDOWS_DEVICE_
//! Windows constructor and init code
COpenGLDriver::COpenGLDriver(const core::dimension2d<s32>& screenSize,
		HWND window, bool stencilBuffer,
		io::IFileSystem* io, bool antiAlias)
: CNullDriver(io, screenSize), COpenGLExtensionHandler(),CurrentOrientation3D(EOO_0),
	CurrentRenderMode(ERM_NONE), Reset2DRenderStates(true), ResetRenderStates(true), Transformation3DChanged(true),
	AntiAlias(antiAlias), IsTextureMatrixIdentity(-1),
	RenderTargetTexture(0), LastSetLight(-1),
	CurrentRendertargetSize(0,0),
	HDc(0), Window(window), HRc(0)
{
	#ifdef _DEBUG
	setDebugName("COpenGLDriver");
	#endif
}

//! inits the open gl driver
bool COpenGLDriver::initDriver(const core::dimension2d<s32>& screenSize,
				HWND window, u32 bits, bool vsync, bool stencilBuffer)
{
#ifdef _IRR_COMPILE_WITH_OPENGL_ES2_
	EGLint error = 0;

	eglWindow = Window;
	HDc = GetDC(Window);

	//eglDisplay = eglGetDisplay(HDc);
    //if(eglDisplay == EGL_NO_DISPLAY)
         eglDisplay = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);

	EGLint majorVersion, minorVersion;
	if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion))
	{
		os::Printer::log("Could not initialize OpenGL-ES2 display.");
	}
	else
	{
		char text[64];
		sprintf(text, "EglDisplay initialized. Egl version %d.%d\n", majorVersion, minorVersion);
		os::Printer::log(text);
	}

	EGLint attribs[] = 
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_DEPTH_SIZE, 16,
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6, 
		EGL_BLUE_SIZE, 5, 
		EGL_ALPHA_SIZE, 0,
		EGL_NONE
	};

	EGLConfig config;
	EGLint num_configs;
	if (!eglChooseConfig(eglDisplay, attribs, &config, 1, &num_configs))
	{
		os::Printer::log("Could not choose config for OpenGL-ES2 display.");
	}

	EGLint contextAttrib[] = 
	{ 
		EGL_CONTEXT_CLIENT_VERSION, 
		2, 
		EGL_NONE 
	};

	eglContext = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, contextAttrib);
	error = eglGetError();
	if(eglContext == EGL_NO_CONTEXT || error != EGL_SUCCESS)
	{
		os::Printer::log("Could not create context for OpenGL-ES2 display.");
	}

	eglSurface = eglCreateWindowSurface(eglDisplay, config, eglWindow, NULL);
	if (eglSurface == EGL_NO_SURFACE) eglSurface = eglCreateWindowSurface(eglDisplay, config, NULL, NULL);
	error = eglGetError();
	if(eglSurface == EGL_NO_SURFACE || error != EGL_SUCCESS)
	{
		os::Printer::log("Could not create surface for OpenGL-ES2 display.");
	}

	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	error = eglGetError();
	if(error != EGL_SUCCESS)
	{
		os::Printer::log("Could not make Context current for OpenGL-ES2 display.");
	}

	genericDriverInit(screenSize, stencilBuffer);

	eglSwapInterval(eglDisplay, vsync ? 1 : 0);

	// set exposed data
	ExposedData.OpenGLWin32.HDc = HDc;
	ExposedData.OpenGLWin32.HRc = HRc;
	ExposedData.OpenGLWin32.HWnd = Window;
#else
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
		1,				// Version Number
		PFD_DRAW_TO_WINDOW |		// Format Must Support Window
		PFD_SUPPORT_OPENGL |		// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,		// Must Support Double Buffering
		PFD_TYPE_RGBA,			// Request An RGBA Format
		bits,				// Select Our Color Depth
		0, 0, 0, 0, 0, 0,		// Color Bits Ignored
		0,				// No Alpha Buffer
		0,				// Shift Bit Ignored
		0,				// No Accumulation Buffer
		0, 0, 0, 0,			// Accumulation Bits Ignored
		24,				// Z-Buffer (Depth Buffer)
		stencilBuffer ? 1 : 0,		// Stencil Buffer Depth
		0,				// No Auxiliary Buffer
		PFD_MAIN_PLANE,			// Main Drawing Layer
		0,				// Reserved
		0, 0, 0				// Layer Masks Ignored
	};

	for (u32 i=0; i<5; ++i)
	{
		if (i == 1)
		{
			if (stencilBuffer)
			{
				os::Printer::log("Cannot create a GL device with stencil buffer, disabling stencil shadows.", ELL_WARNING);
				stencilBuffer = false;
				pfd.cStencilBits = 0;
			}
			else
				continue;
		}
		else
		if (i == 2)
		{
			pfd.cDepthBits = 24;
		}
		if (i == 3)
		{
			if (bits!=16)
				pfd.cDepthBits = 16;
			else
				continue;
		}
		else
		if (i == 4)
		{
			os::Printer::log("Cannot create a GL device context.", ELL_ERROR);
			return false;
		}

		// get hdc
		if (!(HDc=GetDC(window)))
		{
			os::Printer::log("Cannot create a GL device context.", ELL_ERROR);
			continue;
		}

		GLuint PixelFormat;

		// choose pixelformat
		if (!(PixelFormat = ChoosePixelFormat(HDc, &pfd)))
		{
			os::Printer::log("Cannot find a suitable pixelformat.", ELL_ERROR);
			continue;
		}

		// set pixel format
		if(!SetPixelFormat(HDc, PixelFormat, &pfd))
		{
			os::Printer::log("Cannot set the pixel format.", ELL_ERROR);
			continue;
		}

		// create rendering context
		if (!(HRc=wglCreateContext(HDc)))
		{
			os::Printer::log("Cannot create a GL rendering context.", ELL_ERROR);
			continue;
		}

		// activate rendering context
		if(!wglMakeCurrent(HDc, HRc))
		{
			os::Printer::log("Cannot activate GL rendering context", ELL_ERROR);
			continue;
		}

		break;
	}

	if (HDc)
	{
		int pf = GetPixelFormat(HDc);
		DescribePixelFormat(HDc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
		if (pfd.cAlphaBits != 0)
		{
			if (pfd.cRedBits == 8)
				ColorFormat = ECF_A8R8G8B8;
			else
				ColorFormat = ECF_A1R5G5B5;
		}
		else
		{
			if (pfd.cRedBits == 8)
				ColorFormat = ECF_R8G8B8;
			else
				ColorFormat = ECF_R5G6B5;
		}
	}

	genericDriverInit(screenSize, stencilBuffer);

	extGlSwapInterval(vsync ? 1 : 0);

	// set exposed data
	ExposedData.OpenGLWin32.HDc = HDc;
	ExposedData.OpenGLWin32.HRc = HRc;
	ExposedData.OpenGLWin32.HWnd = Window;
#endif

	return true;
}

#endif //IRR_USE_WINDOWS_DEVICE_

// -----------------------------------------------------------------------
// MacOSX CONSTRUCTOR
// -----------------------------------------------------------------------
#ifdef _IRR_USE_OSX_DEVICE_
//! Windows constructor and init code
COpenGLDriver::COpenGLDriver(const SIrrlichtCreationParameters& params,
		io::IFileSystem* io, CIrrDeviceMacOSX *device)
: CNullDriver(io, params.WindowSize), COpenGLExtensionHandler(),
	CurrentRenderMode(ERM_NONE), ResetRenderStates(true), Transformation3DChanged(true),
	AntiAlias(params.AntiAlias), IsTextureMatrixIdentity(-1),
	RenderTargetTexture(0), LastSetLight(-1),
	CurrentRendertargetSize(0,0), ColorFormat(ECF_R8G8B8), _device(device)
{
	#ifdef _DEBUG
	setDebugName("COpenGLDriver");
	#endif
	genericDriverInit(params.WindowSize, params.Stencilbuffer);
	yViewPortOffset = 0;
}

#endif

#ifdef _IRR_USE_IPHONEOS_DEVICE_
COpenGLDriver::COpenGLDriver(const SIrrlichtCreationParameters& params,
		io::IFileSystem* io, CIrrDeviceIPhoneOS *device)
: CNullDriver(io, params.WindowSize), COpenGLExtensionHandler(),
	CurrentRenderMode(ERM_NONE), ResetRenderStates(true), Transformation3DChanged(true),
	AntiAlias(params.AntiAlias), IsTextureMatrixIdentity(-1),
	RenderTargetTexture(0), LastSetLight(-1),
	CurrentRendertargetSize(0,0), ColorFormat(ECF_R8G8B8), _device(device)
{
	#ifdef _DEBUG
	setDebugName("COpenGLDriver");
	#endif
	genericDriverInit(params.WindowSize, params.Stencilbuffer);
	yViewPortOffset = 0;
}
#endif

// -----------------------------------------------------------------------
// LINUX CONSTRUCTOR
// -----------------------------------------------------------------------
#ifdef _IRR_USE_LINUX_DEVICE_
//! Linux constructor and init code
COpenGLDriver::COpenGLDriver(const SIrrlichtCreationParameters& params,
		io::IFileSystem* io)
: CNullDriver(io, params.WindowSize), COpenGLExtensionHandler(),
	CurrentRenderMode(ERM_NONE), ResetRenderStates(true),
	Transformation3DChanged(true), AntiAlias(params.AntiAlias), IsTextureMatrixIdentity(-1),
	RenderTargetTexture(0), LastSetLight(-1), CurrentRendertargetSize(0,0), ColorFormat(ECF_R8G8B8)
{
	#ifdef _DEBUG
	setDebugName("COpenGLDriver");
	#endif
	ExposedData.OpenGLLinux.X11Context = glXGetCurrentContext();
	ExposedData.OpenGLLinux.X11Display = glXGetCurrentDisplay();
	ExposedData.OpenGLLinux.X11Window = (unsigned long)params.WindowId;
	Drawable = glXGetCurrentDrawable();

	genericDriverInit(params.WindowSize, params.Stencilbuffer);

	// set vsync
#ifdef GLX_SGI_swap_control
#ifdef _IRR_OPENGL_USE_EXTPOINTER_
	if (params.Vsync && glxSwapIntervalSGI)
		glxSwapIntervalSGI(1);
#else
	if (params.Vsync)
		glXSwapIntervalSGI(1);
#endif
#endif
}

#endif // _IRR_USE_LINUX_DEVICE_


// -----------------------------------------------------------------------
// SDL CONSTRUCTOR
// -----------------------------------------------------------------------
#ifdef _IRR_USE_SDL_DEVICE_
//! SDL constructor and init code
COpenGLDriver::COpenGLDriver(const SIrrlichtCreationParameters& params,
		io::IFileSystem* io)
: CNullDriver(io, params.WindowSize), COpenGLExtensionHandler(),
	CurrentRenderMode(ERM_NONE), ResetRenderStates(true),
	Transformation3DChanged(true), AntiAlias(params.AntiAlias), IsTextureMatrixIdentity(-1),
	RenderTargetTexture(0), LastSetLight(-1), CurrentRendertargetSize(0,0), ColorFormat(ECF_R8G8B8)
{
	#ifdef _DEBUG
	setDebugName("COpenGLDriver");
	#endif

	genericDriverInit(params.WindowSize, params.Stencilbuffer);
}

#endif // _IRR_USE_SDL_DEVICE_


//! destructor
COpenGLDriver::~COpenGLDriver()
{
	deleteMaterialRenders();

	// I get a blue screen on my laptop, when I do not delete the
	// textures manually before releasing the dc. Oh how I love this.

	deleteAllTextures();

#if defined(_IRR_COMPILE_WITH_OPENGL_ES2_) && !defined(_IRR_USE_IPHONEOS_DEVICE_)
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglTerminate(eglDisplay);

#ifdef _IRR_USE_WINDOWS_DEVICE_
	if (HDc)
		ReleaseDC(Window, HDc);
#endif
#else
#ifdef _IRR_USE_WINDOWS_DEVICE_
	if (HRc)
	{
		if (!wglMakeCurrent(0, 0))
			os::Printer::log("Release of dc and rc failed.", ELL_WARNING);

		if (!wglDeleteContext(HRc))
			os::Printer::log("Release of rendering context failed.", ELL_WARNING);
	}

	if (HDc)
		ReleaseDC(Window, HDc);
#endif
#endif
}

// -----------------------------------------------------------------------
// METHODS
// -----------------------------------------------------------------------

bool COpenGLDriver::genericDriverInit(const core::dimension2d<s32>& screenSize, bool stencilBuffer)
{
	Name=L"OpenGL ";
	Name.append(glGetString(GL_VERSION));
	s32 pos=Name.findNext(L' ', 7);
	if (pos != -1)
		Name=Name.subString(0, pos);
	printVersion();

	// print renderer information
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	if (renderer && vendor)
	{
		os::Printer::log(reinterpret_cast<const c8*>(renderer), reinterpret_cast<const c8*>(vendor), ELL_INFORMATION);
		vendorName = reinterpret_cast<const c8*>(vendor);
	}

	NeedForceCommitTexture = false;

	u32 i;
	for (i=0; i<MATERIAL_MAX_TEXTURES; ++i)
		CurrentTexture[i]=0;
	// load extensions
	initExtensions(stencilBuffer);
	/*if (queryFeature(EVDF_ARB_GLSL))
	{
		char buf[32];
		const u32 maj = ShaderLanguageVersion/100;
		snprintf(buf, 32, "%u.%u", maj, ShaderLanguageVersion-maj*100);
		os::Printer::log("GLSL version", buf, ELL_INFORMATION);
	}
	else
		os::Printer::log("GLSL not available.", ELL_INFORMATION);*/

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
    
    LandscapeMatrix.setRotationDegrees(irr::core::vector3df(0,0,90));
    LandscapeInverseMatrix.setRotationDegrees(irr::core::vector3df(0,0,-90));

	// Reset The Current Viewport
#ifdef _IRR_USE_IPHONEOS_DEVICE_
    glViewport(0, yViewPortOffset, screenSize.Height, screenSize.Width);
#else
	glViewport(0, yViewPortOffset, screenSize.Width, screenSize.Height);
#endif
    
// This needs an SMaterial flag to enable/disable later on, but should become default sometimes
	/*glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	setAmbientLight(SColorf(0.0f,0.0f,0.0f,0.0f));
#ifdef GL_EXT_separate_specular_color
	if (FeatureAvailable[IRR_EXT_separate_specular_color])
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
#endif
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);*/

// This is a fast replacement for NORMALIZE_NORMALS
//	if ((Version>101) || FeatureAvailable[IRR_EXT_rescale_normal])
//		glEnable(GL_RESCALE_NORMAL_EXT);

#ifdef _IRR_COMPILE_WITH_OPENGL_ES2_
	glClearDepthf(1.0f);
#else
	glClearDepth(1.0);
#endif

#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif

	glDepthFunc(GL_LEQUAL);
#ifdef _IRR_USE_RIGHT_HAND_CONVENTION_
	glFrontFace( GL_CCW );
#else
	glFrontFace( GL_CW );
#endif

#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
	if (AntiAlias)
	{
		if (MultiSamplingExtension)
			glEnable(GL_MULTISAMPLE_ARB);

		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
	}
#endif

// currently disabled, because often in software, and thus very slow
//	glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
//	glEnable(GL_POINT_SMOOTH);

	/*UserClipPlane.reallocate(MaxUserClipPlanes);
	UserClipPlaneEnabled.reallocate(MaxUserClipPlanes);
	for (i=0; i<MaxUserClipPlanes; ++i)
	{
		UserClipPlane.push_back(core::plane3df());
		UserClipPlaneEnabled.push_back(false);
	}*/

	// create material renderers
	createMaterialRenderers();

	// set the renderstates
	setRenderStates3DMode();

	//glAlphaFunc(GL_GREATER, 0.f);

	// set fog mode
	//setFog(FogColor, LinearFog, FogStart, FogEnd, FogDensity, PixelFog, RangeFog);

	// create matrix for flipping textures
	TextureFlipMatrix.buildTextureTransform(0.0f, core::vector2df(0,0), core::vector2df(0,1.0f), core::vector2df(1.0f,-1.0f));

	// init soft tex gen
	/*TexGenEnabled = 0;
	for (int i = 0; i < MATERIAL_MAX_TEXTURES; ++i)
	{
		TexGenType[i] = ETGT_NONE;
	}*/

	return true;
}


void COpenGLDriver::createMaterialRenderers()
{
	// create OpenGL material renderers

	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_SOLID(this));
	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_SOLID_2_LAYER(this));
	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_SOLID_2_LAYER_DECAL(this));

	// add the same renderer for all lightmap types
	COpenGLMaterialRenderer_LIGHTMAP* lmr = irrnew COpenGLMaterialRenderer_LIGHTMAP(this);
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP:
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP_ADD:
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP_M2:
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP_M4:
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP_LIGHTING:
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP_LIGHTING_M2:
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP_LIGHTING_M4:
	lmr->drop();

	// add remaining material renderer
	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_DETAIL_MAP(this));
	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_SPHERE_MAP(this));
	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_REFLECTION_2_LAYER(this));
	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_TRANSPARENT_ADD_COLOR(this));
	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL(this));
	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF(this));
	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA(this));
	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER(this));

	// add normal map renderers
	s32 tmp = 0;
	video::IMaterialRenderer* renderer = 0;
	renderer = irrnew COpenGLNormalMapRenderer(this, tmp, MaterialRenderers[EMT_SOLID].Renderer);
	renderer->drop();
	renderer = irrnew COpenGLNormalMapRenderer(this, tmp, MaterialRenderers[EMT_TRANSPARENT_ADD_COLOR].Renderer);
	renderer->drop();
	renderer = irrnew COpenGLNormalMapRenderer(this, tmp, MaterialRenderers[EMT_TRANSPARENT_VERTEX_ALPHA].Renderer);
	renderer->drop();

	// add parallax map renderers
	renderer = irrnew COpenGLParallaxMapRenderer(this, tmp, MaterialRenderers[EMT_SOLID].Renderer);
	renderer->drop();
	renderer = irrnew COpenGLParallaxMapRenderer(this, tmp, MaterialRenderers[EMT_TRANSPARENT_ADD_COLOR].Renderer);
	renderer->drop();
	renderer = irrnew COpenGLParallaxMapRenderer(this, tmp, MaterialRenderers[EMT_TRANSPARENT_VERTEX_ALPHA].Renderer);
	renderer->drop();

	// add basic 1 texture blending
	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_ONETEXTURE_BLEND(this));
	
	// add transpareny texture
	addAndDropMaterialRenderer(irrnew COpenGLMaterialRenderer_TRANSPARENT_TEXTURE_VERTEX_ALPHA(this));
}


//! presents the rendered scene on the screen, returns false if failed
bool COpenGLDriver::endScene()
{
	CNullDriver::endScene();

#ifdef _IRR_COMPILE_WITH_OPENGL_ES2_
	#ifdef _IRR_USE_IPHONEOS_DEVICE_
	#else
		eglSwapBuffers(eglDisplay, eglSurface);
	#endif
	return true;
#else
	glFlush();
#ifdef _IRR_USE_WINDOWS_DEVICE_
	return SwapBuffers(HDc) == TRUE;
#elif defined(_IRR_USE_LINUX_DEVICE_)
	glXSwapBuffers((Display*)ExposedData.OpenGLLinux.X11Display, Drawable);
	return true;
#elif defined(_IRR_USE_OSX_DEVICE_)
	_device->flush();
	return true;
#elif defined(_IRR_USE_SDL_DEVICE_)
	SDL_GL_SwapBuffers();
	return true;
#else
	return false;
#endif
#endif
}


//! clears the zbuffer
bool COpenGLDriver::beginScene(bool backBuffer, bool zBuffer, SColor color,
		void* windowId, core::rect<s32>* sourceRect)
{
	CNullDriver::beginScene(backBuffer, zBuffer, color, windowId, sourceRect);

	GLbitfield mask = 0;

	if (backBuffer)
	{
		const f32 inv = 1.0f / 255.0f;
		glClearColor(color.getRed() * inv, color.getGreen() * inv,
				color.getBlue() * inv, color.getAlpha() * inv);

		mask |= GL_COLOR_BUFFER_BIT;
	}

	if (zBuffer)
	{
		glDepthMask(GL_TRUE);
		LastMaterial.ZWriteEnable=true;
		mask |= GL_DEPTH_BUFFER_BIT;
	}

	glClear(mask);

	core::matrix4 m1;

	if ( CurrentRendertargetSize.Width == 0 )
		m1.buildProjectionMatrixOrtho(f32(ScreenSize.Width), f32(-ScreenSize.Height), -1.0, 1.0);
	else
		m1.buildProjectionMatrixOrtho(f32(CurrentRendertargetSize.Width), f32(-CurrentRendertargetSize.Height), -1.0, 1.0);

	m1.setTranslation(core::vector3df(-1.0f,1.0f,0));

	core::matrix4 m2;
	m2.setTranslation(core::vector3df(0.375f,0.375f,0));

#ifdef _IRR_USE_IPHONEOS_DEVICE_
    if (!isLeftLandscape())
       OrthoMatrix = LandscapeMatrix * m1 * m2;
    else
       OrthoMatrix = LandscapeInverseMatrix * m1 * m2;
#else
	OrthoMatrix = m1 * m2;
#endif

	return true;
}


bool COpenGLDriver::endScene2D()
{
	_IRR_DEBUG_BREAK_IF(CurrentRenderMode != ERM_2D);
	return true;
}

bool COpenGLDriver::beginScene2D(bool resetStates)
{
	Reset2DRenderStates = resetStates;
	//setTransform ( video::ETS_PROJECTION, core::IdentityMatrix );
	//setTransform ( video::ETS_VIEW, core::IdentityMatrix );
	//setTransform ( video::ETS_WORLD, core::IdentityMatrix );
	setTransform ( video::ETS_TEXTURE_0, core::IdentityMatrix );
	//setTransform ( video::ETS_TEXTURE_1, core::IdentityMatrix );
	//setTransform ( video::ETS_TEXTURE_2, core::IdentityMatrix );
	//setTransform ( video::ETS_TEXTURE_3, core::IdentityMatrix );
	//setRenderStates2DMode(false,false,false, true);
	return true;
}


//! Returns the transformation set by setTransform
const core::matrix4& COpenGLDriver::getTransform(E_TRANSFORMATION_STATE state) const
{
	return Matrices[state];
}

const core::matrix4& COpenGLDriver::getWorldViewProjectionMatrix()
{
	updateMatrix();
	return WorldViewProjectionMatrix;
}

const core::matrix4& COpenGLDriver::getWorldViewMatrix()
{
	updateMatrix();
	return WorldViewMatrix;
}

const core::matrix4& COpenGLDriver::getOrthoMatrix()
{
	return OrthoMatrix;
}
const core::matrix4& COpenGLDriver::getWorldMatrix()
{
	return Matrices[irr::video::ETS_WORLD];
}

const core::matrix4& COpenGLDriver::getViewMatrix()
{
	return Matrices[irr::video::ETS_VIEW];
}
    
void COpenGLDriver::updateMatrix()
{
	if (Transformation3DChanged)
	{
		Transformation3DChanged = false;
		WorldViewMatrix = Matrices[irr::video::ETS_VIEW] * Matrices[irr::video::ETS_WORLD]; 
        
    #ifdef _IRR_USE_IPHONEOS_DEVICE_
        if (!isLeftLandscape())
            WorldViewProjectionMatrix = LandscapeMatrix * Matrices[irr::video::ETS_PROJECTION] * WorldViewMatrix;
        else
            WorldViewProjectionMatrix = LandscapeInverseMatrix * Matrices[irr::video::ETS_PROJECTION] * WorldViewMatrix;
    #else
		WorldViewProjectionMatrix = Matrices[irr::video::ETS_PROJECTION] * WorldViewMatrix;
    #endif
	}
}

//! sets transformation
void COpenGLDriver::setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat)
{
	Matrices[state] = mat;

	switch(state)
	{
	case ETS_VIEW:
	case ETS_WORLD:
		{
			// OpenGL only has a model matrix, view and world is not existent. so lets fake these two.
			//glMatrixMode(GL_MODELVIEW);
			//glLoadMatrixf((Matrices[ETS_VIEW] * Matrices[ETS_WORLD]).pointer());
			// we have to update the clip planes to the latest view matrix
			//for (u32 i=0; i<MaxUserClipPlanes; ++i)
			//	if (UserClipPlaneEnabled[i])
			//		uploadClipPlane(i);

			Transformation3DChanged = true;
		}
		break;
	case ETS_PROJECTION:
		{
			//GLfloat glmat[16];
			//createGLMatrix(glmat, mat);
			//GLfloat* glmat = (GLfloat*)mat.pointer();

			//Orientation3DMatrixTransform(glmat, getOrientation3D());
	
			// flip z to compensate OpenGLs right-hand coordinate system
 			//glmat[12] *= -1.0f;
			//glMatrixMode(GL_PROJECTION);
			//glLoadMatrixf(glmat);
			//glmat[12] *= -1.0f;

			Transformation3DChanged = true;
		}
		break;
	case ETS_TEXTURE_0:
	case ETS_TEXTURE_1:
	case ETS_TEXTURE_2:
	case ETS_TEXTURE_3:
	{
		const u32 i = state - ETS_TEXTURE_0;
		const bool isRTT = Material.getTexture(i) && Material.getTexture(i)->isRenderTarget();

		//if (MultiTextureExtension)
			//extGlActiveTexture(GL_TEXTURE0 + i);

		//glMatrixMode(GL_TEXTURE);
		if (mat.isIdentity() && !isRTT)
		{
			//glLoadIdentity();
			IsTextureMatrixIdentity |= (1 << i);
		}
		else
		{
			//GLfloat glmat[16];
			//if (isRTT)
			//	createGLTextureMatrix(glmat, mat * TextureFlipMatrix);
			//else
			//	createGLTextureMatrix(glmat, mat);

			//glLoadMatrixf(glmat);
			IsTextureMatrixIdentity &= ~(1 << i);
		}
		break;
	}
	default:
		break;
	}
}

bool COpenGLDriver::updateVertexHardwareBuffer(SHWBufferLink_opengl *HWBuffer)
{
	if (!HWBuffer) return false;

	const scene::IMeshBuffer* mb = HWBuffer->MeshBuffer;
	const u32 vertexCount=mb->getVertexCount();
	const E_VERTEX_TYPE vType=mb->getVertexType();

	if (vType == EVT_COMPONENT_ARRAYS)
	{
		const S3DVertexComponentArrays *pVertices = (const S3DVertexComponentArrays *)mb->getVertices();
		const void* vertices = pVertices->pPosition;
		const u32 vertexSize = pVertices->iPositionStride;

		//get or create buffer
		bool newBuffer=false;
		if (!HWBuffer->vbo_verticesID)
		{
			extGlGenBuffers(1, &HWBuffer->vbo_verticesID);
			if (!HWBuffer->vbo_verticesID) return false;
			newBuffer=true;
		}
		else if (HWBuffer->vbo_verticesSize < vertexCount*vertexSize)
		{
			newBuffer=true;
		}

		extGlBindBuffer(GL_ARRAY_BUFFER, HWBuffer->vbo_verticesID );

		//copy data to graphics card
		glGetError(); // clear error storage

		if (!newBuffer)
			extGlBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * vertexSize, vertices);
		else
		{
			HWBuffer->vbo_verticesSize = vertexCount*vertexSize;

			if (HWBuffer->Mapped_Vertex==scene::EHM_STATIC)
				extGlBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, vertices, GL_STATIC_DRAW);
			else if (HWBuffer->Mapped_Vertex==scene::EHM_DYNAMIC)
				extGlBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, vertices, GL_DYNAMIC_DRAW);
			else //scene::EHM_STREAM
				extGlBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, vertices, GL_STREAM_DRAW);
		}

		extGlBindBuffer(GL_ARRAY_BUFFER, 0);

		return (glGetError() == GL_NO_ERROR);

	}
	else
	{
		const void* vertices=mb->getVertices();
		const u32 vertexSize = getVertexPitchFromType(vType);

		//buffer vertex data, and convert colours...
		core::array<c8> buffer(vertexSize * vertexCount);

		memcpy(buffer.pointer(), vertices, vertexSize * vertexCount);

		// in order to convert the colors into opengl format (RGBA)
		switch (vType)
		{
			case EVT_STANDARD:
			{
				S3DVertex* pb = reinterpret_cast<S3DVertex*>(buffer.pointer());
				const S3DVertex* po = static_cast<const S3DVertex*>(vertices);
				for (u32 i=0; i<vertexCount; i++)
				{
					po[i].Color.toOpenGLColor((u8*)&(pb[i].Color.color));
				}
			}
			break;
			case EVT_2TCOORDS:
			{
				S3DVertex2TCoords* pb = reinterpret_cast<S3DVertex2TCoords*>(buffer.pointer());
				const S3DVertex2TCoords* po = static_cast<const S3DVertex2TCoords*>(vertices);
				for (u32 i=0; i<vertexCount; i++)
				{
					po[i].Color.toOpenGLColor((u8*)&(pb[i].Color.color));
				}
			}
			break;
			case EVT_TANGENTS:
			{
				S3DVertexTangents* pb = reinterpret_cast<S3DVertexTangents*>(buffer.pointer());
				const S3DVertexTangents* po = static_cast<const S3DVertexTangents*>(vertices);
				for (u32 i=0; i<vertexCount; i++)
				{
					po[i].Color.toOpenGLColor((u8*)&(pb[i].Color.color));
				}
			}
			break;
			default:
			{
				return false;
			}
		}

		//get or create buffer
		bool newBuffer=false;
		if (!HWBuffer->vbo_verticesID)
		{
			extGlGenBuffers(1, &HWBuffer->vbo_verticesID);
			if (!HWBuffer->vbo_verticesID) return false;
			newBuffer=true;
		}
		else if (HWBuffer->vbo_verticesSize < vertexCount*vertexSize)
		{
			newBuffer=true;
		}

		extGlBindBuffer(GL_ARRAY_BUFFER, HWBuffer->vbo_verticesID );

		//copy data to graphics card
		glGetError(); // clear error storage

		if (!newBuffer)
			extGlBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * vertexSize, buffer.const_pointer());
		else
		{
			HWBuffer->vbo_verticesSize = vertexCount*vertexSize;

			if (HWBuffer->Mapped_Vertex==scene::EHM_STATIC)
				extGlBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, buffer.const_pointer(), GL_STATIC_DRAW);
			else if (HWBuffer->Mapped_Vertex==scene::EHM_DYNAMIC)
				extGlBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, buffer.const_pointer(), GL_DYNAMIC_DRAW);
			else //scene::EHM_STREAM
				extGlBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, buffer.const_pointer(), GL_STREAM_DRAW);
		}

		extGlBindBuffer(GL_ARRAY_BUFFER, 0);

		return (glGetError() == GL_NO_ERROR);
	}
}


bool COpenGLDriver::updateIndexHardwareBuffer(SHWBufferLink_opengl *HWBuffer)
{
	if (!HWBuffer) return false;

	const scene::IMeshBuffer* mb = HWBuffer->MeshBuffer;

	const void* indices=mb->getIndices();
	u32 indexCount= mb->getIndexCount();

	GLenum indexSize;
	switch (mb->getIndexType())
	{
		case (EIT_16BIT):
		{
			indexSize=sizeof(u16);
			break;
		}
		case (EIT_32BIT):
		{
			indexSize=sizeof(u32);
			break;
		}
		default:
		{
			return false;
		}
	}


	//get or create buffer
	bool newBuffer=false;
	if (!HWBuffer->vbo_indicesID)
	{
		extGlGenBuffers(1, &HWBuffer->vbo_indicesID);
		if (!HWBuffer->vbo_indicesID) return false;
		newBuffer=true;
	}
	else if (HWBuffer->vbo_indicesSize < indexCount*indexSize)
	{
		newBuffer=true;
	}

	extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, HWBuffer->vbo_indicesID);

	//copy data to graphics card
	glGetError(); // clear error storage
	if (!newBuffer)
		extGlBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexCount * indexSize, indices);
	else
	{
		HWBuffer->vbo_indicesSize = indexCount*indexSize;

		if (HWBuffer->Mapped_Index==scene::EHM_STATIC)
			extGlBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, indices, GL_STATIC_DRAW);
		else if (HWBuffer->Mapped_Index==scene::EHM_DYNAMIC)
			extGlBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, indices, GL_DYNAMIC_DRAW);
		else //scene::EHM_STREAM
			extGlBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, indices, GL_STREAM_DRAW);
	}

	extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return (glGetError() == GL_NO_ERROR);
}


//! updates hardware buffer if needed
bool COpenGLDriver::updateHardwareBuffer(SHWBufferLink *HWBuffer)
{
	if (!HWBuffer)
		return false;

	if (HWBuffer->Mapped_Vertex!=scene::EHM_NEVER)
	{
		if (HWBuffer->ChangedID_Vertex != HWBuffer->MeshBuffer->getChangedID_Vertex()
			|| !((SHWBufferLink_opengl*)HWBuffer)->vbo_verticesID)
		{

			HWBuffer->ChangedID_Vertex = HWBuffer->MeshBuffer->getChangedID_Vertex();

			if (!updateVertexHardwareBuffer((SHWBufferLink_opengl*)HWBuffer))
				return false;
		}
	}

	if (HWBuffer->Mapped_Index!=scene::EHM_NEVER)
	{
		if (HWBuffer->ChangedID_Index != HWBuffer->MeshBuffer->getChangedID_Index()
			|| !((SHWBufferLink_opengl*)HWBuffer)->vbo_indicesID)
		{

			HWBuffer->ChangedID_Index = HWBuffer->MeshBuffer->getChangedID_Index();

			if (!updateIndexHardwareBuffer((SHWBufferLink_opengl*)HWBuffer))
				return false;
		}
	}

	return true;
}


//! Create hardware buffer from meshbuffer
COpenGLDriver::SHWBufferLink *COpenGLDriver::createHardwareBuffer(const scene::IMeshBuffer* mb)
{
	if (!mb || (mb->getHardwareMappingHint_Index()==scene::EHM_NEVER && mb->getHardwareMappingHint_Vertex()==scene::EHM_NEVER))
		return 0;

	SHWBufferLink_opengl *HWBuffer=irrnew SHWBufferLink_opengl(mb);

	//add to map
	HWBufferMap.insert(HWBuffer->MeshBuffer, HWBuffer);

	HWBuffer->ChangedID_Vertex=HWBuffer->MeshBuffer->getChangedID_Vertex();
	HWBuffer->ChangedID_Index=HWBuffer->MeshBuffer->getChangedID_Index();
	HWBuffer->Mapped_Vertex=mb->getHardwareMappingHint_Vertex();
	HWBuffer->Mapped_Index=mb->getHardwareMappingHint_Index();
	HWBuffer->LastUsed=0;
	HWBuffer->vbo_verticesID=0;
	HWBuffer->vbo_indicesID=0;
	HWBuffer->vbo_verticesSize=0;
	HWBuffer->vbo_indicesSize=0;

	if (!updateHardwareBuffer(HWBuffer))
	{
		deleteHardwareBuffer(HWBuffer);
		return 0;
	}

	return HWBuffer;
}


void COpenGLDriver::deleteHardwareBuffer(SHWBufferLink *_HWBuffer)
{
	if (!_HWBuffer) return;

	SHWBufferLink_opengl *HWBuffer=(SHWBufferLink_opengl*)_HWBuffer;
	if (HWBuffer->vbo_verticesID)
	{
		extGlDeleteBuffers(1, &HWBuffer->vbo_verticesID);
		HWBuffer->vbo_verticesID=0;
	}
	if (HWBuffer->vbo_indicesID)
	{
		extGlDeleteBuffers(1, &HWBuffer->vbo_indicesID);
		HWBuffer->vbo_indicesID=0;
	}

	CNullDriver::deleteHardwareBuffer(_HWBuffer);

}


//! Draw hardware buffer
void COpenGLDriver::drawHardwareBuffer(SHWBufferLink *_HWBuffer)
{
	if (!_HWBuffer)
		return;

	SHWBufferLink_opengl *HWBuffer=(SHWBufferLink_opengl*)_HWBuffer;

	updateHardwareBuffer(HWBuffer); //check if update is needed

	HWBuffer->LastUsed=0;//reset count

	const scene::IMeshBuffer* mb = HWBuffer->MeshBuffer;

	const void *vertices=mb->getVertices();
	const void *indexList=mb->getIndices();
	bool vboFlag = false;

	if (HWBuffer->Mapped_Vertex!=scene::EHM_NEVER)
	{
		extGlBindBuffer(GL_ARRAY_BUFFER, HWBuffer->vbo_verticesID);
		vboFlag = true;
		//vertices=0;
	}

	if (HWBuffer->Mapped_Index!=scene::EHM_NEVER)
	{
		extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, HWBuffer->vbo_indicesID);
		indexList=0;
	}

	drawVertexPrimitiveList(vertices, mb->getVertexCount(), indexList, mb->getIndexCount()/3, mb->getVertexType(), scene::EPT_TRIANGLES, mb->getIndexType(), vboFlag);

	if (HWBuffer->Mapped_Vertex!=scene::EHM_NEVER)
		extGlBindBuffer(GL_ARRAY_BUFFER, 0);

	if (HWBuffer->Mapped_Index!=scene::EHM_NEVER)
		extGlBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// small helper function to create vertex buffer object adress offsets
static inline u8* buffer_offset(const long offset)
{
	return ((u8*)0 + offset);
}

// small helper function to create vertex buffer object adress offsets
static inline u8* buffer_offset(const void* p1, const void* p2)
{
	long int offset = (long)p2 - (long)p1;

	return buffer_offset(offset);
}


void COpenGLDriver::drawVertexPrimitiveList(const void* vertices, u32 vertexCount,
		const void* indexList, u32 primitiveCount,
		E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)
{
	drawVertexPrimitiveList(vertices, vertexCount, indexList, primitiveCount, vType, pType, iType, false);
}

const int VertexComponentTypeMap[] =
{
	GL_BYTE,			//CT_BYTE = 0,
	GL_UNSIGNED_BYTE,	//CT_UNSIGNED_BYTE = 1,
	GL_SHORT,			//CT_SHORT = 2,
	GL_UNSIGNED_SHORT,	//CT_UNSIGNED_SHORT = 3,
	GL_INT,				//CT_INT = 4,
	GL_UNSIGNED_INT,	//CT_UNSIGNED_INT = 5,
	GL_FLOAT,			//CT_FLOAT = 6,
};

//! draws a vertex primitive list
void COpenGLDriver::drawVertexPrimitiveList(const void* vertices, u32 vertexCount,
		const void* indexList, u32 primitiveCount,
		E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType, bool fVBOFlag)
{
	if (!primitiveCount || !vertexCount)
		return;

	if (!checkPrimitiveCount(primitiveCount))
		return;

	CNullDriver::drawVertexPrimitiveList(vertices, vertexCount, indexList, primitiveCount, vType, pType, iType);

	setRenderStates3DMode();

	drawVertexPrimitiveList2d3d(vertices, vertexCount, indexList, primitiveCount, vType, pType, iType, fVBOFlag);
}

//! draws a vertex primitive list
void COpenGLDriver::drawVertexPrimitiveList2d3d(const void* vertices, u32 vertexCount,
		const void* indexList, u32 primitiveCount,
		E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType, bool fVBOFlag)
{
	if (!useHighLevelShader)
	{
		return;
	}

	if(vType != EVT_COMPONENT_ARRAYS)
	{
		if (!fVBOFlag)
		{
			// convert colors to gl color format.
			vertexCount *= 4; //reused as color component count
			ColorBuffer.set_used(vertexCount);
			u32 i;

			switch (vType)
			{
				case EVT_STANDARD:
				{
					const S3DVertex* p = static_cast<const S3DVertex*>(vertices);
					for ( i=0; i<vertexCount; i+=4)
					{
						p->Color.toRGBA8(&ColorBuffer[i]);
						++p;
					}
				}
				break;
				case EVT_2TCOORDS:
				{
					const S3DVertex2TCoords* p = static_cast<const S3DVertex2TCoords*>(vertices);
					for ( i=0; i<vertexCount; i+=4)
					{
						p->Color.toRGBA8(&ColorBuffer[i]);
						++p;
					}
				}
				break;
				case EVT_TANGENTS:
				{
					const S3DVertexTangents* p = static_cast<const S3DVertexTangents*>(vertices);
					for ( i=0; i<vertexCount; i+=4)
					{
						p->Color.toRGBA8(&ColorBuffer[i]);
						++p;
					}
				}
				break;
			}
		}

		//CT GLES2 driver
		if (!useHighLevelShader)
		{
		#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
			glEnableClientState(GL_COLOR_ARRAY);
			if (vertices)
			{
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, &ColorBuffer[0]);
			}
		#endif
		}
		else
		{
			extGlEnableVertexAttribArray(EVA_COLOR);
			if (vertices)
			{
				extGlVertexAttribPointer(EVA_COLOR, 4, GL_UNSIGNED_BYTE, true, 0, &ColorBuffer[0]);
			}
		}
	}

	//CT GLES2 driver
	if (!useHighLevelShader)
	{
	#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
		if (MultiTextureExtension)
			extGlClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_VERTEX_ARRAY);
	#endif
	}
	else
	{
		extGlEnableVertexAttribArray(EVA_POSITION);
	}

	if ((pType!=scene::EPT_POINTS) && (pType!=scene::EPT_POINT_SPRITES))
	{
		//CT GLES2 driver
		if (!useHighLevelShader)
		{
		#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		#endif
		}
		else
		{
			extGlEnableVertexAttribArray(EVA_TCOORD0);
		}
	}

	if ((pType!=scene::EPT_POINTS) && (pType!=scene::EPT_POINT_SPRITES))
	{
		//CT GLES2 driver
		if (!useHighLevelShader)
		{
		#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
			glEnableClientState(GL_NORMAL_ARRAY);
		#endif
		}
		else
		{
			extGlEnableVertexAttribArray(EVA_NORMAL);
		}
	}

	switch (vType)
	{
		case EVT_STANDARD:
			if (!fVBOFlag)
			{
				//CT GLES2 driver
				if (!useHighLevelShader)
				{
				#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
					glNormalPointer(GL_FLOAT, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].Normal);
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].TCoords);
					glVertexPointer(3, GL_FLOAT, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].Pos);
				#endif
				}
				else
				{
					extGlVertexAttribPointer(EVA_NORMAL, 3, GL_FLOAT, false, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].Normal);
					extGlVertexAttribPointer(EVA_TCOORD0, 2, GL_FLOAT, false, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].TCoords);
					extGlVertexAttribPointer(EVA_POSITION, 3, GL_FLOAT, false, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].Pos);
				}
			}
			else
			{
				//CT GLES2 driver
				if (!useHighLevelShader)
				{
				#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
					glNormalPointer(GL_FLOAT, sizeof(S3DVertex), buffer_offset(12));
					glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(S3DVertex), buffer_offset(24));
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), buffer_offset(28));
					glVertexPointer(3, GL_FLOAT, sizeof(S3DVertex), 0);
				#endif
				}
				else
				{
					extGlVertexAttribPointer(EVA_NORMAL, 3, GL_FLOAT, false, sizeof(S3DVertex), buffer_offset(12));
					extGlVertexAttribPointer(EVA_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(S3DVertex), buffer_offset(24));
					extGlVertexAttribPointer(EVA_TCOORD0, 2, GL_FLOAT, false, sizeof(S3DVertex), buffer_offset(28));
					extGlVertexAttribPointer(EVA_POSITION, 3, GL_FLOAT, false, sizeof(S3DVertex), 0);
				}
			}

			if (MultiTextureExtension && CurrentTexture[1])
			{
				//CT GLES2 driver
				if (!useHighLevelShader)
				{
				#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
					extGlClientActiveTexture(GL_TEXTURE1);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					if (vertices)
						glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].TCoords);
					else
						glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), buffer_offset(28));
				#endif
				}
				else
				{
					extGlEnableVertexAttribArray(EVA_TCOORD1);
					if (vertices)
						extGlVertexAttribPointer(EVA_TCOORD1, 2, GL_FLOAT, false, sizeof(S3DVertex), &(static_cast<const S3DVertex*>(vertices))[0].TCoords);
					else
						extGlVertexAttribPointer(EVA_TCOORD1, 2, GL_FLOAT, false, sizeof(S3DVertex), buffer_offset(28));
				}
			}

			break;
		case EVT_2TCOORDS:
			if (!fVBOFlag)
			{
				//CT GLES2 driver
				if (!useHighLevelShader)
				{
				#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
					glNormalPointer(GL_FLOAT, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].Normal);
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].TCoords);
					glVertexPointer(3, GL_FLOAT, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].Pos);
				#endif
				}
				else
				{
					extGlVertexAttribPointer(EVA_NORMAL, 3, GL_FLOAT, false, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].Normal);
					extGlVertexAttribPointer(EVA_TCOORD0, 2, GL_FLOAT, false, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].TCoords);
					extGlVertexAttribPointer(EVA_POSITION, 3, GL_FLOAT, false, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].Pos);
				}
			}
			else
			{
				//CT GLES2 driver
				if (!useHighLevelShader)
				{
				#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
					glNormalPointer(GL_FLOAT, sizeof(S3DVertex2TCoords), buffer_offset(12));
					glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(S3DVertex2TCoords), buffer_offset(24));
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), buffer_offset(28));
					glVertexPointer(3, GL_FLOAT, sizeof(S3DVertex2TCoords), buffer_offset(0));
				#endif
				}
				else
				{
					extGlVertexAttribPointer(EVA_NORMAL, 3, GL_FLOAT, false, sizeof(S3DVertex2TCoords), buffer_offset(12));
					extGlVertexAttribPointer(EVA_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(S3DVertex2TCoords), buffer_offset(24));
					extGlVertexAttribPointer(EVA_TCOORD0, 2, GL_FLOAT, false, sizeof(S3DVertex2TCoords), buffer_offset(28));
					extGlVertexAttribPointer(EVA_POSITION, 3, GL_FLOAT, false, sizeof(S3DVertex2TCoords), buffer_offset(0));
				}
			}

			if (MultiTextureExtension)
			{
				//CT GLES2 driver
				if (!useHighLevelShader)
				{
				#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
					extGlClientActiveTexture(GL_TEXTURE1);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					if (vertices)
						glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].TCoords2);
					else
						glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), buffer_offset(36));
				#endif
				}
				else
				{
					extGlEnableVertexAttribArray(EVA_TCOORD1);
					if (vertices)
						extGlVertexAttribPointer(EVA_TCOORD1, 2, GL_FLOAT, false, sizeof(S3DVertex2TCoords), &(static_cast<const S3DVertex2TCoords*>(vertices))[0].TCoords2);
					else
						extGlVertexAttribPointer(EVA_TCOORD1, 2, GL_FLOAT, false, sizeof(S3DVertex2TCoords), buffer_offset(36));
				}
			}
			break;
		case EVT_TANGENTS:
			if (!fVBOFlag)
			{
				//CT GLES2 driver
				if (!useHighLevelShader)
				{
				#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
					glNormalPointer(GL_FLOAT, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Normal);
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].TCoords);
					glVertexPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Pos);
				#endif
				}
				else
				{
					extGlVertexAttribPointer(EVA_NORMAL, 3, GL_FLOAT, false, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Normal);
					extGlVertexAttribPointer(EVA_TCOORD0, 2, GL_FLOAT, false, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].TCoords);
					extGlVertexAttribPointer(EVA_POSITION, 3, GL_FLOAT, false, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Pos);
				}
			}
			else
			{
				//CT GLES2 driver
				if (!useHighLevelShader)
				{
				#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
					glNormalPointer(GL_FLOAT, sizeof(S3DVertexTangents), buffer_offset(12));
					glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(S3DVertexTangents), buffer_offset(24));
					glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertexTangents), buffer_offset(28));
					glVertexPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), buffer_offset(0));
				#endif
				}
				else
				{
					extGlVertexAttribPointer(EVA_NORMAL, 3, GL_FLOAT, false, sizeof(S3DVertexTangents), buffer_offset(12));
					extGlVertexAttribPointer(EVA_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(S3DVertexTangents), buffer_offset(24));
					extGlVertexAttribPointer(EVA_TCOORD0, 2, GL_FLOAT, false, sizeof(S3DVertexTangents), buffer_offset(28));
					extGlVertexAttribPointer(EVA_POSITION, 3, GL_FLOAT, false, sizeof(S3DVertexTangents), buffer_offset(0));
				}
			}

			if (MultiTextureExtension)
			{
				//CT GLES2 driver
				if (!useHighLevelShader)
				{
				#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
					extGlClientActiveTexture(GL_TEXTURE1);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					if (vertices)
						glTexCoordPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Tangent);
					else
						glTexCoordPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), buffer_offset(36));

					extGlClientActiveTexture(GL_TEXTURE2);
					glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
					if (vertices)
						glTexCoordPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Binormal);
					else
						glTexCoordPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), buffer_offset(48));
				#endif
				}
				else
				{
					extGlEnableVertexAttribArray(EVA_TANGENT);
					extGlEnableVertexAttribArray(EVA_BINORMAL);

					if (vertices)
					{
						extGlVertexAttribPointer(EVA_TANGENT, 3, GL_FLOAT, false, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Tangent);
						extGlVertexAttribPointer(EVA_BINORMAL, 3, GL_FLOAT, false, sizeof(S3DVertexTangents), &(static_cast<const S3DVertexTangents*>(vertices))[0].Binormal);
					}
					else
					{
						extGlVertexAttribPointer(EVA_TANGENT, 3, GL_FLOAT, false, sizeof(S3DVertexTangents), buffer_offset(36));
						extGlVertexAttribPointer(EVA_BINORMAL, 3, GL_FLOAT, false, sizeof(S3DVertexTangents), buffer_offset(48));
					}
				}
			}
			break;
		case EVT_COMPONENT_ARRAYS:
			if (!fVBOFlag)
			{
				const S3DVertexComponentArrays *pVertices = (const S3DVertexComponentArrays *)vertices;

				//CT remove unused code
				/*if(pVertices->iPositionType != S3DVertexComponentArrays::ECT_FLOAT)
				{
					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
					const core::vector3df &scale = *pVertices->getPositionScale();
					const core::vector3df &offset = *pVertices->getPositionOffset();
					const GLfloat transform[] = 
					{	scale.X, 0,0,0,
						0, scale.Y, 0,0,
						0, 0, scale.Z,0,
						offset.X, offset.Y, offset.Z, 1 };
					glMultMatrixf(transform);
				}*/

				//CT GLES2 driver
				if (!useHighLevelShader)
				{
				#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
					glVertexPointer(3, 
						VertexComponentTypeMap[pVertices->iPositionType], 
						pVertices->iPositionStride, 
						pVertices->pPosition);
				#endif
				}
				else
				{
					extGlVertexAttribPointer(EVA_POSITION, 3, GL_FLOAT, false, pVertices->iPositionStride, pVertices->pPosition);
				}

				if(pVertices->pNormal)
				{
					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glEnableClientState(GL_NORMAL_ARRAY);
						glNormalPointer(
							VertexComponentTypeMap[pVertices->iNormalType], 
							pVertices->iNormalStride, 
							pVertices->pNormal);
					#endif
					}
					else
					{
						extGlEnableVertexAttribArray(EVA_NORMAL);
						extGlVertexAttribPointer(EVA_NORMAL, 3, GL_FLOAT, false, pVertices->iNormalStride, pVertices->pNormal);
					}
				}
				else
				{
					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glDisableClientState(GL_NORMAL_ARRAY);
					#endif
					}
					else
					{
						extGlDisableVertexAttribArray(EVA_NORMAL);
					}
				}

				if(pVertices->pTexCoord0)
				{
					//CT remove unused code
					/*if(pVertices->iTexCoord0Type != S3DVertexComponentArrays::ECT_FLOAT)
					{
						glMatrixMode(GL_TEXTURE);
						glPushMatrix();
						const core::vector3df &scale = *pVertices->getTexCoord0Scale();
						const core::vector3df &offset = *pVertices->getTexCoord0Offset();
						const GLfloat transform[] = 
						{	scale.X,	0,0, 0,
							0, scale.Y, 0,0,
							0,0,1,0,
							offset.X, offset.Y, 0, 1 };

						glMultMatrixf(transform);
					}*/

					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(2, 
							VertexComponentTypeMap[pVertices->iTexCoord0Type],
							pVertices->iTexCoord0Stride, 
							pVertices->pTexCoord0);
					#endif
					}
					else
					{
						extGlEnableVertexAttribArray(EVA_TCOORD0);
						extGlVertexAttribPointer(EVA_TCOORD0, 2, GL_FLOAT, false, pVertices->iTexCoord0Stride, pVertices->pTexCoord0);
					}
				}
				else
				{
					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					#endif
					}
					else
					{
						extGlDisableVertexAttribArray(EVA_TCOORD0);
					}
				}

				if(MultiTextureExtension && pVertices->pTexCoord1)
				{
					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						extGlClientActiveTexture(GL_TEXTURE1);
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(2, 
							VertexComponentTypeMap[pVertices->iTexCoord1Type],
							pVertices->iTexCoord1Stride, 
							pVertices->pTexCoord1);
					#endif
					}
					else
					{
						extGlDisableVertexAttribArray(EVA_TCOORD1);
						extGlVertexAttribPointer(EVA_TCOORD1, 2, GL_FLOAT, false, pVertices->iTexCoord1Stride, pVertices->pTexCoord1);
					}
				}
				// is disabled by default
// 				else
// 				{
// 					extGlClientActiveTexture(GL_TEXTURE1);
// 					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
// 				}

				if(pVertices->pColor0)
				{
					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glEnable(GL_COLOR_MATERIAL); 
						glEnableClientState(GL_COLOR_ARRAY);
						glColorPointer(4, 
							VertexComponentTypeMap[pVertices->iColor0Type],
							pVertices->iColor0Stride, 
							pVertices->pColor0);
					#endif
					}
					else
					{
						extGlEnableVertexAttribArray(EVA_COLOR);
						extGlVertexAttribPointer(EVA_COLOR, 4, GL_UNSIGNED_BYTE, true, pVertices->iColor0Stride, pVertices->pColor0);
					}
				}
				else
				{
					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glDisableClientState(GL_COLOR_ARRAY); // This won't
						glDisable(GL_COLOR_MATERIAL);
						setMaterialColor(LastMaterial);
					#endif
					}
					else
					{
						extGlDisableVertexAttribArray(EVA_COLOR);
					}
				}
			}
			else
			{
				const S3DVertexComponentArrays *pVertices = (const S3DVertexComponentArrays *)vertices;

				//CT GLES2 driver
				if (!useHighLevelShader)
				{
				#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
					glVertexPointer(3, 
						VertexComponentTypeMap[pVertices->iPositionType], 
						pVertices->iPositionStride, 
						buffer_offset(0));
				#endif
				}
				else
				{
					extGlVertexAttribPointer(EVA_POSITION, 3, GL_FLOAT, false, pVertices->iPositionStride, buffer_offset(0));
				}
				
				if(pVertices->pNormal)
				{
					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glNormalPointer(
							VertexComponentTypeMap[pVertices->iNormalType], 
							pVertices->iNormalStride, 
							buffer_offset(pVertices->pPosition, pVertices->pNormal));
					#endif
					}
					else
					{
						extGlVertexAttribPointer(EVA_NORMAL, 3, GL_FLOAT, false, pVertices->iNormalStride, buffer_offset(pVertices->pPosition, pVertices->pNormal));
					}
				}
				else
				{
					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glDisableClientState(GL_NORMAL_ARRAY);
					#endif
					}
					else
					{
						extGlDisableVertexAttribArray(EVA_NORMAL);
					}
				}

				if ( pVertices->pTexCoord0 && pVertices->pTexCoord1 )
				{
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glTexCoordPointer(2, 
							VertexComponentTypeMap[pVertices->iTexCoord0Type],
							pVertices->iTexCoord0Stride, 
							buffer_offset(pVertices->pPosition, pVertices->pTexCoord0));						
					#endif
					}
					else
					{
						extGlVertexAttribPointer(EVA_TCOORD0, 2, GL_FLOAT, false, pVertices->iTexCoord0Stride, buffer_offset(pVertices->pPosition, pVertices->pTexCoord0));
						
						extGlEnableVertexAttribArray(EVA_TCOORD1);
						extGlVertexAttribPointer(EVA_TCOORD1, 2, GL_FLOAT, false, pVertices->iTexCoord1Stride, buffer_offset(pVertices->pPosition, pVertices->pTexCoord1));
					}
				}
				else if(pVertices->pTexCoord0)
				{
					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glTexCoordPointer(2, 
							VertexComponentTypeMap[pVertices->iTexCoord0Type],
							pVertices->iTexCoord0Stride, 
							buffer_offset(pVertices->pPosition, pVertices->pTexCoord0));
					#endif
					}
					else
					{
						extGlVertexAttribPointer(EVA_TCOORD0, 2, GL_FLOAT, false, pVertices->iTexCoord0Stride, buffer_offset(pVertices->pPosition, pVertices->pTexCoord0));
					}
				}				
				else
				{
					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					#endif
					}
					else
					{
						extGlDisableVertexAttribArray(EVA_TCOORD0);
					}
				}

				if(pVertices->pColor0)
				{
					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glColorPointer(4, 
							VertexComponentTypeMap[pVertices->iColor0Type],
							pVertices->iColor0Stride, 
							buffer_offset(pVertices->pPosition, pVertices->pColor0));
					#endif
					}
					else
					{
						extGlEnableVertexAttribArray(EVA_COLOR);
						extGlVertexAttribPointer(EVA_COLOR, 4, GL_UNSIGNED_BYTE, true, pVertices->iColor0Stride, buffer_offset(pVertices->pPosition, pVertices->pColor0));
					}
				}
				else
				{
					//CT GLES2 driver
					if (!useHighLevelShader)
					{
					#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
						glDisableClientState(GL_COLOR_ARRAY);
						glDisable(GL_COLOR_MATERIAL);
						glColor4ub(LastMaterial.DiffuseColor.getRed(), 
							LastMaterial.DiffuseColor.getGreen(), 
							LastMaterial.DiffuseColor.getBlue(), 
							LastMaterial.DiffuseColor.getAlpha());
					#endif
					}
					else
					{
						extGlDisableVertexAttribArray(EVA_COLOR);
					}
				}
			}
			break;
	}

	//CT remove unused code
	/*SScopedProcessArray<core::vector2df> texGenBuffer[MATERIAL_MAX_TEXTURES];
	if (TexGenEnabled)
	{
		// TODO: cache view*world
		core::matrix4 modelview(core::matrix4::EM4CONST_NOTHING);
		Matrices[ETS_VIEW].mult34(Matrices[ETS_WORLD], modelview);
		// TODO: try to detect scaling and compute this if there is:
		//core::matrix4 invXPose(modelview,
		//					   core::matrix4::EM4CONST_INVERSE_TRANSPOSED);

		// Note about getting arrays below: glGetPointerv is a query on the
		// client side, so it shouldn't be expensive; query strides is expected
		// to be client side info as well, but
		// glGetIntegerv(GL_*_ARRAY_STRIDE,...) could be more demanding; we
		// should find another way (that would clutter too much the rest of the
		// code)
		core::vector3df* positions = NULL;
		int positionsStride = 0;
		core::vector3df* normals = NULL;
		int normalsStride = 0;

		for (int texGenUnit = 0, texGenEnabled = TexGenEnabled;
			 texGenEnabled;
			 ++texGenUnit)
		{
			const int unitMask = (1 << texGenUnit);
			if (texGenEnabled & unitMask)
			{
				// remove from mask now in case of errors later in the loop
				texGenEnabled &= ~unitMask;

				texGenBuffer[texGenUnit].reset(vertexCount);
				if (!texGenBuffer[texGenUnit].get())
				{
					os::Printer::log("COpenGLESDriver::drawVertexPrimitiveList",
									 " failed to allocate process buffer for "
									 "tex gen",
									 ELL_ERROR);
					continue;
				}

				switch (TexGenType[texGenUnit])
				{
					case ETGT_SPHERE_MAP :
					{
						// Note: setting positions and all this way seems
						// overkill, but this is just in prevision of future
						// generation methods that could use the same with more
						// than one texture unit; we probably should do
						// something similar with the modelview matrix
						if (!positions)
						{
							GLvoid* pointer;
							glGetPointerv(GL_VERTEX_ARRAY_POINTER, &pointer);
							positions = reinterpret_cast<core::vector3df*>(pointer);
							glGetIntegerv(GL_VERTEX_ARRAY_STRIDE, &positionsStride);
						}
						if (!normals)
						{
							if (!glIsEnabled(GL_NORMAL_ARRAY))
							{
								os::Printer::log(
									"COpenGLESDriver::drawVertexPrimitiveList",
									" requesting sphere map tex gen with no "
									"normals",
									ELL_ERROR
								);
								texGenBuffer[texGenUnit].reset();
								continue;
							}
							GLvoid* pointer;
							glGetPointerv(GL_NORMAL_ARRAY_POINTER, &pointer);
							normals = reinterpret_cast<core::vector3df*>(pointer);
							glGetIntegerv(GL_NORMAL_ARRAY_STRIDE, &normalsStride);
						}
						
						// TODO: check if it would be better to parse indices
						computeSphereMapTexCoords(modelview,
												  0,
												  vertexCount,
												  positions,
												  positionsStride,
												  normals,
												  normalsStride,
												  texGenBuffer[texGenUnit].get(),
												  sizeof(core::vector2df),
												  getOrientation3D());
					}
					break;

					case ETGT_SIMPLE_SPHERE_MAP :
					{
						if (!normals)
						{
							if (!glIsEnabled(GL_NORMAL_ARRAY))
							{
								os::Printer::log(
									"COpenGLESDriver::drawVertexPrimitiveList",
									" requesting simple sphere map tex gen with no "
									"normals",
									ELL_ERROR
								);
								texGenBuffer[texGenUnit].reset();
								continue;
							}
							GLvoid* pointer;
							glGetPointerv(GL_NORMAL_ARRAY_POINTER, &pointer);
							normals = reinterpret_cast<core::vector3df*>(pointer);
							glGetIntegerv(GL_NORMAL_ARRAY_STRIDE, &normalsStride);
						}

						// TODO: check if it would be better to parse indices
						computeSimpleSphereMapTexCoords(modelview,
														vertexCount,
														normals,
														normalsStride,
														texGenBuffer[texGenUnit].get(),
														sizeof(core::vector2df),
														false,
														getOrientation3D());
					}
					break;

					// add more as required
				}
				if (MultiTextureExtension)
				{
					extGlClientActiveTexture(GL_TEXTURE0 + texGenUnit);
				}
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2,
								  GL_FLOAT,
								  sizeof(core::vector2df),
								  texGenBuffer[texGenUnit].get());
			}
		}
	}*/

	GLenum indexSize=0;

	switch (iType)
	{
		case (EIT_16BIT):
		{
			indexSize=GL_UNSIGNED_SHORT;
			break;
		}
		case (EIT_32BIT):
		{
			indexSize=GL_UNSIGNED_INT;
			break;
		}
	}

	switch (pType)
	{
		case scene::EPT_POINTS:
		case scene::EPT_POINT_SPRITES:
		{
#ifdef GL_ARB_point_sprite
			if (pType==scene::EPT_POINT_SPRITES && FeatureAvailable[IRR_ARB_point_sprite])
				glEnable(GL_POINT_SPRITE_ARB);
#endif
			//CT remove unused code
			/*float quadratic[] = {0.0f, 0.0f, 10.01f};
			extGlPointParameterfv(GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic);
			float maxParticleSize=1.0f;
			glGetFloatv(GL_POINT_SIZE_MAX_ARB, &maxParticleSize);
//			maxParticleSize=maxParticleSize<Material.Thickness?maxParticleSize:Material.Thickness;
//			extGlPointParameterf(GL_POINT_SIZE_MAX_ARB,maxParticleSize);
//			extGlPointParameterf(GL_POINT_SIZE_MIN_ARB,Material.Thickness);
			extGlPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f);
			glPointSize(Material.Thickness);*/
#ifdef GL_ARB_point_sprite
			if (pType==scene::EPT_POINT_SPRITES && FeatureAvailable[IRR_ARB_point_sprite])
				glTexEnvf(GL_POINT_SPRITE_ARB,GL_COORD_REPLACE, GL_TRUE);
#endif
			glDrawArrays(GL_POINTS, 0, primitiveCount);
#ifdef GL_ARB_point_sprite
			if (pType==scene::EPT_POINT_SPRITES && FeatureAvailable[IRR_ARB_point_sprite])
			{
				glDisable(GL_POINT_SPRITE_ARB);
				glTexEnvf(GL_POINT_SPRITE_ARB,GL_COORD_REPLACE, GL_FALSE);
			}
#endif
		}
			break;
		case scene::EPT_LINE_STRIP:
			glDrawElements(GL_LINE_STRIP, primitiveCount+1, indexSize, indexList);
			break;
		case scene::EPT_LINE_LOOP:
			glDrawElements(GL_LINE_LOOP, primitiveCount, indexSize, indexList);
			break;
		case scene::EPT_LINES:
			glDrawElements(GL_LINES, primitiveCount*2, indexSize, indexList);
			break;
		case scene::EPT_TRIANGLE_STRIP:
			glDrawElements(GL_TRIANGLE_STRIP, primitiveCount+2, indexSize, indexList);
			break;
		case scene::EPT_TRIANGLE_FAN:
			glDrawElements(GL_TRIANGLE_FAN, primitiveCount+2, indexSize, indexList);
			break;
		case scene::EPT_TRIANGLES:
			glDrawElements(GL_TRIANGLES, primitiveCount*3, indexSize, indexList);
			break;
		//CT remove unused code
		/*case scene::EPT_QUAD_STRIP:
			glDrawElements(GL_QUAD_STRIP, primitiveCount*2+2, indexSize, indexList);
			break;
		case scene::EPT_QUADS:
			glDrawElements(GL_QUADS, primitiveCount*4, indexSize, indexList);
			break;
		case scene::EPT_POLYGON:
			glDrawElements(GL_POLYGON, primitiveCount, indexSize, indexList);
			break;*/
	}

	if (MultiTextureExtension)
	{
		if (vType==EVT_TANGENTS)
		{
			//CT GLES2 driver
			if (!useHighLevelShader)
			{
			#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
				extGlClientActiveTexture(GL_TEXTURE2);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			#endif
			}
			else
			{
				extGlDisableVertexAttribArray(EVA_TANGENT);
				extGlDisableVertexAttribArray(EVA_BINORMAL);
			}
		}

		if ((vType!=EVT_STANDARD) || CurrentTexture[1])
		{
			//CT GLES2 driver
			if (!useHighLevelShader)
			{
			#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
				extGlClientActiveTexture(GL_TEXTURE1);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			#endif
			}
			else
			{
				extGlDisableVertexAttribArray(EVA_TCOORD1);
			}
		}

		//CT GLES2 driver
		if (!useHighLevelShader)
		{
			extGlClientActiveTexture(GL_TEXTURE0);
		}
	}

	if (!useHighLevelShader)
	{
	#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_COLOR_MATERIAL);
	#endif
	}
	else
	{
		extGlDisableVertexAttribArray(EVA_POSITION);
		extGlDisableVertexAttribArray(EVA_NORMAL);
		extGlDisableVertexAttribArray(EVA_COLOR);
		extGlDisableVertexAttribArray(EVA_TCOORD0);
	}
	
	//CT remove unused code
	/*if(vType == EVT_COMPONENT_ARRAYS)
	{
		const S3DVertexComponentArrays *pVertices = (const S3DVertexComponentArrays *)vertices;

		if(pVertices->iPositionType != S3DVertexComponentArrays::ECT_FLOAT)
		{
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}

		if(pVertices->iTexCoord0Type != S3DVertexComponentArrays::ECT_FLOAT)
		{
			glMatrixMode(GL_TEXTURE);
			glPopMatrix();
		}
	}*/
}


#ifdef _IRR_ENABLE_NATIVE_TEXTURE_FORMAT_
#    define _IRR_FLIP_V 1.0f -
#else
#    define _IRR_FLIP_V
#endif

//! draws a 2d image, using a color and the alpha channel of the texture if
//! desired. The image is drawn at pos, clipped against clipRect (if != 0).
//! Only the subtexture defined by sourceRect is used.
void COpenGLDriver::draw2DImage(const video::ITexture* texture,
				const core::position2d<s32>& pos,
				const core::rect<s32>& sourceRect,
				const core::rect<s32>* clipRect, SColor color,
				bool useAlphaChannelOfTexture)
{
	if (!texture)
		return;

	core::rect<s32> poss(pos, core::abs(sourceRect.getSize()));
	const core::dimension2d<s32>& ss = texture->getOriginalSize();
	const f32 invW = 1.f / static_cast<f32>(ss.Width);
	const f32 invH = 1.f / static_cast<f32>(ss.Height);
	core::rect<f32> tcoords((f32)sourceRect.UpperLeftCorner.X * invW,
							(f32)sourceRect.UpperLeftCorner.Y * invH,
							(f32)sourceRect.LowerRightCorner.X * invW,
							(f32)sourceRect.LowerRightCorner.Y * invH);
	if (texture->isRenderTarget())
	{
		tcoords.UpperLeftCorner.Y = 1.0f - tcoords.UpperLeftCorner.Y;
		tcoords.LowerRightCorner.Y = 1.0f - tcoords.LowerRightCorner.Y;
	}

	if (clipRect && !clip(poss, tcoords, *clipRect))
	{
		return;
	}
#ifdef _IRR_ENABLE_DRAW2DIMAGE_MANUAL_VIEWPORT_CLIPPING_
	if (!clip(poss, tcoords, core::rect<s32>(core::position2d<s32>(0,0),
											 getCurrentRenderTargetSize())))
	{
		return;
	}
#endif

	// ok, we've clipped everything.
	// now draw it.

	//disableTextures(1);
	//if (!setTexture(0, texture)) return;
	material2D.setTexture(0, (video::ITexture*)texture);
	setRenderStates2DMode(color.getAlpha()<255, true, useAlphaChannelOfTexture, Reset2DRenderStates);

#ifdef _IRR_WITH_FRAME_STATISTICS_
	++DrawCalls2D;
#endif

	u16 indices[] = {0, 1, 2, 3};
	S3DVertex vertices[4];
	vertices[0] = S3DVertex((f32)poss.UpperLeftCorner.X, (f32)poss.UpperLeftCorner.Y, 0, 0, 0, 1, color, tcoords.UpperLeftCorner.X, tcoords.UpperLeftCorner.Y);
	vertices[1] = S3DVertex((f32)poss.LowerRightCorner.X, (f32)poss.UpperLeftCorner.Y, 0, 0, 0, 1, color, tcoords.LowerRightCorner.X, tcoords.UpperLeftCorner.Y);
	vertices[2] = S3DVertex((f32)poss.LowerRightCorner.X, (f32)poss.LowerRightCorner.Y, 0, 0, 0, 1, color, tcoords.LowerRightCorner.X, tcoords.LowerRightCorner.Y);
	vertices[3] = S3DVertex((f32)poss.UpperLeftCorner.X, (f32)poss.LowerRightCorner.Y, 0, 0, 0, 1, color, tcoords.UpperLeftCorner.X, tcoords.LowerRightCorner.Y);
	drawVertexPrimitiveList2d3d(vertices, 4, indices, 2, video::EVT_STANDARD, scene::EPT_TRIANGLE_FAN, EIT_16BIT, false);
}


//! The same, but with a four element array of colors, one for each vertex
void COpenGLDriver::draw2DImage(const video::ITexture* texture, const core::rect<s32>& destRect,
		const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect,
		const video::SColor* const colors, bool useAlphaChannelOfTexture)
{
	if (!texture)
		return;

	core::rect<s32> poss(destRect);
	const core::dimension2d<s32>& ss = texture->getOriginalSize();
	const f32 invW = 1.f / static_cast<f32>(ss.Width);
	const f32 invH = 1.f / static_cast<f32>(ss.Height);
	core::rect<f32> tcoords((f32)sourceRect.UpperLeftCorner.X * invW,
							(f32)sourceRect.UpperLeftCorner.Y * invH,
							(f32)sourceRect.LowerRightCorner.X * invW,
							(f32)sourceRect.LowerRightCorner.Y * invH);
	if (texture->isRenderTarget())
	{
		tcoords.UpperLeftCorner.Y = 1.0f - tcoords.UpperLeftCorner.Y;
		tcoords.LowerRightCorner.Y = 1.0f - tcoords.LowerRightCorner.Y;
	}
	SColor useColor[4] =
	{
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF
	};
	if (colors)
	{
		useColor[0] = colors[0];
		useColor[1] = colors[1];
		useColor[2] = colors[2];
		useColor[3] = colors[3];
	}
	if (clipRect && !clip(poss, tcoords, *clipRect, colors ? &useColor[0] : NULL))
	{
		return;
	}

	// ok, we've clipped everything.
	// now draw it.

	//disableTextures(1);
	//setTexture(0, texture);
	material2D.setTexture(0, (video::ITexture*)texture);

	setRenderStates2DMode(useColor[0].getAlpha()<255 || useColor[1].getAlpha()<255 ||
			useColor[2].getAlpha()<255 || useColor[3].getAlpha()<255,
			true, useAlphaChannelOfTexture, Reset2DRenderStates);

#ifdef _IRR_WITH_FRAME_STATISTICS_
	++DrawCalls2D;
#endif

	u16 indices[] = {0, 1, 2, 3};
	S3DVertex vertices[4];
	vertices[0] = S3DVertex((f32)destRect.UpperLeftCorner.X, (f32)destRect.UpperLeftCorner.Y, 0, 0, 0, 1, useColor[0], tcoords.UpperLeftCorner.X, tcoords.UpperLeftCorner.Y);
	vertices[1] = S3DVertex((f32)destRect.LowerRightCorner.X, (f32)destRect.UpperLeftCorner.Y, 0, 0, 0, 1, useColor[3], tcoords.LowerRightCorner.X, tcoords.UpperLeftCorner.Y);
	vertices[2] = S3DVertex((f32)destRect.LowerRightCorner.X, (f32)destRect.LowerRightCorner.Y, 0, 0, 0, 1, useColor[2], tcoords.LowerRightCorner.X, tcoords.LowerRightCorner.Y);
	vertices[3] = S3DVertex((f32)destRect.UpperLeftCorner.X, (f32)destRect.LowerRightCorner.Y, 0, 0, 0, 1, useColor[1], tcoords.UpperLeftCorner.X, tcoords.LowerRightCorner.Y);
	drawVertexPrimitiveList2d3d(vertices, 4, indices, 2, video::EVT_STANDARD, scene::EPT_TRIANGLE_FAN, EIT_16BIT, false);
}

//! The same, but with a four element array of colors, one for each vertex and an arbitrary transform
void COpenGLDriver::draw2DImage(const video::ITexture* texture, const core::rect<s32>& destRect,
		const core::rect<s32>& sourceRect, const core::matrix4& transform, const core::rect<s32>* clipRect,
		const video::SColor* const colors, bool useAlphaChannelOfTexture)
{
	if (!texture)
		return;

	core::rect<s32> poss(destRect);
	const core::dimension2d<s32>& ss = texture->getOriginalSize();
	const f32 invW = 1.f / static_cast<f32>(ss.Width);
	const f32 invH = 1.f / static_cast<f32>(ss.Height);
	core::rect<f32> tcoords((f32)sourceRect.UpperLeftCorner.X * invW,
							(f32)sourceRect.UpperLeftCorner.Y * invH,
							(f32)sourceRect.LowerRightCorner.X * invW,
							(f32)sourceRect.LowerRightCorner.Y * invH);
	if (texture->isRenderTarget())
	{
		tcoords.UpperLeftCorner.Y = 1.0f - tcoords.UpperLeftCorner.Y;
		tcoords.LowerRightCorner.Y = 1.0f - tcoords.LowerRightCorner.Y;
	}
	SColor useColor[4] =
	{
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF
	};
	if (colors)
	{
		useColor[0] = colors[0];
		useColor[1] = colors[1];
		useColor[2] = colors[2];
		useColor[3] = colors[3];
	}
	if (clipRect && !clip(poss, tcoords, *clipRect, colors ? &useColor[0] : NULL))
	{
		return;
	}

	// ok, we've clipped everything.
	// now draw it.

	//disableTextures(1);
	//setTexture(0, texture);
	material2D.setTexture(0, (video::ITexture*)texture);

	setRenderStates2DMode(useColor[0].getAlpha()<255 || useColor[1].getAlpha()<255 ||
			useColor[2].getAlpha()<255 || useColor[3].getAlpha()<255,
			true, useAlphaChannelOfTexture, Reset2DRenderStates);
	setTransform(ETS_WORLD, transform);

#ifdef _IRR_WITH_FRAME_STATISTICS_
	++DrawCalls2D;
#endif

	u16 indices[] = {0, 1, 2, 3};
	S3DVertex vertices[4];
	vertices[0] = S3DVertex((f32)destRect.UpperLeftCorner.X, (f32)destRect.UpperLeftCorner.Y, 0, 0, 0, 1, useColor[0], tcoords.UpperLeftCorner.X, tcoords.UpperLeftCorner.Y);
	vertices[1] = S3DVertex((f32)destRect.LowerRightCorner.X, (f32)destRect.UpperLeftCorner.Y, 0, 0, 0, 1, useColor[3], tcoords.LowerRightCorner.X, tcoords.UpperLeftCorner.Y);
	vertices[2] = S3DVertex((f32)destRect.LowerRightCorner.X, (f32)destRect.LowerRightCorner.Y, 0, 0, 0, 1, useColor[2], tcoords.LowerRightCorner.X, tcoords.LowerRightCorner.Y);
	vertices[3] = S3DVertex((f32)destRect.UpperLeftCorner.X, (f32)destRect.LowerRightCorner.Y, 0, 0, 0, 1, useColor[1], tcoords.UpperLeftCorner.X, tcoords.LowerRightCorner.Y);
	drawVertexPrimitiveList2d3d(vertices, 4, indices, 2, video::EVT_STANDARD, scene::EPT_TRIANGLE_FAN, EIT_16BIT, false);
}

//! draws a set of 2d images, using a color and the alpha channel of the
//! texture if desired. The images are drawn beginning at pos and concatenated
//! in one line. All drawings are clipped against clipRect (if != 0).
//! The subtextures are defined by the array of sourceRects and are chosen
//! by the indices given.
void COpenGLDriver::draw2DImage(const video::ITexture* texture,
				const core::position2d<s32>& pos,
				const core::array<core::rect<s32> >& sourceRects,
				const core::array<s32>& indices,
				const core::rect<s32>* clipRect, SColor color,
				bool useAlphaChannelOfTexture)
{
	if (!texture) return;

	//disableTextures(1);
	//if (!setTexture(0, texture)) return;
	material2D.setTexture(0, (video::ITexture*)texture);

	setRenderStates2DMode(color.getAlpha()<255, true, useAlphaChannelOfTexture, Reset2DRenderStates);

	if (clipRect)
	{
		if (!clipRect->isValid())
			return;

		glEnable(GL_SCISSOR_TEST);
		const core::dimension2d<s32>& renderTargetSize = getCurrentRenderTargetSize();
		glScissor(clipRect->UpperLeftCorner.X, renderTargetSize.Height-clipRect->LowerRightCorner.Y,
			clipRect->getWidth(),clipRect->getHeight());
	}

	const core::dimension2d<s32>& ss = texture->getOriginalSize();
	core::position2d<s32> targetPos(pos);
	// texcoords need to be flipped horizontally for RTTs
	const bool isRTT = texture->isRenderTarget();
	const f32 invW = 1.f / static_cast<f32>(ss.Width);
	const f32 invH = 1.f / static_cast<f32>(ss.Height);

	for (u32 i=0; i<indices.size(); ++i)
	{
		const s32 currentIndex = indices[i];
		if (!sourceRects[currentIndex].isValid())
			break;

		const core::rect<f32> tcoords(
				sourceRects[currentIndex].UpperLeftCorner.X * invW,
				(isRTT?sourceRects[currentIndex].LowerRightCorner.Y:sourceRects[currentIndex].UpperLeftCorner.Y) * invH,
				sourceRects[currentIndex].LowerRightCorner.X * invW,
				(isRTT?sourceRects[currentIndex].UpperLeftCorner.Y:sourceRects[currentIndex].LowerRightCorner.Y) * invH);

		const core::rect<s32> poss(targetPos, sourceRects[currentIndex].getSize());

#ifdef _IRR_WITH_FRAME_STATISTICS_
		++DrawCalls2D;
#endif

		u16 indices[] = {0, 1, 2, 3};
		S3DVertex vertices[4];
		vertices[0] = S3DVertex((f32)poss.UpperLeftCorner.X, (f32)poss.UpperLeftCorner.Y, 0, 0, 0, 1, color, tcoords.UpperLeftCorner.X, tcoords.UpperLeftCorner.Y);
		vertices[1] = S3DVertex((f32)poss.LowerRightCorner.X, (f32)poss.UpperLeftCorner.Y, 0, 0, 0, 1, color, tcoords.LowerRightCorner.X, tcoords.UpperLeftCorner.Y);
		vertices[2] = S3DVertex((f32)poss.LowerRightCorner.X, (f32)poss.LowerRightCorner.Y, 0, 0, 0, 1, color, tcoords.LowerRightCorner.X, tcoords.LowerRightCorner.Y);
		vertices[3] = S3DVertex((f32)poss.UpperLeftCorner.X, (f32)poss.LowerRightCorner.Y, 0, 0, 0, 1, color, tcoords.UpperLeftCorner.X, tcoords.LowerRightCorner.Y);
		drawVertexPrimitiveList2d3d(vertices, 4, indices, 2, video::EVT_STANDARD, scene::EPT_TRIANGLE_FAN, EIT_16BIT, false);

		targetPos.X += sourceRects[currentIndex].getWidth();
	}
	if (clipRect)
		glDisable(GL_SCISSOR_TEST);
}


//! draw a 2d rectangle
void COpenGLDriver::draw2DRectangle(SColor color, const core::rect<s32>& position,
		const core::rect<s32>* clip)
{
	//disableTextures();
	material2D.setTexture(0, NULL);
	
	setRenderStates2DMode(color.getAlpha() < 255, false, false, Reset2DRenderStates);

	core::rect<s32> pos = position;

	if (clip)
		pos.clipAgainst(*clip);

	if (!pos.isValid())
		return;

#ifdef _IRR_WITH_FRAME_STATISTICS_
	++DrawCalls2D;
#endif

	u16 indices[] = {0, 1, 2, 3};
	S3DVertex vertices[4];
	vertices[0] = S3DVertex((f32)pos.UpperLeftCorner.X, (f32)pos.UpperLeftCorner.Y, 0, 0, 0, 1, color, 0, 0);
	vertices[1] = S3DVertex((f32)pos.LowerRightCorner.X, (f32)pos.UpperLeftCorner.Y, 0, 0, 0, 1, color, 0, 0);
	vertices[2] = S3DVertex((f32)pos.LowerRightCorner.X, (f32)pos.LowerRightCorner.Y, 0, 0, 0, 1, color, 0, 0);
	vertices[3] = S3DVertex((f32)pos.UpperLeftCorner.X, (f32)pos.LowerRightCorner.Y, 0, 0, 0, 1, color, 0, 0);
	drawVertexPrimitiveList2d3d(vertices, 4, indices, 2, video::EVT_STANDARD, scene::EPT_TRIANGLE_FAN, EIT_16BIT, false);
}


//! draw an 2d rectangle
void COpenGLDriver::draw2DRectangle(const core::rect<s32>& position,
			SColor colorLeftUp, SColor colorRightUp, SColor colorLeftDown, SColor colorRightDown,
			const core::rect<s32>* clip)
{
	core::rect<s32> pos = position;

	if (clip)
		pos.clipAgainst(*clip);

	if (!pos.isValid())
		return;

	//disableTextures();
	material2D.setTexture(0, NULL);

	setRenderStates2DMode(colorLeftUp.getAlpha() < 255 ||
		colorRightUp.getAlpha() < 255 ||
		colorLeftDown.getAlpha() < 255 ||
		colorRightDown.getAlpha() < 255, false, false, Reset2DRenderStates);

#ifdef _IRR_WITH_FRAME_STATISTICS_
	++DrawCalls2D;
#endif

	u16 indices[] = {0, 1, 2, 3};
	S3DVertex vertices[4];
	vertices[0] = S3DVertex((f32)pos.UpperLeftCorner.X, (f32)pos.UpperLeftCorner.Y, 0, 0, 0, 1, colorLeftUp, 0, 0);
	vertices[1] = S3DVertex((f32)pos.LowerRightCorner.X, (f32)pos.UpperLeftCorner.Y, 0, 0, 0, 1, colorRightUp, 0, 0);
	vertices[2] = S3DVertex((f32)pos.LowerRightCorner.X, (f32)pos.LowerRightCorner.Y, 0, 0, 0, 1, colorRightDown, 0, 0);
	vertices[3] = S3DVertex((f32)pos.UpperLeftCorner.X, (f32)pos.LowerRightCorner.Y, 0, 0, 0, 1, colorLeftDown, 0, 0);
	drawVertexPrimitiveList2d3d(vertices, 4, indices, 2, video::EVT_STANDARD, scene::EPT_TRIANGLE_FAN, EIT_16BIT, false);
}


//! Draws a 2d line.
void COpenGLDriver::draw2DLine(const core::position2d<s32>& start,
				const core::position2d<s32>& end,
				SColor color)
{
	//disableTextures();
	material2D.setTexture(0, NULL);

	setRenderStates2DMode(color.getAlpha() < 255, false, false, Reset2DRenderStates);

#ifdef _IRR_WITH_FRAME_STATISTICS_
	++DrawCalls2D;
#endif

	u16 indices[] = {0, 1};
	S3DVertex vertices[2];
	vertices[0] = S3DVertex((f32)start.X, (f32)start.Y, 0, 0, 0, 1, color, 0, 0);
	vertices[1] = S3DVertex((f32)end.X, (f32)end.Y, 0, 0, 0, 1, color, 1, 1);
	drawVertexPrimitiveList2d3d(vertices, 2, indices, 1, video::EVT_STANDARD, scene::EPT_LINES, EIT_16BIT, false);
}

#undef _IRR_FLIP_V


bool COpenGLDriver::setTexture(u32 stage, const video::ITexture* texture)
{
	if (stage >= MaxTextureUnits)
		return false;

	if (CurrentTexture[stage]==texture)
		return true;

	if (MultiTextureExtension)
		extGlActiveTexture(GL_TEXTURE0 + stage);

	CurrentTexture[stage]=texture;

	if (!texture)
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
		//glDisable(GL_TEXTURE_2D);
		return true;
	}
	else
	{
		static int oldTexID = -1;

		int bindTex = static_cast<const COpenGLTexture*>(texture)->getOpenGLTextureName();
		if ( bindTex != oldTexID )
		{
			if (texture->getDriverType() != EDT_OPENGL)
			{
				//glDisable(GL_TEXTURE_2D);
				os::Printer::log("Fatal Error: Tried to set a texture not owned by this driver.", ELL_ERROR);
				return false;
			}

	#ifdef _IRR_WITH_FRAME_STATISTICS_
			++TextureBindings;
	#endif

			//glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, bindTex);

			oldTexID = bindTex;
		}
	}
	return true;
}


//! disables all textures beginning with the optional fromStage parameter. Otherwise all texture stages are disabled.
//! Returns whether disabling was successful or not.
bool COpenGLDriver::disableTextures(u32 fromStage)
{
	bool result=true;
	for (u32 i=fromStage; i<MaxTextureUnits; ++i)
		result &= setTexture(i, 0);
	return result;
}


//! creates a matrix in supplied GLfloat array to pass to OpenGL
inline void COpenGLDriver::createGLMatrix(GLfloat gl_matrix[16], const core::matrix4& m)
{
	memcpy(gl_matrix, m.pointer(), 16 * sizeof(f32));
}


//! creates a opengltexturematrix from a D3D style texture matrix
inline void COpenGLDriver::createGLTextureMatrix(GLfloat *o, const core::matrix4& m)
{
	o[0] = m[0];
	o[1] = m[1];
	o[2] = 0.f;
	o[3] = 0.f;

	o[4] = m[4];
	o[5] = m[5];
	o[6] = 0.f;
	o[7] = 0.f;

	o[8] = 0.f;
	o[9] = 0.f;
	o[10] = 1.f;
	o[11] = 0.f;

	o[12] = m[8];
	o[13] = m[9];
	o[14] = 0.f;
	o[15] = 1.f;
}


//! returns a device dependent texture from a software surface (IImage)
video::ITexture* COpenGLDriver::createDeviceDependentTexture(IImage* surface, const char* name)
{
	return irrnew COpenGLTexture(surface, name, this);
}


void COpenGLDriver::setMaterialColor(const SMaterial& material)
{
	//CT remove unused code
	/*GLfloat color[4];

	const f32 inv = 1.0f / 255.0f;

	if(material.Lighting)
	{
		color[0] = material.AmbientColor.getRed() * inv;
		color[1] = material.AmbientColor.getGreen() * inv;
		color[2] = material.AmbientColor.getBlue() * inv;
		color[3] = material.AmbientColor.getAlpha() * inv;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);

		color[0] = material.DiffuseColor.getRed() * inv;
		color[1] = material.DiffuseColor.getGreen() * inv;
		color[2] = material.DiffuseColor.getBlue() * inv;
		color[3] = material.DiffuseColor.getAlpha() * inv;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);

		// disable Specular colors if no shininess is set
		if (material.Shininess != 0.0f)
		{
#ifdef GL_EXT_separate_specular_color
			if (FeatureAvailable[IRR_EXT_separate_specular_color])
				glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
#endif
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.Shininess);
			color[0] = material.SpecularColor.getRed() * inv;
			color[1] = material.SpecularColor.getGreen() * inv;
			color[2] = material.SpecularColor.getBlue() * inv;
			color[3] = material.SpecularColor.getAlpha() * inv;
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
		}
#ifdef GL_EXT_separate_specular_color
		else
			if (FeatureAvailable[IRR_EXT_separate_specular_color])
				glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
#endif

		color[0] = material.EmissiveColor.getRed() * inv;
		color[1] = material.EmissiveColor.getGreen() * inv;
		color[2] = material.EmissiveColor.getBlue() * inv;
		color[3] = material.EmissiveColor.getAlpha() * inv;
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
	}
	else
	{
		glColor4ub(material.DiffuseColor.getRed(), 
		material.DiffuseColor.getGreen(), 
		material.DiffuseColor.getBlue(), 
		material.DiffuseColor.getAlpha());
	}*/
}

//! Sets a material. All 3d drawing functions draw geometry now
//! using this material.
//! \param material: Material to be used from now on.
void COpenGLDriver::setMaterial(const SMaterial& material)
{
	Material = material;

	for (s32 i = MaxTextureUnits-1; i>= 0; --i)
	{
		if (material.hasTextureMatrix(i)
			|| !(IsTextureMatrixIdentity & (1 << i))
			|| (Material.getTexture(i) && Material.getTexture(i)->isRenderTarget()))
		{
			setTransform ((E_TRANSFORMATION_STATE) ( ETS_TEXTURE_0 + i ),
						  material.getTextureMatrix(i));
		}
	}
}


//! prints error if an error happened.
bool COpenGLDriver::testGLError()
{
#ifdef _DEBUG
	GLenum g = glGetError();
	switch(g)
	{
	case GL_NO_ERROR:
		return false;
	case GL_INVALID_ENUM:
		os::Printer::log("GL_INVALID_ENUM", ELL_ERROR); break;
	case GL_INVALID_VALUE:
		os::Printer::log("GL_INVALID_VALUE", ELL_ERROR); break;
	case GL_INVALID_OPERATION:
		os::Printer::log("GL_INVALID_OPERATION", ELL_ERROR); break;
#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
	case GL_STACK_OVERFLOW:
		os::Printer::log("GL_STACK_OVERFLOW", ELL_ERROR); break;
	case GL_STACK_UNDERFLOW:
		os::Printer::log("GL_STACK_UNDERFLOW", ELL_ERROR); break;
#endif
	case GL_OUT_OF_MEMORY:
		os::Printer::log("GL_OUT_OF_MEMORY", ELL_ERROR); break;
#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
	case GL_TABLE_TOO_LARGE:
		os::Printer::log("GL_TABLE_TOO_LARGE", ELL_ERROR); break;
#if defined(GL_EXT_framebuffer_object)
	case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
		os::Printer::log("GL_INVALID_FRAMEBUFFER_OPERATION", ELL_ERROR); break;
#endif
#endif
	};
	return true;
#else
	return false;
#endif
}


//! sets the needed renderstates
void COpenGLDriver::setRenderStates3DMode()
{
	/*if (CurrentRenderMode != ERM_3D)
	{
		// Reset Texture Stages
		glDisable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);

		// switch back the matrices
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((Matrices[ETS_VIEW] * Matrices[ETS_WORLD]).pointer());

		GLfloat glmat[16];
		createGLMatrix(glmat, Matrices[ETS_PROJECTION]);
		//glmat[12] *= -1.0f;
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glmat);

		ResetRenderStates = true;
	}*/

	if ( ResetRenderStates || LastMaterial != Material)
	{
		// unset old material

		if (LastMaterial.MaterialType != Material.MaterialType &&
				static_cast<u32>(LastMaterial.MaterialType) < MaterialRenderers.size())
			MaterialRenderers[LastMaterial.MaterialType].Renderer->OnUnsetMaterial();

		// set new material.
		if (static_cast<u32>(Material.MaterialType) < MaterialRenderers.size())
			MaterialRenderers[Material.MaterialType].Renderer->OnSetMaterial(
				Material, LastMaterial, ResetRenderStates, this);

		LastMaterial = Material;
		ResetRenderStates = false;
	}

	if (static_cast<u32>(Material.MaterialType) < MaterialRenderers.size())
		MaterialRenderers[Material.MaterialType].Renderer->OnRender(this, video::EVT_STANDARD);

	CurrentRenderMode = ERM_3D;
}

void COpenGLDriver::setWrapMode(const SMaterial& material)
{
	// texture address mode
	// Has to be checked always because it depends on the textures
	for (u32 u=0; u<MaxTextureUnits; ++u)
	{
		if (material.TextureLayer[u].Texture == 0)
			continue;
		else if (MultiTextureExtension)
			extGlActiveTexture(GL_TEXTURE0 + u);
		else if (u>0)
			break; // stop loop

		GLint mode=GL_REPEAT;
		switch (material.TextureLayer[u].TextureWrap)
		{
			case ETC_REPEAT:
				mode=GL_REPEAT;
				break;
			case ETC_CLAMP:
				mode=GL_CLAMP_TO_EDGE;
				break;
			case ETC_CLAMP_TO_EDGE:
/*#ifdef GL_VERSION_1_2
				if (Version>101)
					mode=GL_CLAMP_TO_EDGE;
				else
#endif
#ifdef GL_SGIS_texture_edge_clamp
				if (FeatureAvailable[IRR_SGIS_texture_edge_clamp])
					mode=GL_CLAMP_TO_EDGE_SGIS;
				else
#endif
					// fallback*/
					mode=GL_CLAMP_TO_EDGE;
				break;
			case ETC_CLAMP_TO_BORDER:
/*#ifdef GL_VERSION_1_3
				if (Version>102)
					mode=GL_CLAMP_TO_BORDER;
				else
#endif
#ifdef GL_ARB_texture_border_clamp
				if (FeatureAvailable[IRR_ARB_texture_border_clamp])
					mode=GL_CLAMP_TO_BORDER_ARB;
				else
#endif
#ifdef GL_SGIS_texture_border_clamp
				if (FeatureAvailable[IRR_SGIS_texture_border_clamp])
					mode=GL_CLAMP_TO_BORDER_SGIS;
				else
#endif*/
					// fallback
					mode=GL_CLAMP_TO_EDGE;
				break;
			case ETC_MIRROR:
/*#ifdef GL_VERSION_1_4
				if (Version>103)
					mode=GL_MIRRORED_REPEAT;
				else
#endif
#ifdef GL_ARB_texture_border_clamp
				if (FeatureAvailable[IRR_ARB_texture_mirrored_repeat])
					mode=GL_MIRRORED_REPEAT_ARB;
				else
#endif
#ifdef GL_IBM_texture_mirrored_repeat
				if (FeatureAvailable[IRR_IBM_texture_mirrored_repeat])
					mode=GL_MIRRORED_REPEAT_IBM;
				else
#endif*/
					mode=GL_REPEAT;
				break;
		}

		static_cast<COpenGLTexture*>(material.TextureLayer[u].Texture)->setWrap(mode);
	}
}


//! Can be called by an IMaterialRenderer to make its work easier.
void COpenGLDriver::setBasicRenderStates(const SMaterial& material, const SMaterial& lastmaterial,
	bool resetAllRenderStates)
{
	//CT remove unused code
	/*if (resetAllRenderStates ||
		lastmaterial.AmbientColor != material.AmbientColor ||
		lastmaterial.DiffuseColor != material.DiffuseColor ||
		lastmaterial.SpecularColor != material.SpecularColor ||
		lastmaterial.EmissiveColor != material.EmissiveColor ||
		lastmaterial.Shininess != material.Shininess)
	{
		setMaterialColor(material);
	}*/

	// Texture filter
	// Has to be checked always because it depends on the textures
	// Filtering has to be set for each texture layer
	for (u32 i=0; i<MaxTextureUnits; ++i)
	{
		const SMaterialLayer& layer = material.TextureLayer[i];
		if (layer.Texture == 0)
			continue;
		if (MultiTextureExtension)
			extGlActiveTexture(GL_TEXTURE0 + i);
		else if (i>0)
			break;

		_IRR_DEBUG_BREAK_IF(layer.Texture->getDriverType() != EDT_OPENGL);

		COpenGLTexture* texture = static_cast<COpenGLTexture*>(layer.Texture);

		texture->setMagFilter((layer.BilinearFilter || layer.TrilinearFilter)
							  ? GL_LINEAR
							  : GL_NEAREST);

		if (texture->hasMipMaps())
			texture->setMinFilter(
				layer.TrilinearFilter ? GL_LINEAR_MIPMAP_LINEAR :
				layer.BilinearFilter ? GL_LINEAR_MIPMAP_NEAREST :
				GL_NEAREST_MIPMAP_NEAREST
			);
		else
			texture->setMinFilter(
				(layer.BilinearFilter || layer.TrilinearFilter)
				? GL_LINEAR
				: GL_NEAREST
			);

#ifdef GL_EXT_texture_filter_anisotropic
		if (AnisotropyExtension)
			texture->setAnisotropy(layer.AnisotropicFilter
								   ? MaxAnisotropy
								   : 1.0f);
#endif

		GLint mode=GL_REPEAT;
		switch (layer.TextureWrap)
		{
			case ETC_REPEAT:
				mode=GL_REPEAT;
				break;
			case ETC_CLAMP:
				mode=GL_CLAMP_TO_EDGE;
				break;
			case ETC_CLAMP_TO_EDGE:
				mode=GL_CLAMP_TO_EDGE;
				break;
			case ETC_CLAMP_TO_BORDER:
				mode=GL_CLAMP_TO_EDGE;
				break;
			case ETC_MIRROR:
				mode=GL_REPEAT;
				break;
			default:
				break;
		}

		texture->setWrap(mode);
	}

	// fillmode
	//CT remove unused code
	/*if (resetAllRenderStates || (lastmaterial.Wireframe != material.Wireframe) || (lastmaterial.PointCloud != material.PointCloud))
		glPolygonMode(GL_FRONT_AND_BACK, material.Wireframe ? GL_LINE : material.PointCloud? GL_POINT : GL_FILL);*/

	// shademode
	//CT remove unused code
	/*if (resetAllRenderStates || (lastmaterial.GouraudShading != material.GouraudShading))
	{
		if (material.GouraudShading)
			glShadeModel(GL_SMOOTH);
		else
			glShadeModel(GL_FLAT);
	}*/

	// lighting
	//CT remove unused code
	/*if (resetAllRenderStates || (lastmaterial.Lighting != material.Lighting))
	{
		if (material.Lighting)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
	}*/

	// zbuffer
	if (resetAllRenderStates || lastmaterial.ZBuffer != material.ZBuffer)
	{
		switch (material.ZBuffer)
		{
			case 0:
				glDisable(GL_DEPTH_TEST);
				break;
			case 1:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc ( GL_LEQUAL );
				break;
			case 2:
				glEnable(GL_DEPTH_TEST);
				glDepthFunc ( GL_EQUAL );
				break;
		}
	}

	// zwrite
	if (resetAllRenderStates || lastmaterial.ZWriteEnable != material.ZWriteEnable)
	{
		if (material.ZWriteEnable)
		{
			glDepthMask(GL_TRUE);
		}
		else
			glDepthMask(GL_FALSE);
	}

	// back face culling
	if (resetAllRenderStates || (lastmaterial.FrontfaceCulling != material.FrontfaceCulling) || (lastmaterial.BackfaceCulling != material.BackfaceCulling))
	{
		if ((material.FrontfaceCulling) && (material.BackfaceCulling))
		{
			glCullFace(GL_FRONT_AND_BACK);
			glEnable(GL_CULL_FACE);
		}
		else
		if (material.BackfaceCulling)
		{
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
		}
		else
		if (material.FrontfaceCulling)
		{
			glCullFace(GL_FRONT);
			glEnable(GL_CULL_FACE);
		}
		else
			glDisable(GL_CULL_FACE);
	}

	// fog
	//CT remove unused code
	/*if (resetAllRenderStates || lastmaterial.FogEnable != material.FogEnable)
	{
		if (material.FogEnable)
			glEnable(GL_FOG);
		else
			glDisable(GL_FOG);
	}*/

	// normalization
	//CT remove unused code
	/*if (resetAllRenderStates || lastmaterial.NormalizeNormals != material.NormalizeNormals)
	{
		if (material.NormalizeNormals)
			glEnable(GL_NORMALIZE);
		else
			glDisable(GL_NORMALIZE);
	}*/

	// thickness
	//CT remove unused code
	/*if (resetAllRenderStates || lastmaterial.Thickness != material.Thickness)
	{
		//glPointSize(material.Thickness);
		glLineWidth(material.Thickness);
	}*/

	//setWrapMode(material);

	// be sure to leave in texture stage 0
	//if (MultiTextureExtension)
	//	extGlActiveTexture(GL_TEXTURE0);
}

//! sets the needed renderstates
void COpenGLDriver::setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel, bool resetStates)
{
	//if (!resetStates) return;

	material2D.setFlag(irr::video::EMF_LIGHTING, false) ;
	material2D.setFlag(irr::video::EMF_ZWRITE_ENABLE, false);
	material2D.setFlag(irr::video::EMF_ZBUFFER, false);
	material2D.setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
	material2D.setFlag(irr::video::EMF_TEXTURE_WRAP, true);

	if (CurrentRenderMode != ERM_2D /*|| Transformation3DChanged*/)
	{
		// unset last 3d material
		/*if (CurrentRenderMode == ERM_3D)
		{
			if (static_cast<u32>(LastMaterial.MaterialType) < MaterialRenderers.size())
				MaterialRenderers[LastMaterial.MaterialType].Renderer->OnUnsetMaterial();

			SMaterial mat;
			mat.ZBuffer=0;
			mat.Lighting=false;
			mat.TextureLayer[0].BilinearFilter=false;
			mat.MaterialType = (irr::video::E_MATERIAL_TYPE)getRenderer2dMaterialType();

			setBasicRenderStates(mat, mat, true);
			LastMaterial = mat;

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_CULL_FACE);
		}*/

		/*if (resetStates) 
		{
			//glMatrixMode(GL_PROJECTION);
			const core::dimension2d<s32>& renderTargetSize = getCurrentRenderTargetSize();
			core::matrix4 m;
			m.buildProjectionMatrixOrtho(f32(renderTargetSize.Width), f32(-renderTargetSize.Height), -1.0, 1.0);
			m.setTranslation(core::vector3df(-1.0f,1.0f,0));
			setTransform(ETS_PROJECTION, m);
			//glLoadMatrixf(m.pointer());

			//glMatrixMode(GL_MODELVIEW);
			m.makeIdentity();
			m.setTranslation(core::vector3df(0.375f,0.375f,0));
			setTransform(ETS_VIEW, m);
			//glLoadMatrixf(m.pointer());
			
			//glMatrixMode(GL_TEXTURE);
			//glLoadIdentity();
		}*/

		//Transformation3DChanged = false;
	}

	if (!texture)
	{
		material2D.setTexture(0, NULL);
	}

	material2D.MaterialType = (irr::video::E_MATERIAL_TYPE)getImageMaterialType(material2D.getTexture(0));
	setMaterial(material2D);

	/*if (alphaChannel || alpha)
	{
		//glEnable(GL_BLEND);
		//glEnable(GL_ALPHA_TEST);
		//glAlphaFunc(GL_GREATER, 0.f);
		Material.MaterialTypeParam = 0.01f;
	}
	else
	{
		//glDisable(GL_BLEND);
		//glDisable(GL_ALPHA_TEST);
		Material.MaterialTypeParam = 0.0f;
	}
	Material.MaterialTypeParam = 0.0f;*/

	if ( ResetRenderStates || LastMaterial != Material)
	{
		// unset old material

		if (LastMaterial.MaterialType != Material.MaterialType &&
				static_cast<u32>(LastMaterial.MaterialType) < MaterialRenderers.size())
			MaterialRenderers[LastMaterial.MaterialType].Renderer->OnUnsetMaterial();

		// set new material.
		if (static_cast<u32>(Material.MaterialType) < MaterialRenderers.size())
			MaterialRenderers[Material.MaterialType].Renderer->OnSetMaterial(
				Material, LastMaterial, ResetRenderStates, this);

		LastMaterial = Material;
		ResetRenderStates = false;
	}

	if (static_cast<u32>(Material.MaterialType) < MaterialRenderers.size())
		MaterialRenderers[Material.MaterialType].Renderer->OnRender(this, video::EVT_STANDARD);

	/*if (texture)
	{
		// uglyness... what we actually want is to remove 'const' from
		// CurrentTexture, but that implies changes to the API (setTexture)
		// which might not be worth doing for now
		COpenGLTexture* texture = const_cast<COpenGLTexture*>(
			static_cast<const COpenGLTexture*>(CurrentTexture[0])
		);
		texture->setMinFilter(GL_NEAREST);
		texture->setMagFilter(GL_NEAREST);
		texture->setWrap(GL_REPEAT);

		if (alphaChannel)
		{
			// if alpha and alpha texture just modulate, otherwise use only the alpha channel
			if (alpha)
			{
				setTexEnvMode(GL_MODULATE);
			}
			else
			{
				setTexEnvMode(GL_COMBINE_EXT);
				setCombineAlpha(GL_REPLACE);
				setSourceAlpha(0, GL_TEXTURE);
				// rgb always modulates
				setCombineRGB(GL_MODULATE);
				setSourceRGB(0, GL_TEXTURE);
				setSourceRGB(1, GL_PRIMARY_COLOR_EXT);
			}
		}
		else
		{
			if (alpha)
			{
				setTexEnvMode(GL_COMBINE_EXT);
				setCombineAlpha(GL_REPLACE);
				setSourceAlpha(0, GL_PRIMARY_COLOR_EXT);
				// rgb always modulates
				setCombineRGB(GL_MODULATE);
				setSourceRGB(0, GL_TEXTURE);
				setSourceRGB(1, GL_PRIMARY_COLOR_EXT);
			}
			else
			{
				setTexEnvMode(GL_MODULATE);
			}
		}
	}*/

	CurrentRenderMode = ERM_2D;
}


//! \return Returns the name of the video driver.
const wchar_t* COpenGLDriver::getName() const
{
	return Name.c_str();
}


//! deletes all dynamic lights there are
void COpenGLDriver::deleteAllDynamicLights()
{
	//CT remove unused code
	/*for (s32 i=0; i<LastSetLight+1; ++i)
		glDisable(GL_LIGHT0 + i);*/

	LastSetLight = -1;

	CNullDriver::deleteAllDynamicLights();
}


//! adds a dynamic light
void COpenGLDriver::addDynamicLight(const SLight& light)
{
	if (LastSetLight == MaxLights-1)
		return;

	setTransform(ETS_WORLD, core::matrix4());

	++LastSetLight;
	CNullDriver::addDynamicLight(light);

	//CT remove unused code
	/*s32 lidx = GL_LIGHT0 + LastSetLight;
	GLfloat data[4];

	switch (light.Type)
	{
	case video::ELT_SPOT:
		data[0] = light.Direction.X;
		data[1] = light.Direction.Y;
		data[2] = light.Direction.Z;
		data[3] = 0.0f;
		glLightfv(lidx, GL_SPOT_DIRECTION, data);

		// set position
		data[0] = light.Position.X;
		data[1] = light.Position.Y;
		data[2] = light.Position.Z;
		data[3] = 1.0f; // 1.0f for positional light
		glLightfv(lidx, GL_POSITION, data);

		glLightf(lidx, GL_SPOT_EXPONENT, light.Falloff);
		glLightf(lidx, GL_SPOT_CUTOFF, light.OuterCone);
	break;
	case video::ELT_POINT:
		// set position
		data[0] = light.Position.X;
		data[1] = light.Position.Y;
		data[2] = light.Position.Z;
		data[3] = 1.0f; // 1.0f for positional light
		glLightfv(lidx, GL_POSITION, data);

		glLightf(lidx, GL_SPOT_EXPONENT, 0.0f);
		glLightf(lidx, GL_SPOT_CUTOFF, 180.0f);
	break;
	case video::ELT_DIRECTIONAL:
		// set direction
		data[0] = light.Direction.X;
		data[1] = light.Direction.Y;
		data[2] = light.Direction.Z;
		data[3] = 0.0f; // 0.0f for directional light
		glLightfv(lidx, GL_POSITION, data);

		glLightf(lidx, GL_SPOT_EXPONENT, 0.0f);
		glLightf(lidx, GL_SPOT_CUTOFF, 180.0f);
	break;
	}

	// set diffuse color
	data[0] = light.DiffuseColor.r;
	data[1] = light.DiffuseColor.g;
	data[2] = light.DiffuseColor.b;
	data[3] = light.DiffuseColor.a;
	glLightfv(lidx, GL_DIFFUSE, data);

	// set specular color
	data[0] = light.SpecularColor.r;
	data[1] = light.SpecularColor.g;
	data[2] = light.SpecularColor.b;
	data[3] = light.SpecularColor.a;
	glLightfv(lidx, GL_SPECULAR, data);

	// set ambient color
	data[0] = light.AmbientColor.r;
	data[1] = light.AmbientColor.g;
	data[2] = light.AmbientColor.b;
	data[3] = light.AmbientColor.a;
	glLightfv(lidx, GL_AMBIENT, data);

	// 1.0f / (constant + linear * d + quadratic*(d*d);

	// set attenuation
	glLightf(lidx, GL_CONSTANT_ATTENUATION, light.Attenuation.X);
	glLightf(lidx, GL_LINEAR_ATTENUATION, light.Attenuation.Y);
	glLightf(lidx, GL_QUADRATIC_ATTENUATION, light.Attenuation.Z);

	glEnable(lidx);*/
}


//! returns the maximal amount of dynamic lights the device can handle
u32 COpenGLDriver::getMaximalDynamicLightAmount() const
{
	return MaxLights;
}


//! Sets the dynamic ambient light color. The default color is
//! (0,0,0,0) which means it is dark.
//! \param color: New color of the ambient light.
void COpenGLDriver::setAmbientLight(const SColorf& color)
{
	//CT remove unused code
	/*AmbientLight = color;
	GLfloat data[4] = {color.r, color.g, color.b, color.a};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, data);*/
}


    
// this code was sent in by Oliver Klems, thank you! (I modified the glViewport
// method just a bit.
void COpenGLDriver::setViewPort(const core::rect<s32>& area)
{
	core::rect<s32> vp = area;
	core::rect<s32> rendert(0,0, getCurrentRenderTargetSize().Width, getCurrentRenderTargetSize().Height);
	vp.clipAgainst(rendert);
#ifdef IPHONEOS
	if (vp.getHeight()>0 && vp.getWidth()>0)
    {
        s32 x = area.UpperLeftCorner.X;
        s32 y = area.UpperLeftCorner.Y;
        s32 w = area.getWidth();
        s32 h = area.getHeight();
        s32 rh = getCurrentRenderTargetSize().Width;
        s32 rw = getCurrentRenderTargetSize().Height;
        
        if(!isLeftLandscape())
        {
            s32 t =x;
            x=y;
            y=t;
            
            t =w;
            w=h;
            h=t;
        }
        else
        {
            s32 t=x;
            x = rw - y - h;
            y = rh - t - w;
            
            t=w;
            w=h;
            h=t;
        }
        glViewport(x,y,w,h);
    }
#else
    if (vp.getHeight()>0 && vp.getWidth()>0)
		glViewport(vp.UpperLeftCorner.X,
                   getCurrentRenderTargetSize().Height - vp.UpperLeftCorner.Y - vp.getHeight(),
                   vp.getWidth(), vp.getHeight());
#endif
	ViewPort = vp;
}


//! Draws a shadow volume into the stencil buffer. To draw a stencil shadow, do
//! this: First, draw all geometry. Then use this method, to draw the shadow
//! volume. Next use IVideoDriver::drawStencilShadow() to visualize the shadow.
void COpenGLDriver::drawStencilShadowVolume(const core::vector3df* triangles, s32 count, bool zfail)
{
	//CT remove unused code
	/*if (!StencilBuffer || !count)
		return;

	// unset last 3d material
	if (CurrentRenderMode == ERM_3D &&
		static_cast<u32>(Material.MaterialType) < MaterialRenderers.size())
	{
		MaterialRenderers[Material.MaterialType].Renderer->OnUnsetMaterial();
		ResetRenderStates = true;
	}

	// store current OpenGL state
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT |
		GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT);

	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE); // no depth buffer writing
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ); // no color buffer drawing
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.0f, 1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(core::vector3df),&triangles[0]);
	glStencilMask(~0);
	glStencilFunc(GL_ALWAYS, 0, ~0);

	// The first parts are not correctly working, yet.
#if 0
#ifdef GL_EXT_stencil_two_side
	if (FeatureAvailable[IRR_EXT_stencil_two_side])
	{
		glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);
#ifdef GL_NV_depth_clamp
		if (FeatureAvailable[IRR_NV_depth_clamp])
			glEnable(GL_DEPTH_CLAMP_NV);
#endif
		glDisable(GL_CULL_FACE);
		if (!zfail)
		{
			// ZPASS Method

			extGlActiveStencilFace(GL_BACK);
			if (FeatureAvailable[IRR_EXT_stencil_wrap])
				glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP_EXT);
			else
				glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			glStencilMask(~0);
			glStencilFunc(GL_ALWAYS, 0, ~0);

			extGlActiveStencilFace(GL_FRONT);
			if (FeatureAvailable[IRR_EXT_stencil_wrap])
				glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP_EXT);
			else
				glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glStencilMask(~0);
			glStencilFunc(GL_ALWAYS, 0, ~0);

			glDrawArrays(GL_TRIANGLES,0,count);
		}
		else
		{
			// ZFAIL Method

			extGlActiveStencilFace(GL_BACK);
			if (FeatureAvailable[IRR_EXT_stencil_wrap])
				glStencilOp(GL_KEEP, GL_INCR_WRAP_EXT, GL_KEEP);
			else
				glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
			glStencilMask(~0);
			glStencilFunc(GL_ALWAYS, 0, ~0);

			extGlActiveStencilFace(GL_FRONT);
			if (FeatureAvailable[IRR_EXT_stencil_wrap])
			glStencilOp(GL_KEEP, GL_DECR_WRAP_EXT, GL_KEEP);
			else
				glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
			glStencilMask(~0);
			glStencilFunc(GL_ALWAYS, 0, ~0);

			glDrawArrays(GL_TRIANGLES,0,count);
		}
	}
	else
#endif
	if (FeatureAvailable[IRR_ATI_separate_stencil])
	{
		glDisable(GL_CULL_FACE);
		if (!zfail)
		{
			// ZPASS Method

			extGlStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR);
			extGlStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR);
			extGlStencilFuncSeparate(GL_FRONT_AND_BACK, GL_ALWAYS, 0, ~0);
			glStencilMask(~0);

			glDrawArrays(GL_TRIANGLES,0,count);
		}
		else
		{
			// ZFAIL Method

			extGlStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR, GL_KEEP);
			extGlStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR, GL_KEEP);
			extGlStencilFuncSeparate(GL_FRONT_AND_BACK, GL_ALWAYS, 0, ~0);

			glDrawArrays(GL_TRIANGLES,0,count);
		}
	}
	else
#endif
	{
		glEnable(GL_CULL_FACE);
		if (!zfail)
		{
			// ZPASS Method

#ifndef _IRR_USE_RIGHT_HAND_CONVENTION_
			glCullFace(GL_BACK);
#else
			glCullFace(GL_FRONT);
#endif

			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glDrawArrays(GL_TRIANGLES,0,count);

#ifndef _IRR_USE_RIGHT_HAND_CONVENTION_
			glCullFace(GL_FRONT);
#else
			glCullFace(GL_BACK);
#endif
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			glDrawArrays(GL_TRIANGLES,0,count);
		}
		else
		{
			// ZFAIL Method

			glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
#ifndef _IRR_USE_RIGHT_HAND_CONVENTION_
			glCullFace(GL_FRONT);
#else
			glCullFace(GL_BACK);
#endif
			glDrawArrays(GL_TRIANGLES,0,count);

			glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
#ifndef _IRR_USE_RIGHT_HAND_CONVENTION_
			glCullFace(GL_BACK);
#else
			glCullFace(GL_FRONT);
#endif
			glDrawArrays(GL_TRIANGLES,0,count);
		}
	}

#ifndef _IRR_USE_RIGHT_HAND_CONVENTION_
			glCullFace(GL_BACK);
#else
			glCullFace(GL_FRONT);
#endif

#ifdef _IRR_WITH_FRAME_STATISTICS_
	++DrawCalls;
#endif

	glDisableClientState(GL_VERTEX_ARRAY); //not stored on stack
	glPopAttrib();*/
}

void COpenGLDriver::drawStencilShadow(bool clearStencilBuffer, video::SColor leftUpEdge,
	video::SColor rightUpEdge, video::SColor leftDownEdge, video::SColor rightDownEdge)
{
	//CT remove unused code
	/*if (!StencilBuffer)
		return;

	disableTextures();

	// store attributes
	glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT );

	glDisable( GL_LIGHTING );
	glDisable(GL_FOG);
	glDepthMask(GL_FALSE);

	glShadeModel( GL_FLAT );
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable( GL_STENCIL_TEST );
	glStencilFunc(GL_NOTEQUAL, 0, ~0);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

#ifdef _IRR_WITH_FRAME_STATISTICS_
	++DrawCalls;
#endif

	// draw a shadow rectangle covering the entire screen using stencil buffer
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_QUADS);

	glColor4ub (rightDownEdge.getRed(), rightDownEdge.getGreen(), rightDownEdge.getBlue(), rightDownEdge.getAlpha() );
	glVertex3f( 1.1f,-1.1f,0.9f);

	glColor4ub (rightUpEdge.getRed(), rightUpEdge.getGreen(), rightUpEdge.getBlue(), rightUpEdge.getAlpha() );
	glVertex3f( 1.1f, 1.1f,0.9f);

	glColor4ub (leftUpEdge.getRed(), leftUpEdge.getGreen(), leftUpEdge.getBlue(), leftUpEdge.getAlpha() );
	glVertex3f(-1.1f, 1.1f,0.9f);

	glColor4ub (leftDownEdge.getRed(), leftDownEdge.getGreen(), leftDownEdge.getBlue(), leftDownEdge.getAlpha() );
	glVertex3f(-1.1f,-1.1f,0.9f);

	glEnd();

	if (clearStencilBuffer)
		glClear(GL_STENCIL_BUFFER_BIT);

	// restore settings
	glPopMatrix();
	glPopAttrib();*/
}


//! Sets the fog mode.
void COpenGLDriver::setFog(SColor c, bool linearFog, f32 start,
			f32 end, f32 density, bool pixelFog, bool rangeFog)
{
	//CT remove unused code
	/*CNullDriver::setFog(c, linearFog, start, end, density, pixelFog, rangeFog);

	glFogf(GL_FOG_MODE, GLfloat(linearFog ? GL_LINEAR : GL_EXP));
#ifdef GL_EXT_fog_coord
	if (FeatureAvailable[IRR_EXT_fog_coord])
		glFogi(GL_FOG_COORDINATE_SOURCE, GL_FRAGMENT_DEPTH);
#endif

	if(linearFog)
	{
		glFogf(GL_FOG_START, start);
		glFogf(GL_FOG_END, end);
	}
	else
		glFogf(GL_FOG_DENSITY, density);

	if (pixelFog)
		glHint(GL_FOG_HINT, GL_NICEST);
	else
		glHint(GL_FOG_HINT, GL_FASTEST);

	SColorf color(c);
	GLfloat data[4] = {color.r, color.g, color.b, color.a};
	glFogfv(GL_FOG_COLOR, data);*/
}

//! Draws a 3d line.
void COpenGLDriver::draw3DLine(const core::vector3df& start,
				const core::vector3df& end, SColor color)
{
	setRenderStates3DMode();

#ifdef _IRR_WITH_FRAME_STATISTICS_
	++DrawCalls;
#endif

	u16 indices[] = {0, 1};
	S3DVertex vertices[2];
	vertices[0] = S3DVertex(start.X, start.Y, start.Z, 0, 0, 1, color, 0, 0);
	vertices[1] = S3DVertex(end.X, end.Y, end.Z, 0, 0, 1, color, 0, 0);
	drawVertexPrimitiveList2d3d(vertices, 2, indices, 1, video::EVT_STANDARD, scene::EPT_LINES, EIT_16BIT, false);
}

//! Only used by the internal engine. Used to notify the driver that
//! the window was resized.
void COpenGLDriver::OnResize(const core::dimension2d<s32>& size)
{
	CNullDriver::OnResize(size);
	glViewport(0, yViewPortOffset, size.Width, size.Height);
}

void COpenGLDriver::OnResize(const core::rect<s32>& size)
{
	CNullDriver::OnResize(size);
	yViewPortOffset = size.UpperLeftCorner.Y;
	glViewport(size.UpperLeftCorner.X, yViewPortOffset, size.LowerRightCorner.X, size.LowerRightCorner.Y);
}

//! Returns type of video driver
E_DRIVER_TYPE COpenGLDriver::getDriverType() const
{
	return EDT_OPENGL;
}


//! returns color format
ECOLOR_FORMAT COpenGLDriver::getColorFormat() const
{
	return ColorFormat;
}


//! Sets a vertex shader constant.
void COpenGLDriver::setVertexShaderConstant(const f32* data, s32 startRegister, s32 constantAmount)
{
#ifdef GL_ARB_vertex_program
	for (s32 i=0; i<constantAmount; ++i)
		extGlProgramLocalParameter4fv(GL_VERTEX_PROGRAM_ARB, startRegister+i, &data[i*4]);
#endif
}

//! Sets a pixel shader constant.
void COpenGLDriver::setPixelShaderConstant(const f32* data, s32 startRegister, s32 constantAmount)
{
#ifdef GL_ARB_fragment_program
	for (s32 i=0; i<constantAmount; ++i)
		extGlProgramLocalParameter4fv(GL_FRAGMENT_PROGRAM_ARB, startRegister+i, &data[i*4]);
#endif
}

//! Sets a constant for the vertex shader based on a name.
bool COpenGLDriver::setVertexShaderConstant(const c8* name, const f32* floats, int count)
{
	//pass this along, as in GLSL the same routine is used for both vertex and fragment shaders
	return setPixelShaderConstant(name, floats, count);
}

//! Sets a constant for the pixel shader based on a name.
bool COpenGLDriver::setPixelShaderConstant(const c8* name, const f32* floats, int count)
{
	os::Printer::log("Error: Please call services->setPixelShaderConstant(), not VideoDriver->setPixelShaderConstant().");
	return false;
}


//! Adds a new material renderer to the VideoDriver, using pixel and/or
//! vertex shaders to render geometry.
s32 COpenGLDriver::addShaderMaterial(const c8* vertexShaderProgram,
	const c8* pixelShaderProgram,
	IShaderConstantSetCallBack* callback,
	E_MATERIAL_TYPE baseMaterial, s32 userData)
{
	s32 nr = -1;
	COpenGLShaderMaterialRenderer* r = irrnew COpenGLShaderMaterialRenderer(
		this, nr, vertexShaderProgram, pixelShaderProgram,
		callback, getMaterialRenderer(baseMaterial), userData);

	r->drop();
	return nr;
}


//! Adds a new material renderer to the VideoDriver, using GLSL to render geometry.
s32 COpenGLDriver::addHighLevelShaderMaterial(
	const c8* vertexShaderProgram,
	const c8* vertexShaderEntryPointName,
	E_VERTEX_SHADER_TYPE vsCompileTarget,
	const c8* pixelShaderProgram,
	const c8* pixelShaderEntryPointName,
	E_PIXEL_SHADER_TYPE psCompileTarget,
	IShaderConstantSetCallBack* callback,
	E_MATERIAL_TYPE baseMaterial,
	const c8** defines,
	s32 userData)
{
	s32 nr = -1;

	COpenGLSLMaterialRenderer* r = irrnew COpenGLSLMaterialRenderer(
		this, nr, vertexShaderProgram, vertexShaderEntryPointName,
		vsCompileTarget, pixelShaderProgram, pixelShaderEntryPointName, psCompileTarget,
		callback,getMaterialRenderer(baseMaterial), defines, userData);

	r->drop();
	return nr;
}

//! Returns a pointer to the IVideoDriver interface. (Implementation for
//! IMaterialRendererServices)
IVideoDriver* COpenGLDriver::getVideoDriver()
{
	return this;
}


//! Returns pointer to the IGPUProgrammingServices interface.
IGPUProgrammingServices* COpenGLDriver::getGPUProgrammingServices()
{
	return this;
}

ITexture* COpenGLDriver::addRenderTargetTexture(const core::dimension2d<s32>& size, const c8* name)
{
	return addRenderTargetTexture(size, name, true, false);
}

ITexture* COpenGLDriver::addRenderTargetTexture(const core::dimension2d<s32>& size, const c8* name, bool colorTexture, bool depthTexture)
{
	//disable mip-mapping
	bool generateMipLevels = getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

	video::ITexture* rtt = 0;
	if (name==0)
		name="rt";
	// if driver supports FrameBufferObjects, use them
	if (queryFeature(EVDF_FRAMEBUFFER_OBJECT))
	{
		rtt = irrnew COpenGLTexture(size, name, this, false, colorTexture, depthTexture);
		addTexture(rtt);
		rtt->drop();
	}
	else
	{
		rtt = addTexture(size, name, ECF_A8R8G8B8);
		if (rtt)
		{
			static_cast<video::COpenGLTexture*>(rtt)->setIsRenderTarget(true);
		}
	}

	//restore mip-mapping
	setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, generateMipLevels);

	return rtt;
}


//! Returns the maximum amount of primitives (mostly vertices) which
//! the device is able to render with one drawIndexedTriangleList
//! call.
u32 COpenGLDriver::getMaximalPrimitiveCount() const
{
	return 65535;// TODO: Fix all loaders to auto-split and then return the correct value: MaxIndices;
}


//! checks triangle count and print warning if wrong
bool COpenGLDriver::setRenderTarget(video::ITexture* texture, bool clearBackBuffer,
					bool clearZBuffer, SColor color)
{
	// check for right driver type

	if (texture && texture->getDriverType() != EDT_OPENGL)
	{
		os::Printer::log("Fatal Error: Tried to set a texture not owned by this driver.", ELL_ERROR);
		return false;
	}

	// check if we should set the previous RT back

	setTexture(0, 0);
	ResetRenderStates=true;
	if (RenderTargetTexture!=0)
	{
		RenderTargetTexture->unbindRTT();
	}

	if (texture)
	{
		// we want to set a new target. so do this.
		RenderTargetTexture = static_cast<COpenGLTexture*>(texture);
		RenderTargetTexture->bindRTT();
		RenderTargetTexture->setIsRenderTarget(true);
		CurrentRendertargetSize = texture->getSize();
#ifdef _IRR_USE_IPHONEOS_DEVICE_
		glViewport(0,yViewPortOffset,CurrentRendertargetSize.Height,CurrentRendertargetSize.Width);
#else
		glViewport(0,yViewPortOffset,CurrentRendertargetSize.Width,CurrentRendertargetSize.Height);
#endif
	}
	else
	{
#ifdef _IRR_USE_IPHONEOS_DEVICE_
		glViewport(0,yViewPortOffset,ScreenSize.Height,ScreenSize.Width);
#else
		glViewport(0,yViewPortOffset,ScreenSize.Width,ScreenSize.Height);
#endif
		
		RenderTargetTexture = 0;
		CurrentRendertargetSize = core::dimension2d<s32>(0,0);
	}

	GLbitfield mask = 0;
	if (clearBackBuffer)
	{
		const f32 inv = 1.0f / 255.0f;
		glClearColor(color.getRed() * inv, color.getGreen() * inv,
				color.getBlue() * inv, color.getAlpha() * inv);

		mask |= GL_COLOR_BUFFER_BIT;
	}
	if (clearZBuffer)
	{
		glDepthMask(GL_TRUE);
		LastMaterial.ZWriteEnable=true;
		mask |= GL_DEPTH_BUFFER_BIT;
	}

	glClear(mask);

	return true;
}


// returns the current size of the screen or rendertarget
const core::dimension2d<s32>& COpenGLDriver::getCurrentRenderTargetSize() const
{
	if ( CurrentRendertargetSize.Width == 0 )
		return ScreenSize;
	else
		return CurrentRendertargetSize;
}


//! Clears the ZBuffer.
void COpenGLDriver::clearZBuffer()
{
	GLboolean enabled = GL_TRUE;
	glGetBooleanv(GL_DEPTH_WRITEMASK, &enabled);

	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);

	glDepthMask(enabled);
}


//! Returns an image created from the last rendered frame.
IImage* COpenGLDriver::createScreenShot()
{
	IImage* newImage = irrnew CImage(ECF_R8G8B8, ScreenSize);

	u8* pPixels = static_cast<u8*>(newImage->lock());
	if (!pPixels)
	{
		newImage->drop();
		return 0;
	}

	// allows to read pixels in top-to-bottom order
/*#ifdef GL_MESA_pack_invert
	if (FeatureAvailable[IRR_MESA_pack_invert])
		glPixelStorei(GL_PACK_INVERT_MESA, GL_TRUE);
#endif*/

	// We want to read the front buffer to get the latest render finished.
#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
	glReadBuffer(GL_FRONT);
#endif

	glReadPixels(0, 0, ScreenSize.Width, ScreenSize.Height, GL_RGB, GL_UNSIGNED_BYTE, pPixels);

#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
	glReadBuffer(GL_BACK);
#endif

/*#ifdef GL_MESA_pack_invert
	if (FeatureAvailable[IRR_MESA_pack_invert])
		glPixelStorei(GL_PACK_INVERT_MESA, GL_FALSE);
	else
#endif*/

	{
		// opengl images are horizontally flipped, so we have to fix that here.
		const s32 pitch=newImage->getPitch();
		u8* p2 = pPixels + (ScreenSize.Height - 1) * pitch;
		u8* tmpBuffer = irrnew u8[pitch];
		for (s32 i=0; i < ScreenSize.Height; i += 2)
		{
			memcpy(tmpBuffer, pPixels, pitch);
			memcpy(pPixels, p2, pitch);
			memcpy(p2, tmpBuffer, pitch);
			pPixels += pitch;
			p2 -= pitch;
		}
		delete [] tmpBuffer;
	}

	newImage->unlock();

	if (testGLError())
	{
		newImage->drop();
		return 0;
	}

	return newImage;
}


//! Set/unset a clipping plane.
//! There are at least 6 clipping planes available for the user to set at will.
//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
//! \param plane: The plane itself.
//! \param enable: If true, enable the clipping plane else disable it.
bool COpenGLDriver::setClipPlane(u32 index, const core::plane3df& plane, bool enable)
{
	/*if (index >= MaxUserClipPlanes)
		return false;

	UserClipPlane[index]=plane;
	enableClipPlane(index, enable);*/
	return true;
}


void COpenGLDriver::uploadClipPlane(u32 index)
{
	// opengl needs an array of doubles for the plane equation
	/*double clip_plane[4];
	clip_plane[0] = UserClipPlane[index].Normal.X;
	clip_plane[1] = UserClipPlane[index].Normal.Y;
	clip_plane[2] = UserClipPlane[index].Normal.Z;
	clip_plane[3] = UserClipPlane[index].D;
	glClipPlane(GL_CLIP_PLANE0 + index, clip_plane);*/
}


//! Enable/disable a clipping plane.
//! There are at least 6 clipping planes available for the user to set at will.
//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
//! \param enable: If true, enable the clipping plane else disable it.
void COpenGLDriver::enableClipPlane(u32 index, bool enable)
{
	return;
	/*if (index >= MaxUserClipPlanes)
		return;
	if (enable)
	{
		if (!UserClipPlaneEnabled[index])
		{
			uploadClipPlane(index);
			glEnable(GL_CLIP_PLANE0 + index);
		}
	}
	else
		glDisable(GL_CLIP_PLANE0 + index);

	UserClipPlaneEnabled[index]=enable;*/
}

void COpenGLDriver::setOrientation3D(E_ORIENTATION newOri)
{
    CurrentOrientation3D = newOri;
}

E_ORIENTATION COpenGLDriver::getOrientation3D()
{
    return CurrentOrientation3D;    
}

void COpenGLDriver::setNeedForceCommitTexture(bool value)
{
	NeedForceCommitTexture = value;
}

bool COpenGLDriver::isNeedForceCommitTexture()
{
	return NeedForceCommitTexture;
}

void COpenGLDriver::forceCommitTexture(COpenGLTexture* texture)
{
	if (!NeedForceCommitTexture) return;

	material2D.setTexture(0, (video::ITexture*)texture);
	setRenderStates2DMode(false, true, false, false);

	u16 indices[] = {0};
	S3DVertex vertices[1];
	vertices[0] = S3DVertex(0, 0, 0, 0, 0, 1, SColor(0xFFFFFFFF), 0, 0);
	drawVertexPrimitiveList2d3d(vertices, 1, indices, 1, video::EVT_STANDARD, scene::EPT_POINTS, EIT_16BIT, false);    
}

//set scissor test
//! \param scissorRect: the scissor rect, upperleft cornor is 0,0
void COpenGLDriver::setScissor(const core::rect<s32>& scissorRect)
{
	glEnable(GL_SCISSOR_TEST);
	const core::dimension2d<s32>& rts = getCurrentRenderTargetSize();
	s32 rw = rts.Width;
	s32 rh = rts.Height;
	E_ORIENTATION orit = getOrientation3D();
	s32 x = scissorRect.UpperLeftCorner.X;
	s32 y = scissorRect.UpperLeftCorner.Y;
	s32 w = scissorRect.getWidth();
	s32 h = scissorRect.getHeight();
#ifdef IPHONEOS
    rw = rts.Height;
	rh = rts.Width;
    if(!isLeftLandscape())
    {
        s32 t = x;
		x = y;
		y = t;
		
		t = w;
		w = h;
		h = t;
    }
    else
    {
        s32 t = x;
		x = rw - y - h;
		y = rh - t - w;
		
		t = w;
		w = h;
		h = t;
    }

#else
    if(orit == EOO_90)
	{
		s32 t = x;
		x = y;
		y = t;
		
		t = w;
		w = h;
		h = t;
	}
	else if (orit == EOO_270)
	{
		s32 t = x;
		x = rw - y - h;
		y = rh - t - w;
		
		t = w;
		w = h;
		h = t;
	}
	else if (orit == EOO_180)
	{
		//y = y + h;
		x = rw - x - w;
	}
	else
	{
		y = rh - y - h;
	}
#endif
    
	glScissor(x, y, w, h);
}

//close scissor test
void COpenGLDriver::resetScissor()
{
	glDisable(GL_SCISSOR_TEST);
}

//! Set alpha/color mask to write in color buffer.
void COpenGLDriver::setColorMask(bool red, bool green, bool blue, bool alpha)
{
	glColorMask( red, green, blue, alpha );
}

} // end namespace
} // end namespace

#endif // _IRR_COMPILE_WITH_OPENGL_

#if !defined(_IRR_COMPILE_WITH_OPENGL_ES_)

namespace irr
{
namespace video
{


// -----------------------------------
// WINDOWS VERSION
// -----------------------------------
#ifdef _IRR_USE_WINDOWS_DEVICE_
IVideoDriver* createOpenGLDriver(const core::dimension2d<s32>& screenSize,
	HWND window, u32 bits, bool stencilBuffer, io::IFileSystem* io, bool vsync, bool antiAlias)
{
#ifdef _IRR_COMPILE_WITH_OPENGL_
	COpenGLDriver* ogl =  irrnew COpenGLDriver(screenSize, window, stencilBuffer, io, antiAlias);
	if (!ogl->initDriver(screenSize, window, bits, vsync, stencilBuffer))
	{
		ogl->drop();
		ogl = 0;
	}
	return ogl;
#else
	return 0;
#endif // _IRR_COMPILE_WITH_OPENGL_
}
#endif // _IRR_USE_WINDOWS_DEVICE_

// -----------------------------------
// MACOSX VERSION
// -----------------------------------
#if defined(_IRR_USE_OSX_DEVICE_)
IVideoDriver* createOpenGLDriver(const SIrrlichtCreationParameters& params,
		io::IFileSystem* io, CIrrDeviceMacOSX *device)
{
#ifdef _IRR_COMPILE_WITH_OPENGL_
	return irrnew COpenGLDriver(params, io, device);
#else
	return 0;
#endif //  _IRR_COMPILE_WITH_OPENGL_
}
#endif // _IRR_USE_OSX_DEVICE_

#if defined(_IRR_USE_IPHONEOS_DEVICE_)
IVideoDriver* createOpenGLDriver(const SIrrlichtCreationParameters& params,
		io::IFileSystem* io, CIrrDeviceIPhoneOS *device)
{
#ifdef _IRR_COMPILE_WITH_OPENGL_
	return irrnew COpenGLDriver(params, io, device);
#else
	return 0;
#endif //  _IRR_COMPILE_WITH_OPENGL_
}
#endif

// -----------------------------------
// X11/SDL VERSION
// -----------------------------------
#if defined(_IRR_USE_LINUX_DEVICE_) || defined(_IRR_USE_SDL_DEVICE_)
IVideoDriver* createOpenGLDriver(const SIrrlichtCreationParameters& params,
		io::IFileSystem* io)
{
#ifdef _IRR_COMPILE_WITH_OPENGL_
	return irrnew COpenGLDriver(params, io);
#else
	return 0;
#endif //  _IRR_COMPILE_WITH_OPENGL_
}
#endif // _IRR_USE_LINUX_DEVICE_

} // end namespace
} // end namespace

#endif

