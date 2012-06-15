// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_VIDEO_OPEN_GL_H_INCLUDED__
#define __C_VIDEO_OPEN_GL_H_INCLUDED__

#include "IrrCompileConfig.h"

#if defined(_IRR_WINDOWS_API_)
	// include windows headers for HWND
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#elif defined(_IRR_USE_OSX_DEVICE_)
	#include "CopenGLo.h"
	#include "CIrrDeviceMacOSX.h"
#elif defined(_IRR_USE_IPHONEOS_DEVICE_)
	#include "CIrrDeviceIPhoneOS.h"
#endif

#ifndef GL_IMG_texture_compression_pvrtc
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG                      0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG                      0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG                     0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG                     0x8C03
#endif

#include "CNullDriver.h"
#include "IMaterialRendererServices.h"
#include "COpenGLExtensionHandler.h"
#include "SIrrCreationParameters.h"

#ifdef _IRR_COMPILE_WITH_OPENGL_

#if defined(_IRR_WINDOWS_API_)
#ifdef _IRR_COMPILE_WITH_OPENGL_ES2_
	#include <EGL/egl.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#ifdef _MSC_VER
	#pragma comment(lib, "libEGL.lib")
	#pragma comment(lib, "libGLESv2.lib")
#endif
#else
	#include <GL/gl.h>
	#include "glext.h"
#ifdef _MSC_VER
	#pragma comment(lib, "OpenGL32.lib")
	#pragma comment(lib, "GLu32.lib")
#endif
#endif
#elif defined(_IRR_USE_IPHONEOS_DEVICE_)
	#include <OpenGLES/ES2/gl.h>
#elif defined(_IRR_USE_OSX_DEVICE_)
	#if defined(_IRR_OPENGL_USE_EXTPOINTER_)
		#define GL_GLEXT_LEGACY 1
	#endif
	#include <OpenGL/gl.h>
	#if defined(_IRR_OPENGL_USE_EXTPOINTER_)
		#include "glext.h"
	#endif
#elif defined(_IRR_USE_SDL_DEVICE_)
	#if defined(_IRR_OPENGL_USE_EXTPOINTER_)
		#define GL_GLEXT_LEGACY 1
		#define GLX_GLXEXT_LEGACY 1
	#else
		#define GL_GLEXT_PROTOTYPES 1
		#define GLX_GLXEXT_PROTOTYPES 1
	#endif
	#define NO_SDL_GLEXT
	#include <SDL/SDL_opengl.h>
	#include "glext.h"
#else
	#if defined(_IRR_OPENGL_USE_EXTPOINTER_)
		#define GL_GLEXT_LEGACY 1
		#define GLX_GLXEXT_LEGACY 1
	#else
		#define GL_GLEXT_PROTOTYPES 1
		#define GLX_GLXEXT_PROTOTYPES 1
	#endif
	#include <GL/gl.h>
	#include <GL/glx.h>
	#if defined(_IRR_OPENGL_USE_EXTPOINTER_)
	#include "glext.h"
	#undef GLX_ARB_get_proc_address // avoid problems with local glxext.h
	#include "glxext.h"
	#endif
#endif

#include "irrTexGen.h"

namespace irr
{
namespace video
{
	class COpenGLTexture;

	class COpenGLDriver : public CNullDriver, public IMaterialRendererServices, public COpenGLExtensionHandler
	{
	public:

		#ifdef _IRR_WINDOWS_API_
		//! win32 constructor
		COpenGLDriver(const core::dimension2d<s32>& screenSize, HWND window,
			bool stencilBuffer, io::IFileSystem* io, bool antiAlias);

		//! inits the windows specific parts of the open gl driver
		bool initDriver(const core::dimension2d<s32>& screenSize, HWND window,
			u32 bits, bool vsync, bool stencilBuffer);
		#endif

		#if defined(_IRR_USE_LINUX_DEVICE_) || defined(_IRR_USE_SDL_DEVICE_)
		COpenGLDriver(const SIrrlichtCreationParameters& params, io::IFileSystem* io);
		#endif

