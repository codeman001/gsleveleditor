#include "CCommonGLMaterialRenderer.h"

#if defined(_IRR_COMPILE_WITH_OPENGL_) || defined(_IRR_COMPILE_WITH_OPENGL_ES_) || defined(_IRR_COMPILE_WITH_PS3_)
#include "CCommonGLDriver.h"
#include "EDriverFeatures.h"
//#include "EVertexAttributes.h"

namespace irr
{
namespace video
{
namespace
{
u32 getGLBlend(E_BLEND_FACTOR factor)
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

u32 getTexelAlpha(E_BLEND_FACTOR factor)
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

} // end anonymous namespace

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_SOLID::onSetMaterial(const SMaterial& material,
													const SMaterial& lastMaterial,
													bool resetAllRenderstates,
													IMaterialRendererServices* services)
{
	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

	if (resetAllRenderstates || (material.MaterialType != lastMaterial.MaterialType ))
	{
		// thanks to Murphy, the following line removed some
		// bugs with several OpenGL implementations.
		Driver->setTexEnvMode(GL_MODULATE);
	}
}

u32 CCommonGLMaterialRenderer_SOLID::getVertexAttributeMask() const
{
	//return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
	return 0;
}

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_ONETEXTURE_BLEND::onSetMaterial(const SMaterial& material,
															   const SMaterial& lastMaterial,
															   bool resetAllRenderstates,
															   IMaterialRendererServices* services)
{
	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

#ifndef _IRR_COMPILE_WITH_PS3_
//		if (material.MaterialType != lastMaterial.MaterialType ||
//			material.MaterialTypeParam != lastMaterial.MaterialTypeParam ||
//			resetAllRenderstates)
	{
		E_BLEND_FACTOR srcFact,dstFact;
		E_MODULATE_FUNC modulate;
		unpack_texureBlendFunc(srcFact, dstFact, modulate, material.getMaterialTypeParam(0));

		Driver->setTexEnvMode(GL_COMBINE);
		Driver->setCombineRGB(GL_MODULATE);
		Driver->setSourceRGB(0, GL_TEXTURE);

		Driver->setRGBScale((f32) modulate);

		_glBlendFunc( getGLBlend(srcFact), getGLBlend(dstFact) );
		_glEnable(GL_BLEND);

		if ( getTexelAlpha(srcFact) || getTexelAlpha(dstFact) )
		{
			Driver->setCombineAlpha(GL_REPLACE);
			Driver->setSourceAlpha(0, GL_TEXTURE);
			Driver->setSourceRGB(1, GL_PRIMARY_COLOR);
		}
		else
		{
			Driver->setSourceRGB(1, GL_PREVIOUS);
		}
	}
#endif		
}

void CCommonGLMaterialRenderer_ONETEXTURE_BLEND::onUnsetMaterial()
{
#ifndef _IRR_COMPILE_WITH_PS3_
	Driver->setTexEnvMode(GL_MODULATE);
	Driver->setRGBScale(1.f);
	Driver->setSourceRGB(1, GL_PREVIOUS);

	_glDisable(GL_BLEND);
#endif
}

u32 CCommonGLMaterialRenderer_ONETEXTURE_BLEND::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
}

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_SOLID_2_LAYER::onSetMaterial(const SMaterial& material,
															const SMaterial& lastMaterial,
															bool resetAllRenderstates,
															IMaterialRendererServices* services)
{
	Driver->disableTextures(2);
	Driver->setTexture(1, material.getTexture(1));
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
#ifndef _IRR_COMPILE_WITH_PS3_
	if (material.getMaterialType() != lastMaterial.getMaterialType()
		|| resetAllRenderstates)
	{
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			Driver->activeTexture(GL_TEXTURE1);
			Driver->setTexEnvMode(GL_COMBINE);
			Driver->setCombineAlpha(GL_REPLACE);
			Driver->setSourceAlpha(0, GL_PRIMARY_COLOR);
			Driver->setCombineRGB(GL_INTERPOLATE);
			Driver->setSourceRGB(0, GL_PREVIOUS);
			Driver->setSourceRGB(1, GL_TEXTURE);
			Driver->setSourceRGB(2, GL_PRIMARY_COLOR);
			Driver->setOperandRGB(2, GL_SRC_ALPHA);
		}
	}
#endif
}

void CCommonGLMaterialRenderer_SOLID_2_LAYER::onUnsetMaterial()
{
#ifndef _IRR_COMPILE_WITH_PS3_
	if (Driver->queryFeature(EVDF_MULTITEXTURE))
	{
		Driver->activeTexture(GL_TEXTURE1);
		Driver->setTexEnvMode(GL_MODULATE);
		Driver->setOperandRGB(2, GL_SRC_COLOR);
		Driver->activeTexture(GL_TEXTURE0);
	}
#endif
}

