// Copyright (C) 2009-2010 Amundis
// Heavily based on the OpenGL driver implemented by Nikolaus Gebhardt
// and OpenGL ES driver implemented by Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h
#ifndef __C_OGLES2_MATERIAL_RENDERER_H_INCLUDED__
#define __C_OGLES2_MATERIAL_RENDERER_H_INCLUDED__

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_OGLES2_

#include "COGLES2SLMaterialRenderer.h"
#include "COGLES2FixedPipelineShader.h"

namespace irr
{
namespace video
{

	//! Base class for all internal OGLES2 material renderers
	class COGLES2MaterialRenderer : public IMaterialRenderer
	{
	public:

		//! Constructor
		COGLES2MaterialRenderer( video::COGLES2Driver* driver ) :
				Driver( driver ),
				FixedPipeline( driver->fixedPipeline() )
		{
		}

		virtual bool OnRender( IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype )
		{
			return FixedPipeline->OnRender( service, vtxtype );
		}

	protected:

		video::COGLES2Driver* Driver;
		COGLES2FixedPipelineShader* FixedPipeline;
	};


	//! Solid material renderer
	class COGLES2MaterialRenderer_SOLID : public COGLES2MaterialRenderer
	{
	public:

		COGLES2MaterialRenderer_SOLID(video::COGLES2Driver* d) :
				COGLES2MaterialRenderer(d)
		{
		}

