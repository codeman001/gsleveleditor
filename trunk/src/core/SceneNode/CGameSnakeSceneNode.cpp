#include "stdafx.h"
#include "CGameSnakeSceneNode.h"

#include "IView.h"

CGameSnakeSceneNode::CGameSnakeSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
	:ISceneNode( parent, mgr, id )
{
	m_snakeTexture = getIView()->getDriver()->getTexture("data/mesh/snakeTexture.jpg");
}

CGameSnakeSceneNode::~CGameSnakeSceneNode()
{

}

void CGameSnakeSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void CGameSnakeSceneNode::OnAnimate(irr::u32 timeMs)
{
	core::list<ISceneNodeAnimator*>::ConstIterator i = getAnimators().begin(), end = getAnimators().end();
	while ( i != end )
	{
		(*i)->animateNode( this, timeMs );
		i++;
	}
}

void CGameSnakeSceneNode::updateMesh( list<core::vector3df>*	listBone )
{
	m_snakeMesh.setSnakeBone(listBone);
	m_snakeMesh.skinSnakeMesh();
}


void CGameSnakeSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (!driver)
		return;


	bool isTransparentPass = SceneManager->getSceneNodeRenderPass() == scene::ESNRP_TRANSPARENT;

	scene::IMesh* m = &m_snakeMesh;
	
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	video::SMaterial& mat = getMaterial(0);
	mat.TextureLayer[0].Texture = m_snakeTexture;

	video::IMaterialRenderer* rnd = driver->getMaterialRenderer( mat.MaterialType );
	bool transparent = (rnd && rnd->isTransparent());

	
	if (transparent == isTransparentPass)
	{
		scene::IMeshBuffer* mb = m->getMeshBuffer(0);
		const video::SMaterial& material = mb->getMaterial();
		
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

		driver->setMaterial(material);
		driver->drawMeshBuffer(mb);
	}
	
}