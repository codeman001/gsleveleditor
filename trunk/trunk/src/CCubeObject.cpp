#include "stdafx.h"
#include "CCubeObject.h"
#include "IView.h"

CCubeObject::CCubeObject( float size )
{
	m_node = getIView()->getSceneMgr()->addCubeSceneNode( size );
	m_node->setMaterialFlag( video::EMF_LIGHTING, false );

	updateNodeRotation();
	updateNodePosition();
}

CCubeObject::~CCubeObject()
{
}

void CCubeObject::updateObject()
{	
	// get line ray	
	scene::ISceneCollisionManager* collMan = getIView()->getSceneMgr()->getSceneCollisionManager();
	
	core::vector2di point( getIView()->m_mouseX, getIView()->m_mouseY );
	core::line3d<f32> ray = collMan->getRayFromScreenCoordinates( point, getIView()->getSceneMgr()->getActiveCamera() );
		
	// plane oxz
	core::plane3df plane( 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );

	core::vector3df hit;
	bool b = plane.getIntersectionWithLine( ray.start, ray.getVector(), hit );
	
	if ( b )
	{
		//setOrientation( hit );
		lookAt( hit );
	}

}