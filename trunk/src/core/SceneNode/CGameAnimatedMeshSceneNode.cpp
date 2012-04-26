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
	):CAnimatedMeshSceneNode(mesh, parent, mgr, owner->getID(), position, rotation, scale)
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

	setDebugDataVisible( EDS_SKELETON );
#endif


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
	driver->setMaterial(debug_mat);
	
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
		font->draw( text, core::rect<s32>( x,y, x + 100, y + 50), SColor(255, 255,255,0) );
	}
#endif

	// draw animesh
	CAnimatedMeshSceneNode::render();

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