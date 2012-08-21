// Copyright (C) 2009-2010 Amundis
// Edit & modify Pham Hong Duc (hongduc.pr@gmail.com)
// Heavily based on the OpenGL driver implemented by Nikolaus Gebhardt
// and OpenGL ES driver implemented by Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef __C_OGLES2_FIXED_PIPELINE_SHADER_H_INCLUDED__
#define __C_OGLES2_FIXED_PIPELINE_SHADER_H_INCLUDED__

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_OGLES2_

#include "COGLES2Driver.h"
#include "COGLES2SLMaterialRenderer.h"
#include "EVertexAttributes.h"

namespace irr
{
namespace video
{
	#define MAX_LIGHTS		2
	#define MAX_POINTLIGHTS	4

	enum AlphaFunc
	{
		ALPHA_GREATER = 0
	};
	
	class COGLES2Driver;

	class COGLES2FixedPipelineShader
	{
	protected:
		enum E_SHADER
		{
			ES_SOLID = 0,
			ES_SOLID_ONETEXTURE,
			ES_SOLID_LIGHT,
			ES_COUNT
		};

		COGLES2SLMaterialRenderer	*MaterialType[ ES_COUNT ];
		COGLES2Driver				*Driver;		
	public:

		COGLES2FixedPipelineShader(video::COGLES2Driver* driver, io::IFileSystem* fs);
		~COGLES2FixedPipelineShader();

		void updateMatrices();
		void bindTexture();
		virtual bool OnRender(IMaterialRendererServices* service, E_VERTEX_TYPE vtxtype);

	public:		

		void enableAlphaTest()
		{
			AlphaTest = true;
		}
		void disableAlphaTest()
		{
			AlphaTest = false;
		}
		void setAlphaValue(float value)
		{
			AlphaValue = value;
		}

		void setAmbientLight(const SColorf& color);

		void enableFog()
		{
			Fog = true;
		}
		void disableFog()
		{
			Fog = false;
		}
		bool isFogEnabled()
		{
			return (Fog == 1);
		}
		void setFog(E_FOG_TYPE type, const SColor& color, f32 start, f32 end, f32 density);

		void setMaterial(const SMaterial& material);

		void setRenderMode(E_MATERIAL_TYPE mode)
		{
			RenderMode = mode;
		}

	private:

		void initData();
		void zeroLightData();

	private:

		struct vec3
		{
			float data[3];
		};
		struct vec4
		{
			float data[4];
		};
		struct mat4
		{
			float data[16];
		};

		float Normalize;

		bool	UseTexture[MATERIAL_MAX_TEXTURES];
		bool	UseTexMatrix[MATERIAL_MAX_TEXTURES];
		int		TextureUnits[MATERIAL_MAX_TEXTURES];
		mat4	TextureMatrix[MATERIAL_MAX_TEXTURES];

		float AlphaTest;
		float AlphaValue;
		AlphaFunc AlphaFunction;

		int				Lighting;
		
		vec4			LightDirection		[MAX_LIGHTS];
		SColorf			LightAmbient		[MAX_LIGHTS];
		SColorf			LightDiffuse		[MAX_LIGHTS];
		SColorf			LightSpecular		[MAX_LIGHTS];
		
		vec4			PointLightPosition	[MAX_POINTLIGHTS];
		SColorf			PointLightAmbient	[MAX_POINTLIGHTS];
		SColorf			PointLightDiffuse	[MAX_POINTLIGHTS];
		SColorf			PointLightSpecular	[MAX_POINTLIGHTS];
		core::vector3df PointLightAttenuation[MAX_POINTLIGHTS];
		SColorf			AmbientColor;

		int Fog;
		int FogType;
		float FogColor[4];
		float FogStart;
		float FogEnd;
		float FogDensity;

		int Clip;
		core::plane3df ClipPlane;

		u32 ColorMaterial;
		SColorf MaterialAmbient;
		SColorf MaterialEmission;
		SColorf MaterialDiffuse;
		SColorf MaterialSpecular;
		float MaterialShininess;
		SMaterial	Material;

		E_MATERIAL_TYPE RenderMode;
	private :
		enum SHADER_UNIFORM
		{
			MVP_MATRIX = 0,
			WORLD_MATRIX,
			NORMALIZE,
			EYE_POSITION,

			LIGHT_DIRECTION,
			LIGHT_AMBIENT,
			LIGHT_DIFFUSE,
			LIGHT_SPECULAR,

			POINTLIGHT_POSITION,
			POINTLIGHT_AMBIENT,
			POINTLIGHT_DIFFUSE,
			POINTLIGHT_SPECULAR,
			POINTLIGHT_ATTENUATION,

			LIGHT_ARRAY_DIRECTION,
			LIGHT_ARRAY_AMBIENT,
			LIGHT_ARRAY_DIFFUSE,
			LIGHT_ARRAY_SPECULAR,

			POINTLIGHT_ARRAY_POSITION,
			POINTLIGHT_ARRAY_AMBIENT,
			POINTLIGHT_ARRAY_DIFFUSE,
			POINTLIGHT_ARRAY_SPECULAR,
			POINTLIGHT_ARRAY_ATTENUATION,

			NUMLIGHT,
			NUMPOINTLIGHT,

			AMBIENT_COLOR,
			MATERIAL_AMBIENT,
			MATERIAL_EMISSION,
			MATERIAL_DIFFUSE,
			MATERIAL_SPECULAR,
			MATERIAL_SHININESS,
			COLOR_MATERIAL,

			TEXTURE_MATRIX,
			CLIP_PLANE,
			ALPHA_TEST,
			ALPHA_VALUE,
			FOG_TYPE,
			FOG_COLOR,
			FOG_START,
			FOG_END,
			FOG_DENSITY,
			TEXTURE_UNIT0,
			TEXTURE_UNIT1,
			UNIFORM_COUNT
		};
		static const char* const sBuiltInShaderUniformNames[];
	};
}
}

#endif //_IRR_COMPILE_WITH_OGLES2_

#endif //__C_OGLES2_FIXED_PIPELINE_SHADER_H_INCLUDED__
