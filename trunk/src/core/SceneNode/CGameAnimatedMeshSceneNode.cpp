#include "stdafx.h"
#include "CGameAnimatedMeshSceneNode.h"
#include "CGameObject.h"

#ifdef GSANIMATION
#include "IView.h"
#endif

CGameAnimatedMeshSceneNode::CGameAnimatedMeshSceneNode
	(
		CGameObject *owner,
		IAnimatedMesh* mesh, ISceneNode* parent, ISceneManager* mgr,
		const core::vector3df& position,
		const core::vector3df& rotation,
		const core::vector3df& scale
	):CAnimatedMeshSceneNode(mesh, parent, mgr, (s32)owner->getID(), position, rotation, scale)
{
	m_owner = owner;
}

CGameAnimatedMeshSceneNode::~CGameAnimatedMeshSceneNode()
{

}

// render
// override render state
void CGameAnimatedMeshSceneNode::render()
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
#endif

	// draw animesh
	CAnimatedMeshSceneNode::render();

#ifdef GSANIMATION		
	// get driver
	IVideoDriver* driver = getSceneManager()->getVideoDriver();

	ISkinnedMesh *mesh = (ISkinnedMesh*)getMesh();
	IView *pView = getIView();

	irr::gui::IGUIFont* font = getSceneManager()->getGUIEnvironment()->getBuiltInFont();

	video::SMaterial debug_mat;
	debug_mat.Lighting = false;
	debug_mat.AntiAliasing = 0;
	debug_mat.ZBuffer = video::ECFN_NEVER;
		
	for (u32 g=0; g < mesh->getAllJoints().size(); ++g)
	{
		ISkinnedMesh::SJoint *joint = mesh->getAllJoints()[g];
		core::vector3df v;
		
		//basic bone
		//core::matrix4 mat1 = joint->GlobalInversedMatrix;
		//mat1.makeInverse();

		//anim bone
		core::matrix4 mat1 = joint->GlobalAnimatedMatrix;

		// get position
		mat1.transformVect( v );
		
		// scale with character
		v *= m_owner->getScale();

		// draw name bone on screen
		int x, y;
		pView->getScreenCoordinatesFrom3DPosition( v, &x, &y );
		wchar_t text[1024];
		uiString::copy<wchar_t, const c8>( text, joint->Name.c_str() );		
		
		// draw bone position
		SColor c = SColor(255,0,0,255);	
		driver->setMaterial(debug_mat);
		driver->draw2DRectangle( c, core::rect<s32>( x - 2, y - 2, x + 2, y + 2 ) ); 

		// draw text
		font->draw( text, core::rect<s32>( x + 10, y, x + 100, y + 50), SColor(255, 255,255,0) );
	}
#endif

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