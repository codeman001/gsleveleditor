#include "stdafx.h"
#include "CGameAnimatedMeshSceneNode.h"
#include "CGameObject.h"

CGameAnimatedMeshSceneNode::CGameAnimatedMeshSceneNode
	(
		CGameObject *owner,
		IAnimatedMesh* mesh, ISceneNode* parent, ISceneManager* mgr,
		const core::vector3df& position,
		const core::vector3df& rotation,
		const core::vector3df& scale
	):CAnimatedMeshSceneNode(mesh, parent, mgr, owner->getID(), position, rotation, scale)
{
	m_owner = owner;
}

CGameAnimatedMeshSceneNode::~CGameAnimatedMeshSceneNode()
{

}

// render
// override render state
void CGameAnimatedMeshSceneNode::render()
{
	CGameObject::ObjectState state = m_owner->getObjectState();
	
	// draw bbox on select
	if ( 
			state == CGameObject::Select ||
			state == CGameObject::Move ||
			state == CGameObject::Rotation ||
			state == CGameObject::Review		
		)
		setDebugDataVisible( EDS_BBOX );

	// draw animesh
	CAnimatedMeshSceneNode::render();
	
	// draw move
	if ( state == CGameObject::Move )
		m_owner->drawFrontUpLeftVector();	
	else if ( state == CGameObject::Rotation )
		m_owner->drawCircleAroundObject();
	
}