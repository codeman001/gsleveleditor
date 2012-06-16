#ifndef _GRASSSCENENODE_
#define _GRASSSCENENODE_

#include "stdafx.h"
#include "CGameObject.h"


class CShaderGrassCallBack : public video::IShaderConstantSetCallBack
{
private:
	int textureID;

public:
    CShaderGrassCallBack()
    {
		// default use TEXTURE0
		textureID = 0;
    }

    virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
    {
		video::IVideoDriver* driver = services->getVideoDriver();
		
		core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		services->setVertexShaderConstant("uPrjMatrix", worldViewProj.pointer(), 16);
		
		worldViewProj = driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("uMvpMatrix", worldViewProj.pointer(), 16);
		
		services->setPixelShaderConstant("grassTexture", (float*)&textureID, 1);
    }
};


class CGameGrassSceneNode: public ISceneNode
{
public:
	CGameGrassSceneNode(CGameObject *owner, IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, 
		const core::vector3df& position = core::vector3df(0,0,0), 
		s32 id = -1);

    virtual ~CGameGrassSceneNode();

	// OnRegisterSceneNode
	// irr scenenode impl
    virtual void OnRegisterSceneNode();

	// render
	// irr scenenode impl
	virtual void render();

	// getBoundingBox
	// irr scenenode impl
	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	// getMaterialCount
	// irr scenenode impl
	virtual u32 getMaterialCount() const;

	// getMaterial
	// irr scenenode impl
	virtual video::SMaterial& getMaterial(u32 i);

protected:
	CGameObject		*m_owner;
	int				m_idGrass;
	IMesh			*m_mesh;
    SMaterial		m_Material;
    f32				m_MaxDistance;
	core::vector3df	m_originalPos;
};

#endif