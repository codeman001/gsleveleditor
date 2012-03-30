// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_OPENGLES_MATERIAL_RENDERER_H_INCLUDED__
#define __C_OPENGLES_MATERIAL_RENDERER_H_INCLUDED__

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_OPENGL_ES_

#include "COpenGLESDriver.h"
#include "IMaterialRenderer.h"

namespace irr
{
namespace video
{

//! Base class for all internal OpenGL material renderers
class COpenGLESMaterialRenderer : public IMaterialRenderer
{
public:

	//! Constructor
	COpenGLESMaterialRenderer(video::COpenGLESDriver* driver) : Driver(driver)
	{
	}

protected:

	video::COpenGLESDriver* Driver;
};


//! Solid material renderer
class COpenGLESMaterialRenderer_SOLID : public COpenGLESMaterialRenderer
{
public:

	COpenGLESMaterialRenderer_SOLID(video::COpenGLESDriver* d)
		: COpenGLESMaterialRenderer(d) {}

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.getTexture(0));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			// thanks to Murphy, the following line removed some
			// bugs with several OpenGL implementations.
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
	}
};


//! Generic Texture Blend
class COpenGLESMaterialRenderer_ONETEXTURE_BLEND : public COpenGLESMaterialRenderer
{
public:

	COpenGLESMaterialRenderer_ONETEXTURE_BLEND(video::COpenGLESDriver* d)
		: COpenGLESMaterialRenderer(d) {}

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.getTexture(0));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

//		if (material.MaterialType != lastMaterial.MaterialType ||
//			material.MaterialTypeParam != lastMaterial.MaterialTypeParam ||
//			resetAllRenderstates)
		{
			E_BLEND_FACTOR srcFact,dstFact;
			E_MODULATE_FUNC modulate;
			u32 alphaSource;
			unpack_texureBlendFunc( srcFact, dstFact, modulate, alphaSource, material.MaterialTypeParam );

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);

			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, (f32) modulate );

			glBlendFunc( getGLBlend(srcFact), getGLBlend(dstFact) );
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);

			if ( getTexelAlpha ( srcFact ) + getTexelAlpha ( dstFact ) )
			{
				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_TEXTURE);

				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR);
			}
		}
	}

	virtual void OnUnsetMaterial()
	{
		glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 1.f );
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);

		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
	}

	private:

		u32 getGLBlend ( E_BLEND_FACTOR factor ) const
		{
			u32 r = 0;
			switch ( factor )
			{
				case EBF_ZERO:			r = GL_ZERO; break;
				case EBF_ONE:			r = GL_ONE; break;
				case EBF_DST_COLOR:		r = GL_DST_COLOR; break;
				case EBF_ONE_MINUS_DST_COLOR:	r = GL_ONE_MINUS_DST_COLOR; break;
				case EBF_SRC_COLOR:		r = GL_SRC_COLOR; break;
				case EBF_ONE_MINUS_SRC_COLOR:	r = GL_ONE_MINUS_SRC_COLOR; break;
				case EBF_SRC_ALPHA:		r = GL_SRC_ALPHA; break;
				case EBF_ONE_MINUS_SRC_ALPHA:	r = GL_ONE_MINUS_SRC_ALPHA; break;
				case EBF_DST_ALPHA:		r = GL_DST_ALPHA; break;
				case EBF_ONE_MINUS_DST_ALPHA:	r = GL_ONE_MINUS_DST_ALPHA; break;
				case EBF_SRC_ALPHA_SATURATE:	r = GL_SRC_ALPHA_SATURATE; break;
			}
			return r;
		}

		u32 getTexelAlpha ( E_BLEND_FACTOR factor ) const
		{
			u32 r;
			switch ( factor )
			{
				case EBF_SRC_ALPHA:		r = 1; break;
				case EBF_ONE_MINUS_SRC_ALPHA:	r = 1; break;
				case EBF_DST_ALPHA:		r = 1; break;
				case EBF_ONE_MINUS_DST_ALPHA:	r = 1; break;
				case EBF_SRC_ALPHA_SATURATE:	r = 1; break;
				default:			r = 0; break;
			}
			return r;
		}
};


//! Solid 2 layer material renderer
class COpenGLESMaterialRenderer_SOLID_2_LAYER : public COpenGLESMaterialRenderer
{
public:

