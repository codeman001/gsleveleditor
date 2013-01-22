#ifndef _CSTATIC_SHADOW_SCENENODE_
#define _CSTATIC_SHADOW_SCENENODE_

#include "stdafx.h"
#include "CGameObject.h"
#include "IView.h"

class CGameStaticShadowSceneNode: public ISceneNode
{
protected:
	CGameObject *m_owner;

	SMeshBuffer* Buffer;
public:
	CGameStaticShadowSceneNode(CGameObject *owner, ISceneNode* parent, ISceneManager* mgr, 
		const core::vector3df& position = core::vector3df(0,0,0), 
		s32 id = -1);

    virtual ~CGameStaticShadowSceneNode();

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

protected:

	// reallocateBuffers
	// calc buffer size
	void reallocateBuffers(int numShadow);
};

#endif