u32 CCommonGLMaterialRenderer_SOLID_2_LAYER::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0 | EVA_TEXCOORD1;
}

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_SOLID_2_LAYER_DECAL::onSetMaterial(const SMaterial& material,
																  const SMaterial& lastMaterial,
																  bool resetAllRenderstates,
																  IMaterialRendererServices* services)
{
	Driver->disableTextures(2);
	Driver->setTexture(1, material.getTexture(1));
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
#ifndef _IRR_COMPILE_WITH_PS3_
	if (material.getMaterialType() != lastMaterial.getMaterialType()
		|| resetAllRenderstates)
	{
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			Driver->activeTexture(GL_TEXTURE1);
			Driver->setTexEnvMode(GL_COMBINE);
			Driver->setCombineAlpha(GL_REPLACE);
			Driver->setSourceAlpha(0, GL_PRIMARY_COLOR);
			Driver->setCombineRGB(GL_INTERPOLATE);
			Driver->setSourceRGB(0, GL_TEXTURE);
			Driver->setSourceRGB(1, GL_PREVIOUS);
			Driver->setSourceRGB(2, GL_TEXTURE);
			Driver->setOperandRGB(2, GL_SRC_ALPHA);
		}
	}
#endif
}

void CCommonGLMaterialRenderer_SOLID_2_LAYER_DECAL::onUnsetMaterial()
{
#ifndef _IRR_COMPILE_WITH_PS3_
	if (Driver->queryFeature(EVDF_MULTITEXTURE))
	{
		Driver->activeTexture(GL_TEXTURE1);
		Driver->setTexEnvMode(GL_MODULATE);
		Driver->setOperandRGB(2, GL_SRC_COLOR);
		Driver->activeTexture(GL_TEXTURE0);
	}
#endif
}

u32 CCommonGLMaterialRenderer_SOLID_2_LAYER_DECAL::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0 | EVA_TEXCOORD1;
}

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_TRANSPARENT_ADD_COLOR::onSetMaterial(const SMaterial& material,
																	const SMaterial& lastMaterial,
																	bool resetAllRenderstates,
																	IMaterialRendererServices* services)
{
	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

	if ((material.getMaterialType() != lastMaterial.getMaterialType()) || resetAllRenderstates)
	{
		_glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		Driver->setTexEnvMode(GL_MODULATE);
		_glEnable(GL_BLEND);
	}
}

void CCommonGLMaterialRenderer_TRANSPARENT_ADD_COLOR::onUnsetMaterial()
{
	_glDisable(GL_BLEND);
}

bool CCommonGLMaterialRenderer_TRANSPARENT_ADD_COLOR::isTransparent() const
{
	return true;
}

u32 CCommonGLMaterialRenderer_TRANSPARENT_ADD_COLOR::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
}

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA::onSetMaterial(const SMaterial& material,
																	   const SMaterial& lastMaterial,
																	   bool resetAllRenderstates,
																	   IMaterialRendererServices* services)
{
	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates)
	{
#if defined(_IRR_COMPILE_WITH_PS3_)
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        _glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        _glEnable(GL_BLEND);
#else
		Driver->setTexEnvMode(GL_COMBINE);

		Driver->setCombineAlpha(GL_REPLACE);
		Driver->setSourceAlpha(0, GL_PRIMARY_COLOR);

		Driver->setCombineRGB(GL_MODULATE);
		Driver->setSourceRGB(0, GL_PRIMARY_COLOR);
		Driver->setSourceRGB(1, GL_TEXTURE);

		_glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		_glEnable(GL_BLEND);
#endif
	}
}

void CCommonGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA::onUnsetMaterial()
{
	// default values
#ifndef _IRR_COMPILE_WITH_PS3_
	Driver->setTexEnvMode(GL_MODULATE);
	Driver->setCombineAlpha(GL_MODULATE);
	Driver->setSourceAlpha(0, GL_TEXTURE);
	Driver->setSourceAlpha(1, GL_PREVIOUS);
	Driver->setCombineRGB(GL_MODULATE);
	Driver->setSourceRGB(0, GL_TEXTURE);
	Driver->setSourceRGB(1, GL_PREVIOUS);
#endif
	
	_glDisable(GL_BLEND);
}

//! Returns if the material is transparent.
bool CCommonGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA::isTransparent() const
{
	return true;
}

