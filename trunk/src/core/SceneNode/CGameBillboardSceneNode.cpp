#include "stdafx.h"
#include "CGameBillboardSceneNode.h"
#include "CGameObject.h"

CGameBillboardSceneNode::CGameBillboardSceneNode( CGameObject *owner,
		ISceneNode* parent, ISceneManager* mgr, s32 id,	
		const core::vector3df& position, 
		const core::dimension2d<f32>& size,
		video::SColor colorTop, video::SColor colorBottom)
	:CBillboardSceneNode( parent, mgr, id, position, size, colorTop, colorBottom ),
	IBillboardSceneNode( parent, mgr, id, position )
{
	m_owner = owner;
}

CGameBillboardSceneNode::~CGameBillboardSceneNode()
{

}

// render
// override render state
void CGameBillboardSceneNode::render()
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

	CBillboardSceneNode::render();
	
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