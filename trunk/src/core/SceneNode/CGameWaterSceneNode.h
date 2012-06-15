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
		IVideoDriver *driver = device->getVideoDriver();

		float time = device->getTimer()->getTime()/1000.0f;
		float size = 1.0f;
		float speed = 1.0f;
		float transparent = 0.8f;
		float brightness = 3.0f;

		core::matrix4 worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(ETS_WORLD);
		
		// set uniform mvp matrix
		services->setPixelShaderConstant("MvpMatrix",		(float*)worldViewProj.pointer(), 16);

		// set uniform
		services->setPixelShaderConstant("WaterTime",		(float*)&time, 1);
		services->setPixelShaderConstant("WaveSize",		(float*)&size, 1);
		services->setPixelShaderConstant("WaveSpeed",		(float*)&speed, 1);		
		services->setPixelShaderConstant("WaterTransparent",(float*)&transparent, 1);
		services->setPixelShaderConstant("WaterBrightness",	(float*)&brightness, 1);
		
		services->setPixelShaderConstant("NormalMap",	(float*)&normalMap, 1);
		services->setPixelShaderConstant("WaterMap",	(float*)&reflectionmap, 1);
    }
};


class CGameWaterSceneNode: public ISceneNode
{
protected:
	IMesh* m_mesh;
	CGameObject *m_owner;
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