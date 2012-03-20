#ifndef _CGAME_MESH_SCENE_
#define _CGAME_MESH_SCENE_

#include "CMeshSceneNode.h"

class CGameObject;

class CGameMeshSceneNode: public CMeshSceneNode
{
protected:
	CGameObject *m_owner;
public:
	CGameMeshSceneNode(
				CGameObject *owner,
				IMesh* mesh, ISceneNode* parent, ISceneManager* mgr,
				const core::vector3df& position = core::vector3df(0,0,0),
				const core::vector3df& rotation = core::vector3df(0,0,0),
				const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)
			);
	virtual ~CGameMeshSceneNode();

	// render
	// override render state
	virtual void render();
};


#endif