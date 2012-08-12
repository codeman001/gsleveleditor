#ifndef _CGAMEDEBUGSCENENODE_
#define _CGAMEDEBUGSCENENODE_


#include "ISceneNode.h"
#include "matrix4.h"

class CGameDebugSceneNode: public ISceneNode
{
protected:
	core::aabbox3d<f32> Box;	
	SMaterial Material;
public:
	CGameDebugSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);
	virtual ~CGameDebugSceneNode();

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