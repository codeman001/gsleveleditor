#include "stdafx.h"
#include "CGameBoxSceneNode.h"
#include "CGameObject.h"

CGameBoxSceneNode::CGameBoxSceneNode
	(
		CGameObject *owner,
		f32 size, ISceneNode* parent, ISceneManager* mgr, s32 id,
		const core::vector3df& position,
		const core::vector3df& rotation,
		const core::vector3df& scale
	)
	:CCubeSceneNode( size, parent, mgr, id, position, rotation, scale )
{
	m_owner = owner;
}

CGameBoxSceneNode::~CGameBoxSceneNode()
{
}

// render
// override render state
void CGameBoxSceneNode::render()
{
#ifdef GSEDITOR
	CGameObject::EObjectState state = m_owner->getObjectState();
	
	// draw bbox on select
	if ( 
			state == CGameObject::Move ||
			state == CGameObject::Review		
		)
		setDebugDataVisible( EDS_BBOX );
	else
		setDebugDataVisible( 0 );

	// call object draw
	m_owner->drawObject();

#endif

	CCubeSceneNode::render();
	
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