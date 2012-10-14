#include "stdafx.h"
#include "CGameDebugSceneNode.h"
#include "gameDebug/CGameDebug.h"

CGameDebugSceneNode::CGameDebugSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
	:ISceneNode( parent, mgr, id )
{
}

CGameDebugSceneNode::~CGameDebugSceneNode()
{
}

void CGameDebugSceneNode::OnRegisterSceneNode()
{
	// update Box;
	Box.reset(0,0,0);

	CGameDebug *debug = CGameDebug::getInstance();
	for ( int i = 0; i < debug->getLinesCount(); i++ )
	{
		Box.addInternalPoint( debug->getLine(i).line.start );
		Box.addInternalPoint( debug->getLine(i).line.end );
	}
	for ( int i = 0; i < debug->getBoxCount(); i++ )
	{		
		Box.addInternalBox( debug->getBox(i).box );
	}


	// make sure allway render debug node
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();	
}

void CGameDebugSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	Material.Lighting = false;
	Material.AntiAliasing=0;

	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

	CGameDebug *debug = CGameDebug::getInstance();
	for ( int i = 0; i < debug->getLinesCount(); i++ )
	{
		const CGameDebug::SLineDebug& line = debug->getLine(i);
		driver->draw3DLine( line.line.start, line.line.end, line.color );		
	}	

	for ( int i = 0; i < debug->getBoxCount(); i++ )
	{		
		driver->draw3DBox( debug->getBox(i).box, debug->getBox(i).color );
	}
}