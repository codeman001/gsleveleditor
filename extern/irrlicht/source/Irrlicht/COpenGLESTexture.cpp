// Copyright (C) 2002-2007 Nikolaus Gebhardt, modify OpenGL|ES by pham hong duc (hongduc.pr@gmail.com)
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"


#ifdef _IRR_COMPILE_WITH_OPENGL_ES_

#include "irrTypes.h"
#include "COpenGLESTexture.h"
#include "COpenGLESDriver.h"
#include "os.h"
#include "CImage.h"
#include "CColorConverter.h"

#include "irrString.h"

namespace irr
{
namespace video
{

// helper function for render to texture
static bool checkFBOStatus(COpenGLESDriver* Driver);

//! constructor for usual textures
COpenGLESTexture::COpenGLESTexture(IImage* image, const char* name, COpenGLESDriver* driver)
 : ITexture(name), Driver(driver), Image(0),
  TextureName(0), InternalFormat(GL_RGB),
  PixelFormat(GL_RGB),
  PixelType(GL_UNSIGNED_BYTE), HasMipMaps(true),
  IsRenderTarget(false),
  ColorFrameBuffer(0), DepthRenderBuffer(0), StencilRenderBuffer(0), Locks(0)
{
	#ifdef _DEBUG
	setDebugName("COpenGLTexture");
	#endif

	getImageData(image);

	HasMipMaps = Driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	if (Image)
	{
		glGenTextures(1, &TextureName);
		copyTexture();
	}
}

//! ColorFrameBuffer constructor (disabled for OpenGL|ES)
COpenGLESTexture::COpenGLESTexture(const core::dimension2d<u32>& size,
                                bool extPackedDepthStencilSupported,
                                const char* name,
                                COpenGLESDriver* driver)
 : ITexture(name), ImageSize(size), Driver(driver), Image(0),
  TextureName(0),
  InternalFormat(GL_RGB),
  PixelFormat(GL_RGB),
  //PixelFormat(GL_RGBA),
  PixelType(GL_UNSIGNED_BYTE), HasMipMaps(false),
  IsRenderTarget(false),
  ColorFrameBuffer(0), DepthRenderBuffer(0), StencilRenderBuffer(0), Locks(0)
{

}


//! destructor
COpenGLESTexture::~COpenGLESTexture()
{
	if (ColorFrameBuffer)
		Driver->extGlDeleteFramebuffers(1, &ColorFrameBuffer);

	if (DepthRenderBuffer)
		glDeleteTextures(1, &DepthRenderBuffer);
	if (StencilRenderBuffer && StencilRenderBuffer != DepthRenderBuffer)
		glDeleteTextures(1, &StencilRenderBuffer);

	ColorFrameBuffer = 0;
	DepthRenderBuffer = 0;
	StencilRenderBuffer = 0;

	glDeleteTextures(1, &TextureName);
	if (Image)
	{
		Image->drop();
		Image=0;
	}

	ImageSize.Width=ImageSize.Height=0;
}


ECOLOR_FORMAT COpenGLESTexture::getBestColorFormat(ECOLOR_FORMAT format)
{
	ECOLOR_FORMAT destFormat = ECF_A8R8G8B8;
	switch (format)
	{
		case ECF_A1R5G5B5:
			if (!Driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT))
				destFormat = ECF_A1R5G5B5;
		break;
		case ECF_R5G6B5:
			if (!Driver->getTextureCreationFlag(ETCF_ALWAYS_32_BIT))
				destFormat = ECF_A1R5G5B5;
		break;
		case ECF_A8R8G8B8:
			if (Driver->getTextureCreationFlag(ETCF_ALWAYS_16_BIT) ||
					Driver->getTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED))
				destFormat = ECF_A1R5G5B5;
		break;
		case ECF_R8G8B8:
			if (Driver->getTextureCreationFlag(ETCF_ALWAYS_16_BIT) ||
					Driver->getTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED))
				destFormat = ECF_A1R5G5B5;
		break;
	}
	if (Driver->getTextureCreationFlag(ETCF_NO_ALPHA_CHANNEL))
	{
		switch (destFormat)
		{
			case ECF_A1R5G5B5:
				destFormat = ECF_R5G6B5;
			break;
			case ECF_A8R8G8B8:
				destFormat = ECF_R8G8B8;
			break;
			default:
			break;
		}
	}
	return destFormat;
}


