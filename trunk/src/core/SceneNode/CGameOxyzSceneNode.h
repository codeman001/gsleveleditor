#ifndef _CGAMEOXYZSCENENODE_
#define _CGAMEOXYZSCENENODE_


#include "ISceneNode.h"

#include "matrix4.h"

class CGameOxyzSceneNode: public ISceneNode
{
protected:
	core::aabbox3d<f32> Box;
	
	//S3DVertex Vertices[4];
	SMaterial Material;
public:
	CGameOxyzSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);
	virtual ~CGameOxyzSceneNode();

	virtual void OnRegisterSceneNode();

	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return Box;
	}

	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual video::SMaterial& getMaterial(u32 i)
	{
		return Material;
	}
};

#endif