u32 CCommonGLMaterialRenderer_TRANSPARENT_VERTEX_ALPHA::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
}

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_TRANSPARENT_TEXTURE_VERTEX_ALPHA::onSetMaterial(const SMaterial& material,
																			   const SMaterial& lastMaterial,
																			   bool resetAllRenderstates,
																			   IMaterialRendererServices* services)
{
	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates)
	{
		Driver->setTexEnvMode(GL_MODULATE);
		_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		_glEnable(GL_BLEND);
	}
}

void CCommonGLMaterialRenderer_TRANSPARENT_TEXTURE_VERTEX_ALPHA::onUnsetMaterial()
{
	// default values
	_glDisable(GL_BLEND);
}

bool CCommonGLMaterialRenderer_TRANSPARENT_TEXTURE_VERTEX_ALPHA::isTransparent() const
{
	return true;
}

u32 CCommonGLMaterialRenderer_TRANSPARENT_TEXTURE_VERTEX_ALPHA::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
}

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL::onSetMaterial(const SMaterial& material,
																		const SMaterial& lastMaterial,
																		bool resetAllRenderstates,
																		IMaterialRendererServices* services)
{
	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
#if defined(_IRR_COMPILE_WITH_PS3_)
	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates
		|| material.getMaterialTypeParam(0) != lastMaterial.getMaterialTypeParam(0) )
	{
		Driver->setTexEnvMode(GL_MODULATE);
		_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		_glEnable(GL_BLEND);
	}
#else
	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates
		|| material.getMaterialTypeParam(0) != lastMaterial.getMaterialTypeParam(0) )
	{
		Driver->setTexEnvMode(GL_COMBINE);
		Driver->setCombineRGB(GL_MODULATE);
		Driver->setSourceRGB(0, GL_TEXTURE);
		Driver->setSourceRGB(1, GL_PREVIOUS);

		Driver->setCombineAlpha(GL_REPLACE);
		Driver->setSourceAlpha(0, GL_TEXTURE);

		_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		_glEnable(GL_BLEND);
	}
#endif
}

void CCommonGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL::onUnsetMaterial()
{
	_glDisable(GL_BLEND);
}

bool CCommonGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL::isTransparent() const
{
	return true;
}

u32 CCommonGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
}

//cgm simple transparent material used for editor
//------------------------------------------------------------------------------
void CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP::onSetMaterial(const SMaterial& material,
																		const SMaterial& lastMaterial,
																		bool resetAllRenderstates,
																		IMaterialRendererServices* services)
{
	Driver->disableTextures(2);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setTexture(1, material.getTexture(0));

	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);


	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates
		|| material.getMaterialTypeParam(0) != lastMaterial.getMaterialTypeParam(0) || material.getDiffuseColor() != lastMaterial.getDiffuseColor())
	{
		GLfloat rgba[4] = {
					(f32)(material.getDiffuseColor().R)/255,
					(f32)(material.getDiffuseColor().G)/255,
					(f32)(material.getDiffuseColor().B)/255,
					(f32)(material.getDiffuseColor().A)/255
				  };

	
		//_glDisable(GL_COLOR_ARRAY);

		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			_glEnable(GL_BLEND);
			_glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);

			//Driver->activeTexture(GL_TEXTURE0);

			Driver->setTexEnvMode(GL_COMBINE);

			Driver->setCombineRGB(GL_MODULATE);
			Driver->setSourceRGB(0, GL_TEXTURE);
			Driver->setSourceRGB(1, GL_PRIMARY_COLOR);

			Driver->setCombineAlpha(GL_MODULATE);
			Driver->setSourceAlpha(0, GL_TEXTURE);
			Driver->setSourceAlpha(1, GL_PRIMARY_COLOR);

			Driver->activeTexture(GL_TEXTURE1);

			glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);

			Driver->setTexEnvMode(GL_COMBINE);

			Driver->setCombineRGB(GL_MODULATE);
			Driver->setSourceRGB(0, GL_CONSTANT);
			Driver->setSourceRGB(1, GL_PREVIOUS);

			Driver->setCombineAlpha(GL_MODULATE);
			Driver->setSourceAlpha(0, GL_CONSTANT);
			Driver->setSourceAlpha(1, GL_PREVIOUS);
		}
		else
		{
			_glEnable(GL_BLEND);
			_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);

			Driver->setTexEnvMode(GL_COMBINE);

			Driver->setCombineRGB(GL_MODULATE);
			Driver->setSourceRGB(0, GL_TEXTURE);
			Driver->setSourceRGB(1, GL_CONSTANT);

			Driver->setCombineAlpha(GL_MODULATE);
			Driver->setSourceAlpha(0, GL_TEXTURE);
			Driver->setSourceAlpha(1, GL_CONSTANT);
		}
		
	}

}

void CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP::onUnsetMaterial()
{
	GLfloat rgba[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);
	_glDisable(GL_BLEND);

	if (Driver->queryFeature(EVDF_MULTITEXTURE))
	{
		Driver->activeTexture(GL_TEXTURE1);
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);

		Driver->setTexEnvMode(GL_MODULATE);

		Driver->activeTexture(GL_TEXTURE0);
		Driver->setTexEnvMode(GL_MODULATE);
	}
}

bool CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP::isTransparent() const
{
	return true;
}

u32 CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
}

bool CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP::onRender (IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype)
{
	Driver->setColorMaterialEnable (false);
	return true;
}
//------------------------------------------------------------------------------
void CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP_NOADD::onSetMaterial(const SMaterial& material,
																		const SMaterial& lastMaterial,
																		bool resetAllRenderstates,
																		IMaterialRendererServices* services)
{
	Driver->disableTextures(2);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setTexture(1, material.getTexture(0));

	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);


	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates
		|| material.getMaterialTypeParam(0) != lastMaterial.getMaterialTypeParam(0) || material.getDiffuseColor() != lastMaterial.getDiffuseColor())
	{
		GLfloat rgba[4] = {
					(f32)(material.getDiffuseColor().R)/255,
					(f32)(material.getDiffuseColor().G)/255,
					(f32)(material.getDiffuseColor().B)/255,
					(f32)(material.getDiffuseColor().A)/255
				  };

	
		//_glDisable(GL_COLOR_ARRAY);

		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			_glEnable(GL_BLEND);
			_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);

			//Driver->activeTexture(GL_TEXTURE0);

			Driver->setTexEnvMode(GL_COMBINE);

			Driver->setCombineRGB(GL_MODULATE);
			Driver->setSourceRGB(0, GL_TEXTURE);
			Driver->setSourceRGB(1, GL_PRIMARY_COLOR);

			Driver->setCombineAlpha(GL_MODULATE);
			Driver->setSourceAlpha(0, GL_TEXTURE);
			Driver->setSourceAlpha(1, GL_PRIMARY_COLOR);

			Driver->activeTexture(GL_TEXTURE1);

			_glEnable(GL_BLEND);
			_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);

			Driver->setTexEnvMode(GL_COMBINE);

			Driver->setCombineRGB(GL_MODULATE);
			Driver->setSourceRGB(0, GL_CONSTANT);
			Driver->setSourceRGB(1, GL_PREVIOUS);

			Driver->setCombineAlpha(GL_MODULATE);
			Driver->setSourceAlpha(0, GL_CONSTANT);
			Driver->setSourceAlpha(1, GL_PREVIOUS);
		}
		else
		{
			_glEnable(GL_BLEND);
			_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);

			Driver->setTexEnvMode(GL_COMBINE);

			Driver->setCombineRGB(GL_MODULATE);
			Driver->setSourceRGB(0, GL_TEXTURE);
			Driver->setSourceRGB(1, GL_CONSTANT);

			Driver->setCombineAlpha(GL_MODULATE);
			Driver->setSourceAlpha(0, GL_TEXTURE);
			Driver->setSourceAlpha(1, GL_CONSTANT);
		}
		
	}

}

void CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP_NOADD::onUnsetMaterial()
{
	GLfloat rgba[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	if (Driver->queryFeature(EVDF_MULTITEXTURE))
	{
		Driver->activeTexture(GL_TEXTURE1);
		
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);
		_glDisable(GL_BLEND);

		Driver->setTexEnvMode(GL_MODULATE);

		Driver->activeTexture(GL_TEXTURE0);

		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);
		_glDisable(GL_BLEND);

		Driver->setTexEnvMode(GL_MODULATE);
	}
	else
	{
		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);
		_glDisable(GL_BLEND);

		Driver->setTexEnvMode(GL_MODULATE);
	}
}

bool CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP_NOADD::isTransparent() const
{
	return true;
}

u32 CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP_NOADD::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
}

bool CCommonGLMaterialRenderer_TRANSPARENT_TRICOMP_NOADD::onRender (IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype)
{
	Driver->setColorMaterialEnable (false);
	return true;
}

//------------------------------------------------------------------------------
void CCommonGLMaterialRenderer_OPAQUE_NO_VERTEXCOLOR::onSetMaterial(const SMaterial& material,
																		const SMaterial& lastMaterial,
																		bool resetAllRenderstates,
																		IMaterialRendererServices* services)
{
	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates
		|| material.getMaterialTypeParam(0) != lastMaterial.getMaterialTypeParam(0) )
	{
		GLfloat rgba[4] = {
					(f32)(material.getDiffuseColor().R)/255,
					(f32)(material.getDiffuseColor().G)/255,
					(f32)(material.getDiffuseColor().B)/255,
					(f32)(material.getDiffuseColor().A)/255
				  };

		_glDisable(GL_BLEND);

		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);

		Driver->setTexEnvMode(GL_COMBINE);

		Driver->setCombineRGB(GL_MODULATE);
		Driver->setSourceRGB(0, GL_TEXTURE);
		Driver->setSourceRGB(1, GL_CONSTANT);
		
	}
}