		#ifdef _IRR_USE_OSX_DEVICE_
		COpenGLDriver(const SIrrlichtCreationParameters& params,
				io::IFileSystem* io, CIrrDeviceMacOSX *device);
		#endif

		#ifdef _IRR_USE_IPHONEOS_DEVICE_
		COpenGLDriver(const SIrrlichtCreationParameters& params,
				io::IFileSystem* io, CIrrDeviceIPhoneOS *device);
		#endif

		//! destructor
		virtual ~COpenGLDriver();

		//! clears the zbuffer
		virtual bool beginScene(bool backBuffer=true, bool zBuffer=true,
				SColor color=SColor(255,0,0,0),
				void* windowId=0,
				core::rect<s32>* sourceRect=0);

		//! presents the rendered scene on the screen, returns false if failed
		virtual bool endScene();

		virtual bool beginScene2D(bool resetStates = true);

		virtual bool endScene2D();

		//! sets transformation
		virtual void setTransform(E_TRANSFORMATION_STATE state, const core::matrix4& mat);

		struct SHWBufferLink_opengl : public SHWBufferLink
		{
			SHWBufferLink_opengl(const scene::IMeshBuffer *_MeshBuffer): SHWBufferLink(_MeshBuffer), vbo_verticesID(0),vbo_indicesID(0){}

			GLuint vbo_verticesID; //tmp
			GLuint vbo_indicesID; //tmp

			GLuint vbo_verticesSize; //tmp
			GLuint vbo_indicesSize; //tmp

		};

		bool updateVertexHardwareBuffer(SHWBufferLink_opengl *HWBuffer);
		bool updateIndexHardwareBuffer(SHWBufferLink_opengl *HWBuffer);

		//! updates hardware buffer if needed
		virtual bool updateHardwareBuffer(SHWBufferLink *HWBuffer);

		//! Create hardware buffer from mesh
		virtual SHWBufferLink *createHardwareBuffer(const scene::IMeshBuffer* mb);

		//! Delete hardware buffer (only some drivers can)
		virtual void deleteHardwareBuffer(SHWBufferLink *HWBuffer);

		//! Draw hardware buffer
		virtual void drawHardwareBuffer(SHWBufferLink *HWBuffer);

		//! draws a vertex primitive list
		virtual void drawVertexPrimitiveList(const void* vertices, u32 vertexCount,
				const void* indexList, u32 primitiveCount,
				E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType);

		//! queries the features of the driver, returns true if feature is available
		virtual bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const
		{
			return FeatureEnabled[feature] && COpenGLExtensionHandler::queryFeature(feature);
		}

		//! Sets a material. All 3d drawing functions draw geometry now
		//! using this material.
		//! \param material: Material to be used from now on.
		virtual void setMaterial(const SMaterial& material);

		//! draws an 2d image, using a color (if color is other then Color(255,255,255,255)) and the alpha channel of the texture if wanted.
		virtual void draw2DImage(const video::ITexture* texture, const core::position2d<s32>& destPos,
			const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect = 0,
			SColor color=SColor(255,255,255,255), bool useAlphaChannelOfTexture=false);

		//! draws a set of 2d images, using a color and the alpha
		/** channel of the texture if desired. The images are drawn
		beginning at pos and concatenated in one line. All drawings
		are clipped against clipRect (if != 0).
		The subtextures are defined by the array of sourceRects
		and are chosen by the indices given.
		\param texture: Texture to be drawn.
		\param pos: Upper left 2d destination position where the image will be drawn.
		\param sourceRects: Source rectangles of the image.
		\param indices: List of indices which choose the actual rectangle used each time.
		\param clipRect: Pointer to rectangle on the screen where the image is clipped to.
		This pointer can be 0. Then the image is not clipped.
		\param color: Color with which the image is colored.
		Note that the alpha component is used: If alpha is other than 255, the image will be transparent.
		\param useAlphaChannelOfTexture: If true, the alpha channel of the texture is
		used to draw the image. */
		virtual void draw2DImage(const video::ITexture* texture,
				const core::position2d<s32>& pos,
				const core::array<core::rect<s32> >& sourceRects,
				const core::array<s32>& indices,
				const core::rect<s32>* clipRect=0,
				SColor color=SColor(255,255,255,255),
				bool useAlphaChannelOfTexture=false);

