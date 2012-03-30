// Copyright (C) 2002-2010 Nikolaus Gebhardt
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

//! constructor for usual textures
COpenGLESTexture::COpenGLESTexture(IImage* origImage, const io::path& name, void* mipmapData, COpenGLESDriver* driver)
	: ITexture(name), ColorFormat(ECF_A8R8G8B8), Driver(driver), Image(0), MipImage(0),
	TextureName(0), InternalFormat(GL_RGBA), PixelFormat(GL_BGRA),
	PixelType(GL_UNSIGNED_BYTE),
	IsRenderTarget(false), AutomaticMipmapUpdate(false),
	ReadOnlyLock(false), KeepImage(true)
{
	#ifdef _DEBUG
	setDebugName("COpenGLESTexture");
	#endif

	HasMipMaps = Driver->getTextureCreationFlag(ETCF_CREATE_MIP_MAPS);
	getImageValues(origImage);

	glGenTextures(1, &TextureName);

	if (ImageSize==TextureSize)
	{
		Image = new CImage(ColorFormat, ImageSize);
		origImage->copyTo(Image);
	}
	else
	{
		Image = new CImage(ColorFormat, TextureSize);
		// scale texture
		origImage->copyToScaling(Image);
	}
	uploadTexture(true, mipmapData);
	if (!KeepImage)
	{
		Image->drop();
		Image=0;
	}
}


//! constructor for basic setup (only for derived classes)
COpenGLESTexture::COpenGLESTexture(const io::path& name, COpenGLESDriver* driver)
	: ITexture(name), ColorFormat(ECF_A8R8G8B8), Driver(driver), Image(0), MipImage(0),
	TextureName(0), InternalFormat(GL_RGBA), PixelFormat(GL_BGRA),
	PixelType(GL_UNSIGNED_BYTE),
	HasMipMaps(true), IsRenderTarget(false), AutomaticMipmapUpdate(false),
	ReadOnlyLock(false), KeepImage(true)
{
	#ifdef _DEBUG
	setDebugName("COpenGLESTexture");
	#endif
}


//! destructor
COpenGLESTexture::~COpenGLESTexture()
{
	if (TextureName)
		glDeleteTextures(1, &TextureName);
	if (Image)
		Image->drop();
}


//! Choose best matching color format, based on texture creation flags
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
		default:
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


//! Get opengl values for the GPU texture storage
GLint COpenGLESTexture::getOpenGLFormatAndParametersFromColorFormat(ECOLOR_FORMAT format,
				GLint& filtering,
				GLenum& colorformat,
				GLenum& type)
{
	// default
	filtering = GL_LINEAR;
	colorformat = GL_RGBA;
	type = GL_UNSIGNED_BYTE;

	switch(format)
	{		
	case ECF_A1R5G5B5:
		// TODO: needs validation (doesn't seems like this works)
		InternalFormat=GL_RGBA;
		PixelFormat=GL_BGRA;
		PixelType=GL_UNSIGNED_SHORT_5_5_5_1;		
		break;
	case ECF_R5G6B5:
		InternalFormat=GL_RGB;
		PixelFormat=GL_RGB;
		PixelType=GL_UNSIGNED_SHORT_5_6_5;
		break;
	case ECF_R8G8B8:
		InternalFormat=GL_RGB;
		PixelFormat=GL_RGB; //GL_BGR;
		PixelType=GL_UNSIGNED_BYTE;
		break;
	case ECF_A8R8G8B8:
		#ifdef _IRR_USE_WINDOWS_DEVICE_
			InternalFormat= GL_BGRA;
			PixelFormat   = GL_BGRA;
		#else
			InternalFormat= GL_RGBA;
			PixelFormat   = GL_BGRA;
		#endif
		PixelType=GL_UNSIGNED_BYTE;
		break;	
	//case ECF_A8:
	//	InternalFormat=GL_ALPHA;
	//	PixelFormat=GL_ALPHA;
	//	break;
	default:
		os::Printer::log("Unsupported texture format", ELL_ERROR);
		break;
	}	
	return InternalFormat;
}


