// Copyright (C) 2009-2010 Amundis
// Heavily based on the OpenGL driver implemented by Nikolaus Gebhardt
// and OpenGL ES driver implemented by Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#include "COGLES2FixedPipelineShader.h"

#ifdef _IRR_COMPILE_WITH_OGLES2_

#include "COGLES2SLMaterialRenderer.h"
#include "COGLES2Utils.h"

namespace irr
{
namespace video
{
	const char* const COGLES2FixedPipelineShader::sBuiltInShaderUniformNames[] =
	{
		"uMvpMatrix",
		"uWorldMatrix",
		"uNormalize",
		"uEyePos",

		"uLightDirection1",
		"uLightDiffuse1",
		"uLightSpecular1",

		"uLightDirection2",
		"uLightDiffuse2",
		"uLightSpecular2",

		"uPointLightPosition1",
		"uPointLightDiffuse1",
		"uPointLightSpecular1",
		"uPointLightAttenuation1",

		"uPointLightPosition2",
		"uPointLightDiffuse2",
		"uPointLightSpecular2",
		"uPointLightAttenuation2",

		"uPointLightPosition3",
		"uPointLightDiffuse3",
		"uPointLightSpecular3",
		"uPointLightAttenuation3",

		"uAmbientColor",
		"uMaterialAmbient",
		"uMaterialEmission",
		"uMaterialDiffuse",
		"uMaterialSpecular",
		"uMaterialShininess",

		"uTextureMatrix",
		"uClipPlane",
		"uAlphaTest",
		"uAlphaValue",
		"uFogType",
		"uFogColor",
		"uFogStart",
		"uFogEnd",
		"uFogDensity",
		"uTextureUnit0",
		"uTextureUnit1",
		0
	};

	const c8 Solid_VertexShaderFile[]				= IRR_OGLES2_SHADER_PATH "COGLES2FixedPipeline_solid.vsh";
	const c8 Solid_FragmentShaderFile[]				= IRR_OGLES2_SHADER_PATH "COGLES2FixedPipeline_solid.fsh";

	const c8 SolidOneTexture_VertexShaderFile[]		= IRR_OGLES2_SHADER_PATH "COGLES2FixedPipeline_solidOneTexture.vsh";
	const c8 SolidOneTexture_FragmentShaderFile[]	= IRR_OGLES2_SHADER_PATH "COGLES2FixedPipeline_solidOneTexture.fsh";

	const c8 SolidLighting_VertexShaderFile[]		= IRR_OGLES2_SHADER_PATH "COGLES2FixedPipeline_solidLighting.vsh";
	const c8 SolidLighting_FragmentShaderFile[]		= IRR_OGLES2_SHADER_PATH "COGLES2FixedPipeline_solidLighting.fsh";

	COGLES2FixedPipelineShader::COGLES2FixedPipelineShader(video::COGLES2Driver *driver, io::IFileSystem* fs)
		:Driver(driver), Normalize(0), AlphaTest(0), AlphaValue(0.f), AlphaFunction(ALPHA_GREATER),
		Lighting(0), Fog(0), FogType(0), FogStart(0.f), FogEnd(0.f), FogDensity(0.f),
		ColorMaterial(0), MaterialShininess(0.f), RenderMode(EMT_SOLID)
	{
		s32 dummy = -1;
		for ( int i = 0; i < ES_COUNT; i++ )
			MaterialType[i] = NULL;

		// init solid shader
		MaterialType[ ES_SOLID ] = new COGLES2SLMaterialRenderer( Driver,
					fs,
					NULL,
					NULL,
					sBuiltInShaderUniformNames,
					UNIFORM_COUNT
				);
		MaterialType[ ES_SOLID ]->initFromFiles( dummy, Solid_VertexShaderFile, Solid_FragmentShaderFile, false );

		// init solid with one texture shader
		MaterialType[ ES_SOLID_ONETEXTURE ] = new COGLES2SLMaterialRenderer( Driver,
					fs,
					NULL,
					NULL,
					sBuiltInShaderUniformNames,
					UNIFORM_COUNT
				);
		MaterialType[ ES_SOLID_ONETEXTURE ]->initFromFiles( dummy, SolidOneTexture_VertexShaderFile, SolidOneTexture_FragmentShaderFile, false );



		// init solid shader with lighting
		MaterialType[ ES_SOLID_LIGHT ] = new COGLES2SLMaterialRenderer( Driver,
					fs,
					NULL,
					NULL,
					sBuiltInShaderUniformNames,
					UNIFORM_COUNT
				);
		MaterialType[ ES_SOLID_LIGHT ]->initFromFiles( dummy, SolidLighting_VertexShaderFile, SolidLighting_FragmentShaderFile, false );

		// init null data
		initData();
		
	};
	
