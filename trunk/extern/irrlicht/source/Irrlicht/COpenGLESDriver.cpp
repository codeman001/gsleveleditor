// Copyright (C) 2002-2007 Nikolaus Gebhardt, modify OpenGL|ES by pham hong duc (hongduc.pr@gmail.com)
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#include "COpenGLESDriver.h"
#include "CNullDriver.h"

#ifdef _IRR_COMPILE_WITH_OPENGL_ES_

#include "COpenGLESDriver.h"
#include "COpenGLESTexture.h"
#include "COpenGLESMaterialRenderer.h"
#include "COpenGLESShaderMaterialRenderer.h"
#include "COpenGLESSLMaterialRenderer.h"
#include "COpenGLESNormalMapRenderer.h"
#include "COpenGLESParallaxMapRenderer.h"
#include "CImage.h"
#include "os.h"

#ifdef _IRR_USE_SDL_DEVICE_
#include <SDL/SDL.h>
#endif

namespace irr
{
namespace video
{

#ifdef _IRR_COMPILE_WITH_WINDOWS_DEVICE_
//! opengl es constructor and init code
COpenGLESDriver::COpenGLESDriver(const core::dimension2d<u32>& screenSize, bool fullscreen, bool stencilBuffer,
							 HWND hWnd, io::IFileSystem* io, bool vsync, bool antiAlias)
  : CNullDriver(io, screenSize),
  CurrentRenderMode(ERM_NONE), ResetRenderStates(true),
  AntiAlias(antiAlias),
  Transformation3DChanged(true), LastSetLight(-1),
  hWnd(hWnd), HDc(GetDC(hWnd)),
  eglSurface(0), eglDisplay(0),eglConfig(0),eglContext(0),
  RenderTargetTexture(0),
  CurrentRendertargetSize(0,0)
{
	StencilBuffer = stencilBuffer;

	eglDisplay = eglGetDisplay((NativeDisplayType) HDc);
	if(eglDisplay == EGL_NO_DISPLAY)
		eglDisplay = eglGetDisplay((NativeDisplayType) EGL_DEFAULT_DISPLAY);

	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		os::Printer::log("Error: eglInitialize() failed.", ELL_ERROR);
	}

	static EGLint s_configAttribs[] =
	{
		EGL_RED_SIZE,		8,
		EGL_GREEN_SIZE, 	8,
		EGL_BLUE_SIZE,		8,
		EGL_ALPHA_SIZE, 	8,
		EGL_DEPTH_SIZE, 	8,
		EGL_STENCIL_SIZE,   8,
		EGL_LUMINANCE_SIZE,    0,
		EGL_SURFACE_TYPE,      EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,   EGL_OPENGL_ES_BIT,
		EGL_NONE
	};

	if(StencilBuffer){
		s_configAttribs[11] = 8;
	}

	EGLint numConfigs;
	eglGetConfigs(eglDisplay, NULL, 0, &numConfigs);
	eglChooseConfig(eglDisplay, s_configAttribs, &eglConfig, 1, &numConfigs);
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, hWnd, NULL);
	
	if(eglSurface == EGL_NO_SURFACE)
	{
		eglGetError(); // Clear error
		eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}
	if( !eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) )
	{
		os::Printer::log("Error: Need running XServer to start Irrlicht Engine.", ELL_ERROR);
	}

#ifdef _DEBUG
	setDebugName("COpenGLESDriver");
#endif

	genericDriverInit(screenSize, stencilBuffer);
	eglSwapInterval(eglDisplay, vsync ? 1 : 0);
}
#endif  // _IRR_SUPPORT_PVR_EMU


//! destructor
COpenGLESDriver::~COpenGLESDriver()
{
	deleteMaterialRenders();

	// I get a blue screen on my laptop, when I do not delete the
	// textures manually before releasing the dc. Oh how I love this.

	deleteAllTextures();

#ifdef _IRR_COMPILE_WITH_WINDOWS_DEVICE_
	if(eglDisplay)
	{
		if( !eglMakeCurrent(eglDisplay,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT) )
			os::Printer::log("Release of display failed.", ELL_WARNING);
		if( !eglTerminate(eglDisplay) )
			os::Printer::log("Release of rendering context failed.", ELL_WARNING);
	}

	if (HDc) 
		ReleaseDC(hWnd, HDc);
#endif
}

// -----------------------------------------------------------------------
// METHODS
// -----------------------------------------------------------------------

