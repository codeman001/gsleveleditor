
// gameswf_irrlicht_material_renderer_gl.hpp
// irrlicht material to support flash blending modes

// http://en.wikipedia.org/wiki/Blend_modes
// http://www.nathanm.com/photoshop-blending-math/
// http://docs.kde.org/stable/en/koffice/krita/layers-composite.html

class MaterialRenderer_GameSWF : public PLATFORM_MATERIAL_RENDERER
{
public:

	MaterialRenderer_GameSWF(PLATFORM_DRIVER* d)
		: PLATFORM_MATERIAL_RENDERER(d)
	{
	}

	virtual void onSetMaterial(const SMaterial& material, const SMaterial& lastMaterial,
		bool resetAllRenderstates, IMaterialRendererServices* services)
	{
		Driver->disableTextures(2);
		Driver->setTexture(0, material.getTexture(0));
		Driver->setTexture(1, material.getTexture(0));
//		Driver->setBasicRenderStates(material, lastMaterial, resetAllRenderstates);

		GLenum srcFact;
		GLenum dstFact;
		GLfloat modulate = 1.0f;
		int blendmode = (int)material.getMaterialTypeParam(0);
		switch(blendmode)
		{
		case gameswf::blend_mode::MULTIPLY:
			
#if !defined(GLITCH_COMPILE_WITH_OPENGLES_2) && !defined(GLITCH_COMPILE_WITH_OPENGL_NO_FIXED)
			Driver->setTexEnvMode(GL_MODULATE);
#endif
			srcFact = GL_DST_COLOR;
			dstFact = GL_ONE_MINUS_SRC_ALPHA;

#if !defined(GLITCH_COMPILE_WITH_OPENGLES_2) && !defined(GLITCH_COMPILE_WITH_OPENGL_NO_FIXED) && !defined(GLITCH_COMPILE_WITH_PS3)
			if (SupportsMultiTexture())
			{
				SetActiveTexture(1);
				Driver->setTexEnvMode(GL_COMBINE);
				
				Driver->setCombineAlpha(GL_REPLACE);
				Driver->setSourceAlpha(0, GL_PREVIOUS);
				//Driver->setSourceAlpha(1, GL_PREVIOUS);
				Driver->setOperandAlpha(0, GL_SRC_ALPHA);
				
				Driver->setCombineRGB(GL_MODULATE);
				Driver->setSourceRGB(0, GL_PREVIOUS);
				Driver->setSourceRGB(1, GL_PREVIOUS);
				Driver->setOperandRGB(0, GL_SRC_ALPHA);
				Driver->setOperandRGB(1, GL_SRC_COLOR);
			}
#endif
			break;

		case gameswf::blend_mode::SCREEN:
#if !defined(GLITCH_COMPILE_WITH_OPENGLES_2) && !defined(GLITCH_COMPILE_WITH_OPENGL_NO_FIXED)
			Driver->setTexEnvMode(GL_MODULATE);
#endif
			srcFact = GL_ONE;
			dstFact = GL_ONE_MINUS_SRC_COLOR;

#if !defined(GLITCH_COMPILE_WITH_OPENGLES_2) && !defined(GLITCH_COMPILE_WITH_OPENGL_NO_FIXED) && !defined(GLITCH_COMPILE_WITH_PS3)
			if (SupportsMultiTexture())
			{
				SetActiveTexture(1);
				Driver->setTexEnvMode(GL_COMBINE);
				
				Driver->setCombineAlpha(GL_REPLACE);
				Driver->setSourceAlpha(0, GL_PREVIOUS);
				//Driver->setSourceAlpha(1, GL_PREVIOUS);
				Driver->setOperandAlpha(0, GL_SRC_ALPHA);
				
				Driver->setCombineRGB(GL_MODULATE);
				Driver->setSourceRGB(0, GL_PREVIOUS);
				Driver->setSourceRGB(1, GL_PREVIOUS);
				Driver->setOperandRGB(0, GL_SRC_ALPHA);
				Driver->setOperandRGB(1, GL_SRC_COLOR);
			}
#endif
			break;
		
		case gameswf::blend_mode::OVERLAY:
#if !defined(GLITCH_COMPILE_WITH_OPENGLES_2) && !defined(GLITCH_COMPILE_WITH_OPENGL_NO_FIXED)
			Driver->setTexEnvMode(GL_MODULATE);
#endif
			srcFact = GL_DST_COLOR;
			dstFact = GL_ONE;

#if !defined(GLITCH_COMPILE_WITH_OPENGLES_2) && !defined(GLITCH_COMPILE_WITH_OPENGL_NO_FIXED) && !defined(GLITCH_COMPILE_WITH_PS3)
			if (SupportsMultiTexture())
			{
				SetActiveTexture(1);
				Driver->setTexEnvMode(GL_COMBINE);
				
				Driver->setCombineAlpha(GL_REPLACE);
				Driver->setSourceAlpha(0, GL_PREVIOUS);
				//Driver->setSourceAlpha(1, GL_PREVIOUS);
				Driver->setOperandAlpha(0, GL_SRC_ALPHA);
				
				Driver->setCombineRGB(GL_MODULATE);
				Driver->setSourceRGB(0, GL_PREVIOUS);
				Driver->setSourceRGB(1, GL_PREVIOUS);
				Driver->setOperandRGB(0, GL_SRC_ALPHA);
				Driver->setOperandRGB(1, GL_SRC_COLOR);
			}
#endif
			break;

		default:
			break;
		}

#if !defined(GLITCH_COMPILE_WITH_OPENGLES_2) && !defined(GLITCH_COMPILE_WITH_OPENGL_NO_FIXED)
		Driver->setRGBScale(modulate);
#endif

		glBlendFunc(srcFact, dstFact);
		glEnable(GL_BLEND);
	}

	inline bool SupportsMultiTexture()
	{
#ifdef GLITCH_COMPILE_WITH_OPENGL_ES
		return true;
#else
		return Driver->queryFeature(EVDF_MULTITEXTURE);
#endif
	}

	inline void SetActiveTexture(unsigned int index)
	{
		Driver->setActiveTexture(GL_TEXTURE0 + index);
	}

	virtual void OnUnsetMaterial()
	{
		
#if !defined(GLITCH_COMPILE_WITH_OPENGLES_2) && !defined(GLITCH_COMPILE_WITH_OPENGL_NO_FIXED)
		if (Driver->queryFeature(EVDF_MULTITEXTURE))
		{
			SetActiveTexture(1);
			Driver->setTexEnvMode(GL_MODULATE);
			Driver->setOperandRGB(2, GL_SRC_COLOR);
			SetActiveTexture(0);
		}

		Driver->setTexEnvMode(GL_MODULATE);
		Driver->setRGBScale(1.f);
#	if !defined(GLITCH_COMPILE_WITH_PS3)
		Driver->setSourceRGB(1, GL_PREVIOUS);
#	endif
#endif

		glDisable(GL_BLEND);
	}

	virtual glitch::u32 getVertexAttributeMask() const
	{
		return glitch::video::EVAB_POSITION | glitch::video::EVAB_COLOR0 | glitch::video::EVAB_TEXCOORD0;
	}

private:
	unsigned int getGLBlend ( E_BLEND_FACTOR factor ) const
	{
		unsigned int r = 0;
		switch ( factor )
		{
			case EBF_ZERO:			r = GL_ZERO; break;
			case EBF_ONE: default:	r = GL_ONE; break;
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

	unsigned int getTexelAlpha ( E_BLEND_FACTOR factor ) const
	{
		unsigned int r;
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