void CCommonGLMaterialRenderer_OPAQUE_NO_VERTEXCOLOR::onUnsetMaterial()
{
	GLfloat rgba[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);
}

bool CCommonGLMaterialRenderer_OPAQUE_NO_VERTEXCOLOR::isTransparent() const
{
	return false;
}

u32 CCommonGLMaterialRenderer_OPAQUE_NO_VERTEXCOLOR::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
}

bool CCommonGLMaterialRenderer_OPAQUE_NO_VERTEXCOLOR::onRender (IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype)
{
	Driver->setColorMaterialEnable (true);
	return true;
}

//------------------------------------------------------------------------------
void CCommonGLMaterialRenderer_TRANSPARENT_NO_VERTEXCOLOR::onSetMaterial(const SMaterial& material,
																		const SMaterial& lastMaterial,
																		bool resetAllRenderstates,
																		IMaterialRendererServices* services)
{
	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates
		|| material.getMaterialTypeParam(0) != lastMaterial.getMaterialTypeParam(0) )
	{
		GLfloat rgba[4] = {
					(f32)(material.getDiffuseColor().R)/255,
					(f32)(material.getDiffuseColor().G)/255,
					(f32)(material.getDiffuseColor().B)/255,
					(f32)(material.getDiffuseColor().A)/255
				  };

		_glEnable(GL_BLEND);
		_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);

		Driver->setTexEnvMode(GL_COMBINE);

		Driver->setCombineRGB(GL_MODULATE);
		Driver->setSourceRGB(0, GL_TEXTURE);
		Driver->setSourceRGB(1, GL_CONSTANT);

		Driver->setCombineAlpha(GL_MODULATE);
		Driver->setSourceAlpha(0, GL_TEXTURE);
		Driver->setSourceAlpha(1, GL_PREVIOUS);
	}
}

void CCommonGLMaterialRenderer_TRANSPARENT_NO_VERTEXCOLOR::onUnsetMaterial()
{
	GLfloat rgba[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);
	_glDisable(GL_BLEND);
}

bool CCommonGLMaterialRenderer_TRANSPARENT_NO_VERTEXCOLOR::isTransparent() const
{
	return true;
}

u32 CCommonGLMaterialRenderer_TRANSPARENT_NO_VERTEXCOLOR::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
}

bool CCommonGLMaterialRenderer_TRANSPARENT_NO_VERTEXCOLOR::onRender (IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype)
{
	Driver->setColorMaterialEnable (true);
	return true;
}
//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF::onSetMaterial(const SMaterial& material,
																			const SMaterial& lastMaterial,
																			bool resetAllRenderstates,
																			IMaterialRendererServices* services)
{
	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates)
	{
#if defined(_IRR_COMPILE_WITH_PS3_)
		Driver->setTexEnvMode(GL_MODULATE);
#else
		Driver->setTexEnvMode(GL_COMBINE);
		Driver->setCombineRGB(GL_MODULATE);
		Driver->setSourceRGB(0, GL_TEXTURE);
		Driver->setSourceRGB(1, GL_PREVIOUS);

		Driver->setCombineAlpha(GL_REPLACE);
		Driver->setSourceAlpha(0, GL_TEXTURE);
#endif
		_glEnable(GL_ALPHA_TEST);
		//glAlphaFunc(GL_GREATER, 0.5f);
	}
}

void CCommonGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF::onUnsetMaterial()
{
	_glDisable(GL_ALPHA_TEST);
}

bool CCommonGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF::isTransparent() const
{
	return false;  // this material is not really transparent because it does no blending.
}

