#include "stdafx.h"
#include "CGameGrassSceneNode.h"
#include "CGameObject.h"
#include "IView.h"

CGameGrassSceneNode::CGameGrassSceneNode(CGameObject *owner, IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, const core::vector3df& position, s32 id)
	:ISceneNode( parent, mgr, id, position, core::vector3df(0,0,0) )
{
	m_owner = owner;
	m_mesh	= mesh;
	
	if ( m_mesh == NULL )
		return;

	// grab mesh
	m_mesh->grab();
	
	static s32 s_materialGrass = -1;
	if ( s_materialGrass == -1 )
	{
		CShaderGrassCallBack* grassShaderCB = new CShaderGrassCallBack();

		s_materialGrass = mgr->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles
		(
			getIView()->getPath("shader/extern/grass.vert"), "vertexMain",	video::EVST_VS_1_1,
			getIView()->getPath("shader/extern/grass.frag"), "pixelMain",	video::EPST_PS_1_1,
			grassShaderCB, 
			video::EMT_TRANSPARENT_ALPHA_CHANNEL
		);

		grassShaderCB->drop();
	}
	
	m_Material = m_mesh->getMeshBuffer(0)->getMaterial();
	m_Material.MaterialType = (E_MATERIAL_TYPE)s_materialGrass;
    m_Material.MaterialTypeParam = 0.3f;

}

CGameGrassSceneNode::~CGameGrassSceneNode()
{
	m_mesh->drop();
}

// OnRegisterSceneNode
// irr scenenode impl
void CGameGrassSceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

// render
// irr scenenode impl
void CGameGrassSceneNode::render()
{
	ISceneManager	*smgr = getSceneManager();
	IVideoDriver	*driver = smgr->getVideoDriver();
	

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

	// set world transform
	driver->setTransform( video::ETS_WORLD, getAbsoluteTransformation());

	// set current material with config shader
    driver->setMaterial( m_Material);

	// draw mesh with grass shader
	int meshCount = m_mesh->getMeshBufferCount();
	for ( int i = 0; i < meshCount; i++ )
		driver->drawMeshBuffer(m_mesh->getMeshBuffer(i));

	// draw bouding box
	if ( DebugDataVisible & scene::EDS_BBOX )
	{
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		video::SMaterial deb_m;
		deb_m.Lighting = false;
		driver->setMaterial(deb_m);

		core::aabbox3d<f32> tbox = m_mesh->getBoundingBox();
		getAbsoluteTransformation().transformBoxEx(tbox);

		driver->draw3DBox( tbox, video::SColor(255,255,255,255));
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

// getBoundingBox
// irr scenenode impl
const core::aabbox3d<f32>& CGameGrassSceneNode::getBoundingBox() const
{
	return m_mesh->getBoundingBox();
}

// getMaterialCount
// irr scenenode impl
u32 CGameGrassSceneNode::getMaterialCount() const
{
	return 1;
}

// getMaterial
// irr scenenode impl
video::SMaterial& CGameGrassSceneNode::getMaterial(u32 i)
{
	return m_Material;
}