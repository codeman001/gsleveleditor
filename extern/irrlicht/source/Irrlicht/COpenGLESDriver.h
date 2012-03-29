// Copyright (C) 2002-2010 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_VIDEO_OPEN_GL_ES_H_INCLUDED__
#define __C_VIDEO_OPEN_GL_ES_H_INCLUDED__


#include "IrrCompileConfig.h"

#if defined(_IRR_COMPILE_WITH_OPENGL_ES_)
#include "COpenGLESHeaders.h"

#include "CNullDriver.h"
#include "COpenGLESExtensionHandler.h"

namespace irr
{

namespace video
{

class COpenGLESTexture;

class COpenGLESDriver: public CNullDriver, public COpenGLESExtensionHandler
{
protected:
	SMaterial Material, LastMaterial;
	COpenGLESTexture* RenderTargetTexture;
	const ITexture* CurrentTexture[MATERIAL_MAX_TEXTURES];
	core::array<ITexture*> DepthTextures;

public:

	//! queries the features of the driver, returns true if feature is available
	virtual bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const
	{
		return FeatureEnabled[feature] && COpenGLESExtensionHandler::queryFeature(feature);
	}

	//! sets the current Texture
	//! Returns whether setting was a success or not.
	bool setActiveTexture(u32 stage, const video::ITexture* texture);

	//! checks if an OpenGL error has happend and prints it
	//! for performance reasons only available in debug mode
	bool testGLError();

	ITexture* createDepthTexture(ITexture* texture, bool shared=true);
	void removeDepthTexture(ITexture* texture);

};

}

}

#endif

#endif

