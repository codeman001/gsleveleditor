
// gameswf_irrlicht_material_renderer_wii.hpp
// irrlicht material to support flash blending modes

void draw2DVertexPrimitiveList(irr::video::IVideoDriver* videoDriver, SS3DVertexComponentArrays& components, int vertexCount, const u16 *pIndexList, u32 primitiveCount, irr::scene::E_PRIMITIVE_TYPE pType)
{

	// Flush+Sync all buffers
	DCFlushRangeNoSync(components.Position, vertexCount * components.PositionStride);
	DCFlushRangeNoSync(components.Color0, vertexCount * components.Color0Stride);
	DCFlushRangeNoSync(components.TexCoord[0].Coord, vertexCount * components.TexCoord[0].Stride);
	PPCSync();

	// zbuffer
	GXSetZMode(s_material.getFlag(EMF_ZBUFFER), s_material.getZBufferFunc() == ECFN_EQUAL ? GX_EQUAL : GX_LEQUAL, s_material.getFlag(EMF_ZWRITE_ENABLE));
	
	// blending
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

	GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
	GXSetTevSwapModeTable(GX_TEV_SWAP0,GX_CH_RED,GX_CH_GREEN,GX_CH_BLUE,GX_CH_ALPHA);

	// culling
	GXSetCullMode(GX_CULL_NONE);

	if(s_material.getTexture(0))
	{
		GXSetChanCtrl(GX_COLOR0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_CLAMP, GX_AF_NONE);
		GXSetChanCtrl(GX_ALPHA0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
		GXSetNumTevStages(1);
		GXSetNumTexGens(1);
		GXSetNumChans(0);

		irr::video::SColor color = components.Color0[0];
		GXColor trans_fx = { color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha() };
		//GXColor trans_fx = { 0xff, 0xff, 0xff, 0xff };
		GXSetTevColor(GX_TEVREG0, trans_fx);

		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
		GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);

		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_DISABLE, GX_TEVPREV);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);

		static_cast<irr::video::CWiiTexture*>(s_material.getTexture(0))->bind( 0 );
     
		GXInvalidateTexAll();

		// set up vertex descriptors
		GXClearVtxDesc();
		GXSetVtxDesc( GX_VA_POS,  GX_DIRECT );
		GXSetVtxDesc( GX_VA_TEX0,  GX_DIRECT );
	    
		GXSetVtxAttrFmt( GX_VTXFMT0, GX_VA_POS,  GX_POS_XY, GX_F32, 0 );
		GXSetVtxAttrFmt( GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST,  GX_F32, 0 );

		int nverts = 0;
		if(pType == irr::scene::EPT_TRIANGLE_STRIP)
		{
			nverts = primitiveCount + 2;
			GXBegin( GX_TRIANGLESTRIP, GX_VTXFMT0, nverts ) ;
		}
		else
		{
			nverts = primitiveCount * 3;
			GXBegin( GX_TRIANGLES, GX_VTXFMT0, nverts ) ;
		}
		
		for(int i = 0; i < nverts; i++)
		{
			irr::core::vector3df& pos = components.Position[pIndexList[i]];
			GXPosition2f32( pos.X, pos.Y );
			irr::core::vector2df& tcoords = components.TexCoord[0].Coord[pIndexList[i]];
			GXTexCoord2f32( tcoords.X, tcoords.Y );
		}
		GXEnd();
	}
	else
	{
		// setup TEV for vertex color only
		GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
		GXSetNumTevStages(1);
		GXSetNumChans(1);
		GXSetNumTexGens(0);				
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0 );
		GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

		// set up vertex descriptors
		GXClearVtxDesc();
		GXSetVtxDesc( GX_VA_POS,  GX_DIRECT );
		GXSetVtxDesc( GX_VA_CLR0,  GX_DIRECT );
	    
		GXSetVtxAttrFmt( GX_VTXFMT0, GX_VA_POS,  GX_POS_XY, GX_F32, 0 );
		GXSetVtxAttrFmt( GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA,  GX_RGBA8, 0 );

		int nverts = 0;
		if(pType == irr::scene::EPT_TRIANGLE_STRIP)
		{
			nverts = primitiveCount + 2;
			GXBegin( GX_TRIANGLESTRIP, GX_VTXFMT0, nverts ) ;
		}
		else
		{
			nverts = primitiveCount * 3;
			GXBegin( GX_TRIANGLES, GX_VTXFMT0, nverts ) ;
		}
		
		for(int i = 0; i < nverts; i++)
		{
			irr::core::vector3df& pos = components.Position[pIndexList[i]];
			GXPosition2f32( pos.X, pos.Y ) ;	  
			irr::video::SColor& color = components.Color0[pIndexList[i]];
			//GXColor1u32(color.color);
			GXColor4u8(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
		}
		GXEnd();
	}

	//GXDrawDone();
}