		//! Draws a part of the texture into the rectangle.
		virtual void draw2DImage(const video::ITexture* texture, const core::rect<s32>& destRect,
			const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect = 0,
			const video::SColor* const colors=0, bool useAlphaChannelOfTexture=false);

		//! Draws a part of the texture into the rectangle.
		virtual void draw2DImage(const video::ITexture* texture, const core::rect<s32>& destRect,
			const core::rect<s32>& sourceRect, const core::matrix4& transform, const core::rect<s32>* clipRect = 0,
			const video::SColor* const colors=0, bool useAlphaChannelOfTexture=false);


		//! draw an 2d rectangle
		virtual void draw2DRectangle(SColor color, const core::rect<s32>& pos,
			const core::rect<s32>* clip = 0);

		//!Draws an 2d rectangle with a gradient.
		virtual void draw2DRectangle(const core::rect<s32>& pos,
			SColor colorLeftUp, SColor colorRightUp, SColor colorLeftDown, SColor colorRightDown,
			const core::rect<s32>* clip = 0);

		//! Draws a 2d line.
		virtual void draw2DLine(const core::position2d<s32>& start,
					const core::position2d<s32>& end,
					SColor color=SColor(255,255,255,255));

		//! Draws a 3d line.
		virtual void draw3DLine(const core::vector3df& start,
					const core::vector3df& end,
					SColor color = SColor(255,255,255,255));

		//! \return Returns the name of the video driver. Example: In case of the Direct3D8
		//! driver, it would return "Direct3D8.1".
		virtual const wchar_t* getName() const;

		//! deletes all dynamic lights there are
		virtual void deleteAllDynamicLights();

		//! adds a dynamic light
		virtual void addDynamicLight(const SLight& light);

		//! returns the maximal amount of dynamic lights the device can handle
		virtual u32 getMaximalDynamicLightAmount() const;

		//! Sets the dynamic ambient light color. The default color is
		//! (0,0,0,0) which means it is dark.
		//! \param color: New color of the ambient light.
		virtual void setAmbientLight(const SColorf& color);

		//! Draws a shadow volume into the stencil buffer. To draw a stencil shadow, do
		//! this: First, draw all geometry. Then use this method, to draw the shadow
		//! volume. Then, use IVideoDriver::drawStencilShadow() to visualize the shadow.
		virtual void drawStencilShadowVolume(const core::vector3df* triangles, s32 count, bool zfail);

		//! Fills the stencil shadow with color. After the shadow volume has been drawn
		//! into the stencil buffer using IVideoDriver::drawStencilShadowVolume(), use this
		//! to draw the color of the shadow.
		virtual void drawStencilShadow(bool clearStencilBuffer=false,
			video::SColor leftUpEdge = video::SColor(0,0,0,0),
			video::SColor rightUpEdge = video::SColor(0,0,0,0),
			video::SColor leftDownEdge = video::SColor(0,0,0,0),
			video::SColor rightDownEdge = video::SColor(0,0,0,0));

		//! sets a viewport
		virtual void setViewPort(const core::rect<s32>& area);

		//! Sets the fog mode.
		virtual void setFog(SColor color, bool linearFog, f32 start,
			f32 end, f32 density, bool pixelFog, bool rangeFog);

		//! Only used by the internal engine. Used to notify the driver that
		//! the window was resized.
		virtual void OnResize(const core::dimension2d<s32>& size);
		virtual void OnResize(const core::rect<s32>& size);

		//! Returns type of video driver
		virtual E_DRIVER_TYPE getDriverType() const;