	COGLES2FixedPipelineShader::~COGLES2FixedPipelineShader()
	{
		for ( int i = 0; i < ES_COUNT; i++ )			
		{
			if ( MaterialType[i] != NULL )
				delete MaterialType[i];
		}
	}

	void COGLES2FixedPipelineShader::initData()
	{
		for (u32 i = 0; i < MATERIAL_MAX_TEXTURES; ++i)
			TextureUnits[i] = i;
				
		zeroLightData();

		memset(FogColor, 0, sizeof(FogColor));
		memset(&ClipPlane, 0, sizeof(ClipPlane));

		memset(&MaterialAmbient,	0, sizeof(MaterialAmbient));
		memset(&MaterialEmission,	0, sizeof(MaterialEmission));
		memset(&MaterialDiffuse,	0, sizeof(MaterialDiffuse));
		memset(&MaterialSpecular,	0, sizeof(MaterialSpecular));
	}

	void COGLES2FixedPipelineShader::zeroLightData()
	{
		memset(LightDirection,	0, sizeof(LightDirection));		
		memset(LightAmbient,	0, sizeof(LightAmbient));		
		memset(LightDiffuse,	0, sizeof(LightDiffuse));		
		memset(LightSpecular,	0, sizeof(LightSpecular));
		
		memset(PointLightPosition,		0, sizeof(PointLightPosition));
		memset(PointLightAmbient,		0, sizeof(PointLightAmbient));
		memset(PointLightDiffuse,		0, sizeof(PointLightDiffuse));
		memset(PointLightSpecular,		0, sizeof(PointLightSpecular));
		memset(PointLightAttenuation,	0, sizeof(PointLightAttenuation));
	}

