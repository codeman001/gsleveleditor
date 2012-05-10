#include "stdafx.h"
#include "CGameWaterSceneNode.h"

CGameWaterSceneNode::CGameWaterSceneNode(CGameObject *owner, IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, 
		const core::vector3df& position,
		s32 id)
	:ISceneNode( parent, mgr, id, position, core::vector3df(0,0,0) )
{

}

CGameWaterSceneNode::~CGameWaterSceneNode()
{

}