bool COpenGLESDriver::genericDriverInit(const core::dimension2d<u32>& screenSize, bool stencilBuffer)
{
	Name=L"OpenGL|ES ";
	Name.append(glGetString(GL_VERSION));
	s32 pos=Name.findNext(L' ', 7);

	if (pos != -1)
		Name=Name.subString(0, pos);
	printVersion();

	// print renderer information
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	if (renderer && vendor)
		os::Printer::log(reinterpret_cast<const c8*>(renderer), reinterpret_cast<const c8*>(vendor), ELL_INFORMATION);

	u32 i;
	for (i=0; i<MATERIAL_MAX_TEXTURES; ++i)
		CurrentTexture[i]=0;

	// load extensions
	initExtensions(stencilBuffer);
	if (queryFeature(EVDF_ARB_GLSL))
	{
		char buf[32];
		const u32 maj = ShaderLanguageVersion/100;
		snprintf(buf, 32, "%u.%u", maj, ShaderLanguageVersion-maj*100);
		os::Printer::log("GLSL version", buf, ELL_INFORMATION);
	}
	else
		os::Printer::log("GLSL not available.", ELL_INFORMATION);

	glViewport(0, 0, screenSize.Width, screenSize.Height); // Reset The Current Viewport
	setAmbientLight(SColorf(0.0f,0.0f,0.0f,0.0f));

#ifdef GL_EXT_separate_specular_color
	if (FeatureAvailable[IRR_EXT_separate_specular_color])
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
#endif

	glClearDepthf(1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glDepthFunc(GL_LEQUAL);
	glFrontFace( GL_CW );


	if (AntiAlias && MultiSamplingExtension)
		glEnable(GL_MULTISAMPLE);

	UserClipPlane.reallocate(MaxUserClipPlanes);
	UserClipPlaneEnabled.reallocate(MaxUserClipPlanes);

	for (i=0; i<MaxUserClipPlanes; ++i)
	{
		UserClipPlane.push_back(core::plane3df());
		UserClipPlaneEnabled.push_back(false);
	}

	// create material renderers
	createMaterialRenderers();

	// set the renderstates
	ResetRenderStates = true;
	setRenderStates3DMode();

	// set fog mode
	//bool LinearFog = false;
	//setFog(FogColor, LinearFog, FogStart, FogEnd, FogDensity, PixelFog, RangeFog);

	return true;
}


void COpenGLESDriver::createMaterialRenderers()
{
	// create OpenGL material renderers
	addAndDropMaterialRenderer(new COpenGLESMaterialRenderer_SOLID( this));
	addAndDropMaterialRenderer(new COpenGLESMaterialRenderer_SOLID_2_LAYER( this));

	// add the same renderer for all lightmap types
	COpenGLESMaterialRenderer_LIGHTMAP* lmr = new COpenGLESMaterialRenderer_LIGHTMAP( this);
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP:
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP_ADD:
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP_M2:
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP_M4:
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP_LIGHTING:
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP_LIGHTING_M2:
	addMaterialRenderer(lmr); // for EMT_LIGHTMAP_LIGHTING_M4:
	lmr->drop();

	// add remaining material renderer
	addAndDropMaterialRenderer(new COpenGLESMaterialRenderer_DETAIL_MAP( this));
	addAndDropMaterialRenderer(new COpenGLESMaterialRenderer_SPHERE_MAP( this));
	addAndDropMaterialRenderer(new COpenGLESMaterialRenderer_REFLECTION_2_LAYER( this));
	addAndDropMaterialRenderer(new COpenGLESMaterialRenderer_TRANSPARENT_ADD_COLOR( this));
	addAndDropMaterialRenderer(new COpenGLESMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL( this));
	addAndDropMaterialRenderer(new COpenGLESMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF( this));
	addAndDropMaterialRenderer(new COpenGLESMaterialRenderer_TRANSPARENT_VERTEX_ALPHA( this));
	addAndDropMaterialRenderer(new COpenGLESMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER( this));

#ifdef _IRR_HAS_SHADER
	// add normal map renderers
	s32 tmp = 0;
	video::IMaterialRenderer* renderer = 0;
	renderer = new COpenGLESNormalMapRenderer(this, tmp, MaterialRenderers[EMT_SOLID].Renderer);
	renderer->drop();
	renderer = new COpenGLESNormalMapRenderer(this, tmp, MaterialRenderers[EMT_TRANSPARENT_ADD_COLOR].Renderer);
	renderer->drop();
	renderer = new COpenGLESNormalMapRenderer(this, tmp, MaterialRenderers[EMT_TRANSPARENT_VERTEX_ALPHA].Renderer);
	renderer->drop();

	// add parallax map renderers
	renderer = new COpenGLESParallaxMapRenderer(this, tmp, MaterialRenderers[EMT_SOLID].Renderer);
	renderer->drop();
	renderer = new COpenGLESParallaxMapRenderer(this, tmp, MaterialRenderers[EMT_TRANSPARENT_ADD_COLOR].Renderer);
	renderer->drop();
	renderer = new COpenGLESParallaxMapRenderer(this, tmp, MaterialRenderers[EMT_TRANSPARENT_VERTEX_ALPHA].Renderer);
	renderer->drop();
#endif // _IRR_HAS_SHADER

	// add basic 1 texture blending
	addAndDropMaterialRenderer(new COpenGLESMaterialRenderer_ONETEXTURE_BLEND(this));
}


//! presents the rendered scene on the screen, returns false if failed
bool COpenGLESDriver::endScene()
{
	CNullDriver::endScene();

	glFlush();

#ifdef _IRR_COMPILE_WITH_OPENGL_ES_
	eglSwapBuffers( eglDisplay, eglSurface );
	return true;
#endif

}

//! clears the zbuffer
bool COpenGLESDriver::beginScene(bool backBuffer, bool zBuffer,
				SColor color,
				const SExposedVideoData& videoData,
				core::rect<s32>* sourceRect)
{
	CNullDriver::beginScene(backBuffer, zBuffer, color);

	GLbitfield mask = 0;

	if (backBuffer)
	{
		const f32 inv = 1.0f / 255.0f;
		glClearColor(color.getRed() * inv, color.getGreen() * inv, color.getBlue() * inv, color.getAlpha() * inv);
		mask |= GL_COLOR_BUFFER_BIT;
	}

	if (zBuffer)
	{
		glDepthMask(GL_TRUE);
		mask |= GL_DEPTH_BUFFER_BIT;
	}

	glClear(mask);
	return true;
}



//! Returns the transformation set by setTransform
const core::matrix4& COpenGLESDriver::getTransform(E_TRANSFORMATION_STATE state) const
{
	return Matrices[state];
}



//! sets transformation
void COpenGLESDriver::setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat)
{
	GLfloat glmat[16];
	Matrices[state] = mat;
	Transformation3DChanged = true;

	switch(state)
	{
	case ETS_VIEW:
	case ETS_WORLD:
		{
			// OpenGL only has a model matrix, view and world is not existent. so lets fake these two.
			createGLMatrix(glmat, Matrices[ETS_VIEW] * Matrices[ETS_WORLD]);
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(glmat);
			// we have to update the clip planes to the latest view matrix
			for (u32 i=0; i<MaxUserClipPlanes; ++i)
				if (UserClipPlaneEnabled[i])
					uploadClipPlane(i);
		}
		break;
	case ETS_PROJECTION:
		createGLMatrix(glmat, mat);
		// flip z to compensate OpenGLs right-hand coordinate system
		glmat[12] *= -1.0f;
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glmat);
		break;
	case ETS_TEXTURE_0:
	case ETS_TEXTURE_1:
	case ETS_TEXTURE_2:
	case ETS_TEXTURE_3:
	{
		const u32 i = state - ETS_TEXTURE_0;
		const bool isRTT = Material.getTexture(i) && Material.getTexture(i)->isRenderTarget();

		if (MultiTextureExtension)
			extGlActiveTexture(GL_TEXTURE0 + i);

		glMatrixMode(GL_TEXTURE);
		if (mat.isIdentity() && !isRTT)
			glLoadIdentity();
		else
		{
			createGLTextureMatrix(glmat, mat);
			if (isRTT)
				glmat[5] *= -1.0f;
			glLoadMatrixf(glmat);
		}
		break;
	}
	default:
		break;
	}
}



//! draws a vertex primitive list
void COpenGLESDriver::drawVertexPrimitiveList(const void* vertices, u32 vertexCount, const void* pIndexList, u32 primitiveCount, E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType)
{
	const u16* indexList = (u16*)pIndexList;

	if (!primitiveCount || !vertexCount)
		return;

	if (!checkPrimitiveCount(primitiveCount))
		return;

	CNullDriver::drawVertexPrimitiveList(vertices, vertexCount, indexList, primitiveCount, vType, pType);

	// convert colors to gl color format.
	vertexCount *= 4; //reused as color component count
	ColorBuffer.set_used(vertexCount);
	u32 i;

	switch (vType)
	{
		case EVT_STANDARD:
		{
			const S3DVertex* p = reinterpret_cast<const S3DVertex*>(vertices);
			for ( i=0; i<vertexCount; i+=4)
			{
				p->Color.toOpenGLColor(&ColorBuffer[i]);
				++p;
			}
		}
		break;
		case EVT_2TCOORDS:
		{
			const S3DVertex2TCoords* p = reinterpret_cast<const S3DVertex2TCoords*>(vertices);
			for ( i=0; i<vertexCount; i+=4)
			{
				p->Color.toOpenGLColor(&ColorBuffer[i]);
				++p;
			}
		}
		break;
		case EVT_TANGENTS:
		{
			const S3DVertexTangents* p = reinterpret_cast<const S3DVertexTangents*>(vertices);
			for ( i=0; i<vertexCount; i+=4)
			{
				p->Color.toOpenGLColor(&ColorBuffer[i]);
				++p;
			}
		}
		break;
	}

	// draw everything
	setRenderStates3DMode();

	if (MultiTextureExtension)
		extGlClientActiveTexture(GL_TEXTURE0);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	if ((pType!=scene::EPT_POINTS) && (pType!=scene::EPT_POINT_SPRITES))
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	if ((pType!=scene::EPT_POINTS) && (pType!=scene::EPT_POINT_SPRITES))
		glEnableClientState(GL_NORMAL_ARRAY);

	glColorPointer(4, GL_UNSIGNED_BYTE, 0, &ColorBuffer[0]);
	switch (vType)
	{
		case EVT_STANDARD:
			glVertexPointer(3, GL_FLOAT, sizeof(S3DVertex), &(reinterpret_cast<const S3DVertex*>(vertices))[0].Pos);
			glNormalPointer(GL_FLOAT, sizeof(S3DVertex), &(reinterpret_cast<const S3DVertex*>(vertices))[0].Normal);
			glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), &(reinterpret_cast<const S3DVertex*>(vertices))[0].TCoords);
			if (MultiTextureExtension && CurrentTexture[1])
			{
				extGlClientActiveTexture(GL_TEXTURE1);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex), &(reinterpret_cast<const S3DVertex*>(vertices))[0].TCoords);
			}
			break;
		case EVT_2TCOORDS:
			glVertexPointer(3, GL_FLOAT, sizeof(S3DVertex2TCoords), &(reinterpret_cast<const S3DVertex2TCoords*>(vertices))[0].Pos);
			glNormalPointer(GL_FLOAT, sizeof(S3DVertex2TCoords), &(reinterpret_cast<const S3DVertex2TCoords*>(vertices))[0].Normal);
			// texture coordinates
			glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), &(reinterpret_cast<const S3DVertex2TCoords*>(vertices))[0].TCoords);
			if (MultiTextureExtension)
			{
				extGlClientActiveTexture(GL_TEXTURE1);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertex2TCoords), &(reinterpret_cast<const S3DVertex2TCoords*>(vertices))[0].TCoords2);
			}
			break;
		case EVT_TANGENTS:
			glVertexPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), &(reinterpret_cast<const S3DVertexTangents*>(vertices))[0].Pos);
			glNormalPointer(GL_FLOAT, sizeof(S3DVertexTangents), &(reinterpret_cast<const S3DVertexTangents*>(vertices))[0].Normal);
			// texture coordinates
			glTexCoordPointer(2, GL_FLOAT, sizeof(S3DVertexTangents), &(reinterpret_cast<const S3DVertexTangents*>(vertices))[0].TCoords);
			if (MultiTextureExtension)
			{
				extGlClientActiveTexture(GL_TEXTURE1);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), &(reinterpret_cast<const S3DVertexTangents*>(vertices))[0].Tangent);

				extGlClientActiveTexture(GL_TEXTURE2);
				glEnableClientState ( GL_TEXTURE_COORD_ARRAY );
				glTexCoordPointer(3, GL_FLOAT, sizeof(S3DVertexTangents), &(reinterpret_cast<const S3DVertexTangents*>(vertices))[0].Binormal);
			}
			break;
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

