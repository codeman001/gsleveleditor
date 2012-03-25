#ifndef _GAMESNAKESCENENODE_
#define _GAMESNAKESCENENODE_

#include "stdafx.h"
#include "CSnakeMesh.h"

class CGameSnakeSceneNode: public ISceneNode
{
protected:	
	CSnakeMesh			m_snakeMesh;
	ITexture			*m_snakeTexture;
public:
	CGameSnakeSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);
	virtual ~CGameSnakeSceneNode();

	virtual void OnRegisterSceneNode();

	virtual void OnAnimate(irr::u32 timeMs);

	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return m_snakeMesh.getBoundingBox();
	}

	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual video::SMaterial& getMaterial(u32 i)
	{
		return m_snakeMesh.getMeshBuffer(0)->getMaterial();
	}

	inline CSnakeMesh*	getSnakeMesh()
	{
		return &m_snakeMesh;
	}

	virtual void updateMesh( list<core::vector3df>* listBone );
};	

#endif