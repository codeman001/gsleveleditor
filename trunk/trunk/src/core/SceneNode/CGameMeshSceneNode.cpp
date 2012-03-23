#include "stdafx.h"
#include "CGameMeshSceneNode.h"
#include "CGameObject.h"

CGameMeshSceneNode::CGameMeshSceneNode(
				CGameObject *owner,
				IMesh* mesh, ISceneNode* parent, ISceneManager* mgr,
				const core::vector3df& position,
				const core::vector3df& rotation,
				const core::vector3df& scale
			)
	:CMeshSceneNode(mesh, parent, mgr, owner->getID(), position, rotation, scale)
{
	m_owner = owner;
}

CGameMeshSceneNode::~CGameMeshSceneNode()
{

}

// render
// override render state
void CGameMeshSceneNode::render()
{
#ifdef GSEDITOR
	CGameObject::ObjectState state = m_owner->getObjectState();
	
	// draw bbox on select
	if ( 
			state == CGameObject::Move ||
			state == CGameObject::Review		
		)
		setDebugDataVisible( EDS_BBOX );
	else
		setDebugDataVisible( 0 );
#endif

	// draw mesh
	CMeshSceneNode::render();

#ifdef GSEDITOR	
	// draw move
	if ( 
			state == CGameObject::Move || 
			state == CGameObject::Rotation ||
			state == CGameObject::Scale
		)
		m_owner->drawFrontUpLeftVector();	
	
	if ( state == CGameObject::Rotation )
		m_owner->drawCircleAroundObject();	
#endif

}