#ifdef GL_ARB_point_sprite
			if (pType==scene::EPT_POINT_SPRITES && FeatureAvailable[IRR_ARB_point_sprite])
				glTexEnvf(GL_POINT_SPRITE_ARB,GL_COORD_REPLACE, GL_TRUE);
#endif

			glDrawArrays(GL_POINTS, 0, primitiveCount);

#ifdef GL_ARB_point_sprite
			if (pType==scene::EPT_POINT_SPRITES && FeatureAvailable[IRR_ARB_point_sprite])
			{
				glDisable(GL_POINT_SPRITE);
				glTexEnvf(GL_POINT_SPRITE,GL_COORD_REPLACE, GL_FALSE);
			}
#endif
		}
			break;
		case scene::EPT_LINE_STRIP:
			glDrawElements(GL_LINE_STRIP, primitiveCount+1, GL_UNSIGNED_SHORT, indexList);
			break;
		case scene::EPT_LINE_LOOP:
			glDrawElements(GL_LINE_LOOP, primitiveCount, GL_UNSIGNED_SHORT, indexList);
			break;
		case scene::EPT_LINES:
			glDrawElements(GL_LINES, primitiveCount*2, GL_UNSIGNED_SHORT, indexList);
			break;
	
		// for OpenGL|ES
		//note that PolygonMode in OpenGL ES is not directly supported. The following provides
		//a workaround for PolygonMode(GL_LINES) or PolygonMode(GL_POINTS). However, the wireframe
		//mode, which corresponds to GL_LINES, is inefficiently emulated because a large number
		//of function calls are needed.
        //
		//QUAD_STRIP, QUADS and POLYGON are also not directly supported. We use triangle strips,
		//triangle loops to render them.
		case scene::EPT_TRIANGLE_STRIP:
			if(Material.Wireframe)
			{
				for(u32 i = 0; i<primitiveCount; i++)
					glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT, indexList+i);
			}
			else
				glDrawElements(Material.PointCloud ? GL_POINTS : GL_TRIANGLE_STRIP, primitiveCount+2, GL_UNSIGNED_SHORT, indexList);
			break;
		case scene::EPT_TRIANGLE_FAN:
			if(Material.Wireframe)
			{
				//draw the outer line loop
				glDrawElements(GL_LINE_LOOP, primitiveCount, GL_UNSIGNED_SHORT, indexList);

				//draw inner lines
				u16 lineIndex[2];
				lineIndex[0] = indexList[0];
				for(u32 i = 2; i <= primitiveCount; i++)
				{
					lineIndex[1] = indexList[i];
					glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, lineIndex);
				}
			}
			glDrawElements(Material.PointCloud ? GL_POINTS : GL_TRIANGLE_FAN, primitiveCount+2, GL_UNSIGNED_SHORT, indexList);
			break;
		case scene::EPT_TRIANGLES:
			if(Material.Wireframe)
			{
				for(u32 i = 0; i < primitiveCount; i++)
					glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_SHORT, indexList+(i*3));
			}
			else
				glDrawElements(Material.PointCloud ? GL_POINTS : GL_TRIANGLES, primitiveCount*3, GL_UNSIGNED_SHORT, indexList);
			break;
		case scene::EPT_QUAD_STRIP:
			if(Material.Wireframe)
			{
				u16 lineIndex[4];
				for(u32 i = 0; i < primitiveCount; i++)
				{
					lineIndex[0] = indexList[i*2];
					lineIndex[1] = indexList[i*2+1];
					lineIndex[2] = indexList[i*2+3];
					lineIndex[3] = indexList[i*2+2];
					glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, lineIndex);
				}
			}
			else
				glDrawElements(Material.PointCloud ? GL_POINTS : GL_TRIANGLE_STRIP, primitiveCount*2+2, GL_UNSIGNED_SHORT, indexList);
			break;
		case scene::EPT_QUADS:
			if(Material.Wireframe)
			{
				for(u32 i = 0; i < primitiveCount; i++)
					glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, indexList+i*4);
			}
			else if(Material.PointCloud)
				glDrawElements(GL_POINTS, primitiveCount*4, GL_UNSIGNED_SHORT, indexList);
			else
			{
                for(u32 i = 0; i < primitiveCount; i++)
					glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, indexList+i*4);
			}
			break;
		case scene::EPT_POLYGON:
			if(Material.Wireframe)
			{
				for(u32 i = 0; i < primitiveCount; i++)
					glDrawElements(GL_LINE_LOOP, primitiveCount, GL_UNSIGNED_SHORT, indexList);
			}
			else
				glDrawElements(Material.PointCloud ? GL_POINTS : GL_TRIANGLE_FAN, primitiveCount, GL_UNSIGNED_SHORT, indexList);
			break;

	}

	if (MultiTextureExtension)
	{
		if (vType==EVT_TANGENTS)
		{
			extGlClientActiveTexture(GL_TEXTURE2);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		if ((vType!=EVT_STANDARD) || CurrentTexture[1])
		{
			extGlClientActiveTexture(GL_TEXTURE1);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		extGlClientActiveTexture(GL_TEXTURE0);
	}

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


void drawQuads(	core::rect<f32>& npos, core::rect<f32>& tcoords, video::SColor* useColor=NULL)
{
	GLfloat points[2*4];
	GLfloat texcoords[2*4];
	GLubyte colors[4*4];

	//lower left
	points[0] = npos.UpperLeftCorner.X;
	points[1] = npos.LowerRightCorner.Y;
	texcoords[0] = tcoords.UpperLeftCorner.X;
	texcoords[1] = tcoords.LowerRightCorner.Y;

	//upper left
	points[2] = npos.UpperLeftCorner.X;
	points[3] = npos.UpperLeftCorner.Y;
	texcoords[2] = tcoords.UpperLeftCorner.X;
	texcoords[3] = tcoords.UpperLeftCorner.Y;

	//lower right
	points[4] = npos.LowerRightCorner.X;
	points[5] = npos.LowerRightCorner.Y;
	texcoords[4] = tcoords.LowerRightCorner.X;
	texcoords[5] = tcoords.LowerRightCorner.Y;

	//upper right
	points[6] = npos.LowerRightCorner.X;
	points[7] = npos.UpperLeftCorner.Y;
	texcoords[6] = tcoords.LowerRightCorner.X;
	texcoords[7] = tcoords.UpperLeftCorner.Y;

	if(useColor){
		for (u32 i = 0; i < 4; i++){
			useColor[i].toOpenGLColor(&colors[i*4]);
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, points);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	if(useColor){
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
		glEnableClientState(GL_COLOR_ARRAY);
	}
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	if(useColor){
		glDisableClientState(GL_COLOR_ARRAY);
	}
}

//! draws a 2d image, using a color and the alpha channel of the texture if
//! desired. The image is drawn at pos, clipped against clipRect (if != 0).
//! Only the subtexture defined by sourceRect is used.
void COpenGLESDriver::draw2DImage(const video::ITexture* texture,
				const core::position2d<s32>& pos,
				const core::rect<s32>& sourceRect,
				const core::rect<s32>* clipRect, SColor color,
				bool useAlphaChannelOfTexture)
{	
	if (!texture)
		return;

	if (!sourceRect.isValid())
		return;

	core::position2d<s32> targetPos(pos);
	core::position2d<s32> sourcePos(sourceRect.UpperLeftCorner);
	// This needs to be signed as it may go negative.
	core::dimension2d<s32> sourceSize(sourceRect.getSize());
	if (clipRect)
	{
		if (targetPos.X < clipRect->UpperLeftCorner.X)
		{
			sourceSize.Width += targetPos.X - clipRect->UpperLeftCorner.X;
			if (sourceSize.Width <= 0)
				return;

			sourcePos.X -= targetPos.X - clipRect->UpperLeftCorner.X;
			targetPos.X = clipRect->UpperLeftCorner.X;
		}

		if (targetPos.X + sourceSize.Width > clipRect->LowerRightCorner.X)
		{
			sourceSize.Width -= (targetPos.X + sourceSize.Width) - clipRect->LowerRightCorner.X;
			if (sourceSize.Width <= 0)
				return;
		}

		if (targetPos.Y < clipRect->UpperLeftCorner.Y)
		{
			sourceSize.Height += targetPos.Y - clipRect->UpperLeftCorner.Y;
			if (sourceSize.Height <= 0)
				return;

			sourcePos.Y -= targetPos.Y - clipRect->UpperLeftCorner.Y;
			targetPos.Y = clipRect->UpperLeftCorner.Y;
		}

		if (targetPos.Y + sourceSize.Height > clipRect->LowerRightCorner.Y)
		{
			sourceSize.Height -= (targetPos.Y + sourceSize.Height) - clipRect->LowerRightCorner.Y;
			if (sourceSize.Height <= 0)
				return;
		}
	}

	// clip these coordinates

	if (targetPos.X<0)
	{
		sourceSize.Width += targetPos.X;
		if (sourceSize.Width <= 0)
			return;

		sourcePos.X -= targetPos.X;
		targetPos.X = 0;
	}

	const core::dimension2d<u32>& renderTargetSize = getCurrentRenderTargetSize();

	if (targetPos.X + sourceSize.Width > (s32)renderTargetSize.Width)
	{
		sourceSize.Width -= (targetPos.X + sourceSize.Width) - renderTargetSize.Width;
		if (sourceSize.Width <= 0)
			return;
	}

	if (targetPos.Y<0)
	{
		sourceSize.Height += targetPos.Y;
		if (sourceSize.Height <= 0)
			return;

		sourcePos.Y -= targetPos.Y;
		targetPos.Y = 0;
	}

	if (targetPos.Y + sourceSize.Height > (s32)renderTargetSize.Height)
	{
		sourceSize.Height -= (targetPos.Y + sourceSize.Height) - renderTargetSize.Height;
		if (sourceSize.Height <= 0)
			return;
	}

	// ok, we've clipped everything.
	// now draw it.

	// texcoords need to be flipped horizontally for RTTs
	const bool isRTT = texture->isRenderTarget();
	const core::dimension2d<u32>& ss = texture->getOriginalSize();
	const f32 invW = 1.f / static_cast<f32>(ss.Width);
	const f32 invH = 1.f / static_cast<f32>(ss.Height);
	core::rect<f32> tcoords(
			sourcePos.X * invW,
			(isRTT?(sourcePos.Y + sourceSize.Height):sourcePos.Y) * invH,
			(sourcePos.X + sourceSize.Width) * invW,
			(isRTT?sourcePos.Y:(sourcePos.Y + sourceSize.Height)) * invH);

	core::rect<f32> poss( (f32)targetPos.X, (f32)targetPos.Y, (f32)targetPos.X + (f32)sourceSize.Width, (f32)targetPos.Y + (f32)sourceSize.Height );

	disableTextures(1);
	if (!setTexture(0, texture))
		return;

	setRenderStates2DMode(color.getAlpha()<255, true, useAlphaChannelOfTexture);
	glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
	drawQuads( poss, tcoords );
	
}



//! The same, but with a four element array of colors, one for each vertex
void COpenGLESDriver::draw2DImage(const video::ITexture* texture, const core::rect<s32>& destRect,
		const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect,
		video::SColor* colors, bool useAlphaChannelOfTexture)
{
	
}



//! draws a set of 2d images, using a color and the alpha channel of the
//! texture if desired. The images are drawn beginning at pos and concatenated
//! in one line. All drawings are clipped against clipRect (if != 0).
//! The subtextures are defined by the array of sourceRects and are chosen
//! by the indices given.
void COpenGLESDriver::draw2DImage(const video::ITexture* texture,
				const core::position2d<s32>& pos,
				const core::array<core::rect<s32> >& sourceRects,
				const core::array<s32>& indices,
				const core::rect<s32>* clipRect, SColor color,
				bool useAlphaChannelOfTexture)
{

}



//! draw a 2d rectangle
void COpenGLESDriver::draw2DRectangle(SColor color, const core::rect<s32>& position,
		const core::rect<s32>* clip)
{
	disableTextures();
	setRenderStates2DMode(color.getAlpha() < 255, false, false);

	core::rect<s32> pos = position;

	if (clip)
		pos.clipAgainst(*clip);

	if (!pos.isValid())
		return;

	glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());


	GLfloat points[2*4];
	GLfloat x1 = (float)position.UpperLeftCorner.X;
	GLfloat y1 = (float)position.UpperLeftCorner.Y;
	GLfloat x2 = (float)position.LowerRightCorner.X;
	GLfloat y2 = (float)position.LowerRightCorner.Y;

	//lower left
	points[0] = x1;
	points[1] = y2;

	//upper left
	points[2] = x1;
	points[3] = y1;

	//lower right
	points[4] = x2;
	points[5] = y2;

	//upper right
	points[6] = x2;
	points[7] = y1;

	glVertexPointer(2, GL_FLOAT, 0, points);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

}



//! draw an 2d rectangle
void COpenGLESDriver::draw2DRectangle(const core::rect<s32>& position,
			SColor colorLeftUp, SColor colorRightUp, SColor colorLeftDown, SColor colorRightDown,
			const core::rect<s32>* clip)
{
	core::rect<s32> pos = position;

	if (clip)
		pos.clipAgainst(*clip);

	if (!pos.isValid())
		return;

	setRenderStates2DMode(colorLeftUp.getAlpha() < 255 ||
		colorRightUp.getAlpha() < 255 ||
		colorLeftDown.getAlpha() < 255 ||
		colorRightDown.getAlpha() < 255, false, false);

	disableTextures();


	core::rect<f32> npos;
	npos.UpperLeftCorner.X = (float)position.UpperLeftCorner.X;
	npos.UpperLeftCorner.Y = (float)position.UpperLeftCorner.Y;
	npos.LowerRightCorner.X = (float)position.LowerRightCorner.X;
	npos.LowerRightCorner.Y = (float)position.LowerRightCorner.Y;

	GLfloat points[2*4];
	GLubyte colors[4*4];

	//lower left
	points[0] = npos.UpperLeftCorner.X;
	points[1] = npos.LowerRightCorner.Y;
	colorLeftDown.toOpenGLColor(&colors[0]);

	//upper left
	points[2] = npos.UpperLeftCorner.X;
	points[3] = npos.UpperLeftCorner.Y;
	colorLeftUp.toOpenGLColor(&colors[4]);

	//lower right
	points[4] = npos.LowerRightCorner.X;
	points[5] = npos.LowerRightCorner.Y;
	colorRightDown.toOpenGLColor(&colors[8]);

	//upper right
	points[6] = npos.LowerRightCorner.X;
	points[7] = npos.UpperLeftCorner.Y;
	colorRightUp.toOpenGLColor(&colors[12]);

	glVertexPointer(2, GL_FLOAT, 0, points);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}



//! Draws a 2d line.
void COpenGLESDriver::draw2DLine(const core::position2d<s32>& start,
				const core::position2d<s32>& end,
				SColor color)
{	
	disableTextures();
	setRenderStates2DMode(color.getAlpha() < 255, false, false);

	GLfloat points[2*2];
	points[0] = (float)start.X;
	points[1] = (float)start.Y;
	points[2] = (float)end.X;
	points[3] = (float)end.Y;

	glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
	glVertexPointer(2, GL_FLOAT, 0, points);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);

}



bool COpenGLESDriver::setTexture(u32 stage, const video::ITexture* texture)
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
		glDisable(GL_TEXTURE_2D);
		return true;
	}
	else
	{
		if (texture->getDriverType() != EDT_OPENGLES)
		{
			glDisable(GL_TEXTURE_2D);
			os::Printer::log("Fatal Error: Tried to set a texture not owned by this driver.", ELL_ERROR);
			return false;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,
			static_cast<const COpenGLESTexture*>(texture)->getOpenGLTextureName());
	}
	return true;
}



