#ifndef _CGAME_ANI_MESH_SCENE_
#define _CGAME_ANI_MESH_SCENE_

#include "CAnimatedMeshSceneNode.h"

class CGameObject;

class CGameAnimatedMeshSceneNode: public CAnimatedMeshSceneNode
{
protected:
	CGameObject *m_owner;
public:
	CGameAnimatedMeshSceneNode(
				CGameObject *owner,
				IAnimatedMesh* mesh, ISceneNode* parent, ISceneManager* mgr,
				const core::vector3df& position = core::vector3df(0,0,0),
				const core::vector3df& rotation = core::vector3df(0,0,0),
				const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)
			);
	virtual ~CGameAnimatedMeshSceneNode();

	// render
	// override render state
	virtual void render();
};

#endif