		//! get color format of the current color buffer
		virtual ECOLOR_FORMAT getColorFormat() const;

		//! Returns the transformation set by setTransform
		virtual const core::matrix4& getTransform(E_TRANSFORMATION_STATE state) const;

		const core::matrix4& getWorldViewProjectionMatrix();
		const core::matrix4& getWorldViewMatrix();
		const core::matrix4& getOrthoMatrix();

		const core::matrix4& getWorldMatrix();
		const core::matrix4& getViewMatrix();

		//! Can be called by an IMaterialRenderer to make its work easier.
		virtual void setBasicRenderStates(const SMaterial& material, const SMaterial& lastmaterial,
			bool resetAllRenderstates);

		//! Sets a vertex shader constant.
		virtual void setVertexShaderConstant(const f32* data, s32 startRegister, s32 constantAmount=1);

		//! Sets a pixel shader constant.
		virtual void setPixelShaderConstant(const f32* data, s32 startRegister, s32 constantAmount=1);

		//! Sets a constant for the vertex shader based on a name.
		virtual bool setVertexShaderConstant(const c8* name, const f32* floats, int count);

		//! Sets a constant for the pixel shader based on a name.
		virtual bool setPixelShaderConstant(const c8* name, const f32* floats, int count);

		//! sets the current Texture
		//! Returns whether setting was a success or not.
		IRRLICHT_API bool setTexture(u32 stage, const video::ITexture* texture);

		//! disables all textures beginning with the optional fromStage parameter. Otherwise all texture stages are disabled.
		//! Returns whether disabling was successful or not.
		IRRLICHT_API bool disableTextures(u32 fromStage=0);

		//! Adds a new material renderer to the VideoDriver, using
		//! extGLGetObjectParameteriv(shaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &status)
		//! pixel and/or vertex shaders to render geometry.
		virtual s32 addShaderMaterial(const c8* vertexShaderProgram, const c8* pixelShaderProgram,
			IShaderConstantSetCallBack* callback, E_MATERIAL_TYPE baseMaterial, s32 userData);

		//! Adds a new material renderer to the VideoDriver, using GLSL to render geometry.
		virtual s32 addHighLevelShaderMaterial(const c8* vertexShaderProgram, const c8* vertexShaderEntryPointName,
			E_VERTEX_SHADER_TYPE vsCompileTarget, const c8* pixelShaderProgram, const c8* pixelShaderEntryPointName,
			E_PIXEL_SHADER_TYPE psCompileTarget, IShaderConstantSetCallBack* callback, E_MATERIAL_TYPE baseMaterial,
			const c8** defines,
			s32 userData);

		//! Returns pointer to the IGPUProgrammingServices interface.
		virtual IGPUProgrammingServices* getGPUProgrammingServices();

		//! Returns a pointer to the IVideoDriver interface. (Implementation for
		//! IMaterialRendererServices)
		virtual IVideoDriver* getVideoDriver();

		//! Returns the maximum amount of primitives (mostly vertices) which
		//! the device is able to render with one drawIndexedTriangleList
		//! call.
		virtual u32 getMaximalPrimitiveCount() const;

		virtual ITexture* addRenderTargetTexture(const core::dimension2d<s32>& size,
				const c8* name);

		ITexture* addRenderTargetTexture(const core::dimension2d<s32>& size,
				const c8* name, bool colorTexture=true, bool depthTexture=false);

		virtual bool setRenderTarget(video::ITexture* texture, bool clearBackBuffer,
					bool clearZBuffer, SColor color);

		//! Clears the ZBuffer.
		virtual void clearZBuffer();

		//! Returns an image created from the last rendered frame.
		virtual IImage* createScreenShot();

		//! checks if an OpenGL error has happend and prints it
		//! for performance reasons only available in debug mode
		bool testGLError();

		//! Set/unset a clipping plane.
		//! There are at least 6 clipping planes available for the user to set at will.
		//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
		//! \param plane: The plane itself.
		//! \param enable: If true, enable the clipping plane else disable it.
		virtual bool setClipPlane(u32 index, const core::plane3df& plane, bool enable=false);