u32 CCommonGLMaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
}

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_LIGHTMAP::onSetMaterial(const SMaterial& material,
													   const SMaterial& lastMaterial,
													   bool resetAllRenderstates,
													   IMaterialRendererServices* services)
{
	Driver->disableTextures(2);
	Driver->setTexture(1, material.getTexture(1));
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

#if defined (_IRR_USE_PS3_DEVICE_)
	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates)
	{
		// diffuse map
		switch (material.getMaterialType())
		{
			case EMT_LIGHTMAP_LIGHTING:
			case EMT_LIGHTMAP_LIGHTING_M2:
			case EMT_LIGHTMAP_LIGHTING_M4:
				Driver->setTexEnvMode(GL_MODULATE);
				break;
			case EMT_LIGHTMAP_ADD:
			case EMT_LIGHTMAP:
			case EMT_LIGHTMAP_M2:
			case EMT_LIGHTMAP_M4:
			default:
				Driver->setTexEnvMode(GL_REPLACE);
				break;
		}

		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			// detail map
			Driver->activeTexture(GL_TEXTURE1);
			Driver->setTexEnvMode(GL_MODULATE);
			Driver->activeTexture(GL_TEXTURE0);
		}
	}
#else
	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates)
	{
		// diffuse map

		switch (material.getMaterialType())
		{
			case EMT_LIGHTMAP_LIGHTING:
			case EMT_LIGHTMAP_LIGHTING_M2:
			case EMT_LIGHTMAP_LIGHTING_M4:
				Driver->setTexEnvMode(GL_MODULATE);
				break;
			case EMT_LIGHTMAP_ADD:
			case EMT_LIGHTMAP:
			case EMT_LIGHTMAP_M2:
			case EMT_LIGHTMAP_M4:
			default:
				Driver->setTexEnvMode(GL_REPLACE);
				break;
		}

		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			// lightmap

			Driver->activeTexture(GL_TEXTURE1);
			Driver->setTexEnvMode(GL_COMBINE);

			if (material.getMaterialType() == EMT_LIGHTMAP_ADD)
				Driver->setCombineRGB(GL_ADD_SIGNED);
			else
				Driver->setCombineRGB(GL_MODULATE);

			Driver->setSourceRGB(0, GL_PREVIOUS);
			Driver->setSourceRGB(1, GL_TEXTURE);

			Driver->setCombineAlpha(GL_MODULATE);
			Driver->setSourceAlpha(0, GL_PREVIOUS);
			Driver->setSourceAlpha(1, GL_PREVIOUS);

			switch (material.getMaterialType())
			{
				case EMT_LIGHTMAP_M4:
				case EMT_LIGHTMAP_LIGHTING_M4:
					Driver->setRGBScale(4.0f);
					break;
				case EMT_LIGHTMAP_M2:
				case EMT_LIGHTMAP_LIGHTING_M2:
					Driver->setRGBScale(2.0f);
					break;
				default:
					Driver->setRGBScale(1.0f);
			}
		}
	}
#endif
}

void CCommonGLMaterialRenderer_LIGHTMAP::onUnsetMaterial()
{
	if (Driver->queryFeature(EVDF_MULTITEXTURE))
	{
		Driver->activeTexture(GL_TEXTURE1);
		Driver->setRGBScale(1.0f);
		Driver->setTexEnvMode(GL_MODULATE);
		Driver->activeTexture(GL_TEXTURE0);
		Driver->setTexEnvMode(GL_MODULATE);
	}
}

u32 CCommonGLMaterialRenderer_LIGHTMAP::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0 | EVA_TEXCOORD1;
}

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_DETAIL_MAP::onSetMaterial(
	const SMaterial& material,
	const SMaterial& lastMaterial,
	bool resetAllRenderstates,
	IMaterialRendererServices* services
)
{
	Driver->disableTextures(2);
	Driver->setTexture(1, material.getTexture(1));
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
#if defined (_IRR_USE_PS3_DEVICE_)
	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates)
	{
		// diffuse map

		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			// detail map
			Driver->activeTexture(GL_TEXTURE1);
			Driver->setTexEnvMode(GL_MODULATE);
			Driver->activeTexture(GL_TEXTURE0);
		}
	}
#else
	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates)
	{
		// diffuse map

		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			// detail map
			Driver->activeTexture(GL_TEXTURE1);

			Driver->setTexEnvMode(GL_COMBINE);
			Driver->setCombineRGB(GL_ADD_SIGNED);
			Driver->setSourceRGB(0, GL_PREVIOUS);
			Driver->setSourceRGB(1, GL_TEXTURE);
			Driver->activeTexture(GL_TEXTURE0);
		}
	}
#endif
}
	
void CCommonGLMaterialRenderer_DETAIL_MAP::onUnsetMaterial()
{
	Driver->activeTexture(GL_TEXTURE1);
	Driver->setTexEnvMode(GL_MODULATE);
	Driver->activeTexture(GL_TEXTURE0);
}