//! disables all textures beginning with the optional fromStage parameter. Otherwise all texture stages are disabled.
//! Returns whether disabling was successful or not.
bool COpenGLESDriver::disableTextures(u32 fromStage)
{
	bool result=true;
	for (u32 i=fromStage; i<MaxTextureUnits; ++i)
		result &= setTexture(i, 0);
	return result;
}



//! creates a matrix in supplied GLfloat array to pass to OpenGL
inline void COpenGLESDriver::createGLMatrix(GLfloat gl_matrix[16], const core::matrix4& m)
{
	memcpy ( gl_matrix, m.pointer(), 16 * sizeof(f32) );
}


//! creates a opengltexturematrix from a D3D style texture matrix
inline void COpenGLESDriver::createGLTextureMatrix(GLfloat *o, const core::matrix4& m)
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
video::ITexture* COpenGLESDriver::createDeviceDependentTexture(IImage* surface, const char* name)
{
	return new COpenGLESTexture(surface, name, this);
}

video::ITexture* COpenGLESDriver::createDeviceDependentTexture(IImage* surface, const io::path& name, void* mipmapData)
{
	return new COpenGLESTexture(surface, name.c_str(), this);
}

//! Sets a material. All 3d drawing functions draw geometry now
//! using this material.
//! \param material: Material to be used from now on.
void COpenGLESDriver::setMaterial(const SMaterial& material)
{
	Material = material;

	for (s32 i = MaxTextureUnits-1; i>= 0; --i)
	{
		setTransform ((E_TRANSFORMATION_STATE) ( ETS_TEXTURE_0 + i ),
				material.getTextureMatrix(i));
	}
}



