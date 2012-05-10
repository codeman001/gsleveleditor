#ifndef _CGAME_WATER_SCENENODE_
#define _CGAME_WATER_SCENENODE_

#include "stdafx.h"
#include "CGameObject.h"

class CGameWaterSceneNode: public ISceneNode
{
public:
	CGameWaterSceneNode(CGameObject *owner, IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, 
		const core::vector3df& position = core::vector3df(0,0,0), 
		s32 id = -1);

    virtual ~CGameWaterSceneNode();
};

#endif