// prepare values ImageSize, TextureSize, and ColorFormat based on image
void COpenGLESTexture::getImageValues(IImage* image)
{
	if (!image)
	{
		os::Printer::log("No image for OpenGL texture.", ELL_ERROR);
		return;
	}

	ImageSize = image->getDimension();

	if ( !ImageSize.Width || !ImageSize.Height)
	{
		os::Printer::log("Invalid size of image for OpenGL Texture.", ELL_ERROR);
		return;
	}

	const f32 ratio = (f32)ImageSize.Width/(f32)ImageSize.Height;
	if ((ImageSize.Width>Driver->MaxTextureSize) && (ratio >= 1.0f))
	{
		ImageSize.Width = Driver->MaxTextureSize;
		ImageSize.Height = (u32)(Driver->MaxTextureSize/ratio);
	}
	else if (ImageSize.Height>Driver->MaxTextureSize)
	{
		ImageSize.Height = Driver->MaxTextureSize;
		ImageSize.Width = (u32)(Driver->MaxTextureSize*ratio);
	}
	TextureSize=ImageSize.getOptimalSize(!Driver->queryFeature(EVDF_TEXTURE_NPOT));

	ColorFormat = getBestColorFormat(image->getColorFormat());
}


//! copies the the texture into an open gl texture.
void COpenGLESTexture::uploadTexture(bool newTexture, void* mipmapData, u32 level)
{
	// check which image needs to be uploaded
	IImage* image = level?MipImage:Image;
	if (!image)
	{
		os::Printer::log("No image for OpenGL texture to upload", ELL_ERROR);
		return;
	}

	// get correct opengl color data values
	GLenum oldInternalFormat = InternalFormat;
	GLint filtering;
	InternalFormat = getOpenGLFormatAndParametersFromColorFormat(ColorFormat, filtering, PixelFormat, PixelType);
	// make sure we don't change the internal format of existing images
	if (!newTexture)
		InternalFormat=oldInternalFormat;

	Driver->setActiveTexture(0, this);
	if (Driver->testGLError())
		os::Printer::log("Could not bind Texture", ELL_ERROR);

	// mipmap handling for main texture
	if (!level && newTexture)
	{
#ifndef DISABLE_MIPMAPPING
#ifdef GL_SGIS_generate_mipmap
		// auto generate if possible and no mipmap data is given
		if (HasMipMaps && !mipmapData && Driver->queryFeature(EVDF_MIP_MAP_AUTO_UPDATE))
		{
			if (Driver->getTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED))
				glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_FASTEST);
			else if (Driver->getTextureCreationFlag(ETCF_OPTIMIZED_FOR_QUALITY))
				glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_NICEST);
			else
				glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_DONT_CARE);
			// automatically generate and update mipmaps
			glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
			AutomaticMipmapUpdate=true;
		}
		else
#endif
		{
			// Either generate manually due to missing capability
			// or use predefined mipmap data
			AutomaticMipmapUpdate=false;
			regenerateMipMapLevels(mipmapData);
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
			os::Printer::log("Did not create OpenGL texture mip maps.", ELL_INFORMATION);
#endif
		{
			// enable bilinear filter without mipmaps
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

	// now get image data and upload to GPU
	void* source = image->lock();
	if (newTexture)
		glTexImage2D(GL_TEXTURE_2D, level, InternalFormat, image->getDimension().Width,
			image->getDimension().Height, 0, PixelFormat, PixelType, source);
	else
		glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, image->getDimension().Width,
			image->getDimension().Height, PixelFormat, PixelType, source);
	image->unlock();

	if (Driver->testGLError())
		os::Printer::log("Could not glTexImage2D", ELL_ERROR);
}