	COpenGLESMaterialRenderer_SOLID_2_LAYER(video::COpenGLESDriver* d)
		: COpenGLESMaterialRenderer(d) {}

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(2);
		Driver->setTexture(1, material.getTexture(1));
		Driver->setTexture(0, material.getTexture(0));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				Driver->extGlActiveTexture(GL_TEXTURE1);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

				Driver->extGlActiveTexture(GL_TEXTURE0);
			}

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
	}
};


//! Transparent add color material renderer
class COpenGLESMaterialRenderer_TRANSPARENT_ADD_COLOR : public COpenGLESMaterialRenderer
{
public:

	COpenGLESMaterialRenderer_TRANSPARENT_ADD_COLOR(video::COpenGLESDriver* d)
		: COpenGLESMaterialRenderer(d) {}

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.getTexture(0));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if ((material.MaterialType != lastMaterial.MaterialType) || resetAllRenderstates)
		{
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_BLEND);
		}
	}

	virtual void OnUnsetMaterial()
	{
		glDisable(GL_BLEND);
	}

	//! Returns if the material is transparent.
	virtual bool isTransparent() const
	{
		return true;
	}
};


//! Transparent vertex alpha material renderer
class COpenGLESMaterialRenderer_TRANSPARENT_VERTEX_ALPHA : public COpenGLESMaterialRenderer
{
public:

	COpenGLESMaterialRenderer_TRANSPARENT_VERTEX_ALPHA(video::COpenGLESDriver* d)
		: COpenGLESMaterialRenderer(d) {}

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.getTexture(0));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PRIMARY_COLOR );

			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PRIMARY_COLOR );
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);

			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
		}
		glDepthMask(GL_FALSE);
	}

	virtual void OnUnsetMaterial()
	{
		// default values
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE );
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_TEXTURE );
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_PREVIOUS );
		glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE );
		glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);

		glDisable(GL_BLEND);
	}

	//! Returns if the material is transparent.
	virtual bool isTransparent() const
	{
		return true;
	}
};


//! Transparent alpha channel material renderer
class COpenGLESMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL : public COpenGLESMaterialRenderer
{
public:

	COpenGLESMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL(video::COpenGLESDriver* d)
		: COpenGLESMaterialRenderer(d) {}

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.getTexture(0));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates
			|| material.MaterialTypeParam != lastMaterial.MaterialTypeParam )
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);

			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_TEXTURE);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST);

			f32 refValue = material.MaterialTypeParam;
			if ( refValue == 0.0f )
				refValue = 0.5f;

			glAlphaFunc(GL_GREATER, refValue);
		}
	}

	virtual void OnUnsetMaterial()
	{
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
	}

	//! Returns if the material is transparent.
	virtual bool isTransparent() const
	{
		return true;
	}
};



//! Transparent alpha channel material renderer
class COpenGLESMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF : public COpenGLESMaterialRenderer
{
public:

	COpenGLESMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF(video::COpenGLESDriver* d)
		: COpenGLESMaterialRenderer(d) {}

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.getTexture(0));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.5);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}
	}

	virtual void OnUnsetMaterial()
	{
		glDisable(GL_ALPHA_TEST);
	}

	//! Returns if the material is transparent.
	virtual bool isTransparent() const
	{
		return false;  // this material is not really transparent because it does no blending.
	}
};


//! material renderer for all kinds of lightmaps
class COpenGLESMaterialRenderer_LIGHTMAP : public COpenGLESMaterialRenderer
{
public:

	COpenGLESMaterialRenderer_LIGHTMAP(video::COpenGLESDriver* d)
		: COpenGLESMaterialRenderer(d) {}

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(2);
		Driver->setTexture(1, material.getTexture(1));
		Driver->setTexture(0, material.getTexture(0));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			// diffuse map

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			switch (material.MaterialType)
			{
				case EMT_LIGHTMAP_LIGHTING:
				case EMT_LIGHTMAP_LIGHTING_M2:
				case EMT_LIGHTMAP_LIGHTING_M4:
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
					break;
				case EMT_LIGHTMAP_ADD:
				case EMT_LIGHTMAP:
				case EMT_LIGHTMAP_M2:
				case EMT_LIGHTMAP_M4:
				default:
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
					break;
			}
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
			glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR );

			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				// lightmap

				Driver->extGlActiveTexture(GL_TEXTURE1);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

				if (material.MaterialType == EMT_LIGHTMAP_ADD)
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD_SIGNED);
				else
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);

				glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);

				switch (material.MaterialType)
				{
					case EMT_LIGHTMAP_M4:
					case EMT_LIGHTMAP_LIGHTING_M4:
						glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 4.0f);
						break;
					case EMT_LIGHTMAP_M2:
					case EMT_LIGHTMAP_LIGHTING_M2:
						glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 2.0f);
						break;
					default:
						glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 1.0f);
				}
			}
		}
	}

	virtual void OnUnsetMaterial()
	{
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			Driver->extGlActiveTexture(GL_TEXTURE1);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 1.f );
			Driver->extGlActiveTexture(GL_TEXTURE0);
		}
	}
};