//! prints error if an error happened.
bool COpenGLESDriver::testGLError()
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
	case GL_STACK_OVERFLOW:
		os::Printer::log("GL_STACK_OVERFLOW", ELL_ERROR); break;
	case GL_STACK_UNDERFLOW:
		os::Printer::log("GL_STACK_UNDERFLOW", ELL_ERROR); break;
	case GL_OUT_OF_MEMORY:
		os::Printer::log("GL_OUT_OF_MEMORY", ELL_ERROR); break;
#ifdef GL_ARB_imaging
	case GL_TABLE_TOO_LARGE:
		os::Printer::log("GL_TABLE_TOO_LARGE", ELL_ERROR); break;
#endif
	};
	return true;
#else
	return false;
#endif
}



//! sets the needed renderstates
void COpenGLESDriver::setRenderStates3DMode()
{
	if (CurrentRenderMode != ERM_3D)
	{
		// Reset Texture Stages
		if (FeatureAvailable[IRR_ARB_texture_env_combine])
			glTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE );
		glDisable(GL_ALPHA_TEST);
		glDisable( GL_BLEND );
		glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );

		// switch back the matrices
		GLfloat glmat[16];

		createGLMatrix(glmat, Matrices[ETS_VIEW] * Matrices[ETS_WORLD]);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(glmat);

		createGLMatrix(glmat, Matrices[ETS_PROJECTION]);
		glmat[12] *= -1.0f;
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(glmat);

		ResetRenderStates = true;
	}

	if ( ResetRenderStates || LastMaterial != Material)
	{
		// unset old material

		if (LastMaterial.MaterialType != Material.MaterialType &&
			LastMaterial.MaterialType >= 0 && LastMaterial.MaterialType < static_cast<s32>(MaterialRenderers.size()))
			MaterialRenderers[LastMaterial.MaterialType].Renderer->OnUnsetMaterial();

		// set new material.
		if (Material.MaterialType >= 0 && Material.MaterialType < static_cast<s32>(MaterialRenderers.size()))
			MaterialRenderers[Material.MaterialType].Renderer->OnSetMaterial(
				Material, LastMaterial, ResetRenderStates, this);

		LastMaterial = Material;
		ResetRenderStates = false;
	}

	if (Material.MaterialType >= 0 && Material.MaterialType < static_cast<s32>(MaterialRenderers.size()))
		MaterialRenderers[Material.MaterialType].Renderer->OnRender(this, video::EVT_STANDARD);

	CurrentRenderMode = ERM_3D;
}


//! Can be called by an IMaterialRenderer to make its work easier.
void COpenGLESDriver::setBasicRenderStates(const SMaterial& material, const SMaterial& lastmaterial,
	bool resetAllRenderStates)
{
	if (resetAllRenderStates ||
		lastmaterial.AmbientColor != material.AmbientColor ||
		lastmaterial.DiffuseColor != material.DiffuseColor ||
		lastmaterial.SpecularColor != material.SpecularColor ||
		lastmaterial.EmissiveColor != material.EmissiveColor ||
		lastmaterial.Shininess != material.Shininess)
	{
		GLfloat color[4];

		const f32 inv = 1.0f / 255.0f;

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

	// Texture filter
	// Has to be checked always because it depends on the textures
	// Filtering has to be set for each texture layer
	for (u32 i=0; i<MaxTextureUnits; ++i)
	{
		if (!material.getTexture(i))
			continue;
		if (MultiTextureExtension)
			extGlActiveTexture(GL_TEXTURE0 + i);
		else if (i>0)
			break;

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			(material.TextureLayer[i].BilinearFilter || material.TextureLayer[i].TrilinearFilter) ? GL_LINEAR : GL_NEAREST);

		if (material.getTexture(i) && material.getTexture(i)->hasMipMaps())
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				material.TextureLayer[i].TrilinearFilter ? GL_LINEAR_MIPMAP_LINEAR : material.TextureLayer[i].BilinearFilter ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST );
		else
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				(material.TextureLayer[i].BilinearFilter || material.TextureLayer[i].TrilinearFilter) ? GL_LINEAR : GL_NEAREST);

#ifdef GL_EXT_texture_filter_anisotropic
		if (AnisotropyExtension)
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
				material.TextureLayer[i].AnisotropicFilter ? MaxAnisotropy : 1.0f );
#endif
	}


	// shademode
	if (resetAllRenderStates || lastmaterial.GouraudShading != material.GouraudShading)
	{
		if (material.GouraudShading)
			glShadeModel(GL_SMOOTH);
		else
			glShadeModel(GL_FLAT);
	}

	// lighting

	if (resetAllRenderStates || lastmaterial.Lighting != material.Lighting)
	{
		if (material.Lighting)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
	}

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
	if (resetAllRenderStates || lastmaterial.BackfaceCulling != material.BackfaceCulling)
	{
		if (material.BackfaceCulling)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
	}

	// fog
	if (resetAllRenderStates || lastmaterial.FogEnable != material.FogEnable)
	{
		if (material.FogEnable)
			glEnable(GL_FOG);
		else
			glDisable(GL_FOG);
	}

	// normalization
	if (resetAllRenderStates || lastmaterial.NormalizeNormals != material.NormalizeNormals)
	{
		if (material.NormalizeNormals)
			glEnable(GL_NORMALIZE);
		else
			glDisable(GL_NORMALIZE);
	}

	// thickness
	if (resetAllRenderStates || lastmaterial.Thickness != material.Thickness)
	{
		glPointSize(material.Thickness);
		glLineWidth(material.Thickness);
	}

	// texture address mode
	for (u32 u=0; u<MaxTextureUnits; ++u)
	{
		if	(	
				resetAllRenderStates || 
				lastmaterial.TextureLayer[u].TextureWrapU != material.TextureLayer[u].TextureWrapU ||
				lastmaterial.TextureLayer[u].TextureWrapV != material.TextureLayer[u].TextureWrapV

			)
		{
			GLint mode=GL_REPEAT;
			switch (material.TextureLayer[u].TextureWrapU)
			{
				case ETC_REPEAT:
					mode=GL_REPEAT;
					break;
				case ETC_CLAMP:
					mode=GL_CLAMP_TO_EDGE;
					break;
				case ETC_CLAMP_TO_EDGE:
#ifdef GL_VERSION_1_2
					if (Version>101)
						mode=GL_CLAMP_TO_EDGE;
					else
#endif
#ifdef GL_SGIS_texture_edge_clamp
					if (FeatureAvailable[IRR_SGIS_texture_edge_clamp])
						mode=GL_CLAMP_TO_EDGE_SGIS;
					else
#endif						
						mode=GL_CLAMP_TO_EDGE;
					break;
				case ETC_CLAMP_TO_BORDER:
#ifdef GL_VERSION_1_3
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
#endif
						// fallback
						mode=GL_CLAMP_TO_EDGE;
					break;
			}

			if (MultiTextureExtension)
				extGlActiveTexture(GL_TEXTURE0 + u);
			else if (u>0)
				break;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
		}
	}
	// be sure to leave in texture stage 0
	if (MultiTextureExtension)
		extGlActiveTexture(GL_TEXTURE0);
}