		//! Enable/disable a clipping plane.
		//! There are at least 6 clipping planes available for the user to set at will.
		//! \param index: The plane index. Must be between 0 and MaxUserClipPlanes.
		//! \param enable: If true, enable the clipping plane else disable it.
		virtual void enableClipPlane(u32 index, bool enable);

		//! Returns the graphics card vendor name.
		virtual core::stringc getVendorInfo() {return vendorName;};

		//! Returns the color buffer for opengl rendering
		virtual core::array<u8>* getColorBuffer() {return &ColorBuffer;}

		virtual void setOrientation3D(E_ORIENTATION newOri);
		virtual E_ORIENTATION getOrientation3D();        
		virtual void setScissor(const core::rect<s32>& scissorRect);

		virtual void resetScissor();

		//! Set alpha/color mask to write in color buffer.
		virtual void setColorMask(bool red, bool green, bool blue, bool alpha);

		void setTexEnvMode(GLenum mode);
		void setCombineRGB(GLenum mode);
		void setCombineAlpha(GLenum mode);
		void setSourceRGB(int i, GLenum src);
		void setSourceAlpha(int i, GLenum src);
		void setOperandRGB(int i, GLenum oper);
		void setOperandAlpha(int i, GLenum oper);
		void setRGBScale(GLfloat scale);
		void setAlphaScale(GLfloat scale);

		void setTexGen(int texunit, E_TEX_GEN_TYPE texgen);

		void forceCommitTexture(COpenGLTexture* texture);
		void setNeedForceCommitTexture(bool value);
		bool isNeedForceCommitTexture();

	private:
		//! set opengl material colors
		void setMaterialColor(const SMaterial& material);

		//! draws a vertex primitive list
		void drawVertexPrimitiveList(const void* vertices, u32 vertexCount,
				const void* indexList, u32 primitiveCount,
				E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType, bool fVBOFlag);

		void drawVertexPrimitiveList2d3d(const void* vertices, u32 vertexCount,
			const void* indexList, u32 primitiveCount,
			E_VERTEX_TYPE vType, scene::E_PRIMITIVE_TYPE pType, E_INDEX_TYPE iType, bool fVBOFlag);

		void uploadClipPlane(u32 index);

		//! inits the parts of the open gl driver used on all platforms
		bool genericDriverInit(const core::dimension2d<s32>& screenSize, bool stencilBuffer);
		//! returns a device dependent texture from a software surface (IImage)
		virtual video::ITexture* createDeviceDependentTexture(IImage* surface, const char* name);

		//! creates a transposed matrix in supplied GLfloat array to pass to OpenGL
		inline void createGLMatrix(GLfloat gl_matrix[16], const core::matrix4& m);
		inline void createGLTextureMatrix(GLfloat gl_matrix[16], const core::matrix4& m);

		//! Set GL pipeline to desired texture wrap modes of the material
		void setWrapMode(const SMaterial& material);

		//! sets the needed renderstates
		void setRenderStates3DMode();

		//! sets the needed renderstates
		void setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel, bool resetStates);

		// returns the current size of the screen or rendertarget
		virtual const core::dimension2d<s32>& getCurrentRenderTargetSize() const;

		void createMaterialRenderers();

		void updateMatrix();

		core::stringw Name;
		core::matrix4 Matrices[ETS_COUNT];
		core::array<u8> ColorBuffer;

		core::matrix4 WorldViewProjectionMatrix;
		core::matrix4 WorldViewMatrix;

		core::matrix4 OrthoMatrix;

		core::matrix4 LandscapeMatrix;
		core::matrix4 LandscapeInverseMatrix;
		
		bool NeedForceCommitTexture;

		//! enumeration for rendering modes such as 2d and 3d for minizing the switching of renderStates.
		enum E_RENDER_MODE
		{
			ERM_NONE = 0,	// no render state has been set yet.
			ERM_2D,		// 2d drawing rendermode
			ERM_3D		// 3d rendering mode
		};

