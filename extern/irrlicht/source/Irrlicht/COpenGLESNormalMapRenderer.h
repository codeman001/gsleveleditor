// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_OPENGLES_NORMAL_MAP_RENDERER_H_INCLUDED__
#define __C_OPENGLES_NORMAL_MAP_RENDERER_H_INCLUDED__

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_OPENGL_ES_
#ifdef _IRR_HAS_SHADER

#include "COpenGLESShaderMaterialRenderer.h"
#include "IShaderConstantSetCallBack.h"

namespace irr
{
namespace video
{

//! Class for rendering normal maps with OpenGL
class COpenGLESNormalMapRenderer : public COpenGLESShaderMaterialRenderer, public IShaderConstantSetCallBack
{
public:

	//! Constructor
	COpenGLESNormalMapRenderer(video::COpenGLDriver* driver,
		s32& outMaterialTypeNr, IMaterialRenderer* baseMaterial);

	//! Destructor
	~COpenGLESNormalMapRenderer();

	//! Called by the engine when the vertex and/or pixel shader constants for an
	//! material renderer should be set.
	virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData);

	//! Returns the render capability of the material.
	virtual s32 getRenderCapability() const;

protected:

	bool CompiledShaders;
};


} // end namespace video
} // end namespace irr

#endif // #ifndef _IRR_HAS_SHADER

#endif
#endif