		virtual void OnSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
				bool resetAllRenderstates, IMaterialRendererServices* services)
		{			
			FixedPipeline->setRenderMode(EMT_SOLID);
			FixedPipeline->setMaterial(material);

			Driver->disableTextures(1);
			Driver->setActiveTexture(0, material.getTexture(0));
			Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);			
		}
	};


	//! Generic Texture Blend
	class COGLES2MaterialRenderer_ONETEXTURE_BLEND : public COGLES2MaterialRenderer
	{
	public:

		COGLES2MaterialRenderer_ONETEXTURE_BLEND( video::COGLES2Driver* d )
				: COGLES2MaterialRenderer( d ) {}

		virtual void OnSetMaterial( const SMaterial& material, const SMaterial& lastMaterial,
									bool resetAllRenderstates, IMaterialRendererServices* services )
		{
			Driver->disableTextures( 1 );
			Driver->setActiveTexture( 0, material.getTexture( 0 ) );
			Driver->setBasicRenderStates( material, lastMaterial, resetAllRenderstates );
			
			FixedPipeline->setMaterial( material );			

			{
				E_BLEND_FACTOR srcFact, dstFact;
				E_MODULATE_FUNC modulate;
				u32 alphaSource;
				unpack_texureBlendFunc( srcFact, dstFact, modulate, alphaSource, material.MaterialTypeParam );
				
				FixedPipeline->enableAlphaTest();
				FixedPipeline->setAlphaValue( 0.f );

				Driver->enableBlend();
				Driver->blendFunc( srcFact, dstFact );

				if (alphaSource&EAS_TEXTURE)
					FixedPipeline->setRenderMode( EMT_TRANSPARENT_ALPHA_CHANNEL );
				else
					FixedPipeline->setRenderMode( EMT_TRANSPARENT_VERTEX_ALPHA );	
			}
		}

		virtual void OnUnsetMaterial()
		{		
			Driver->disableBlend();
			FixedPipeline->disableAlphaTest();
		}

		//! Returns if the material is transparent.
		/** Is not always transparent, but mostly. */
		virtual bool isTransparent() const
		{
			return true;
		}
	};


	//! Solid 2 layer material renderer
	class COGLES2MaterialRenderer_SOLID_2_LAYER : public COGLES2MaterialRenderer
	{
	public:

		COGLES2MaterialRenderer_SOLID_2_LAYER( video::COGLES2Driver* d )
				: COGLES2MaterialRenderer( d ) {}

		virtual void OnSetMaterial( const SMaterial& material, const SMaterial& lastMaterial,
				bool resetAllRenderstates, IMaterialRendererServices* services )
		{
			Driver->disableTextures( 2 );
			Driver->setActiveTexture( 1, material.getTexture( 1 ) );
			Driver->setActiveTexture( 0, material.getTexture( 0 ) );
			Driver->setBasicRenderStates( material, lastMaterial, resetAllRenderstates );

		}

		virtual void OnUnsetMaterial()
		{

		}
	};


	//! Transparent add color material renderer
	class COGLES2MaterialRenderer_TRANSPARENT_ADD_COLOR : public COGLES2MaterialRenderer
	{
	public:

		COGLES2MaterialRenderer_TRANSPARENT_ADD_COLOR( video::COGLES2Driver* d )
				: COGLES2MaterialRenderer( d ) {}

		virtual void OnSetMaterial( const SMaterial& material, const SMaterial& lastMaterial,
				bool resetAllRenderstates, IMaterialRendererServices* services )
		{
			if (( material.MaterialType != lastMaterial.MaterialType ) || resetAllRenderstates )
			{
				Driver->blendFunc( EBF_ONE, EBF_ONE_MINUS_SRC_COLOR );
				Driver->enableBlend();
				FixedPipeline->setRenderMode( EMT_SOLID );
			}
			FixedPipeline->setMaterial( material );
			Driver->disableTextures( 1 );
			Driver->setActiveTexture( 0, material.getTexture( 0 ) );
			Driver->setBasicRenderStates( material, lastMaterial, resetAllRenderstates );
		}

		virtual void OnUnsetMaterial()
		{
			Driver->disableBlend();
		}

		//! Returns if the material is transparent.
		virtual bool isTransparent() const
		{
			return true;
		}
	};


	//! Transparent vertex alpha material renderer
	class COGLES2MaterialRenderer_TRANSPARENT_VERTEX_ALPHA : public COGLES2MaterialRenderer
	{
	public:

		COGLES2MaterialRenderer_TRANSPARENT_VERTEX_ALPHA( video::COGLES2Driver* d )
				: COGLES2MaterialRenderer( d ) {}

		virtual void OnSetMaterial( const SMaterial& material, const SMaterial& lastMaterial,
				bool resetAllRenderstates, IMaterialRendererServices* services )
		{
			if ( material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates )
			{
				Driver->blendFunc( EBF_ONE, EBF_ONE_MINUS_SRC_ALPHA );
				Driver->enableBlend();
				FixedPipeline->setRenderMode( EMT_TRANSPARENT_VERTEX_ALPHA );
			}
			FixedPipeline->setMaterial( material );
			Driver->disableTextures( 1 );
			Driver->setActiveTexture( 0, material.getTexture( 0 ) );
			Driver->setBasicRenderStates( material, lastMaterial, resetAllRenderstates );
		}

		virtual void OnUnsetMaterial()
		{
			Driver->disableBlend();
		}

		//! Returns if the material is transparent.
		virtual bool isTransparent() const
		{
			return true;
		}
	};


	//! Transparent alpha channel material renderer
	class COGLES2MaterialRenderer_TRANSPARENT_ALPHA_CHANNEL : public COGLES2MaterialRenderer
	{
	public:

		COGLES2MaterialRenderer_TRANSPARENT_ALPHA_CHANNEL( video::COGLES2Driver* d )
				: COGLES2MaterialRenderer( d ) {}

		virtual void OnSetMaterial( const SMaterial& material, const SMaterial& lastMaterial,
				bool resetAllRenderstates, IMaterialRendererServices* services )
		{
			Driver->disableTextures( 1 );
			Driver->setActiveTexture( 0, material.getTexture( 0 ) );
			Driver->setBasicRenderStates( material, lastMaterial, resetAllRenderstates );

			if ( material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates
				|| material.MaterialTypeParam != lastMaterial.MaterialTypeParam )
			{
				Driver->blendFunc( EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA );
				Driver->enableBlend();
				FixedPipeline->setMaterial( material );
				FixedPipeline->enableAlphaTest();

				FixedPipeline->setAlphaValue( material.MaterialTypeParam );
				FixedPipeline->setRenderMode( EMT_TRANSPARENT_ALPHA_CHANNEL );
			}
		}

		virtual void OnUnsetMaterial()
		{
			FixedPipeline->disableAlphaTest();
			Driver->disableBlend();
		}

		//! Returns if the material is transparent.
		virtual bool isTransparent() const
		{
			return true;
		}
	};


	//! Transparent alpha channel material renderer
	class COGLES2MaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF : public COGLES2MaterialRenderer
	{
	public:

		COGLES2MaterialRenderer_TRANSPARENT_ALPHA_CHANNEL_REF( video::COGLES2Driver* d )
				: COGLES2MaterialRenderer( d ) {}

		virtual void OnSetMaterial( const SMaterial& material, const SMaterial& lastMaterial,
				bool resetAllRenderstates, IMaterialRendererServices* services )
		{
			Driver->disableTextures( 1 );
			Driver->setActiveTexture( 0, material.getTexture( 0 ) );
			Driver->setBasicRenderStates( material, lastMaterial, resetAllRenderstates );

			if ( material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates )
			{
				FixedPipeline->enableAlphaTest();
				FixedPipeline->setAlphaValue( 0.5f );
				//TODO : OpenGL ES 2.0 Port glTexEnv
				//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			}
		}

		virtual void OnUnsetMaterial()
		{
			FixedPipeline->disableAlphaTest();
		}

		//! Returns if the material is transparent.
		virtual bool isTransparent() const
		{
			return false; // this material is not really transparent because it does no blending.
		}
	};


	//! material renderer for all kinds of lightmaps
	class COGLES2MaterialRenderer_LIGHTMAP : public COGLES2MaterialRenderer
	{
	public:

		COGLES2MaterialRenderer_LIGHTMAP( video::COGLES2Driver* d )
				: COGLES2MaterialRenderer( d ) {}

		virtual void OnSetMaterial( const SMaterial& material, const SMaterial& lastMaterial,
				bool resetAllRenderstates, IMaterialRendererServices* services )
		{			
			FixedPipeline->setRenderMode( EMT_LIGHTMAP );			
			FixedPipeline->setMaterial( material );

			Driver->disableTextures( 2 );
			Driver->setActiveTexture( 1, material.getTexture( 1 ) );
			Driver->setActiveTexture( 0, material.getTexture( 0 ) );
			Driver->setBasicRenderStates( material, lastMaterial, resetAllRenderstates );

			if ( material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates )
			{
				// diffuse map

				switch ( material.MaterialType )
				{
					case EMT_LIGHTMAP_LIGHTING:
					case EMT_LIGHTMAP_LIGHTING_M2:
					case EMT_LIGHTMAP_LIGHTING_M4:
						//TODO : OpenGL ES 2.0 Port glTexEnv
						//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
						break;
					case EMT_LIGHTMAP_ADD:
					case EMT_LIGHTMAP:
					case EMT_LIGHTMAP_M2:
					case EMT_LIGHTMAP_M4:
					default:
						//TODO : OpenGL ES 2.0 Port glTexEnv
						//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
						break;
				}

				if ( Driver->queryFeature( EVDF_MULTITEXTURE ) )
				{
					// lightmap

					//Driver->extGlActiveTexture(GL_TEXTURE1);
					//TODO : OpenGL ES 2.0 Port glTexEnv
					//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

					if ( material.MaterialType == EMT_LIGHTMAP_ADD )
					{
						//TODO : OpenGL ES 2.0 Port glTexEnv
						//glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD_SIGNED);
					}
					else
					{
						//TODO : OpenGL ES 2.0 Port glTexEnv
						//glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
					}

					//TODO : OpenGL ES 2.0 Port glTexEnv
					//glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_PREVIOUS);
					//glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_TEXTURE);

					//glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
					//glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_PREVIOUS);
					//glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PREVIOUS);

					switch ( material.MaterialType )
					{
						case EMT_LIGHTMAP_M4:
						case EMT_LIGHTMAP_LIGHTING_M4:
							//TODO : OpenGL ES 2.0 Port glTexEnv
							//glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 4.0f);
							break;
						case EMT_LIGHTMAP_M2:
						case EMT_LIGHTMAP_LIGHTING_M2:
							//TODO : OpenGL ES 2.0 Port glTexEnv
							//glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 2.0f);
							break;
						default:
							//TODO : OpenGL ES 2.0 Port glTexEnv
							//glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 1.0f);
							break;
					}
				}
			}
		}

		virtual void OnUnsetMaterial()
		{			
		}
	};


	//! detail map material renderer
	class COGLES2MaterialRenderer_DETAIL_MAP : public COGLES2MaterialRenderer
	{
	public:

		COGLES2MaterialRenderer_DETAIL_MAP( video::COGLES2Driver* d )
				: COGLES2MaterialRenderer( d ) {}

		virtual void OnSetMaterial( const SMaterial& material, const SMaterial& lastMaterial,
									bool resetAllRenderstates, IMaterialRendererServices* services )
		{
			if ( material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates )
			{				
				FixedPipeline->setRenderMode( EMT_DETAIL_MAP );
			}
			FixedPipeline->setMaterial( material );
			Driver->disableTextures( 2 );
			Driver->setActiveTexture( 1, material.getTexture( 1 ) );
			Driver->setActiveTexture( 0, material.getTexture( 0 ) );
			Driver->setBasicRenderStates( material, lastMaterial, resetAllRenderstates );
		}
	};


	//! sphere map material renderer
	class COGLES2MaterialRenderer_SPHERE_MAP : public COGLES2MaterialRenderer
	{
	public:

		COGLES2MaterialRenderer_SPHERE_MAP( video::COGLES2Driver* d )
				: COGLES2MaterialRenderer( d ) {}

		virtual void OnSetMaterial( const SMaterial& material, const SMaterial& lastMaterial,
				bool resetAllRenderstates, IMaterialRendererServices* services )
		{
			if ( material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates )
			{
				FixedPipeline->setRenderMode( EMT_SPHERE_MAP );
			}
			FixedPipeline->setMaterial( material );
			Driver->disableTextures( 1 );
			Driver->setActiveTexture( 0, material.getTexture( 0 ) );
			Driver->setBasicRenderStates( material, lastMaterial, resetAllRenderstates );
		}

		virtual void OnUnsetMaterial()
		{
		}
	};


	//! reflection 2 layer material renderer
	class COGLES2MaterialRenderer_REFLECTION_2_LAYER : public COGLES2MaterialRenderer
	{
	public:

		COGLES2MaterialRenderer_REFLECTION_2_LAYER( video::COGLES2Driver* d )
				: COGLES2MaterialRenderer( d ) {}

		virtual void OnSetMaterial( const SMaterial& material, const SMaterial& lastMaterial,
				bool resetAllRenderstates, IMaterialRendererServices* services )
		{
			if ( material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates )
			{
				FixedPipeline->setRenderMode( EMT_REFLECTION_2_LAYER );
			}
			FixedPipeline->setMaterial( material );
			Driver->disableTextures( 2 );
			Driver->setActiveTexture( 0, material.getTexture( 1 ) );
			Driver->setActiveTexture( 1, material.getTexture( 0 ) );
			Driver->setBasicRenderStates( material, lastMaterial, resetAllRenderstates );
		}
	};


	//! reflection 2 layer material renderer
	class COGLES2MaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER : public COGLES2MaterialRenderer
	{
	public:

		COGLES2MaterialRenderer_TRANSPARENT_REFLECTION_2_LAYER( video::COGLES2Driver* d )
				: COGLES2MaterialRenderer( d ) {}

		virtual void OnSetMaterial( const SMaterial& material, const SMaterial& lastMaterial,
				bool resetAllRenderstates, IMaterialRendererServices* services )
		{
			Driver->disableTextures( 2 );
			Driver->setActiveTexture( 1, material.getTexture( 1 ) );
			Driver->setActiveTexture( 0, material.getTexture( 0 ) );
			Driver->setBasicRenderStates( material, lastMaterial, resetAllRenderstates );

			if ( material.MaterialType != lastMaterial.MaterialType || resetAllRenderstates )
			{
				Driver->blendFunc( EBF_ONE, EBF_ONE_MINUS_SRC_ALPHA );
				Driver->enableBlend();
			}
		}

		virtual void OnUnsetMaterial()
		{			
			Driver->disableBlend();
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
#endif

