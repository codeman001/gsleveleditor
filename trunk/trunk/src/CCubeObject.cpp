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

}