u32 CCommonGLMaterialRenderer_DETAIL_MAP::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0 | EVA_TEXCOORD1;
}

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_SPHERE_MAP::onSetMaterial(
	const SMaterial& material,
	const SMaterial& lastMaterial,
	bool resetAllRenderstates,
	IMaterialRendererServices* services
)
{
	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
#ifndef _IRR_COMPILE_WITH_PS3_
	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates)
	{
		Driver->setTexGen(0, ETGT_SIMPLE_SPHERE_MAP);
// 		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
// 		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

// 		_glEnable(GL_TEXTURE_GEN_S);
// 		_glEnable(GL_TEXTURE_GEN_T);
	}
#endif
}

void CCommonGLMaterialRenderer_SPHERE_MAP::onUnsetMaterial()
{
#ifndef _IRR_COMPILE_WITH_PS3_
	Driver->setTexGen(0, ETGT_NONE);
// 	_glDisable(GL_TEXTURE_GEN_S);
// 	_glDisable(GL_TEXTURE_GEN_T);
#endif
}

u32 CCommonGLMaterialRenderer_SPHERE_MAP::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_NORMAL | EVA_TEXCOORD0;
}

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_REFLECTION_2_LAYER::onSetMaterial(
	const SMaterial& material,
	const SMaterial& lastMaterial,
	bool resetAllRenderstates,
	IMaterialRendererServices* services
)
{
	Driver->disableTextures(2);
	Driver->setTexture(1, material.getTexture(1));
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
#ifndef _IRR_COMPILE_WITH_PS3_
	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates)
	{
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			Driver->activeTexture(GL_TEXTURE1);
			Driver->setTexEnvMode(GL_COMBINE);
			Driver->setCombineRGB(GL_ADD);
			Driver->setSourceRGB(0, GL_PREVIOUS);
			Driver->setSourceRGB(1, GL_TEXTURE);
			Driver->setTexGen(1, ETGT_SIMPLE_SPHERE_MAP);
		}
// 		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
// 		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
// 		_glEnable(GL_TEXTURE_GEN_S);
// 		_glEnable(GL_TEXTURE_GEN_T);
	}
#endif
}

void CCommonGLMaterialRenderer_REFLECTION_2_LAYER::onUnsetMaterial()
{
#ifndef _IRR_COMPILE_WITH_PS3_
	if (Driver->queryFeature(EVDF_MULTITEXTURE))
	{
		Driver->activeTexture(GL_TEXTURE1);
		Driver->setTexEnvMode(GL_MODULATE);
		Driver->setTexGen(1, ETGT_NONE);
//	}
// 	_glDisable(GL_TEXTURE_GEN_S);
// 	_glDisable(GL_TEXTURE_GEN_T);
//	if (Driver->queryFeature(EVDF_MULTITEXTURE))
//	{
		Driver->activeTexture(GL_TEXTURE0);
	}
#endif
}

u32 CCommonGLMaterialRenderer_REFLECTION_2_LAYER::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_NORMAL | EVA_COLOR0 | EVA_TEXCOORD0 | EVA_TEXCOORD1;
}

//------------------------------------------------------------------------------

void CCommonGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER::onSetMaterial(
	const SMaterial& material,
	const SMaterial& lastMaterial,
	bool resetAllRenderstates,
	IMaterialRendererServices* services
)
{
	Driver->disableTextures(2);
	Driver->setTexture(1, material.getTexture(1));
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);
#ifndef _IRR_COMPILE_WITH_PS3_
	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates)
	{
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			Driver->activeTexture(GL_TEXTURE1);
			Driver->setTexEnvMode(GL_COMBINE);
			Driver->setCombineRGB(GL_ADD);
			Driver->setSourceRGB(0, GL_PREVIOUS);
			Driver->setSourceRGB(1, GL_TEXTURE);
			Driver->setTexGen(1, ETGT_SIMPLE_SPHERE_MAP);
		}
// 		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
// 		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
// 		_glEnable(GL_TEXTURE_GEN_S);
// 		_glEnable(GL_TEXTURE_GEN_T);

		_glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
		_glEnable(GL_BLEND);
	}
#endif
}

void CCommonGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER::onUnsetMaterial()
{
#ifndef _IRR_COMPILE_WITH_PS3_
	if (Driver->queryFeature(EVDF_MULTITEXTURE))
	{
		Driver->activeTexture(GL_TEXTURE1);
		Driver->setTexEnvMode(GL_MODULATE);
		Driver->setTexGen(1, ETGT_NONE);
// 	}
// 	_glDisable(GL_TEXTURE_GEN_S);
// 	_glDisable(GL_TEXTURE_GEN_T);
// 	if (Driver->queryFeature(EVDF_MULTITEXTURE))
// 	{
		Driver->activeTexture(GL_TEXTURE0);
	}
	_glDisable(GL_BLEND);
#endif
}

bool CCommonGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER::isTransparent() const
{
	return true;
}

