#include "stdafx.h"
#include "CGameParticleContainerSceneNode.h"
#include "IView.h"

CGameParticleContainerSceneNode::CGameParticleContainerSceneNode(
			CGameObject *owner,
			ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position,
			const core::vector3df& rotation,
			const core::vector3df& scale
			)
	:ISceneNode( parent, mgr, id, position, rotation, scale )
{
	m_owner = owner;

	// turn on culling when draw
	AutomaticCullingState = EAC_BOX;
}

CGameParticleContainerSceneNode::~CGameParticleContainerSceneNode()
{
}

const core::aabbox3d<f32>& CGameParticleContainerSceneNode::getBoundingBox() const
{
	return Box;
}

//! This method is called just before the rendering process of the whole scene.
void CGameParticleContainerSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);

		// only register child if container is visible
		ISceneNode::OnRegisterSceneNode();

		// need update bouding box
		ISceneNodeList::Iterator it = Children.begin(), end = Children.end();
		
		// box is equal first child
		Box.reset(core::vector3df(0,0,0));
		if ( it != end )
		{
			Box = (*it)->getBoundingBox();
			it++;
		}

		// update another child
		for (; it != end; ++it)
		{			
			Box.addInternalBox( (*it)->getBoundingBox() );
		}
	}
}

//! does nothing.
void CGameParticleContainerSceneNode::render()
{
	IVideoDriver *driver = getIView()->getDriver();

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
	
	if ( DebugDataVisible & scene::EDS_BBOX )
	{		
		driver->setTransform(video::ETS_WORLD, getAbsoluteTransformation() );
		video::SMaterial deb_m;
		deb_m.Lighting = false;
		driver->setMaterial(deb_m);
		
		driver->draw3DBox( Box, video::SColor(255,255,255,255));
	}

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