// Copyright (C) 2002-2010 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "COpenGLESDriver.h"
#include "COpenGLESTexture.h"

#include "CNullDriver.h"

namespace irr
{
namespace video
{


//! sets the current Texture
//! Returns whether setting was a success or not.
bool COpenGLESDriver::setActiveTexture(u32 stage, const video::ITexture* texture)
{
	if (stage >= MaxTextureUnits)
		return false;

	if (CurrentTexture[stage]==texture)
		return true;
	
	if (MultiTextureExtension)
		activeTexture(GL_TEXTURE0 + stage);

	CurrentTexture[stage]=texture;

	if (!texture)
	{
		glDisable(GL_TEXTURE_2D);
		return true;
	}
	else
	{
		if (texture->getDriverType() != EDT_OPENGL)
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

//! prints error if an error happened.
bool COpenGLESDriver::testGLError()
{
#ifdef _DEBUG
	GLenum g = glGetError();
	switch (g)
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
#if defined(GL_EXT_framebuffer_object)
	case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
		os::Printer::log("GL_INVALID_FRAMEBUFFER_OPERATION", ELL_ERROR); break;
#endif
	};
	return true;
#else
	return false;
#endif
}

//! get depth texture for the given render target texture
ITexture* COpenGLESDriver::createDepthTexture(ITexture* texture, bool shared)
{
	if ((texture->getDriverType() != EDT_OPENGL) || (!texture->isRenderTarget()))
		return 0;
	COpenGLESTexture* tex = static_cast<COpenGLESTexture*>(texture);

	if (!tex->isFrameBufferObject())
		return 0;

	if (shared)
	{
		for (u32 i=0; i<DepthTextures.size(); ++i)
		{
			if (DepthTextures[i]->getSize()==texture->getSize())
			{
				DepthTextures[i]->grab();
				return DepthTextures[i];
			}
		}
		DepthTextures.push_back(new COpenGLFBODepthTexture(texture->getSize(), "depth1", this));
		return DepthTextures.getLast();
	}
	return (new COpenGLFBODepthTexture(texture->getSize(), "depth1", this));
}


void COpenGLESDriver::removeDepthTexture(ITexture* texture)
{
	for (u32 i=0; i<DepthTextures.size(); ++i)
	{
		if (texture==DepthTextures[i])
		{
			DepthTextures.erase(i);
			return;
		}
	}
}


} // namespace video
} // namespace irr