u32 CCommonGLMaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_NORMAL | EVA_COLOR0 | EVA_TEXCOORD0 | EVA_TEXCOORD1;
}

//------------------------------------------------------------------------------


void CCommonGLMaterialRenderer_2D_ALPHA::onSetMaterial(const SMaterial& material, 
						   const SMaterial& lastMaterial,
						   bool resetAllRenderstates, 
						   IMaterialRendererServices* services)
{
	_glEnable(GL_BLEND);
	_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

#if defined(_IRR_COMPILE_WITH_PS3_)
	if (material.getFlag(EMF_2D_USE_TEXTURE_ALPHA) || material.getFlag(EMF_2D_USE_VERTEX_ALPHA))
	{
		Driver->setTexEnvMode(GL_MODULATE);
	}
#else
	if (material.getFlag(EMF_2D_USE_TEXTURE_ALPHA))
	{
		// if alpha and alpha texture just modulate, otherwise use only the alpha channel
		if (material.getFlag(EMF_2D_USE_VERTEX_ALPHA))
		{
			Driver->setTexEnvMode(GL_MODULATE);
		}
		else
		{
			Driver->setTexEnvMode(GL_COMBINE);
			Driver->setCombineAlpha(GL_REPLACE);
			Driver->setSourceAlpha(0, GL_TEXTURE);
			// rgb always modulates
			Driver->setCombineRGB(GL_MODULATE);
			Driver->setSourceRGB(0, GL_TEXTURE);
			Driver->setSourceRGB(1, GL_PRIMARY_COLOR);
		}
	}
	else
	{
		if (material.getFlag(EMF_2D_USE_VERTEX_ALPHA))
		{
			Driver->setTexEnvMode(GL_COMBINE);
			Driver->setCombineAlpha(GL_REPLACE);
			Driver->setSourceAlpha(0, GL_PRIMARY_COLOR);
			// rgb always modulates
			Driver->setCombineRGB(GL_MODULATE);
			Driver->setSourceRGB(0, GL_TEXTURE);
			Driver->setSourceRGB(1, GL_PRIMARY_COLOR);
		}
		else
		{
			Driver->setTexEnvMode(GL_MODULATE);
		}
	}
#endif
}
			
void CCommonGLMaterialRenderer_2D_ALPHA::onUnsetMaterial()
{
	_glDisable(GL_BLEND);
	Driver->setTexEnvMode(GL_MODULATE);
}

	//! Returns if the material is transparent.
bool CCommonGLMaterialRenderer_2D_ALPHA::isTransparent() const
{
	return true;
}

u32 CCommonGLMaterialRenderer_2D_ALPHA::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_TEXCOORD0 | EVA_COLOR0;
}




//cgm materials for particles
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCommonGLMaterialRenderer_ADDITIVE_REPLACE::onSetMaterial(const SMaterial& material,
																	const SMaterial& lastMaterial,
																	bool resetAllRenderstates,
																	IMaterialRendererServices* services)
{
	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates)
	{
		_glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		Driver->setTexEnvMode(GL_REPLACE);
		_glEnable(GL_BLEND);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCommonGLMaterialRenderer_ADDITIVE_REPLACE::onUnsetMaterial()
{
	_glDisable(GL_BLEND);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    
bool CCommonGLMaterialRenderer_ADDITIVE_REPLACE::isTransparent() const
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u32 CCommonGLMaterialRenderer_ADDITIVE_REPLACE::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCommonGLMaterialRenderer_ADDITIVE_MODULATE::onSetMaterial(const SMaterial& material,
																	const SMaterial& lastMaterial,
																	bool resetAllRenderstates,
																	IMaterialRendererServices* services)
{
	Driver->disableTextures(1);
	Driver->setTexture(0, material.getTexture(0));
	Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

	if (material.getMaterialType() != lastMaterial.getMaterialType() || resetAllRenderstates)
	{
		_glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		Driver->setTexEnvMode(GL_MODULATE);
		_glEnable(GL_BLEND);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCommonGLMaterialRenderer_ADDITIVE_MODULATE::onUnsetMaterial()
{
	_glDisable(GL_BLEND);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CCommonGLMaterialRenderer_ADDITIVE_MODULATE::isTransparent() const
{
	return !false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u32 CCommonGLMaterialRenderer_ADDITIVE_MODULATE::getVertexAttributeMask() const
{
	return EVA_POSITION | EVA_COLOR0 | EVA_TEXCOORD0;
}

} // end namespace video
} // end namespace irr

#endif // _IRR_COMPILE_WITH_OPENGL_ || _IRR_COMPILE_WITH_OPENGL_ES_
