#include "stdafx.h"
#include "CGameOxyzSceneNode.h"

CGameOxyzSceneNode::CGameOxyzSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
	:ISceneNode( parent, mgr, id )
{

}

CGameOxyzSceneNode::~CGameOxyzSceneNode()
{
}

void CGameOxyzSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void CGameOxyzSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	Material.Lighting = false;
	Material.AntiAliasing=0;

	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	SColor color(255, 100,100,100);

	core::vector3df start;
	core::vector3df end;

	int		nGrid = 50;
	float	gridSize = 100.0f;
	float	size = (float)nGrid * gridSize * 0.5f;

	start	= core::vector3df( -size, 0, -size );
	end		= core::vector3df(  size, 0, -size );

	for ( int i = 0; i <= nGrid; i++ )
	{
		driver->draw3DLine( start, end, color );
		start.Z += gridSize;
		end.Z	+= gridSize;
	}

	start	= core::vector3df( -size,0,-size );
	end		= core::vector3df( -size,0, size );

	for ( int i = 0; i <= nGrid; i++ )
	{
		driver->draw3DLine( start, end, color );
		start.X += gridSize;
		end.X	+= gridSize;
	}
		
	Box.addInternalPoint( core::vector3df(-size*2, 0, -size*2) );
	Box.addInternalPoint( core::vector3df( size*2, 10, size*2) );	
}