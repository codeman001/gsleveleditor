#ifndef GAMEBILLBOARD_SCENENODE_
#define GAMEBILLBOARD_SCENENODE_

#include "stdafx.h"
#include "CBillboardSceneNode.h"

class CGameObject;

class CGameBillboardSceneNode: public CBillboardSceneNode
{
protected:
	CGameObject *m_owner;
public:
	
	CGameBillboardSceneNode( CGameObject *owner,
		ISceneNode* parent, ISceneManager* mgr, s32 id,	
		const core::vector3df& position, const core::dimension2d<f32>& size,
		video::SColor colorTop=video::SColor(0xFFFFFFFF),video::SColor colorBottom=video::SColor(0xFFFFFFFF));

	virtual ~CGameBillboardSceneNode();

	// render
	// override render state
	virtual void render();
};

#endif