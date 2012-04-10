#include "stdafx.h"
#include "CGameContainerSceneNode.h"

CGameContainerSceneNode::CGameContainerSceneNode(
			CGameObject *owner,
			ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position,
			const core::vector3df& rotation,
			const core::vector3df& scale
			)
	:ISceneNode( parent, mgr, id, position, rotation, scale )
{
	m_owner = owner;
}

CGameContainerSceneNode::~CGameContainerSceneNode()
{
}

//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CGameContainerSceneNode::getBoundingBox() const
{
	return Box;
}

//! This method is called just before the rendering process of the whole scene.
void CGameContainerSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);

		// only register child if container is visible
		ISceneNode::OnRegisterSceneNode();
	}
}

//! does nothing.
void CGameContainerSceneNode::render()
{
}