//! detail map  material renderer
class COpenGLESMaterialRenderer_DETAIL_MAP : public COpenGLESMaterialRenderer
{
public:

	COpenGLESMaterialRenderer_DETAIL_MAP(video::COpenGLESDriver* d)
		: COpenGLESMaterialRenderer(d) {}

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(2);
		Driver->setTexture(1, material.getTexture(1));
		Driver->setTexture(0, material.getTexture(0));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			// diffuse map

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				// detail map

				Driver->extGlActiveTexture(GL_TEXTURE1);

				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD_SIGNED);

				glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE0_RGB,GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND0_RGB,GL_SRC_COLOR);

				glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE1_RGB, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND1_RGB,GL_SRC_COLOR);
				Driver->extGlActiveTexture(GL_TEXTURE0);
			}
		}
	}
};


//! sphere map material renderer
class COpenGLESMaterialRenderer_SPHERE_MAP : public COpenGLESMaterialRenderer
{
public:

	COpenGLESMaterialRenderer_SPHERE_MAP(video::COpenGLESDriver* d)
		: COpenGLESMaterialRenderer(d) {}

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(1);
		Driver->setTexture(0, material.getTexture(0));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				Driver->extGlActiveTexture(GL_TEXTURE1);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

				Driver->extGlActiveTexture(GL_TEXTURE0);
			}

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glDisable(GL_ALPHA_TEST);
		}
	}

	virtual void OnUnsetMaterial()
	{
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
			Driver->extGlActiveTexture(GL_TEXTURE0);

	}
};


//! reflection 2 layer material renderer
class COpenGLESMaterialRenderer_REFLECTION_2_LAYER : public COpenGLESMaterialRenderer
{
public:

	COpenGLESMaterialRenderer_REFLECTION_2_LAYER(video::COpenGLESDriver* d)
		: COpenGLESMaterialRenderer(d) {}

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(2);
		Driver->setTexture(1, material.getTexture(1));
		Driver->setTexture(0, material.getTexture(0));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE );
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR );

				Driver->extGlActiveTexture(GL_TEXTURE1);

				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);

			}

		}
	}

	virtual void OnUnsetMaterial()
	{
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			Driver->extGlActiveTexture(GL_TEXTURE1);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}

		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			Driver->extGlActiveTexture(GL_TEXTURE0);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
	}
};


//! reflection 2 layer material renderer
class COpenGLESMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER : public COpenGLESMaterialRenderer
{
public:

	COpenGLESMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER(video::COpenGLESDriver* d)
		: COpenGLESMaterialRenderer(d) {}

	virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(2);
		Driver->setTexture(1, material.getTexture(1));
		Driver->setTexture(0, material.getTexture(0));
		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		if (material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates)
		{
			if (Driver->queryFeature(EVDF_MULTITEXTURE))
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE );
				glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR );

				Driver->extGlActiveTexture(GL_TEXTURE1);

				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
				glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
			}

			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			glEnable(GL_BLEND);
		}
	}

	virtual void OnUnsetMaterial()
	{
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			Driver->extGlActiveTexture(GL_TEXTURE1);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}

		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			Driver->extGlActiveTexture(GL_TEXTURE0);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
		glDisable(GL_BLEND);
	}

	//! Returns if the material is transparent.
	virtual bool isTransparent() const
	{
		return true;
	}
};

} // end namespace video
} // end namespace irr

#endif

#endif // __C_OPENGLES_MATERIAL_RENDERER_H_INCLUDED__
