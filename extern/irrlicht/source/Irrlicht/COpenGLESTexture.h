// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_OPEN_GLES_TEXTURE_H_INCLUDED__
#define __C_OPEN_GLES_TEXTURE_H_INCLUDED__

#include "ITexture.h"
#include "IImage.h"

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_OPENGL_ES_

#ifdef _IRR_COMPILE_WITH_WINDOWS_DEVICE_  
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <GLES/egl.h>
#endif


namespace irr
{
namespace video
{

class COpenGLESDriver;


//! OpenGL texture.
class COpenGLESTexture : public ITexture
{
public:

	//! constructor
	COpenGLESTexture(IImage* surface, const char* name, COpenGLESDriver* driver=0);
	//! FrameBufferObject constructor (disabled for OpenGL|ES)
	COpenGLESTexture(const core::dimension2d<u32>& size, bool extPackedDepthStencilSupported, const char* name, COpenGLESDriver* driver=0);

	//! destructor
	virtual ~COpenGLESTexture();

	//! lock function
	virtual void* lock(bool readOnly = false, u32 mipmapLevel=0);

	//! unlock function
	virtual void unlock();

	//! Returns original size of the texture.
	virtual const core::dimension2d<u32>& getOriginalSize() const;

	//! Returns size of the texture.
	virtual const core::dimension2d<u32>& getSize() const;

	//! returns driver type of texture (=the driver, who created the texture)
	virtual E_DRIVER_TYPE getDriverType() const;

	//! returns color format of texture
	virtual ECOLOR_FORMAT getColorFormat() const;

	//! returns pitch of texture (in bytes)
	virtual u32 getPitch() const;

	//! return open gl texture name
	GLuint getOpenGLTextureName() const;

	//! return whether this texture has mipmaps
	virtual bool hasMipMaps() const;

	//! Regenerates the mip map levels of the texture. Useful after
	//! locking and modifying the texture
	virtual void regenerateMipMapLevels(void* mipmapData = 0);

	//! Is it a render target?
	virtual bool isRenderTarget() const;

	//! Is it a FrameBufferObject?
	bool isFrameBufferObject() const;

	//! Bind FrameBufferObject (valid only if isFrameBufferObject() returns true).
	void bindFrameBufferObject();

	//! Unbind FrameBufferObject (valid only if isFrameBufferObject() returns true).
	void unbindFrameBufferObject();

	//! sets whether this texture is intended to be used as a render target.
  void setRenderTarget(bool isTarget);

private:

	//! get the desired color format based on texture creation flags and the input format.
	ECOLOR_FORMAT getBestColorFormat(ECOLOR_FORMAT format);

	//! convert the image into an internal image with better properties for this driver.
	void getImageData(IImage* image);

	//! copies the the texture into an open gl texture.
	//! \param: newTexture is true if method is called from a newly created texture for the first time. Otherwise call with false to improve memory handling.
	void copyTexture(bool newTexture=true);

	//! returns the size of a texture which would be the optimize size for rendering it
	inline u32 getTextureSizeFromSurfaceSize(u32 size) const;

	core::dimension2d<u32> ImageSize;
	COpenGLESDriver* Driver;
	IImage* Image;

	GLuint TextureName;
	GLint InternalFormat;
	GLenum PixelFormat;
	GLenum PixelType;
	bool HasMipMaps;
	bool IsRenderTarget;
	bool AutomaticMipmapUpdate;

	GLuint ColorFrameBuffer; // for FBO path
	GLuint DepthRenderBuffer; // for FBO path
	GLuint StencilRenderBuffer; // for FBO path

	u32 Locks;
};


} // end namespace video
} // end namespace irr

#endif
#endif // __C_OPEN_GLES_TEXTURE_H_INCLUDED__