//! sets the needed renderstates
void COpenGLESDriver::setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel)
{	
	if (CurrentRenderMode != ERM_2D || Transformation3DChanged)
	{
		// unset last 3d material
		if (CurrentRenderMode == ERM_3D)
		{
			if (static_cast<u32>(LastMaterial.MaterialType) < MaterialRenderers.size())
				MaterialRenderers[LastMaterial.MaterialType].Renderer->OnUnsetMaterial();
		}
		if (Transformation3DChanged)
		{
			glMatrixMode(GL_PROJECTION);

			const core::dimension2d<u32>& renderTargetSize = getCurrentRenderTargetSize();
			core::matrix4 m;
			m.buildProjectionMatrixOrthoLH(f32(renderTargetSize.Width), f32(-(s32)(renderTargetSize.Height)), -1.0, 1.0);
			m.setTranslation(core::vector3df(-1,1,0));
			glLoadMatrixf(m.pointer());

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.375, 0.375, 0.0);

			// Make sure we set first texture matrix
			if (MultiTextureExtension)
				extGlActiveTexture(GL_TEXTURE0);

			Transformation3DChanged = false;
		}
		if (!OverrideMaterial2DEnabled)
		{
			setBasicRenderStates(InitMaterial2D, LastMaterial, true);
			LastMaterial = InitMaterial2D;
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	if (OverrideMaterial2DEnabled)
	{
		OverrideMaterial2D.Lighting=false;
		OverrideMaterial2D.ZBuffer=ECFN_NEVER;
		OverrideMaterial2D.ZWriteEnable=false;
		setBasicRenderStates(OverrideMaterial2D, LastMaterial, false);
		LastMaterial = OverrideMaterial2D;
	}

	if (alphaChannel || alpha)
	{
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.f);
	}
	else
	{
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
	}

	if (texture)
	{
		if (!OverrideMaterial2DEnabled)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		setTransform(ETS_TEXTURE_0, core::IdentityMatrix);

		if (alphaChannel)
		{
			// if alpha and alpha texture just modulate, otherwise use only the alpha channel
			if (alpha)
			{
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
			else
			{
#if defined(GL_ARB_texture_env_combine) || defined(GL_EXT_texture_env_combine)
				if (FeatureAvailable[IRR_ARB_texture_env_combine]||FeatureAvailable[IRR_EXT_texture_env_combine])
				{
#ifdef GL_ARB_texture_env_combine
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
					// rgb always modulates
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PRIMARY_COLOR_ARB);
#else
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
					// rgb always modulates
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PRIMARY_COLOR_EXT);
#endif
				}
				else
#endif
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
		}
		else
		{
			if (alpha)
			{
#if defined(GL_ARB_texture_env_combine) || defined(GL_EXT_texture_env_combine)
				if (FeatureAvailable[IRR_ARB_texture_env_combine]||FeatureAvailable[IRR_EXT_texture_env_combine])
				{
#ifdef GL_ARB_texture_env_combine
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PRIMARY_COLOR_ARB);
					// rgb always modulates
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PRIMARY_COLOR_ARB);
#else
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_PRIMARY_COLOR_EXT);
					// rgb always modulates
					glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
					glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PRIMARY_COLOR_EXT);
#endif
				}
				else
#endif
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
			else
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
		}
	}

	CurrentRenderMode = ERM_2D;
}


//! \return Returns the name of the video driver. Example: In case of the Direct3D8
//! driver, it would return "COpenGL|ES".
const wchar_t* COpenGLESDriver::getName() const
{
	return Name.c_str();
}


//! deletes all dynamic lights there are
void COpenGLESDriver::deleteAllDynamicLights()
{
	for (s32 i=0; i<LastSetLight+1; ++i)
		glDisable(GL_LIGHT0 + i);

	LastSetLight = -1;

	CNullDriver::deleteAllDynamicLights();
}


//! adds a dynamic light
s32 COpenGLESDriver::addDynamicLight(const SLight& light)
{
	if (LastSetLight == MaxLights-1)
	{		
		return -1;
	}

	setTransform(ETS_WORLD, core::matrix4());

	++LastSetLight;
	CNullDriver::addDynamicLight(light);

	s32 lidx = GL_LIGHT0 + LastSetLight;
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
		data[0] = -light.Direction.X;
		data[1] = -light.Direction.Y;
		data[2] = -light.Direction.Z;
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

	glEnable(lidx);
	return lidx;
}


//! returns the maximal amount of dynamic lights the device can handle
u32 COpenGLESDriver::getMaximalDynamicLightAmount() const
{
	return MaxLights;
}


//! Sets the dynamic ambient light color. The default color is
//! (0,0,0,0) which means it is dark.
//! \param color: New color of the ambient light.
void COpenGLESDriver::setAmbientLight(const SColorf& color)
{
	GLfloat data[4] = {color.r, color.g, color.b, color.a};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, data);
}


// this code was sent in by Oliver Klems, thank you! (I modified the glViewport
// method just a bit.
void COpenGLESDriver::setViewPort(const core::rect<s32>& area)
{
	core::rect<s32> vp = area;
	core::rect<s32> rendert(0,0, getCurrentRenderTargetSize().Width, getCurrentRenderTargetSize().Height);
	vp.clipAgainst(rendert);

	if (vp.getHeight()>0 && vp.getWidth()>0)
		glViewport(vp.UpperLeftCorner.X,
		            getCurrentRenderTargetSize().Height - vp.UpperLeftCorner.Y - vp.getHeight(),
			          vp.getWidth(), vp.getHeight());

	ViewPort = vp;
}

//! Draws a shadow volume into the stencil buffer. To draw a stencil shadow, do
//! this: First, draw all geometry. Then use this method, to draw the shadow
//! volume. Next use IVideoDriver::drawStencilShadow() to visualize the shadow.
void COpenGLESDriver::drawStencilShadowVolume(const core::vector3df* triangles, s32 count, bool zfail)
{
	if (!StencilBuffer || !count)
		return;

	// unset last 3d material
	if (CurrentRenderMode == ERM_3D &&
	Material.MaterialType >= 0 && Material.MaterialType < static_cast<s32>(MaterialRenderers.size()))
	{
		MaterialRenderers[Material.MaterialType].Renderer->OnUnsetMaterial();
		ResetRenderStates = true;
	}

	// store current OpenGL state
	GLboolean bLighting = glIsEnabled(GL_LIGHTING);
	GLboolean bFog      = glIsEnabled(GL_FOG);
	GLboolean bStencil  = glIsEnabled(GL_STENCIL_TEST);
	GLboolean bCull     = glIsEnabled(GL_CULL_FACE);
	GLboolean bDepthMask;
	glGetBooleanv(GL_DEPTH_WRITEMASK, &bDepthMask);
	GLboolean bColorMask[4];
	glGetBooleanv(GL_COLOR_WRITEMASK, bColorMask);
	GLint depthFunc;
	glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
	GLint stencilFunc, stencilRef, stencilMask;
	glGetIntegerv(GL_STENCIL_FUNC, &stencilFunc);
	glGetIntegerv(GL_STENCIL_REF, &stencilRef);
	glGetIntegerv(GL_STENCIL_VALUE_MASK, &stencilMask);
	GLint stencilFail, stencilZFail, stencilZPass;
	glGetIntegerv(GL_STENCIL_FAIL, &stencilFail);
	glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL,&stencilZFail);
	glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS,&stencilZPass);
	GLint cullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &cullFaceMode);

	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE); // no depth buffer writing
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE ); // no color buffer drawing
	glEnable(GL_STENCIL_TEST);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,sizeof(core::vector3df),&triangles[0]);
	glStencilMask(~0);
	glStencilFunc(GL_ALWAYS, 0, ~0);


	{
		glEnable(GL_CULL_FACE);
		if (!zfail)
		{
			// ZPASS Method

			glCullFace(GL_BACK);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glDrawArrays(GL_TRIANGLES,0,count);

			glCullFace(GL_FRONT);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
			glDrawArrays(GL_TRIANGLES,0,count);
		}
		else
		{
			// ZFAIL Method

			glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
			glCullFace(GL_FRONT);
			glDrawArrays(GL_TRIANGLES,0,count);

			glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
			glCullFace(GL_BACK);
			glDrawArrays(GL_TRIANGLES,0,count);
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY); //not stored on stack


	if(bLighting) 
		glEnable(GL_LIGHTING);
	else 
		glDisable(GL_LIGHTING);

	if(bFog) 
		glEnable(GL_FOG);
	else 
		glDisable(GL_FOG);

	if(bStencil) 
		glEnable(GL_STENCIL_TEST);
	else 
		glDisable(GL_STENCIL_TEST);

	if(bCull) 
		glEnable(GL_CULL_FACE);
	else 
		glDisable(GL_CULL_FACE);

	glDepthMask(bDepthMask);
	glDepthFunc(depthFunc);
	glColorMask(bColorMask[0],bColorMask[1],bColorMask[2],bColorMask[3]);
	glStencilOp(stencilFail, stencilZFail, stencilZPass);
	glStencilFunc(stencilFunc,stencilRef,stencilMask);
	glCullFace(cullFaceMode);

}



