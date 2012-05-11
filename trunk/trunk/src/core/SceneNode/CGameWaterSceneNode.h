#ifndef _CGAME_WATER_SCENENODE_
#define _CGAME_WATER_SCENENODE_

#include "stdafx.h"
#include "CGameObject.h"
#include "IView.h"

class CWaterGrassCallBack : public video::IShaderConstantSetCallBack
{
private:
	int normalMap;
	int reflectionmap;

public:
    CWaterGrassCallBack()
    {		
		normalMap = 0;
		reflectionmap = 1;
    }

    virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
    {
		IrrlichtDevice *device = getIView()->getDevice();

		float time = device->getTimer()->getTime()/1000.0f;

		services->setPixelShaderConstant("waterTime",	(float*)&time, 1);
		services->setPixelShaderConstant("NormalMap",	(float*)&normalMap, 1);
		services->setPixelShaderConstant("t1",			(float*)&reflectionmap, 1);
    }
};


class CGameWaterSceneNode: public ISceneNode
{
protected:
	IMesh* m_mesh;
public:
	CGameWaterSceneNode(CGameObject *owner, IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, 
		const core::vector3df& position = core::vector3df(0,0,0), 
		s32 id = -1);

    virtual ~CGameWaterSceneNode();

	// OnRegisterSceneNode
	// ISceneNode implement
	virtual void OnRegisterSceneNode();

	// render
	// ISceneNode implement
	virtual void render();

	// getBoundingBox
	// ISceneNode implement
	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	// getMaterialCount
	// ISceneNode implement
	virtual u32 getMaterialCount() const;

	// getMaterial
	// ISceneNode implement
	virtual video::SMaterial& getMaterial(u32 i);

};

#endif