		E_RENDER_MODE CurrentRenderMode;
        E_ORTHO_ORIENTATION CurrentOrientation3D;

		//! bool set by beginScene2D to reset renderstates or not when call to setRenderStates2D are made
		bool Reset2DRenderStates; 

		//! bool to make all renderstates reset if set to true.
		bool ResetRenderStates;
		bool Transformation3DChanged;
		bool AntiAlias;
		u32 IsTextureMatrixIdentity;

		SMaterial material2D;

		SMaterial Material, LastMaterial;
		COpenGLTexture* RenderTargetTexture;
		const ITexture* CurrentTexture[MATERIAL_MAX_TEXTURES];
		s32 LastSetLight;
		core::array<core::plane3df> UserClipPlane;
		core::array<bool> UserClipPlaneEnabled;
		int yViewPortOffset;

		core::dimension2d<s32> CurrentRendertargetSize;

		core::stringc vendorName;

		core::matrix4 TextureFlipMatrix;

#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		//! Implements a state shadow layer for caching some render states.
		struct SShadowState
		{
			// Add more as needed
			struct STexEnv
			{
			#if !defined(_IRR_COMPILE_WITH_OPENGL_ES2_)
				GLenum Mode;
				GLenum CombineRGB;
				GLenum CombineAlpha;

				struct SSource
				{
					GLenum RGB;
					GLenum Alpha;
					GLenum OperandRGB;
					GLenum OperandAlpha;

				} Source[3];
				
				GLfloat RGBScale;
				GLfloat AlphaScale;
			#endif

				STexEnv();

			} TexEnv[MATERIAL_MAX_TEXTURES];

			SShadowState();

		} ShadowState;
#endif

		//! Bit mask of which tex units have soft tex gen.
		int TexGenEnabled;
		//! Soft tex gen type per tex unit.
		E_TEX_GEN_TYPE TexGenType[MATERIAL_MAX_TEXTURES];

		//! Color buffer format
		ECOLOR_FORMAT ColorFormat;

		#ifdef _IRR_WINDOWS_API_
			HDC HDc; // Private GDI Device Context
			HWND Window;
			HGLRC HRc; // Permanent Rendering Context
		#elif defined(_IRR_USE_LINUX_DEVICE_)
			GLXDrawable Drawable;
		#elif defined(_IRR_USE_IPHONEOS_DEVICE_)
			CIrrDeviceIPhoneOS *_device;
		#elif defined(_IRR_USE_OSX_DEVICE_)
			CIrrDeviceMacOSX *_device;
		#endif
	};

	//! Changes GL_TEXTURE_ENV_MODE's value, if different than before
	inline void COpenGLDriver::setTexEnvMode(GLenum mode)
	{
//CT remove unused code
/*#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		SShadowState::STexEnv& texenv
			= ShadowState.TexEnv[ExtensionShadowState.ActiveTexture - GL_TEXTURE0];
		if (mode != texenv.Mode)
#endif
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
			texenv.Mode = mode;
#endif
		}*/
}

	//! Changes GL_COMBINE_RGB's value, if different than before
	inline void COpenGLDriver::setCombineRGB(GLenum mode)
	{
//CT remove unused code
/*#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		SShadowState::STexEnv& texenv
			= ShadowState.TexEnv[ExtensionShadowState.ActiveTexture - GL_TEXTURE0];
		if (mode != texenv.CombineRGB)
#endif
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, mode);
#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
			texenv.CombineRGB = mode;
#endif
		}*/
	}

	//! Changes GL_COMBINE_ALPHA's value, if different than before
	inline void COpenGLDriver::setCombineAlpha(GLenum mode)
	{
//CT remove unused code
/*#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		SShadowState::STexEnv& texenv
			= ShadowState.TexEnv[ExtensionShadowState.ActiveTexture - GL_TEXTURE0];
		if (mode != texenv.CombineAlpha)
#endif
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, mode);
#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
			texenv.CombineAlpha = mode;
#endif
		}*/
	}

