#include "stdafx.h"
#include "CGameContainerSceneNode.h"
#include "CGameColladaSceneNode.h"
#include "IView.h"

#pragma region GAME_CONTAINER

//////////////////////////////////////////////////////////
// CGameContainerSceneNode implement
// Manager all game scenenode
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
	if (IsVisible && m_owner)
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
	if ( m_owner == NULL )
		return;

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

#pragma endregion


#pragma region GAME_COLLADA_CONTAINER

//////////////////////////////////////////////////////////
// CGameColladaContainerSceneNode implement
// Manager collada node
//////////////////////////////////////////////////////////


CGameColladaContainerSceneNode::CGameColladaContainerSceneNode(
			CGameObject *owner,
			ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position,
			const core::vector3df& rotation,
			const core::vector3df& scale
			)
	:CGameContainerSceneNode(owner, parent, mgr, id, position, rotation,scale)
{
	m_containSkinMesh = false;
}


CGameColladaContainerSceneNode::~CGameColladaContainerSceneNode()
{
	std::vector<ISceneNode*>::iterator i = m_boudingMeshNode.begin(), end = m_boudingMeshNode.end();
	while( i != end )
	{
		(*i)->drop();
		i++;
	}
	m_boudingMeshNode.clear();
}

//! This method is called just before the rendering process of the whole scene.
void CGameColladaContainerSceneNode::OnRegisterSceneNode()
{
	if (IsVisible && m_owner)
	{
		SceneManager->registerNodeForRendering(this);

		// only register child if container is visible
		ISceneNode::OnRegisterSceneNode();

		// need update bouding box
		if ( m_containSkinMesh == false )
		{
			// update bbox from MESH
			std::vector<ISceneNode*>::iterator it = m_boudingMeshNode.begin(), end = m_boudingMeshNode.end();
			
			// box is equal first child
			Box.reset(core::vector3df(0,0,0));

			bool first = true;

			// update another child
			for (; it != end; ++it)
			{	
				CGameColladaSceneNode *node = (CGameColladaSceneNode*)(*it);			
				if ( node->ColladaMesh )
				{
					if ( first == true )
					{
						Box = computeChildBoudingBox( node );
						first = false;
					}
					else
						Box.addInternalBox( computeChildBoudingBox( node ) );
				}
			}
		}
		else
		{
			// update bbox from BONE

			std::queue<ISceneNode*>	queue;
			ISceneNodeList::ConstIterator it = Children.begin(), end = Children.end();
			while ( it != end )
			{
				queue.push( (*it) );
				it++;
			}
			
			bool first = true;
			Box.reset(0,0,0);

			while ( queue.size() )
			{
				CGameColladaSceneNode* top = (CGameColladaSceneNode*)queue.front();
				queue.pop();

				// only get bounding box of bone scene node
				if ( top->getMesh() == NULL && top->isHaveBoneBBox() )
				{
					// call child OnAnimate					
					core::aabbox3df bbBox = top->getBoundingBox();
					top->LocalAbsoluteMatrix.transformBox(bbBox);

					if ( first )
					{
						Box = bbBox;
						first = false;
					}
					else
					{
						Box.addInternalBox( bbBox );
					}					
				}

				it	= top->getChildren().begin();
				end = top->getChildren().end();
				while ( it != end )
				{
					queue.push( (*it) );
					it++;
				}

			}
		}
	}
}

//! Overide OnAnimate
void CGameColladaContainerSceneNode::OnAnimate(irr::u32 timeMs)
{	
	if ( IsVisible == false )
		return;

	// animate this node with all animators
	ISceneNodeAnimatorList::Iterator ait = Animators.begin(), aend = Animators.end();
	while (ait != aend)
	{		
		(*ait)->animateNode(this, timeMs);
		++ait;
	}

	// update absolute position
	updateAbsolutePosition();

	// call child OnAnimate and destroy recursive
	std::queue<ISceneNode*>	queue;
	ISceneNodeList::ConstIterator it = Children.begin(), end = Children.end();
	while ( it != end )
	{
		queue.push( (*it) );
		it++;
	}		

	while ( queue.size() )
	{
		ISceneNode* top = queue.front();
		queue.pop();

		// call child OnAnimate
		top->OnAnimate( timeMs );			

		it	= top->getChildren().begin();
		end = top->getChildren().end();
		while ( it != end )
		{
			queue.push( (*it) );
			it++;
		}
	}    
    
    // update node reference
    for ( int i = 0, n = (int)m_nodeReference.size(); i < n; i++)
    {
        CGameColladaSceneNode *colladaNode1  = (CGameColladaSceneNode*) m_nodeReference[i].node1;
        CGameColladaSceneNode *colladaNode2  = (CGameColladaSceneNode*) m_nodeReference[i].node2;
     
        // check anim layer
        int currentLayer = colladaNode2->getCurrentAnimLayer();
        if ( colladaNode1->getCurrentAnimLayer() != currentLayer )
            continue;
        
        // calc reference matrix
        core::matrix4 matParentInv	= colladaNode2->BaseAbsoluteAnimationMatrixLayer[currentLayer];
        core::matrix4 matNode		= colladaNode1->BaseAbsoluteAnimationMatrixLayer[currentLayer];
        
        matParentInv.makeInverse();
        core::matrix4 relativeMtx = matParentInv*matNode;

        // recalc animation matrix
        colladaNode1->AbsoluteAnimationMatrix = colladaNode2->AbsoluteAnimationMatrix*relativeMtx;
		
		// calc absolute transform
		core::matrix4 absoluteTransform = colladaNode2->getAbsoluteTransformation()*relativeMtx;
		colladaNode1->setAbsoluteTransform(absoluteTransform);
		colladaNode1->updateChildAbsoluteTransform();
    }
    
    
    // update skin
	std::vector<ISceneNode*>::iterator itSkin = m_boudingMeshNode.begin(), endSkin = m_boudingMeshNode.end();
	while ( itSkin != endSkin )
	{
		((CGameColladaSceneNode*)(*itSkin))->skin();
		itSkin++;
	}

}

// compute bb
core::aabbox3df	CGameColladaContainerSceneNode::computeChildBoudingBox( ISceneNode *node )
{
	core::aabbox3d<f32> box = node->getBoundingBox();
	core::matrix4	mat;
		
	while ( node != this && node != NULL )
	{
		mat = ((CGameColladaSceneNode*)node)->RelativeMatrix * mat;
		node = node->getParent();
	}

	mat.transformBoxEx(box);
	return box;
}

void CGameColladaContainerSceneNode::render()
{
	if (m_owner == NULL)
		return;

	IVideoDriver *driver = getIView()->getDriver();

	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	video::SMaterial deb_m;
	deb_m.Lighting = false;
	driver->setMaterial(deb_m);

	core::aabbox3d<f32> tbox = Box;
	getAbsoluteTransformation().transformBoxEx(tbox);

	driver->draw3DBox( tbox, video::SColor(255,0,255,0));


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

#pragma endregion