void COpenGLESDriver::drawStencilShadow(bool clearStencilBuffer, video::SColor leftUpEdge,
	video::SColor rightUpEdge, video::SColor leftDownEdge, video::SColor rightDownEdge)
{
	if (!StencilBuffer)
		return;

	disableTextures();

	// store attributes
	GLboolean bLighting = glIsEnabled(GL_LIGHTING);
	GLboolean bFog      = glIsEnabled(GL_FOG);
	GLboolean bStencil  = glIsEnabled(GL_STENCIL_TEST);
	GLboolean bBlend    = glIsEnabled(GL_BLEND);
	GLboolean bDepthMask;
	glGetBooleanv(GL_DEPTH_WRITEMASK, &bDepthMask);
	GLboolean bColorMask[4];
	glGetBooleanv(GL_COLOR_WRITEMASK, bColorMask);
	GLint depthFunc;
	glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
	GLint stencilFunc, stencilRef, stencilMask;
	glGetIntegerv(GL_STENCIL_FUNC, &stencilFunc);
	glGetIntegerv(GL_STENCIL_REF, &stencilRef);
	glGetIntegerv(GL_STENCIL_VALUE_MASK, &stencilMask);
	GLint stencilFail, stencilZFail, stencilZPass;
	glGetIntegerv(GL_STENCIL_FAIL, &stencilFail);
	glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL,&stencilZFail);
	glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS,&stencilZPass);
	GLint shadeModel;
	glGetIntegerv(GL_SHADE_MODEL, &shadeModel);
	GLint frontFace;
	glGetIntegerv(GL_FRONT_FACE, &frontFace);
	GLint blendSrc, blendDst;
	glGetIntegerv(GL_BLEND_SRC, &blendSrc);
	glGetIntegerv(GL_BLEND_DST, &blendDst);


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

	// draw a shadow rectangle covering the entire screen using stencil buffer
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	GLfloat points[3*4] = {
		-1.1f, 1.1f, 0.90f,
		-1.1f,-1.1f,0.90f,
		1.1f, 1.1f,0.90f,
		1.1f,-1.1f,0.90f,
	};
	GLubyte colors[4*4];
	leftUpEdge.toOpenGLColor(&colors[0]);
	leftDownEdge.toOpenGLColor(&colors[4]);
	rightUpEdge.toOpenGLColor(&colors[8]);
	rightDownEdge.toOpenGLColor(&colors[12]);

	glVertexPointer(3, GL_FLOAT, 0, points);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);


	if (clearStencilBuffer)
		glClear(GL_STENCIL_BUFFER_BIT);

	// restore settings
	glPopMatrix();

	if(bLighting) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);

	if(bFog) glEnable(GL_FOG);
	else glDisable(GL_FOG);

	if(bStencil) glEnable(GL_STENCIL_TEST);
	else glDisable(GL_STENCIL_TEST);

	if(bBlend) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);

	glDepthMask(bDepthMask);
	glDepthFunc(depthFunc);
	glShadeModel(shadeModel);
	glFrontFace(frontFace);
	glBlendFunc(blendSrc, blendDst);
	glColorMask(bColorMask[0],bColorMask[1],bColorMask[2],bColorMask[3]);
	glStencilOp(stencilFail, stencilZFail, stencilZPass);
	glStencilFunc(stencilFunc,stencilRef,stencilMask);

}


//! Sets the fog mode.
void COpenGLESDriver::setFog(SColor c, bool linearFog, f32 start,
			f32 end, f32 density, bool pixelFog, bool rangeFog)
{
	CNullDriver::setFog(c, linearFog? EFT_FOG_LINEAR:EFT_FOG_EXP, start, end, density, pixelFog, rangeFog);

	glFogf(GL_FOG_MODE, (GLfloat)(linearFog ? GL_LINEAR : GL_EXP) );

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
	glFogfv(GL_FOG_COLOR, data);
}



//! Draws a 3d line.
void COpenGLESDriver::draw3DLine(const core::vector3df& start,
				const core::vector3df& end, SColor color)
{
	setRenderStates3DMode();

	static GLfloat points[3*2];

	points[0] = start.X;
	points[1] = start.Y;
	points[2] = start.Z;

	points[3] = end.X;
	points[4] = end.Y;
	points[5] = end.Z;

	glColor4ub(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

	glVertexPointer(3, GL_FLOAT, 0, points);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);
}



//! Only used by the internal engine. Used to notify the driver that
//! the window was resized.
void COpenGLESDriver::OnResize(const core::dimension2d<u32>& size)
{
	CNullDriver::OnResize(size);
	glViewport(0, 0, size.Width, size.Height);
}



//! Returns type of video driver
E_DRIVER_TYPE COpenGLESDriver::getDriverType() const
{
	return EDT_OPENGLES;
}



//! Sets a vertex shader constant.
void COpenGLESDriver::setVertexShaderConstant(const f32* data, s32 startRegister, s32 constantAmount)
{
#ifdef IRR_HAS_SHADER
#ifdef GL_ARB_vertex_program
	for (s32 i=0; i<constantAmount; ++i)
		extGlProgramLocalParameter4fv(GL_VERTEX_PROGRAM_ARB, startRegister+i, &data[i*4]);
#endif
#endif
}

//! Sets a pixel shader constant.
void COpenGLESDriver::setPixelShaderConstant(const f32* data, s32 startRegister, s32 constantAmount)
{
#ifdef IRR_HAS_SHADER
#ifdef GL_ARB_fragment_program
	for (s32 i=0; i<constantAmount; ++i)
		extGlProgramLocalParameter4fv(GL_FRAGMENT_PROGRAM_ARB, startRegister+i, &data[i*4]);
#endif
#endif
}

//! Sets a constant for the vertex shader based on a name.
bool COpenGLESDriver::setVertexShaderConstant(const c8* name, const f32* floats, int count)
{
#ifdef IRR_HAS_SHADER
	//pass this along, as in GLSL the same routine is used for both vertex and fragment shaders
	return setPixelShaderConstant(name, floats, count);
#endif
  return false;
}

//! Sets a vertex shader const
bool COpenGLESDriver::setVertexShaderConstant(const c8* name, int value )
{
	return false;
}

//! Sets a constant for the pixel shader based on a name.
bool COpenGLESDriver::setPixelShaderConstant(const c8* name, const f32* floats, int count)
{
#ifdef IRR_HAS_SHADER
	os::Printer::log("Error: Please call services->setPixelShaderConstant(), not VideoDriver->setPixelShaderConstant().");
	return false;
#endif
  return false;
}