	//! Changes GL_SOURCEi_RGB's value, if different than before
	inline void COpenGLDriver::setSourceRGB(int i, GLenum src)
	{
//CT remove unused code
/*#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		_IRR_DEBUG_BREAK_IF(i < 0 || i > 2);
		SShadowState::STexEnv::SSource& source
			= ShadowState.TexEnv[ExtensionShadowState.ActiveTexture - GL_TEXTURE0].Source[i];
		if (src != source.RGB)
#endif
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT + i, src);
#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
			source.RGB = src;
#endif
		}*/
	}

	//! Changes GL_SOURCEi_ALPHA's value, if different than before
	inline void COpenGLDriver::setSourceAlpha(int i, GLenum src)
	{
//CT remove unused code
/*#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		_IRR_DEBUG_BREAK_IF(i < 0 || i > 2);
		SShadowState::STexEnv::SSource& source
			= ShadowState.TexEnv[ExtensionShadowState.ActiveTexture - GL_TEXTURE0].Source[i];
		if (src != source.Alpha)
#endif
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT + i, src);
#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
			source.Alpha = src;
#endif
		}*/
	}

	//! Changes GL_OPERANDi_RGB's value, if different than before
	inline void COpenGLDriver::setOperandRGB(int i, GLenum oper)
	{
//CT remove unused code
/*#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		_IRR_DEBUG_BREAK_IF(i < 0 || i > 2);
		SShadowState::STexEnv::SSource& source
			= ShadowState.TexEnv[ExtensionShadowState.ActiveTexture - GL_TEXTURE0].Source[i];
		if (oper != source.OperandRGB)
#endif
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT + i, oper);
#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
			source.OperandRGB = oper;
#endif
		}*/
	}

	//! Changes GL_OPERANDi_ALPHA's value, if different than before
	inline void COpenGLDriver::setOperandAlpha(int i, GLenum oper)
	{
//CT remove unused code
/*#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		_IRR_DEBUG_BREAK_IF(i < 0 || i > 2);
		SShadowState::STexEnv::SSource& source
			= ShadowState.TexEnv[ExtensionShadowState.ActiveTexture - GL_TEXTURE0].Source[i];
		if (oper != source.OperandAlpha)
#endif
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_EXT + i, oper);
#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
			source.OperandAlpha = oper;
#endif
		}*/
	}

	//! Changes GL_RGB_SCALE's value, if different than before
	inline void COpenGLDriver::setRGBScale(GLfloat scale)
	{
//CT remove unused code
/*#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		SShadowState::STexEnv& texenv
			= ShadowState.TexEnv[ExtensionShadowState.ActiveTexture - GL_TEXTURE0];
		if (scale != texenv.RGBScale)
#endif
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, scale);
#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
			texenv.RGBScale = scale;
#endif
		}*/
	}

	//! Changes GL_ALPHA_SCALE's value, if different than before
	inline void COpenGLDriver::setAlphaScale(GLfloat scale)
	{
//CT remove unused code
/*#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
		SShadowState::STexEnv& texenv
			= ShadowState.TexEnv[ExtensionShadowState.ActiveTexture - GL_TEXTURE0];
		if (scale != texenv.AlphaScale)
#endif
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_ALPHA_SCALE, scale);
#ifdef _IRR_OPENGL_USE_SHADOW_STATE_
			texenv.AlphaScale = scale;
#endif
		}*/
	}

	//! Tells the driver to regenerate texture coordinates of given type.
	inline void COpenGLDriver::setTexGen(int texunit, E_TEX_GEN_TYPE texgen)
	{
		if (texunit < 1 || MultiTextureExtension)
		{
			TexGenType[texunit] = texgen;
			if (texgen)
			{
				TexGenEnabled |= (1 << texunit);
			}
			else
			{
				TexGenEnabled &= ~(1 << texunit);
			}
		}
	}

} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OPENGL_
#endif