//! lock function
void* COpenGLESTexture::lock(bool readOnly, u32 mipmapLevel)
{
	// store info about which image is locked
	IImage* image = (mipmapLevel==0)?Image:MipImage;
	ReadOnlyLock |= readOnly;
	MipLevelStored = mipmapLevel;

	// if data not available or might have changed on GPU download it
	if (!image || IsRenderTarget)
	{
		// prepare the data storage if necessary
		if (!image)
		{
			if (mipmapLevel)
			{
				u32 i=0;
				u32 width = TextureSize.Width;
				u32 height = TextureSize.Height;
				do
				{
					if (width>1)
						width>>=1;
					if (height>1)
						height>>=1;
					++i;
				}
				while (i != mipmapLevel);
				MipImage = image = new CImage(ECF_A8R8G8B8, core::dimension2du(width,height));
			}
			else
				Image = image = new CImage(ECF_A8R8G8B8, ImageSize);
			ColorFormat = ECF_A8R8G8B8;
		}
		if (!image)
			return 0;

		u8* pixels = static_cast<u8*>(image->lock());
		if (!pixels)
			return 0;

		// we need to keep the correct texture bound later on
		GLint tmpTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &tmpTexture);
		glBindTexture(GL_TEXTURE_2D, TextureName);

		// allows to read pixels in top-to-bottom order
#ifdef GL_MESA_pack_invert
		if (Driver->queryOpenGLFeature(COpenGLExtensionHandler::IRR_MESA_pack_invert))
			glPixelStorei(GL_PACK_INVERT_MESA, GL_TRUE);
#endif

		// download GPU data as ARGB8 to pixels;
		// do not implement
		// glGetTexImage(GL_TEXTURE_2D, mipmapLevel, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);

#ifdef GL_MESA_pack_invert
		if (Driver->queryOpenGLFeature(COpenGLExtensionHandler::IRR_MESA_pack_invert))
			glPixelStorei(GL_PACK_INVERT_MESA, GL_FALSE);
		else
#endif
		{
			// opengl images are horizontally flipped, so we have to fix that here.
			const s32 pitch=image->getPitch();
			u8* p2 = pixels + (image->getDimension().Height - 1) * pitch;
			u8* tmpBuffer = new u8[pitch];
			for (u32 i=0; i < image->getDimension().Height; i += 2)
			{
				memcpy(tmpBuffer, pixels, pitch);
				memcpy(pixels, p2, pitch);
				memcpy(p2, tmpBuffer, pitch);
				pixels += pitch;
				p2 -= pitch;
			}
			delete [] tmpBuffer;
		}
		image->unlock();

		//reset old bound texture
		glBindTexture(GL_TEXTURE_2D, tmpTexture);
	}
	return image->lock();
}


//! unlock function
void COpenGLESTexture::unlock()
{
	// test if miplevel or main texture was locked
	IImage* image = MipImage?MipImage:Image;
	if (!image)
		return;
	// unlock image to see changes
	image->unlock();
	// copy texture data to GPU
	if (!ReadOnlyLock)
		uploadTexture(false, 0, MipLevelStored);
	ReadOnlyLock = false;
	// cleanup local image
	if (MipImage)
	{
		MipImage->drop();
		MipImage=0;
	}
	else if (!KeepImage)
	{
		Image->drop();
		Image=0;
	}
	// update information
	if (Image)
		ColorFormat=Image->getColorFormat();
	else
		ColorFormat=ECF_A8R8G8B8;
}


//! Returns size of the original image.
const core::dimension2d<u32>& COpenGLESTexture::getOriginalSize() const
{
	return ImageSize;
}


//! Returns size of the texture.
const core::dimension2d<u32>& COpenGLESTexture::getSize() const
{
	return TextureSize;
}


//! returns driver type of texture, i.e. the driver, which created the texture
E_DRIVER_TYPE COpenGLESTexture::getDriverType() const
{
	return EDT_OPENGL;
}


//! returns color format of texture
ECOLOR_FORMAT COpenGLESTexture::getColorFormat() const
{
	return ColorFormat;
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

unsigned int COpenGLESTexture::getTextureID()
{
	return TextureName;
}


//! Returns whether this texture has mipmaps
bool COpenGLESTexture::hasMipMaps() const
{
	return HasMipMaps;
}


//! Regenerates the mip map levels of the texture. Useful after locking and
//! modifying the texture
void COpenGLESTexture::regenerateMipMapLevels(void* mipmapData)
{
	if (AutomaticMipmapUpdate || !HasMipMaps || !Image)
		return;
	if ((Image->getDimension().Width==1) && (Image->getDimension().Height==1))
		return;

	// Manually create mipmaps or use prepared version
	u32 width=Image->getDimension().Width;
	u32 height=Image->getDimension().Height;
	u32 i=0;
	u8* target = static_cast<u8*>(mipmapData);
	do
	{
		if (width>1)
			width>>=1;
		if (height>1)
			height>>=1;
		++i;
		if (!target)
			target = new u8[width*height*Image->getBytesPerPixel()];
		// create scaled version if no mipdata available
		if (!mipmapData)
			Image->copyToScaling(target, width, height, Image->getColorFormat());
		glTexImage2D(GL_TEXTURE_2D, i, InternalFormat, width, height,
				0, PixelFormat, PixelType, target);
		// get next prepared mipmap data if available
		if (mipmapData)
		{
			mipmapData = static_cast<u8*>(mipmapData)+width*height*Image->getBytesPerPixel();
			target = static_cast<u8*>(mipmapData);
		}
	}
	while (width!=1 || height!=1);
	// cleanup
	if (!mipmapData)
		delete [] target;
}


bool COpenGLESTexture::isRenderTarget() const
{
	 return IsRenderTarget;
}


void COpenGLESTexture::setIsRenderTarget(bool isTarget)
{
	 IsRenderTarget = isTarget;
}


bool COpenGLESTexture::isFrameBufferObject() const
{
	return false;
}


//! Bind Render Target Texture
void COpenGLESTexture::bindRTT()
{
}


//! Unbind Render Target Texture
void COpenGLESTexture::unbindRTT()
{
	Driver->setActiveTexture(0, this);

	// Copy Our ViewPort To The Texture
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, getSize().Width, getSize().Height);
}


