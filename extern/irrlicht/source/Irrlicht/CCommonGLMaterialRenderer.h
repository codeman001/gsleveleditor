//-*-c++-*-
// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_COMMON_GL_MATERIAL_RENDERER_H_INCLUDED__
#define __C_COMMON_GL_MATERIAL_RENDERER_H_INCLUDED__

#include "IrrCompileConfig.h"

#if defined(_IRR_COMPILE_WITH_OPENGL_ES_) || defined(_IRR_COMPILE_WITH_PS3_)
#include "CCommonGLHeaders.h"
#include "IMaterialRenderer.h"
#include "os.h"

namespace irr
{
namespace video
{

class CCommonGLDriver;

//! Base class for all internal OpenGL material renderers
class CCommonGLMaterialRenderer : public IMaterialRenderer
{
public:

	//! Constructor
	CCommonGLMaterialRenderer(video::CCommonGLDriver* driver) : Driver(driver)
	{
	}

protected:

	CCommonGLDriver* Driver;
};


//! Solid material renderer
class CCommonGLMaterialRenderer_SOLID : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_SOLID(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual u32 getVertexAttributeMask() const;

};


//! Generic Texture Blend
class CCommonGLMaterialRenderer_ONETEXTURE_BLEND : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_ONETEXTURE_BLEND(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);
	virtual void onUnsetMaterial();

	virtual u32 getVertexAttributeMask() const;

};


//! Solid 2 layer material renderer
class CCommonGLMaterialRenderer_SOLID_2_LAYER : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_SOLID_2_LAYER(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);
	virtual void onUnsetMaterial();

	virtual u32 getVertexAttributeMask() const;
};

//! Solid 2 layer material (with decal) renderer
class CCommonGLMaterialRenderer_SOLID_2_LAYER_DECAL : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_SOLID_2_LAYER_DECAL(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);
	virtual void onUnsetMaterial();

	virtual u32 getVertexAttributeMask() const;
};

//! Transparent add color material renderer
class CCommonGLMaterialRenderer_TRANSPARENT_ADD_COLOR : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_TRANSPARENT_ADD_COLOR(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual u32 getVertexAttributeMask() const;
};

//! Transparent vertex alpha material renderer
class CCommonGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual u32 getVertexAttributeMask() const;
};

//! Transparent texture X vertex alpha material renderer
class CCommonGLMaterialRenderer_TRANSPARENT_TEXTURE_VERTEX_ALPHA : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_TRANSPARENT_TEXTURE_VERTEX_ALPHA(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual u32 getVertexAttributeMask() const;
};

//! Transparent alpha channel material renderer
class CCommonGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual u32 getVertexAttributeMask() const;
};


class CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual u32 getVertexAttributeMask() const;

	virtual bool onRender (IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype);
};

class CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP_NOADD : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP_NOADD(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual u32 getVertexAttributeMask() const;

	virtual bool onRender (IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype);
};

class CCommonGLMaterialRenderer_SPHEREMAP_ADD : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_SPHEREMAP_ADD(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual u32 getVertexAttributeMask() const;
};

//cgm material for particles
//! Non Transparent add color material renderer - replace
class CCommonGLMaterialRenderer_ADDITIVE_REPLACE : public irr::video::CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_ADDITIVE_REPLACE(irr::video::CCommonGLDriver* d)
		: irr::video::CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const irr::video::SMaterial& material,
							   const irr::video::SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   irr::video::IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual irr::u32 getVertexAttributeMask() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//cgm material for particles
class CCommonGLMaterialRenderer_ADDITIVE_MODULATE : public irr::video::CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_ADDITIVE_MODULATE(irr::video::CCommonGLDriver* d)
		: irr::video::CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const irr::video::SMaterial& material,
							   const irr::video::SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   irr::video::IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual irr::u32 getVertexAttributeMask() const;
};

//cgm opaque material without vertex color
class CCommonGLMaterialRenderer_OPAQUE_NO_VERTEXCOLOR : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_OPAQUE_NO_VERTEXCOLOR(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual u32 getVertexAttributeMask() const;

	virtual bool onRender (IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype);
};

//cgm transparent material without vertex color
class CCommonGLMaterialRenderer_TRANSPARENT_NO_VERTEXCOLOR : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_TRANSPARENT_NO_VERTEXCOLOR(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual u32 getVertexAttributeMask() const;

	virtual bool onRender (IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype);
};


//! Transparent alpha channel material renderer
class CCommonGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual u32 getVertexAttributeMask() const;
};

//! material renderer for all kinds of lightmaps
class CCommonGLMaterialRenderer_LIGHTMAP : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_LIGHTMAP(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	virtual u32 getVertexAttributeMask() const;
};

//! detail map  material renderer
class CCommonGLMaterialRenderer_DETAIL_MAP : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_DETAIL_MAP(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();
	
	virtual u32 getVertexAttributeMask() const;
};

//! sphere map material renderer
class CCommonGLMaterialRenderer_SPHERE_MAP : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_SPHERE_MAP(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	virtual u32 getVertexAttributeMask() const;
};

//! reflection 2 layer material renderer
class CCommonGLMaterialRenderer_REFLECTION_2_LAYER : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_REFLECTION_2_LAYER(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	virtual u32 getVertexAttributeMask() const;
};

//! reflection 2 layer material renderer
class CCommonGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER : public CCommonGLMaterialRenderer
{
public:

	CCommonGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER(video::CCommonGLDriver* d)
		: CCommonGLMaterialRenderer(d) {}

	virtual void onSetMaterial(const SMaterial& material,
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates,
							   IMaterialRendererServices* services);

	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;

	virtual u32 getVertexAttributeMask() const;
};

//! 2D  alpha material renderer
class CCommonGLMaterialRenderer_2D_ALPHA : public CCommonGLMaterialRenderer
{
public:
	CCommonGLMaterialRenderer_2D_ALPHA( video::CCommonGLDriver* driver )
		: CCommonGLMaterialRenderer ( driver ) {}
	
	virtual void onSetMaterial(const SMaterial& material, 
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates, 
							   IMaterialRendererServices* services);	
			
	virtual void onUnsetMaterial();

	//! Returns if the material is transparent.
	virtual bool isTransparent() const;
	virtual u32 getVertexAttributeMask() const;
};

//! unsupported material renderer
class CCommonGLMaterialRenderer_UNSUPPORTED : public CCommonGLMaterialRenderer
{
public:
	CCommonGLMaterialRenderer_UNSUPPORTED ( video::CCommonGLDriver* driver )
		: CCommonGLMaterialRenderer ( driver ) {}
	
	virtual void onSetMaterial(const SMaterial& material, 
							   const SMaterial& lastMaterial,
							   bool resetAllRenderstates, 
							   IMaterialRendererServices* services)
	{
		os::Printer::log("Unsupported material renderer", ELL_WARNING);
	}
		
	virtual s32 getRenderCapability() const { return 1; }
			
	virtual u32 getVertexAttributeMask() const
	{
		return 0;
	}
};

} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OPENGL_ || _IRR_COMPILE_WITH_OPENGL_ES_
#endif // __C_COMMON_GL_MATERIAL_RENDERER_H_INCLUDED__