#ifdef IRR_HAS_SHADER
//! Adds a new material renderer to the VideoDriver, using pixel and/or
//! vertex shaders to render geometry.
s32 COpenGLESDriver::addShaderMaterial(const c8* vertexShaderProgram,
	const c8* pixelShaderProgram,
	IShaderConstantSetCallBack* callback,
	E_MATERIAL_TYPE baseMaterial, s32 userData)
{
	s32 nr = -1;
	COpenGLShaderMaterialRenderer* r = new COpenGLShaderMaterialRenderer(
		this, nr, vertexShaderProgram, pixelShaderProgram,
		callback, getMaterialRenderer(baseMaterial), userData);

	r->drop();
	return nr;
}

//! Adds a new material renderer to the VideoDriver, using GLSL to render geometry.
s32 COpenGLESDriver::addHighLevelShaderMaterial(
	const c8* vertexShaderProgram,
	const c8* vertexShaderEntryPointName,
	E_VERTEX_SHADER_TYPE vsCompileTarget,
	const c8* pixelShaderProgram,
	const c8* pixelShaderEntryPointName,
	E_PIXEL_SHADER_TYPE psCompileTarget,
	IShaderConstantSetCallBack* callback,
	E_MATERIAL_TYPE baseMaterial,
	s32 userData)
{
	s32 nr = -1;

	COpenGLSLMaterialRenderer* r = new COpenGLSLMaterialRenderer(
		this, nr, vertexShaderProgram, vertexShaderEntryPointName,
		vsCompileTarget, pixelShaderProgram, pixelShaderEntryPointName, psCompileTarget,
		callback,getMaterialRenderer(baseMaterial), userData);

	r->drop();
	return nr;
}
#endif

//! Returns a pointer to the IVideoDriver interface. (Implementation for
//! IMaterialRendererServices)
IVideoDriver* COpenGLESDriver::getVideoDriver()
{
	return this;
}

#ifdef IRR_HAS_SHADER
//! Returns pointer to the IGPUProgrammingServices interface.
IGPUProgrammingServices* COpenGLESDriver::getGPUProgrammingServices()
{
	return this;
}
#endif

ITexture* COpenGLESDriver::createRenderTargetTexture(const core::dimension2d<u32>& size, const c8* name)
{
	//disable mip-mapping
	bool generateMipLevels = getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

	video::ITexture* rtt = 0;
	if (name==0)
		name="rt";
#if defined(GL_EXT_framebuffer_object)
	// if driver supports FrameBufferObjects, use them
	if (queryFeature(EVDF_FRAMEBUFFER_OBJECT))
        	rtt = new COpenGLTexture(size, PackedDepthStencilExtension, name, this);
	else
#endif
	{
		rtt = addTexture(size, name);
		if (rtt)
		{
			rtt->grab();
			static_cast<video::COpenGLESTexture*>(rtt)->setRenderTarget(true);
		}
	}

	//restore mip-mapping
	setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, generateMipLevels);

	return rtt;
}


//! Returns the maximum amount of primitives (mostly vertices) which
//! the device is able to render with one drawIndexedTriangleList
//! call.
u32 COpenGLESDriver::getMaximalPrimitiveCount() const
{
	return 65535;// TODO: Fix all loaders to auto-split and then return the correct value: MaxIndices;
}


//! checks triangle count and print warning if wrong
bool COpenGLESDriver::setRenderTarget(video::ITexture* texture, bool clearBackBuffer,
								 bool clearZBuffer, SColor color)
{
	// check for right driver type

	if (texture && texture->getDriverType() != EDT_OPENGLES)
	{
		os::Printer::log("Fatal Error: Tried to set a texture not owned by this driver.", ELL_ERROR);
		return false;
	}

	// check if we should set the previous RT back

	bool ret = true;

	setTexture(0, 0);
	ResetRenderStates=true;
	if (RenderTargetTexture!=0)
	{
		if (RenderTargetTexture->isFrameBufferObject())
		{
			RenderTargetTexture->unbindFrameBufferObject();
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, RenderTargetTexture->getOpenGLTextureName());

			// Copy Our ViewPort To The Texture
			glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,
			RenderTargetTexture->getSize().Width, RenderTargetTexture->getSize().Height);
		}
	}

	if (texture)
	{
		// we want to set a new target. so do this.
		glViewport(0, 0, texture->getSize().Width, texture->getSize().Height);
		RenderTargetTexture = static_cast<COpenGLESTexture*>(texture);
		CurrentRendertargetSize = texture->getSize();

		if (RenderTargetTexture->isFrameBufferObject())
		{
			RenderTargetTexture->bindFrameBufferObject();
		}
	}
	else
	{
		glViewport(0,0,ScreenSize.Width,ScreenSize.Height);
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
		mask |= GL_DEPTH_BUFFER_BIT;
	}

	glClear(mask);

	return ret;
}


// returns the current size of the screen or rendertarget
const core::dimension2d<u32>& COpenGLESDriver::getCurrentRenderTargetSize() const
{
	if ( CurrentRendertargetSize.Width == 0 )
		return ScreenSize;
	else
		return CurrentRendertargetSize;
}

//! Clears the ZBuffer.
void COpenGLESDriver::clearZBuffer()
{
   GLboolean enabled = GL_TRUE;
   glGetBooleanv(GL_DEPTH_WRITEMASK, &enabled);

   glDepthMask(GL_TRUE);
   glClear(GL_DEPTH_BUFFER_BIT);

   glDepthMask(enabled);
}

//! Returns an image created from the last rendered frame.
IImage* COpenGLESDriver::createScreenShot()
{
	IImage* newImage = new CImage(ECF_R8G8B8, ScreenSize);

	u8* pPixels = reinterpret_cast<u8*>(newImage->lock());
	if (!pPixels)
	{
		newImage->drop();
		return 0;
	}

	glReadPixels(0, 0, ScreenSize.Width, ScreenSize.Height, GL_RGB, GL_UNSIGNED_BYTE, pPixels);

	// opengl images are inverted, so we have to fix that here.
	s32 pitch=newImage->getPitch();
	u8* p2 = pPixels + (ScreenSize.Height - 1) * pitch;
	u8* tmpBuffer = new u8[pitch];
	for (u32 i=0; i < ScreenSize.Height; i += 2)
	{
		memcpy(tmpBuffer, pPixels, pitch);
		memcpy(pPixels, p2, pitch);
		memcpy(p2, tmpBuffer, pitch);
		pPixels += pitch;
		p2 -= pitch;
	}
	delete [] tmpBuffer;

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
bool COpenGLESDriver::setClipPlane(u32 index, const core::plane3df& plane, bool enable)
{
	if (index >= MaxUserClipPlanes)
		return false;

	UserClipPlane[index]=plane;
	enableClipPlane(index, enable);
	return true;
}


typedef float ClipPlaneArgType;
#define glClipPlane glClipPlanef

void COpenGLESDriver::uploadClipPlane(u32 index)
{
	// opengl needs an array of doubles for the plane equation
	ClipPlaneArgType clip_plane[4];
	clip_plane[0] = UserClipPlane[index].Normal.X;
	clip_plane[1] = UserClipPlane[index].Normal.Y;
	clip_plane[2] = UserClipPlane[index].Normal.Z;
	clip_plane[3] = UserClipPlane[index].D;
	glClipPlane(GL_CLIP_PLANE0 + index, clip_plane);
}

//! Enable/disable a clipping plane.
//! There are at least 6 clipping planes available for the user to set at will.
//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
//! \param enable: If true, enable the clipping plane else disable it.
void COpenGLESDriver::enableClipPlane(u32 index, bool enable)
{
	if (index >= MaxUserClipPlanes)
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

	UserClipPlaneEnabled[index]=enable;
}


} // end namespace
} // end namespace

#endif // _IRR_COMPILE_WITH_OPENGL_ES_

namespace irr
{
namespace video
{


// -----------------------------------
// WINDOWS VERSION
// -----------------------------------
#ifdef _IRR_COMPILE_WITH_WINDOWS_DEVICE_

IVideoDriver* createOpenGLESDriver(const irr::SIrrlichtCreationParameters& params, io::IFileSystem* io)
{
#ifdef _IRR_COMPILE_WITH_OPENGL_ES_
	COpenGLESDriver* ogl = new COpenGLESDriver
		(
			params.WindowSize, 
			params.Fullscreen, 
			params.Stencilbuffer, 
			(HWND)params.WindowId, 
			io, 
			(bool)(params.AntiAlias > 0),
			params.Vsync
		);
	return ogl;
#else
	return NULL;
#endif
}
#endif


} // end namespace
} // end namespace


