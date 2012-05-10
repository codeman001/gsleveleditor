#include "stdafx.h"
#include "CGameContainerSceneNode.h"
#include "IView.h"

//////////////////////////////////////////////////////////
// CGameContainerSceneNode implement
//////////////////////////////////////////////////////////

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

	// turn on culling when draw
	AutomaticCullingState = EAC_BOX;
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

		// need update bouding box
		ISceneNodeList::Iterator it = Children.begin(), end = Children.end();
		
		// box is equal first child
		Box.reset(core::vector3df(0,0,0));
		if ( it != end )
		{
			Box = (*it)->getTransformedBoundingBox();
			it++;
		}

		// update another child
		for (; it != end; ++it)
		{			
			Box.addInternalBox( (*it)->getTransformedBoundingBox() );
		}
	}
}

//! does nothing.
void CGameContainerSceneNode::render()
{
	IVideoDriver *driver = getIView()->getDriver();

	if ( DebugDataVisible & scene::EDS_BBOX )
	{
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		video::SMaterial deb_m;
		deb_m.Lighting = false;
		driver->setMaterial(deb_m);

		core::aabbox3d<f32> tbox = Box;
		getAbsoluteTransformation().transformBoxEx(tbox);

		driver->draw3DBox( tbox, video::SColor(255,255,0,255));
	}
}


//////////////////////////////////////////////////////////
// CGameChildContainerSceneNode implement
//////////////////////////////////////////////////////////


CGameChildContainerSceneNode::CGameChildContainerSceneNode(
			CGameObject *owner,
			ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position,
			const core::vector3df& rotation,
			const core::vector3df& scale
			)
	:CGameContainerSceneNode(owner, parent, mgr, id, position, rotation,scale)
{
}


CGameChildContainerSceneNode::~CGameChildContainerSceneNode()
{
	vector<ISceneNode*>::iterator i = m_boundingBoxOfChild.begin(), end = m_boundingBoxOfChild.end();
	while( i != end )
	{
		(*i)->drop();
		i++;
	}
	m_boundingBoxOfChild.clear();
}

//! This method is called just before the rendering process of the whole scene.
void CGameChildContainerSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);

		// only register child if container is visible
		ISceneNode::OnRegisterSceneNode();

		// need update bouding box
		vector<ISceneNode*>::iterator it = m_boundingBoxOfChild.begin(), end = m_boundingBoxOfChild.end();
		
		// box is equal first child
		Box.reset(core::vector3df(0,0,0));

		// update another child
		for (; it != end; ++it)
		{			
			Box.addInternalBox( (*it)->getBoundingBox() );
		}
	}
}

void CGameChildContainerSceneNode::render()
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

	// call object draw
	m_owner->drawObject();	

	// draw bouding box
	if ( DebugDataVisible & scene::EDS_BBOX )
	{
		IVideoDriver *driver = getIView()->getDriver();

		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		video::SMaterial deb_m;
		deb_m.Lighting = false;
		driver->setMaterial(deb_m);

		core::aabbox3d<f32> tbox = Box;
		getAbsoluteTransformation().transformBoxEx(tbox);

		driver->draw3DBox( tbox, video::SColor(255,255,255,255));
	}

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