/* FBO Textures */

// helper function for render to texture
static bool checkFBOStatus(COpenGLESDriver* Driver);

//! RTT ColorFrameBuffer constructor
COpenGLESFBOTexture::COpenGLESFBOTexture(const core::dimension2d<u32>& size,
					const io::path& name, COpenGLESDriver* driver,
					const ECOLOR_FORMAT format)
	: COpenGLESTexture(name, driver), DepthTexture(0), ColorFrameBuffer(0)
{
	#ifdef _DEBUG
	setDebugName("COpenGLESTexture_FBO");
	#endif

	ImageSize = size;
	TextureSize = size;

	GLint FilteringType;
	InternalFormat = getOpenGLFormatAndParametersFromColorFormat(format, FilteringType, PixelFormat, PixelType);

	HasMipMaps = false;
	IsRenderTarget = true;
	

	// generate frame buffer
	Driver->genFramebuffers(1, &ColorFrameBuffer);
	Driver->bindFramebuffer(GL_FRAMEBUFFER, ColorFrameBuffer);

	// generate color texture
	glGenTextures(1, &TextureName);
	Driver->setActiveTexture(0, this);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilteringType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, ImageSize.Width,
		ImageSize.Height, 0, PixelFormat, PixelType, 0);

	// attach color texture to frame buffer
	Driver->framebufferTexture2D(GL_FRAMEBUFFER,
						GL_COLOR_ATTACHMENT0,
						GL_TEXTURE_2D,
						TextureName,
						0);
	unbindRTT();
}


//! destructor
COpenGLESFBOTexture::~COpenGLESFBOTexture()
{
	if (DepthTexture)
		if (DepthTexture->drop())
			Driver->removeDepthTexture(DepthTexture);
	if (ColorFrameBuffer)
		Driver->deleteFramebuffers(1, &ColorFrameBuffer);
}


bool COpenGLESFBOTexture::isFrameBufferObject() const
{
	return true;
}


//! Bind Render Target Texture
void COpenGLESFBOTexture::bindRTT()
{
	if (ColorFrameBuffer != 0)
		Driver->bindFramebuffer(GL_FRAMEBUFFER, ColorFrameBuffer);
}


//! Unbind Render Target Texture
void COpenGLESFBOTexture::unbindRTT()
{
	if (ColorFrameBuffer != 0)
		Driver->bindFramebuffer(GL_FRAMEBUFFER, 0);
}


/* FBO Depth Textures */

