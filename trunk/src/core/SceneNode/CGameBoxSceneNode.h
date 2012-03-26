#ifndef _BOXSCENE_NODE_
#define _BOXSCENE_NODE_

#include "CCubeSceneNode.h"

class CGameObject;

class CGameBoxSceneNode: public CCubeSceneNode
{
protected:
	CGameObject *m_owner;

public:
	CGameBoxSceneNode(
			CGameObject *owner,
			f32 size, ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)
			);
	virtual ~CGameBoxSceneNode();

	// render
	// override render state
	virtual void render();
};

#endif