void COpenGLESTexture::getImageData(IImage* image)
{
	if (!image)
	{
		os::Printer::log("No image for OpenGL|ES texture.", ELL_ERROR);
		return;
	}

	ImageSize = image->getDimension();

	if ( !ImageSize.Width || !ImageSize.Height)
	{
		os::Printer::log("Invalid size of image for OpenGL|ES Texture.", ELL_ERROR);
		return;
	}

	core::dimension2d<u32> nImageSize;
	if (Driver->queryFeature(EVDF_TEXTURE_NPOT))
		nImageSize=ImageSize;
	else
	{
		nImageSize.Width = getTextureSizeFromSurfaceSize(ImageSize.Width);
		nImageSize.Height = getTextureSizeFromSurfaceSize(ImageSize.Height);
	}

	ECOLOR_FORMAT destFormat = image->getColorFormat(); // getBestColorFormat(image->getColorFormat());

	Image = new CImage(destFormat, nImageSize);

	if (ImageSize==nImageSize)
		image->copyTo(Image);
	else
	{		
		// scale texture
		image->copyToScaling(Image);
	}
}



//! copies the the texture into an open gl texture.
void COpenGLESTexture::copyTexture(bool newTexture)
{
	glBindTexture(GL_TEXTURE_2D, TextureName);
	if (Driver->testGLError())
		os::Printer::log("Could not bind Texture", ELL_ERROR);

	switch (Image->getColorFormat())
	{
		case ECF_A1R5G5B5:
			printf("ECF_A1R5G5B5");
			InternalFormat=GL_RGBA;
			PixelFormat=GL_RGBA;
			PixelType=GL_UNSIGNED_SHORT_5_5_5_1;
			break;
		case ECF_R5G6B5:
			printf("ECF_R5G6B5");
			InternalFormat=GL_RGB;
			PixelFormat=GL_RGB; //GL_BGR;
			PixelType=GL_UNSIGNED_SHORT_5_6_5;
			break;
		case ECF_R8G8B8:
			printf("ECF_R8G8B8");
			InternalFormat=GL_RGB;
			PixelFormat=GL_RGB; //GL_BGR;
			PixelType=GL_UNSIGNED_BYTE;
			break;
		case ECF_A8R8G8B8:
			printf("ECF_A8R8G8B8");
			InternalFormat=GL_RGBA;
			PixelFormat=GL_RGBA;
			PixelType=GL_UNSIGNED_BYTE;

			break;
		default:
			os::Printer::log("Unsupported texture format", ELL_ERROR);
			break;
	}

	if (newTexture)
	{
#ifndef DISABLE_MIPMAPPING
		if (HasMipMaps && Driver->queryFeature(EVDF_MIP_MAP_AUTO_UPDATE))
		{
			// automatically generate and update mipmaps
			glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
			AutomaticMipmapUpdate=true;
		}
		else
		{
			AutomaticMipmapUpdate=false;
			regenerateMipMapLevels();
		}
		if (HasMipMaps) // might have changed in regenerateMipMapLevels
		{
			// enable bilinear mipmap filter
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
#else
			HasMipMaps=false;
			os::Printer::log("Did not create OpenGL texture mip maps.", ELL_ERROR);
#endif
		{
			// enable bilinear filter without mipmaps
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

	void* source = Image->lock();
	if (newTexture)
		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, Image->getDimension().Width,
			Image->getDimension().Height, 0, PixelFormat, PixelType, source);
	else
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Image->getDimension().Width,
			Image->getDimension().Height, PixelFormat, PixelType, source);
	Image->unlock();

	if (Driver->testGLError())
		os::Printer::log("Could not glTexImage2D", ELL_ERROR);
}



//! returns the size of a texture which would be the optimal size for rendering it
inline u32 COpenGLESTexture::getTextureSizeFromSurfaceSize(u32 size) const
{
	u32 ts = 0x01;
	while(ts < size)
		ts <<= 1;

	return ts;
}


//! lock function
void* COpenGLESTexture::lock(bool readOnly, u32 mipmapLevel)
{
	if (Image)
	{
		++Locks;
		return Image->lock();
	}
	else
		return 0;
}



//! unlock function
void COpenGLESTexture::unlock()
{
	if (Image)
	{
		--Locks;
		Image->unlock();
		copyTexture(false);
	}
}



//! Returns size of the original image.
const core::dimension2d<u32>& COpenGLESTexture::getOriginalSize() const
{
	return ImageSize;
}



//! Returns of the texture.
const core::dimension2d<u32>& COpenGLESTexture::getSize() const
{
	if (Image)
		return Image->getDimension();
	else
		return ImageSize;
}



//! returns driver type of texture (=the driver, who created the texture)
E_DRIVER_TYPE COpenGLESTexture::getDriverType() const
{
	return EDT_OPENGLES;
}



//! returns color format of texture
ECOLOR_FORMAT COpenGLESTexture::getColorFormat() const
{
	if (Image)
		return Image->getColorFormat();
	else
		return ECF_A8R8G8B8;
}



//! returns pitch of texture (in bytes)
u32 COpenGLESTexture::getPitch() const
{
	if (Image)
		return Image->getPitch();
	else
		return 0;
}



//! return open gl texture name
GLuint COpenGLESTexture::getOpenGLTextureName() const
{
	return TextureName;
}


//! Returns whether this texture has mipmaps
//! return true if texture has mipmaps
bool COpenGLESTexture::hasMipMaps() const
{
	return HasMipMaps;
}


//! Regenerates the mip map levels of the texture. Useful after locking and
//! modifying the texture
void COpenGLESTexture::regenerateMipMapLevels(void* mipmapData)
{
	if (AutomaticMipmapUpdate || !HasMipMaps)
		return;

	if ((Image->getDimension().Width==1) && (Image->getDimension().Height==1))
		return;

	// Manually create mipmaps
	u32 width=Image->getDimension().Width;
	u32 height=Image->getDimension().Height;
	u32 i=0;
	u8* target = new u8[Image->getImageDataSizeInBytes()];
	do
	{
		if (width>1)
			width>>=1;
		if (height>1)
			height>>=1;
		++i;
		Image->copyToScaling(target, width, height, Image->getColorFormat());
		glTexImage2D(GL_TEXTURE_2D, i, InternalFormat, width, height,
				0, PixelFormat, PixelType, target);
	}
	while (width!=1 || height!=1);
	delete [] target;
	Image->unlock();
}


bool COpenGLESTexture::isFrameBufferObject() const
{
    return ColorFrameBuffer != 0;
}

bool COpenGLESTexture::isRenderTarget() const
{
    return IsRenderTarget;
}

void COpenGLESTexture::setRenderTarget(bool isTarget)
{
    IsRenderTarget = isTarget;
}

//! Bind ColorFrameBuffer (valid only if isFrameBufferObject() returns true).
void COpenGLESTexture::bindFrameBufferObject()
{
#ifdef GL_EXT_framebuffer_object
    if (ColorFrameBuffer != 0)
        Driver->extGlBindFramebuffer(GL_FRAMEBUFFER_EXT, ColorFrameBuffer);
#endif
}


//! Unbind ColorFrameBuffer (valid only if isFrameBufferObject() returns true).
void COpenGLESTexture::unbindFrameBufferObject()
{
#ifdef GL_EXT_framebuffer_object
    if (ColorFrameBuffer != 0)
        Driver->extGlBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
#endif
}

bool checkFBOStatus(COpenGLESDriver* Driver)
{
#ifdef GL_EXT_framebuffer_object
	GLenum status = Driver->extGlCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);

	switch (status)
	{
		//Our FBO is perfect, return true
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			return true;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			os::Printer::log("FBO has invalid read buffer", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			os::Printer::log("FBO has invalid draw buffer", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			os::Printer::log("FBO has one or several incomplete image attachments", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			os::Printer::log("FBO has one or several image attachments with different internal formats", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			os::Printer::log("FBO has one or several image attachments with different dimensions", ELL_ERROR);
			break;

// not part of fbo_object anymore, but won't harm as it is just a return value
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT
		case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
			os::Printer::log("FBO has a duplicate image attachment", ELL_ERROR);
			break;
#endif

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			os::Printer::log("FBO missing an image attachment", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			os::Printer::log("FBO format unsupported", ELL_ERROR);
			break;

		default:
			break;
	}
	os::Printer::log("FBO error", ELL_ERROR);
#endif
	return false;
}

} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OPENGL_ES_