//! RTT DepthBuffer constructor
COpenGLESFBODepthTexture::COpenGLESFBODepthTexture(
		const core::dimension2d<u32>& size,
		const io::path& name,
		COpenGLESDriver* driver,
		bool useStencil)
	: COpenGLESFBOTexture(size, name, driver), DepthRenderBuffer(0),
	StencilRenderBuffer(0), UseStencil(useStencil)
{
#ifdef _DEBUG
	setDebugName("COpenGLESTextureFBO_Depth");
#endif

	ImageSize = size;
	TextureSize = size;
	InternalFormat = GL_RGBA;
	PixelFormat = GL_RGBA;
	PixelType = GL_UNSIGNED_BYTE;
	HasMipMaps = false;

	if (useStencil)
	{
		glGenTextures(1, &DepthRenderBuffer);
		glBindTexture(GL_TEXTURE_2D, DepthRenderBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef GL_EXT_packed_depth_stencil
		if (Driver->queryOpenGLFeature(COpenGLExtensionHandler::IRR_EXT_packed_depth_stencil))
		{
			// generate packed depth stencil texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL_EXT, ImageSize.Width,
				ImageSize.Height, 0, GL_DEPTH_STENCIL_EXT, GL_UNSIGNED_INT_24_8_EXT, 0);
			StencilRenderBuffer = DepthRenderBuffer; // stencil is packed with depth
		}
		else // generate separate stencil and depth textures
#endif
		{
			// generate depth texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, ImageSize.Width,
				ImageSize.Height, 0, GL_DEPTH_COMPONENT16_OES, GL_UNSIGNED_BYTE, 0);
		}
	}
#ifdef _IRR_COMPILE_WITH_OPENGL_ES_
	else
	{
		// generate depth buffer
		Driver->genRenderbuffers(1, &DepthRenderBuffer);
		Driver->bindRenderbuffer(GL_RENDERBUFFER, DepthRenderBuffer);
		Driver->renderbufferStorage(GL_RENDERBUFFER,
				GL_DEPTH_COMPONENT16_OES, ImageSize.Width,
				ImageSize.Height);		
	}
#endif
}


//! destructor
COpenGLESFBODepthTexture::~COpenGLESFBODepthTexture()
{	
	if (DepthRenderBuffer && UseStencil)
		glDeleteTextures(1, &DepthRenderBuffer);
	else
		Driver->deleteRenderbuffers(1, &DepthRenderBuffer);
	if (StencilRenderBuffer && StencilRenderBuffer != DepthRenderBuffer)
		glDeleteTextures(1, &StencilRenderBuffer);
}


//combine depth texture and rtt
bool COpenGLESFBODepthTexture::attach(ITexture* renderTex)
{
	if (!renderTex)
		return false;
	video::COpenGLESFBOTexture* rtt = static_cast<video::COpenGLESFBOTexture*>(renderTex);
	rtt->bindRTT();

	if (UseStencil)
	{		
		// attach stencil texture to stencil buffer
		Driver->framebufferTexture2D(GL_FRAMEBUFFER,
						GL_STENCIL_ATTACHMENT,
						GL_TEXTURE_2D,
						StencilRenderBuffer,
						0);

		// attach depth texture to depth buffer
		Driver->framebufferTexture2D(GL_FRAMEBUFFER,
						GL_DEPTH_ATTACHMENT,
						GL_TEXTURE_2D,
						DepthRenderBuffer,
						0);
	}
	else
	{
		// attach depth renderbuffer to depth buffer
		Driver->framebufferRenderbuffer(GL_FRAMEBUFFER,
						GL_DEPTH_ATTACHMENT,
						GL_RENDERBUFFER,
						DepthRenderBuffer);
	}


	// check the status
	if (!checkFBOStatus(Driver))
	{
		os::Printer::log("FBO incomplete");
		return false;
	}
	rtt->DepthTexture=this;
	grab(); // grab the depth buffer, not the RTT
	rtt->unbindRTT();
	return true;
}


//! Bind Render Target Texture
void COpenGLESFBODepthTexture::bindRTT()
{
}


//! Unbind Render Target Texture
void COpenGLESFBODepthTexture::unbindRTT()
{
}


bool checkFBOStatus(COpenGLESDriver* Driver)
{

	GLenum status = Driver->checkFramebufferStatus(GL_FRAMEBUFFER);

	switch (status)
	{
		//Our FBO is perfect, return true
		case GL_FRAMEBUFFER_COMPLETE:
			return true;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			os::Printer::log("FBO has one or several incomplete image attachments", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
			os::Printer::log("FBO has one or several image attachments with different internal formats", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
			os::Printer::log("FBO has one or several image attachments with different dimensions", ELL_ERROR);
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			os::Printer::log("FBO missing an image attachment", ELL_ERROR);
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			os::Printer::log("FBO format unsupported", ELL_ERROR);
			break;

		default:
			break;
	}

	os::Printer::log("FBO error", ELL_ERROR);
	return false;
}


} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OPENGL_