	bool COGLES2FixedPipelineShader::OnRender(IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype)
	{
		bool statusOk = true;		

		COGLES2SLMaterialRenderer *mat = NULL;
				
		Lighting	= Material.Lighting;
		Fog			= Material.FogEnable;
		Normalize	= Material.NormalizeNormals;
		
		int nTexture = 0;
		for (u32 i = 0; i < MATERIAL_MAX_TEXTURES; ++i)
		{
			UseTexture[i] = Material.getTexture(i) != 0;
			if (UseTexture[i])
			{
				nTexture++;

				//UseTexMatrix[i] = false;
				//const core::matrix4& texMat = Material.getTextureMatrix(i);
				//if (!texMat.isIdentity())
				//{
				//	UseTexMatrix[i] = true;
				//	memcpy(&TextureMatrix[i], texMat.pointer(), sizeof(mat4));
				//}
			}
		}		

		if ( RenderMode == EMT_SOLID )
		{
			if ( Lighting == false )
			{				
				if ( nTexture == 0 )
					mat = MaterialType[ ES_SOLID ];
				else
					mat = MaterialType[ ES_SOLID_ONETEXTURE ];
			}
			else
			{
				mat = MaterialType[ ES_SOLID_LIGHT ];
			}

		}
		else
		{			
			if ( nTexture == 0 )
			{
				// primitive (line, rect)
				mat = MaterialType[ ES_SOLID ];
			}
			else
			{
				// transparent (alpha, add)
				mat = MaterialType[ ES_SOLID_ONETEXTURE ];
			}
		}
		
		
		// use program
		mat->useProgram();

		/* Matrices Upload */		
		core::matrix4 worldViewProj = Driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= Driver->getTransform(video::ETS_VIEW);
		worldViewProj *= Driver->getTransform(ETS_WORLD);
		mat->setUniform(MVP_MATRIX, worldViewProj.pointer());

		//core::matrix4 world = Driver->getTransform(ETS_WORLD);
		//mat->setUniform(WORLD_MATRIX, world.pointer());

		/* Textures Upload */
        if ( nTexture > 0 )
        {
            mat->setUniform(TEXTURE_UNIT0, &TextureUnits[0]);
            //mat->setUniform(TEXTURE_UNIT1, &TextureUnits[1]);
        }
						
		if ( Lighting )
		{
			// set material color		
			if (MaterialAmbient != Material.AmbientColor)
			{
				MaterialAmbient   = Material.AmbientColor;
				mat->setUniform(MATERIAL_AMBIENT, &MaterialAmbient);
			}
			if (MaterialEmission != Material.EmissiveColor)
			{
				MaterialEmission  = Material.EmissiveColor;
				mat->setUniform(MATERIAL_EMISSION, &MaterialEmission);
			}
			if (MaterialDiffuse != Material.DiffuseColor)
			{
				MaterialDiffuse   = Material.DiffuseColor;
				mat->setUniform(MATERIAL_DIFFUSE, &MaterialDiffuse);
			}
			if (MaterialSpecular != Material.SpecularColor)
			{
				MaterialSpecular  = Material.SpecularColor;
				mat->setUniform(MATERIAL_SPECULAR, &MaterialSpecular);
			}
			if (MaterialShininess != Material.Shininess)
			{
				MaterialShininess = Material.Shininess;
				mat->setUniform(MATERIAL_SHININESS, &MaterialShininess);
			}


			// get light on scene
			u32 cnt = Driver->getDynamicLightCount();
			
			core::matrix4 invWorld;
			Driver->getTransform(ETS_WORLD).getInverse(invWorld);

			// support
			const int kDirectionLight	= MAX_LIGHTS;
			const int kPointLight		= MAX_POINTLIGHTS;

			int nLightDirection = 0;
			int nLightPoint		= 0;

			zeroLightData();

			for ( size_t i = 0; i < cnt; ++i )
			{
				if ( nLightDirection < kDirectionLight || nLightPoint < kPointLight )
				{
					if ( Driver->isLightTurnOn(i) == false )
						continue;

					video::SLight light;
					light = Driver->getDynamicLight( i );					

					switch ( light.Type )
					{
						case ELT_DIRECTIONAL:
							if ( nLightDirection < kDirectionLight )
							{
								invWorld.rotateVect(( f32* )&LightDirection[nLightDirection],light.Direction );
								LightAmbient[nLightDirection]		= light.AmbientColor;
								LightDiffuse[nLightDirection]		= light.DiffuseColor;
								LightSpecular[nLightDirection]		= light.SpecularColor;

								nLightDirection++;
							}
							break;				
						case ELT_POINT:
							if ( nLightPoint < kPointLight )
							{
								invWorld.transformVect(( f32* )&PointLightPosition[nLightPoint],light.Position );
								PointLightAttenuation[nLightPoint]	= light.Attenuation;
								PointLightAmbient[nLightPoint]		= light.AmbientColor;
								PointLightDiffuse[nLightPoint]		= light.DiffuseColor;
								PointLightSpecular[nLightPoint]		= light.SpecularColor;
								PointLightAttenuation[nLightPoint]	= light.Attenuation;

								nLightPoint++;
							}
							break;
						default:
							break;
					}				
				}
				else
					break;
			}
			
			mat->setUniform( LIGHT_DIRECTION1,		&LightDirection[0]);
			mat->setUniform( LIGHT_DIFFUSE1,		&LightDiffuse[0]);
			mat->setUniform( LIGHT_SPECULAR1,		&LightSpecular[0]);

			mat->setUniform( LIGHT_DIRECTION2,		&LightDirection[1]);
			mat->setUniform( LIGHT_DIFFUSE2,		&LightDiffuse[1]);
			mat->setUniform( LIGHT_SPECULAR2,		&LightSpecular[1]);

			mat->setUniform( POINTLIGHT_POSITION1,		&PointLightPosition[0]);
			mat->setUniform( POINTLIGHT_DIFFUSE1,		&PointLightDiffuse[0]);
			mat->setUniform( POINTLIGHT_SPECULAR1,		&PointLightSpecular[0]);
			mat->setUniform( POINTLIGHT_ATTENUATION1,	&PointLightAttenuation[0]);

			mat->setUniform( POINTLIGHT_POSITION2,		&PointLightPosition[1]);
			mat->setUniform( POINTLIGHT_DIFFUSE2,		&PointLightDiffuse[1]);
			mat->setUniform( POINTLIGHT_SPECULAR2,		&PointLightSpecular[1]);
			mat->setUniform( POINTLIGHT_ATTENUATION2,	&PointLightAttenuation[1]);

			mat->setUniform( POINTLIGHT_POSITION3,		&PointLightPosition[2]);
			mat->setUniform( POINTLIGHT_DIFFUSE3,		&PointLightDiffuse[2]);
			mat->setUniform( POINTLIGHT_SPECULAR3,		&PointLightSpecular[2]);
			mat->setUniform( POINTLIGHT_ATTENUATION3,	&PointLightAttenuation[2]);

			AmbientColor = Driver->getAmbientLight();
			mat->setUniform( AMBIENT_COLOR, &AmbientColor );
		}

		return statusOk ;
	};

	void COGLES2FixedPipelineShader::setMaterial(const SMaterial &material)
	{
		Material = material;	
	}

}
}

#endif //_IRR_COMPILE_WITH_OGLES2_
